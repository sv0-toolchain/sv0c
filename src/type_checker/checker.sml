(* Type checking for a Milestone-1 slice: literals, paths, let with PatBind,
   return / block tail, and unit functions with empty body. E04xx = types. *)

structure Checker :> CHECKER = struct

  type venv = (string * Types.ty) list

  val loopDepth = ref 0

  fun withLoop (f : unit -> 'a) : 'a =
    let val () = loopDepth := !loopDepth + 1
        val r = f () handle ex => (loopDepth := !loopDepth - 1; raise ex)
    in
      loopDepth := !loopDepth - 1;
      r
    end

  fun extend (env : venv) (x : string) (t : Types.ty) : venv = (x, t) :: env

  fun lookup [] x =
        raise Fail ("E0401: unbound value in type checker: `" ^ x ^ "`")
    | lookup ((n, t) :: r) x =
        if String.compare (n, x) = EQUAL then t else lookup r x

  datatype variantShape = VSUnit | VSTuple of Types.ty list

  val structDefs : (string * (string * Types.ty) list) list ref = ref []
  val enumDefs : (string * (string * variantShape) list) list ref = ref []

  fun structNames () : string list = map #1 (!structDefs)
  fun enumNames () : string list = map #1 (!enumDefs)

  fun fieldsOfStruct (sn : string) : (string * Types.ty) list =
    case List.find (fn (n, _) => n = sn) (!structDefs) of
      SOME (_, fs) => fs
    | NONE => raise Fail ("E0420: unknown struct type `" ^ sn ^ "`")

  fun variantShapeOf (en : string) (vn : string) : variantShape =
    case List.find (fn (n, _) => n = en) (!enumDefs) of
      SOME (_, vs) =>
        (case List.find (fn (v, _) => v = vn) vs of
           SOME (_, sh) => sh
         | NONE =>
             raise Fail ("E0421: unknown variant `" ^ en ^ "::" ^ vn ^ "`"))
    | NONE => raise Fail ("E0422: unknown enum type `" ^ en ^ "`")

  fun ctorTy (en : string) (sh : variantShape) : Types.ty =
    case sh of
      VSUnit => Types.TyFn ([], Types.TyEnum en)
    | VSTuple ts => Types.TyFn (ts, Types.TyEnum en)

  fun pathKey (p : Ast.path) : string = String.concatWith "::" p

  fun astTyToTy (t : Ast.ty) : Types.ty option =
    case t of
      Ast.TyName ([name], _) =>
        (case name of
           "unit" => SOME Types.TyUnit
         | "bool" => SOME Types.TyBool
         | "char" => SOME Types.TyChar
         | "i32" => SOME (Types.TyInt 32)
         | "u32" => SOME (Types.TyUint 32)
         | "i64" => SOME (Types.TyInt 64)
         | "u64" => SOME (Types.TyUint 64)
         | "i128" => SOME (Types.TyInt 128)
         | "u128" => SOME (Types.TyUint 128)
         | "i8" => SOME (Types.TyInt 8)
         | "u8" => SOME (Types.TyUint 8)
         | "i16" => SOME (Types.TyInt 16)
         | "u16" => SOME (Types.TyUint 16)
         | "isize" => SOME Types.TyIsize
         | "usize" => SOME Types.TyUsize
         | "f32" => SOME (Types.TyFloat 32)
         | "f64" => SOME (Types.TyFloat 64)
         | "str" => SOME Types.TyString
         | "string" => SOME Types.TyString
         | "String" => SOME Types.TyString
         | _ =>
             if List.exists (fn s => s = name) (structNames ()) then
               SOME (Types.TyStruct name)
             else if List.exists (fn e => e = name) (enumNames ()) then
               SOME (Types.TyEnum name)
             else
               NONE)
    | Ast.TyUnit _ => SOME Types.TyUnit
    | Ast.TyRef (t2, _) =>
        (case astTyToTy t2 of
           SOME u => SOME (Types.TyRef u)
         | NONE => NONE)
    | Ast.TyRefMut (t2, _) =>
        (case astTyToTy t2 of
           SOME u => SOME (Types.TyRefMut u)
         | NONE => NONE)
    | Ast.TyTuple (ts, _) =>
        let
          fun optMap f [] = SOME []
            | optMap f (x :: xs) =
                case f x of
                  NONE => NONE
                | SOME y =>
                    (case optMap f xs of
                       NONE => NONE
                     | SOME ys => SOME (y :: ys))
        in
          case optMap astTyToTy ts of
            SOME us => SOME (Types.TyTuple us)
          | NONE => NONE
        end
    | _ => NONE

  fun expect (got : Types.ty, want : Types.ty) : unit =
    if Unify.unify (got, want) then ()
    else raise Fail "E0400: type mismatch"

  fun isIntegralTy (t : Types.ty) : bool =
    case t of
      Types.TyInt _ => true
    | Types.TyUint _ => true
    | Types.TyIsize => true
    | Types.TyUsize => true
    | _ => false

  (* `?` on TyEnum en: must match fn return type; Ok/Err each carry one field. *)
  fun okErrPayloadTys (en : string) : Types.ty * Types.ty =
    case (variantShapeOf en "Ok", variantShapeOf en "Err") of
      (VSTuple [tOk], VSTuple [tErr]) => (tOk, tErr)
    | _ =>
        raise Fail
          "E0441: `?` requires variants Ok(T) and Err(E) with exactly one field each"

  fun initTypes (prog : Ast.program) : unit =
    let
      val structs = List.mapPartial (fn Ast.ItemStruct r => SOME r | _ => NONE) prog
      val enums = List.mapPartial (fn Ast.ItemEnum r => SOME r | _ => NONE) prog
      val sn = map #name structs
      val en = map #name enums
      val () =
        app (fn n =>
               if List.exists (fn m => m = n) en then
                 raise Fail ("E0417: name `" ^ n ^ "` is both struct and enum")
               else ()) sn
      fun namedOnlyTy name =
        if List.exists (fn x => x = name) sn then SOME (Types.TyStruct name)
        else if List.exists (fn x => x = name) en then SOME (Types.TyEnum name)
        else NONE
      fun resolveFieldTy (t : Ast.ty) : Types.ty =
        case t of
          Ast.TyName ([name], _) =>
            (case name of
               "unit" => Types.TyUnit
             | "bool" => Types.TyBool
             | "char" => Types.TyChar
             | "i32" => Types.TyInt 32
             | "i8" => Types.TyInt 8
             | "i16" => Types.TyInt 16
             | "i64" => Types.TyInt 64
             | "u32" => Types.TyUint 32
             | "u8" => Types.TyUint 8
             | "u16" => Types.TyUint 16
             | "u64" => Types.TyUint 64
             | _ =>
                 (case namedOnlyTy name of
                    SOME u => u
                  | NONE => raise Fail "E0406: unknown type in struct/enum field"))
        | Ast.TyUnit _ => Types.TyUnit
        | Ast.TyTuple (ts, _) =>
            Types.TyTuple (map resolveFieldTy ts)
        | Ast.TyRef _ =>
            raise Fail "E0424: ref types in struct fields not supported in this slice"
        | Ast.TyRefMut _ =>
            raise Fail "E0424: ref types in struct fields not supported in this slice"
        | _ =>
            raise Fail "E0424: type form not supported in struct/enum field"
      fun structOne (r : {
            name : Ast.ident,
            fields : (Ast.ident * Ast.ty) list,
            span : Span.span
          }) =
        (#name r, map (fn (f, aty) => (f, resolveFieldTy aty)) (#fields r))
      fun variantName v =
        case v of
          Ast.VariantUnit (n, _) => n
        | Ast.VariantTuple (n, _, _) => n
        | Ast.VariantStruct (n, _, _) => n
      fun variantShape v =
        case v of
          Ast.VariantUnit _ => VSUnit
        | Ast.VariantTuple (_, ts, _) => VSTuple (map resolveFieldTy ts)
        | Ast.VariantStruct (_, fs, _) =>
            VSTuple (map (resolveFieldTy o #2) fs)
      fun dupVariants names =
        let
          fun go [] _ = ()
            | go (x :: xs) seen =
                if List.exists (fn y => String.compare (y, x) = EQUAL) seen then
                  raise Fail "E0423: duplicate variant in enum"
                else
                  go xs (x :: seen)
        in
          go names []
        end
      fun enumOne (r : {
            name : Ast.ident,
            variants : Ast.variant list,
            span : Span.span
          }) =
        let val vnames = map variantName (#variants r)
        in
          dupVariants vnames;
          (#name r, map (fn v => (variantName v, variantShape v)) (#variants r))
        end
      val sd = map structOne structs
      val ed = map enumOne enums
    in
      structDefs := sd;
      enumDefs := ed
    end

  datatype binopKind = Arith | Logic | Cmp

  fun binopClass (b : Ast.binop) : binopKind =
    case b of
      (Ast.Add | Ast.Sub | Ast.Mul | Ast.Div | Ast.Rem | Ast.BitAnd | Ast.BitOr
      | Ast.BitXor | Ast.Shl | Ast.Shr) => Arith
    | (Ast.And | Ast.Or) => Logic
    | (Ast.Eq | Ast.Neq | Ast.Lt | Ast.Gt | Ast.Leq | Ast.Geq) => Cmp

  fun stmtReturns (st : Ast.stmt) : bool =
    case st of
      Ast.StmtSemi (Ast.ExprReturn _, _) => true
    | Ast.StmtExpr (Ast.ExprReturn _, _) => true
    | _ => false

  fun lastStmtReturns (stmts : Ast.stmt list) : bool =
    case List.rev stmts of
      st :: _ => stmtReturns st
    | [] => false

  fun bindPat (scrutTy : Types.ty) (pat : Ast.pat) (env : venv) : venv =
    case pat of
      Ast.PatWild _ => env
    | Ast.PatBind (x, _) =>
        if
          Unify.unify (scrutTy, Types.TyInt 32) orelse Unify.unify (scrutTy, Types.TyBool)
          orelse (case scrutTy of Types.TyEnum _ => true | _ => false)
        then
          extend env x scrutTy
        else
          raise Fail "E0425: pattern binding does not match scrutinee type"
    | Ast.PatLit (Ast.IntLit _, _) =>
        (expect (scrutTy, Types.TyInt 32); env)
    | Ast.PatLit (Ast.BoolLit _, _) =>
        (expect (scrutTy, Types.TyBool); env)
    | Ast.PatEnum (path, inners, _) =>
        (case path of
           [en, vn] =>
             let val () = expect (scrutTy, Types.TyEnum en)
             in
               case variantShapeOf en vn of
                 VSUnit =>
                   (case inners of
                      [] => env
                    | _ => raise Fail "E0426: enum pattern arity mismatch")
               | VSTuple ts =>
                   let
                     fun bindMany e [] [] = e
                       | bindMany e (p :: ps) (t :: tr) =
                           bindMany (bindPat t p e) ps tr
                       | bindMany _ _ _ =
                           raise Fail "E0426: enum pattern arity mismatch"
                   in
                     bindMany env inners ts
                   end
             end
         | _ => raise Fail "E0427: bad enum pattern path")
    | _ => raise Fail "E0428: pattern form not supported in match"

  (* retTy is the enclosing function's return type (for return/blocks in subexprs). *)
  fun synth (retTy : Types.ty) (env : venv) (e : Ast.expr) : Types.ty =
    case e of
      Ast.ExprLit (Ast.IntLit _, _) => Types.TyInt 32
    | Ast.ExprLit (Ast.BoolLit _, _) => Types.TyBool
    | Ast.ExprLit (Ast.UnitLit, _) => Types.TyUnit
    | Ast.ExprLit (Ast.StringLit _, _) => Types.TyString
    | Ast.ExprTuple ([e1], _) => synth retTy env e1
    | Ast.ExprTuple _ =>
        raise Fail "E0446: multi-element tuples are not supported in this slice"
    | Ast.ExprPath ([x], _) =>
        (case lookup env x of
           Types.TyFn _ =>
             raise Fail "E0411: function values are not supported in this slice"
         | t => t)
    | Ast.ExprPath (segs, _) =>
        let val k = pathKey segs
        in
          case lookup env k of
            Types.TyFn ([], resTy) => resTy
          | Types.TyFn _ =>
              raise Fail
                "E0419: use call syntax for enum constructor with payload"
          | _ =>
              raise Fail ("E0401: unbound value in type checker: `" ^ k ^ "`")
        end
    | Ast.ExprBlock (stmts, opt, _) =>
        let
          val envAfter =
            List.foldl (fn (st, acc) => checkStmt retTy acc st) env stmts
        in
          case opt of
            SOME e2 => synth retTy envAfter e2
          | NONE =>
              if lastStmtReturns stmts then retTy else Types.TyUnit
        end
    | Ast.ExprUnop (Ast.Neg, e1, _) =>
        (expect (synth retTy env e1, Types.TyInt 32); Types.TyInt 32)
    | Ast.ExprUnop (Ast.Not, e1, _) =>
        (expect (synth retTy env e1, Types.TyBool); Types.TyBool)
    | Ast.ExprUnop (Ast.BitNot, e1, _) =>
        (expect (synth retTy env e1, Types.TyInt 32); Types.TyInt 32)
    | Ast.ExprUnop _ =>
        raise Fail "E0402: unary operator not supported in type checker slice"
    | Ast.ExprBinop (b, l, r, _) =>
        (case binopClass b of
           Arith =>
             (expect (synth retTy env l, Types.TyInt 32);
              expect (synth retTy env r, Types.TyInt 32);
              Types.TyInt 32)
         | Logic =>
             (expect (synth retTy env l, Types.TyBool);
              expect (synth retTy env r, Types.TyBool);
              Types.TyBool)
         | Cmp =>
             let
               val t1 = synth retTy env l
               val t2 = synth retTy env r
             in
               if
                 (Unify.unify (t1, Types.TyInt 32) andalso Unify.unify (t2, Types.TyInt 32))
                 orelse (Unify.unify (t1, Types.TyBool) andalso Unify.unify (t2, Types.TyBool))
               then
                 Types.TyBool
               else
                 raise Fail "E0400: type mismatch"
             end)
    | Ast.ExprIf (c, th, NONE, _) =>
        (expect (synth retTy env c, Types.TyBool);
         expect (synth retTy env th, Types.TyUnit);
         Types.TyUnit)
    | Ast.ExprIf (c, th, SOME el, _) =>
        let
          val () = expect (synth retTy env c, Types.TyBool)
          val t1 = synth retTy env th
          val t2 = synth retTy env el
        in
          if Unify.unify (t1, t2) then t1 else raise Fail "E0400: type mismatch"
        end
    | Ast.ExprWhile (cond, invs, body, _) =>
        let
          val () = app (fn inv => expect (synth retTy env inv, Types.TyBool)) invs
          val () = expect (synth retTy env cond, Types.TyBool)
          val () = withLoop (fn () => (ignore (synth retTy env body); ()))
        in
          Types.TyUnit
        end
    | Ast.ExprFor (Ast.PatBind (x, _), Ast.ExprRange (SOME lo, SOME hi, _), body, _) =>
        let
          val () = expect (synth retTy env lo, Types.TyInt 32)
          val () = expect (synth retTy env hi, Types.TyInt 32)
          val envB = extend env x (Types.TyInt 32)
          val () = withLoop (fn () => (ignore (synth retTy envB body); ()))
        in
          Types.TyUnit
        end
    | Ast.ExprFor _ =>
        raise Fail "E0402: for-loop requires a simple binding and lo..hi range"
    | Ast.ExprLoop (body, _) =>
        (withLoop (fn () => (ignore (synth retTy env body); ())); Types.TyUnit)
    | Ast.ExprBreak (NONE, _) =>
        if !loopDepth > 0 then Types.TyUnit
        else raise Fail "E0414: break outside loop"
    | Ast.ExprBreak (SOME _, _) =>
        raise Fail "E0415: break with value not supported in this slice"
    | Ast.ExprContinue _ =>
        if !loopDepth > 0 then Types.TyUnit
        else raise Fail "E0416: continue outside loop"
    | Ast.ExprCall (Ast.ExprPath (segs, _), args, _) =>
        if case segs of [nm] => String.compare (nm, "println") = EQUAL | _ => false then
          case args of
            [Ast.ExprLit (Ast.StringLit _, _)] =>
              (ignore (synth retTy env (List.hd args)); Types.TyUnit)
          | _ => raise Fail "E0444: println expects a single string literal argument"
        else
          let val k = pathKey segs
          in
            case lookup env k of
              Types.TyFn (paramTys, outTy) =>
                let
                  fun zipCheck ([] : Ast.expr list) ([] : Types.ty list) = ()
                    | zipCheck (a :: ar) (t :: tr) =
                        (expect (synth retTy env a, t); zipCheck ar tr)
                    | zipCheck _ _ =
                        raise Fail "E0413: arity mismatch in type checker"
                in
                  zipCheck args paramTys;
                  outTy
                end
            | _ => raise Fail "E0412: call target is not a function"
          end
    | Ast.ExprStruct (path, fields, _) =>
        (case path of
           [sn] =>
             let
               val fs = fieldsOfStruct sn
               fun tyOfField f =
                 case List.find (fn (n, _) => n = f) fs of
                   SOME (_, t) => t
                 | NONE =>
                     raise Fail ("E0429: struct `" ^ sn ^ "` has no field `" ^ f ^ "`")
               fun haveField f =
                 case List.find (fn (n, _) => n = f) fields of
                   SOME _ => true
                 | NONE => false
               val () =
                 app (fn (f, _) =>
                        if haveField f then ()
                        else raise Fail ("E0430: missing field `" ^ f ^ "` in struct literal"))
                   fs
               val () =
                 app (fn (f, _) =>
                        case List.find (fn (n, _) => n = f) fs of
                          SOME _ => ()
                        | NONE =>
                            raise Fail ("E0435: unknown field `" ^ f ^ "` in struct literal"))
                   fields
               val () =
                 app (fn (f, e) =>
                        expect (synth retTy env e, tyOfField f)) fields
             in
               Types.TyStruct sn
             end
         | _ => raise Fail "E0431: struct literal needs a simple type name")
    | Ast.ExprField (e1, f, _) =>
        (case synth retTy env e1 of
           Types.TyStruct sn =>
             (case List.find (fn (n, _) => n = f) (fieldsOfStruct sn) of
                SOME (_, t) => t
              | NONE =>
                  raise Fail ("E0429: struct `" ^ sn ^ "` has no field `" ^ f ^ "`"))
         | _ => raise Fail "E0432: field access on non-struct value")
    | Ast.ExprMatch (scrut, arms, _) =>
        let
          val scrutTy = synth retTy env scrut
          val () =
            case scrutTy of
              Types.TyEnum _ => ()
            | Types.TyBool => ()
            | Types.TyInt 32 => ()
            | _ =>
                raise Fail
                  "E0433: match scrutinee must be enum, bool, or i32 in this slice"
          fun armTy (Ast.Arm (pat, guard, body)) =
            let
              val envP = bindPat scrutTy pat env
              val () =
                case guard of
                  NONE => ()
                | SOME g => expect (synth retTy envP g, Types.TyBool)
            in
              synth retTy envP body
            end
          val armTys = map armTy arms
        in
          case armTys of
            [] => raise Fail "E0434: match needs at least one arm"
          | t :: ts =>
              if List.all (fn u => Unify.unify (t, u)) ts then t
              else raise Fail "E0400: type mismatch"
        end
    | Ast.ExprCast (e1, tgtAst, _) =>
        let
          val tSrc = synth retTy env e1
          val tTgt =
            case astTyToTy tgtAst of
              SOME t => t
            | NONE => raise Fail "E0406: unknown type in as-cast target"
        in
          if isIntegralTy tSrc andalso isIntegralTy tTgt then tTgt
          else raise Fail "E0440: as-cast is only supported between integral types"
        end
    | Ast.ExprTry (e1, _) =>
        let val tScr = synth retTy env e1
        in
          case tScr of
            Types.TyEnum en =>
              let
                val () =
                  case retTy of
                    Types.TyEnum en2 =>
                      if en = en2 then ()
                      else
                        raise Fail
                          "E0442: `?` scrutinee enum must match function return type"
                  | _ =>
                      raise Fail
                        "E0443: `?` requires the enclosing function to return the same enum type"
                val (tOk, _) = okErrPayloadTys en
              in
                tOk
              end
          | _ => raise Fail "E0445: `?` operand must be an enum (Ok/Err shape)"
        end
    | _ =>
        raise Fail "E0402: expression form not supported in type checker slice"

  and checkExprStmt (env : venv) (retTy : Types.ty) (e : Ast.expr) : unit =
    case e of
      Ast.ExprReturn (NONE, _) => expect (Types.TyUnit, retTy)
    | Ast.ExprReturn (SOME e2, _) => expect (synth retTy env e2, retTy)
    | _ => ignore (synth retTy env e)

  and checkStmt (retTy : Types.ty) (env : venv) (st : Ast.stmt) : venv =
    case st of
      Ast.StmtLet (Ast.PatBind (x, _), _, to, SOME init, _) =>
        let
          val tyInit = synth retTy env init
          val tyBind =
            case to of
              NONE => tyInit
            | SOME at =>
                (case astTyToTy at of
                   SOME t => (expect (tyInit, t); t)
                 | NONE => raise Fail "E0406: unknown type in let annotation")
        in
          extend env x tyBind
        end
    | Ast.StmtLet _ =>
        raise Fail "E0407: let in this slice requires PatBind and initializer"
    | Ast.StmtSemi (e, _) => (checkExprStmt env retTy e; env)
    | Ast.StmtExpr (e, _) => (checkExprStmt env retTy e; env)
    | Ast.StmtItem _ =>
        raise Fail "E0405: nested items are not supported in type checker slice"

  fun exprReferencesResult (e : Ast.expr) : bool =
    case e of
      Ast.ExprPath (["result"], _) => true
    | Ast.ExprBinop (_, l, r, _) =>
        exprReferencesResult l orelse exprReferencesResult r
    | Ast.ExprUnop (_, e1, _) => exprReferencesResult e1
    | Ast.ExprCall (f, args, _) =>
        exprReferencesResult f orelse List.exists exprReferencesResult args
    | Ast.ExprIf (c, th, el, _) =>
        exprReferencesResult c orelse exprReferencesResult th
        orelse (case el of SOME z => exprReferencesResult z | NONE => false)
    | Ast.ExprWhile (c, invs, b, _) =>
        exprReferencesResult c
        orelse List.exists exprReferencesResult invs
        orelse exprReferencesResult b
    | Ast.ExprFor (_, it, b, _) =>
        exprReferencesResult it orelse exprReferencesResult b
    | Ast.ExprLoop (b, _) => exprReferencesResult b
    | Ast.ExprMatch (scr, arms, _) =>
        exprReferencesResult scr
        orelse List.exists
          (fn Ast.Arm (_, g, bd) =>
             (case g of SOME ge => exprReferencesResult ge | NONE => false)
             orelse exprReferencesResult bd) arms
    | Ast.ExprField (e1, _, _) => exprReferencesResult e1
    | Ast.ExprStruct (_, fs, _) =>
        List.exists (exprReferencesResult o #2) fs
    | Ast.ExprTuple (es, _) => List.exists exprReferencesResult es
    | Ast.ExprBreak (NONE, _) => false
    | Ast.ExprBreak (SOME e, _) => exprReferencesResult e
    | Ast.ExprContinue _ => false
    | Ast.ExprCast (e1, _, _) => exprReferencesResult e1
    | Ast.ExprTry (e1, _) => exprReferencesResult e1
    | _ => false

  fun checkContract (env : venv) (retTy : Types.ty) (c : Ast.contract) : unit =
    case c of
      Ast.Requires (e, _) => expect (synth Types.TyUnit env e, Types.TyBool)
    | Ast.Ensures (e, _) =>
        let
          val envR = extend env "result" retTy
        in
          if Unify.unify (retTy, Types.TyUnit) andalso exprReferencesResult e then
            raise Fail "E0520: ensures cannot use result when return type is unit"
          else
            expect (synth retTy envR e, Types.TyBool)
        end
    | Ast.LoopInvariant _ =>
        raise Fail "E0502: loop_invariant not type-checked in this slice"

  fun checkBlock (env : venv) (retTy : Types.ty)
    (stmts : Ast.stmt list, optTail : Ast.expr option) : unit =
    let
      val envAfter = List.foldl (fn (st, acc) => checkStmt retTy acc st) env stmts
    in
      case optTail of
        SOME e => expect (synth retTy envAfter e, retTy)
      | NONE =>
          if lastStmtReturns stmts then ()
          else if Unify.unify (retTy, Types.TyUnit) then ()
          else raise Fail "E0408: function body must return, end with return, or be unit"
    end

  fun checkExprAsBody (env : venv) (retTy : Types.ty) (body : Ast.expr) : unit =
    case body of
      Ast.ExprBlock (stmts, opt, _) => checkBlock env retTy (stmts, opt)
    | _ => expect (synth retTy env body, retTy)

  fun itemFnTy (r : {
        name : Ast.ident, params : (Ast.pat * Ast.ty) list,
        ret : Ast.ty option, contracts : Ast.contract list,
        body : Ast.expr, span : Span.span
      }) : Types.ty =
    let
      val retTy =
        case #ret r of
          NONE => raise Fail "E0409: function needs an explicit return type in this slice"
        | SOME at =>
            (case astTyToTy at of
               SOME t => t
             | NONE => raise Fail "E0406: unknown return type")
      val paramTys =
        List.map
          (fn (_, pt) =>
             case astTyToTy pt of
               SOME t => t
             | NONE => raise Fail "E0406: unknown parameter type")
          (#params r)
    in
      Types.TyFn (paramTys, retTy)
    end

  fun modEnvFromProg (prog : Ast.program) : venv =
    let
      fun one (it : Ast.item, acc : venv) : venv =
        case it of
          Ast.ItemFn r => extend acc (#name r) (itemFnTy r)
        | _ => acc
      val acc0 = List.foldl one [] prog
      val acc1 = extend acc0 "println" (Types.TyFn ([Types.TyString], Types.TyUnit))
    in
      List.foldl
        (fn ((en, vars), acc) =>
           List.foldl
             (fn ((vn, sh), a) =>
                extend a (pathKey [en, vn]) (ctorTy en sh)) acc vars) acc1
        (!enumDefs)
    end

  fun checkFn (modEnv : venv)
    (r : {
        name : Ast.ident, params : (Ast.pat * Ast.ty) list,
        ret : Ast.ty option, contracts : Ast.contract list,
        body : Ast.expr, span : Span.span
      }) : unit =
    let
      val retTy =
        case #ret r of
          NONE => raise Fail "E0409: function needs an explicit return type in this slice"
        | SOME at =>
            (case astTyToTy at of
               SOME t => t
             | NONE => raise Fail "E0406: unknown return type")
      val env1 =
        List.foldl
          (fn ((p, pt), acc) =>
             case p of
               Ast.PatBind (x, _) =>
                 (case astTyToTy pt of
                    SOME t => extend acc x t
                  | NONE => raise Fail "E0406: unknown parameter type")
             | _ => raise Fail "E0410: parameters must be simple bindings here")
          modEnv (#params r)
      val () = app (checkContract env1 retTy) (#contracts r)
    in
      checkExprAsBody env1 retTy (#body r)
    end

  fun check (prog : Ast.program) : Ast.program =
    let
      val () = initTypes prog
      val modEnv = modEnvFromProg prog
      fun checkOne it =
        case it of
          Ast.ItemFn r => checkFn modEnv r
        | _ => ()
    in
      (app checkOne prog; prog)
    end
end

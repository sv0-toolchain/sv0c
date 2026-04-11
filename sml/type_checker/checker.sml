(* Type checking for a Milestone-1 slice: literals, paths, let with PatBind,
   return / block tail, and unit functions with empty body. E04xx = types. *)

structure Checker :> CHECKER = struct

  type venv = (string * Types.ty * bool) list
  (* Third component: assignable (`let mut`); plain `let` and params are false. *)

  val loopDepth = ref 0

  (* Set for the duration of checkContract on each requires/ensures expression. *)
  val inContractExpr : bool ref = ref false
  val contractParamNames : string list ref = ref []

  fun withContractExpr (thunk : unit -> unit) : unit =
    let val () = inContractExpr := true
    in (thunk (); inContractExpr := false) handle ex =>
      (inContractExpr := false; raise ex)
    end

  fun isContractParam (x : string) : bool =
    List.exists (fn p => String.compare (p, x) = EQUAL) (!contractParamNames)

  fun withLoop (f : unit -> 'a) : 'a =
    let val () = loopDepth := !loopDepth + 1
        val r = f () handle ex => (loopDepth := !loopDepth - 1; raise ex)
    in
      loopDepth := !loopDepth - 1;
      r
    end

  fun extend (env : venv) (x : string) (t : Types.ty) (mut : bool) : venv =
    (x, t, mut) :: env

  fun lookup [] x =
        raise Fail ("E0401: unbound value in type checker: `" ^ x ^ "`")
    | lookup ((n, t, _) :: r) x =
        if String.compare (n, x) = EQUAL then t else lookup r x

  fun envEntry (env : venv) (x : string) : (Types.ty * bool) option =
    case env of
      [] => NONE
    | (n, t, m) :: r =>
        if String.compare (n, x) = EQUAL then SOME (t, m) else envEntry r x

  datatype variantShape = VSUnit | VSTuple of Types.ty list | VSStruct of (string * Types.ty) list

  val structDefs : (string * (string * Types.ty) list) list ref = ref []
  val enumDefs : (string * (string * variantShape) list) list ref = ref []

  val currentTypeParams : (string * Types.ty) list ref = ref []

  fun withTypeParams (tps : Ast.ident list) (f : unit -> 'a) : 'a =
    let
      val saved = !currentTypeParams
      val bindings = map (fn n => (n, Types.freshVar ())) tps
      val () = currentTypeParams := bindings @ saved
      val r = f () handle ex => (currentTypeParams := saved; raise ex)
    in
      currentTypeParams := saved; r
    end

  (* `use mod::Ty` import aliases (short name -> mangled canonical type name). *)
  val tyImportAliases : (string * string) list ref = ref []

  fun clearTyImportAliases () : unit = tyImportAliases := []

  fun addTyImportAlias (from : string) (to : string) : unit =
    tyImportAliases := (from, to) :: !tyImportAliases

  fun canonTyImport (n : string) : string =
    case List.find (fn (a, _) => String.compare (a, n) = EQUAL) (!tyImportAliases) of
      SOME (_, b) =>
        if String.compare (b, n) = EQUAL then n else canonTyImport b
    | NONE => n

  fun structNames () : string list = map #1 (!structDefs)
  fun enumNames () : string list = map #1 (!enumDefs)

  fun fieldsOfStruct (sn : string) : (string * Types.ty) list =
    let val s0 = canonTyImport sn
    in
      case List.find (fn (n, _) => String.compare (n, s0) = EQUAL) (!structDefs) of
        SOME (_, fs) => fs
      | NONE => raise Fail ("E0420: unknown struct type `" ^ sn ^ "`")
    end

  fun variantShapeOf (en : string) (vn : string) : variantShape =
    let val e0 = canonTyImport en
    in
      case List.find (fn (n, _) => String.compare (n, e0) = EQUAL) (!enumDefs) of
        SOME (_, vs) =>
          (case List.find (fn (v, _) => String.compare (v, vn) = EQUAL) vs of
             SOME (_, sh) => sh
           | NONE =>
               raise Fail ("E0421: unknown variant `" ^ en ^ "::" ^ vn ^ "`"))
      | NONE => raise Fail ("E0422: unknown enum type `" ^ en ^ "`")
    end

  fun ctorTy (en : string) (sh : variantShape) : Types.ty =
    case sh of
      VSUnit => Types.TyFn ([], Types.TyEnum en)
    | VSTuple ts => Types.TyFn (ts, Types.TyEnum en)
    | VSStruct fs => Types.TyFn (map #2 fs, Types.TyEnum en)

  fun pathKey (p : Ast.path) : string = String.concatWith "::" p

  fun astTyToTy (t : Ast.ty) : Types.ty option =
    case t of
      Ast.TyName (["Vec"], [elemTy], _) =>
        (case astTyToTy elemTy of
           SOME u => SOME (Types.TyNamed ("Vec", [u]))
         | NONE => NONE)
    | Ast.TyName (["Box"], [elemTy], _) =>
        (case astTyToTy elemTy of
           SOME u => SOME (Types.TyNamed ("Box", [u]))
         | NONE => NONE)
    | Ast.TyName ([name], _, _) =>
        let val name0 = canonTyImport name
        in
          case name0 of
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
              if List.exists (fn s => String.compare (s, name0) = EQUAL) (structNames ()) then
                SOME (Types.TyStruct name0)
              else if List.exists (fn e => String.compare (e, name0) = EQUAL) (enumNames ()) then
                SOME (Types.TyEnum name0)
              else
                (case List.find (fn (n, _) => n = name0) (!currentTypeParams) of
                   SOME (_, tv) => SOME tv
                 | NONE => NONE)
        end
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

  (* `?` on TyEnum en: detect Ok/Err (Result) or Some/None (Option). *)
  fun trySuccessPayloadTy (en : string) : Types.ty =
    let
      val hasOk = (variantShapeOf en "Ok"; true) handle Fail _ => false
      val hasErr = (variantShapeOf en "Err"; true) handle Fail _ => false
      val hasSome = (variantShapeOf en "Some"; true) handle Fail _ => false
      val hasNone = (variantShapeOf en "None"; true) handle Fail _ => false
    in
      if hasOk andalso hasErr then
        case variantShapeOf en "Ok" of
          VSTuple [tOk] => tOk
        | _ => raise Fail "E0441: `?` requires Ok(T) with exactly one payload field"
      else if hasSome andalso hasNone then
        case variantShapeOf en "Some" of
          VSTuple [tSome] => tSome
        | _ => raise Fail "E0441: `?` requires Some(T) with exactly one payload field"
      else
        raise Fail "E0441: `?` requires an enum with Ok(T)/Err(E) or Some(T)/None"
    end

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
          Ast.TyName (["Vec"], [elemTy], _) =>
            Types.TyNamed ("Vec", [resolveFieldTy elemTy])
        | Ast.TyName (["Box"], [elemTy], _) =>
            Types.TyNamed ("Box", [resolveFieldTy elemTy])
        | Ast.TyName ([name], _, _) =>
            (case name of
               "unit" => Types.TyUnit
             | "bool" => Types.TyBool
             | "char" => Types.TyChar
             | "string" => Types.TyString
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
                  | NONE =>
                      (case List.find (fn (n, _) => n = name) (!currentTypeParams) of
                         SOME (_, tv) => tv
                       | NONE => raise Fail ("E0406: unknown type in struct/enum field: `" ^ name ^ "`"))))
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
            name : Ast.ident, type_params : Ast.ident list,
            fields : (Ast.ident * Ast.ty) list,
            span : Span.span
          }) =
        withTypeParams (#type_params r) (fn () =>
          (#name r, map (fn (f, aty) => (f, resolveFieldTy aty)) (#fields r)))
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
            VSStruct (map (fn (f, t) => (f, resolveFieldTy t)) fs)
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
            name : Ast.ident, type_params : Ast.ident list,
            variants : Ast.variant list,
            span : Span.span
          }) =
        withTypeParams (#type_params r) (fn () =>
        let val vnames = map variantName (#variants r)
        in
          dupVariants vnames;
          (#name r, map (fn v => (variantName v, variantShape v)) (#variants r))
        end)
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

  datatype assign_lhs = ALVar of string | ALField of string * string

  fun classifyAssignLhs (lhs : Ast.expr) : assign_lhs option =
    case lhs of
      Ast.ExprPath ([x], _) => SOME (ALVar x)
    | Ast.ExprField (Ast.ExprPath ([b], _), fld, _) => SOME (ALField (b, fld))
    | _ => NONE

  fun tyOfStructField (sn : string) (f : string) : Types.ty =
    case List.find (fn (n, _) => n = f) (fieldsOfStruct sn) of
      SOME (_, t) => t
    | NONE =>
        raise Fail ("E0429: struct `" ^ sn ^ "` has no field `" ^ f ^ "`")

  fun assignLhsBad () : 'a =
    raise Fail
      "E0449: assignment left-hand side must be a simple name or a one-level field `p.f` of a local binding in this slice"

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
          orelse (case scrutTy of
                    Types.TyEnum _ => true
                  | Types.TyStruct _ => true
                  | Types.TyNamed _ => true
                  | Types.TyString => true
                  | Types.TyVar _ => true
                  | _ => false)
        then
          extend env x scrutTy false
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
    | Ast.PatStruct ([en, vn], fieldPats, _) =>
        let val () = expect (scrutTy, Types.TyEnum en)
        in
          case variantShapeOf en vn of
            VSStruct fs =>
              let
                fun bindField env (fieldName, fieldPat) =
                  case List.find (fn (n, _) => n = fieldName) fs of
                    SOME (_, fieldTy) => bindPat fieldTy fieldPat env
                  | NONE =>
                      raise Fail ("E0429: unknown field `" ^ fieldName
                                  ^ "` in struct pattern `" ^ en ^ "::" ^ vn ^ "`")
              in
                List.foldl (fn (fp, e) => bindField e fp) env fieldPats
              end
          | _ => raise Fail ("E0429: struct pattern on non-struct variant `"
                             ^ en ^ "::" ^ vn ^ "`")
        end
    | _ => raise Fail "E0428: pattern form not supported in match"

  (* Multi-segment path calls: builtins, quantifiers, println, user fns. *)
  fun synthExprCall (retTy : Types.ty) (env : venv) (segs : Ast.path)
    (args : Ast.expr list) : Types.ty =
    case segs of
      ["old"] =>
        if not (!inContractExpr) then
          raise Fail
            "E0521: old(...) is only allowed in requires/ensures (hint: place it in a contract clause)"
        else
          (case args of
             [Ast.ExprPath ([x], _)] =>
                   if not (isContractParam x) then
                     raise Fail
                       "E0522: old(...) argument must be a function parameter (hint: use a parameter name, not a local binding)"
                   else
                     lookup env x
               | _ =>
                   raise Fail
                     "E0522: old(...) expects a single parameter name (hint: use old(x), not an expression such as old(x + 1))")
    | ["forall"] =>
        if not (!inContractExpr) then
          raise Fail
            "E0523: quantifiers are only allowed in requires/ensures (hint: forall(i in lo..hi, pred) with i32 range)"
        else
          (case args of
             [Ast.ExprPath ([i], _), dom, body] =>
               (case dom of
                  Ast.ExprRange (SOME lo, SOME hi, _) =>
                    let
                      val () = expect (synth retTy env lo, Types.TyInt 32)
                      val () = expect (synth retTy env hi, Types.TyInt 32)
                      val envI = extend env i (Types.TyInt 32) false
                    in
                      expect (synth retTy envI body, Types.TyBool);
                      Types.TyBool
                    end
                | _ =>
                    raise Fail
                      "E0523: forall domain must be a half-open i32 range `lo..hi`")
           | _ =>
               raise Fail "E0523: forall expects `(i in lo..hi, predicate)`")
    | ["exists"] =>
        if not (!inContractExpr) then
          raise Fail
            "E0523: quantifiers are only allowed in requires/ensures (hint: exists(i in lo..hi, pred) with i32 range)"
        else
          (case args of
             [Ast.ExprPath ([i], _), dom, body] =>
               (case dom of
                  Ast.ExprRange (SOME lo, SOME hi, _) =>
                    let
                      val () = expect (synth retTy env lo, Types.TyInt 32)
                      val () = expect (synth retTy env hi, Types.TyInt 32)
                      val envI = extend env i (Types.TyInt 32) false
                    in
                      expect (synth retTy envI body, Types.TyBool);
                      Types.TyBool
                    end
                | _ =>
                    raise Fail
                      "E0523: exists domain must be a half-open i32 range `lo..hi`")
           | _ =>
               raise Fail "E0523: exists expects `(i in lo..hi, predicate)`")
    | ["no_alias"] =>
        if not (!inContractExpr) then
          raise Fail
            "E0526: no_alias(...) is only allowed in requires/ensures (hint: no_alias is checked only in contract clauses; move it to requires or ensures)"
        else
          (case args of
             [a, b] =>
               let
                 val ta = synth retTy env a
                 val tb = synth retTy env b
               in
                 case (ta, tb) of
                   (Types.TyRef t1, Types.TyRef t2) =>
                     if Unify.unify (t1, t2) then Types.TyBool
                     else
                       raise Fail
                         "E0524: no_alias expects two references to the same inner type"
                 | (Types.TyRefMut t1, Types.TyRefMut t2) =>
                     if Unify.unify (t1, t2) then Types.TyBool
                     else
                       raise Fail
                         "E0524: no_alias expects two references to the same inner type"
                 | (Types.TyRef t1, Types.TyRefMut t2) =>
                     if Unify.unify (t1, t2) then Types.TyBool
                     else
                       raise Fail
                         "E0524: no_alias expects two references to the same inner type"
                 | (Types.TyRefMut t1, Types.TyRef t2) =>
                     if Unify.unify (t1, t2) then Types.TyBool
                     else
                       raise Fail
                         "E0524: no_alias expects two references to the same inner type"
                 | _ =>
                     raise Fail
                       "E0524: no_alias expects `&x` / `&mut x` arguments of the same `T`"
               end
           | _ => raise Fail "E0524: no_alias expects exactly two arguments")
    | [nm] =>
        if String.compare (nm, "println") = EQUAL then
          case args of
            [arg] =>
              let val argTy = synth retTy env arg
              in
                if Unify.unify (argTy, Types.TyString) then Types.TyUnit
                else raise Fail
                  "E0444: println expects a string argument (hint: println(\"text\") or println(s) where s: string)"
              end
          | _ =>
              raise Fail
                "E0444: println expects exactly one argument"
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
    | _ =>
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

  (* retTy is the enclosing function's return type (for return/blocks in subexprs). *)
  and synth (retTy : Types.ty) (env : venv) (e : Ast.expr) : Types.ty =
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
              if lastStmtReturns stmts then Types.freshVar () else Types.TyUnit
        end
    | Ast.ExprUnop (Ast.Neg, e1, _) =>
        (expect (synth retTy env e1, Types.TyInt 32); Types.TyInt 32)
    | Ast.ExprUnop (Ast.Not, e1, _) =>
        (expect (synth retTy env e1, Types.TyBool); Types.TyBool)
    | Ast.ExprUnop (Ast.BitNot, e1, _) =>
        (expect (synth retTy env e1, Types.TyInt 32); Types.TyInt 32)
    | Ast.ExprUnop (Ast.Borrow, e1, _) =>
        if not (!inContractExpr) then
          raise Fail
            "E0525: `&` is only allowed in requires/ensures expressions (hint: use `&` only inside contract clauses, e.g. no_alias(&a, &b))"
        else
          (case e1 of
             Ast.ExprPath ([x], _) =>
               (case lookup env x of
                  Types.TyFn _ =>
                    raise Fail "E0525: cannot take address of a function in a contract"
                | t => Types.TyRef t)
           | _ =>
               raise Fail "E0525: `&` in contracts requires a simple parameter name")
    | Ast.ExprUnop (Ast.BorrowMut, e1, _) =>
        if not (!inContractExpr) then
          raise Fail
            "E0525: `&mut` is only allowed in requires/ensures expressions (hint: use `&mut` only inside contract clauses)"
        else
          (case e1 of
             Ast.ExprPath ([x], _) =>
               (case lookup env x of
                  Types.TyFn _ =>
                    raise Fail "E0525: cannot take address of a function in a contract"
                | t => Types.TyRefMut t)
           | _ =>
               raise Fail "E0525: `&mut` in contracts requires a simple parameter name")
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
        let
          val () = expect (synth retTy env c, Types.TyBool)
          val tTh = synth retTy env th
          val () =
            case th of
              Ast.ExprBlock (stmts, NONE, _) =>
                if lastStmtReturns stmts then () else expect (tTh, Types.TyUnit)
            | Ast.ExprBlock (_, SOME _, _) => expect (tTh, Types.TyUnit)
            | _ => expect (tTh, Types.TyUnit)
        in
          Types.TyUnit
        end
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
          val envB = extend env x (Types.TyInt 32) false
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
        synthExprCall retTy env segs args
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
              let fun isVar (Types.TyVar _) = true
                    | isVar _ = false
                  val concrete =
                    case List.find (fn u => not (isVar u)) (t :: ts) of
                      SOME c => c
                    | NONE => t
              in
              if List.all (fn u => Unify.unify (concrete, u)) (t :: ts) then concrete
              else raise Fail "E0400: type mismatch"
              end
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
              in
                trySuccessPayloadTy en
              end
          | _ => raise Fail "E0445: `?` operand must be an enum (Ok/Err or Some/None shape)"
        end
    | _ =>
        raise Fail "E0402: expression form not supported in type checker slice"

  and checkPlainAssign (env : venv) (retTy : Types.ty) (lhs : Ast.expr)
    (rhs : Ast.expr) : unit =
    case classifyAssignLhs lhs of
      NONE => assignLhsBad ()
    | SOME (ALVar x) =>
        (case envEntry env x of
           SOME (t, true) => expect (synth retTy env rhs, t)
         | SOME (_, false) =>
             raise Fail
               ("E0448: cannot assign to immutable binding `" ^ x
                ^ "` (hint: use `let mut " ^ x ^ "` if you need assignment)")
         | NONE =>
             raise Fail ("E0401: unbound value in type checker: `" ^ x ^ "`"))
    | SOME (ALField (b, f)) =>
        (case envEntry env b of
           SOME (Types.TyStruct sn, true) =>
             let val tFld = tyOfStructField sn f
             in expect (synth retTy env rhs, tFld) end
         | SOME (_, false) =>
             raise Fail
               ("E0448: cannot assign to field through immutable binding `" ^ b
                ^ "` (hint: use `let mut " ^ b ^ "` if you need field assignment)")
         | SOME _ =>
             raise Fail
               "E0449: field assignment requires the base binding to have struct type"
         | NONE =>
             raise Fail ("E0401: unbound value in type checker: `" ^ b ^ "`"))

  and checkCompoundAssign (env : venv) (retTy : Types.ty) (b : Ast.binop)
    (lhs : Ast.expr) (rhs : Ast.expr) : unit =
    case binopClass b of
      Arith =>
        (case classifyAssignLhs lhs of
           NONE => assignLhsBad ()
         | SOME (ALVar x) =>
             (case envEntry env x of
                SOME (t, true) =>
                  (expect (t, Types.TyInt 32); expect (synth retTy env rhs, Types.TyInt 32))
              | SOME (_, false) =>
                  raise Fail
                    ("E0448: cannot assign to immutable binding `" ^ x
                     ^ "` (hint: use `let mut " ^ x ^ "` if you need compound assignment)")
              | NONE =>
                  raise Fail ("E0401: unbound value in type checker: `" ^ x ^ "`"))
         | SOME (ALField (base, f)) =>
             (case envEntry env base of
                SOME (Types.TyStruct sn, true) =>
                  let val tFld = tyOfStructField sn f
                  in
                    expect (tFld, Types.TyInt 32);
                    expect (synth retTy env rhs, Types.TyInt 32)
                  end
              | SOME (_, false) =>
                  raise Fail
                    ("E0448: cannot assign to field through immutable binding `" ^ base
                     ^ "` (hint: use `let mut " ^ base
                     ^ "` if you need compound field assignment)")
              | SOME _ =>
                  raise Fail
                    "E0449: field assignment requires the base binding to have struct type"
              | NONE =>
                  raise Fail ("E0401: unbound value in type checker: `" ^ base ^ "`")))
    | _ =>
        raise Fail
          "E0450: compound assignment (`+=`, etc.) only supports arithmetic operators in this slice"

  and checkExprStmt (env : venv) (retTy : Types.ty) (e : Ast.expr) : unit =
    case e of
      Ast.ExprReturn (NONE, _) => expect (Types.TyUnit, retTy)
    | Ast.ExprReturn (SOME e2, _) => expect (synth retTy env e2, retTy)
    | Ast.ExprAssign (lhs, rhs, _) => checkPlainAssign env retTy lhs rhs
    | Ast.ExprAssignOp (b, lhs, rhs, _) => checkCompoundAssign env retTy b lhs rhs
    | _ => ignore (synth retTy env e)

  and checkStmt (retTy : Types.ty) (env : venv) (st : Ast.stmt) : venv =
    case st of
      Ast.StmtLet (Ast.PatBind (x, _), isMut, to, SOME init, _) =>
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
          extend env x tyBind isMut
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
    | Ast.ExprAssign (l, r, _) =>
        exprReferencesResult l orelse exprReferencesResult r
    | Ast.ExprAssignOp (_, l, r, _) =>
        exprReferencesResult l orelse exprReferencesResult r
    | _ => false

  fun checkContract (env : venv) (retTy : Types.ty) (c : Ast.contract) : unit =
    case c of
      Ast.Requires (e, _) =>
        withContractExpr (fn () => expect (synth retTy env e, Types.TyBool))
    | Ast.Ensures (e, _) =>
        let
          val envR = extend env "result" retTy false
        in
          if Unify.unify (retTy, Types.TyUnit) andalso exprReferencesResult e then
            raise Fail "E0520: ensures cannot use result when return type is unit"
          else
            withContractExpr (fn () => expect (synth retTy envR e, Types.TyBool))
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
        name : Ast.ident, type_params : Ast.ident list,
        params : (Ast.pat * Ast.ty) list,
        ret : Ast.ty option, contracts : Ast.contract list,
        body : Ast.expr, span : Span.span
      }) : Types.ty =
    withTypeParams (#type_params r) (fn () =>
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
    end)

  fun envFind (env : venv) (x : string) : Types.ty option =
    case envEntry env x of
      SOME (t, _) => SOME t
    | NONE => NONE

  fun modEnvFromProg (prog : Ast.program) : venv =
    let
      fun one (it : Ast.item, acc : venv) : venv =
        case it of
          Ast.ItemFn r => extend acc (#name r) (itemFnTy r) false
        | _ => acc
      val acc0 = List.foldl one [] prog
      val acc1 = extend acc0 "println" (Types.TyFn ([Types.TyString], Types.TyUnit)) false
      val acc1a = extend acc1 "string_len" (Types.TyFn ([Types.TyString], Types.TyInt 32)) false
      val acc1b = extend acc1a "string_eq" (Types.TyFn ([Types.TyString, Types.TyString], Types.TyBool)) false
      val acc1c = extend acc1b "string_concat" (Types.TyFn ([Types.TyString, Types.TyString], Types.TyString)) false
      val acc1d = extend acc1c "string_char_at" (Types.TyFn ([Types.TyString, Types.TyInt 32], Types.TyInt 32)) false
      val acc1e = extend acc1d "string_substr" (Types.TyFn ([Types.TyString, Types.TyInt 32, Types.TyInt 32], Types.TyString)) false
      val vecElem = Types.freshVar ()
      val acc1f = extend acc1e "vec_new" (Types.TyFn ([], Types.TyNamed ("Vec", [vecElem]))) false
      val acc1g = extend acc1f "vec_push" (Types.TyFn ([Types.TyNamed ("Vec", [Types.freshVar ()]), Types.freshVar ()], Types.TyUnit)) false
      val acc1h = extend acc1g "vec_len" (Types.TyFn ([Types.TyNamed ("Vec", [Types.freshVar ()])], Types.TyInt 32)) false
      val acc1i = extend acc1h "vec_get" (Types.TyFn ([Types.TyNamed ("Vec", [Types.freshVar ()]), Types.TyInt 32], Types.freshVar ())) false
      val acc1j = extend acc1i "vec_set" (Types.TyFn ([Types.TyNamed ("Vec", [Types.freshVar ()]), Types.TyInt 32, Types.freshVar ()], Types.TyUnit)) false
      val acc1k = extend acc1j "box_new" (Types.TyFn ([Types.freshVar ()], Types.TyNamed ("Box", [Types.freshVar ()]))) false
      val acc1l = extend acc1k "box_deref" (Types.TyFn ([Types.TyNamed ("Box", [Types.freshVar ()])], Types.freshVar ())) false
      val acc2 =
        List.foldl
          (fn ((en, vars), acc) =>
             List.foldl
               (fn ((vn, sh), a) =>
                  extend a (pathKey [en, vn]) (ctorTy en sh) false) acc vars) acc1l
          (!enumDefs)
      fun oneUse (it : Ast.item, acc : venv) : venv =
        case it of
          Ast.ItemUse ([m, nm], _) =>
            let val q = m ^ "__" ^ nm
            in
              case envFind acc q of
                SOME t => extend acc nm t false
              | NONE =>
                  if List.exists (fn s => String.compare (s, q) = EQUAL) (structNames ()) then
                    (addTyImportAlias nm q; acc)
                  else if List.exists (fn e => String.compare (e, q) = EQUAL) (enumNames ()) then
                    let
                      val vars =
                        case List.find (fn (en, _) => String.compare (en, q) = EQUAL) (!enumDefs) of
                          SOME (_, vs) => vs
                        | NONE => []
                      val () = addTyImportAlias nm q
                    in
                      List.foldl
                        (fn ((vn, sh), a) =>
                           extend a (pathKey [nm, vn]) (ctorTy q sh) false) acc vars
                    end
                  else acc
            end
        | _ => acc
    in
      List.foldl oneUse acc2 prog
    end

  fun checkFn (modEnv : venv)
    (r : {
        name : Ast.ident, type_params : Ast.ident list,
        params : (Ast.pat * Ast.ty) list,
        ret : Ast.ty option, contracts : Ast.contract list,
        body : Ast.expr, span : Span.span
      }) : unit =
    withTypeParams (#type_params r) (fn () =>
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
                    SOME t => extend acc x t false
                  | NONE => raise Fail "E0406: unknown parameter type")
             | _ => raise Fail "E0410: parameters must be simple bindings here")
          modEnv (#params r)
      val paramNames =
        List.mapPartial
          (fn (p, _) =>
             case p of Ast.PatBind (x, _) => SOME x | _ => NONE) (#params r)
      val () = contractParamNames := paramNames
      val () =
        (app (checkContract env1 retTy) (#contracts r); contractParamNames := [])
        handle ex => (contractParamNames := []; raise ex)
    in
      checkExprAsBody env1 retTy (#body r)
    end)

  fun check (prog : Ast.program) : Ast.program =
    let
      val () = clearTyImportAliases ()
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

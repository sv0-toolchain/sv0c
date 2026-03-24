(* Type checking for a Milestone-1 slice: literals, paths, let with PatBind,
   return / block tail, and unit functions with empty body. E04xx = types. *)

structure Checker :> CHECKER = struct

  type venv = (string * Types.ty) list

  fun extend (env : venv) (x : string) (t : Types.ty) : venv = (x, t) :: env

  fun lookup [] x =
        raise Fail ("E0401: unbound value in type checker: `" ^ x ^ "`")
    | lookup ((n, t) :: r) x =
        if String.compare (n, x) = EQUAL then t else lookup r x

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
         | _ => NONE)
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

  (* retTy is the enclosing function's return type (for return/blocks in subexprs). *)
  fun synth (retTy : Types.ty) (env : venv) (e : Ast.expr) : Types.ty =
    case e of
      Ast.ExprLit (Ast.IntLit _, _) => Types.TyInt 32
    | Ast.ExprLit (Ast.BoolLit _, _) => Types.TyBool
    | Ast.ExprLit (Ast.UnitLit, _) => Types.TyUnit
    | Ast.ExprPath ([x], _) => lookup env x
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

  fun checkContract (env : venv) (c : Ast.contract) : unit =
    case c of
      Ast.Requires (e, _) => expect (synth Types.TyUnit env e, Types.TyBool)
    | Ast.Ensures _ =>
        raise Fail "E0501: ensures contracts are not type-checked in this slice"
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

  fun checkFn (r : {
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
      val env0 : venv = []
      val env1 =
        List.foldl
          (fn ((p, pt), acc) =>
             case p of
               Ast.PatBind (x, _) =>
                 (case astTyToTy pt of
                    SOME t => extend acc x t
                  | NONE => raise Fail "E0406: unknown parameter type")
             | _ => raise Fail "E0410: parameters must be simple bindings here")
          env0 (#params r)
      val () = app (checkContract env1) (#contracts r)
    in
      checkExprAsBody env1 retTy (#body r)
    end

  fun checkItem (it : Ast.item) : unit =
    case it of
      Ast.ItemFn r => checkFn r
    | _ => ()

  fun check (prog : Ast.program) : Ast.program =
    (app checkItem prog; prog)
end

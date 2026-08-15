(* BH-12: alpha-rename shadowed local bindings so no emitted scope redeclares a
   name. sv0 permits `let` shadowing (and a nested block / match arm may re-bind a
   name live in an enclosing scope), but the C backend flattens every local into
   one function scope and the VM keys slots by name — both mis-handle a re-bound
   name. This AST->AST pass runs after checking and before lowering, for BOTH the
   C and VM targets (they share `Lowering.lower`), so one pass fixes both.

   Only a binding that actually SHADOWS a currently-live binding of the same name
   is renamed (to `x__N`, N a fresh global id); a `let x = <init>` reads the old
   `x` because the init is renamed under the pre-binding env. Non-shadowing code
   is returned structurally unchanged, so the whole (shadow-free) bootstrap corpus
   is byte-identical and the self-host loop is unaffected. Every binding form
   participates — `let`, match-arm patterns, `for` indices — and every scope
   (block, match arm, for body) is entered with the enclosing env, so references
   always resolve to the right binding. *)

structure ShadowRename :> sig
  val renameProgram : Ast.program -> Ast.program
end = struct

  val freshCtr = ref 0
  fun freshName (x : string) : string =
    (freshCtr := !freshCtr + 1; x ^ "__" ^ Int.toString (!freshCtr))

  type env = (string * string) list
  fun aliasOf (env : env) (x : string) : string =
    case List.find (fn (n, _) => n = x) env of SOME (_, a) => a | NONE => x
  fun isBound (env : env) (x : string) : bool =
    List.exists (fn (n, _) => n = x) env

  (* Give a source name a C alias: fresh iff it shadows a live binding. *)
  fun bindName (env : env) (x : string) : string * env =
    let val a = if isBound env x then freshName x else x
    in (a, (x, a) :: env) end

  (* Rename a pattern's binders; thread env through nested binders. Returns the
     rewritten pattern and the env extended with its bindings. *)
  fun renamePat (env : env) (p : Ast.pat) : Ast.pat * env =
    case p of
      Ast.PatWild _ => (p, env)
    | Ast.PatLit _ => (p, env)
    | Ast.PatBind (x, sp) =>
        let val (a, env') = bindName env x in (Ast.PatBind (a, sp), env') end
    | Ast.PatTuple (ps, sp) =>
        let val (ps', env') = renamePatList env ps in (Ast.PatTuple (ps', sp), env') end
    | Ast.PatEnum (path, ps, sp) =>
        let val (ps', env') = renamePatList env ps in (Ast.PatEnum (path, ps', sp), env') end
    | Ast.PatStruct (path, fps, sp) =>
        let
          fun go env [] acc = (rev acc, env)
            | go env ((f, fp) :: rest) acc =
                let val (fp', env') = renamePat env fp
                in go env' rest ((f, fp') :: acc) end
          val (fps', env') = go env fps []
        in (Ast.PatStruct (path, fps', sp), env') end
    | Ast.PatOr (ps, sp) =>
        (* Or-patterns bind the same names in each alternative; rename each under
           the SAME incoming env and keep one resulting env. *)
        let
          fun go [] envAcc acc = (rev acc, envAcc)
            | go (q :: rest) envAcc acc =
                let val (q', env') = renamePat env q
                in go rest env' (q' :: acc) end
          val (ps', env') = go ps env []
        in (Ast.PatOr (ps', sp), env') end

  and renamePatList (env : env) (ps : Ast.pat list) : Ast.pat list * env =
    case ps of
      [] => ([], env)
    | p :: rest =>
        let val (p', env1) = renamePat env p
            val (rest', env2) = renamePatList env1 rest
        in (p' :: rest', env2) end

  fun renameExpr (env : env) (e : Ast.expr) : Ast.expr =
    case e of
      Ast.ExprLit _ => e
    | Ast.ExprPath ([x], sp) => Ast.ExprPath ([aliasOf env x], sp)
    | Ast.ExprPath _ => e  (* multi-seg: enum/module paths, not a local *)
    | Ast.ExprUnop (u, e1, sp) => Ast.ExprUnop (u, renameExpr env e1, sp)
    | Ast.ExprBinop (b, e1, e2, sp) =>
        Ast.ExprBinop (b, renameExpr env e1, renameExpr env e2, sp)
    | Ast.ExprCall (f, args, sp) =>
        Ast.ExprCall (renameExpr env f, map (renameExpr env) args, sp)
    | Ast.ExprMethodCall (obj, m, args, sp) =>
        Ast.ExprMethodCall (renameExpr env obj, m, map (renameExpr env) args, sp)
    | Ast.ExprField (e1, f, sp) => Ast.ExprField (renameExpr env e1, f, sp)
    | Ast.ExprTupleField (e1, i, sp) => Ast.ExprTupleField (renameExpr env e1, i, sp)
    | Ast.ExprIndex (e1, e2, sp) =>
        Ast.ExprIndex (renameExpr env e1, renameExpr env e2, sp)
    | Ast.ExprBlock (stmts, opt, sp) =>
        let val (stmts', env') = renameStmts env stmts
        in Ast.ExprBlock (stmts', Option.map (renameExpr env') opt, sp) end
    | Ast.ExprIf (c, t, eo, sp) =>
        Ast.ExprIf (renameExpr env c, renameExpr env t,
                    Option.map (renameExpr env) eo, sp)
    | Ast.ExprMatch (scrut, arms, sp) =>
        Ast.ExprMatch (renameExpr env scrut, map (renameArm env) arms, sp)
    | Ast.ExprWhile (c, invs, body, sp) =>
        Ast.ExprWhile (renameExpr env c, map (renameExpr env) invs,
                       renameExpr env body, sp)
    | Ast.ExprFor (p, iter, body, sp) =>
        let val iter' = renameExpr env iter
            val (p', env') = renamePat env p
        in Ast.ExprFor (p', iter', renameExpr env' body, sp) end
    | Ast.ExprLoop (body, sp) => Ast.ExprLoop (renameExpr env body, sp)
    | Ast.ExprReturn (eo, sp) => Ast.ExprReturn (Option.map (renameExpr env) eo, sp)
    | Ast.ExprBreak (eo, sp) => Ast.ExprBreak (Option.map (renameExpr env) eo, sp)
    | Ast.ExprContinue _ => e
    | Ast.ExprAssign (l, r, sp) =>
        Ast.ExprAssign (renameExpr env l, renameExpr env r, sp)
    | Ast.ExprAssignOp (b, l, r, sp) =>
        Ast.ExprAssignOp (b, renameExpr env l, renameExpr env r, sp)
    | Ast.ExprCast (e1, t, sp) => Ast.ExprCast (renameExpr env e1, t, sp)
    | Ast.ExprRange (lo, hi, sp) =>
        Ast.ExprRange (Option.map (renameExpr env) lo,
                       Option.map (renameExpr env) hi, sp)
    | Ast.ExprTry (e1, sp) => Ast.ExprTry (renameExpr env e1, sp)
    | Ast.ExprAssert (e1, sp) => Ast.ExprAssert (renameExpr env e1, sp)
    | Ast.ExprStruct (path, fields, sp) =>
        Ast.ExprStruct (path, map (fn (n, ex) => (n, renameExpr env ex)) fields, sp)
    | Ast.ExprTuple (es, sp) => Ast.ExprTuple (map (renameExpr env) es, sp)
    | Ast.ExprArray (es, sp) => Ast.ExprArray (map (renameExpr env) es, sp)

  and renameArm (env : env) (Ast.Arm (p, guard, body)) : Ast.arm =
    let val (p', env') = renamePat env p
    in Ast.Arm (p', Option.map (renameExpr env') guard, renameExpr env' body) end

  and renameStmts (env : env) (sts : Ast.stmt list) : Ast.stmt list * env =
    case sts of
      [] => ([], env)
    | st :: rest =>
        let val (st', env1) = renameStmt env st
            val (rest', env2) = renameStmts env1 rest
        in (st' :: rest', env2) end

  and renameStmt (env : env) (st : Ast.stmt) : Ast.stmt * env =
    case st of
      Ast.StmtLet (pat, mut, tyo, inito, sp) =>
        (* Rename the initializer under the OLD env (a `let x = x + 1` reads the
           previous x), then bind the pattern. *)
        let val inito' = Option.map (renameExpr env) inito
            val (pat', env') = renamePat env pat
        in (Ast.StmtLet (pat', mut, tyo, inito', sp), env') end
    | Ast.StmtExpr (e, sp) => (Ast.StmtExpr (renameExpr env e, sp), env)
    | Ast.StmtSemi (e, sp) => (Ast.StmtSemi (renameExpr env e, sp), env)
    | Ast.StmtItem _ => (st, env)

  (* Seed the env with the function's parameters (alias = name — params keep their
     signature names), so a `let` that shadows a param is renamed. *)
  fun paramNames (params : (Ast.pat * Ast.ty) list) : env =
    List.mapPartial
      (fn (Ast.PatBind (x, _), _) => SOME (x, x) | _ => NONE) params

  fun renameItem (it : Ast.item) : Ast.item =
    case it of
      Ast.ItemFn {name, type_params, params, ret, contracts, body, span} =>
        Ast.ItemFn {name = name, type_params = type_params, params = params,
                    ret = ret, contracts = contracts,
                    body = renameExpr (paramNames params) body, span = span}
    | Ast.ItemTrait {name, methods, span} =>
        Ast.ItemTrait {name = name, methods = map renameItem methods, span = span}
    | Ast.ItemImpl {trait_path, self_ty, items, span} =>
        Ast.ItemImpl {trait_path = trait_path, self_ty = self_ty,
                      items = map renameItem items, span = span}
    | _ => it

  fun renameProgram (items : Ast.program) : Ast.program = map renameItem items

end

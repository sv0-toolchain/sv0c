(* Name resolution (Milestone 1): duplicate top-level detection, unbound identifier
   checks, and type path validation for single-segment paths and Self in impl/trait. *)

structure Resolver :> RESOLVER = struct

  val recordedImportAliases : (string * string) list ref = ref []

  fun peekImportAliases () : (string * string) list = !recordedImportAliases

  type ctx = { allowSelf : bool }

  fun pathToString (p : Ast.path) : string =
    String.concatWith "::" p

  fun patBinds (p : Ast.pat) : string list =
    case p of
      Ast.PatWild _ => []
    | Ast.PatBind (x, _) => [x]
    | Ast.PatLit _ => []
    | Ast.PatTuple (ps, _) => List.concat (List.map patBinds ps)
    | Ast.PatStruct (_, fs, _) => List.concat (List.map (patBinds o #2) fs)
    | Ast.PatEnum (_, ps, _) => List.concat (List.map patBinds ps)
    | Ast.PatOr _ =>
        raise Fail "E0306: | patterns are not supported in name resolution yet"

  fun bindPatternLocals env p =
    List.foldl (fn (x, acc) => Env.bindLocal acc x) env (patBinds p)

  fun resolveTy (ctx : ctx) (env : Env.env) (t : Ast.ty) : unit =
    case t of
      Ast.TyName (path, sp) =>
        if Env.lookupType env ctx path then ()
        else
          raise Diagnostic.Diag
            (Diagnostic.error
               ("E0301", "unknown type `" ^ pathToString path ^ "`", sp))
    | Ast.TyRef (t2, _) => resolveTy ctx env t2
    | Ast.TyRefMut (t2, _) => resolveTy ctx env t2
    | Ast.TyArray (t2, e, _) =>
        (resolveTy ctx env t2; resolveExpr ctx env e)
    | Ast.TySlice (t2, _) => resolveTy ctx env t2
    | Ast.TyTuple (ts, _) => app (resolveTy ctx env) ts
    | Ast.TyUnit _ => ()
    | Ast.TyInfer _ => ()

  and resolveExpr (ctx : ctx) (env : Env.env) (e : Ast.expr) : unit =
    case e of
      Ast.ExprLit _ => ()
    | Ast.ExprPath (segs, sp) =>
        let val key = pathToString segs
        in
          if Env.lookupValue env key then ()
          else
            raise Diagnostic.Diag
              (Diagnostic.error
                 ("E0300", "unbound identifier `" ^ key ^ "`", sp))
        end
    | Ast.ExprUnop (_, e1, _) => resolveExpr ctx env e1
    | Ast.ExprBinop (_, a, b, _) =>
        (resolveExpr ctx env a; resolveExpr ctx env b)
    | Ast.ExprCall (Ast.ExprPath (["forall"], _), [Ast.ExprPath ([i], _), dom, body], _) =>
        ( resolveExpr ctx env dom
        ; resolveExpr ctx (Env.bindLocal env i) body)
    | Ast.ExprCall (Ast.ExprPath (["exists"], _), [Ast.ExprPath ([i], _), dom, body], _) =>
        ( resolveExpr ctx env dom
        ; resolveExpr ctx (Env.bindLocal env i) body)
    | Ast.ExprCall (f, args, _) =>
        let
          val () =
            case f of
              Ast.ExprPath (segs, _) =>
                let val key = pathToString segs
                in
                  case Env.lookupFnArity env key of
                    SOME n =>
                      if length args <> n then
                        raise Fail
                          ("E0307: wrong number of arguments for `" ^ key ^ "`")
                      else ()
                  | NONE =>
                      if length segs = 1 then
                        case segs of
                          [nm] =>
                            (case Env.lookupFnArity env nm of
                               SOME n =>
                                 if length args <> n then
                                   raise Fail
                                     ("E0307: wrong number of arguments for `" ^
                                      nm ^ "`")
                                 else ()
                             | NONE => ())
                        | _ => ()
                      else
                        ()
                end
            | _ => ()
        in
          (resolveExpr ctx env f; app (resolveExpr ctx env) args)
        end
    | Ast.ExprMethodCall (recv, _, args, _) =>
        (resolveExpr ctx env recv; app (resolveExpr ctx env) args)
    | Ast.ExprField (e1, _, _) => resolveExpr ctx env e1
    | Ast.ExprTupleField (e1, _, _) => resolveExpr ctx env e1
    | Ast.ExprIndex (a, b, _) =>
        (resolveExpr ctx env a; resolveExpr ctx env b)
    | Ast.ExprBlock (stmts, optTail, _) =>
        let
          val envB = Env.enterScope env
          val envEnd =
            List.foldl (fn (st, acc) => resolveStmt ctx acc st) envB stmts
        in
          case optTail of NONE => () | SOME e2 => resolveExpr ctx envEnd e2
        end
    | Ast.ExprIf (cond, th, el, _) =>
        (resolveExpr ctx env cond;
         resolveExpr ctx env th;
         case el of NONE => () | SOME e2 => resolveExpr ctx env e2)
    | Ast.ExprMatch (scr, arms, _) =>
        (resolveExpr ctx env scr;
         app
           (fn Ast.Arm (pat, guard, body) =>
              let
                val () = resolvePatShape ctx env pat
                val envA = Env.enterScope env
                val envP = bindPatternLocals envA pat
                val () =
                  case guard of
                    NONE => ()
                  | SOME g => resolveExpr ctx envP g
              in
                resolveExpr ctx envP body
              end)
           arms)
    | Ast.ExprWhile (cond, invs, body, _) =>
        ( resolveExpr ctx env cond
        ; app (resolveExpr ctx env) invs
        ; resolveExpr ctx env body)
    | Ast.ExprFor (pat, it, body, _) =>
        let
          val () = resolveExpr ctx env it
          val () = resolvePatShape ctx env pat
          val envL = Env.enterScope env
          val envP = bindPatternLocals envL pat
        in
          resolveExpr ctx envP body
        end
    | Ast.ExprLoop (body, _) => resolveExpr ctx env body
    | Ast.ExprReturn (eo, _) =>
        (case eo of NONE => () | SOME e1 => resolveExpr ctx env e1)
    | Ast.ExprBreak (eo, _) =>
        (case eo of NONE => () | SOME e1 => resolveExpr ctx env e1)
    | Ast.ExprContinue _ => ()
    | Ast.ExprAssign (a, b, _) =>
        (resolveExpr ctx env a; resolveExpr ctx env b)
    | Ast.ExprAssignOp (_, a, b, _) =>
        (resolveExpr ctx env a; resolveExpr ctx env b)
    | Ast.ExprCast (e1, t, _) =>
        (resolveExpr ctx env e1; resolveTy ctx env t)
    | Ast.ExprRange (a, b, _) =>
        (case a of NONE => () | SOME e1 => resolveExpr ctx env e1;
         case b of NONE => () | SOME e2 => resolveExpr ctx env e2)
    | Ast.ExprTry (e1, _) => resolveExpr ctx env e1
    | Ast.ExprAssert (e1, _) => resolveExpr ctx env e1
    | Ast.ExprStruct (path, fs, _) =>
        (if Env.lookupType env ctx path then ()
         else raise Fail ("E0301: unknown type `" ^ pathToString path ^ "`");
         app (fn (_, e1) => resolveExpr ctx env e1) fs)
    | Ast.ExprTuple (es, _) => app (resolveExpr ctx env) es
    | Ast.ExprArray (es, _) => app (resolveExpr ctx env) es

  and resolvePatShape (ctx : ctx) (env : Env.env) (p : Ast.pat) : unit =
    case p of
      Ast.PatWild _ => ()
    | Ast.PatBind _ => ()
    | Ast.PatLit _ => ()
    | Ast.PatTuple (ps, _) => app (resolvePatShape ctx env) ps
    | Ast.PatStruct (path, fs, _) =>
        (if length path >= 2 then
           if Env.lookupValue env (pathToString path) then ()
           else
             raise Fail
               ("E0300: unbound enum constructor in pattern `" ^
                pathToString path ^ "`")
         else if Env.lookupType env ctx path then ()
         else raise Fail ("E0301: unknown type `" ^ pathToString path ^ "` in pattern");
         app (fn (_, p2) => resolvePatShape ctx env p2) fs)
    | Ast.PatEnum (path, ps, _) =>
        (if Env.lookupValue env (pathToString path) then ()
         else
           raise Fail
             ("E0300: unbound enum constructor in pattern `" ^
              pathToString path ^ "`");
         app (resolvePatShape ctx env) ps)
    | Ast.PatOr _ =>
        raise Fail "E0306: | patterns are not supported in name resolution yet"

  and resolveStmt (ctx : ctx) (env : Env.env) (st : Ast.stmt) : Env.env =
    case st of
      Ast.StmtLet (pat, _, to, eo, _) =>
        let
          val () = (case to of NONE => () | SOME t => resolveTy ctx env t)
          val () = (case eo of NONE => () | SOME e1 => resolveExpr ctx env e1)
        in
          bindPatternLocals env pat
        end
    | Ast.StmtExpr (e1, _) => (resolveExpr ctx env e1; env)
    | Ast.StmtSemi (e1, _) => (resolveExpr ctx env e1; env)
    | Ast.StmtItem (inner, _) =>
        (resolveTopItem ctx env inner; env)

  and resolveContract (ctx : ctx) (env : Env.env) (c : Ast.contract) : unit =
    case c of
      Ast.Requires (e1, _) => resolveExpr ctx env e1
    | Ast.Ensures (e1, _) => resolveExpr ctx env e1
    | Ast.LoopInvariant (e1, _) => resolveExpr ctx env e1

  and resolveFn (ctx : ctx) (env : Env.env)
    (r : {name : Ast.ident, params : (Ast.pat * Ast.ty) list,
          ret : Ast.ty option, contracts : Ast.contract list,
          body : Ast.expr, span : Span.span}) : unit =
    let
      val () = app (fn (_, t) => resolveTy ctx env t) (#params r)
      val () =
        case #ret r of
          NONE => ()
        | SOME t => resolveTy ctx env t
      val env0 = Env.enterScope env
      val env1 =
        List.foldl (fn ((p, _), acc) => bindPatternLocals acc p) env0 (#params r)
      (* ensures/requires may reference `result` (checker types it as the fn return). *)
      val envContracts = Env.bindLocal env1 "result"
      val () = app (resolveContract ctx envContracts) (#contracts r)
    in
      resolveExpr ctx env1 (#body r)
    end

  and resolveStruct (ctx : ctx) (env : Env.env)
    (r : {name : Ast.ident, fields : (Ast.ident * Ast.ty) list,
          span : Span.span}) : unit =
    app (fn (_, t) => resolveTy ctx env t) (#fields r)

  and resolveEnum (ctx : ctx) (env : Env.env)
    (r : {name : Ast.ident, variants : Ast.variant list, span : Span.span}) : unit =
    let
      fun variant v =
        case v of
          Ast.VariantUnit _ => ()
        | Ast.VariantTuple (_, ts, _) => app (resolveTy ctx env) ts
        | Ast.VariantStruct (_, fs, _) => app (fn (_, t) => resolveTy ctx env t) fs
    in
      app variant (#variants r)
    end

  and resolveTrait (ctx : ctx) (env : Env.env)
    (r : {name : Ast.ident, methods : Ast.item list, span : Span.span}) : unit =
    let
      val ctxT = { allowSelf = true }
      fun one m =
        case m of
          Ast.ItemFn f => resolveFn ctxT env f
        | _ => ()
    in
      app one (#methods r)
    end

  and resolveImpl (ctx : ctx) (env : Env.env)
    (r : {trait_path : Ast.path option, self_ty : Ast.ty, items : Ast.item list,
          span : Span.span}) : unit =
    let
      val () = resolveTy ctx env (#self_ty r)
      val ctxI = { allowSelf = true }
      fun one m =
        case m of
          Ast.ItemFn f => resolveFn ctxI env f
        | _ => ()
    in
      app one (#items r)
    end

  and resolveTopItem (ctx : ctx) (env : Env.env) (it : Ast.item) : unit =
    case it of
      Ast.ItemFn r => resolveFn ctx env r
    | Ast.ItemStruct r => resolveStruct ctx env r
    | Ast.ItemEnum r => resolveEnum ctx env r
    | Ast.ItemTrait r => resolveTrait ctx env r
    | Ast.ItemImpl r => resolveImpl ctx env r
    | Ast.ItemTypeAlias (_, t, _) => resolveTy ctx env t
    | Ast.ItemUse _ => ()
    | Ast.ItemModule _ => ()

  fun variantStem (v : Ast.variant) : string =
    case v of
      Ast.VariantUnit (n, _) => n
    | Ast.VariantTuple (n, _, _) => n
    | Ast.VariantStruct (n, _, _) => n

  fun enumVariantReg (en : string) (v : Ast.variant) : string * int =
    case v of
      Ast.VariantUnit (n, _) => (pathToString [en, n], 0)
    | Ast.VariantTuple (n, tys, _) => (pathToString [en, n], length tys)
    | Ast.VariantStruct (n, fs, _) => (pathToString [en, n], length fs)

  fun registerItem (env : Env.env) (it : Ast.item) : Env.env =
    case it of
      Ast.ItemFn { name, params, ... } =>
        Env.registerFnArity (Env.registerModuleValue env name) name (length params)
    | Ast.ItemStruct { name, ... } => Env.registerModuleType env name
    | Ast.ItemEnum { name, variants, ... } =>
        let
          val e1 = Env.registerModuleType env name
        in
          List.foldl
            (fn (v, e) =>
               let val (q, ar) = enumVariantReg name v
               in Env.registerFnArity (Env.registerModuleValue e q) q ar end) e1
            variants
        end
    | Ast.ItemTrait { name, ... } => Env.registerModuleType env name
    | Ast.ItemTypeAlias (name, _, _) => Env.registerModuleType env name
    | Ast.ItemImpl _ => env
    | Ast.ItemUse _ => env
    | Ast.ItemModule _ => env

  fun applyUseClause (prog : Ast.program) (env : Env.env) (it : Ast.item) :
    Env.env =
    case it of
      Ast.ItemUse (path, span) =>
        (case path of
           [m, nm] =>
             let
               val tgt = m ^ "__" ^ nm
               fun asDiag msg =
                 raise Diagnostic.Diag (Diagnostic.error ("E0309", msg, span))
               fun findEnum () =
                 List.find
                   (fn Ast.ItemEnum {name, ...} => name = tgt | _ => false) prog
             in
               case Env.lookupFnArity env tgt of
                 SOME _ =>
                   ( recordedImportAliases := (nm, tgt) :: !recordedImportAliases
                   ; Env.registerValueAlias env nm tgt
                     handle Fail msg => asDiag msg)
               | NONE =>
                   if Env.lookupType env {allowSelf = false} [tgt] then
                     case findEnum () of
                       SOME (Ast.ItemEnum {variants, ...}) =>
                         let
                           val envT =
                             Env.registerTypeAlias env nm tgt
                               handle Fail msg => asDiag msg
                         in
                           List.foldl
                             (fn (v, e) =>
                                let
                                  val vn = variantStem v
                                  val from = pathToString [nm, vn]
                                  val to = pathToString [tgt, vn]
                                in
                                  ( recordedImportAliases := (from, to) ::
                                      !recordedImportAliases
                                  ; Env.registerValueAlias e from to
                                    handle Fail msg => asDiag msg)
                                end) envT variants
                         end
                     | NONE =>
                         Env.registerTypeAlias env nm tgt
                           handle Fail msg => asDiag msg
                   else
                     asDiag
                       ("unknown module item `" ^ pathToString path ^ "`")
             end
         | _ =>
             raise Diagnostic.Diag
               (Diagnostic.error
                  ( "E0309"
                  , "use expects `module_name::item_name` in this slice"
                  , span)))
    | _ => env

  fun withIntrinsics (e : Env.env) : Env.env =
    let
      val e1 =
        Env.registerFnArity (Env.registerModuleValue e "println") "println" 1
      val e2 = Env.registerFnArity (Env.registerModuleValue e1 "old") "old" 1
      val e3 = Env.registerFnArity (Env.registerModuleValue e2 "forall") "forall" 3
      val e4 = Env.registerFnArity (Env.registerModuleValue e3 "exists") "exists" 3
    in
      Env.registerFnArity (Env.registerModuleValue e4 "no_alias") "no_alias" 2
    end

  fun resolve (prog : Ast.program) : Ast.program =
    let
      val () = recordedImportAliases := []
      (* Intrinsics first so user `fn println` / `fn old` / … collides with E0302 (reserved). *)
      val env0 =
        List.foldl (fn (it, acc) => registerItem acc it) (withIntrinsics Env.empty) prog
      val env1 =
        List.foldl (fn (it, e) => applyUseClause prog e it) env0 prog
      val ctx0 : ctx = { allowSelf = false }
      val () = app (resolveTopItem ctx0 env1) prog
    in
      prog
    end
end

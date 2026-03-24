(* Lower a type-checked program to IR (one block per fn). *)

structure Lowering :> LOWERING = struct

  val tmpCtr = ref 0

  (* While lowering an ensures expression that may read `result`, map to this C slot. *)
  val ensuresResultSlot : string option ref = ref NONE

  val retValueSlot : string = "_sv0ret"

  fun freshTmp () =
    let val n = !tmpCtr
    in tmpCtr := n + 1; "_sv0t" ^ Int.toString n end

  fun astBinopToC (b : Ast.binop) : string =
    case b of
      Ast.Add => "+"
    | Ast.Sub => "-"
    | Ast.Mul => "*"
    | Ast.Div => "/"
    | Ast.Rem => "%"
    | Ast.BitAnd => "&"
    | Ast.BitOr => "|"
    | Ast.BitXor => "^"
    | Ast.Shl => "<<"
    | Ast.Shr => ">>"
    | Ast.And => "&&"
    | Ast.Or => "||"
    | Ast.Eq => "=="
    | Ast.Neq => "!="
    | Ast.Lt => "<"
    | Ast.Gt => ">"
    | Ast.Leq => "<="
    | Ast.Geq => ">="

  fun lowerLit (l : Ast.literal) : Ir.value =
    case l of
      Ast.IntLit i => Ir.VInt i
    | Ast.BoolLit b => Ir.VBool b
    | Ast.UnitLit => Ir.VUnit
    | _ => raise Fail "literal not supported in lowering slice"

  fun mentionsResult (e : Ast.expr) : bool =
    case e of
      Ast.ExprPath (["result"], _) => true
    | Ast.ExprBinop (_, l, r, _) =>
        mentionsResult l orelse mentionsResult r
    | Ast.ExprUnop (_, e1, _) => mentionsResult e1
    | Ast.ExprCall (f, args, _) =>
        mentionsResult f orelse List.exists mentionsResult args
    | Ast.ExprIf (c, th, el, _) =>
        mentionsResult c orelse mentionsResult th
        orelse (case el of SOME z => mentionsResult z | NONE => false)
    | _ => false

  fun unopToC (u : Ast.unop) : string =
    case u of
      Ast.Neg => "-"
    | Ast.Not => "!"
    | Ast.BitNot => "~"
    | _ => raise Fail "unop not supported in lowering slice"

  fun paramName ((p : Ast.pat, _) : Ast.pat * Ast.ty) : string =
    case p of
      Ast.PatBind (x, _) => x
    | _ => raise Fail "parameter must be PatBind in lowering slice"

  (* Instrs that compute e, ending with a simple value (for Binop/Unop leaves). *)
  and lowerExprToValue (e : Ast.expr) : Ir.instr list * Ir.value =
    case e of
      Ast.ExprBlock (stmts, opt, _) =>
        let val is = List.concat (map lowerStmt stmts)
        in case opt of
             SOME e2 =>
               let val (i2, v2) = lowerExprToValue e2 in (is @ i2, v2) end
           | NONE => (is, Ir.VUnit)
           end
    | Ast.ExprLit (l, _) => ([], lowerLit l)
    | Ast.ExprPath ([x], _) =>
        (case !ensuresResultSlot of
           SOME rs =>
             if x = "result" then ([], Ir.VVar rs) else ([], Ir.VVar x)
         | NONE => ([], Ir.VVar x))
    | Ast.ExprCall (Ast.ExprPath ([callee], _), args, _) =>
        let
          fun oneArg a =
            let val (ia, va) = lowerExprToValue a in (ia, va) end
          val pairs = map oneArg args
          val inss = List.concat (map #1 pairs)
          val vals = map #2 pairs
          val t = freshTmp ()
        in
          (inss @ [Ir.Call (SOME t, callee, vals)], Ir.VVar t)
        end
    | Ast.ExprBinop (b, l, r, _) =>
        let
          val (il, vl) = lowerExprToValue l
          val (ir, vr) = lowerExprToValue r
          val t = freshTmp ()
        in
          (il @ ir @ [Ir.Assign (t, Ir.Binop (astBinopToC b, vl, vr))], Ir.VVar t)
        end
    | Ast.ExprUnop (u, e1, _) =>
        let
          val (i, v) = lowerExprToValue e1
          val t = freshTmp ()
        in
          (i @ [Ir.Assign (t, Ir.Unop (unopToC u, v))], Ir.VVar t)
        end
    | Ast.ExprIf (c, th, SOME el, _) =>
        let
          val t = freshTmp ()
          val u = freshTmp ()
          val (ic, ec) = lowerExprWithInstrs c
          val thSt = lowerIntoVarInstrs th u
          val elSt = lowerIntoVarInstrs el u
        in
          ( ic @ [Ir.DeclVar t, Ir.DeclVar u, Ir.IfElse (ec, thSt, elSt),
                  Ir.Store (t, Ir.Load u)]
          , Ir.VVar t)
        end
    | Ast.ExprIf (c, th, NONE, _) =>
        let val (ic, ec) = lowerExprWithInstrs c
        in (ic @ [Ir.IfElse (ec, lowerExprForEffect th, [])], Ir.VUnit) end
    | _ =>
        let
          val (instrs, rhs) = lowerExprWithInstrs e
          val t = freshTmp ()
        in
          (instrs @ [Ir.Assign (t, rhs)], Ir.VVar t)
        end

  (* Build Ir.expr; may use lowerExprToValue for non-leaf operands. *)
  and lowerExprWithInstrs (e : Ast.expr) : Ir.instr list * Ir.expr =
    case e of
      Ast.ExprBlock (stmts, opt, _) =>
        let val is = List.concat (map lowerStmt stmts)
        in case opt of
             SOME e2 =>
               let val (i2, rhs) = lowerExprWithInstrs e2 in (is @ i2, rhs) end
           | NONE => (is, Ir.Literal Ir.VUnit)
           end
    | Ast.ExprLit (l, _) => ([], Ir.Literal (lowerLit l))
    | Ast.ExprPath ([x], _) =>
        (case !ensuresResultSlot of
           SOME rs =>
             if x = "result" then ([], Ir.Load rs) else ([], Ir.Load x)
         | NONE => ([], Ir.Load x))
    | Ast.ExprBinop (b, l, r, _) =>
        let
          val (il, vl) = lowerExprToValue l
          val (ir, vr) = lowerExprToValue r
        in
          (il @ ir, Ir.Binop (astBinopToC b, vl, vr))
        end
    | Ast.ExprUnop (u, e1, _) =>
        let
          val (i, v) = lowerExprToValue e1
        in
          (i, Ir.Unop (unopToC u, v))
        end
    | _ =>
        let
          val (instrs, v) = lowerExprToValue e
        in
          case v of
            Ir.VVar x => (instrs, Ir.Load x)
          | _ => (instrs, Ir.Literal v)
        end

  (* Assign into existing variable name t (outer scope already declares t if needed). *)
  and lowerIntoVarInstrs (e : Ast.expr) (t : string) : Ir.instr list =
    case e of
      Ast.ExprBlock (stmts, opt, _) =>
        let val is = List.concat (map lowerStmt stmts)
        in case opt of
             SOME e2 => is @ lowerIntoVarInstrs e2 t
           | NONE => is @ [Ir.Store (t, Ir.Literal Ir.VUnit)]
           end
    | Ast.ExprLit (l, _) => [Ir.Store (t, Ir.Literal (lowerLit l))]
    | Ast.ExprPath ([x], _) =>
        (case !ensuresResultSlot of
           SOME rs =>
             if x = "result" then [Ir.Store (t, Ir.Load rs)]
             else [Ir.Store (t, Ir.Load x)]
         | NONE => [Ir.Store (t, Ir.Load x)])
    | Ast.ExprIf (c, th, SOME el, _) =>
        let
          val u = freshTmp ()
          val (ic, ec) = lowerExprWithInstrs c
          val thSt = lowerIntoVarInstrs th u
          val elSt = lowerIntoVarInstrs el u
        in
          ic @ [Ir.DeclVar u, Ir.IfElse (ec, thSt, elSt), Ir.Store (t, Ir.Load u)]
        end
    | Ast.ExprIf (c, th, NONE, _) =>
        let val (ic, ec) = lowerExprWithInstrs c
        in
          ic
          @ [Ir.IfElse (ec, lowerIntoVarInstrs th t,
               [Ir.Store (t, Ir.Literal Ir.VUnit)])]
        end
    | _ =>
        let
          val (instrs, rhs) = lowerExprWithInstrs e
        in
          instrs @ [Ir.Store (t, rhs)]
        end

  (* Evaluate for side effects / discarded unit result (blocks, if without else). *)
  and lowerExprForEffect (e : Ast.expr) : Ir.instr list =
    case e of
      Ast.ExprBlock (stmts, opt, _) =>
        let val is = List.concat (map lowerStmt stmts)
        in case opt of NONE => is | SOME e2 => is @ lowerExprForEffect e2 end
    | Ast.ExprIf (c, th, NONE, _) =>
        let val (ic, ec) = lowerExprWithInstrs c
        in ic @ [Ir.IfElse (ec, lowerExprForEffect th, [])] end
    | Ast.ExprIf (c, th, SOME el, _) =>
        let val (ic, ec) = lowerExprWithInstrs c
        in ic @ [Ir.IfElse (ec, lowerExprForEffect th, lowerExprForEffect el)] end
    | _ =>
        let val (instrs, _) = lowerExprToValue e in instrs end

  and lowerStmt (st : Ast.stmt) : Ir.instr list =
    case st of
      Ast.StmtLet (Ast.PatBind (x, _), _, _, SOME init, _) =>
        let val (instrs, rhs) = lowerExprWithInstrs init
        in instrs @ [Ir.Assign (x, rhs)] end
    | Ast.StmtSemi (Ast.ExprReturn (SOME e, _), _) => lowerReturn e
    | Ast.StmtSemi (Ast.ExprReturn (NONE, _), _) => [Ir.Return NONE]
    | Ast.StmtExpr (Ast.ExprReturn (SOME e, _), _) => lowerReturn e
    | Ast.StmtExpr (Ast.ExprReturn (NONE, _), _) => [Ir.Return NONE]
    | Ast.StmtSemi (e, _) => lowerExprForEffect e
    | Ast.StmtExpr (e, _) => lowerExprForEffect e
    | _ => raise Fail "statement not supported in lowering slice"

  and lowerReturn (e : Ast.expr) : Ir.instr list =
    let val (instrs, v) = lowerExprToValue e
    in instrs @ [Ir.Return (SOME v)] end

  fun lowerBlock (stmts : Ast.stmt list, opt : Ast.expr option) : Ir.instr list =
    let
      val is = List.concat (map lowerStmt stmts)
      val tail =
        case opt of
          NONE => []
        | SOME e => lowerReturn e
    in
      is @ tail
    end

  fun lowerBody (body : Ast.expr) : Ir.instr list =
    case body of
      Ast.ExprBlock (stmts, opt, _) => lowerBlock (stmts, opt)
    | e => lowerReturn e

  fun retSyntaxIsUnit (r : {
        name : Ast.ident, params : (Ast.pat * Ast.ty) list,
        ret : Ast.ty option, contracts : Ast.contract list,
        body : Ast.expr, span : Span.span
      }) : bool =
    case #ret r of
      SOME (Ast.TyName (["unit"], _)) => true
    | SOME (Ast.TyUnit _) => true
    | _ => false

  fun storeValueToSlot (slot : string) (v : Ir.value) : Ir.instr list =
    case v of
      Ir.VVar x =>
        if x = slot then [] else [Ir.Store (slot, Ir.Load x)]
    | _ => [Ir.Store (slot, Ir.Literal v)]

  fun ensGlue (ensParts : (Ir.instr list * Ir.expr) list) (fnName : string) :
    Ir.instr list =
    List.concat (map (fn (pre, ec) => pre @ [Ir.Ensures (ec, fnName)]) ensParts)

  fun injectEnsuresAndRetSlot (fnName : string) (useRetSlot : bool)
    (ensParts : (Ir.instr list * Ir.expr) list) (is : Ir.instr list) : Ir.instr list =
    if null ensParts then
      is
    else
      let
        val eg = ensGlue ensParts fnName
        fun inj xs =
          List.concat (
            map (fn ins =>
              case ins of
                Ir.Return (SOME v) =>
                  if useRetSlot then
                    storeValueToSlot retValueSlot v @ eg
                    @ [Ir.Return (SOME (Ir.VVar retValueSlot))]
                  else
                    eg @ [Ir.Return (SOME v)]
              | Ir.Return NONE => eg @ [Ir.Return NONE]
              | Ir.IfElse (c, th, el) =>
                  [Ir.IfElse (c, inj th, inj el)]
              | x => [x]) xs)
      in
        inj is
      end

  fun lowerEnsuresClause (e : Ast.expr) (mapResult : bool) :
    Ir.instr list * Ir.expr =
    if mapResult then
      let
        val old = !ensuresResultSlot
      in
        ensuresResultSlot := SOME retValueSlot;
        let val r = lowerExprWithInstrs e
        in (ensuresResultSlot := old; r) end
      end
    else
      lowerExprWithInstrs e

  fun lowerFn (r : {
        name : Ast.ident, params : (Ast.pat * Ast.ty) list,
        ret : Ast.ty option, contracts : Ast.contract list,
        body : Ast.expr, span : Span.span
      }) : Ir.block =
    let
      val fnName = #name r
      val contracts = #contracts r
      val reqs =
        List.mapPartial
          (fn c =>
             case c of
               Ast.Requires (e, _) => SOME e
             | _ => NONE) contracts
      val enss =
        List.mapPartial
          (fn c =>
             case c of
               Ast.Ensures (e, _) => SOME e
             | _ => NONE) contracts
      val useRetSlot =
        not (retSyntaxIsUnit r) andalso List.exists mentionsResult enss
      val reqInstrs =
        List.concat (
          map (fn e =>
                 let val (pre, ec) = lowerExprWithInstrs e
                 in pre @ [Ir.Requires (ec, fnName)] end) reqs)
      val ensParts =
        map (fn e => lowerEnsuresClause e useRetSlot) enss
      val () = (tmpCtr := 0; ensuresResultSlot := NONE)
      val body0 = lowerBody (#body r)
      val body1 = injectEnsuresAndRetSlot fnName useRetSlot ensParts body0
      val decl = if useRetSlot then [Ir.DeclVar retValueSlot] else []
    in
      {
        label = fnName,
        params = map paramName (#params r),
        instrs = decl @ reqInstrs @ body1
      }
    end

  fun lower (prog : Ast.program) : Ir.program =
    let
      fun one it =
        case it of
          Ast.ItemFn r => SOME (lowerFn r)
        | _ => NONE
    in
      List.mapPartial one prog
    end
end

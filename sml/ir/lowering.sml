(* Lower a type-checked program to IR (one block per fn). *)

structure Lowering :> LOWERING = struct

  val tmpCtr = ref 0
  val ensuresResultSlot : string option ref = ref NONE
  val currentFnName : string ref = ref ""
  val retValueSlot : string = "_sv0ret"

  val theProg : Ast.program ref = ref []
  val structFieldOrder : (string * string list) list ref = ref []
  (* enum name -> (variant name -> tag), max payload ints *)
  val enumVariants : (string * ((string * int) list * int)) list ref = ref []

  val importAliases : (string * string) list ref = ref []

  fun setImportAliases (xs : (string * string) list) : unit = importAliases := xs

  val currentFnParams : (string * Ast.ty) list ref = ref []
  val scrutLocals : (string * string) list ref = ref []

  fun freshTmp () =
    let val n = !tmpCtr
    in tmpCtr := n + 1; "_sv0t" ^ Int.toString n end

  fun splitQName (s : string) : string list =
    let
      val sz = String.size s
      fun findDbl (i : int) : int option =
        if i + 1 >= sz then NONE
        else if String.sub (s, i) = #":" andalso String.sub (s, i + 1) = #":" then
          SOME i
        else findDbl (i + 1)
    in
      case findDbl 0 of
        NONE => [s]
      | SOME i =>
          String.substring (s, 0, i)
          :: splitQName (String.substring (s, i + 2, sz - (i + 2)))
    end

  fun resolveFnCallee (n : string) : string =
    case
      List.find (fn (a, _) => String.compare (a, n) = EQUAL) (!importAliases)
    of
      SOME (_, t) =>
        (case splitQName t of
           [_] => t
         | _ =>
             raise Fail ("lowering: import alias for `" ^ n ^ "` is not a function"))
    | NONE => n

  fun resolveEnumCtorPath (en : string) (vn : string) : string * string =
    let val k = en ^ "::" ^ vn
    in
      case
        List.find (fn (a, _) => String.compare (a, k) = EQUAL) (!importAliases)
      of
        SOME (_, t) =>
          (case splitQName t of
             [e2, v2] => (e2, v2)
           | _ => (en, vn))
      | NONE => (en, vn)
    end

  datatype assign_lhs = ALVar of string | ALField of string * string

  fun classifyAssignLhs (lhs : Ast.expr) : assign_lhs option =
    case lhs of
      Ast.ExprPath ([x], _) => SOME (ALVar x)
    | Ast.ExprField (Ast.ExprPath ([b], _), fld, _) => SOME (ALField (b, fld))
    | _ => NONE

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

  fun astTyToCString (t : Ast.ty) : string =
    case t of
      Ast.TyUnit _ => "void"
    | Ast.TyName (["unit"], _) => "void"
    | Ast.TyName (["i32"], _) => "int"
    | Ast.TyName (["bool"], _) => "int"
    | Ast.TyName (["i8"], _) => "int8_t"
    | Ast.TyName (["u8"], _) => "uint8_t"
    | Ast.TyName (["i16"], _) => "int16_t"
    | Ast.TyName (["u16"], _) => "uint16_t"
    | Ast.TyName (["i64"], _) => "int64_t"
    | Ast.TyName (["u64"], _) => "uint64_t"
    | Ast.TyName (["isize"], _) => "intptr_t"
    | Ast.TyName (["usize"], _) => "uintptr_t"
    | Ast.TyName ([n], _) => n
    | _ => "int"

  fun calleeRetCty (name : string) : string =
    let val name0 = resolveFnCallee name
    in
      case
        List.find
          (fn it =>
             case it of Ast.ItemFn r => #name r = name0 | _ => false) (!theProg)
      of
        SOME (Ast.ItemFn r) =>
          (case #ret r of SOME at => astTyToCString at | NONE => "int")
      | _ => "int"
    end

  fun exprInitCty (e : Ast.expr) : string =
    case e of
      Ast.ExprStruct (n :: _, _, _) => n
    | Ast.ExprPath ([en, vn], _) => #1 (resolveEnumCtorPath en vn)
    | Ast.ExprCall (Ast.ExprPath ([f], _), _, _) => calleeRetCty f
    | Ast.ExprCall (Ast.ExprPath ([en, vn], _), _, _) =>
        #1 (resolveEnumCtorPath en vn)
    | Ast.ExprTuple ([e1], _) => exprInitCty e1
    | Ast.ExprLit (Ast.BoolLit _, _) => "int"
    | Ast.ExprLit (Ast.IntLit _, _) => "int"
    | _ => "int"

  fun matchScrutCty (e : Ast.expr) : string =
    case e of
      Ast.ExprPath ([en, vn], _) => #1 (resolveEnumCtorPath en vn)
    | Ast.ExprPath ([x], _) =>
        (case List.find (fn (v, _) => v = x) (!scrutLocals) of
           SOME (_, ct) => ct
         | NONE =>
             (case List.find (fn (n, _) => n = x) (!currentFnParams) of
                SOME (_, aty) => astTyToCString aty
              | NONE => "int"))
    | Ast.ExprCall (Ast.ExprPath ([f], _), _, _) => calleeRetCty f
    | Ast.ExprTuple ([e1], _) => matchScrutCty e1
    | Ast.ExprLit (Ast.BoolLit _, _) => "int"
    | Ast.ExprLit (Ast.IntLit _, _) => "int"
    | _ => "int"

  fun structFields (sn : string) : string list =
    case List.find (fn (n, _) => n = sn) (!structFieldOrder) of
      SOME (_, fs) => fs
    | NONE => raise Fail ("lowering: unknown struct `" ^ sn ^ "`")

  fun enumTag (en : string) (vn : string) : int =
    case List.find (fn (n, _) => n = en) (!enumVariants) of
      SOME (_, (tags, _)) =>
        (case List.find (fn (v, _) => v = vn) tags of
           SOME (_, k) => k
         | NONE => raise Fail ("lowering: unknown enum variant `" ^ en ^ "::" ^ vn ^ "`"))
    | NONE => raise Fail ("lowering: unknown enum `" ^ en ^ "`")

  fun valueToExpr (v : Ir.value) : Ir.expr =
    case v of
      Ir.VVar xv => Ir.Load xv
    | _ => Ir.Literal v

  fun enumStorePayload (dst : string) (vals : Ir.value list) : Ir.instr list =
    let
      fun pName i = "p" ^ Int.toString i
      fun one (i, v) = Ir.StoreField (dst, pName i, valueToExpr v)
    in
      List.tabulate (length vals, fn i => one (i, List.nth (vals, i)))
    end

  fun findVariantAst (en : string) (vn : string) : Ast.variant =
    case List.find (fn it => case it of Ast.ItemEnum r => #name r = en | _ => false)
      (!theProg) of
      SOME (Ast.ItemEnum {variants, ...}) =>
        (case List.find (fn v =>
               let val n =
                 case v of
                   Ast.VariantUnit (nm, _) => nm
                 | Ast.VariantTuple (nm, _, _) => nm
                 | Ast.VariantStruct (nm, _, _) => nm
               in n = vn end) variants of
           SOME v => v
         | NONE => raise Fail ("lowering: variant not in AST `" ^ en ^ "::" ^ vn ^ "`"))
    | _ => raise Fail ("lowering: enum not in AST `" ^ en ^ "`")

  fun lowerLit (l : Ast.literal) : Ir.value =
    case l of
      Ast.IntLit i => Ir.VInt i
    | Ast.BoolLit b => Ir.VBool b
    | Ast.UnitLit => Ir.VUnit
    | Ast.StringLit s => Ir.VString s
    | _ => raise Fail "literal not supported in lowering slice"

  fun okPayloadCty (en : string) : string =
    case findVariantAst en "Ok" of
      Ast.VariantTuple (_, [aty], _) => astTyToCString aty
    | _ =>
        raise Fail "lowering: `?` requires Ok(T) with exactly one payload field"

  fun scanLets (stmts : Ast.stmt list) : (string * string) list =
    List.concat (
      map (fn st =>
        case st of
          Ast.StmtLet (Ast.PatBind (x, _), _, to, SOME init, _) =>
            let
              val cty =
                case to of
                  SOME at => astTyToCString at
                | NONE => exprInitCty init
            in
              [(x, cty)]
            end
        | _ => []) stmts)

  fun withScrutLocals (extra : (string * string) list) (thunk : unit -> 'a) : 'a =
    let
      val old = !scrutLocals
      val () = scrutLocals := extra @ old
      val res = thunk () handle ex => (scrutLocals := old; raise ex)
      val () = scrutLocals := old
    in
      res
    end

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
    | Ast.ExprWhile (c, invs, b, _) =>
        mentionsResult c orelse List.exists mentionsResult invs
        orelse mentionsResult b
    | Ast.ExprFor (_, it, b, _) =>
        mentionsResult it orelse mentionsResult b
    | Ast.ExprLoop (b, _) => mentionsResult b
    | Ast.ExprMatch (scr, arms, _) =>
        mentionsResult scr
        orelse List.exists
          (fn Ast.Arm (_, g, bd) =>
             (case g of SOME ge => mentionsResult ge | NONE => false)
             orelse mentionsResult bd) arms
    | Ast.ExprField (e1, _, _) => mentionsResult e1
    | Ast.ExprStruct (_, fs, _) =>
        List.exists (fn (_, fe) => mentionsResult fe) fs
    | Ast.ExprTuple (es, _) => List.exists mentionsResult es
    | Ast.ExprBreak (NONE, _) => false
    | Ast.ExprBreak (SOME e, _) => mentionsResult e
    | Ast.ExprContinue _ => false
    | Ast.ExprCast (e1, _, _) => mentionsResult e1
    | Ast.ExprTry (e1, _) => mentionsResult e1
    | _ => false

  fun unopToC (u : Ast.unop) : string =
    case u of
      Ast.Neg => "-"
    | Ast.Not => "!"
    | Ast.BitNot => "~"
    | Ast.Borrow => "&"
    | Ast.BorrowMut => "&"
    | _ =>
        raise Fail
          "E0540: lowering: unary operator not supported (use `&` only in contracts)"

  fun oldSlot (x : string) : string = "_sv0old_" ^ x

  (* Parameter names referenced by old(x) in a contract expression. *)
  fun allOldNames (e : Ast.expr) : string list =
    let
      fun go ex =
        case ex of
          Ast.ExprCall (Ast.ExprPath (["old"], _), [Ast.ExprPath ([x], _)], _) => [x]
        | Ast.ExprLit _ => []
        | Ast.ExprPath _ => []
        | Ast.ExprBinop (_, l, r, _) => go l @ go r
        | Ast.ExprUnop (_, e1, _) => go e1
        | Ast.ExprCall (f, args, _) => go f @ List.concat (map go args)
        | Ast.ExprIf (c, th, el, _) =>
            go c @ go th @ (case el of SOME z => go z | NONE => [])
        | Ast.ExprTuple (es, _) => List.concat (map go es)
        | Ast.ExprRange (a, b, _) =>
            (case a of SOME z => go z | NONE => [])
            @ (case b of SOME z => go z | NONE => [])
        | Ast.ExprField (e1, _, _) => go e1
        | Ast.ExprStruct (_, fs, _) =>
            List.concat (map (fn (_, fe) => go fe) fs)
        | Ast.ExprWhile (c, invs, b, _) =>
            go c @ List.concat (map go invs) @ go b
        | Ast.ExprFor (_, it, b, _) => go it @ go b
        | Ast.ExprLoop (b, _) => go b
        | Ast.ExprMatch (scr, arms, _) =>
            go scr
            @ List.concat (
                map (fn Ast.Arm (_, g, bd) =>
                  (case g of SOME ge => go ge | NONE => []) @ go bd) arms)
        | Ast.ExprCast (e1, _, _) => go e1
        | Ast.ExprTry (e1, _) => go e1
        | Ast.ExprBlock (stmts, opt, _) =>
            List.concat (map goStmt stmts)
            @ (case opt of SOME z => go z | NONE => [])
        | _ => []
      and goStmt st =
        case st of
          Ast.StmtLet (_, _, _, NONE, _) => []
        | Ast.StmtLet (_, _, _, SOME ini, _) => go ini
        | Ast.StmtSemi (e, _) => go e
        | Ast.StmtExpr (e, _) => go e
        | Ast.StmtItem _ => []
    in
      go e
    end

  fun paramName ((p : Ast.pat, _) : Ast.pat * Ast.ty) : string =
    case p of
      Ast.PatBind (x, _) => x
    | _ => raise Fail "parameter must be PatBind in lowering slice"

  fun matchArmSetup (pat : Ast.pat) (scrVar : string) (scrCty : string) :
    Ir.instr list * Ir.expr * Ir.instr list =
    case pat of
      Ast.PatWild _ =>
        ([], Ir.Literal (Ir.VBool true), [])
    | Ast.PatBind (x, _) =>
        let
          val bindPre =
            if scrCty = "int" then [Ir.Assign (x, Ir.Load scrVar)]
            else [Ir.DeclNamed (scrCty, x), Ir.Store (x, Ir.Load scrVar)]
        in
          ([], Ir.Literal (Ir.VBool true), bindPre)
        end
    | Ast.PatLit (Ast.IntLit i, _) =>
        ( [],
          Ir.Binop ("==", Ir.VVar scrVar, Ir.VInt i),
          [] )
    | Ast.PatLit (Ast.BoolLit true, _) =>
        ( [],
          Ir.Binop ("!=", Ir.VVar scrVar, Ir.VInt 0),
          [] )
    | Ast.PatLit (Ast.BoolLit false, _) =>
        ( [],
          Ir.Binop ("==", Ir.VVar scrVar, Ir.VInt 0),
          [] )
    | Ast.PatEnum (path, inners, _) =>
        (case path of
           [en, vn] =>
             let
               val k = enumTag en vn
               val cond =
                 Ir.Binop
                   ( "=="
                   , Ir.VMember (Ir.VVar scrVar, "tag")
                   , Ir.VInt (IntInf.fromInt k))
               val va = findVariantAst en vn
               val bindPre =
                 case va of
                   Ast.VariantUnit _ => []
                 | Ast.VariantTuple _ =>
                     let
                       fun bindOne (i, p) =
                         case p of
                           Ast.PatWild _ => []
                         | Ast.PatBind (xb, _) =>
                             [ Ir.Assign
                                 ( xb
                                 , Ir.Literal
                                     (Ir.VMember (Ir.VVar scrVar, "p" ^ Int.toString i))
                                 ) ]
                         | _ =>
                             raise Fail
                               "lowering: enum tuple arm pattern not supported"
                       val pairs = ListPair.zip (List.tabulate (length inners, fn i => i), inners)
                     in
                       List.concat (map bindOne pairs)
                     end
                 | Ast.VariantStruct _ =>
                     raise Fail
                       "lowering: struct enum variants in match not supported yet"
             in
               ([], cond, bindPre)
             end
         | _ => raise Fail "lowering: bad PatEnum path")
    | _ => raise Fail "lowering: match pattern not supported"

  fun lowerExprToValue (e : Ast.expr) : Ir.instr list * Ir.value =
    case e of
      Ast.ExprBlock (stmts, opt, _) =>
        withScrutLocals (scanLets stmts) (fn () =>
          let val is = List.concat (map lowerStmt stmts)
          in case opt of
               SOME e2 =>
                 let val (i2, v2) = lowerExprToValue e2 in (is @ i2, v2) end
             | NONE => (is, Ir.VUnit) end)
    | Ast.ExprLit (l, _) => ([], lowerLit l)
    | Ast.ExprTuple ([e1], _) => lowerExprToValue e1
    | Ast.ExprPath ([x], _) =>
        (case !ensuresResultSlot of
           SOME rs =>
             if x = "result" then ([], Ir.VVar rs) else ([], Ir.VVar x)
         | NONE => ([], Ir.VVar x))
    | Ast.ExprPath ([en, vn], _) =>
        let
          val (en0, vn0) = resolveEnumCtorPath en vn
          val t = freshTmp ()
        in
          ( [Ir.DeclNamed (en0, t)] @ enumConstructInstrs t en0 vn0 []
          , Ir.VVar t)
        end
    | Ast.ExprField (e1, f, _) =>
        let val (i1, v1) = lowerExprToValue e1 in (i1, Ir.VMember (v1, f)) end
    | Ast.ExprStruct (path, fields, _) =>
        let
          val sn = hd path
          val order = structFields sn
          val t = freshTmp ()
          fun findField n =
            case List.find (fn (nm, _) => nm = n) fields of
              SOME (_, ex) => ex
            | NONE => raise Fail ("lowering: missing struct field `" ^ n ^ "`")
          fun oneField fname =
            let val (pi, ei) = lowerExprWithInstrs (findField fname)
            in pi @ [Ir.StoreField (t, fname, ei)] end
        in
          ([Ir.DeclNamed (sn, t)] @ List.concat (map oneField order), Ir.VVar t)
        end
    | Ast.ExprCall (Ast.ExprPath ([en, vn], _), args, _) =>
        let
          val (en0, vn0) = resolveEnumCtorPath en vn
          val t = freshTmp ()
        in
          ( [Ir.DeclNamed (en0, t)] @ enumConstructInstrs t en0 vn0 args
          , Ir.VVar t)
        end
    | Ast.ExprCall (Ast.ExprPath (["println"], _), [Ast.ExprLit (Ast.StringLit s, _)], _) =>
        ([Ir.Call (NONE, "sv0_println", [Ir.VString s], "void")], Ir.VUnit)
    | Ast.ExprCall (Ast.ExprPath (["old"], _), [Ast.ExprPath ([x], _)], _) =>
        ([], Ir.VVar (oldSlot x))
    | Ast.ExprCall (Ast.ExprPath (["no_alias"], _), [a, b], _) =>
        let
          val (ia, va) = lowerExprToValue a
          val (ib, vb) = lowerExprToValue b
          val t = freshTmp ()
        in
          ( ia @ ib @ [Ir.Call (SOME t, "sv0_no_alias", [va, vb], "bool")]
          , Ir.VVar t)
        end
    | Ast.ExprUnop (Ast.Borrow, Ast.ExprPath ([x], _), _) =>
        ([], Ir.VAddrOf x)
    | Ast.ExprUnop (Ast.BorrowMut, Ast.ExprPath ([x], _), _) =>
        ([], Ir.VAddrOf x)
    | Ast.ExprCall (Ast.ExprPath ([callee], _), args, _) =>
        let
          fun oneArg a =
            let val (ia, va) = lowerExprToValue a in (ia, va) end
          val pairs = map oneArg args
          val inss = List.concat (map #1 pairs)
          val vals = map #2 pairs
          val t = freshTmp ()
          val callee0 = resolveFnCallee callee
          val rty = calleeRetCty callee
        in
          (inss @ [Ir.Call (SOME t, callee0, vals, rty)], Ir.VVar t)
        end
    | Ast.ExprMatch (scrut, arms, _) =>
        let
          val sct = matchScrutCty scrut
          val (isS, scrVar) = bindScrutVar scrut sct
          val out = freshTmp ()
          val isA = lowerMatchArms out scrVar sct arms
        in
          (isS @ [Ir.DeclVar out] @ isA, Ir.VVar out)
        end
    | Ast.ExprBinop (b, l, r, _) =>
        let
          val (il, vl) = lowerExprToValue l
          val (ir, vr) = lowerExprToValue r
          val t = freshTmp ()
        in
          (il @ ir @ [Ir.Assign (t, Ir.Binop (astBinopToC b, vl, vr))], Ir.VVar t)
        end
    | Ast.ExprUnop (Ast.Borrow, _, _) =>
        raise Fail "E0541: lowering: `&` expects a simple name (checker should reject this)"
    | Ast.ExprUnop (Ast.BorrowMut, _, _) =>
        raise Fail "E0541: lowering: `&mut` expects a simple name (checker should reject this)"
    | Ast.ExprUnop (u, e1, _) =>
        let
          val (i, v) = lowerExprToValue e1
          val t = freshTmp ()
        in
          (i @ [Ir.Assign (t, Ir.Unop (unopToC u, v))], Ir.VVar t)
        end
    | Ast.ExprCast (e1, tgtAst, _) =>
        let
          val cty = astTyToCString tgtAst
          val castOp = "(" ^ cty ^ ")"
          val (i1, v1) = lowerExprToValue e1
          val t = freshTmp ()
        in
          if cty = "int" then
            (i1 @ [Ir.Assign (t, Ir.Unop (castOp, v1))], Ir.VVar t)
          else
            ( i1 @ [Ir.DeclNamed (cty, t), Ir.Store (t, Ir.Unop (castOp, v1))]
            , Ir.VVar t)
        end
    | Ast.ExprTry (e1, _) =>
        let
          val en = matchScrutCty e1
          val () =
            if en = "int" orelse en = "void" then
              raise Fail "lowering: `?` expected enum-typed expression"
            else
              ()
          val errK = IntInf.fromInt (enumTag en "Err")
          val pCty = okPayloadCty en
          val (i1, ee) = lowerExprWithInstrs e1
          val u = freshTmp ()
          val out = freshTmp ()
          val declOut =
            if pCty = "int" then [Ir.DeclVar out] else [Ir.DeclNamed (pCty, out)]
          val cond =
            Ir.Binop ("==", Ir.VMember (Ir.VVar u, "tag"), Ir.VInt errK)
          val retErr = [Ir.Return (SOME (Ir.VVar u))]
          val takeOk = [Ir.Store (out, Ir.FieldAccess (Ir.VVar u, "p0"))]
        in
          (* Declare payload slot before if/else so it is in scope after `?`. *)
          ( i1 @ [Ir.DeclNamed (en, u), Ir.Store (u, ee)] @ declOut
            @ [Ir.IfElse (cond, retErr, takeOk)]
          , Ir.VVar out)
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
    | Ast.ExprWhile (cond, invs, body, span) =>
        (lowerExprForEffect (Ast.ExprWhile (cond, invs, body, span)), Ir.VUnit)
    | Ast.ExprFor (p, it, body, span) =>
        (lowerExprForEffect (Ast.ExprFor (p, it, body, span)), Ir.VUnit)
    | Ast.ExprLoop (body, span) =>
        (lowerExprForEffect (Ast.ExprLoop (body, span)), Ir.VUnit)
    | Ast.ExprBreak _ =>
        raise Fail "E0402: break in value position in lowering slice"
    | Ast.ExprContinue _ =>
        raise Fail "E0402: continue in value position in lowering slice"
    | Ast.ExprAssign (lhs, rhs, sp) =>
        (lowerExprForEffect (Ast.ExprAssign (lhs, rhs, sp)), Ir.VUnit)
    | Ast.ExprAssignOp (b, lhs, rhs, sp) =>
        (lowerExprForEffect (Ast.ExprAssignOp (b, lhs, rhs, sp)), Ir.VUnit)
    | _ =>
        let
          val (instrs, rhs) = lowerExprWithInstrs e
          val t = freshTmp ()
        in
          (instrs @ [Ir.Assign (t, rhs)], Ir.VVar t)
        end

  and lowerExprWithInstrs (e : Ast.expr) : Ir.instr list * Ir.expr =
    case e of
      Ast.ExprBlock (stmts, opt, _) =>
        withScrutLocals (scanLets stmts) (fn () =>
          let val is = List.concat (map lowerStmt stmts)
          in case opt of
               SOME e2 =>
                 let val (i2, rhs) = lowerExprWithInstrs e2 in (is @ i2, rhs) end
             | NONE => (is, Ir.Literal Ir.VUnit) end)
    | Ast.ExprLit (l, _) => ([], Ir.Literal (lowerLit l))
    | Ast.ExprTuple ([e1], _) => lowerExprWithInstrs e1
    | Ast.ExprCall (Ast.ExprPath (["forall"], _), [Ast.ExprPath ([iN], _), Ast.ExprRange (SOME lo, SOME hi, _), bod], _) =>
        lowerQuant true iN lo hi bod
    | Ast.ExprCall (Ast.ExprPath (["exists"], _), [Ast.ExprPath ([iN], _), Ast.ExprRange (SOME lo, SOME hi, _), bod], _) =>
        lowerQuant false iN lo hi bod
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
        let val (i, v) = lowerExprToValue e1 in (i, Ir.Unop (unopToC u, v)) end
    | _ =>
        let val (instrs, v) = lowerExprToValue e
        in case v of Ir.VVar x => (instrs, Ir.Load x) | _ => (instrs, Ir.Literal v) end

  and lowerQuant (isForall : bool) (iName : string) (lo : Ast.expr) (hi : Ast.expr)
    (body : Ast.expr) : Ir.instr list * Ir.expr =
    let
      val hiT = freshTmp ()
      val cntT = freshTmp ()
      val accT = freshTmp ()
      val (iHi, eHi) = lowerExprWithInstrs hi
      val (iLo, eLo) = lowerExprWithInstrs lo
      val initPart =
        iHi @ [Ir.Assign (hiT, eHi)] @ iLo @ [Ir.Assign (cntT, eLo)]
      val initAcc =
        if isForall then Ir.Assign (accT, Ir.Literal (Ir.VBool true))
        else Ir.Assign (accT, Ir.Literal (Ir.VBool false))
      val cond = Ir.Binop ("<", Ir.VVar cntT, Ir.VVar hiT)
      val pT = freshTmp ()
      val (preB, ep) = lowerExprWithInstrs body
      val predBlock = preB @ [Ir.DeclVar pT, Ir.Store (pT, ep)]
      val br =
        if isForall then
          Ir.IfElse
            ( Ir.Binop ("==", Ir.VVar pT, Ir.VInt 0)
            , [Ir.Store (accT, Ir.Literal (Ir.VBool false)), Ir.Break]
            , [])
        else
          Ir.IfElse
            ( Ir.Binop ("!=", Ir.VVar pT, Ir.VInt 0)
            , [Ir.Store (accT, Ir.Literal (Ir.VBool true)), Ir.Break]
            , [])
      val inner = Ir.Block (Ir.Assign (iName, Ir.Load cntT) :: predBlock @ [br])
      val incr = Ir.Store (cntT, Ir.Binop ("+", Ir.VVar cntT, Ir.VInt 1))
    in
      (initPart @ [initAcc, Ir.While (cond, [inner, incr])], Ir.Load accT)
    end

  and lowerIntoVarInstrs (e : Ast.expr) (t : string) : Ir.instr list =
    case e of
      Ast.ExprBlock (stmts, opt, _) =>
        withScrutLocals (scanLets stmts) (fn () =>
          let val is = List.concat (map lowerStmt stmts)
          in case opt of
               SOME e2 => is @ lowerIntoVarInstrs e2 t
             | NONE => is @ [Ir.Store (t, Ir.Literal Ir.VUnit)] end)
    | Ast.ExprLit (l, _) => [Ir.Store (t, Ir.Literal (lowerLit l))]
    | Ast.ExprTuple ([e1], _) => lowerIntoVarInstrs e1 t
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
        let val (instrs, rhs) = lowerExprWithInstrs e
        in instrs @ [Ir.Store (t, rhs)] end

  and lowerExprForEffect (e : Ast.expr) : Ir.instr list =
    case e of
      Ast.ExprBlock (stmts, opt, _) =>
        withScrutLocals (scanLets stmts) (fn () =>
          let val is = List.concat (map lowerStmt stmts)
          in case opt of NONE => is | SOME e2 => is @ lowerExprForEffect e2 end)
    | Ast.ExprIf (c, th, NONE, _) =>
        let val (ic, ec) = lowerExprWithInstrs c
        in ic @ [Ir.IfElse (ec, lowerExprForEffect th, [])] end
    | Ast.ExprIf (c, th, SOME el, _) =>
        let val (ic, ec) = lowerExprWithInstrs c
        in ic @ [Ir.IfElse (ec, lowerExprForEffect th, lowerExprForEffect el)] end
    | Ast.ExprWhile (cond, invs, body, _) =>
        let
          val (ic, ec) = lowerExprWithInstrs cond
          val invInstrs =
            List.concat (
              map (fn inv =>
                let val (pre, ie) = lowerExprWithInstrs inv
                in pre @ [Ir.Requires (ie, !currentFnName)] end) invs)
          val bi = lowerExprForEffect body
        in
          ic @ [Ir.While (ec, invInstrs @ bi)]
        end
    | Ast.ExprFor (Ast.PatBind (x, _), Ast.ExprRange (SOME lo, SOME hi, _), body, _) =>
        let
          val hiT = freshTmp ()
          val iT = freshTmp ()
          val (iHi, eHi) = lowerExprWithInstrs hi
          val (iLo, eLo) = lowerExprWithInstrs lo
          val initParts =
            iHi @ [Ir.Assign (hiT, eHi)] @ iLo @ [Ir.Assign (iT, eLo)]
          val cond = Ir.Binop ("<", Ir.VVar iT, Ir.VVar hiT)
          val iterBody =
            [ Ir.Block (Ir.Assign (x, Ir.Load iT) :: lowerExprForEffect body)
            , Ir.Store (iT, Ir.Binop ("+", Ir.VVar iT, Ir.VInt 1))]
        in
          initParts @ [Ir.While (cond, iterBody)]
        end
    | Ast.ExprFor _ =>
        raise Fail "for-loop lowering: expected PatBind and lo..hi range"
    | Ast.ExprLoop (body, _) =>
        [Ir.While (Ir.Literal (Ir.VBool true), lowerExprForEffect body)]
    | Ast.ExprBreak (NONE, _) => [Ir.Break]
    | Ast.ExprBreak (SOME _, _) =>
        raise Fail "break with value not supported in lowering slice"
    | Ast.ExprContinue _ => [Ir.Continue]
    | Ast.ExprAssign (lhs, rhs, _) =>
        (case classifyAssignLhs lhs of
           SOME (ALVar x) =>
             let val (ir, vr) = lowerExprWithInstrs rhs
             in ir @ [Ir.Store (x, vr)] end
         | SOME (ALField (b, f)) =>
             let val (ir, vr) = lowerExprWithInstrs rhs
             in ir @ [Ir.StoreField (b, f, vr)] end
         | NONE =>
             raise Fail "lowering: assignment lhs must be a name or one-level field")
    | Ast.ExprAssignOp (b, lhs, rhs, _) =>
        let
          val opStr = astBinopToC b
          val (irR, vR) = lowerExprToValue rhs
        in
          case classifyAssignLhs lhs of
            SOME (ALVar x) =>
              irR @ [Ir.Store (x, Ir.Binop (opStr, Ir.VVar x, vR))]
          | SOME (ALField (base, fld)) =>
              irR
              @ [ Ir.StoreField
                    ( base
                    , fld
                    , Ir.Binop (opStr, Ir.VMember (Ir.VVar base, fld), vR)
                    )
                ]
          | NONE =>
              raise Fail "lowering: compound-assign lhs must be a name or one-level field"
        end
    | _ =>
        let val (instrs, _) = lowerExprToValue e in instrs end

  and lowerStmt (st : Ast.stmt) : Ir.instr list =
    case st of
      Ast.StmtLet (Ast.PatBind (x, _), _, to, SOME init, _) =>
        let
          val cty =
            case to of
              SOME at => astTyToCString at
            | NONE => exprInitCty init
          val () = scrutLocals := (x, cty) :: !scrutLocals
        in
          case init of
            Ast.ExprStruct (path, fields, _) =>
              let
                val sn = hd path
                val order = structFields sn
                fun findField n =
                  case List.find (fn (nm, _) => nm = n) fields of
                    SOME (_, ex) => ex
                  | NONE => raise Fail ("lowering: missing struct field `" ^ n ^ "`")
                fun oneField fname =
                  let val (pi, ei) = lowerExprWithInstrs (findField fname)
                  in pi @ [Ir.StoreField (x, fname, ei)] end
              in
                [Ir.DeclNamed (sn, x)] @ List.concat (map oneField order)
              end
          | Ast.ExprCall (Ast.ExprPath ([en, vn], _), args, _) =>
              let val (en0, vn0) = resolveEnumCtorPath en vn
              in [Ir.DeclNamed (en0, x)] @ enumConstructInstrs x en0 vn0 args end
          | Ast.ExprPath ([en, vn], _) =>
              let val (en0, vn0) = resolveEnumCtorPath en vn
              in [Ir.DeclNamed (en0, x)] @ enumConstructInstrs x en0 vn0 [] end
          | _ =>
              let
                val (instrs, rhs) = lowerExprWithInstrs init
                val cty =
                  case to of
                    SOME at => astTyToCString at
                  | NONE => exprInitCty init
              in
                if cty = "int" then instrs @ [Ir.Assign (x, rhs)]
                else instrs @ [Ir.DeclNamed (cty, x), Ir.Store (x, rhs)]
              end
        end
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

  and lowerMatchArms (out : string) (scrVar : string) (scrCty : string)
    (arms : Ast.arm list) : Ir.instr list =
    case arms of
      [] => raise Fail "lowering: non-exhaustive match"
    | Ast.Arm (pat, guard, body) :: rest =>
        let
          val isLast = null rest
          val next = if isLast then [] else lowerMatchArms out scrVar scrCty rest
          val (cpre, cexp, bindPre) = matchArmSetup pat scrVar scrCty
          val bodyIs = bindPre @ lowerIntoVarInstrs body out
          val th =
            case guard of
              NONE => bodyIs
            | SOME g =>
                let val (gi, ge) = lowerExprWithInstrs g
                in gi @ [Ir.IfElse (ge, bodyIs, next)] end
        in
          cpre @ [Ir.IfElse (cexp, th, next)]
        end

  and bindScrutVar (scrut : Ast.expr) (cty : string) : Ir.instr list * string =
    let val (is, v) = lowerExprToValue scrut
    in
      case v of
        Ir.VVar x => (is, x)
      | Ir.VInt _ =>
          let val t = freshTmp ()
          in (is @ [Ir.Assign (t, Ir.Literal v)], t) end
      | Ir.VBool _ =>
          let val t = freshTmp ()
          in (is @ [Ir.Assign (t, Ir.Literal v)], t) end
      | _ =>
          let
            val t = freshTmp ()
            val (ii, ee) = lowerExprWithInstrs scrut
          in
            if cty = "int" then (is @ ii @ [Ir.Assign (t, ee)], t)
            else (is @ ii @ [Ir.DeclNamed (cty, t), Ir.Store (t, ee)], t)
          end
    end

  and enumConstructInstrs (dst : string) (en : string) (vn : string)
    (args : Ast.expr list) : Ir.instr list =
    let
      fun oneArg a =
        let val (ia, va) = lowerExprToValue a in (ia, va) end
      val pairs = map oneArg args
      val inss = List.concat (map #1 pairs)
      val vals = map #2 pairs
      val k = enumTag en vn
    in
      inss
      @ [Ir.StoreField (dst, "tag", Ir.Literal (Ir.VInt (IntInf.fromInt k)))]
      @ enumStorePayload dst vals
    end

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
              | Ir.While (c, th) => [Ir.While (c, inj th)]
              | Ir.Block th => [Ir.Block (inj th)]
              | Ir.Break => [Ir.Break]
              | Ir.Continue => [Ir.Continue]
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

  fun fnRetCty (r : {
        name : Ast.ident, params : (Ast.pat * Ast.ty) list,
        ret : Ast.ty option, contracts : Ast.contract list,
        body : Ast.expr, span : Span.span
      }) : string =
    case #ret r of
      NONE => "int"
    | SOME at => astTyToCString at

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
      val contractExprs = reqs @ enss
      fun uniqOldNames [] acc = rev acc
        | uniqOldNames (x :: xr) acc =
            if List.exists (fn u => String.compare (u, x) = EQUAL) acc then
              uniqOldNames xr acc
            else
              uniqOldNames xr (x :: acc)
      val oldMentions =
        uniqOldNames (List.concat (map allOldNames contractExprs)) []
      val oldTargets =
        List.filter
          (fn (p, _) =>
             case p of
               Ast.PatBind (x, _) =>
                 List.exists (fn nm => String.compare (nm, x) = EQUAL) oldMentions
             | _ => false) (#params r)
      val oldInstrs =
        List.concat (
          map (fn pt =>
            let
              val x = paramName pt
              val cty = astTyToCString (#2 pt)
              val s = oldSlot x
            in
              if cty = "int" then [Ir.DeclVar s, Ir.Store (s, Ir.Load x)]
              else [Ir.DeclNamed (cty, s), Ir.Store (s, Ir.Load x)]
            end) oldTargets)
      val ps =
        map (fn pt => (paramName pt, astTyToCString (#2 pt))) (#params r)
      val () =
        ( currentFnName := fnName
        ; tmpCtr := 0
        ; ensuresResultSlot := NONE
        ; scrutLocals := []
        ; currentFnParams := map (fn (p, t) => (paramName (p, t), t)) (#params r))
      val reqInstrs =
        List.concat (
          map (fn e =>
                 let val (pre, ec) = lowerExprWithInstrs e
                 in pre @ [Ir.Requires (ec, fnName)] end) reqs)
      val body0 = lowerBody (#body r)
      val ensParts =
        map (fn e => lowerEnsuresClause e useRetSlot) enss
      val body1 = injectEnsuresAndRetSlot fnName useRetSlot ensParts body0
      val decl = if useRetSlot then [Ir.DeclVar retValueSlot] else []
      val retTy = fnRetCty r
    in
      {
        label = fnName,
        params = ps,
        retCType = retTy,
        instrs = decl @ oldInstrs @ reqInstrs @ body1
      }
    end

  fun variantSlots (v : Ast.variant) : int =
    case v of
      Ast.VariantUnit _ => 0
    | Ast.VariantTuple (_, ts, _) => length ts
    | Ast.VariantStruct (_, fs, _) => length fs

  fun emitStructTd (r : {
        name : Ast.ident, fields : (Ast.ident * Ast.ty) list, span : Span.span
      }) : string =
    let
      val lines =
        map (fn (f, aty) => "  " ^ astTyToCString aty ^ " " ^ f ^ ";\n") (#fields r)
    in
      "typedef struct {\n" ^ String.concat lines ^ "} " ^ #name r ^ ";\n"
    end

  fun emitEnumTd (r : {
        name : Ast.ident, variants : Ast.variant list, span : Span.span
      }) : string =
    let
      val maxP =
        List.foldl (fn (v, m) => Int.max (variantSlots v, m)) 0 (#variants r)
      val ps =
        if maxP <= 0 then ""
        else
          String.concat (
            List.tabulate (maxP, fn i => "  int p" ^ Int.toString i ^ ";\n"))
    in
      "typedef struct {\n  int tag;\n" ^ ps ^ "} " ^ #name r ^ ";\n"
    end

  fun collectTypedefs (prog : Ast.program) : string =
    String.concat (
      List.mapPartial (fn Ast.ItemStruct r => SOME (emitStructTd r) | _ => NONE)
        prog
      @ List.mapPartial (fn Ast.ItemEnum r => SOME (emitEnumTd r) | _ => NONE)
          prog)

  fun buildStructOrder (prog : Ast.program) : (string * string list) list =
    List.mapPartial
      (fn Ast.ItemStruct {name, fields, ...} =>
         SOME (name, map #1 fields) | _ => NONE) prog

  fun buildEnumVariants (prog : Ast.program) : (string * ((string * int) list * int)) list =
    let
      fun tagl i [] = []
        | tagl i (v :: vs) =
            let val nm =
              case v of
                Ast.VariantUnit (n, _) => n
              | Ast.VariantTuple (n, _, _) => n
              | Ast.VariantStruct (n, _, _) => n
            in
              (nm, i) :: tagl (i + 1) vs
            end
      fun oneEnum (name, variants) =
        let
          val maxP =
            List.foldl (fn (v, m) => Int.max (variantSlots v, m)) 0 variants
        in
          (name, (tagl 0 variants, maxP))
        end
    in
      List.mapPartial
        (fn Ast.ItemEnum {name, variants, ...} =>
           SOME (oneEnum (name, variants)) | _ => NONE) prog
    end

  fun lower (prog : Ast.program) : Ir.program =
    let
      val () =
        ( theProg := prog
        ; structFieldOrder := buildStructOrder prog
        ; enumVariants := buildEnumVariants prog)
      fun one it =
        case it of
          Ast.ItemFn r => SOME (lowerFn r)
        | _ => NONE
    in
      {typedefs = collectTypedefs prog, blocks = List.mapPartial one prog}
    end
end

(* IR + AST -> sv0 bytecode (.sv0b). See Bytecode structure and sv0doc/bytecode/. *)

structure VmCodegen :> VM_CODEGEN = struct

  structure A = Ast
  structure I = Ir
  structure B = Bytecode

  fun int32 (i : IntInf.int) : Int32.int =
    Int32.fromLarge (IntInf.toLarge i)

  fun variantSlots (v : A.variant) : int =
    case v of
      A.VariantUnit _ => 0
    | A.VariantTuple (_, ts, _) => length ts
    | A.VariantStruct (_, fs, _) => length fs

  fun structLayouts (prog : A.program) : (string * string list) list =
    List.mapPartial
      (fn A.ItemStruct {name, fields, ...} =>
         SOME (name, map #1 fields) | _ => NONE) prog

  fun enumLayouts (prog : A.program) : (string * int) list =
    List.mapPartial
      (fn A.ItemEnum {name, variants, ...} =>
         let
           val maxP =
             List.foldl (fn (v, m) => Int.max (variantSlots v, m)) 0 variants
         in
           SOME (name, 1 + maxP)
         end
      | _ => NONE) prog

  fun widthOfCty (structs : (string * string list) list) (enums : (string * int) list)
    (cty : string) : int =
    if cty = "void" then 0
    else if String.isPrefix "int" cty orelse String.isPrefix "uint" cty orelse cty = "bool" then 1
    else
      case List.find (fn (n, _) => n = cty) structs of
        SOME (_, fs) => length fs
      | NONE =>
          case List.find (fn (n, _) => n = cty) enums of
            SOME (_, w) => w
          | NONE => 1

  fun indexOfField (fields : string list) (f : string) : int =
    let fun go i [] = raise Fail ("vm: bad field `" ^ f ^ "`")
          | go i (h :: t) = if h = f then i else go (i + 1) t
    in go 0 fields end

  fun enumFieldNames (width : int) : string list =
    "tag" :: List.tabulate (Int.max (0, width - 1), fn i => "p" ^ Int.toString i)

  type slot_info = {base : int, width : int, fields : string list}

  type slot_env = (string * slot_info) list

  fun lookupSlot (env : slot_env) (x : string) : slot_info =
    case List.find (fn (n, _) => n = x) env of
      SOME (_, info) => info
    | NONE => raise Fail ("vm: unbound local `" ^ x ^ "`")

  fun encLen (i : B.insn) : int = Word8Vector.length (B.encodeInsn i)
  fun encLens (xs : B.insn list) : int =
    List.foldl (fn (i, a) => a + encLen i) 0 xs

  type pool = {strings : string list ref, index : (string * int) list ref}

  fun poolInit () : pool =
    {strings = ref [], index = ref []}

  fun poolAdd (p : pool) (s : string) : int =
    case List.find (fn (t, _) => t = s) (! (#index p)) of
      SOME (_, i) => i
    | NONE =>
        let val i = length (! (#strings p))
        in
          #strings p := ! (#strings p) @ [s];
          #index p := (s, i) :: ! (#index p);
          i
        end

  fun binopToInsn (opStr : string) : B.insn =
    case opStr of
      "+" => B.ADD_I32
    | "-" => B.SUB_I32
    | "*" => B.MUL_I32
    | "/" => B.DIV_I32
    | "%" => B.MOD_I32
    | "==" => B.EQ
    | "!=" => B.NEQ
    | "<" => B.LT
    | ">" => B.GT
    | "<=" => B.LTE
    | ">=" => B.GTE
    | "&&" => B.AND
    | "||" => B.OR
    | "&" => B.BIT_AND
    | "|" => B.BIT_OR
    | "^" => B.BIT_XOR
    | "<<" => B.SHL
    | ">>" => B.SHR
    | _ => raise Fail ("vm: unsupported binop `" ^ opStr ^ "`")

  fun emitValue (structs : (string * string list) list) (enums : (string * int) list)
    (env : slot_env) (pool : pool) (v : I.value) : B.insn list =
    case v of
      I.VInt i => [B.PUSH_I32 (int32 i)]
    | I.VBool b => [B.PUSH_BOOL b]
    | I.VUnit => [B.PUSH_UNIT]
    | I.VString s => [B.PUSH_STRING (poolAdd pool s)]
    | I.VVar x =>
        let val {base, width, ...} = lookupSlot env x
        in List.tabulate (width, fn k => B.LOAD_LOCAL (base + k)) end
    | I.VMember (I.VVar x, f) =>
        let val {base, fields, ...} = lookupSlot env x
        in
          if null fields then raise Fail "vm: VMember on scalar"
          else [B.LOAD_LOCAL (base + indexOfField fields f)]
        end
    | I.VMember (v2, f) =>
        emitExpr structs enums env pool (I.FieldAccess (v2, f))
    | I.VAddrOf _ => raise Fail "vm: VAddrOf in bytecode slice"
    | I.VFloat _ => raise Fail "vm: float in bytecode slice"

  and emitExpr (structs : (string * string list) list) (enums : (string * int) list)
    (env : slot_env) (pool : pool) (e : I.expr) : B.insn list =
    case e of
      I.Literal v => emitValue structs enums env pool v
    | I.Load x =>
        let val {base, width, ...} = lookupSlot env x
        in List.tabulate (width, fn k => B.LOAD_LOCAL (base + k)) end
    | I.Binop (bop, v1, v2) =>
        emitValue structs enums env pool v1 @ emitValue structs enums env pool v2 @ [binopToInsn bop]
    | I.Unop ("!", v) => emitValue structs enums env pool v @ [B.NOT]
    | I.Unop ("-", v) => emitValue structs enums env pool v @ [B.NEG_I32]
    | I.Unop (_, v) =>
        (* cast-like (int8_t), (uint32_t), etc. — identity for int-width VM *)
        emitValue structs enums env pool v
    | I.FieldAccess (v, f) =>
        (case v of
           I.VVar x =>
             let val {base, fields, ...} = lookupSlot env x
             in
               if null fields then raise Fail "vm: FieldAccess on scalar"
               else [B.LOAD_LOCAL (base + indexOfField fields f)]
             end
         | _ => raise Fail "vm: FieldAccess needs VVar base in this slice")
    | I.IndexAccess _ => raise Fail "vm: IndexAccess not supported"

  fun bindParam (structs : (string * string list) list) (enums : (string * int) list)
    (name : string, cty : string) (slot : int) : string * slot_info =
    let
      val w = widthOfCty structs enums cty
      val fields =
        case List.find (fn (n, _) => n = cty) structs of
          SOME (_, fs) => fs
        | NONE =>
            case List.find (fn (n, _) => n = cty) enums of
              SOME (_, ew) => enumFieldNames ew
            | NONE => []
    in
      (name, {base = slot, width = w, fields = fields})
    end

  fun allocLocal (structs : (string * string list) list) (enums : (string * int) list)
    (cty : string) (name : string) (slot : int) : string * slot_info =
    bindParam structs enums (name, cty) slot

  fun valueWidth (env : slot_env) (v : I.value) : int =
    case v of
      I.VVar x => #width (lookupSlot env x)
    | _ => 1

  fun emit (ast : A.program, prog : I.program) : Word8Vector.vector =
    let
      val structs = structLayouts ast
      val enums = enumLayouts ast
      val blocks = #blocks prog
      val pool = poolInit ()
      val funcOrder =
        let
          fun isMain (b : I.block) = #label b = "main"
          val mains = List.filter isMain blocks
          val rest = List.filter (not o isMain) blocks
        in
          rest @ (case mains of m :: _ => [m] | [] => [])
        end
      val nameToIndex =
        #2 (List.foldl (fn (b, (i, m)) => (i + 1, (#label b, i) :: m)) (0, []) funcOrder)
      fun indexOfFn lab =
        case List.find (fn (n, _) => n = lab) nameToIndex of
          SOME (_, i) => i
        | NONE => raise Fail ("vm: unknown function `" ^ lab ^ "`")

      fun localCountAndEnv (b : I.block) : int * slot_env =
        let
          val next = ref 0
          val env = ref ([] : slot_env)
          fun addBinding (name, info as {base, width, ...}) =
            ( env := (name, info) :: !env
            ; next := base + width
            )
          fun ensureVarInt (x : string) =
            case List.find (fn (n, _) => n = x) (!env) of
              SOME _ => ()
            | NONE =>
                let val s = !next
                in addBinding (x, {base = s, width = 1, fields = []}) end
          val () =
            app
              (fn (pname, pcty) =>
                 let
                   val s = !next
                   val (n, info) = bindParam structs enums (pname, pcty) s
                 in
                   addBinding (n, info)
                 end) (#params b)
          fun scanInstr ins =
            case ins of
              I.DeclVar x => ensureVarInt x
            | I.DeclNamed (cty, x) =>
                let val s = !next
                    val (n, info) = allocLocal structs enums cty x s
                in addBinding (n, info) end
            | I.Assign (x, _) => ensureVarInt x
            | I.Call (SOME d, _, _, rty) =>
                let
                  val s = !next
                  val w = Int.max (1, widthOfCty structs enums rty)
                  val fields : string list =
                    if w = 1 then []
                    else
                      (case List.find (fn (n, _) => n = rty) structs of
                         SOME (_, fs) => fs
                       | NONE =>
                           case List.find (fn (n, _) => n = rty) enums of
                             SOME (_, ew) => enumFieldNames ew
                           | NONE => [])
                in
                  addBinding (d, {base = s, width = w, fields = fields})
                end
            | I.IfElse (_, th, el) => (app scanInstr th; app scanInstr el)
            | I.While (_, body) => app scanInstr body
            | I.Block body => app scanInstr body
            | _ => ()
          val () = app scanInstr (#instrs b)
        in
          (!next, !env)
        end

      fun emitInstr (env : slot_env) (retTy : string) (ins : I.instr) : B.insn list =
        case ins of
          I.Nop => []
        | I.DeclVar _ => []
        | I.DeclNamed _ => []
        | I.Assign (x, e) =>
            emitExpr structs enums env pool e
            @ (let val {base, width, ...} = lookupSlot env x
               in
                 List.tabulate (width, fn k => B.STORE_LOCAL (base + width - 1 - k))
               end)
            (* stack has v_w ... v1 TOS; store TOS to base+w-1, ... *)
        | I.Store (x, e) =>
            emitExpr structs enums env pool e
            @ (let val {base, width, ...} = lookupSlot env x
               in List.tabulate (width, fn k => B.STORE_LOCAL (base + width - 1 - k)) end)
        | I.StoreField (base, fld, e) =>
            let
              val {base = b0, fields, ...} = lookupSlot env base
              val off = indexOfField fields fld
            in
              emitExpr structs enums env pool e @ [B.STORE_LOCAL (b0 + off)]
            end
        | I.IfElse (ec, th, el) =>
            let
              val c = emitExpr structs enums env pool ec
              val th' = emitBlock env retTy th
              val el' = emitBlock env retTy el
              val offElse = encLens th' + encLen (B.JUMP 0)
              val offEnd = encLens el'
            in
              c @ [B.JUMP_IF_NOT offElse] @ th' @ [B.JUMP offEnd] @ el'
            end
        | I.While (ec, body) =>
            let
              val condC = emitExpr structs enums env pool ec
              val bodyC = emitBlock env retTy body
              val forward = encLens bodyC + encLen (B.JUMP 0)
              val back =
                ~(encLens condC + encLen (B.JUMP_IF_NOT 0) + encLens bodyC + encLen (B.JUMP 0))
            in
              condC @ [B.JUMP_IF_NOT forward] @ bodyC @ [B.JUMP back]
            end
        | I.Block xs => emitBlock env retTy xs
        | I.Break => raise Fail "vm: break not supported in VM backend"
        | I.Continue => raise Fail "vm: continue not supported in VM backend"
        | I.Return NONE =>
            if retTy = "void" then [B.RETURN_SLOTS 0] else raise Fail "vm: bad void return"
        | I.Return (SOME v) =>
            emitValue structs enums env pool v @ [B.RETURN_SLOTS (valueWidth env v)]
        | I.Call (NONE, "sv0_println", [I.VString s], _) =>
            [B.PUSH_STRING (poolAdd pool s), B.CALL_BUILTIN 0]
        | I.Call (NONE, f, vs, _) =>
            let
              val nargs =
                List.foldl (fn (v, a) => a + (case v of I.VVar x => (#width (lookupSlot env x)) | _ => 1)) 0 vs
              val pushes =
                List.concat
                  (map
                    (fn v =>
                       case v of
                         I.VVar x =>
                           let val {base, width, ...} = lookupSlot env x
                           in List.tabulate (width, fn k => B.LOAD_LOCAL (base + k)) end
                       | _ => emitValue structs enums env pool v) vs)
              val ix = indexOfFn f
            in
              pushes @ [B.CALL (ix, nargs)]
            end
        | I.Call (SOME d, f, vs, rty) =>
            let
              val nargs =
                List.foldl (fn (v, a) => a + (case v of I.VVar x => (#width (lookupSlot env x)) | _ => 1)) 0 vs
              val pushes =
                List.concat
                  (map
                    (fn v =>
                       case v of
                         I.VVar x =>
                           let val {base, width, ...} = lookupSlot env x
                           in List.tabulate (width, fn k => B.LOAD_LOCAL (base + k)) end
                       | _ => emitValue structs enums env pool v) vs)
              val ix = indexOfFn f
              val {base, width, ...} = lookupSlot env d
              val stores =
                List.tabulate (width, fn k => B.STORE_LOCAL (base + width - 1 - k))
            in
              pushes @ [B.CALL (ix, nargs)] @ stores
            end
        | I.Requires (e, fnName) =>
            emitExpr structs enums env pool e
            @ [B.CONTRACT_CHECK (poolAdd pool ("requires failed: " ^ fnName))]
        | I.Ensures (e, fnName) =>
            emitExpr structs enums env pool e
            @ [B.CONTRACT_CHECK (poolAdd pool ("ensures failed: " ^ fnName))]
        | _ => raise Fail "vm: instruction not supported"

      and emitBlock (env : slot_env) (retTy : string) (xs : I.instr list) : B.insn list =
        List.concat (map (emitInstr env retTy) xs)

      fun emitFn (b : I.block) : B.func_entry =
        let
          val (lc, env0) = localCountAndEnv b
          val code = emitBlock env0 (#retCType b) (#instrs b)
        in
          { nameIdx = poolAdd pool (#label b)
          , arity =
              List.foldl (fn ((_, cty), a) => a + widthOfCty structs enums cty) 0 (#params b)
          , localCount = lc
          , code = code
          }
        end

      val funcs = map emitFn funcOrder
      val strings = !(#strings pool)
    in
      B.encodeFile {strings = strings, funcs = funcs}
    end

end

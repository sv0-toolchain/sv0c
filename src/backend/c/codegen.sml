(* C99 emission for a flat IR: one C function per block label. *)

structure Codegen :> CODEGEN = struct

  fun intLit (i : IntInf.int) : string = IntInf.toString i

  fun cTypeForRet (instrs : Ir.instr list) : string =
    case List.rev instrs of
      (Ir.Return NONE) :: _ => "void"
    | [] => "void"
    | _ => "int"

  fun emitValue (v : Ir.value) : string =
    case v of
      Ir.VInt i => intLit i
    | Ir.VBool true => "1"
    | Ir.VBool false => "0"
    | Ir.VVar x => x
    | Ir.VUnit => "0"
    | _ => raise Fail "value not supported in codegen slice"

  fun emitCallArgs (vs : Ir.value list) : string =
    String.concatWith ", " (map emitValue vs)

  fun emitExpr (e : Ir.expr) : string =
    case e of
      Ir.Literal v => emitValue v
    | Ir.Load x => x
    | Ir.Binop (opStr, v1, v2) =>
        "(" ^ emitValue v1 ^ " " ^ opStr ^ " " ^ emitValue v2 ^ ")"
    | Ir.Unop (opStr, v) => "(" ^ opStr ^ emitValue v ^ ")"
    | _ => raise Fail "expression not supported in codegen slice"

  fun emitParamList (names : string list) : string =
    case names of
      [] => "void"
    | _ => String.concatWith ", " (map (fn n => "int " ^ n) names)

  (* isStatic: true for helpers (need forward decl), false for main. *)
  fun emitFnProto (isStatic : bool) (b : Ir.block) : string =
    let
      val retTy = cTypeForRet (#instrs b)
      val staticKw = if isStatic then "static " else ""
    in
      staticKw ^ retTy ^ " " ^ #label b ^ "(" ^ emitParamList (#params b) ^ ")"
    end

  fun emitInstr (indent : string) (retTy : string) (ins : Ir.instr) : string =
    case ins of
      Ir.Nop => ""
    | Ir.DeclVar x => indent ^ "int " ^ x ^ ";\n"
    | Ir.Assign (x, e) => indent ^ "int " ^ x ^ " = " ^ emitExpr e ^ ";\n"
    | Ir.Store (x, e) => indent ^ x ^ " = " ^ emitExpr e ^ ";\n"
    | Ir.IfElse (ec, th, el) =>
        indent ^ "if (" ^ emitExpr ec ^ ") {\n"
        ^ String.concat (map (emitInstr (indent ^ "  ") retTy) th)
        ^ indent ^ "} else {\n"
        ^ String.concat (map (emitInstr (indent ^ "  ") retTy) el)
        ^ indent ^ "}\n"
    | Ir.Call (SOME d, f, vs) =>
        indent ^ "int " ^ d ^ " = " ^ f ^ "(" ^ emitCallArgs vs ^ ");\n"
    | Ir.Call (NONE, f, vs) =>
        indent ^ f ^ "(" ^ emitCallArgs vs ^ ");\n"
    | Ir.Requires (e, fnName) =>
        indent ^ "sv0_requires(" ^ emitExpr e ^ ", \"" ^ fnName ^ "\");\n"
    | Ir.Ensures (e, fnName) =>
        indent ^ "sv0_ensures(" ^ emitExpr e ^ ", \"" ^ fnName ^ "\");\n"
    | Ir.Return NONE =>
        if retTy = "void" then indent ^ "return;\n"
        else raise Fail "invalid return without value for int function"
    | Ir.Return (SOME v) => indent ^ "return " ^ emitValue v ^ ";\n"
    | _ => raise Fail "instruction not supported in codegen slice"

  fun emitBlockBody (b : Ir.block) : string =
    let val retTy = cTypeForRet (#instrs b)
    in String.concat (map (emitInstr "  " retTy) (#instrs b)) end

  fun emitBlockDefn (isStatic : bool) (b : Ir.block) : string =
    emitFnProto isStatic b ^ " {\n" ^ emitBlockBody b ^ "}\n\n"

  fun splitMain (bs : Ir.block list) : Ir.block list * Ir.block option =
    let
      fun isMain (b : Ir.block) = #label b = "main"
      val mains = List.filter isMain bs
      val rest = List.filter (not o isMain) bs
    in
      (rest, case mains of m :: _ => SOME m | [] => NONE)
    end

  fun emit (prog : Ir.program) : string =
    if null prog then
      "#include \"sv0_runtime.h\"\n\
      \int main(void) { return 0; }\n"
    else
      let
        val (others, mainOpt) = splitMain prog
        val prelude = "#include \"sv0_runtime.h\"\n\n"
        val forwardDecls =
          String.concat (map (fn b => emitFnProto true b ^ ";\n") others)
        val statics =
          String.concat (map (fn b => emitBlockDefn true b) others)
        val mainPart =
          case mainOpt of
            SOME b => emitBlockDefn false b
          | NONE => "int main(void) { return 0; }\n"
      in
        prelude ^ forwardDecls ^ "\n" ^ statics ^ mainPart
      end
end

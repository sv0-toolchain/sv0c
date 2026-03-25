(* C99 emission for a flat IR: one C function per block label. *)

structure Codegen :> CODEGEN = struct

  fun intLit (i : IntInf.int) : string = IntInf.toString i

  fun cEscapeString (s : string) : string =
    let
      val n = String.size s
      fun one i =
        case String.sub (s, i) of
          #"\"" => "\\\""
        | #"\\" => "\\\\"
        | #"\n" => "\\n"
        | #"\t" => "\\t"
        | #"\r" => "\\r"
        | c =>
            if Char.ord c < 32 then
              "\\" ^ Int.toString (Char.ord c)
            else
              String.str c
    in
      String.concat (List.tabulate (n, one))
    end

  fun emitValue (v : Ir.value) : string =
    case v of
      Ir.VInt i => intLit i
    | Ir.VBool true => "1"
    | Ir.VBool false => "0"
    | Ir.VVar x => x
    | Ir.VUnit => "0"
    | Ir.VString s => "\"" ^ cEscapeString s ^ "\""
    | Ir.VMember (Ir.VVar x, f) => x ^ "." ^ f
    | Ir.VMember (v2, f) => "(" ^ emitValue v2 ^ ")." ^ f
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
    | Ir.FieldAccess (v, f) => "(" ^ emitValue v ^ ")." ^ f
    | _ => raise Fail "expression not supported in codegen slice"

  fun emitParamList (ps : (string * string) list) : string =
    case ps of
      [] => "void"
    | _ =>
        String.concatWith ", " (map (fn (n, cty) => cty ^ " " ^ n) ps)

  (* isStatic: true for helpers (need forward decl), false for main. *)
  fun emitFnProto (isStatic : bool) (b : Ir.block) : string =
    let
      val retTy = #retCType b
      val staticKw = if isStatic then "static " else ""
    in
      staticKw ^ retTy ^ " " ^ #label b ^ "(" ^ emitParamList (#params b) ^ ")"
    end

  fun emitInstr (indent : string) (retTy : string) (ins : Ir.instr) : string =
    case ins of
      Ir.Nop => ""
    | Ir.DeclVar x => indent ^ "int " ^ x ^ ";\n"
    | Ir.DeclNamed (cty, x) => indent ^ cty ^ " " ^ x ^ ";\n"
    | Ir.Assign (x, e) => indent ^ "int " ^ x ^ " = " ^ emitExpr e ^ ";\n"
    | Ir.Store (x, e) => indent ^ x ^ " = " ^ emitExpr e ^ ";\n"
    | Ir.StoreField (base, fld, e) =>
        indent ^ base ^ "." ^ fld ^ " = " ^ emitExpr e ^ ";\n"
    | Ir.IfElse (ec, th, el) =>
        indent ^ "if (" ^ emitExpr ec ^ ") {\n"
        ^ String.concat (map (emitInstr (indent ^ "  ") retTy) th)
        ^ indent ^ "} else {\n"
        ^ String.concat (map (emitInstr (indent ^ "  ") retTy) el)
        ^ indent ^ "}\n"
    | Ir.While (ec, body) =>
        indent ^ "while (" ^ emitExpr ec ^ ") {\n"
        ^ String.concat (map (emitInstr (indent ^ "  ") retTy) body)
        ^ indent ^ "}\n"
    | Ir.Block body =>
        indent ^ "{\n"
        ^ String.concat (map (emitInstr (indent ^ "  ") retTy) body)
        ^ indent ^ "}\n"
    | Ir.Break => indent ^ "break;\n"
    | Ir.Continue => indent ^ "continue;\n"
    | Ir.Call (SOME d, f, vs, rty) =>
        indent ^ rty ^ " " ^ d ^ " = " ^ f ^ "(" ^ emitCallArgs vs ^ ");\n"
    | Ir.Call (NONE, f, vs, _) =>
        indent ^ f ^ "(" ^ emitCallArgs vs ^ ");\n"
    | Ir.Requires (e, fnName) =>
        indent ^ "sv0_requires(" ^ emitExpr e ^ ", \"" ^ fnName ^ "\");\n"
    | Ir.Ensures (e, fnName) =>
        indent ^ "sv0_ensures(" ^ emitExpr e ^ ", \"" ^ fnName ^ "\");\n"
    | Ir.Return NONE =>
        if retTy = "void" then indent ^ "return;\n"
        else raise Fail "invalid return without value for non-void function"
    | Ir.Return (SOME v) =>
        if retTy = "void" then indent ^ "return;\n"
        else indent ^ "return " ^ emitValue v ^ ";\n"
    | _ => raise Fail "instruction not supported in codegen slice"

  fun emitBlockBody (b : Ir.block) : string =
    let val retTy = #retCType b
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
    let val blocks = #blocks prog
    in
      if null blocks then
        "#include \"sv0_runtime.h\"\n\
        \int main(void) { return 0; }\n"
      else
        let
          val (others, mainOpt) = splitMain blocks
          val prelude = "#include \"sv0_runtime.h\"\n\n"
          val td = #typedefs prog
          val tdPart = if td = "" then "" else td ^ "\n"
          val forwardDecls =
            String.concat (map (fn b => emitFnProto true b ^ ";\n") others)
          val statics =
            String.concat (map (fn b => emitBlockDefn true b) others)
          val mainPart =
            case mainOpt of
              SOME b => emitBlockDefn false b
            | NONE => "int main(void) { return 0; }\n"
        in
          prelude ^ tdPart ^ forwardDecls ^ "\n" ^ statics ^ mainPart
        end
    end
end

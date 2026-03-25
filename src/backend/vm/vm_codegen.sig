signature VM_CODEGEN = sig
  (* Lower IR + AST layout metadata to a .sv0b byte vector (see Bytecode.encodeFile). *)
  val emit : Ast.program * Ir.program -> Word8Vector.vector
end

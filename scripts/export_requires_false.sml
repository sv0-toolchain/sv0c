(* Emit C for a program whose requires(false) must abort at runtime. *)
CM.make "sources.cm";
val src = "fn main() -> i32 requires(false) { return 0; }\n";
val toks = Lexer.tokenize "contract_fail.sv0" src;
val ast = Parser.parse toks;
val ast = Resolver.resolve ast;
val () = Lowering.setImportAliases (Resolver.peekImportAliases ());
val ast = Checker.check ast;
val ast = ContractAnalyzer.analyze ast;
val ast = Link.stripLinkDirectives ast;
val ir = Lowering.lower ast;
val c = Codegen.emit ir;
val () =
  let val out : TextIO.outstream = TextIO.openOut "build/contract_requires_fail.c"
  in TextIO.output (out, c); TextIO.closeOut out end;
OS.Process.exit OS.Process.success;

(* Emit build/e2e_generated.c for Makefile e2e target. Run from sv0c: sml < scripts/export_e2e.sml *)
CM.make "sources.cm";
val nl = String.str #"\n"
val src =
  String.concat
    [ "fn inc(x: i32) -> i32 requires(true) ensures(true) { return x + 1; }"
    , nl
    , "fn main() -> i32 requires(true) ensures(true) {"
    , nl
    , "  while false loop_invariant(true) {};"
    , nl
    , "  for i in 0..0 {};"
    , nl
    , "  return inc(41);"
    , nl
    , "}"
    , nl
    ]
val toks = Lexer.tokenize "e2e.sv0" src;
val ast = Parser.parse toks;
val ast = Resolver.resolve ast;
val () = Lowering.setImportAliases (Resolver.peekImportAliases ());
val ast = Checker.check ast;
val ast = ContractAnalyzer.analyze ast;
val ast = Link.stripLinkDirectives ast;
val ir = Lowering.lower ast;
val c = Codegen.emit ir;
val () =
  let val out : TextIO.outstream = TextIO.openOut "build/e2e_generated.c"
  in TextIO.output (out, c); TextIO.closeOut out end;
OS.Process.exit OS.Process.success;

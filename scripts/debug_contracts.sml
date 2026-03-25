CM.make "sources.cm";
fun try s =
  let val ast = Resolver.resolve (Parser.parse (Lexer.tokenize "<t>" s))
      val () = Lowering.setImportAliases (Resolver.peekImportAliases ())
  in
    (Checker.check ast; "OK")
  end
  handle Fail m => m;
print (try "fn main() -> unit ensures(result) { }\n");
print "\n";
print (try "fn main() -> i32 ensures(result >= 0) { return 7; }\n");
OS.Process.exit OS.Process.success;

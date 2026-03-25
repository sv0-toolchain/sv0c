structure Main = struct
  fun compile filename =
    let
      val ins = TextIO.openIn filename
      val source = TextIO.inputAll ins
      val () = TextIO.closeIn ins
      val tokens = Lexer.tokenize filename source
      val ast = Parser.parse tokens
      val ast = Resolver.resolve ast
      val ast = Checker.check ast
      val ast = ContractAnalyzer.analyze ast
      val ir = Lowering.lower ast
      val cCode = Codegen.emit ir
    in
      print cCode
    end
    handle Fail msg =>
      TextIO.output (TextIO.stdErr, "sv0c error: " ^ msg ^ "\n")
    | IO.Io {name, ...} =>
      TextIO.output (TextIO.stdErr, "sv0c error: cannot open " ^ name ^ "\n")

  fun main (_, args) =
    case args of
      [filename] => (compile filename; OS.Process.success)
    | _ =>
        (TextIO.output (TextIO.stdErr,
           "sv0c: sv0 bootstrap compiler\n\
           \usage: sv0c <file.sv0>\n");
         OS.Process.failure)
end

structure Main = struct

  fun baseNameSv0 (path : string) : string =
    let val f = OS.Path.file path
        val n = String.size f
    in
      if n > 4 andalso String.substring (f, n - 4, 4) = ".sv0" then
        String.substring (f, 0, n - 4)
      else
        f
    end

  fun writeBytes (path : string) (vec : Word8Vector.vector) : unit =
    let val outs = BinIO.openOut path
        val () = BinIO.output (outs, vec)
        val () = BinIO.closeOut outs
    in
      ()
    end

  fun readFile (filename : string) : string =
    let
      val ins = TextIO.openIn filename
      val s = TextIO.inputAll ins
      val () = TextIO.closeIn ins
    in
      s
    end

  fun compileProgram (sourceOpt : string option) (ast : Ast.program) : unit =
    let
      val ast = Resolver.resolve ast
      val () = Lowering.setImportAliases (Resolver.peekImportAliases ())
      val ast = Checker.check ast
      val ast = ContractAnalyzer.analyze ast
      val ast = Link.stripLinkDirectives ast
      val ir = Lowering.lower ast
      val cCode = Codegen.emit ir
    in
      print cCode
    end
    handle Diagnostic.Diag d =>
      ( TextIO.output (TextIO.stdErr, Diagnostic.format sourceOpt d)
      ; TextIO.flushOut TextIO.stdErr
      )
    | Fail msg =>
      TextIO.output (TextIO.stdErr, "sv0c error: " ^ msg ^ "\n")
    | IO.Io {name, ...} =>
      TextIO.output (TextIO.stdErr, "sv0c error: cannot open " ^ name ^ "\n")

  fun compileFile (path : string) : unit =
    let
      val source = readFile path
      val ast = Parser.parse (Lexer.tokenize path source)
    in
      compileProgram (SOME source) ast
    end

  fun compileProjectDir (dir : string) : unit =
    let val ast = Link.linkProjectDir dir
    in compileProgram NONE ast end

  fun compileProgramVm (sourceOpt : string option) (ast : Ast.program) (outPath : string) : unit =
    let
      val ast = Resolver.resolve ast
      val () = Lowering.setImportAliases (Resolver.peekImportAliases ())
      val ast = Checker.check ast
      val ast = ContractAnalyzer.analyze ast
      val ast = Link.stripLinkDirectives ast
      val ir = Lowering.lower ast
      val bytes = VmCodegen.emit (ast, ir)
      val () = OS.FileSys.mkDir "build/vm" handle OS.SysErr _ => ()
      val () = writeBytes outPath bytes
    in
      ()
    end
    handle Diagnostic.Diag d =>
      ( TextIO.output (TextIO.stdErr, Diagnostic.format sourceOpt d)
      ; TextIO.flushOut TextIO.stdErr
      )
    | Fail msg =>
      TextIO.output (TextIO.stdErr, "sv0c error: " ^ msg ^ "\n")
    | IO.Io {name, ...} =>
      TextIO.output (TextIO.stdErr, "sv0c error: cannot open " ^ name ^ "\n")

  fun compileFileVm (path : string) : unit =
    let
      val source = readFile path
      val ast = Parser.parse (Lexer.tokenize path source)
      val out = "build/vm/" ^ baseNameSv0 path ^ ".sv0b"
    in
      compileProgramVm (SOME source) ast out
    end

  fun main (_, args) : OS.Process.status =
    case args of
      ["--target=vm", filename] =>
        (compileFileVm filename; OS.Process.success)
    | "--target" :: "vm" :: filename :: _ =>
        (compileFileVm filename; OS.Process.success)
    | ["--project", dir] =>
        (compileProjectDir dir; OS.Process.success)
    | [filename] =>
        (compileFile filename; OS.Process.success)
    | _ =>
        ( TextIO.output (TextIO.stdErr,
            "sv0c: sv0 bootstrap compiler\n\
            \usage: sv0c <file.sv0>\n\
            \       sv0c --target=vm <file.sv0>   (emit build/vm/<stem>.sv0b)\n\
            \       sv0c --project <dir>   (all *.sv0 in directory)\n")
        ; OS.Process.failure)
end

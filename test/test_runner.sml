structure TestRunner = struct
  val passed = ref 0
  val failed = ref 0

  fun check name cond =
    if cond then
      (passed := !passed + 1;
       print ("  PASS: " ^ name ^ "\n"))
    else
      (failed := !failed + 1;
       print ("  FAIL: " ^ name ^ "\n"))

  fun summary () =
    let
      val p = !passed
      val f = !failed
    in
      print ("\n" ^ Int.toString (p + f) ^ " tests, " ^
             Int.toString p ^ " passed, " ^
             Int.toString f ^ " failed\n");
      if f > 0 then OS.Process.failure else OS.Process.success
    end

  fun lex s = Lexer.tokenize "<test>" s
  fun toks s = List.map #1 (lex s)
  fun firstTok s = #1 (hd (lex s))

  fun run () =
    let
      val () = print "sv0c test suite\n"
      val () = print "===============\n\n"

      (* --- span --- *)
      val () = print "[span]\n"
      val sp = Span.bogus
      val () = check "Span.bogus toString" (Span.toString sp = "0:0-0:0")
      val p1 = Span.pos (1, 1, 0)
      val p2 = Span.pos (1, 10, 9)
      val sp1 = Span.span ("test.sv0", p1, p2)
      val () = check "Span.toString with file"
                 (Span.toString sp1 = "test.sv0:1:1-1:10")
      val () = check "Span.length" (Span.length sp1 = 9)

      (* --- diagnostic --- *)
      val () = print "\n[diagnostic]\n"
      val errSpan = Span.span ("main.sv0",
                               Span.pos (3, 18, 40),
                               Span.pos (3, 25, 47))
      val d = Diagnostic.error ("E0301", "narrowing cast may overflow", errSpan)
      val () = check "Diagnostic.hasErrors" (Diagnostic.hasErrors [d])
      val d2 = Diagnostic.withHelp (d, ["use .truncate()"])
      val source = "fn main() {\n" ^
                   "    let x: u32 = 1000;\n" ^
                   "    let z: u8 = x as u8;\n" ^
                   "}\n"
      val fmt = Diagnostic.format (SOME source) d2
      val () = check "format has error code" (String.isSubstring "E0301" fmt)
      val () = check "format has help" (String.isSubstring "truncate" fmt)

      (* --- lexer: empty and whitespace --- *)
      val () = print "\n[lexer: basics]\n"
      val () = check "empty input" (toks "" = [Token.EOF])
      val () = check "whitespace only" (toks "   \n\t  " = [Token.EOF])
      val () = check "line comment" (toks "// comment\n" = [Token.EOF])
      val () = check "block comment" (toks "/* comment */" = [Token.EOF])
      val () = check "nested block comment"
                 (toks "/* /* nested */ */" = [Token.EOF])

      (* --- lexer: keywords --- *)
      val () = print "\n[lexer: keywords]\n"
      val () = check "let" (firstTok "let" = Token.LET)
      val () = check "mut" (firstTok "mut" = Token.MUT)
      val () = check "fn" (firstTok "fn" = Token.FN)
      val () = check "return" (firstTok "return" = Token.RETURN)
      val () = check "if" (firstTok "if" = Token.IF)
      val () = check "else" (firstTok "else" = Token.ELSE)
      val () = check "match" (firstTok "match" = Token.MATCH)
      val () = check "while" (firstTok "while" = Token.WHILE)
      val () = check "for" (firstTok "for" = Token.FOR)
      val () = check "in" (firstTok "in" = Token.IN)
      val () = check "loop" (firstTok "loop" = Token.LOOP)
      val () = check "break" (firstTok "break" = Token.BREAK)
      val () = check "continue" (firstTok "continue" = Token.CONTINUE)
      val () = check "struct" (firstTok "struct" = Token.STRUCT)
      val () = check "enum" (firstTok "enum" = Token.ENUM)
      val () = check "trait" (firstTok "trait" = Token.TRAIT)
      val () = check "impl" (firstTok "impl" = Token.IMPL)
      val () = check "type" (firstTok "type" = Token.TYPE)
      val () = check "newtype" (firstTok "newtype" = Token.NEWTYPE)
      val () = check "where" (firstTok "where" = Token.WHERE)
      val () = check "pub" (firstTok "pub" = Token.PUB)
      val () = check "module" (firstTok "module" = Token.MODULE)
      val () = check "use" (firstTok "use" = Token.USE)
      val () = check "as" (firstTok "as" = Token.AS)
      val () = check "self" (firstTok "self" = Token.SELF)
      val () = check "Self" (firstTok "Self" = Token.SELF_TYPE)
      val () = check "requires" (firstTok "requires" = Token.REQUIRES)
      val () = check "ensures" (firstTok "ensures" = Token.ENSURES)
      val () = check "loop_invariant"
                 (firstTok "loop_invariant" = Token.LOOP_INVARIANT)
      val () = check "forall" (firstTok "forall" = Token.FORALL)
      val () = check "exists" (firstTok "exists" = Token.EXISTS)
      val () = check "borrows" (firstTok "borrows" = Token.BORROWS)
      val () = check "no_alias" (firstTok "no_alias" = Token.NO_ALIAS)
      val () = check "old" (firstTok "old" = Token.OLD)
      val () = check "result" (firstTok "result" = Token.RESULT)
      val () = check "const" (firstTok "const" = Token.CONST)
      val () = check "static" (firstTok "static" = Token.STATIC)
      val () = check "true" (firstTok "true" = Token.TRUE)
      val () = check "false" (firstTok "false" = Token.FALSE)
      val () = check "assert" (firstTok "assert" = Token.ASSERT)
      val () = check "move" (firstTok "move" = Token.MOVE)
      val () = check "unsafe" (firstTok "unsafe" = Token.UNSAFE)
      val () = check "project" (firstTok "project" = Token.PROJECT)

      (* Some/None/Ok/Err are identifiers, not keywords (D19) *)
      val () = check "Some is IDENT"
                 (firstTok "Some" = Token.IDENT "Some")
      val () = check "None is IDENT"
                 (firstTok "None" = Token.IDENT "None")
      val () = check "Ok is IDENT"
                 (firstTok "Ok" = Token.IDENT "Ok")
      val () = check "Err is IDENT"
                 (firstTok "Err" = Token.IDENT "Err")

      (* --- lexer: identifiers --- *)
      val () = print "\n[lexer: identifiers]\n"
      val () = check "simple ident"
                 (firstTok "foo" = Token.IDENT "foo")
      val () = check "ident with digits"
                 (firstTok "foo123" = Token.IDENT "foo123")
      val () = check "underscore ident"
                 (firstTok "_bar" = Token.IDENT "_bar")
      val () = check "keyword prefix is ident"
                 (firstTok "letter" = Token.IDENT "letter")

      (* --- lexer: integers --- *)
      val () = print "\n[lexer: integers]\n"
      val () = check "zero"
                 (firstTok "0" = Token.INT_LIT (IntInf.fromInt 0))
      val () = check "decimal"
                 (firstTok "42" = Token.INT_LIT (IntInf.fromInt 42))
      val () = check "decimal with underscores"
                 (firstTok "1_000_000" = Token.INT_LIT (IntInf.fromInt 1000000))
      val () = check "hex"
                 (firstTok "0xFF" = Token.INT_LIT (IntInf.fromInt 255))
      val () = check "binary"
                 (firstTok "0b1010" = Token.INT_LIT (IntInf.fromInt 10))
      val () = check "octal"
                 (firstTok "0o77" = Token.INT_LIT (IntInf.fromInt 63))

      (* --- lexer: floats --- *)
      val () = print "\n[lexer: floats]\n"
      val () = check "simple float"
                 (firstTok "3.14" = Token.FLOAT_LIT "3.14")
      val () = check "float with exp"
                 (firstTok "1e5" = Token.FLOAT_LIT "1e5")
      val () = check "float with neg exp"
                 (firstTok "2.5e-3" = Token.FLOAT_LIT "2.5e-3")

      (* --- lexer: strings --- *)
      val () = print "\n[lexer: strings]\n"
      val () = check "simple string"
                 (firstTok "\"hello\"" = Token.STRING_LIT "hello")
      val () = check "string with escapes"
                 (firstTok "\"a\\nb\"" = Token.STRING_LIT "a\nb")
      val () = check "empty string"
                 (firstTok "\"\"" = Token.STRING_LIT "")
      val () = check "string with tab"
                 (firstTok "\"a\\tb\"" = Token.STRING_LIT "a\tb")

      (* --- lexer: chars --- *)
      val () = print "\n[lexer: chars]\n"
      val () = check "simple char"
                 (firstTok "'a'" = Token.CHAR_LIT #"a")
      val () = check "escaped char"
                 (firstTok "'\\n'" = Token.CHAR_LIT #"\n")

      (* --- lexer: operators --- *)
      val () = print "\n[lexer: operators]\n"
      val () = check "+" (firstTok "+" = Token.PLUS)
      val () = check "-" (firstTok "-" = Token.MINUS)
      val () = check "*" (firstTok "*" = Token.STAR)
      val () = check "/" (firstTok "/" = Token.SLASH)
      val () = check "%" (firstTok "%" = Token.PERCENT)
      val () = check "==" (firstTok "==" = Token.EQEQ)
      val () = check "!=" (firstTok "!=" = Token.BANGEQ)
      val () = check "<" (firstTok "<" = Token.LT)
      val () = check ">" (firstTok ">" = Token.GT)
      val () = check "<=" (firstTok "<=" = Token.LTEQ)
      val () = check ">=" (firstTok ">=" = Token.GTEQ)
      val () = check "&&" (firstTok "&&" = Token.AMPAMP)
      val () = check "||" (firstTok "||" = Token.PIPEPIPE)
      val () = check "<<" (firstTok "<<" = Token.LTLT)
      val () = check ">>" (firstTok ">>" = Token.GTGT)
      val () = check "<<=" (firstTok "<<=" = Token.LTLTEQ)
      val () = check ">>=" (firstTok ">>=" = Token.GTGTEQ)
      val () = check "->" (firstTok "->" = Token.ARROW)
      val () = check "=>" (firstTok "=>" = Token.FATARROW)
      val () = check ".." (firstTok ".." = Token.DOTDOT)
      val () = check "..=" (firstTok "..=" = Token.DOTDOTEQ)
      val () = check "::" (firstTok "::" = Token.COLONCOLON)
      val () = check "+=" (firstTok "+=" = Token.PLUSEQ)
      val () = check "-=" (firstTok "-=" = Token.MINUSEQ)
      val () = check "&=" (firstTok "&=" = Token.AMPEQ)
      val () = check "|=" (firstTok "|=" = Token.PIPEEQ)
      val () = check "?" (firstTok "?" = Token.QUESTION)

      (* --- lexer: delimiters --- *)
      val () = print "\n[lexer: delimiters]\n"
      val () = check "(" (firstTok "(" = Token.LPAREN)
      val () = check ")" (firstTok ")" = Token.RPAREN)
      val () = check "{" (firstTok "{" = Token.LBRACE)
      val () = check "}" (firstTok "}" = Token.RBRACE)
      val () = check "[" (firstTok "[" = Token.LBRACKET)
      val () = check "]" (firstTok "]" = Token.RBRACKET)
      val () = check "," (firstTok "," = Token.COMMA)
      val () = check ";" (firstTok ";" = Token.SEMICOLON)
      val () = check ":" (firstTok ":" = Token.COLON)
      val () = check "." (firstTok "." = Token.DOT)
      val () = check "#" (firstTok "#" = Token.HASH)

      (* --- lexer: span tracking --- *)
      val () = print "\n[lexer: spans]\n"
      val ts = lex "let x = 42;"
      val (_, sp1) = hd ts
      val () = check "first token starts at col 1"
                 (Span.posCol (Span.spanStart sp1) = 1)
      val (_, sp2) = List.nth (ts, 1)
      val () = check "second token at col 5"
                 (Span.posCol (Span.spanStart sp2) = 5)

      val ts2 = lex "a\nb"
      val (_, spB) = List.nth (ts2, 1)
      val () = check "newline advances line"
                 (Span.posLine (Span.spanStart spB) = 2)

      (* --- lexer: multi-token sequences --- *)
      val () = print "\n[lexer: sequences]\n"
      val ts3 = toks "fn main() -> i32 { return 0; }"
      val () = check "fn decl token count" (length ts3 = 12)
      val () = check "fn decl starts with FN"
                 (hd ts3 = Token.FN)

      val ts4 = toks "x + y * z"
      val () = check "expr tokens"
                 (ts4 = [Token.IDENT "x", Token.PLUS, Token.IDENT "y",
                         Token.STAR, Token.IDENT "z", Token.EOF])

      (* --- parser --- *)
      val () = print "\n[parser]\n"
      fun parseSrc s = Parser.parse (Lexer.tokenize "<test>" s)
      val pMain = parseSrc "fn main() -> i32 { return 0; }"
      val () = check "parse single top-level fn" (length pMain = 1)
      val () = check "parse fn name main"
                 (case pMain of
                    Ast.ItemFn {name, ...} :: _ => name = "main"
                  | _ => false)
      val (_, restExpr) = Parser.parseExpr (Lexer.tokenize "<e>" "1 + 2")
      val () = check "parseExpr leaves EOF"
                 (case restExpr of
                    (Token.EOF, _) :: _ => true
                  | _ => false)

      (* --- name resolution --- *)
      val () = print "\n[resolver]\n"
      fun resolveCatch s =
        (Resolver.resolve (parseSrc s); NONE)
        handle Fail msg => SOME msg
      val () = check "resolve trivial main"
                 (resolveCatch "fn main() -> i32 { return 0; }" = NONE)
      val () = check "resolve let binds return"
                 (resolveCatch
                    "fn main() -> i32 { let x = 0; return x; }" = NONE)
      val () = check "resolve rejects unbound identifier"
                 (case resolveCatch "fn main() -> i32 { return x; }" of
                    SOME m => String.isSubstring "E0300" m
                  | NONE => false)
      val () = check "resolve rejects duplicate fns"
                 (case resolveCatch "fn a() -> unit {} fn a() -> unit {}" of
                    SOME m => String.isSubstring "E0302" m
                  | NONE => false)
      val () = check "resolve rejects wrong call arity"
                 (case resolveCatch
                        "fn f(a: i32) -> i32 { return a; }\n\
                        \fn main() -> i32 { return f(1, 2); }" of
                    SOME m => String.isSubstring "E0307" m
                  | NONE => false)

      (* --- type checker --- *)
      val () = print "\n[checker]\n"
      fun checkCatch s =
        (Checker.check (Resolver.resolve (parseSrc s)); NONE)
        handle Fail msg => SOME msg
      val () = check "checker rejects bool for i32 return"
                 (case checkCatch "fn main() -> i32 { return true; }" of
                    SOME m => String.isSubstring "E0400" m
                  | NONE => false)
      val () = check "checker accepts literal return"
                 (checkCatch "fn main() -> i32 { return 42; }" = NONE)
      val () = check "checker accepts let and return path"
                 (checkCatch
                    "fn main() -> i32 { let x: i32 = 7; return x; }" = NONE)
      val () = check "checker rejects int/bool binop"
                 (case checkCatch "fn main() -> i32 { return 1 + true; }" of
                    SOME m => String.isSubstring "E0400" m
                  | NONE => false)
      val () = check "checker accepts if-else int"
                 (checkCatch
                    "fn main() -> i32 { return if true { 40 } else { 2 }; }" = NONE)
      val () = check "checker accepts let with inferred binop"
                 (checkCatch
                    "fn main() -> i32 { let x = 1 + 2; return x; }" = NONE)
      val () = check "checker accepts call to sibling fn"
                 (checkCatch
                    "fn inc(x: i32) -> i32 { return x + 1; }\n\
                    \fn main() -> i32 { return inc(41); }" = NONE)
      val () = check "checker accepts requires and ensures true"
                 (checkCatch
                    "fn main() -> i32 requires(true) ensures(true) { return 42; }" = NONE)
      val () = check "checker accepts ensures with result"
                 (checkCatch
                    "fn main() -> i32 ensures(result >= 0) { return 7; }" = NONE)
      val () = check "checker rejects result in ensures for unit return"
                 (case checkCatch
                        "fn main() -> unit ensures(result) { }" of
                    SOME m => String.isSubstring "E0520" m
                  | NONE => false)
      val () = check "checker rejects break outside loop"
                 (case checkCatch "fn main() -> unit { break; }" of
                    SOME m => String.isSubstring "E0414" m
                  | NONE => false)
      val () = check "checker rejects for without range"
                 (* Parenthesize iterator: `for x in y {}` parses `y {}` as struct literal. *)
                 (case checkCatch
                        "fn main() -> unit { let y: i32 = 1; for x in (y) { } }" of
                    SOME m => String.isSubstring "E0402" m
                  | NONE => false)
      val () = check "checker accepts while with loop_invariant"
                 (checkCatch
                    "fn main() -> unit { while false loop_invariant(true) { } }" =
                    NONE)
      val () = check "checker accepts for range"
                 (checkCatch
                    "fn main() -> unit { for i in 0..1 { } }" = NONE)

      (* --- end-to-end (parse through C string) --- *)
      val () = print "\n[e2e]\n"
      fun compileToC s =
        let
          val ast = Resolver.resolve (parseSrc s)
          val ast = Checker.check ast
          val ast = ContractAnalyzer.analyze ast
        in
          Codegen.emit (Lowering.lower ast)
        end
      val c42 = compileToC "fn main() -> i32 { return 42; }"
      val () = check "e2e C includes return 42"
                 (String.isSubstring "return 42" c42)
      val () = check "e2e C includes sv0_runtime.h"
                 (String.isSubstring "sv0_runtime.h" c42)
      val cAdd = compileToC "fn main() -> i32 { return 1 + 2; }"
      val () = check "e2e C binop return"
                 (String.isSubstring "+" cAdd andalso String.isSubstring "return" cAdd)
      val cIf = compileToC "fn main() -> i32 { return if true { 40 } else { 2 }; }"
      val () = check "e2e C if-else"
                 (String.isSubstring "if (" cIf andalso String.isSubstring "else" cIf)
      val cCall =
        compileToC
          "fn inc(x: i32) -> i32 { return x + 1; }\n\
          \fn main() -> i32 { return inc(41); }"
      val () = check "e2e C static helper and call"
                 (String.isSubstring "static int inc" cCall
                  andalso String.isSubstring "inc(41)" cCall)
      val cContracts =
        compileToC
          "fn main() -> i32 requires(true) ensures(true) { return 42; }"
      val () = check "e2e C emits sv0_requires and sv0_ensures"
                 (String.isSubstring "sv0_requires" cContracts
                  andalso String.isSubstring "sv0_ensures" cContracts)
      val nl = String.str #"\n"
      val cLoop =
        compileToC
          ("fn main() -> i32 requires(true) ensures(true) {" ^ nl ^
           "  while false loop_invariant(true) {};" ^ nl ^
           "  for i in 0..0 {};" ^ nl ^ "  return 42;" ^ nl ^ "}")
      val () = check "e2e C while and for (for desugars to while)"
                 (String.isSubstring "while (" cLoop)
      val () = check "e2e C loop invariant uses sv0_requires"
                 (String.isSubstring "sv0_requires" cLoop)

      (* --- pipeline stubs --- *)
      val () = print "\n[pipeline]\n"
      val tokens = Lexer.tokenize "<test>" ""
      val ast = Parser.parse tokens
      val () = check "Parser.parse empty" (null ast)
      val ast = Resolver.resolve ast
      val ast = Checker.check ast
      val ast = ContractAnalyzer.analyze ast
      val ir = Lowering.lower ast
      val c = Codegen.emit ir
      val () = check "Codegen.emit produces C"
                 (String.isSubstring "int main" c)
    in
      summary ()
    end
end

val _ = TestRunner.run ()

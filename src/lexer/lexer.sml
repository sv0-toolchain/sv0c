structure Lexer :> LEXER = struct
  type state = {
    source : string,
    file : string,
    pos : int,
    line : int,
    col : int
  }

  fun init file source =
    {source = source, file = file, pos = 0, line = 1, col = 1}

  fun atEnd ({source, pos, ...} : state) =
    pos >= String.size source

  fun peek ({source, pos, ...} : state) =
    if pos >= String.size source then NONE
    else SOME (String.sub (source, pos))

  fun peekAt ({source, pos, ...} : state, offset) =
    let val i = pos + offset
    in if i < 0 orelse i >= String.size source then NONE
       else SOME (String.sub (source, i))
    end

  fun advance ({source, file, pos, line, col} : state) =
    if pos >= String.size source then
      {source = source, file = file, pos = pos, line = line, col = col}
    else
      let val c = String.sub (source, pos)
      in {source = source, file = file, pos = pos + 1,
          line = if c = #"\n" then line + 1 else line,
          col = if c = #"\n" then 1 else col + 1}
      end

  fun makeSpan (s1 : state, s2 : state) =
    Span.span (#file s1,
               Span.pos (#line s1, #col s1, #pos s1),
               Span.pos (#line s2, #col s2, #pos s2))

  fun extract ({source, ...} : state, startOff, endOff) =
    String.substring (source, startOff, endOff - startOff)

  val keywords = [
    ("as", Token.AS), ("assert", Token.ASSERT),
    ("borrows", Token.BORROWS), ("break", Token.BREAK),
    ("const", Token.CONST), ("continue", Token.CONTINUE),
    ("else", Token.ELSE), ("ensures", Token.ENSURES),
    ("enum", Token.ENUM), ("exists", Token.EXISTS),
    ("false", Token.FALSE), ("fn", Token.FN),
    ("for", Token.FOR), ("forall", Token.FORALL),
    ("if", Token.IF), ("impl", Token.IMPL),
    ("in", Token.IN), ("let", Token.LET),
    ("loop", Token.LOOP),
    ("loop_invariant", Token.LOOP_INVARIANT),
    ("match", Token.MATCH), ("module", Token.MODULE),
    ("move", Token.MOVE), ("mut", Token.MUT),
    ("newtype", Token.NEWTYPE),
    ("no_alias", Token.NO_ALIAS), ("old", Token.OLD),
    ("project", Token.PROJECT), ("pub", Token.PUB),
    ("requires", Token.REQUIRES), ("result", Token.RESULT),
    ("return", Token.RETURN), ("self", Token.SELF),
    ("Self", Token.SELF_TYPE),
    ("static", Token.STATIC), ("struct", Token.STRUCT),
    ("trait", Token.TRAIT), ("true", Token.TRUE),
    ("type", Token.TYPE), ("unsafe", Token.UNSAFE),
    ("use", Token.USE), ("where", Token.WHERE),
    ("while", Token.WHILE)
  ]

  fun lookupKeyword s =
    case List.find (fn (k, _) => k = s) keywords of
      SOME (_, tok) => tok
    | NONE => Token.IDENT s

  fun isIdentStart c = Char.isAlpha c orelse c = #"_"
  fun isIdentCont c = Char.isAlphaNum c orelse c = #"_"
  fun isHexDigit c = Char.isHexDigit c
  fun isBinDigit c = c = #"0" orelse c = #"1"
  fun isOctDigit c = c >= #"0" andalso c <= #"7"

  fun skipWS s =
    case peek s of
      SOME c => if Char.isSpace c then skipWS (advance s) else s
    | NONE => s

  fun skipLineComment s =
    case peek s of
      SOME #"\n" => s
    | SOME _ => skipLineComment (advance s)
    | NONE => s

  fun skipBlockComment s depth =
    if depth = 0 then s
    else
      case peek s of
        NONE => s
      | SOME #"/" =>
          (case peekAt (s, 1) of
             SOME #"*" =>
               skipBlockComment (advance (advance s)) (depth + 1)
           | _ => skipBlockComment (advance s) depth)
      | SOME #"*" =>
          (case peekAt (s, 1) of
             SOME #"/" =>
               skipBlockComment (advance (advance s)) (depth - 1)
           | _ => skipBlockComment (advance s) depth)
      | SOME _ => skipBlockComment (advance s) depth

  fun skipAll s =
    let val s = skipWS s
    in case peek s of
         SOME #"/" =>
           (case peekAt (s, 1) of
              SOME #"/" =>
                skipAll (skipLineComment (advance (advance s)))
            | SOME #"*" =>
                skipAll (skipBlockComment (advance (advance s)) 1)
            | _ => s)
       | _ => s
    end

  fun scanIdent s0 =
    let
      fun go s =
        case peek s of
          SOME c => if isIdentCont c then go (advance s) else s
        | NONE => s
      val s1 = go (advance s0)
      val text = extract (s0, #pos s0, #pos s1)
      val tok = lookupKeyword text
    in ((tok, makeSpan (s0, s1)), s1) end

  fun scanDigits pred s =
    let
      fun go s =
        case peek s of
          SOME c =>
            if pred c orelse c = #"_" then go (advance s) else s
        | NONE => s
    in go s end

  fun scanNumber s0 =
    let
      val c0 = valOf (peek s0)
      val s1 = advance s0

      datatype base = Dec | Hex | Bin | Oct

      val (b, s) =
        if c0 = #"0" then
          case peek s1 of
            SOME #"x" => (Hex, advance s1)
          | SOME #"X" => (Hex, advance s1)
          | SOME #"b" => (Bin, advance s1)
          | SOME #"B" => (Bin, advance s1)
          | SOME #"o" => (Oct, advance s1)
          | SOME #"O" => (Oct, advance s1)
          | _ => (Dec, s1)
        else (Dec, s1)

      val digitPred =
        case b of Hex => isHexDigit | Bin => isBinDigit
                | Oct => isOctDigit | _ => Char.isDigit

      val s = scanDigits digitPred s
      val isFloat = ref false

      val s =
        if b = Dec then
          case peek s of
            SOME #"." =>
              (case peekAt (s, 1) of
                 SOME c =>
                   if Char.isDigit c then
                     (isFloat := true;
                      scanDigits Char.isDigit (advance (advance s)))
                   else s
               | NONE => s)
          | _ => s
        else s

      val s =
        if b = Dec then
          case peek s of
            SOME c =>
              if c = #"e" orelse c = #"E" then
                let
                  fun hasExpDigit off =
                    case peekAt (s, off) of
                      SOME d => Char.isDigit d
                    | NONE => false
                  val isExp =
                    hasExpDigit 1 orelse
                    (case peekAt (s, 1) of
                       SOME #"+" => hasExpDigit 2
                     | SOME #"-" => hasExpDigit 2
                     | _ => false)
                in
                  if isExp then
                    let
                      val s = advance s
                      val s = case peek s of
                                SOME #"+" => advance s
                              | SOME #"-" => advance s
                              | _ => s
                    in isFloat := true; scanDigits Char.isDigit s end
                  else s
                end
              else s
          | NONE => s
        else s

      val text = extract (s0, #pos s0, #pos s)
    in
      if !isFloat then
        ((Token.FLOAT_LIT text, makeSpan (s0, s)), s)
      else
        let
          val clean = String.translate
            (fn #"_" => "" | c => String.str c) text
          val prefixLen = case b of Dec => 0 | _ => 2
          val numStr = String.extract (clean, prefixLen, NONE)
          val radix = case b of
                        Hex => StringCvt.HEX | Bin => StringCvt.BIN
                      | Oct => StringCvt.OCT | _ => StringCvt.DEC
          val value =
            case IntInf.scan radix Substring.getc
                   (Substring.full numStr) of
              SOME (n, _) => n
            | NONE => IntInf.fromInt 0
        in
          ((Token.INT_LIT value, makeSpan (s0, s)), s)
        end
    end

  fun scanEscape s =
    case peek s of
      SOME #"n" => (#"\n", advance s)
    | SOME #"t" => (#"\t", advance s)
    | SOME #"r" => (#"\r", advance s)
    | SOME #"\\" => (#"\\", advance s)
    | SOME #"\"" => (#"\"", advance s)
    | SOME #"'" => (#"'", advance s)
    | SOME #"0" => (Char.chr 0, advance s)
    | SOME #"x" =>
        let
          val s = advance s
          val d1 = case peek s of SOME c => c | NONE => #"0"
          val s = advance s
          val d2 = case peek s of SOME c => c | NONE => #"0"
          val s = advance s
          val hex = String.implode [d1, d2]
          val n = case Int.scan StringCvt.HEX Substring.getc
                    (Substring.full hex) of
                    SOME (v, _) => v
                  | NONE => 0
        in (Char.chr (n mod 256), s) end
    | SOME c => (c, advance s)
    | NONE => (#"\\", s)

  fun scanString s0 =
    let
      val s = advance s0
      fun go s acc =
        case peek s of
          NONE =>
            let val text = String.implode (rev acc)
            in ((Token.STRING_LIT text, makeSpan (s0, s)), s) end
        | SOME #"\"" =>
            let val s = advance s
                val text = String.implode (rev acc)
            in ((Token.STRING_LIT text, makeSpan (s0, s)), s) end
        | SOME #"\\" =>
            let val (c, s) = scanEscape (advance s)
            in go s (c :: acc) end
        | SOME c => go (advance s) (c :: acc)
    in go s [] end

  fun scanChar s0 =
    let
      val s = advance s0
      val (c, s) =
        case peek s of
          SOME #"\\" => scanEscape (advance s)
        | SOME c => (c, advance s)
        | NONE => (#" ", s)
      val s = case peek s of SOME #"'" => advance s | _ => s
    in ((Token.CHAR_LIT c, makeSpan (s0, s)), s) end

  fun scanOp s0 =
    let
      val c = valOf (peek s0)
      val s1 = advance s0
      fun tok1 t = ((t, makeSpan (s0, s1)), s1)
      fun tok2 t = let val s2 = advance s1
                   in ((t, makeSpan (s0, s2)), s2) end
      fun tok3 t = let val s2 = advance (advance s1)
                   in ((t, makeSpan (s0, s2)), s2) end
    in case c of
         #"(" => tok1 Token.LPAREN
       | #")" => tok1 Token.RPAREN
       | #"{" => tok1 Token.LBRACE
       | #"}" => tok1 Token.RBRACE
       | #"[" => tok1 Token.LBRACKET
       | #"]" => tok1 Token.RBRACKET
       | #"," => tok1 Token.COMMA
       | #";" => tok1 Token.SEMICOLON
       | #"#" => tok1 Token.HASH
       | #"~" => tok1 Token.TILDE
       | #"?" => tok1 Token.QUESTION
       | #":" =>
           (case peek s1 of
              SOME #":" => tok2 Token.COLONCOLON
            | _ => tok1 Token.COLON)
       | #"." =>
           (case peek s1 of
              SOME #"." =>
                (case peekAt (s1, 1) of
                   SOME #"=" => tok3 Token.DOTDOTEQ
                 | _ => tok2 Token.DOTDOT)
            | _ => tok1 Token.DOT)
       | #"+" =>
           (case peek s1 of
              SOME #"=" => tok2 Token.PLUSEQ
            | _ => tok1 Token.PLUS)
       | #"-" =>
           (case peek s1 of
              SOME #">" => tok2 Token.ARROW
            | SOME #"=" => tok2 Token.MINUSEQ
            | _ => tok1 Token.MINUS)
       | #"*" =>
           (case peek s1 of
              SOME #"=" => tok2 Token.STAREQ
            | _ => tok1 Token.STAR)
       | #"/" =>
           (case peek s1 of
              SOME #"=" => tok2 Token.SLASHEQ
            | _ => tok1 Token.SLASH)
       | #"%" =>
           (case peek s1 of
              SOME #"=" => tok2 Token.PERCENTEQ
            | _ => tok1 Token.PERCENT)
       | #"=" =>
           (case peek s1 of
              SOME #"=" => tok2 Token.EQEQ
            | SOME #">" => tok2 Token.FATARROW
            | _ => tok1 Token.EQ)
       | #"!" =>
           (case peek s1 of
              SOME #"=" => tok2 Token.BANGEQ
            | _ => tok1 Token.BANG)
       | #"<" =>
           (case peek s1 of
              SOME #"=" => tok2 Token.LTEQ
            | SOME #"<" =>
                (case peekAt (s1, 1) of
                   SOME #"=" => tok3 Token.LTLTEQ
                 | _ => tok2 Token.LTLT)
            | _ => tok1 Token.LT)
       | #">" =>
           (case peek s1 of
              SOME #"=" => tok2 Token.GTEQ
            | SOME #">" =>
                (case peekAt (s1, 1) of
                   SOME #"=" => tok3 Token.GTGTEQ
                 | _ => tok2 Token.GTGT)
            | _ => tok1 Token.GT)
       | #"&" =>
           (case peek s1 of
              SOME #"&" => tok2 Token.AMPAMP
            | SOME #"=" => tok2 Token.AMPEQ
            | _ => tok1 Token.AMP)
       | #"|" =>
           (case peek s1 of
              SOME #"|" => tok2 Token.PIPEPIPE
            | SOME #"=" => tok2 Token.PIPEEQ
            | _ => tok1 Token.PIPE)
       | #"^" =>
           (case peek s1 of
              SOME #"=" => tok2 Token.CARETEQ
            | _ => tok1 Token.CARET)
       | _ => tok1 (Token.IDENT (String.str c))
    end

  fun next s =
    let val s = skipAll s
    in
      if atEnd s then ((Token.EOF, makeSpan (s, s)), s)
      else
        let val c = valOf (peek s)
        in
          if isIdentStart c then scanIdent s
          else if Char.isDigit c then scanNumber s
          else if c = #"\"" then scanString s
          else if c = #"'" then scanChar s
          else scanOp s
        end
    end

  fun tokenize file source =
    let
      fun go s acc =
        let val ((tok, sp), s') = next s
        in case tok of
             Token.EOF => rev ((tok, sp) :: acc)
           | _ => go s' ((tok, sp) :: acc)
        end
    in go (init file source) [] end
end

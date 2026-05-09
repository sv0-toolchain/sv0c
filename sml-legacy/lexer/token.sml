structure Token = struct
  datatype token =
    (* literals *)
      INT_LIT of IntInf.int
    | FLOAT_LIT of string
    | STRING_LIT of string
    | CHAR_LIT of char
    | BOOL_LIT of bool
    (* identifiers *)
    | IDENT of string
    (* delimiters *)
    | LPAREN | RPAREN | LBRACE | RBRACE | LBRACKET | RBRACKET
    | COMMA | SEMICOLON | COLON | COLONCOLON | DOT | DOTDOT | DOTDOTEQ
    | ARROW | FATARROW | HASH
    (* operators *)
    | PLUS | MINUS | STAR | SLASH | PERCENT
    | AMP | AMPAMP | PIPE | PIPEPIPE | CARET | TILDE
    | BANG | QUESTION
    | EQ | EQEQ | BANGEQ | LT | GT | LTEQ | GTEQ
    | LTLT | GTGT
    | PLUSEQ | MINUSEQ | STAREQ | SLASHEQ | PERCENTEQ
    | AMPEQ | PIPEEQ | CARETEQ | LTLTEQ | GTGTEQ
    (* 43 keywords *)
    | AS | ASSERT | BORROWS | BREAK | CONST | CONTINUE
    | ELSE | ENSURES | ENUM | EXISTS | FALSE | FN
    | FOR | FORALL | IF | IMPL | IN | LET
    | LOOP | LOOP_INVARIANT | MATCH | MODULE | MOVE
    | MUT | NEWTYPE | NO_ALIAS | OLD | PROJECT | PUB
    | REQUIRES | RESULT | RETURN | SELF | SELF_TYPE
    | STATIC | STRUCT | TRAIT | TRUE | TYPE | UNSAFE | USE
    | WHERE | WHILE
    (* special *)
    | EOF

  type located = token * Span.span

  fun toString (INT_LIT n) = IntInf.toString n
    | toString (FLOAT_LIT s) = s
    | toString (STRING_LIT s) = "\"" ^ String.toString s ^ "\""
    | toString (CHAR_LIT c) = "'" ^ Char.toString c ^ "'"
    | toString (BOOL_LIT b) = Bool.toString b
    | toString (IDENT s) = s
    | toString LPAREN = "(" | toString RPAREN = ")"
    | toString LBRACE = "{" | toString RBRACE = "}"
    | toString LBRACKET = "[" | toString RBRACKET = "]"
    | toString COMMA = "," | toString SEMICOLON = ";"
    | toString COLON = ":" | toString COLONCOLON = "::"
    | toString DOT = "." | toString DOTDOT = ".."
    | toString DOTDOTEQ = "..=" | toString ARROW = "->"
    | toString FATARROW = "=>" | toString HASH = "#"
    | toString PLUS = "+" | toString MINUS = "-"
    | toString STAR = "*" | toString SLASH = "/"
    | toString PERCENT = "%" | toString AMP = "&"
    | toString AMPAMP = "&&" | toString PIPE = "|"
    | toString PIPEPIPE = "||" | toString CARET = "^"
    | toString TILDE = "~" | toString BANG = "!"
    | toString QUESTION = "?" | toString EQ = "="
    | toString EQEQ = "==" | toString BANGEQ = "!="
    | toString LT = "<" | toString GT = ">"
    | toString LTEQ = "<=" | toString GTEQ = ">="
    | toString LTLT = "<<" | toString GTGT = ">>"
    | toString PLUSEQ = "+=" | toString MINUSEQ = "-="
    | toString STAREQ = "*=" | toString SLASHEQ = "/="
    | toString PERCENTEQ = "%=" | toString AMPEQ = "&="
    | toString PIPEEQ = "|=" | toString CARETEQ = "^="
    | toString LTLTEQ = "<<=" | toString GTGTEQ = ">>="
    | toString AS = "as" | toString ASSERT = "assert"
    | toString BORROWS = "borrows" | toString BREAK = "break"
    | toString CONST = "const" | toString CONTINUE = "continue"
    | toString ELSE = "else" | toString ENSURES = "ensures"
    | toString ENUM = "enum" | toString EXISTS = "exists"
    | toString FALSE = "false" | toString FN = "fn"
    | toString FOR = "for" | toString FORALL = "forall"
    | toString IF = "if" | toString IMPL = "impl"
    | toString IN = "in" | toString LET = "let"
    | toString LOOP = "loop"
    | toString LOOP_INVARIANT = "loop_invariant"
    | toString MATCH = "match" | toString MODULE = "module"
    | toString MOVE = "move" | toString MUT = "mut"
    | toString NEWTYPE = "newtype"
    | toString NO_ALIAS = "no_alias" | toString OLD = "old"
    | toString PROJECT = "project" | toString PUB = "pub"
    | toString REQUIRES = "requires" | toString RESULT = "result"
    | toString RETURN = "return" | toString SELF = "self"
    | toString SELF_TYPE = "Self" | toString STATIC = "static"
    | toString STRUCT = "struct" | toString TRAIT = "trait"
    | toString TRUE = "true" | toString TYPE = "type"
    | toString UNSAFE = "unsafe" | toString USE = "use"
    | toString WHERE = "where" | toString WHILE = "while"
    | toString EOF = "<eof>"
end

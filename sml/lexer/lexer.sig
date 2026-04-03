signature LEXER = sig
  val tokenize : string -> string -> Token.located list
end

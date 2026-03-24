signature PARSER = sig
  val parse : Token.located list -> Ast.program
  val parseExpr : Token.located list -> Ast.expr * Token.located list
end

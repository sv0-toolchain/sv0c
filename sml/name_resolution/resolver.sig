signature RESOLVER = sig
  val resolve : Ast.program -> Ast.program

  (* Populated by the last `resolve`: short name -> mangled (`lib__f` or `lib__E::Ok`). *)
  val peekImportAliases : unit -> (string * string) list
end

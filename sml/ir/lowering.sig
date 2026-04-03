signature LOWERING = sig
  (* Must be set after `Resolver.resolve` when `use` imports are present. *)
  val setImportAliases : (string * string) list -> unit

  val lower : Ast.program -> Ir.program
end

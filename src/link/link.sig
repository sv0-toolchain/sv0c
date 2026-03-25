signature LINK = sig
  (* All `*.sv0` in `dir`, sorted by path. Same mangling rules as linkDirectory. *)
  val linkProjectDir : string -> Ast.program

  (* `entryPath` must be a file; uses parent directory as project root. *)
  val linkDirectory : string -> Ast.program

  (* Remove `module` / `use` items before codegen (already resolved). *)
  val stripLinkDirectives : Ast.program -> Ast.program
end

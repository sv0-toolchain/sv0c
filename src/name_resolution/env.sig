(* Environment for name resolution: module-level values/types and nested local frames.
   Locals are newest-first within each frame; frames are innermost-first on the stack. *)
signature ENV = sig
  type env

  val empty : env

  (* Module-level registration (pass 1). Raises Fail on duplicate or illegal shadow of prelude type. *)
  val registerModuleValue : env -> string -> env
  val registerModuleType : env -> string -> env

  (* Record arity for a module-level function (after registerModuleValue for same name). *)
  val registerFnArity : env -> string -> int -> env

  (* Import `localName` as an alias for function/ctor `targetName` (e.g. lib__foo). *)
  val registerValueAlias : env -> string -> string -> env

  (* Import type name `localName` as alias for canonical mangled type `targetName`. *)
  val registerTypeAlias : env -> string -> string -> env

  (* Open/close a block scope (fn body, ExprBlock, etc.). *)
  val enterScope : env -> env
  val exitScope : env -> env

  (* Bind a local name in the innermost frame (shadowing allowed). *)
  val bindLocal : env -> string -> env

  val lookupValue : env -> string -> bool

  (* Arity of a module-level function, if registered. *)
  val lookupFnArity : env -> string -> int option

  (* Single-segment type paths, optional Self (impl bodies), and prelude primitives.
     Type aliases from `use` are resolved to their canonical name before lookup. *)
  val lookupType : env -> {allowSelf : bool} -> Ast.path -> bool
end

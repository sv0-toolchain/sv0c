structure Env :> ENV = struct
  datatype env = Env of {
    modVals : string list,
    fnArity : (string * int) list,
    modTys : string list,
    tyAlias : (string * string) list,
    frames : string list list
  }

  val preludeTypes : string list =
    [ "i8", "i16", "i32", "i64", "i128", "u8", "u16", "u32", "u64", "u128",
      "isize", "usize", "f32", "f64", "bool", "char", "str", "string",
      "String", "unit" ]

  fun isPreludeType t = List.exists (fn p => p = t) preludeTypes

  val empty : env =
    Env { modVals = [], fnArity = [], modTys = [], tyAlias = [], frames = [] }

  fun dupModVal name (Env r) =
    List.exists (fn n => n = name) (#modVals r)

  fun dupModTy name (Env r) =
    List.exists (fn n => n = name) (#modTys r)

  fun hasTyAliasName (Env r) from =
    List.exists (fn (a, _) => String.compare (a, from) = EQUAL) (#tyAlias r)

  fun resolveCanonicalTy (e as Env r) t =
    case List.find (fn (a, _) => String.compare (a, t) = EQUAL) (#tyAlias r) of
      SOME (_, b) =>
        if String.compare (b, t) = EQUAL then t else resolveCanonicalTy e b
    | NONE => t

  fun typeIsRegistered (Env r) t =
    isPreludeType t
    orelse List.exists (fn n => String.compare (n, t) = EQUAL) (#modTys r)

  fun registerModuleValue e name =
    case e of
      Env r =>
        if dupModVal name e then
          raise Fail ("E0302: duplicate module-level value `" ^ name ^ "`")
        else
          Env
            {
              modVals = name :: #modVals r,
              fnArity = #fnArity r,
              modTys = #modTys r,
              tyAlias = #tyAlias r,
              frames = #frames r
            }

  fun registerFnArity e name arity =
    case e of
      Env r =>
        if List.exists (fn (n, _) => String.compare (n, name) = EQUAL) (#fnArity r)
        then
          raise Fail ("E0308: duplicate function arity for `" ^ name ^ "`")
        else
          Env
            {
              modVals = #modVals r,
              fnArity = (name, arity) :: #fnArity r,
              modTys = #modTys r,
              tyAlias = #tyAlias r,
              frames = #frames r
            }

  fun registerModuleType e name =
    case e of
      Env r =>
        if isPreludeType name then
          raise Fail ("E0303: cannot redefine built-in type `" ^ name ^ "`")
        else if dupModTy name e then
          raise Fail ("E0304: duplicate type `" ^ name ^ "`")
        else
          Env
            {
              modVals = #modVals r,
              fnArity = #fnArity r,
              modTys = name :: #modTys r,
              tyAlias = #tyAlias r,
              frames = #frames r
            }

  fun enterScope (Env r) =
    Env
      {
        modVals = #modVals r,
        fnArity = #fnArity r,
        modTys = #modTys r,
        tyAlias = #tyAlias r,
        frames = [] :: #frames r
      }

  fun exitScope (Env r) =
    case #frames r of
      _ :: rest =>
        Env
          {
            modVals = #modVals r,
            fnArity = #fnArity r,
            modTys = #modTys r,
            tyAlias = #tyAlias r,
            frames = rest
          }
    | [] => Env r

  fun bindLocal e name =
    case e of
      Env r =>
        case #frames r of
          [] =>
            Env
              {
                modVals = #modVals r,
                fnArity = #fnArity r,
                modTys = #modTys r,
                tyAlias = #tyAlias r,
                frames = [[name]]
              }
        | frame :: rest =>
            Env
              {
                modVals = #modVals r,
                fnArity = #fnArity r,
                modTys = #modTys r,
                tyAlias = #tyAlias r,
                frames = (name :: frame) :: rest
              }

  fun inFrame name frame =
    let
      fun scan [] = false
        | scan (n :: ns) =
            if String.compare (n, name) = EQUAL then true else scan ns
    in
      scan frame
    end

  fun lookupValue (Env r) name =
    let
      fun inFrames [] = false
        | inFrames (f :: fs) = if inFrame name f then true else inFrames fs
    in
      inFrames (#frames r)
      orelse List.exists (fn n => String.compare (n, name) = EQUAL) (#modVals r)
    end

  fun lookupFnArity (Env r) name =
    let
      fun find [] = NONE
        | find ((n, a) :: rest) =
            if String.compare (n, name) = EQUAL then SOME a else find rest
    in
      find (#fnArity r)
    end

  fun registerValueAlias e localName targetName =
    case e of
      Env r =>
        if dupModVal localName e then
          raise Fail ("E0302: duplicate module-level value `" ^ localName ^ "`")
        else
          case lookupFnArity e targetName of
            SOME ar =>
              registerFnArity (registerModuleValue e localName) localName ar
          | NONE =>
              raise Fail
                ("E0310: unknown import target `" ^ targetName ^ "` for value alias")

  fun registerTypeAlias e from to =
    case e of
      Env r =>
        if isPreludeType from then
          raise Fail ("E0303: cannot use built-in type name as import alias")
        else if hasTyAliasName e from orelse dupModTy from e then
          raise Fail ("E0304: duplicate type `" ^ from ^ "`")
        else if not (typeIsRegistered e to) then
          raise Fail ("E0311: unknown type for import alias `" ^ to ^ "`")
        else
          Env
            {
              modVals = #modVals r,
              fnArity = #fnArity r,
              modTys = #modTys r,
              tyAlias = (from, to) :: #tyAlias r,
              frames = #frames r
            }

  fun lookupType (e as Env r) (ctx : {allowSelf : bool}) path =
    case path of
      [] => false
    | [ "Self" ] => #allowSelf ctx
    | [ t ] =>
        let val canon = resolveCanonicalTy e t
        in
          isPreludeType canon
          orelse List.exists (fn n => String.compare (n, canon) = EQUAL) (#modTys r)
        end
    | _ => false
end

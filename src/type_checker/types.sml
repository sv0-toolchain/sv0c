structure Types = struct
  datatype ty =
      TyUnit
    | TyBool
    | TyChar
    | TyInt of int
    | TyUint of int
    | TyFloat of int
    | TyIsize
    | TyUsize
    | TyString
    | TyRef of ty
    | TyRefMut of ty
    | TyArray of ty * int
    | TySlice of ty
    | TyTuple of ty list
    | TyFn of ty list * ty
    | TyNamed of string * ty list
    | TyVar of int
    | TyError

  val nextVar = ref 0

  fun freshVar () =
    let val n = !nextVar
    in nextVar := n + 1; TyVar n end
end

structure Ir = struct
  type label = string

  datatype value =
      VInt of IntInf.int
    | VFloat of string
    | VBool of bool
    | VString of string
    | VUnit
    | VVar of string

  datatype instr =
      Nop
    | Assign of string * expr
    | Return of value option
    | Branch of value * label * label
    | Jump of label
    | Call of string option * string * value list

  and expr =
      Literal of value
    | Binop of string * value * value
    | Unop of string * value
    | Load of string
    | FieldAccess of value * string
    | IndexAccess of value * value

  type block = {label : label, instrs : instr list}

  type program = block list
end

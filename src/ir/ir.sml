structure Ir = struct
  type label = string

  datatype value =
      VInt of IntInf.int
    | VFloat of string
    | VBool of bool
    | VString of string
    | VUnit
    | VVar of string
    | VMember of value * string

  datatype instr =
      Nop
    | DeclVar of string
    | DeclNamed of string * string
    | Assign of string * expr
    | Store of string * expr
    | StoreField of string * string * expr
    | IfElse of expr * instr list * instr list
    | While of expr * instr list
    | Block of instr list
    | Break
    | Continue
    | Return of value option
    | Branch of value * label * label
    | Jump of label
    | Call of string option * string * value list * string (* dst, fn, args, result C type *)
    | Requires of expr * string (* condition, function name for diagnostics *)
    | Ensures of expr * string

  and expr =
      Literal of value
    | Binop of string * value * value
    | Unop of string * value
    | Load of string
    | FieldAccess of value * string
    | IndexAccess of value * value

  type block =
      { label : label
      , params : (string * string) list (* name, C type *)
      , retCType : string
      , instrs : instr list
      }

  type program = {typedefs : string, blocks : block list}
end

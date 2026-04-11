structure Ast = struct
  type ident = string
  type path = ident list

  datatype literal =
      IntLit of IntInf.int
    | FloatLit of string
    | StringLit of string
    | CharLit of char
    | BoolLit of bool
    | UnitLit

  datatype unop = Neg | Not | BitNot | Deref | Borrow | BorrowMut

  datatype binop =
      Add | Sub | Mul | Div | Rem
    | BitAnd | BitOr | BitXor | Shl | Shr
    | And | Or
    | Eq | Neq | Lt | Gt | Leq | Geq

  datatype ty =
      TyName of path * ty list * Span.span
    | TyRef of ty * Span.span
    | TyRefMut of ty * Span.span
    | TyArray of ty * expr * Span.span
    | TySlice of ty * Span.span
    | TyTuple of ty list * Span.span
    | TyUnit of Span.span
    | TyInfer of Span.span

  and pat =
      PatWild of Span.span
    | PatBind of ident * Span.span
    | PatLit of literal * Span.span
    | PatTuple of pat list * Span.span
    | PatStruct of path * (ident * pat) list * Span.span
    | PatEnum of path * pat list * Span.span
    | PatOr of pat list * Span.span

  and expr =
      ExprLit of literal * Span.span
    | ExprPath of path * Span.span
    | ExprUnop of unop * expr * Span.span
    | ExprBinop of binop * expr * expr * Span.span
    | ExprCall of expr * expr list * Span.span
    | ExprMethodCall of expr * ident * expr list * Span.span
    | ExprField of expr * ident * Span.span
    | ExprTupleField of expr * int * Span.span
    | ExprIndex of expr * expr * Span.span
    | ExprBlock of stmt list * expr option * Span.span
    | ExprIf of expr * expr * expr option * Span.span
    | ExprMatch of expr * arm list * Span.span
    | ExprWhile of expr * expr list * expr * Span.span
    | ExprFor of pat * expr * expr * Span.span
    | ExprLoop of expr * Span.span
    | ExprReturn of expr option * Span.span
    | ExprBreak of expr option * Span.span
    | ExprContinue of Span.span
    | ExprAssign of expr * expr * Span.span
    | ExprAssignOp of binop * expr * expr * Span.span
    | ExprCast of expr * ty * Span.span
    | ExprRange of expr option * expr option * Span.span
    | ExprTry of expr * Span.span
    | ExprAssert of expr * Span.span
    | ExprStruct of path * (ident * expr) list * Span.span
    | ExprTuple of expr list * Span.span
    | ExprArray of expr list * Span.span

  and arm = Arm of pat * expr option * expr

  and stmt =
      StmtLet of pat * bool * ty option * expr option * Span.span
    | StmtExpr of expr * Span.span
    | StmtSemi of expr * Span.span
    | StmtItem of item * Span.span

  and contract =
      Requires of expr * Span.span
    | Ensures of expr * Span.span
    | LoopInvariant of expr * Span.span

  and item =
      ItemFn of {
        name : ident, type_params : ident list,
        params : (pat * ty) list,
        ret : ty option, contracts : contract list,
        body : expr, span : Span.span
      }
    | ItemStruct of {
        name : ident, type_params : ident list,
        fields : (ident * ty) list, span : Span.span
      }
    | ItemEnum of {
        name : ident, type_params : ident list,
        variants : variant list, span : Span.span
      }
    | ItemTrait of {
        name : ident, methods : item list, span : Span.span
      }
    | ItemImpl of {
        trait_path : path option, self_ty : ty,
        items : item list, span : Span.span
      }
    | ItemUse of path * Span.span
    | ItemModule of path * Span.span
    | ItemTypeAlias of ident * ty * Span.span

  and variant =
      VariantUnit of ident * Span.span
    | VariantTuple of ident * ty list * Span.span
    | VariantStruct of ident * (ident * ty) list * Span.span

  type program = item list
end

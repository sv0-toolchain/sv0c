structure Unify :> UNIFY = struct
  fun unifyList (xs : Types.ty list, ys : Types.ty list) : bool =
    case (xs, ys) of
      ([], []) => true
    | (x :: xr, y :: yr) => unify (x, y) andalso unifyList (xr, yr)
    | _ => false

  and unify (a : Types.ty, b : Types.ty) : bool =
    case (a, b) of
      (Types.TyVar i, Types.TyVar j) => i = j
    | (Types.TyVar _, _) => false
    | (_, Types.TyVar _) => false
    | (Types.TyUnit, Types.TyUnit) => true
    | (Types.TyBool, Types.TyBool) => true
    | (Types.TyStruct a, Types.TyStruct b) => a = b
    | (Types.TyEnum a, Types.TyEnum b) => a = b
    | (Types.TyChar, Types.TyChar) => true
    | (Types.TyInt w1, Types.TyInt w2) => w1 = w2
    | (Types.TyUint w1, Types.TyUint w2) => w1 = w2
    | (Types.TyFloat w1, Types.TyFloat w2) => w1 = w2
    | (Types.TyIsize, Types.TyIsize) => true
    | (Types.TyUsize, Types.TyUsize) => true
    | (Types.TyString, Types.TyString) => true
    | (Types.TyRef t1, Types.TyRef t2) => unify (t1, t2)
    | (Types.TyRefMut t1, Types.TyRefMut t2) => unify (t1, t2)
    | (Types.TyArray (t1, n1), Types.TyArray (t2, n2)) =>
        n1 = n2 andalso unify (t1, t2)
    | (Types.TySlice t1, Types.TySlice t2) => unify (t1, t2)
    | (Types.TyTuple ts1, Types.TyTuple ts2) => unifyList (ts1, ts2)
    | (Types.TyFn (a1, r1), Types.TyFn (a2, r2)) =>
        unifyList (a1, a2) andalso unify (r1, r2)
    | (Types.TyNamed (n1, g1), Types.TyNamed (n2, g2)) =>
        n1 = n2 andalso unifyList (g1, g2)
    | (Types.TyError, Types.TyError) => true
    | _ => false
end

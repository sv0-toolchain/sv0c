structure Unify :> UNIFY = struct
  fun unifyList (xs : Types.ty list, ys : Types.ty list) : bool =
    case (xs, ys) of
      ([], []) => true
    | (x :: xr, y :: yr) => unify (x, y) andalso unifyList (xr, yr)
    | _ => false

  and unify (a : Types.ty, b : Types.ty) : bool =
    case (a, b) of
      (Types.TyVar _, _) => true
    | (_, Types.TyVar _) => true
    | (Types.TyUnit, Types.TyUnit) => true
    | (Types.TyBool, Types.TyBool) => true
    | (Types.TyStruct a, Types.TyStruct b) => a = b
    | (Types.TyEnum a, Types.TyEnum b) => a = b
    | (Types.TyChar, Types.TyChar) => true
    (* Width-blind for same-category numeric types (BUGS.md #2,
       sv0-mathlib): checker.sml's own literal synthesis always picks a
       single default width per category (TyInt 32 for every int literal,
       TyFloat 64 for every float literal — see synth's ExprLit cases) with
       no bidirectional "check against the expected type" pass anywhere,
       so a strict per-width equality here made a perfectly legal
       `fn f(x: i64) -> i64 { ... return 0 - 1; ... }` (no i64-typed value
       anywhere in that specific expression to infer the width from) fail
       E0400 even though the literal value is exactly representable at
       every width. Mirrors the native checker (sv0c/lib/checker.sv0),
       which never tracked integer/float width at the type-checker level
       at all (TY_INT/TY_FLOAT are single categories there, with the real
       C width resolved later, downstream, in lowering/emission) — the
       strict per-width check here was actually stricter than the backend
       this SML checker's own VM target is meant to parity-match, not a
       real extra safety margin. Signed vs. unsigned and int vs. float
       still don't unify (those categories stay distinct above/below). *)
    | (Types.TyInt _, Types.TyInt _) => true
    | (Types.TyUint _, Types.TyUint _) => true
    | (Types.TyFloat _, Types.TyFloat _) => true
    | (Types.TyIsize, Types.TyIsize) => true
    | (Types.TyUsize, Types.TyUsize) => true
    | (Types.TyString, Types.TyString) => true
    | (Types.TyRef t1, Types.TyRef t2) => unify (t1, t2)
    | (Types.TyRefMut t1, Types.TyRefMut t2) => unify (t1, t2)
    | (Types.TyRef t1, Types.TyRefMut t2) => unify (t1, t2)
    | (Types.TyRefMut t1, Types.TyRef t2) => unify (t1, t2)
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

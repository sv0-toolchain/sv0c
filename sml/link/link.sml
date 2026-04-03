structure Link :> LINK = struct

  fun isSv0 (name : string) : bool =
    String.isSuffix ".sv0" name

  fun isHidden (name : string) : bool =
    name = "." orelse name = ".."
    orelse (String.size name > 0 andalso String.sub (name, 0) = #".")

  fun walkSv0 (dir : string) (acc : string list) : string list =
    let
      val d = OS.FileSys.openDir dir
      fun loop acc =
        case OS.FileSys.readDir d of
          NONE => (OS.FileSys.closeDir d; acc)
        | SOME name =>
            if isHidden name then loop acc
            else
              let val path = OS.Path.concat (dir, name)
              in
                if OS.FileSys.isDir path then loop (walkSv0 path acc)
                else if isSv0 name then loop (path :: acc)
                else loop acc
              end
    in
      loop acc
    end

  (* All *.sv0 under dir, any depth; skips hidden dirs/files (names starting with '.'). *)
  fun listSv0 (dir : string) : string list =
    ListMergeSort.sort
      (fn (a, b) => case String.compare (a, b) of GREATER => false | _ => true)
      (walkSv0 dir [])

  fun fileStem (path : string) : string =
    case OS.Path.splitBaseExt (OS.Path.file path) of
      {base, ...} => base

  fun splitModule (defaultStem : string) (items : Ast.program) : string * Ast.program =
    case items of
      Ast.ItemModule (p, _) :: rest =>
        (case p of
           [m] => (m, rest)
         | _ =>
             raise Fail "E0320: module path must be a single identifier in this slice")
    | _ => (defaultStem, items)

  fun collectTops (items : Ast.item list) : string list =
    List.concat (
      map (fn it =>
        case it of
          Ast.ItemFn {name, ...} => [name]
        | Ast.ItemStruct {name, ...} => [name]
        | Ast.ItemEnum {name, ...} => [name]
        | Ast.ItemTrait {name, ...} => [name]
        | Ast.ItemTypeAlias (n, _, _) => [n]
        | _ => []) items)

  fun mangle (modId : string) (x : string) : string = modId ^ "__" ^ x

  fun inTops (tops : string list) (a : string) : bool =
    List.exists (fn t => String.compare (t, a) = EQUAL) tops

  fun mapPathSegs (tops : string list) (modId : string) (segs : Ast.path) :
    Ast.path =
    case segs of
      [a, b] =>
        if inTops tops a then [mangle modId a, b] else segs
    | [a] =>
        if inTops tops a then [mangle modId a] else segs
    | _ => segs

  fun mapTy (tops : string list) (modId : string) (t : Ast.ty) : Ast.ty =
    case t of
      Ast.TyName (p, sp) => Ast.TyName (mapPathSegs tops modId p, sp)
    | Ast.TyRef (t2, sp) => Ast.TyRef (mapTy tops modId t2, sp)
    | Ast.TyRefMut (t2, sp) => Ast.TyRefMut (mapTy tops modId t2, sp)
    | Ast.TyArray (t2, e, sp) =>
        Ast.TyArray (mapTy tops modId t2, mapExpr tops modId e, sp)
    | Ast.TySlice (t2, sp) => Ast.TySlice (mapTy tops modId t2, sp)
    | Ast.TyTuple (ts, sp) => Ast.TyTuple (map (mapTy tops modId) ts, sp)
    | Ast.TyUnit sp => Ast.TyUnit sp
    | Ast.TyInfer sp => Ast.TyInfer sp

  and mapPat (tops : string list) (modId : string) (p : Ast.pat) : Ast.pat =
    case p of
      Ast.PatWild sp => Ast.PatWild sp
    | Ast.PatBind (x, sp) => Ast.PatBind (x, sp)
    | Ast.PatLit (lit, sp) => Ast.PatLit (lit, sp)
    | Ast.PatTuple (ps, sp) => Ast.PatTuple (map (mapPat tops modId) ps, sp)
    | Ast.PatStruct (path, fs, sp) =>
        Ast.PatStruct
          (mapPathSegs tops modId path, map (fn (f, p2) => (f, mapPat tops modId p2)) fs, sp)
    | Ast.PatEnum (path, ps, sp) =>
        Ast.PatEnum (mapPathSegs tops modId path, map (mapPat tops modId) ps, sp)
    | Ast.PatOr (ps, sp) => Ast.PatOr (map (mapPat tops modId) ps, sp)

  and mapExpr (tops : string list) (modId : string) (e : Ast.expr) : Ast.expr =
    case e of
      Ast.ExprLit (lit, sp) => Ast.ExprLit (lit, sp)
    | Ast.ExprPath (segs, sp) => Ast.ExprPath (mapPathSegs tops modId segs, sp)
    | Ast.ExprUnop (opr, e1, sp) =>
        Ast.ExprUnop (opr, mapExpr tops modId e1, sp)
    | Ast.ExprBinop (opr, a, b, sp) =>
        Ast.ExprBinop
          (opr, mapExpr tops modId a, mapExpr tops modId b, sp)
    | Ast.ExprCall (f, args, sp) =>
        Ast.ExprCall
          (mapExpr tops modId f, map (mapExpr tops modId) args, sp)
    | Ast.ExprMethodCall (recv, m, args, sp) =>
        Ast.ExprMethodCall
          ( mapExpr tops modId recv
          , m
          , map (mapExpr tops modId) args
          , sp)
    | Ast.ExprField (e1, f, sp) =>
        Ast.ExprField (mapExpr tops modId e1, f, sp)
    | Ast.ExprTupleField (e1, i, sp) =>
        Ast.ExprTupleField (mapExpr tops modId e1, i, sp)
    | Ast.ExprIndex (a, b, sp) =>
        Ast.ExprIndex (mapExpr tops modId a, mapExpr tops modId b, sp)
    | Ast.ExprBlock (stmts, opt, sp) =>
        Ast.ExprBlock (map (mapStmt tops modId) stmts, Option.map (mapExpr tops modId) opt, sp)
    | Ast.ExprIf (c, th, el, sp) =>
        Ast.ExprIf
          ( mapExpr tops modId c
          , mapExpr tops modId th
          , Option.map (mapExpr tops modId) el
          , sp)
    | Ast.ExprMatch (scr, arms, sp) =>
        Ast.ExprMatch
          ( mapExpr tops modId scr
          , map
              (fn Ast.Arm (pat, g, bd) =>
                 Ast.Arm
                   ( mapPat tops modId pat
                   , Option.map (mapExpr tops modId) g
                   , mapExpr tops modId bd)) arms
          , sp)
    | Ast.ExprWhile (cond, invs, body, sp) =>
        Ast.ExprWhile
          ( mapExpr tops modId cond
          , map (mapExpr tops modId) invs
          , mapExpr tops modId body
          , sp)
    | Ast.ExprFor (pat, it, body, sp) =>
        Ast.ExprFor
          ( mapPat tops modId pat
          , mapExpr tops modId it
          , mapExpr tops modId body
          , sp)
    | Ast.ExprLoop (body, sp) => Ast.ExprLoop (mapExpr tops modId body, sp)
    | Ast.ExprReturn (eo, sp) =>
        Ast.ExprReturn (Option.map (mapExpr tops modId) eo, sp)
    | Ast.ExprBreak (eo, sp) =>
        Ast.ExprBreak (Option.map (mapExpr tops modId) eo, sp)
    | Ast.ExprContinue sp => Ast.ExprContinue sp
    | Ast.ExprAssign (a, b, sp) =>
        Ast.ExprAssign (mapExpr tops modId a, mapExpr tops modId b, sp)
    | Ast.ExprAssignOp (opr, a, b, sp) =>
        Ast.ExprAssignOp
          (opr, mapExpr tops modId a, mapExpr tops modId b, sp)
    | Ast.ExprCast (e1, t, sp) =>
        Ast.ExprCast (mapExpr tops modId e1, mapTy tops modId t, sp)
    | Ast.ExprRange (a, b, sp) =>
        Ast.ExprRange
          (Option.map (mapExpr tops modId) a, Option.map (mapExpr tops modId) b, sp)
    | Ast.ExprTry (e1, sp) => Ast.ExprTry (mapExpr tops modId e1, sp)
    | Ast.ExprAssert (e1, sp) => Ast.ExprAssert (mapExpr tops modId e1, sp)
    | Ast.ExprStruct (path, fs, sp) =>
        Ast.ExprStruct
          ( mapPathSegs tops modId path
          , map (fn (f, e2) => (f, mapExpr tops modId e2)) fs
          , sp)
    | Ast.ExprTuple (es, sp) => Ast.ExprTuple (map (mapExpr tops modId) es, sp)
    | Ast.ExprArray (es, sp) => Ast.ExprArray (map (mapExpr tops modId) es, sp)

  and mapStmt (tops : string list) (modId : string) (st : Ast.stmt) : Ast.stmt =
    case st of
      Ast.StmtLet (pat, mut, to, eo, sp) =>
        Ast.StmtLet
          ( mapPat tops modId pat
          , mut
          , Option.map (mapTy tops modId) to
          , Option.map (mapExpr tops modId) eo
          , sp)
    | Ast.StmtExpr (e1, sp) => Ast.StmtExpr (mapExpr tops modId e1, sp)
    | Ast.StmtSemi (e1, sp) => Ast.StmtSemi (mapExpr tops modId e1, sp)
    | Ast.StmtItem (inner, sp) =>
        Ast.StmtItem (mapItem tops modId inner, sp)

  and mapContract (tops : string list) (modId : string) (c : Ast.contract) :
    Ast.contract =
    case c of
      Ast.Requires (e, sp) => Ast.Requires (mapExpr tops modId e, sp)
    | Ast.Ensures (e, sp) => Ast.Ensures (mapExpr tops modId e, sp)
    | Ast.LoopInvariant (e, sp) =>
        Ast.LoopInvariant (mapExpr tops modId e, sp)

  and mapFn (tops : string list) (modId : string)
    (r : {
       name : Ast.ident, params : (Ast.pat * Ast.ty) list,
       ret : Ast.ty option, contracts : Ast.contract list,
       body : Ast.expr, span : Span.span
     }) =
    {
      name =
        if #name r = "main" then "main" else mangle modId (#name r),
      params =
        map (fn (p, t) => (mapPat tops modId p, mapTy tops modId t)) (#params r),
      ret = Option.map (mapTy tops modId) (#ret r),
      contracts = map (mapContract tops modId) (#contracts r),
      body = mapExpr tops modId (#body r),
      span = #span r
    }

  and mapStruct (tops : string list) (modId : string)
    (r : {
       name : Ast.ident, fields : (Ast.ident * Ast.ty) list,
       span : Span.span
     }) =
    {
      name = mangle modId (#name r),
      fields = map (fn (f, t) => (f, mapTy tops modId t)) (#fields r),
      span = #span r
    }

  and mapEnum (tops : string list) (modId : string)
    (r : {
       name : Ast.ident, variants : Ast.variant list, span : Span.span
     }) =
    {
      name = mangle modId (#name r),
      variants =
        map
          (fn v =>
            case v of
              Ast.VariantUnit (n, sp) => Ast.VariantUnit (n, sp)
            | Ast.VariantTuple (n, ts, sp) =>
                Ast.VariantTuple (n, map (mapTy tops modId) ts, sp)
            | Ast.VariantStruct (n, fs, sp) =>
                Ast.VariantStruct
                  (n, map (fn (f, t) => (f, mapTy tops modId t)) fs, sp)) (#variants r),
      span = #span r
    }

  and mapTrait (tops : string list) (modId : string)
    (r : {
       name : Ast.ident, methods : Ast.item list, span : Span.span
     }) =
    {
      name = mangle modId (#name r),
      methods = map (mapItem tops modId) (#methods r),
      span = #span r
    }

  and mapImpl (tops : string list) (modId : string)
    (r : {
       trait_path : Ast.path option, self_ty : Ast.ty, items : Ast.item list,
       span : Span.span
     }) =
    {
      trait_path = Option.map (mapPathSegs tops modId) (#trait_path r),
      self_ty = mapTy tops modId (#self_ty r),
      items = map (mapItem tops modId) (#items r),
      span = #span r
    }

  and mapItem (tops : string list) (modId : string) (it : Ast.item) : Ast.item =
    case it of
      Ast.ItemFn r => Ast.ItemFn (mapFn tops modId r)
    | Ast.ItemStruct r => Ast.ItemStruct (mapStruct tops modId r)
    | Ast.ItemEnum r => Ast.ItemEnum (mapEnum tops modId r)
    | Ast.ItemTrait r => Ast.ItemTrait (mapTrait tops modId r)
    | Ast.ItemImpl r => Ast.ItemImpl (mapImpl tops modId r)
    | Ast.ItemTypeAlias (n, t, sp) =>
        Ast.ItemTypeAlias (mangle modId n, mapTy tops modId t, sp)
    | Ast.ItemUse _ => it
    | Ast.ItemModule _ => it

  fun mapProgramUnit (path : string) (items : Ast.program) : Ast.program =
    let
      val stem = fileStem path
      val (modNm, body) = splitModule stem items
      val tops = collectTops body
    in
      map (mapItem tops modNm) body
    end

  fun parseFile (path : string) : Ast.program =
    let
      val ins = TextIO.openIn path
      val src = TextIO.inputAll ins
      val () = TextIO.closeIn ins
    in
      Parser.parse (Lexer.tokenize path src)
    end

  fun linkProjectDir (dir : string) : Ast.program =
    let
      val paths = listSv0 dir
      val () =
        if null paths then raise Fail ("E0321: no .sv0 files in `" ^ dir ^ "`")
        else ()
    in
      List.concat (map (fn p => mapProgramUnit p (parseFile p)) paths)
    end

  fun linkDirectory (entryPath : string) : Ast.program =
    let
      val dir =
        case OS.Path.dir entryPath of
          "" => "."
        | d => d
    in
      linkProjectDir dir
    end

  fun stripLinkDirectives (prog : Ast.program) : Ast.program =
    List.filter
      (fn it =>
        case it of
          Ast.ItemUse _ => false
        | Ast.ItemModule _ => false
        | _ => true) prog
end

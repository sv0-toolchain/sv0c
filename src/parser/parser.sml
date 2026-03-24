structure Parser :> PARSER = struct
  type ts = Token.located list

  fun tok [] = NONE
    | tok ((t, _) :: _) = SOME t

  fun sp [] = Span.bogus
    | sp ((_, s) :: _) = s

  fun advance [] = []
    | advance (_ :: rest) = rest

  fun expectTok wanted ts =
    case ts of
      [] => raise Fail ("expected " ^ Token.toString wanted ^ " (EOF)")
    | (t, s) :: rest =>
        if t = wanted then rest
        else raise Fail ("expected " ^ Token.toString wanted ^ " got " ^
                         Token.toString t ^ " at " ^ Span.toString s)

  fun merge2 (s1, s2) = Span.merge (s1, s2)

  fun skipOptionalPub ts =
    case ts of
      (Token.PUB, _) :: (Token.LPAREN, _) :: (Token.PROJECT, _) ::
      (Token.RPAREN, _) :: r => r
    | (Token.PUB, _) :: r => r
    | _ => ts

  fun skipOptionalUnsafe ts =
    case ts of
      (Token.UNSAFE, _) :: r => r
    | _ => ts

  fun skipGenericParams ts =
    case ts of
      (Token.LT, _) :: _ =>
        let
          fun depth d [] = raise Fail "unterminated <>"
            | depth d ((Token.LT, _) :: xs) = depth (d + 1) xs
            | depth d ((Token.GT, _) :: xs) =
                if d = 1 then xs else depth (d - 1) xs
            | depth d (_ :: xs) = depth d xs
        in
          depth 1 (tl ts)
        end
    | _ => ts

  fun skipWhereClause ts =
    case ts of
      (Token.WHERE, _) :: rest =>
        let
          fun skip r =
            case r of
              [] => r
            | (Token.LBRACE, _) :: _ => r
            | (Token.FN, _) :: _ => r
            | (Token.SEMICOLON, _) :: xs => xs
            | _ :: xs => skip xs
        in
          skip rest
        end
    | _ => ts

  fun skipAttributes ts =
    case ts of
      (Token.HASH, _) :: (Token.LBRACKET, _) :: rest =>
        let
          fun d n [] = ts
            | d n ((Token.LBRACKET, _) :: xs) = d (n + 1) xs
            | d n ((Token.RBRACKET, _) :: xs) =
                if n = 1 then xs else d (n - 1) xs
            | d n (_ :: xs) = d n xs
        in
          skipAttributes (d 1 rest)
        end
    | _ => ts

  (* --- paths --- *)
  fun parsePath ts : Ast.path * ts =
    case ts of
      [] => raise Fail "expected path"
    | (Token.IDENT i, _) :: rest =>
        let
          fun loop acc r =
            case r of
              (Token.COLONCOLON, _) :: (Token.IDENT j, _) :: r2 =>
                loop (acc @ [j]) r2
            | _ => (acc, r)
        in
          loop [i] rest
        end
    | (_, s) :: _ =>
        raise Fail ("expected identifier in path at " ^ Span.toString s)

  (* --- types --- *)
  fun parseType ts : Ast.ty * ts =
    case ts of
      [] => raise Fail "expected type"
    | (Token.LPAREN, s1) :: rest =>
        (case rest of
           (Token.RPAREN, s2) :: r2 =>
             (Ast.TyUnit (merge2 (s1, s2)), r2)
         | _ =>
             let
               val (t1, r1) = parseType rest
               fun commas acc cur r =
                 case r of
                   (Token.COMMA, _) :: r2 =>
                     (case r2 of
                        (Token.RPAREN, s2) :: r3 =>
                          (Ast.TyTuple (rev (cur :: acc), merge2 (s1, s2)), r3)
                      | _ =>
                          let val (tn, rn) = parseType r2
                          in commas acc tn rn end)
                 | (Token.RPAREN, s2) :: r2 =>
                     if null acc then (cur, r2)
                     else (Ast.TyTuple (rev (cur :: acc), merge2 (s1, s2)), r2)
                 | _ => raise Fail "expected , or ) in tuple type"
             in
               commas [] t1 r1
             end)
    | (Token.AMP, s1) :: rest =>
        (case rest of
           (Token.MUT, _) :: r2 =>
             let val (ty, r3) = parseType r2
             in (Ast.TyRefMut (ty, s1), r3) end
         | _ =>
             let val (ty, r2) = parseType rest
             in (Ast.TyRef (ty, s1), r2) end)
    | (Token.LBRACKET, s1) :: rest =>
        let val (elem, r1) = parseType rest
        in
          case r1 of
            (Token.SEMICOLON, _) :: r2 =>
              (case r2 of
                 (Token.INT_LIT n, sn) :: r3 =>
                   (case r3 of
                      (Token.RBRACKET, s2) :: r4 =>
                        (Ast.TyArray (elem,
                          Ast.ExprLit (Ast.IntLit n, sn), merge2 (s1, s2)), r4)
                    | _ => raise Fail "expected ] after array length")
               | _ =>
                   raise Fail
                     "array length in type must be integer literal (full expr: TODO)")
          | (Token.RBRACKET, s2) :: r2 =>
              (Ast.TySlice (elem, merge2 (s1, s2)), r2)
          | _ => raise Fail "expected ; or ] in array type"
        end
    | (Token.SELF_TYPE, s) :: rest =>
        (case rest of
           (Token.COLONCOLON, _) :: (Token.IDENT field, s2) :: r2 =>
             let
               val r3 =
                 case r2 of
                   (Token.LT, _) :: _ => skipGenericParams r2
                 | _ => r2
             in
               (Ast.TyName (["Self", field], merge2 (s, sp r3)), r3)
             end
         | _ => (Ast.TyName (["Self"], s), rest))
    | (Token.IDENT name, s) :: rest =>
        if List.exists (fn n => n = name)
             ["i8", "i16", "i32", "i64", "i128", "u8", "u16", "u32", "u64",
              "u128", "usize", "isize", "f32", "f64", "bool", "char", "byte",
              "string"] then
          (Ast.TyName ([name], s), rest)
        else
          let
            val (p, r0) = parsePath ts
            val r1 =
              case r0 of
                (Token.LT, _) :: _ => skipGenericParams r0
              | _ => r0
          in
            (Ast.TyName (p, s), r1)
          end
    | (_, s) :: _ =>
        raise Fail ("unexpected token in type: " ^ Span.toString s)

  (* --- patterns --- *)
  fun parsePat ts : Ast.pat * ts =
    let
      val (a, r0) = parsePatAtom ts
      fun ors left r =
        case r of
          (Token.PIPE, _) :: r2 =>
            let val (b, r3) = parsePatAtom r2
            in
              case left of
                Ast.PatOr (ps, spanPat) =>
                  ors (Ast.PatOr (ps @ [b], spanPat)) r3
              | _ => ors (Ast.PatOr ([left, b], merge2 (sp ts, sp r3))) r3
            end
        | _ => (left, r)
    in
      ors a r0
    end

  and parsePatAtom ts : Ast.pat * ts =
    case ts of
      [] => raise Fail "expected pattern"
    | (Token.IDENT "_", s) :: r => (Ast.PatWild s, r)
    | (Token.MUT, s1) :: (Token.IDENT i, s2) :: r =>
        (Ast.PatBind (i, merge2 (s1, s2)), r)
    | (Token.IDENT i, s) :: r => (Ast.PatBind (i, s), r)
    | (Token.MINUS, s0) :: r1 =>
        (case r1 of
           (Token.INT_LIT n, s) :: r2 =>
             (Ast.PatLit (Ast.IntLit (IntInf.~ n), merge2 (s0, s)), r2)
         | (Token.FLOAT_LIT f, s) :: r2 =>
             (Ast.PatLit (Ast.FloatLit ("-" ^ f), merge2 (s0, s)), r2)
         | _ => raise Fail "expected literal after - in pattern")
    | (t, s) :: r1 =>
        (case litFromTok (t, s) of
           SOME lit => (Ast.PatLit (lit, s), r1)
         | NONE =>
             case ts of
               (Token.LPAREN, s1) :: r2 =>
                 let val (p, r3) = parsePat r2
                 in
                   case r3 of
                     (Token.RPAREN, s2) :: r4 =>
                       (case p of
                          Ast.PatTuple _ => (p, r4)
                        | _ =>
                            (case r2 of
                               (Token.RPAREN, _) :: _ =>
                                 (Ast.PatTuple ([], merge2 (s1, s2)), r4)
                             | _ =>
                                 (Ast.PatTuple ([p], merge2 (s1, s2)), r4)))
                   | (Token.COMMA, _) :: _ =>
                       let
                         fun pats acc r =
                           case r of
                             (Token.RPAREN, s2) :: r2 =>
                               (Ast.PatTuple (rev acc, merge2 (s1, s2)), r2)
                           | _ =>
                               let val (pn, rn) = parsePat r
                               in
                                 case rn of
                                   (Token.COMMA, _) :: r2 => pats (pn :: acc) r2
                                 | (Token.RPAREN, s2) :: r2 =>
                                     (Ast.PatTuple (rev (pn :: acc), merge2 (s1, s2)), r2)
                                 | _ => raise Fail "expected ) in tuple pattern"
                               end
                       in
                         pats [p] r3
                       end
                   | _ => raise Fail "expected ) in pattern"
                 end
             | _ =>
                 let val (path, r2) = parsePath ts
                 in
                   case r2 of
                     (Token.LBRACE, s2) :: r3 =>
                       let
                         fun fields r acc =
                           case r of
                             (Token.RBRACKET, _) :: _ => raise Fail "bad struct pat"
                           | (Token.DOTDOT, _) :: (Token.RBRACE, s3) :: r4 =>
                               (rev acc, r4)
                           | (Token.RBRACE, s3) :: r4 => (rev acc, r4)
                           | _ =>
                               let val (fld, r4) =
                                 case r of
                                   (Token.IDENT f, _) :: (Token.COLON, _) :: r5 =>
                                     let val (pp, r6) = parsePat r5
                                     in ((f, pp), r6) end
                                 | (Token.IDENT f, _) :: r5 => ((f, Ast.PatBind (f, sp r)), r5)
                                 | _ => raise Fail "struct pattern field"
                               in
                                 case r4 of
                                   (Token.COMMA, _) :: r5 => fields r5 (fld :: acc)
                                 | (Token.RBRACE, _) :: _ =>
                                     fields r4 (fld :: acc)
                                 | _ => raise Fail "expected , or } in struct pat"
                               end
                       in
                         let val (fs, r4) = fields r3 []
                         in (Ast.PatStruct (path, fs, merge2 (sp ts, sp r4)), r4) end
                       end
                     | (Token.LPAREN, _) :: _ =>
                         let
                           fun args r acc =
                             case r of
                               (Token.RPAREN, s3) :: r4 =>
                                 (Ast.PatEnum (path, rev acc, merge2 (sp ts, s3)), r4)
                             | _ =>
                                 let val (pa, r5) = parsePat r
                                 in
                                   case r5 of
                                     (Token.COMMA, _) :: r6 => args r6 (pa :: acc)
                                   | (Token.RPAREN, s3) :: r6 =>
                                       (Ast.PatEnum (path, rev (pa :: acc), merge2 (sp ts, s3)), r6)
                                   | _ => raise Fail "enum pattern args"
                                 end
                         in
                           args (tl r2) []
                         end
                     | _ => raise Fail "bad pattern after path"
                 end)
  and litFromTok (t, s) : Ast.literal option =
    case t of
      Token.INT_LIT n => SOME (Ast.IntLit n)
    | Token.FLOAT_LIT f => SOME (Ast.FloatLit f)
    | Token.STRING_LIT str => SOME (Ast.StringLit str)
    | Token.CHAR_LIT c => SOME (Ast.CharLit c)
    | Token.TRUE => SOME (Ast.BoolLit true)
    | Token.FALSE => SOME (Ast.BoolLit false)
    | _ => NONE

  fun exprSpan e =
    case e of
      Ast.ExprLit (_, s) => s
    | Ast.ExprPath (_, s) => s
    | Ast.ExprUnop (_, _, s) => s
    | Ast.ExprBinop (_, _, _, s) => s
    | Ast.ExprCall (_, _, s) => s
    | Ast.ExprMethodCall (_, _, _, s) => s
    | Ast.ExprField (_, _, s) => s
    | Ast.ExprTupleField (_, _, s) => s
    | Ast.ExprIndex (_, _, s) => s
    | Ast.ExprBlock (_, _, s) => s
    | Ast.ExprIf (_, _, _, s) => s
    | Ast.ExprMatch (_, _, s) => s
    | Ast.ExprWhile (_, _, s) => s
    | Ast.ExprFor (_, _, _, s) => s
    | Ast.ExprLoop (_, s) => s
    | Ast.ExprReturn (_, s) => s
    | Ast.ExprBreak (_, s) => s
    | Ast.ExprContinue s => s
    | Ast.ExprAssign (_, _, s) => s
    | Ast.ExprAssignOp (_, _, _, s) => s
    | Ast.ExprCast (_, _, s) => s
    | Ast.ExprRange (_, _, s) => s
    | Ast.ExprTry (_, s) => s
    | Ast.ExprAssert (_, s) => s
    | Ast.ExprStruct (_, _, s) => s
    | Ast.ExprTuple (_, s) => s
    | Ast.ExprArray (_, s) => s

  (* --- binops --- *)
  fun binopOf Token.PLUS = SOME Ast.Add
    | binopOf Token.MINUS = SOME Ast.Sub
    | binopOf Token.STAR = SOME Ast.Mul
    | binopOf Token.SLASH = SOME Ast.Div
    | binopOf Token.PERCENT = SOME Ast.Rem
    | binopOf Token.AMP = SOME Ast.BitAnd
    | binopOf Token.PIPE = SOME Ast.BitOr
    | binopOf Token.CARET = SOME Ast.BitXor
    | binopOf Token.LTLT = SOME Ast.Shl
    | binopOf Token.GTGT = SOME Ast.Shr
    | binopOf Token.AMPAMP = SOME Ast.And
    | binopOf Token.PIPEPIPE = SOME Ast.Or
    | binopOf Token.EQEQ = SOME Ast.Eq
    | binopOf Token.BANGEQ = SOME Ast.Neq
    | binopOf Token.LT = SOME Ast.Lt
    | binopOf Token.GT = SOME Ast.Gt
    | binopOf Token.LTEQ = SOME Ast.Leq
    | binopOf Token.GTEQ = SOME Ast.Geq
    | binopOf _ = NONE

  fun assignBinop Token.PLUSEQ = SOME Ast.Add
    | assignBinop Token.MINUSEQ = SOME Ast.Sub
    | assignBinop Token.STAREQ = SOME Ast.Mul
    | assignBinop Token.SLASHEQ = SOME Ast.Div
    | assignBinop Token.PERCENTEQ = SOME Ast.Rem
    | assignBinop Token.AMPEQ = SOME Ast.BitAnd
    | assignBinop Token.PIPEEQ = SOME Ast.BitOr
    | assignBinop Token.CARETEQ = SOME Ast.BitXor
    | assignBinop Token.LTLTEQ = SOME Ast.Shl
    | assignBinop Token.GTGTEQ = SOME Ast.Shr
    | assignBinop _ = NONE

  (* --- expression parsing (Pratt / precedence climbing) --- *)
  fun parseExpr ts = parseRangeExpr ts
  and parseRangeExpr ts =
    let val (left, r0) = parseOrExpr ts
    in
      case r0 of
        (Token.DOTDOT, s1) :: r1 =>
          let val (right, r2) = parseOrExpr r1
          in (Ast.ExprRange (SOME left, SOME right, merge2 (exprSpan left, exprSpan right)), r2) end
      | (Token.DOTDOTEQ, s1) :: r1 =>
          let val (right, r2) = parseOrExpr r1
          in (Ast.ExprRange (SOME left, SOME right, merge2 (exprSpan left, exprSpan right)), r2) end
      | _ => (left, r0)
    end

  and parseOrExpr ts = parseLeftAssoc parseAndExpr (fn Token.PIPEPIPE => true | _ => false) ts
  and parseAndExpr ts = parseLeftAssoc parseCmpExpr (fn Token.AMPAMP => true | _ => false) ts

  and parseCmpExpr ts =
    let val (left, r0) = parseBitOrExpr ts
    in
      case r0 of
        (tok, _) :: r1 =>
          (case binopOf tok of
             SOME b =>
               if tok = Token.EQEQ orelse tok = Token.BANGEQ orelse tok = Token.LT
                  orelse tok = Token.GT orelse tok = Token.LTEQ orelse tok = Token.GTEQ then
                 let val (right, r2) = parseBitOrExpr r1
                 in
                   (Ast.ExprBinop (b, left, right,
                     merge2 (exprSpan left, exprSpan right)), r2)
                 end
               else (left, r0)
           | NONE => (left, r0))
      | _ => (left, r0)
    end

  and parseBitOrExpr ts = parseLeftAssoc parseBitXorExpr (fn Token.PIPE => true | _ => false) ts
  and parseBitXorExpr ts = parseLeftAssoc parseBitAndExpr (fn Token.CARET => true | _ => false) ts
  and parseBitAndExpr ts = parseLeftAssoc parseShiftExpr (fn Token.AMP => true | _ => false) ts
  and parseShiftExpr ts = parseLeftAssoc parseAddExpr (fn Token.LTLT => true | Token.GTGT => true | _ => false) ts
  and parseAddExpr ts = parseLeftAssoc parseMulExpr (fn Token.PLUS => true | Token.MINUS => true | _ => false) ts
  and parseMulExpr ts = parseLeftAssoc parseCastExpr (fn Token.STAR => true | Token.SLASH => true | Token.PERCENT => true | _ => false) ts

  and parseLeftAssoc sub isOp ts =
    let
      val (first, r0) = sub ts
      fun loop left r =
        case r of
          (tok, _) :: r1 =>
            if isOp tok then
              (case binopOf tok of
                 SOME b =>
                   let val (right, r2) = sub r1
                   in
                     loop (Ast.ExprBinop (b, left, right,
                       merge2 (exprSpan left, exprSpan right))) r2
                   end
               | NONE => (left, r))
            else (left, r)
          | _ => (left, r)
    in
      loop first r0
    end

  and parseCastExpr ts =
    let
      val (first, r0) = parseUnaryExpr ts
      fun casts left r =
        case r of
          (Token.AS, _) :: r1 =>
            let val (ty, r2) = parseType r1
            in
              casts (Ast.ExprCast (left, ty, merge2 (exprSpan left, sp r2))) r2
            end
        | _ => (left, r)
    in
      casts first r0
    end

  and parseUnaryExpr ts =
    case ts of
      (Token.MINUS, s) :: r =>
        let val (e, r2) = parseUnaryExpr r
        in (Ast.ExprUnop (Ast.Neg, e, merge2 (s, exprSpan e)), r2) end
    | (Token.BANG, s) :: r =>
        let val (e, r2) = parseUnaryExpr r
        in (Ast.ExprUnop (Ast.Not, e, merge2 (s, exprSpan e)), r2) end
    | (Token.TILDE, s) :: r =>
        let val (e, r2) = parseUnaryExpr r
        in (Ast.ExprUnop (Ast.BitNot, e, merge2 (s, exprSpan e)), r2) end
    | (Token.STAR, s) :: r =>
        let val (e, r2) = parseUnaryExpr r
        in (Ast.ExprUnop (Ast.Deref, e, merge2 (s, exprSpan e)), r2) end
    | (Token.AMP, s) :: r =>
        (case r of
           (Token.MUT, _) :: r2 =>
             let val (e, r3) = parseUnaryExpr r2
             in (Ast.ExprUnop (Ast.BorrowMut, e, merge2 (s, exprSpan e)), r3) end
         | _ =>
             let val (e, r2) = parseUnaryExpr r
             in (Ast.ExprUnop (Ast.Borrow, e, merge2 (s, exprSpan e)), r2) end)
    | _ => parsePostfixExpr ts

  and parsePostfixExpr ts =
    let val (first, r0) = parsePrimaryExpr ts
    fun loop e r =
      case r of
        (Token.LPAREN, s2) :: r1 =>
          let val (args, r2) = parseArgList r1
          in
            case r2 of
              (Token.RPAREN, s3) :: r3 =>
                loop (Ast.ExprCall (e, args, merge2 (exprSpan e, s3))) r3
            | _ => raise Fail "expected )"
          end
      | (Token.DOT, _) :: (Token.IDENT m, s3) :: (Token.LPAREN, _) :: r4 =>
          let val (args, r5) = parseArgList r4
          in
            case r5 of
              (Token.RPAREN, s4) :: r6 =>
                loop (Ast.ExprMethodCall (e, m, args, merge2 (exprSpan e, s4))) r6
            | _ => raise Fail "expected ) after method"
          end
      | (Token.DOT, _) :: (Token.IDENT fld, _) :: r2 =>
          loop (Ast.ExprField (e, fld, merge2 (exprSpan e, sp r2))) r2
      | (Token.DOT, _) :: (Token.INT_LIT n, s3) :: r2 =>
          loop (Ast.ExprTupleField (e, IntInf.toInt n, merge2 (exprSpan e, s3))) r2
      | (Token.LBRACKET, s2) :: r1 =>
          let val (ix, r2) = parseExpr r1
          in
            case r2 of
              (Token.RBRACKET, s3) :: r3 =>
                loop (Ast.ExprIndex (e, ix, merge2 (exprSpan e, s3))) r3
            | _ => raise Fail "expected ]"
          end
      | (Token.QUESTION, s2) :: r1 =>
          loop (Ast.ExprTry (e, merge2 (exprSpan e, s2))) r1
      | _ => (e, r)
    in
      loop first r0
    end

  and parseArgList r =
    case r of
      (Token.RPAREN, _) :: _ => ([], r)
    | _ =>
        let
          val (e1, r1) = parseExpr r
          fun more acc rr =
            case rr of
              (Token.COMMA, _) :: (Token.RPAREN, _) :: r2 => (rev acc, rr)
            | (Token.COMMA, _) :: r2 =>
                let val (en, rn) = parseExpr r2
                in more (en :: acc) rn end
            | _ => (rev acc, rr)
        in
          more [e1] r1
        end

  and parsePrimaryExpr ts =
    case ts of
      [] => raise Fail "expected expression"
    | (t, s) :: r1 =>
        (case litFromTok (t, s) of
           SOME lit => (Ast.ExprLit (lit, s), r1)
         | NONE =>
             case t of
               Token.SELF => (Ast.ExprPath (["self"], s), r1)
             | Token.SELF_TYPE => (Ast.ExprPath (["Self"], s), r1)
             | Token.RESULT => (Ast.ExprPath (["result"], s), r1)
             | Token.IDENT i =>
                 let val (path, r2) = parsePath ts
                 in
                   case r2 of
                     (Token.LBRACE, _) :: r3 =>
                       let val (fields, r4) = parseStructFields r3
                       in
                         (Ast.ExprStruct (path, fields, merge2 (s, sp r4)), r4)
                       end
                   | _ => (Ast.ExprPath (path, s), r2)
                 end
             | Token.LPAREN =>
                 (case r1 of
                    (Token.RPAREN, s2) :: r2 =>
                      (Ast.ExprLit (Ast.UnitLit, merge2 (s, s2)), r2)
                  | _ =>
                      let val (e1, r2) = parseExpr r1
                      in
                        case r2 of
                          (Token.RPAREN, s2) :: r3 =>
                            (Ast.ExprTuple ([e1], merge2 (s, s2)), r3)
                        | (Token.COMMA, _) :: r3 =>
                            let
                              fun tup acc rr =
                                case rr of
                                  (Token.RPAREN, s2) :: r4 =>
                                    (Ast.ExprTuple (rev acc, merge2 (s, s2)), r4)
                                | _ =>
                                    let val (en, rn) = parseExpr rr
                                    in
                                      case rn of
                                        (Token.COMMA, _) :: r5 => tup (en :: acc) r5
                                      | (Token.RPAREN, s2) :: r5 =>
                                          (Ast.ExprTuple (rev (en :: acc), merge2 (s, s2)), r5)
                                      | _ => raise Fail "tuple )"
                                    end
                            in
                              tup [e1] r3
                            end
                        | _ => raise Fail "expected )"
                      end)
             | Token.LBRACKET =>
                 let val (elems, r2) = parseArrayBody r1
                 in
                   case r2 of
                     (Token.RBRACKET, s2) :: r3 =>
                       (Ast.ExprArray (elems, merge2 (s, s2)), r3)
                   | _ => raise Fail "expected ]"
                 end
             | Token.LBRACE => parseBlock ts
             | Token.IF => parseIfExpr r1
             | Token.MATCH => parseMatchExpr r1
             | Token.WHILE => parseWhileExpr r1
             | Token.FOR => parseForExpr r1
             | Token.LOOP => parseLoopExpr r1
             | Token.RETURN => parseReturnExpr s r1
             | Token.UNSAFE =>
                 let val (b, r2) = parseBlock r1
                 in
                   case b of
                     Ast.ExprBlock (ss, eo, sb) =>
                       (Ast.ExprBlock (ss, eo, merge2 (s, sb)), r2)
                   | _ => (b, r2)
                 end
             | Token.ASSERT =>
                 let val (e, r2) = parseExpr r1
                 in (Ast.ExprAssert (e, merge2 (s, exprSpan e)), r2) end
             | Token.OLD =>
                 (case r1 of
                    (Token.LPAREN, _) :: r2 =>
                      let val (e, r3) = parseExpr r2
                      in
                        case r3 of
                          (Token.RPAREN, s2) :: r4 =>
                            (Ast.ExprCall (Ast.ExprPath (["old"], s), [e],
                              merge2 (s, s2)), r4)
                        | _ => raise Fail "expected ) after old("
                      end
                  | _ => raise Fail "expected ( after old")
             | Token.FORALL => parseQuant true s r1
             | Token.EXISTS => parseQuant false s r1
             | Token.NO_ALIAS =>
                 (case r1 of
                    (Token.LPAREN, _) :: r2 =>
                      let val (a, r3) = parseExpr r2
                      in
                        case r3 of
                          (Token.COMMA, _) :: r4 =>
                            let val (b, r5) = parseExpr r4
                            in
                              case r5 of
                                (Token.RPAREN, s2) :: r6 =>
                                  (Ast.ExprCall (
                                     Ast.ExprPath (["no_alias"], s), [a, b],
                                     merge2 (s, s2)), r6)
                              | _ => raise Fail "no_alias"
                            end
                        | _ => raise Fail "no_alias"
                      end
                  | _ => raise Fail "no_alias")
             | _ =>
                 raise Fail ("unexpected token in expression: " ^ Token.toString t))

  and parseStructFields r =
    case r of
      (Token.RBRACE, _) :: r2 => ([], r)
    | _ =>
        let
          fun one rr =
            case rr of
              (Token.DOTDOT, _) :: (Token.IDENT f, _) :: (Token.COLON, _) :: r2 =>
                let val (e, r3) = parseExpr r2
                in ((f, e), r3) end
            | (Token.IDENT f, _) :: (Token.COLON, _) :: r2 =>
                let val (e, r3) = parseExpr r2
                in ((f, e), r3) end
            | (Token.IDENT f, _) :: r2 => ((f, Ast.ExprPath ([f], sp rr)), r2)
            | _ => raise Fail "struct field"
          val (fd, r2) = one r
        in
          case r2 of
            (Token.COMMA, _) :: r3 =>
              let val (rest, r4) = parseStructFields r3
              in (fd :: rest, r4) end
          | (Token.RBRACE, _) :: r3 => ([fd], r2)
          | _ => raise Fail "struct fields"
        end

  and parseArrayBody r =
    case r of
      (Token.RBRACKET, _) :: _ => ([], r)
    | _ =>
        let val (e1, r1) = parseExpr r
        in
          case r1 of
            (Token.SEMICOLON, _) :: r2 =>
              let val (e2, r3) = parseExpr r2
              in ([e1, e2], r3) end
          | (Token.COMMA, _) :: r2 =>
              let
                fun more acc rr =
                  case rr of
                    (Token.RBRACKET, _) :: _ => (rev acc, rr)
                  | _ =>
                      let val (en, rn) = parseExpr rr
                      in
                        case rn of
                          (Token.COMMA, _) :: r3 => more (en :: acc) r3
                        | (Token.RBRACKET, _) :: _ => (rev (en :: acc), rn)
                        | _ => raise Fail "array"
                      end
              in
                more [e1] r2
              end
          | (Token.RBRACKET, _) :: _ => ([e1], r1)
          | _ => raise Fail "array body"
        end

  and parseIfExpr ts =
    let val (cond, r1) = parseExpr ts
        val (thenB, r2) = parseBlock r1
    in
      case r2 of
        (Token.ELSE, _) :: r3 =>
          (case r3 of
             (Token.IF, _) :: r4 =>
               let val (elseE, r5) = parseIfExpr r4
               in
                 (Ast.ExprIf (cond, thenB, SOME elseE,
                   merge2 (exprSpan cond, exprSpan elseE)), r5)
               end
           | _ =>
               let val (elseB, r5) = parseBlock r3
               in
                 (Ast.ExprIf (cond, thenB, SOME elseB,
                   merge2 (exprSpan cond, exprSpan elseB)), r5)
               end)
      | _ =>
          (Ast.ExprIf (cond, thenB, NONE, merge2 (exprSpan cond, exprSpan thenB)), r2)
    end

  and parseMatchExpr ts =
    let val (scr, r1) = parseExpr ts
    in
      case r1 of
        (Token.LBRACE, s1) :: r2 =>
          let val (arms, r3) = parseMatchArms r2
          in
            case r3 of
              (Token.RBRACE, s2) :: r4 =>
                (Ast.ExprMatch (scr, arms, merge2 (s1, s2)), r4)
            | _ => raise Fail "expected } after match"
          end
      | _ => raise Fail "expected { after match expr"
    end

  and parseMatchArms r =
    case r of
      (Token.RBRACE, _) :: _ => ([], r)
    | _ =>
        let val (p, r1) = parsePat r
            val (g, r2) =
              case r1 of
                (Token.IF, _) :: r3 =>
                  let val (ge, r4) = parseExpr r3
                  in (SOME ge, r4) end
              | _ => (NONE, r1)
        in
          case r2 of
            (Token.FATARROW, _) :: r3 =>
              let
                val (body, r4) =
                  case r3 of
                    (Token.LBRACE, _) :: _ =>
                      let val (be, r5) = parseBlock r3
                      in (be, r5) end
                  | _ =>
                      let val (e, r5) = parseExpr r3
                      in (e, r5) end
                val r6 =
                  case r4 of
                    (Token.COMMA, _) :: r5 => r5
                  | _ => r4
                val arm = Ast.Arm (p, g, body)
              in
                case r6 of
                  (Token.RBRACE, _) :: _ => ([arm], r6)
                | _ =>
                    let val (rest, r7) = parseMatchArms r6
                    in (arm :: rest, r7) end
              end
          | _ => raise Fail "expected => in match arm"
        end

  and parseWhileExpr ts =
    let val (cond, r1) = parseExpr ts
        val r2 = parseLoopInv r1
        val (body, r3) = parseBlock r2
    in
      (Ast.ExprWhile (cond, body, merge2 (exprSpan cond, exprSpan body)), r3)
    end

  and parseLoopInv r =
    case r of
      (Token.LOOP_INVARIANT, _) :: (Token.LPAREN, _) :: r1 =>
        let val (_, r2) = parseExpr r1
        in
          case r2 of
            (Token.RPAREN, _) :: r3 => parseLoopInv r3
          | _ => raise Fail "loop_invariant )"
        end
    | _ => r

  and parseForExpr ts =
    let val (p, r1) = parsePat ts
    in
      case r1 of
        (Token.IN, _) :: r2 =>
          let val (it, r3) = parseExpr r2
              val (body, r4) = parseBlock r3
          in
            (Ast.ExprFor (p, it, body, merge2 (sp ts, exprSpan body)), r4)
          end
      | _ => raise Fail "expected in in for"
    end

  and parseLoopExpr ts =
    let val (body, r1) = parseBlock ts
    in (Ast.ExprLoop (body, merge2 (sp ts, exprSpan body)), r1) end

  and parseReturnExpr s ts =
    case ts of
      (Token.SEMICOLON, _) :: _ => (Ast.ExprReturn (NONE, s), ts)
    | (Token.RBRACE, _) :: _ => (Ast.ExprReturn (NONE, s), ts)
    | _ =>
        let val (e, r1) = parseExpr ts
        in (Ast.ExprReturn (SOME e, merge2 (s, exprSpan e)), r1) end

  and parseQuant isForall s ts =
    case ts of
      (Token.LPAREN, _) :: r1 =>
        let val (id, r2) =
              case r1 of
                (Token.IDENT i, _) :: r3 => (i, r3)
              | _ => raise Fail "quant var"
            val r4 =
              case r2 of
                (Token.IN, _) :: r5 => r5
              | _ => raise Fail "in in quant"
            val (dom, r6) = parseExpr r4
        in
          case r6 of
            (Token.COMMA, _) :: r7 =>
              let val (body, r8) = parseExpr r7
              in
                case r8 of
                  (Token.RPAREN, s2) :: r9 =>
                    let
                      val name = if isForall then "forall" else "exists"
                    in
                      (Ast.ExprCall (
                         Ast.ExprPath ([name], s),
                         [Ast.ExprPath ([id], sp r1), dom, body],
                         merge2 (s, s2)), r9)
                    end
                | _ => raise Fail "quant )"
              end
          | _ => raise Fail "quant ,"
        end
    | _ => raise Fail "quant ("


  (* --- block --- *)
  and parseBlock ts =
    case ts of
      (Token.LBRACE, s1) :: r1 =>
        let
          fun loop r stmts =
            case r of
              (Token.RBRACE, s2) :: r2 =>
                (Ast.ExprBlock (rev stmts, NONE, merge2 (s1, s2)), r2)
            | _ =>
                (case tryStmt r of
                   SOME (st, r2) => loop r2 (st :: stmts)
                 | NONE =>
                     let val (e, r2) = parseExpr r
                     in
                       case r2 of
                         (Token.RBRACE, s2) :: r3 =>
                           (Ast.ExprBlock (rev stmts, SOME e, merge2 (s1, s2)), r3)
                       | (Token.SEMICOLON, s2) :: r3 =>
                           loop r3 (Ast.StmtSemi (e, merge2 (exprSpan e, s2)) :: stmts)
                       | _ =>
                           raise Fail ("expected ; or } after expr in block at " ^
                                       Span.toString (sp r2))
                     end)
        in
          loop r1 []
        end
    | _ => raise Fail "expected {"

  and tryStmt r : (Ast.stmt * ts) option =
    case r of
      (Token.LET, s) :: r1 =>
        SOME (parseLetStmt s r1)
    | (Token.BREAK, s) :: r1 =>
        (case r1 of
           (Token.SEMICOLON, s2) :: r2 =>
             SOME (Ast.StmtSemi (Ast.ExprBreak (NONE, s), merge2 (s, s2)), r2)
         | _ =>
             let val (e, r2) = parseExpr r1
             in
               case r2 of
                 (Token.SEMICOLON, s2) :: r3 =>
                   SOME (Ast.StmtSemi (Ast.ExprBreak (SOME e, merge2 (s, exprSpan e)),
                     merge2 (s, s2)), r3)
               | _ => NONE
             end)
    | (Token.CONTINUE, s) :: r1 =>
        (case r1 of
           (Token.SEMICOLON, s2) :: r2 =>
             SOME (Ast.StmtSemi (Ast.ExprContinue s, merge2 (s, s2)), r2)
         | _ => NONE)
    | (Token.ASSERT, s) :: r1 =>
        let val (e, r2) = parseExpr r1
        in
          case r2 of
            (Token.SEMICOLON, s2) :: r3 =>
              SOME (Ast.StmtSemi (Ast.ExprAssert (e, merge2 (s, exprSpan e)),
                merge2 (s, s2)), r3)
          | _ => NONE
        end
    | _ =>
        (case tryAssignStmt r of
           SOME p => SOME p
         | NONE => NONE)

  and parseLetStmt s ts =
    let
      val (isMut, r1) =
        case ts of
          (Token.MUT, _) :: r2 => (true, r2)
        | _ => (false, ts)
      val (p, r2) = parsePat r1
      val (tyOpt, r3) =
        case r2 of
          (Token.COLON, _) :: r4 =>
            let val (ty, r5) = parseType r4
            in (SOME ty, r5) end
        | _ => (NONE, r2)
      val (initOpt, r4) =
        case r3 of
          (Token.EQ, _) :: r5 =>
            let val (e, r6) = parseExpr r5
            in (SOME e, r6) end
        | _ => (NONE, r3)
    in
      case r4 of
        (Token.SEMICOLON, s2) :: r5 =>
          (Ast.StmtLet (p, isMut, tyOpt, initOpt, merge2 (s, s2)), r5)
      | _ => raise Fail "expected ; after let"
    end

  and tryAssignStmt r =
    let
      fun isAssignTok t =
        t = Token.EQ orelse assignBinop t <> NONE
    in
      case tryParseAssignLHS r of
        SOME (lhs, r1) =>
          (case r1 of
             (tok, s2) :: r2 =>
               if tok = Token.EQ then
                 let val (rhs, r3) = parseExpr r2
                 in
                   case r3 of
                     (Token.SEMICOLON, s3) :: r4 =>
                       SOME (Ast.StmtSemi (Ast.ExprAssign (lhs, rhs,
                         merge2 (exprSpan lhs, exprSpan rhs)), merge2 (s2, s3)), r4)
                   | _ => NONE
                 end
               else
                 (case assignBinop tok of
                    SOME b =>
                      let val (rhs, r3) = parseExpr r2
                      in
                        case r3 of
                          (Token.SEMICOLON, s3) :: r4 =>
                            SOME (Ast.StmtSemi (Ast.ExprAssignOp (b, lhs, rhs,
                              merge2 (exprSpan lhs, exprSpan rhs)), merge2 (s2, s3)), r4)
                        | _ => NONE
                      end
                  | NONE => NONE)
           | _ => NONE)
      | NONE => NONE
    end

  and parseAssignTarget ts : Ast.expr * ts =
    case ts of
      (Token.STAR, s) :: r =>
        let val (e, r2) = parseAssignTarget r
        in (Ast.ExprUnop (Ast.Deref, e, s), r2) end
    | (Token.IDENT i, s) :: r =>
        let
          val base = Ast.ExprPath ([i], s)
          fun chain e r =
            case r of
              (Token.DOT, _) :: (Token.IDENT j, _) :: r2 =>
                chain (Ast.ExprField (e, j, merge2 (exprSpan e, sp r2))) r2
            | (Token.DOT, _) :: (Token.INT_LIT n, s2) :: r2 =>
                chain (Ast.ExprTupleField (e, IntInf.toInt n, merge2 (exprSpan e, s2))) r2
            | (Token.LBRACKET, s2) :: r2 =>
                let val (ix, r3) = parseExpr r2
                in
                  case r3 of
                    (Token.RBRACKET, s4) :: r4 =>
                      chain (Ast.ExprIndex (e, ix, merge2 (exprSpan e, s4))) r4
                  | _ => raise Fail "expected ]"
                end
            | _ => (e, r)
        in
          chain base r
        end
    | _ => raise Fail "bad assign target"

  and tryParseAssignLHS ts =
    case ts of
      (Token.STAR, _) :: _ =>
        ((SOME (parseAssignTarget ts)) handle Fail _ => NONE)
    | (Token.IDENT _, _) :: _ =>
        ((let val (lhs, r1) = parseAssignTarget ts
          in
            case r1 of
              (tok, _) :: _ =>
                if tok = Token.EQ orelse assignBinop tok <> NONE then SOME (lhs, r1)
                else NONE
            | _ => NONE
          end) handle Fail _ => NONE)
    | _ => NONE

  fun pathOfTy (Ast.TyName (p, _)) = SOME p
    | pathOfTy _ = NONE

  fun parseFnBody ts : Ast.item * ts =
    let
      val ts0 = skipOptionalUnsafe ts
      val (sFn, r0) =
        case ts0 of (Token.FN, s) :: r => (s, r) | _ => raise Fail "expected fn"
      val (fnName, r1) =
        case r0 of
          (Token.IDENT n, _) :: r => (n, r)
        | _ => raise Fail "expected fn name"
      val r2 = skipGenericParams r1
      val r3 = expectTok Token.LPAREN r2
      val (params, r4) = parseParamList r3
      val r5 = expectTok Token.RPAREN r4
      val (ret, r6) =
        case r5 of
          (Token.ARROW, _) :: r7 =>
            let val (t, r8) = parseType r7
            in (SOME t, r8) end
        | _ => (NONE, r5)
      val r7 = skipWhereClause r6
      val (contracts, r8) = parseContracts r7
      val (body, r9) = parseBlock r8
    in
      (Ast.ItemFn {
         name = fnName, params = params, ret = ret,
         contracts = contracts, body = body,
         span = merge2 (sFn, exprSpan body)
       }, r9)
    end

  and parseParam ts : (Ast.pat * Ast.ty) * ts =
    case ts of
      (Token.MUT, s1) :: (Token.SELF, s2) :: (Token.COLON, _) :: r =>
        let val (ty, r2) = parseType r
        in ((Ast.PatBind ("self", merge2 (s1, s2)), ty), r2) end
    | (Token.SELF, s) :: (Token.COLON, _) :: r =>
        let val (ty, r2) = parseType r
        in ((Ast.PatBind ("self", s), ty), r2) end
    | (Token.MUT, _) :: (Token.IDENT i, s) :: (Token.COLON, _) :: r =>
        let val (ty, r2) = parseType r
        in ((Ast.PatBind (i, s), ty), r2) end
    | (Token.IDENT i, s) :: (Token.COLON, _) :: r =>
        let val (ty, r2) = parseType r
        in ((Ast.PatBind (i, s), ty), r2) end
    | _ => raise Fail "bad parameter"

  and parseParamList ts : (Ast.pat * Ast.ty) list * ts =
    case ts of
      (Token.RPAREN, _) :: _ => ([], ts)
    | _ =>
        let val (p0, r1) = parseParam ts
            fun more acc r =
              case r of
                (Token.COMMA, _) :: r2 =>
                  (case r2 of
                     (Token.RPAREN, _) :: _ => (rev acc, r)
                   | _ =>
                       let val (pn, rn) = parseParam r2
                       in more (pn :: acc) rn end)
              | (Token.RPAREN, _) :: r2 => (rev acc, r)
              | _ => raise Fail "param list"
        in
          more [p0] r1
        end

  and parseContract ts : Ast.contract * ts =
    case ts of
      (Token.REQUIRES, s) :: (Token.LPAREN, _) :: r =>
        let val (e, r2) = parseExpr r
        in
          case r2 of
            (Token.RPAREN, s2) :: r3 =>
              (Ast.Requires (e, merge2 (s, s2)), r3)
          | _ => raise Fail "requires )"
        end
    | (Token.ENSURES, s) :: (Token.LPAREN, _) :: r =>
        let val (e, r2) = parseExpr r
        in
          case r2 of
            (Token.RPAREN, s2) :: r3 =>
              (Ast.Ensures (e, merge2 (s, s2)), r3)
          | _ => raise Fail "ensures )"
        end
    | (Token.LOOP_INVARIANT, s) :: (Token.LPAREN, _) :: r =>
        let val (e, r2) = parseExpr r
        in
          case r2 of
            (Token.RPAREN, s2) :: r3 =>
              (Ast.LoopInvariant (e, merge2 (s, s2)), r3)
          | _ => raise Fail "loop_invariant )"
        end
    | _ => raise Fail "contract"

  and parseContracts ts : Ast.contract list * ts =
    let
      fun loop acc r =
        case r of
          (Token.REQUIRES, _) :: _ =>
            let val (c, r2) = parseContract r
            in loop (c :: acc) r2 end
        | (Token.ENSURES, _) :: _ =>
            let val (c, r2) = parseContract r
            in loop (c :: acc) r2 end
        | (Token.LOOP_INVARIANT, _) :: _ =>
            let val (c, r2) = parseContract r
            in loop (c :: acc) r2 end
        | _ => (rev acc, r)
    in
      loop [] ts
    end

  and parseStructItem ts : Ast.item * ts =
    case ts of
      (Token.STRUCT, s0) :: (Token.IDENT n, _) :: r1 =>
        let val r2 = skipGenericParams r1
            val r3 = expectTok Token.LBRACE r2
            fun fields acc r =
              case r of
                (Token.RBRACE, s1) :: r2 => (rev acc, merge2 (s0, s1), r2)
              | _ =>
                  let val r4 = skipOptionalPub r
                      val (fnm, r5) =
                        case r4 of
                          (Token.IDENT f, _) :: r5 => (f, r5)
                        | _ => raise Fail "field name"
                      val r6 = expectTok Token.COLON r5
                      val (fty, r7) = parseType r6
                      val acc2 = (fnm, fty) :: acc
                  in
                    case r7 of
                      (Token.COMMA, _) :: r8 => fields acc2 r8
                    | (Token.RBRACE, s1) :: r8 =>
                        (rev acc2, merge2 (s0, s1), r8)
                    | _ => raise Fail "struct field"
                  end
            val (fs, spanS, rEnd) = fields [] r3
        in
          (Ast.ItemStruct {name = n, fields = fs, span = spanS}, rEnd)
        end
    | _ => raise Fail "struct"

  and parseEnumItem ts : Ast.item * ts =
    case ts of
      (Token.ENUM, s0) :: (Token.IDENT n, _) :: r1 =>
        let val r2 = skipGenericParams r1
            val r3 = expectTok Token.LBRACE r2
            fun variants acc r =
              case r of
                (Token.RBRACE, s1) :: r2 => (rev acc, merge2 (s0, s1), r2)
              | (Token.IDENT vn, sv) :: r4 =>
                  let val (v, r5) =
                    case r4 of
                      (Token.LPAREN, _) :: r5 =>
                        let
                          fun tyList acc2 rr =
                            case rr of
                              (Token.RPAREN, s2) :: r6 =>
                                (Ast.VariantTuple (vn, rev acc2, merge2 (sv, s2)), r6)
                            | _ =>
                                let val (t, r7) = parseType rr
                                in
                                  case r7 of
                                    (Token.COMMA, _) :: r8 => tyList (t :: acc2) r8
                                  | (Token.RPAREN, s2) :: r8 =>
                                      (Ast.VariantTuple (vn, rev (t :: acc2), merge2 (sv, s2)), r8)
                                  | _ => raise Fail "enum tuple"
                                end
                        in
                          tyList [] r5
                        end
                    | (Token.LBRACE, _) :: r5 =>
                        let
                          fun fld acc2 rr =
                            case rr of
                              (Token.RBRACE, s2) :: r6 =>
                                (Ast.VariantStruct (vn, rev acc2, merge2 (sv, s2)), r6)
                            | _ =>
                                let val (fnm, r7) =
                                      case rr of
                                        (Token.IDENT f, _) :: r7 => (f, r7)
                                      | _ => raise Fail "enum struct field"
                                    val r8 = expectTok Token.COLON r7
                                    val (fty, r9) = parseType r8
                                in
                                  case r9 of
                                    (Token.COMMA, _) :: r10 => fld ((fnm, fty) :: acc2) r10
                                  | (Token.RBRACE, s2) :: r10 =>
                                      (Ast.VariantStruct (vn, rev ((fnm, fty) :: acc2),
                                        merge2 (sv, s2)), r10)
                                  | _ => raise Fail "enum struct"
                                end
                        in
                          fld [] r5
                        end
                    | _ => (Ast.VariantUnit (vn, sv), r4)
                  in
                    case r5 of
                      (Token.COMMA, _) :: r6 => variants (v :: acc) r6
                    | (Token.RBRACE, s1) :: r6 =>
                        (rev (v :: acc), merge2 (s0, s1), r6)
                    | _ => raise Fail "enum variant sep"
                  end
              | _ => raise Fail "enum variant"
            val (vs, spanE, rEnd) = variants [] r3
        in
          (Ast.ItemEnum {name = n, variants = vs, span = spanE}, rEnd)
        end
    | _ => raise Fail "enum"

  and parseTypeAliasItem ts : Ast.item * ts =
    case ts of
      (Token.TYPE, s0) :: (Token.IDENT n, _) :: r1 =>
        let val r2 = skipGenericParams r1
            val r3 = expectTok Token.EQ r2
            val (ty, r4) = parseType r3
        in
          case r4 of
            (Token.SEMICOLON, s1) :: r5 =>
              (Ast.ItemTypeAlias (n, ty, merge2 (s0, s1)), r5)
          | _ => raise Fail "type alias ;"
        end
    | _ => raise Fail "type alias"

  and parseTraitItem ts : Ast.item * ts =
    case ts of
      (Token.TRAIT, s0) :: (Token.IDENT n, _) :: r1 =>
        let val r2 = skipGenericParams r1
            val r3 = expectTok Token.LBRACE r2
            fun methods acc r =
              case r of
                (Token.RBRACE, s1) :: r2 => (rev acc, merge2 (s0, s1), r2)
              | _ =>
                  let val (it, r2) = parseFnBody r
                  in methods (it :: acc) r2 end
            val (ms, spn, rEnd) = methods [] r3
        in
          (Ast.ItemTrait {name = n, methods = ms, span = spn}, rEnd)
        end
    | _ => raise Fail "trait"

  and parseImplItem ts : Ast.item * ts =
    case ts of
      (Token.IMPL, s0) :: r0 =>
        let val r1 = skipGenericParams r0
            val (tr, r2) = parseType r1
            val (selfTy, r3, traitOpt) =
              case r2 of
                (Token.FOR, _) :: r4 =>
                  let val (sty, r5) = parseType r4
                  in (sty, r5, pathOfTy tr) end
              | _ => (tr, r2, NONE)
            val r6 = skipWhereClause r3
            val r7 = expectTok Token.LBRACE r6
            fun implItems acc r =
              case r of
                (Token.RBRACE, s1) :: r2 => (rev acc, merge2 (s0, s1), r2)
              | _ =>
                  let val (it, r2) = parseFnBody r
                  in implItems (it :: acc) r2 end
            val (is, spn, rEnd) = implItems [] r7
        in
          (Ast.ItemImpl {
             trait_path = traitOpt, self_ty = selfTy,
             items = is, span = spn
           }, rEnd)
        end
    | _ => raise Fail "impl"

  and parseUseItem ts : Ast.item * ts =
    case ts of
      (Token.USE, s0) :: r1 =>
        let val (p, r2) = parsePath r1
        in
          case r2 of
            (Token.SEMICOLON, s1) :: r3 =>
              (Ast.ItemUse (p, merge2 (s0, s1)), r3)
          | _ => raise Fail "use ;"
        end
    | _ => raise Fail "use"

  and parseModuleDecl ts : Ast.item option * ts =
    case ts of
      (Token.MODULE, s0) :: r1 =>
        let val (p, r2) = parsePath r1
        in
          case r2 of
            (Token.SEMICOLON, s1) :: r3 =>
              (SOME (Ast.ItemModule (p, merge2 (s0, s1))), r3)
          | _ => raise Fail "module ;"
        end
    | _ => (NONE, ts)

  and parseItem ts : Ast.item * ts =
    let val ts0 = skipOptionalPub (skipAttributes ts)
    in
      case ts0 of
        (Token.MODULE, _) :: _ =>
          (case parseModuleDecl ts0 of
             (SOME it, r) => (it, r)
           | (NONE, _) => raise Fail "module")
      | (Token.USE, _) :: _ => parseUseItem ts0
      | (Token.FN, _) :: _ => parseFnBody ts0
      | (Token.STRUCT, _) :: _ => parseStructItem ts0
      | (Token.ENUM, _) :: _ => parseEnumItem ts0
      | (Token.TRAIT, _) :: _ => parseTraitItem ts0
      | (Token.IMPL, _) :: _ => parseImplItem ts0
      | (Token.TYPE, _) :: _ => parseTypeAliasItem ts0
      | (Token.NEWTYPE, _) :: _ =>
          raise Fail "newtype not implemented"
      | (_, s) :: _ =>
          raise Fail ("unexpected item at " ^ Span.toString s)
      | [] => raise Fail "unexpected EOF in item"
    end

  val parse =
    fn tokens =>
      let
        fun loop acc r =
          case r of
            (Token.EOF, _) :: _ => rev acc
          | [] => rev acc
          | _ =>
              let val r0 = skipAttributes r
              in
                case r0 of
                  (Token.EOF, _) :: _ => rev acc
                | _ =>
                    (case parseModuleDecl r0 of
                       (SOME m, r1) => loop (m :: acc) r1
                     | (NONE, _) =>
                         let val (it, r1) = parseItem r0
                         in loop (it :: acc) r1 end)
                    handle Fail _ => rev acc
              end
      in
        loop [] tokens
      end
end

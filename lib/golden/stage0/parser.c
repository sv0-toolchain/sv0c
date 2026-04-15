#include "sv0_runtime.h"

static int TK_INT_LIT(void);
static int TK_FLOAT_LIT(void);
static int TK_STRING_LIT(void);
static int TK_CHAR_LIT(void);
static int TK_BOOL_LIT(void);
static int TK_IDENT(void);
static int TK_LPAREN(void);
static int TK_RPAREN(void);
static int TK_LBRACE(void);
static int TK_RBRACE(void);
static int TK_LBRACKET(void);
static int TK_RBRACKET(void);
static int TK_COMMA(void);
static int TK_SEMICOLON(void);
static int TK_COLON(void);
static int TK_COLONCOLON(void);
static int TK_DOT(void);
static int TK_DOTDOT(void);
static int TK_ARROW(void);
static int TK_FATARROW(void);
static int TK_HASH(void);
static int TK_PLUS(void);
static int TK_MINUS(void);
static int TK_STAR(void);
static int TK_SLASH(void);
static int TK_PERCENT(void);
static int TK_AMP(void);
static int TK_AMPAMP(void);
static int TK_PIPE(void);
static int TK_PIPEPIPE(void);
static int TK_CARET(void);
static int TK_BANG(void);
static int TK_EQ(void);
static int TK_EQEQ(void);
static int TK_BANGEQ(void);
static int TK_LT(void);
static int TK_GT(void);
static int TK_LTEQ(void);
static int TK_GTEQ(void);
static int TK_LTLT(void);
static int TK_GTGT(void);
static int TK_PLUSEQ(void);
static int TK_MINUSEQ(void);
static int TK_STAREQ(void);
static int TK_SLASHEQ(void);
static int TK_PERCENTEQ(void);
static int TK_AMPEQ(void);
static int TK_PIPEEQ(void);
static int TK_CARETEQ(void);
static int TK_LTLTEQ(void);
static int TK_GTGTEQ(void);
static int TK_PUB(void);
static int TK_PROJECT(void);
static int TK_UNSAFE(void);
static int TK_FN(void);
static int TK_WHERE(void);
static int TK_TRUE(void);
static int TK_FALSE(void);
static int TK_EOF(void);
static int TK_SELF_TYPE(void);
static int TK_MUT(void);
static int TK_LET(void);
static int TK_IF(void);
static int TK_ELSE(void);
static int TK_MATCH(void);
static int TK_RETURN(void);
static int TK_WHILE(void);
static int TK_LOOP(void);
static int TK_FOR(void);
static int TK_BREAK(void);
static int TK_CONTINUE(void);
static int TK_STRUCT(void);
static int TK_ENUM(void);
static int TK_IMPL(void);
static int TK_TRAIT(void);
static int TK_TYPE(void);
static int TK_USE(void);
static int TK_AS(void);
static int TK_IN(void);
static int ps_peek(int tags, int pos);
static int ps_at_end(int tags, int pos);
static int ps_start(int starts, int pos);
static int ps_end(int ends, int pos);
static int ps_advance(int pos);
static int ps_expect(int tags, int pos, int expected);
static int ps_match(int tags, int pos, int expected);
static const char* ps_tok_text(const char* source, int starts, int ends, int pos);
static int parse_path(int tags, int pos, int path_out);
static int parse_generic_params(int tags, int pos, int names_out);
static int ps_skip_optional_pub(int tags, int pos);
static int ps_skip_optional_unsafe(int tags, int pos);
static int ps_skip_generic_params(int tags, int pos);
static int ps_skip_where_clause(int tags, int pos);
static int ps_skip_attributes(int tags, int pos);
static int parse_use_item(int tags, int pos, int path_out);
static int parse_module_decl(int tags, int pos, int path_out);
static int AST_ADD(void);
static int AST_SUB(void);
static int AST_MUL(void);
static int AST_DIV(void);
static int AST_REM(void);
static int AST_EQ(void);
static int AST_NEQ(void);
static int AST_LT(void);
static int AST_GT(void);
static int AST_LEQ(void);
static int AST_GEQ(void);
static int AST_AND(void);
static int AST_OR(void);
static int AST_BITAND(void);
static int AST_BITOR(void);
static int AST_BITXOR(void);
static int AST_SHL(void);
static int AST_SHR(void);
static int binop_of(int tok_tag);
static int has_binop(int tok_tag);
static int assign_binop(int tok_tag);
static int has_assign_binop(int tok_tag);
static int is_compound_assign(int tok_tag);
static int is_assign_tok(int tok_tag);
static int path_of_ty_valid(int ty_tag);
static int LIT_TAG_INT(void);
static int LIT_TAG_FLOAT(void);
static int LIT_TAG_STRING(void);
static int LIT_TAG_CHAR(void);
static int LIT_TAG_BOOL(void);
static int LIT_TAG_NONE(void);
static int lit_from_tok(int tok_tag);
static int is_literal_tok(int tok_tag);
static int expr_has_span(int expr_tag);
static int is_pub_token(int tok_tag);
static int is_unsafe_token(int tok_tag);
static int is_where_token(int tok_tag);
static int is_hash_token(int tok_tag);
static int is_lt_token(int tok_tag);
static int is_gt_token(int tok_tag);
static int generic_params_update_depth(int tok_tag, int depth);
static int generic_params_done(int depth);
static int where_clause_stop(int tok_tag);
static int attr_bracket_update(int tok_tag, int depth);
static int ty_is_name(int ty_tag);
static int span_min(int a, int b);
static int span_max(int a, int b);
static int span_merge_start(int s1, int s2);
static int span_merge_end(int e1, int e2);
static int is_cmp_token(int tok_tag);
static int is_additive_token(int tok_tag);
static int is_multiplicative_token(int tok_tag);
static int is_shift_token(int tok_tag);
static int is_unary_prefix(int tok_tag);
static int prec_of_binop(int binop_tag);
static int is_right_assoc(int binop_tag);
static int next_prec(int current, int right_assoc);
static int binop_is_comparison(int binop_tag);
static int binop_is_arithmetic(int binop_tag);
static int binop_is_logical(int binop_tag);
static int is_builtin_type_name(const char* source, int starts, int ends, int pos);
static int parse_type(int tags, int starts, int ends, const char* source, int pos, int ty_tags, int ty_data1, int ty_data2, int ty_data3, int path_pool);
static int path_pattern_from(int tags, int starts, int ends, const char* source, int pos, int pat_tags, int pat_data1, int pat_data2, int pat_data3, int path_pool, int pp_start, int pp_count);
static int parse_pat_atom(int tags, int starts, int ends, const char* source, int pos, int pat_tags, int pat_data1, int pat_data2, int pat_data3, int path_pool);
static int parse_pat(int tags, int starts, int ends, const char* source, int pos, int pat_tags, int pat_data1, int pat_data2, int pat_data3, int path_pool);
static int parse_arg_list(int tags, int starts, int ends, const char* source, int pos, int et, int ed1, int ed2, int ed3, int ed4, int pp);
static int parse_struct_fields(int tags, int starts, int ends, const char* source, int pos, int et, int ed1, int ed2, int ed3, int ed4, int pp, int sf_names);
static int parse_primary_expr(int tags, int starts, int ends, const char* source, int pos, int et, int ed1, int ed2, int ed3, int ed4, int pp, int sf_names, int allow_struct);
static int parse_postfix_expr(int tags, int starts, int ends, const char* source, int pos, int et, int ed1, int ed2, int ed3, int ed4, int pp, int sf_names, int allow_struct);
static int parse_unary_expr(int tags, int starts, int ends, const char* source, int pos, int et, int ed1, int ed2, int ed3, int ed4, int pp, int sf_names, int allow_struct);
static int parse_cast_expr(int tags, int starts, int ends, const char* source, int pos, int et, int ed1, int ed2, int ed3, int ed4, int pp, int sf_names, int ty_tags, int ty_d1, int ty_d2, int ty_d3, int allow_struct);
static int parse_mul_expr(int tags, int starts, int ends, const char* source, int pos, int et, int ed1, int ed2, int ed3, int ed4, int pp, int sf_names, int ty_tags, int ty_d1, int ty_d2, int ty_d3, int allow_struct);
static int parse_add_expr(int tags, int starts, int ends, const char* source, int pos, int et, int ed1, int ed2, int ed3, int ed4, int pp, int sf_names, int ty_tags, int ty_d1, int ty_d2, int ty_d3, int allow_struct);
static int parse_shift_expr(int tags, int starts, int ends, const char* source, int pos, int et, int ed1, int ed2, int ed3, int ed4, int pp, int sf_names, int ty_tags, int ty_d1, int ty_d2, int ty_d3, int allow_struct);
static int parse_bit_and_expr(int tags, int starts, int ends, const char* source, int pos, int et, int ed1, int ed2, int ed3, int ed4, int pp, int sf_names, int ty_tags, int ty_d1, int ty_d2, int ty_d3, int allow_struct);
static int parse_bit_xor_expr(int tags, int starts, int ends, const char* source, int pos, int et, int ed1, int ed2, int ed3, int ed4, int pp, int sf_names, int ty_tags, int ty_d1, int ty_d2, int ty_d3, int allow_struct);
static int parse_bit_or_expr(int tags, int starts, int ends, const char* source, int pos, int et, int ed1, int ed2, int ed3, int ed4, int pp, int sf_names, int ty_tags, int ty_d1, int ty_d2, int ty_d3, int allow_struct);
static int parse_cmp_expr(int tags, int starts, int ends, const char* source, int pos, int et, int ed1, int ed2, int ed3, int ed4, int pp, int sf_names, int ty_tags, int ty_d1, int ty_d2, int ty_d3, int allow_struct);
static int parse_and_expr(int tags, int starts, int ends, const char* source, int pos, int et, int ed1, int ed2, int ed3, int ed4, int pp, int sf_names, int ty_tags, int ty_d1, int ty_d2, int ty_d3, int allow_struct);
static int parse_or_expr(int tags, int starts, int ends, const char* source, int pos, int et, int ed1, int ed2, int ed3, int ed4, int pp, int sf_names, int ty_tags, int ty_d1, int ty_d2, int ty_d3, int allow_struct);
static int parse_range_expr(int tags, int starts, int ends, const char* source, int pos, int et, int ed1, int ed2, int ed3, int ed4, int pp, int sf_names, int ty_tags, int ty_d1, int ty_d2, int ty_d3, int allow_struct);
static int parse_expr(int tags, int starts, int ends, const char* source, int pos, int et, int ed1, int ed2, int ed3, int ed4, int pp, int allow_struct);
static int test_binop_of(void);
static int test_assign_binop(void);
static int test_is_assign_tok(void);
static int test_path_of_ty(void);
static int test_lit_from_tok(void);
static int test_expr_span(void);
static int test_skip_helpers(void);
static int test_span_merge(void);
static int test_precedence(void);
static int test_ty_name(void);
static int test_pratt_prec(void);
static int test_attr_bracket(void);
static int test_is_builtin_type_name(void);
static int test_parse_type_unit(void);
static int test_parse_type_builtin(void);
static int test_parse_type_ref(void);
static int test_parse_type_ref_mut(void);
static int test_parse_type_slice(void);
static int test_parse_type_array(void);
static int test_parse_type_generic(void);
static int test_parse_type_paren(void);
static int test_parse_pat_wild(void);
static int test_parse_pat_bind(void);
static int test_parse_pat_bind_mut(void);
static int test_parse_pat_lit(void);
static int test_parse_pat_neg_lit(void);
static int test_parse_pat_unit(void);
static int test_parse_pat_tuple(void);
static int test_parse_pat_enum(void);
static int test_parse_pat_or(void);
static int test_parse_pat_paren(void);
static int test_parse_pat_bare_enum(void);
static int test_ps_nav(void);
static int test_parse_path(void);
static int test_ps_skip(void);
static int test_parse_generic_params(void);
static int test_parse_use_item(void);
static int test_parse_module_decl(void);
static int test_parse_expr_lit(void);
static int test_parse_expr_path(void);
static int test_parse_expr_binop(void);
static int test_parse_expr_unary(void);
static int test_parse_expr_call(void);
static int test_parse_expr_field(void);
static int test_parse_expr_tuple(void);
static int test_parse_expr_array(void);
static int test_parse_expr_cast(void);
static int test_parse_expr_precedence(void);
static int test_parse_expr_index(void);

static int TK_INT_LIT(void) {
  return 0;
}

static int TK_FLOAT_LIT(void) {
  return 1;
}

static int TK_STRING_LIT(void) {
  return 2;
}

static int TK_CHAR_LIT(void) {
  return 3;
}

static int TK_BOOL_LIT(void) {
  return 4;
}

static int TK_IDENT(void) {
  return 5;
}

static int TK_LPAREN(void) {
  return 6;
}

static int TK_RPAREN(void) {
  return 7;
}

static int TK_LBRACE(void) {
  return 8;
}

static int TK_RBRACE(void) {
  return 9;
}

static int TK_LBRACKET(void) {
  return 10;
}

static int TK_RBRACKET(void) {
  return 11;
}

static int TK_COMMA(void) {
  return 12;
}

static int TK_SEMICOLON(void) {
  return 13;
}

static int TK_COLON(void) {
  return 14;
}

static int TK_COLONCOLON(void) {
  return 15;
}

static int TK_DOT(void) {
  return 16;
}

static int TK_DOTDOT(void) {
  return 17;
}

static int TK_ARROW(void) {
  return 19;
}

static int TK_FATARROW(void) {
  return 20;
}

static int TK_HASH(void) {
  return 21;
}

static int TK_PLUS(void) {
  return 22;
}

static int TK_MINUS(void) {
  return 23;
}

static int TK_STAR(void) {
  return 24;
}

static int TK_SLASH(void) {
  return 25;
}

static int TK_PERCENT(void) {
  return 26;
}

static int TK_AMP(void) {
  return 27;
}

static int TK_AMPAMP(void) {
  return 28;
}

static int TK_PIPE(void) {
  return 29;
}

static int TK_PIPEPIPE(void) {
  return 30;
}

static int TK_CARET(void) {
  return 31;
}

static int TK_BANG(void) {
  return 33;
}

static int TK_EQ(void) {
  return 35;
}

static int TK_EQEQ(void) {
  return 36;
}

static int TK_BANGEQ(void) {
  return 37;
}

static int TK_LT(void) {
  return 38;
}

static int TK_GT(void) {
  return 39;
}

static int TK_LTEQ(void) {
  return 40;
}

static int TK_GTEQ(void) {
  return 41;
}

static int TK_LTLT(void) {
  return 42;
}

static int TK_GTGT(void) {
  return 43;
}

static int TK_PLUSEQ(void) {
  return 44;
}

static int TK_MINUSEQ(void) {
  return 45;
}

static int TK_STAREQ(void) {
  return 46;
}

static int TK_SLASHEQ(void) {
  return 47;
}

static int TK_PERCENTEQ(void) {
  return 48;
}

static int TK_AMPEQ(void) {
  return 49;
}

static int TK_PIPEEQ(void) {
  return 50;
}

static int TK_CARETEQ(void) {
  return 51;
}

static int TK_LTLTEQ(void) {
  return 52;
}

static int TK_GTGTEQ(void) {
  return 53;
}

static int TK_PUB(void) {
  return 82;
}

static int TK_PROJECT(void) {
  return 81;
}

static int TK_UNSAFE(void) {
  return 93;
}

static int TK_FN(void) {
  return 65;
}

static int TK_WHERE(void) {
  return 95;
}

static int TK_TRUE(void) {
  return 91;
}

static int TK_FALSE(void) {
  return 64;
}

static int TK_EOF(void) {
  return 97;
}

static int TK_SELF_TYPE(void) {
  return 87;
}

static int TK_MUT(void) {
  return 77;
}

static int TK_LET(void) {
  return 71;
}

static int TK_IF(void) {
  return 68;
}

static int TK_ELSE(void) {
  return 60;
}

static int TK_MATCH(void) {
  return 74;
}

static int TK_RETURN(void) {
  return 85;
}

static int TK_WHILE(void) {
  return 96;
}

static int TK_LOOP(void) {
  return 72;
}

static int TK_FOR(void) {
  return 66;
}

static int TK_BREAK(void) {
  return 57;
}

static int TK_CONTINUE(void) {
  return 59;
}

static int TK_STRUCT(void) {
  return 89;
}

static int TK_ENUM(void) {
  return 62;
}

static int TK_IMPL(void) {
  return 69;
}

static int TK_TRAIT(void) {
  return 90;
}

static int TK_TYPE(void) {
  return 92;
}

static int TK_USE(void) {
  return 94;
}

static int TK_AS(void) {
  return 54;
}

static int TK_IN(void) {
  return 70;
}

static int ps_peek(int tags, int pos) {
  int _sv0t0 = sv0_vec_len(tags);
  if ((pos >= _sv0t0)) {
    return 97;
  } else {
  }
  int _sv0t1 = sv0_vec_get(tags, pos);
  return _sv0t1;
}

static int ps_at_end(int tags, int pos) {
  int _sv0t0 = ps_peek(tags, pos);
  int _sv0t1 = (_sv0t0 == 97);
  return _sv0t1;
}

static int ps_start(int starts, int pos) {
  int _sv0t0 = sv0_vec_get(starts, pos);
  return _sv0t0;
}

static int ps_end(int ends, int pos) {
  int _sv0t0 = sv0_vec_get(ends, pos);
  return _sv0t0;
}

static int ps_advance(int pos) {
  int _sv0t0 = (pos + 1);
  return _sv0t0;
}

static int ps_expect(int tags, int pos, int expected) {
  int _sv0t0 = ps_peek(tags, pos);
  if ((_sv0t0 != expected)) {
    int _sv0t1 = (0 - 1);
    return _sv0t1;
  } else {
  }
  int _sv0t2 = (pos + 1);
  return _sv0t2;
}

static int ps_match(int tags, int pos, int expected) {
  int _sv0t0 = ps_peek(tags, pos);
  if ((_sv0t0 == expected)) {
    int _sv0t1 = (pos + 1);
    return _sv0t1;
  } else {
  }
  return pos;
}

static const char* ps_tok_text(const char* source, int starts, int ends, int pos) {
  int _sv0t0 = sv0_vec_get(starts, pos);
  int s = _sv0t0;
  int _sv0t1 = sv0_vec_get(ends, pos);
  int e = _sv0t1;
  int _sv0t2 = (e - s);
  const char* _sv0t3 = sv0_string_substr(source, s, _sv0t2);
  return _sv0t3;
}

static int parse_path(int tags, int pos, int path_out) {
  int _sv0t0 = ps_peek(tags, pos);
  int tag = _sv0t0;
  if ((tag != 5)) {
    int _sv0t1 = (0 - 1);
    return _sv0t1;
  } else {
  }
  sv0_vec_push(path_out, pos);
  int p = (pos + 1);
  int done = 0;
  while ((done != 1)) {
    int _sv0t2 = ps_peek(tags, p);
    if ((_sv0t2 == 15)) {
      int _sv0t3 = (p + 1);
      int _sv0t4 = ps_peek(tags, _sv0t3);
      int next = _sv0t4;
      if ((next == 5)) {
        int _sv0t5 = (p + 1);
        sv0_vec_push(path_out, _sv0t5);
        p = (p + 2);
      } else {
        done = 1;
      }
    } else {
      done = 1;
    }
  }
  return p;
}

static int parse_generic_params(int tags, int pos, int names_out) {
  int _sv0t0 = ps_peek(tags, pos);
  if ((_sv0t0 != 38)) {
    return pos;
  } else {
  }
  int p = (pos + 1);
  int done = 0;
  while ((done != 1)) {
    int _sv0t1 = ps_peek(tags, p);
    int t = _sv0t1;
    if ((t == 39)) {
      p = (p + 1);
      done = 1;
    } else {
      if ((t == 97)) {
        done = 1;
      } else {
        if ((t == 5)) {
          sv0_vec_push(names_out, p);
          p = (p + 1);
          int _sv0t2 = ps_peek(tags, p);
          int after = _sv0t2;
          if ((after == 14)) {
            p = (p + 1);
            int skipping = 1;
            while (skipping) {
              int _sv0t3 = ps_peek(tags, p);
              int st = _sv0t3;
              if ((st == 12)) {
                skipping = 0;
              } else {
                if ((st == 39)) {
                  skipping = 0;
                } else {
                  if ((st == 97)) {
                    skipping = 0;
                  } else {
                    p = (p + 1);
                  }
                }
              }
            }
          } else {
          }
          int _sv0t4 = ps_peek(tags, p);
          if ((_sv0t4 == 12)) {
            p = (p + 1);
          } else {
          }
        } else {
          p = (p + 1);
        }
      }
    }
  }
  return p;
}

static int ps_skip_optional_pub(int tags, int pos) {
  int _sv0t0 = ps_peek(tags, pos);
  if ((_sv0t0 != 82)) {
    return pos;
  } else {
  }
  int _sv0t1 = (pos + 1);
  int _sv0t2 = ps_peek(tags, _sv0t1);
  if ((_sv0t2 == 6)) {
    int _sv0t3 = (pos + 2);
    int _sv0t4 = ps_peek(tags, _sv0t3);
    if ((_sv0t4 == 81)) {
      int _sv0t5 = (pos + 3);
      int _sv0t6 = ps_peek(tags, _sv0t5);
      if ((_sv0t6 == 7)) {
        int _sv0t7 = (pos + 4);
        return _sv0t7;
      } else {
      }
    } else {
    }
  } else {
  }
  int _sv0t8 = (pos + 1);
  return _sv0t8;
}

static int ps_skip_optional_unsafe(int tags, int pos) {
  int _sv0t0 = ps_peek(tags, pos);
  if ((_sv0t0 == 93)) {
    int _sv0t1 = (pos + 1);
    return _sv0t1;
  } else {
  }
  return pos;
}

static int ps_skip_generic_params(int tags, int pos) {
  int _sv0t0 = ps_peek(tags, pos);
  if ((_sv0t0 != 38)) {
    return pos;
  } else {
  }
  int p = (pos + 1);
  int depth = 1;
  while ((depth > 0)) {
    int _sv0t1 = ps_peek(tags, p);
    int t = _sv0t1;
    if ((t == 97)) {
      return p;
    } else {
    }
    if ((t == 38)) {
      depth = (depth + 1);
    } else {
    }
    if ((t == 39)) {
      depth = (depth - 1);
    } else {
    }
    p = (p + 1);
  }
  return p;
}

static int ps_skip_where_clause(int tags, int pos) {
  int _sv0t0 = ps_peek(tags, pos);
  if ((_sv0t0 != 95)) {
    return pos;
  } else {
  }
  int p = (pos + 1);
  int done = 0;
  while ((done != 1)) {
    int _sv0t1 = ps_peek(tags, p);
    int t = _sv0t1;
    if ((t == 97)) {
      return p;
    } else {
    }
    if ((t == 8)) {
      return p;
    } else {
    }
    if ((t == 65)) {
      return p;
    } else {
    }
    if ((t == 13)) {
      int _sv0t2 = (p + 1);
      return _sv0t2;
    } else {
    }
    p = (p + 1);
  }
  return p;
}

static int ps_skip_attributes(int tags, int pos) {
  int p = pos;
  int scanning = 1;
  while (scanning) {
    int _sv0t0 = ps_peek(tags, p);
    if ((_sv0t0 == 21)) {
      int _sv0t1 = (p + 1);
      int _sv0t2 = ps_peek(tags, _sv0t1);
      if ((_sv0t2 == 10)) {
        p = (p + 2);
        int depth = 1;
        while ((depth > 0)) {
          int _sv0t3 = ps_peek(tags, p);
          int t = _sv0t3;
          if ((t == 97)) {
            return p;
          } else {
          }
          if ((t == 10)) {
            depth = (depth + 1);
          } else {
          }
          if ((t == 11)) {
            depth = (depth - 1);
          } else {
          }
          p = (p + 1);
        }
      } else {
        scanning = 0;
      }
    } else {
      scanning = 0;
    }
  }
  return p;
}

static int parse_use_item(int tags, int pos, int path_out) {
  int _sv0t0 = ps_peek(tags, pos);
  if ((_sv0t0 != 94)) {
    int _sv0t1 = (0 - 1);
    return _sv0t1;
  } else {
  }
  int p1 = (pos + 1);
  int _sv0t2 = parse_path(tags, p1, path_out);
  int p2 = _sv0t2;
  if ((p2 < 0)) {
    int _sv0t3 = (0 - 1);
    return _sv0t3;
  } else {
  }
  int _sv0t4 = ps_expect(tags, p2, 13);
  int p3 = _sv0t4;
  return p3;
}

static int parse_module_decl(int tags, int pos, int path_out) {
  int _sv0t0 = ps_peek(tags, pos);
  if ((_sv0t0 != 75)) {
    return pos;
  } else {
  }
  int p1 = (pos + 1);
  int _sv0t1 = parse_path(tags, p1, path_out);
  int p2 = _sv0t1;
  if ((p2 < 0)) {
    int _sv0t2 = (0 - 1);
    return _sv0t2;
  } else {
  }
  int _sv0t3 = ps_expect(tags, p2, 13);
  int p3 = _sv0t3;
  return p3;
}

static int AST_ADD(void) {
  return 0;
}

static int AST_SUB(void) {
  return 1;
}

static int AST_MUL(void) {
  return 2;
}

static int AST_DIV(void) {
  return 3;
}

static int AST_REM(void) {
  return 4;
}

static int AST_EQ(void) {
  return 5;
}

static int AST_NEQ(void) {
  return 6;
}

static int AST_LT(void) {
  return 7;
}

static int AST_GT(void) {
  return 8;
}

static int AST_LEQ(void) {
  return 9;
}

static int AST_GEQ(void) {
  return 10;
}

static int AST_AND(void) {
  return 11;
}

static int AST_OR(void) {
  return 12;
}

static int AST_BITAND(void) {
  return 13;
}

static int AST_BITOR(void) {
  return 14;
}

static int AST_BITXOR(void) {
  return 15;
}

static int AST_SHL(void) {
  return 16;
}

static int AST_SHR(void) {
  return 17;
}

static int binop_of(int tok_tag) {
  if ((tok_tag == 22)) {
    return 0;
  } else {
  }
  if ((tok_tag == 23)) {
    return 1;
  } else {
  }
  if ((tok_tag == 24)) {
    return 2;
  } else {
  }
  if ((tok_tag == 25)) {
    return 3;
  } else {
  }
  if ((tok_tag == 26)) {
    return 4;
  } else {
  }
  if ((tok_tag == 27)) {
    return 13;
  } else {
  }
  if ((tok_tag == 29)) {
    return 14;
  } else {
  }
  if ((tok_tag == 31)) {
    return 15;
  } else {
  }
  if ((tok_tag == 42)) {
    return 16;
  } else {
  }
  if ((tok_tag == 43)) {
    return 17;
  } else {
  }
  if ((tok_tag == 28)) {
    return 11;
  } else {
  }
  if ((tok_tag == 30)) {
    return 12;
  } else {
  }
  if ((tok_tag == 36)) {
    return 5;
  } else {
  }
  if ((tok_tag == 37)) {
    return 6;
  } else {
  }
  if ((tok_tag == 38)) {
    return 7;
  } else {
  }
  if ((tok_tag == 39)) {
    return 8;
  } else {
  }
  if ((tok_tag == 40)) {
    return 9;
  } else {
  }
  if ((tok_tag == 41)) {
    return 10;
  } else {
  }
  int _sv0t0 = (0 - 1);
  return _sv0t0;
}

static int has_binop(int tok_tag) {
  int _sv0t0 = binop_of(tok_tag);
  int _sv0t1 = (_sv0t0 >= 0);
  return _sv0t1;
}

static int assign_binop(int tok_tag) {
  if ((tok_tag == 44)) {
    return 0;
  } else {
  }
  if ((tok_tag == 45)) {
    return 1;
  } else {
  }
  if ((tok_tag == 46)) {
    return 2;
  } else {
  }
  if ((tok_tag == 47)) {
    return 3;
  } else {
  }
  if ((tok_tag == 48)) {
    return 4;
  } else {
  }
  if ((tok_tag == 49)) {
    return 13;
  } else {
  }
  if ((tok_tag == 50)) {
    return 14;
  } else {
  }
  if ((tok_tag == 51)) {
    return 15;
  } else {
  }
  if ((tok_tag == 52)) {
    return 16;
  } else {
  }
  if ((tok_tag == 53)) {
    return 17;
  } else {
  }
  int _sv0t0 = (0 - 1);
  return _sv0t0;
}

static int has_assign_binop(int tok_tag) {
  int _sv0t0 = assign_binop(tok_tag);
  int _sv0t1 = (_sv0t0 >= 0);
  return _sv0t1;
}

static int is_compound_assign(int tok_tag) {
  if ((tok_tag >= 44)) {
    if ((tok_tag <= 53)) {
      return 1;
    } else {
    }
  } else {
  }
  return 0;
}

static int is_assign_tok(int tok_tag) {
  if ((tok_tag == 35)) {
    return 1;
  } else {
  }
  int _sv0t0 = has_assign_binop(tok_tag);
  return _sv0t0;
}

static int path_of_ty_valid(int ty_tag) {
  int _sv0t0 = (ty_tag == 0);
  return _sv0t0;
}

static int LIT_TAG_INT(void) {
  return 0;
}

static int LIT_TAG_FLOAT(void) {
  return 1;
}

static int LIT_TAG_STRING(void) {
  return 3;
}

static int LIT_TAG_CHAR(void) {
  return 4;
}

static int LIT_TAG_BOOL(void) {
  return 5;
}

static int LIT_TAG_NONE(void) {
  int _sv0t0 = (0 - 1);
  return _sv0t0;
}

static int lit_from_tok(int tok_tag) {
  if ((tok_tag == 0)) {
    return 0;
  } else {
  }
  if ((tok_tag == 1)) {
    return 1;
  } else {
  }
  if ((tok_tag == 2)) {
    return 3;
  } else {
  }
  if ((tok_tag == 3)) {
    return 4;
  } else {
  }
  if ((tok_tag == 91)) {
    return 5;
  } else {
  }
  if ((tok_tag == 64)) {
    return 5;
  } else {
  }
  int _sv0t0 = (0 - 1);
  return _sv0t0;
}

static int is_literal_tok(int tok_tag) {
  int _sv0t0 = lit_from_tok(tok_tag);
  int _sv0t1 = (_sv0t0 >= 0);
  return _sv0t1;
}

static int expr_has_span(int expr_tag) {
  if ((expr_tag >= 0)) {
    if ((expr_tag <= 27)) {
      return 1;
    } else {
    }
  } else {
  }
  return 0;
}

static int is_pub_token(int tok_tag) {
  int _sv0t0 = (tok_tag == 82);
  return _sv0t0;
}

static int is_unsafe_token(int tok_tag) {
  int _sv0t0 = (tok_tag == 93);
  return _sv0t0;
}

static int is_where_token(int tok_tag) {
  int _sv0t0 = (tok_tag == 95);
  return _sv0t0;
}

static int is_hash_token(int tok_tag) {
  int _sv0t0 = (tok_tag == 21);
  return _sv0t0;
}

static int is_lt_token(int tok_tag) {
  int _sv0t0 = (tok_tag == 38);
  return _sv0t0;
}

static int is_gt_token(int tok_tag) {
  int _sv0t0 = (tok_tag == 39);
  return _sv0t0;
}

static int generic_params_update_depth(int tok_tag, int depth) {
  if ((tok_tag == 38)) {
    int _sv0t0 = (depth + 1);
    return _sv0t0;
  } else {
  }
  if ((tok_tag == 39)) {
    int _sv0t1 = (depth - 1);
    return _sv0t1;
  } else {
  }
  return depth;
}

static int generic_params_done(int depth) {
  int _sv0t0 = (depth <= 0);
  return _sv0t0;
}

static int where_clause_stop(int tok_tag) {
  if ((tok_tag == 8)) {
    return 1;
  } else {
  }
  if ((tok_tag == 65)) {
    return 1;
  } else {
  }
  if ((tok_tag == 13)) {
    return 1;
  } else {
  }
  if ((tok_tag == 97)) {
    return 1;
  } else {
  }
  return 0;
}

static int attr_bracket_update(int tok_tag, int depth) {
  if ((tok_tag == 10)) {
    int _sv0t0 = (depth + 1);
    return _sv0t0;
  } else {
  }
  if ((tok_tag == 11)) {
    int _sv0t1 = (depth - 1);
    return _sv0t1;
  } else {
  }
  return depth;
}

static int ty_is_name(int ty_tag) {
  int _sv0t0 = (ty_tag == 0);
  return _sv0t0;
}

static int span_min(int a, int b) {
  if ((a <= b)) {
    return a;
  } else {
  }
  return b;
}

static int span_max(int a, int b) {
  if ((a >= b)) {
    return a;
  } else {
  }
  return b;
}

static int span_merge_start(int s1, int s2) {
  int _sv0t0 = span_min(s1, s2);
  return _sv0t0;
}

static int span_merge_end(int e1, int e2) {
  int _sv0t0 = span_max(e1, e2);
  return _sv0t0;
}

static int is_cmp_token(int tok_tag) {
  if ((tok_tag == 36)) {
    return 1;
  } else {
  }
  if ((tok_tag == 37)) {
    return 1;
  } else {
  }
  if ((tok_tag == 38)) {
    return 1;
  } else {
  }
  if ((tok_tag == 39)) {
    return 1;
  } else {
  }
  if ((tok_tag == 40)) {
    return 1;
  } else {
  }
  if ((tok_tag == 41)) {
    return 1;
  } else {
  }
  return 0;
}

static int is_additive_token(int tok_tag) {
  if ((tok_tag == 22)) {
    return 1;
  } else {
  }
  if ((tok_tag == 23)) {
    return 1;
  } else {
  }
  return 0;
}

static int is_multiplicative_token(int tok_tag) {
  if ((tok_tag == 24)) {
    return 1;
  } else {
  }
  if ((tok_tag == 25)) {
    return 1;
  } else {
  }
  if ((tok_tag == 26)) {
    return 1;
  } else {
  }
  return 0;
}

static int is_shift_token(int tok_tag) {
  if ((tok_tag == 42)) {
    return 1;
  } else {
  }
  if ((tok_tag == 43)) {
    return 1;
  } else {
  }
  return 0;
}

static int is_unary_prefix(int tok_tag) {
  if ((tok_tag == 23)) {
    return 1;
  } else {
  }
  if ((tok_tag == 33)) {
    return 1;
  } else {
  }
  if ((tok_tag == 27)) {
    return 1;
  } else {
  }
  if ((tok_tag == 24)) {
    return 1;
  } else {
  }
  return 0;
}

static int prec_of_binop(int binop_tag) {
  if ((binop_tag == 12)) {
    return 1;
  } else {
  }
  if ((binop_tag == 11)) {
    return 2;
  } else {
  }
  if ((binop_tag == 5)) {
    return 3;
  } else {
  }
  if ((binop_tag == 6)) {
    return 3;
  } else {
  }
  if ((binop_tag == 7)) {
    return 3;
  } else {
  }
  if ((binop_tag == 8)) {
    return 3;
  } else {
  }
  if ((binop_tag == 9)) {
    return 3;
  } else {
  }
  if ((binop_tag == 10)) {
    return 3;
  } else {
  }
  if ((binop_tag == 14)) {
    return 4;
  } else {
  }
  if ((binop_tag == 15)) {
    return 5;
  } else {
  }
  if ((binop_tag == 13)) {
    return 6;
  } else {
  }
  if ((binop_tag == 16)) {
    return 7;
  } else {
  }
  if ((binop_tag == 17)) {
    return 7;
  } else {
  }
  if ((binop_tag == 0)) {
    return 8;
  } else {
  }
  if ((binop_tag == 1)) {
    return 8;
  } else {
  }
  if ((binop_tag == 2)) {
    return 9;
  } else {
  }
  if ((binop_tag == 3)) {
    return 9;
  } else {
  }
  if ((binop_tag == 4)) {
    return 9;
  } else {
  }
  return 0;
}

static int is_right_assoc(int binop_tag) {
  return 0;
}

static int next_prec(int current, int right_assoc) {
  if (right_assoc) {
    return current;
  } else {
  }
  int _sv0t0 = (current + 1);
  return _sv0t0;
}

static int binop_is_comparison(int binop_tag) {
  int _sv0t0 = prec_of_binop(binop_tag);
  int _sv0t1 = (_sv0t0 == 3);
  return _sv0t1;
}

static int binop_is_arithmetic(int binop_tag) {
  int _sv0t0 = prec_of_binop(binop_tag);
  int p = _sv0t0;
  if ((p == 8)) {
    return 1;
  } else {
  }
  if ((p == 9)) {
    return 1;
  } else {
  }
  return 0;
}

static int binop_is_logical(int binop_tag) {
  int _sv0t0 = prec_of_binop(binop_tag);
  int p = _sv0t0;
  if ((p == 1)) {
    return 1;
  } else {
  }
  if ((p == 2)) {
    return 1;
  } else {
  }
  return 0;
}

static int is_builtin_type_name(const char* source, int starts, int ends, int pos) {
  const char* _sv0t0 = ps_tok_text(source, starts, ends, pos);
  const char* txt;
  txt = _sv0t0;
  int _sv0t1 = sv0_string_eq(txt, "i8");
  if (_sv0t1) {
    return 1;
  } else {
  }
  int _sv0t2 = sv0_string_eq(txt, "i16");
  if (_sv0t2) {
    return 1;
  } else {
  }
  int _sv0t3 = sv0_string_eq(txt, "i32");
  if (_sv0t3) {
    return 1;
  } else {
  }
  int _sv0t4 = sv0_string_eq(txt, "i64");
  if (_sv0t4) {
    return 1;
  } else {
  }
  int _sv0t5 = sv0_string_eq(txt, "i128");
  if (_sv0t5) {
    return 1;
  } else {
  }
  int _sv0t6 = sv0_string_eq(txt, "u8");
  if (_sv0t6) {
    return 1;
  } else {
  }
  int _sv0t7 = sv0_string_eq(txt, "u16");
  if (_sv0t7) {
    return 1;
  } else {
  }
  int _sv0t8 = sv0_string_eq(txt, "u32");
  if (_sv0t8) {
    return 1;
  } else {
  }
  int _sv0t9 = sv0_string_eq(txt, "u64");
  if (_sv0t9) {
    return 1;
  } else {
  }
  int _sv0t10 = sv0_string_eq(txt, "u128");
  if (_sv0t10) {
    return 1;
  } else {
  }
  int _sv0t11 = sv0_string_eq(txt, "usize");
  if (_sv0t11) {
    return 1;
  } else {
  }
  int _sv0t12 = sv0_string_eq(txt, "isize");
  if (_sv0t12) {
    return 1;
  } else {
  }
  int _sv0t13 = sv0_string_eq(txt, "f32");
  if (_sv0t13) {
    return 1;
  } else {
  }
  int _sv0t14 = sv0_string_eq(txt, "f64");
  if (_sv0t14) {
    return 1;
  } else {
  }
  int _sv0t15 = sv0_string_eq(txt, "bool");
  if (_sv0t15) {
    return 1;
  } else {
  }
  int _sv0t16 = sv0_string_eq(txt, "char");
  if (_sv0t16) {
    return 1;
  } else {
  }
  int _sv0t17 = sv0_string_eq(txt, "byte");
  if (_sv0t17) {
    return 1;
  } else {
  }
  int _sv0t18 = sv0_string_eq(txt, "string");
  if (_sv0t18) {
    return 1;
  } else {
  }
  return 0;
}

static int parse_type(int tags, int starts, int ends, const char* source, int pos, int ty_tags, int ty_data1, int ty_data2, int ty_data3, int path_pool) {
  int _sv0t0 = ps_peek(tags, pos);
  int t = _sv0t0;
  if ((t == 6)) {
    int p1 = (pos + 1);
    int _sv0t1 = ps_peek(tags, p1);
    if ((_sv0t1 == 7)) {
      sv0_vec_push(ty_tags, 6);
      sv0_vec_push(ty_data1, 0);
      sv0_vec_push(ty_data2, 0);
      sv0_vec_push(ty_data3, 0);
      int _sv0t2 = (p1 + 1);
      return _sv0t2;
    } else {
    }
    int _sv0t3 = parse_type(tags, starts, ends, source, p1, ty_tags, ty_data1, ty_data2, ty_data3, path_pool);
    int p2 = _sv0t3;
    if ((p2 < 0)) {
      int _sv0t4 = (0 - 1);
      return _sv0t4;
    } else {
    }
    int _sv0t5 = sv0_vec_len(ty_tags);
    int first_idx = (_sv0t5 - 1);
    int p = p2;
    int count = 1;
    int saw_comma = 0;
    int done = 0;
    while ((done != 1)) {
      int _sv0t6 = ps_peek(tags, p);
      int tc = _sv0t6;
      if ((tc == 12)) {
        saw_comma = 1;
        p = (p + 1);
        int _sv0t7 = ps_peek(tags, p);
        if ((_sv0t7 == 7)) {
          sv0_vec_push(ty_tags, 5);
          sv0_vec_push(ty_data1, first_idx);
          sv0_vec_push(ty_data2, count);
          sv0_vec_push(ty_data3, 0);
          int _sv0t8 = (p + 1);
          return _sv0t8;
        } else {
        }
        int _sv0t9 = parse_type(tags, starts, ends, source, p, ty_tags, ty_data1, ty_data2, ty_data3, path_pool);
        int pn = _sv0t9;
        if ((pn < 0)) {
          int _sv0t10 = (0 - 1);
          return _sv0t10;
        } else {
        }
        count = (count + 1);
        p = pn;
      } else {
        if ((tc == 7)) {
          if (saw_comma) {
            sv0_vec_push(ty_tags, 5);
            sv0_vec_push(ty_data1, first_idx);
            sv0_vec_push(ty_data2, count);
            sv0_vec_push(ty_data3, 0);
          } else {
          }
          int _sv0t11 = (p + 1);
          return _sv0t11;
        } else {
        }
        int _sv0t12 = (0 - 1);
        return _sv0t12;
      }
    }
    int _sv0t13 = (0 - 1);
    return _sv0t13;
  } else {
  }
  if ((t == 27)) {
    int p1 = (pos + 1);
    int _sv0t14 = ps_peek(tags, p1);
    if ((_sv0t14 == 77)) {
      int _sv0t15 = (p1 + 1);
      int _sv0t16 = parse_type(tags, starts, ends, source, _sv0t15, ty_tags, ty_data1, ty_data2, ty_data3, path_pool);
      int p2 = _sv0t16;
      if ((p2 < 0)) {
        int _sv0t17 = (0 - 1);
        return _sv0t17;
      } else {
      }
      int _sv0t18 = sv0_vec_len(ty_tags);
      int inner = (_sv0t18 - 1);
      sv0_vec_push(ty_tags, 2);
      sv0_vec_push(ty_data1, inner);
      sv0_vec_push(ty_data2, 0);
      sv0_vec_push(ty_data3, 0);
      return p2;
    } else {
    }
    int _sv0t19 = parse_type(tags, starts, ends, source, p1, ty_tags, ty_data1, ty_data2, ty_data3, path_pool);
    int p2 = _sv0t19;
    if ((p2 < 0)) {
      int _sv0t20 = (0 - 1);
      return _sv0t20;
    } else {
    }
    int _sv0t21 = sv0_vec_len(ty_tags);
    int inner = (_sv0t21 - 1);
    sv0_vec_push(ty_tags, 1);
    sv0_vec_push(ty_data1, inner);
    sv0_vec_push(ty_data2, 0);
    sv0_vec_push(ty_data3, 0);
    return p2;
  } else {
  }
  if ((t == 10)) {
    int p1 = (pos + 1);
    int _sv0t22 = parse_type(tags, starts, ends, source, p1, ty_tags, ty_data1, ty_data2, ty_data3, path_pool);
    int p2 = _sv0t22;
    if ((p2 < 0)) {
      int _sv0t23 = (0 - 1);
      return _sv0t23;
    } else {
    }
    int _sv0t24 = sv0_vec_len(ty_tags);
    int elem = (_sv0t24 - 1);
    int _sv0t25 = ps_peek(tags, p2);
    if ((_sv0t25 == 13)) {
      int p3 = (p2 + 1);
      int _sv0t26 = ps_peek(tags, p3);
      if ((_sv0t26 != 0)) {
        int _sv0t27 = (0 - 1);
        return _sv0t27;
      } else {
      }
      int len_pos = p3;
      int p4 = (p3 + 1);
      int _sv0t28 = ps_peek(tags, p4);
      if ((_sv0t28 != 11)) {
        int _sv0t29 = (0 - 1);
        return _sv0t29;
      } else {
      }
      sv0_vec_push(ty_tags, 3);
      sv0_vec_push(ty_data1, elem);
      sv0_vec_push(ty_data2, len_pos);
      sv0_vec_push(ty_data3, 0);
      int _sv0t30 = (p4 + 1);
      return _sv0t30;
    } else {
    }
    int _sv0t31 = ps_peek(tags, p2);
    if ((_sv0t31 == 11)) {
      sv0_vec_push(ty_tags, 4);
      sv0_vec_push(ty_data1, elem);
      sv0_vec_push(ty_data2, 0);
      sv0_vec_push(ty_data3, 0);
      int _sv0t32 = (p2 + 1);
      return _sv0t32;
    } else {
    }
    int _sv0t33 = (0 - 1);
    return _sv0t33;
  } else {
  }
  if ((t == 87)) {
    int p1 = (pos + 1);
    int _sv0t34 = ps_peek(tags, p1);
    if ((_sv0t34 == 15)) {
      int _sv0t35 = (p1 + 1);
      int _sv0t36 = ps_peek(tags, _sv0t35);
      if ((_sv0t36 == 5)) {
        int _sv0t37 = sv0_vec_len(path_pool);
        int path_start = _sv0t37;
        sv0_vec_push(path_pool, pos);
        int _sv0t38 = (p1 + 1);
        sv0_vec_push(path_pool, _sv0t38);
        int p2 = (p1 + 2);
        int _sv0t39 = ps_peek(tags, p2);
        if ((_sv0t39 == 38)) {
          int _sv0t40 = ps_skip_generic_params(tags, p2);
          p2 = _sv0t40;
        } else {
        }
        sv0_vec_push(ty_tags, 0);
        sv0_vec_push(ty_data1, path_start);
        sv0_vec_push(ty_data2, 2);
        sv0_vec_push(ty_data3, 0);
        return p2;
      } else {
      }
    } else {
    }
    int _sv0t41 = sv0_vec_len(path_pool);
    int path_start = _sv0t41;
    sv0_vec_push(path_pool, pos);
    sv0_vec_push(ty_tags, 0);
    sv0_vec_push(ty_data1, path_start);
    sv0_vec_push(ty_data2, 1);
    sv0_vec_push(ty_data3, 0);
    return p1;
  } else {
  }
  if ((t == 5)) {
    int _sv0t42 = is_builtin_type_name(source, starts, ends, pos);
    if (_sv0t42) {
      int _sv0t43 = sv0_vec_len(path_pool);
      int path_start = _sv0t43;
      sv0_vec_push(path_pool, pos);
      sv0_vec_push(ty_tags, 0);
      sv0_vec_push(ty_data1, path_start);
      sv0_vec_push(ty_data2, 1);
      sv0_vec_push(ty_data3, 0);
      int _sv0t44 = (pos + 1);
      return _sv0t44;
    } else {
    }
    int _sv0t45 = sv0_vec_len(path_pool);
    int path_start = _sv0t45;
    int _sv0t46 = parse_path(tags, pos, path_pool);
    int p1 = _sv0t46;
    if ((p1 < 0)) {
      int _sv0t47 = (0 - 1);
      return _sv0t47;
    } else {
    }
    int _sv0t48 = sv0_vec_len(path_pool);
    int seg_count = (_sv0t48 - path_start);
    int _sv0t49 = ps_peek(tags, p1);
    if ((_sv0t49 == 38)) {
      int p = (p1 + 1);
      int arg_count = 0;
      int _sv0t50 = ps_peek(tags, p);
      if ((_sv0t50 == 39)) {
        sv0_vec_push(ty_tags, 0);
        sv0_vec_push(ty_data1, path_start);
        sv0_vec_push(ty_data2, seg_count);
        sv0_vec_push(ty_data3, 0);
        int _sv0t51 = (p + 1);
        return _sv0t51;
      } else {
      }
      int done = 0;
      while ((done != 1)) {
        int _sv0t52 = ps_peek(tags, p);
        if ((_sv0t52 == 39)) {
          done = 1;
        } else {
          int _sv0t53 = parse_type(tags, starts, ends, source, p, ty_tags, ty_data1, ty_data2, ty_data3, path_pool);
          int p2 = _sv0t53;
          if ((p2 < 0)) {
            int _sv0t54 = (0 - 1);
            return _sv0t54;
          } else {
          }
          arg_count = (arg_count + 1);
          int _sv0t55 = ps_peek(tags, p2);
          if ((_sv0t55 == 12)) {
            p = (p2 + 1);
          } else {
            int _sv0t56 = ps_peek(tags, p2);
            if ((_sv0t56 == 39)) {
              p = p2;
            } else {
              int _sv0t57 = (0 - 1);
              return _sv0t57;
            }
          }
        }
      }
      sv0_vec_push(ty_tags, 0);
      sv0_vec_push(ty_data1, path_start);
      sv0_vec_push(ty_data2, seg_count);
      sv0_vec_push(ty_data3, arg_count);
      int _sv0t58 = (p + 1);
      return _sv0t58;
    } else {
    }
    sv0_vec_push(ty_tags, 0);
    sv0_vec_push(ty_data1, path_start);
    sv0_vec_push(ty_data2, seg_count);
    sv0_vec_push(ty_data3, 0);
    return p1;
  } else {
  }
  int _sv0t59 = (0 - 1);
  return _sv0t59;
}

static int path_pattern_from(int tags, int starts, int ends, const char* source, int pos, int pat_tags, int pat_data1, int pat_data2, int pat_data3, int path_pool, int pp_start, int pp_count) {
  int _sv0t0 = ps_peek(tags, pos);
  int t = _sv0t0;
  if ((t == 8)) {
    int p = (pos + 1);
    int field_count = 0;
    int done = 0;
    while ((done != 1)) {
      int _sv0t1 = ps_peek(tags, p);
      int ft = _sv0t1;
      if ((ft == 9)) {
        sv0_vec_push(pat_tags, 4);
        sv0_vec_push(pat_data1, pp_start);
        sv0_vec_push(pat_data2, pp_count);
        sv0_vec_push(pat_data3, field_count);
        int _sv0t2 = (p + 1);
        return _sv0t2;
      } else {
      }
      if ((ft == 17)) {
        int _sv0t3 = (p + 1);
        int _sv0t4 = ps_peek(tags, _sv0t3);
        if ((_sv0t4 == 9)) {
          sv0_vec_push(pat_tags, 4);
          sv0_vec_push(pat_data1, pp_start);
          sv0_vec_push(pat_data2, pp_count);
          sv0_vec_push(pat_data3, field_count);
          int _sv0t5 = (p + 2);
          return _sv0t5;
        } else {
        }
      } else {
      }
      if ((ft != 5)) {
        int _sv0t6 = (0 - 1);
        return _sv0t6;
      } else {
      }
      int fname_pos = p;
      p = (p + 1);
      int _sv0t7 = ps_peek(tags, p);
      if ((_sv0t7 == 14)) {
        p = (p + 1);
        int _sv0t8 = parse_pat(tags, starts, ends, source, p, pat_tags, pat_data1, pat_data2, pat_data3, path_pool);
        int pv = _sv0t8;
        if ((pv < 0)) {
          int _sv0t9 = (0 - 1);
          return _sv0t9;
        } else {
        }
        p = pv;
      } else {
        sv0_vec_push(pat_tags, 1);
        sv0_vec_push(pat_data1, fname_pos);
        sv0_vec_push(pat_data2, 0);
        sv0_vec_push(pat_data3, 0);
      }
      field_count = (field_count + 1);
      int _sv0t10 = ps_peek(tags, p);
      if ((_sv0t10 == 12)) {
        p = (p + 1);
      } else {
      }
    }
    int _sv0t11 = (0 - 1);
    return _sv0t11;
  } else {
  }
  if ((t == 6)) {
    int p = (pos + 1);
    int _sv0t12 = ps_peek(tags, p);
    if ((_sv0t12 == 7)) {
      sv0_vec_push(pat_tags, 5);
      sv0_vec_push(pat_data1, pp_start);
      sv0_vec_push(pat_data2, pp_count);
      sv0_vec_push(pat_data3, 0);
      int _sv0t13 = (p + 1);
      return _sv0t13;
    } else {
    }
    int arg_count = 0;
    int done = 0;
    while ((done != 1)) {
      int _sv0t14 = parse_pat(tags, starts, ends, source, p, pat_tags, pat_data1, pat_data2, pat_data3, path_pool);
      int pa = _sv0t14;
      if ((pa < 0)) {
        int _sv0t15 = (0 - 1);
        return _sv0t15;
      } else {
      }
      arg_count = (arg_count + 1);
      p = pa;
      int _sv0t16 = ps_peek(tags, p);
      if ((_sv0t16 == 12)) {
        p = (p + 1);
      } else {
        int _sv0t17 = ps_peek(tags, p);
        if ((_sv0t17 == 7)) {
          done = 1;
        } else {
          int _sv0t18 = (0 - 1);
          return _sv0t18;
        }
      }
    }
    sv0_vec_push(pat_tags, 5);
    sv0_vec_push(pat_data1, pp_start);
    sv0_vec_push(pat_data2, pp_count);
    sv0_vec_push(pat_data3, arg_count);
    int _sv0t19 = (p + 1);
    return _sv0t19;
  } else {
  }
  sv0_vec_push(pat_tags, 5);
  sv0_vec_push(pat_data1, pp_start);
  sv0_vec_push(pat_data2, pp_count);
  sv0_vec_push(pat_data3, 0);
  return pos;
}

static int parse_pat_atom(int tags, int starts, int ends, const char* source, int pos, int pat_tags, int pat_data1, int pat_data2, int pat_data3, int path_pool) {
  int _sv0t0 = ps_peek(tags, pos);
  int t = _sv0t0;
  if ((t == 77)) {
    int _sv0t1 = (pos + 1);
    int _sv0t2 = ps_peek(tags, _sv0t1);
    if ((_sv0t2 == 5)) {
      sv0_vec_push(pat_tags, 1);
      int _sv0t3 = (pos + 1);
      sv0_vec_push(pat_data1, _sv0t3);
      sv0_vec_push(pat_data2, 1);
      sv0_vec_push(pat_data3, 0);
      int _sv0t4 = (pos + 2);
      return _sv0t4;
    } else {
    }
    int _sv0t5 = (0 - 1);
    return _sv0t5;
  } else {
  }
  if ((t == 5)) {
    const char* _sv0t6 = ps_tok_text(source, starts, ends, pos);
    const char* txt;
    txt = _sv0t6;
    int _sv0t7 = sv0_string_eq(txt, "_");
    if (_sv0t7) {
      sv0_vec_push(pat_tags, 0);
      sv0_vec_push(pat_data1, 0);
      sv0_vec_push(pat_data2, 0);
      sv0_vec_push(pat_data3, 0);
      int _sv0t8 = (pos + 1);
      return _sv0t8;
    } else {
    }
    int _sv0t9 = (pos + 1);
    int _sv0t10 = ps_peek(tags, _sv0t9);
    if ((_sv0t10 == 15)) {
      int _sv0t11 = sv0_vec_len(path_pool);
      int pp_start = _sv0t11;
      int _sv0t12 = parse_path(tags, pos, path_pool);
      int p1 = _sv0t12;
      if ((p1 < 0)) {
        int _sv0t13 = (0 - 1);
        return _sv0t13;
      } else {
      }
      int _sv0t14 = sv0_vec_len(path_pool);
      int pp_count = (_sv0t14 - pp_start);
      int _sv0t15 = path_pattern_from(tags, starts, ends, source, p1, pat_tags, pat_data1, pat_data2, pat_data3, path_pool, pp_start, pp_count);
      return _sv0t15;
    } else {
    }
    sv0_vec_push(pat_tags, 1);
    sv0_vec_push(pat_data1, pos);
    sv0_vec_push(pat_data2, 0);
    sv0_vec_push(pat_data3, 0);
    int _sv0t16 = (pos + 1);
    return _sv0t16;
  } else {
  }
  if ((t == 23)) {
    int _sv0t17 = (pos + 1);
    int _sv0t18 = ps_peek(tags, _sv0t17);
    int nt = _sv0t18;
    if ((nt == 0)) {
      sv0_vec_push(pat_tags, 2);
      sv0_vec_push(pat_data1, 0);
      int _sv0t19 = (pos + 1);
      sv0_vec_push(pat_data2, _sv0t19);
      sv0_vec_push(pat_data3, 1);
      int _sv0t20 = (pos + 2);
      return _sv0t20;
    } else {
    }
    if ((nt == 1)) {
      sv0_vec_push(pat_tags, 2);
      sv0_vec_push(pat_data1, 1);
      int _sv0t21 = (pos + 1);
      sv0_vec_push(pat_data2, _sv0t21);
      sv0_vec_push(pat_data3, 1);
      int _sv0t22 = (pos + 2);
      return _sv0t22;
    } else {
    }
    int _sv0t23 = (0 - 1);
    return _sv0t23;
  } else {
  }
  int _sv0t24 = lit_from_tok(t);
  int lt = _sv0t24;
  if ((lt >= 0)) {
    sv0_vec_push(pat_tags, 2);
    sv0_vec_push(pat_data1, lt);
    sv0_vec_push(pat_data2, pos);
    sv0_vec_push(pat_data3, 0);
    int _sv0t25 = (pos + 1);
    return _sv0t25;
  } else {
  }
  if ((t == 6)) {
    int p1 = (pos + 1);
    int _sv0t26 = ps_peek(tags, p1);
    if ((_sv0t26 == 7)) {
      sv0_vec_push(pat_tags, 3);
      sv0_vec_push(pat_data1, 0);
      sv0_vec_push(pat_data2, 0);
      sv0_vec_push(pat_data3, 0);
      int _sv0t27 = (p1 + 1);
      return _sv0t27;
    } else {
    }
    int _sv0t28 = sv0_vec_len(pat_tags);
    int first_idx = _sv0t28;
    int _sv0t29 = parse_pat(tags, starts, ends, source, p1, pat_tags, pat_data1, pat_data2, pat_data3, path_pool);
    int p2 = _sv0t29;
    if ((p2 < 0)) {
      int _sv0t30 = (0 - 1);
      return _sv0t30;
    } else {
    }
    int _sv0t31 = ps_peek(tags, p2);
    if ((_sv0t31 == 7)) {
      int _sv0t32 = (p2 + 1);
      return _sv0t32;
    } else {
    }
    int _sv0t33 = ps_peek(tags, p2);
    if ((_sv0t33 == 12)) {
      int p = (p2 + 1);
      int count = 1;
      int done = 0;
      while ((done != 1)) {
        int _sv0t34 = ps_peek(tags, p);
        if ((_sv0t34 == 7)) {
          sv0_vec_push(pat_tags, 3);
          sv0_vec_push(pat_data1, first_idx);
          sv0_vec_push(pat_data2, count);
          sv0_vec_push(pat_data3, 0);
          int _sv0t35 = (p + 1);
          return _sv0t35;
        } else {
        }
        int _sv0t36 = parse_pat(tags, starts, ends, source, p, pat_tags, pat_data1, pat_data2, pat_data3, path_pool);
        int pn = _sv0t36;
        if ((pn < 0)) {
          int _sv0t37 = (0 - 1);
          return _sv0t37;
        } else {
        }
        count = (count + 1);
        p = pn;
        int _sv0t38 = ps_peek(tags, p);
        if ((_sv0t38 == 12)) {
          p = (p + 1);
        } else {
          int _sv0t39 = ps_peek(tags, p);
          if ((_sv0t39 != 7)) {
            int _sv0t40 = (0 - 1);
            return _sv0t40;
          } else {
          }
        }
      }
    } else {
    }
    int _sv0t41 = (0 - 1);
    return _sv0t41;
  } else {
  }
  int _sv0t42 = (0 - 1);
  return _sv0t42;
}

static int parse_pat(int tags, int starts, int ends, const char* source, int pos, int pat_tags, int pat_data1, int pat_data2, int pat_data3, int path_pool) {
  int _sv0t0 = sv0_vec_len(pat_tags);
  int first_idx = _sv0t0;
  int _sv0t1 = parse_pat_atom(tags, starts, ends, source, pos, pat_tags, pat_data1, pat_data2, pat_data3, path_pool);
  int p1 = _sv0t1;
  if ((p1 < 0)) {
    int _sv0t2 = (0 - 1);
    return _sv0t2;
  } else {
  }
  int _sv0t3 = ps_peek(tags, p1);
  if ((_sv0t3 != 29)) {
    return p1;
  } else {
  }
  int p = p1;
  int count = 1;
  int or_done = 0;
  while ((or_done != 1)) {
    int _sv0t4 = ps_peek(tags, p);
    if ((_sv0t4 != 29)) {
      or_done = 1;
    } else {
      p = (p + 1);
      int _sv0t5 = parse_pat_atom(tags, starts, ends, source, p, pat_tags, pat_data1, pat_data2, pat_data3, path_pool);
      int pa = _sv0t5;
      if ((pa < 0)) {
        int _sv0t6 = (0 - 1);
        return _sv0t6;
      } else {
      }
      count = (count + 1);
      p = pa;
    }
  }
  sv0_vec_push(pat_tags, 6);
  sv0_vec_push(pat_data1, first_idx);
  sv0_vec_push(pat_data2, count);
  sv0_vec_push(pat_data3, 0);
  return p;
}

static int parse_arg_list(int tags, int starts, int ends, const char* source, int pos, int et, int ed1, int ed2, int ed3, int ed4, int pp) {
  int _sv0t0 = ps_peek(tags, pos);
  if ((_sv0t0 == 7)) {
    return pos;
  } else {
  }
  int _sv0t1 = sv0_vec_len(et);
  int first_idx = _sv0t1;
  int _sv0t2 = parse_expr(tags, starts, ends, source, pos, et, ed1, ed2, ed3, ed4, pp, 1);
  int p1 = _sv0t2;
  if ((p1 < 0)) {
    int _sv0t3 = (0 - 1);
    return _sv0t3;
  } else {
  }
  int p = p1;
  int done = 0;
  while ((done != 1)) {
    int _sv0t4 = ps_peek(tags, p);
    int t = _sv0t4;
    if ((t == 12)) {
      int _sv0t5 = (p + 1);
      int _sv0t6 = ps_peek(tags, _sv0t5);
      int nt = _sv0t6;
      if ((nt == 7)) {
        done = 1;
      } else {
        p = (p + 1);
        int _sv0t7 = parse_expr(tags, starts, ends, source, p, et, ed1, ed2, ed3, ed4, pp, 1);
        int pa = _sv0t7;
        if ((pa < 0)) {
          int _sv0t8 = (0 - 1);
          return _sv0t8;
        } else {
        }
        p = pa;
      }
    } else {
      done = 1;
    }
  }
  return p;
}

static int parse_struct_fields(int tags, int starts, int ends, const char* source, int pos, int et, int ed1, int ed2, int ed3, int ed4, int pp, int sf_names) {
  int _sv0t0 = ps_peek(tags, pos);
  if ((_sv0t0 == 9)) {
    int _sv0t1 = (pos + 1);
    return _sv0t1;
  } else {
  }
  int p = pos;
  int done = 0;
  while ((done != 1)) {
    int _sv0t2 = ps_peek(tags, p);
    int t = _sv0t2;
    if ((t == 9)) {
      done = 1;
      p = (p + 1);
    } else {
      if ((t == 17)) {
        int _sv0t3 = (p + 1);
        int _sv0t4 = ps_peek(tags, _sv0t3);
        int nt = _sv0t4;
        if ((nt == 5)) {
          int _sv0t5 = (p + 2);
          int _sv0t6 = ps_peek(tags, _sv0t5);
          int n2 = _sv0t6;
          if ((n2 == 14)) {
            int _sv0t7 = (p + 1);
            sv0_vec_push(sf_names, _sv0t7);
            int _sv0t8 = (p + 3);
            int _sv0t9 = parse_expr(tags, starts, ends, source, _sv0t8, et, ed1, ed2, ed3, ed4, pp, 1);
            int pv = _sv0t9;
            if ((pv < 0)) {
              int _sv0t10 = (0 - 1);
              return _sv0t10;
            } else {
            }
            p = pv;
          } else {
            int _sv0t11 = (0 - 1);
            return _sv0t11;
          }
        } else {
          int _sv0t12 = (0 - 1);
          return _sv0t12;
        }
      } else {
        if ((t == 5)) {
          int _sv0t13 = (p + 1);
          int _sv0t14 = ps_peek(tags, _sv0t13);
          int nt = _sv0t14;
          if ((nt == 14)) {
            sv0_vec_push(sf_names, p);
            int _sv0t15 = (p + 2);
            int _sv0t16 = parse_expr(tags, starts, ends, source, _sv0t15, et, ed1, ed2, ed3, ed4, pp, 1);
            int pv = _sv0t16;
            if ((pv < 0)) {
              int _sv0t17 = (0 - 1);
              return _sv0t17;
            } else {
            }
            p = pv;
          } else {
            sv0_vec_push(sf_names, p);
            int _sv0t18 = sv0_vec_len(pp);
            int pps = _sv0t18;
            sv0_vec_push(pp, p);
            sv0_vec_push(et, 1);
            sv0_vec_push(ed1, pps);
            sv0_vec_push(ed2, 1);
            sv0_vec_push(ed3, 0);
            sv0_vec_push(ed4, 0);
            p = (p + 1);
          }
        } else {
          int _sv0t19 = (0 - 1);
          return _sv0t19;
        }
      }
      int _sv0t20 = ps_peek(tags, p);
      int ct = _sv0t20;
      if ((ct == 12)) {
        p = (p + 1);
      } else {
        if ((ct != 9)) {
          int _sv0t21 = (0 - 1);
          return _sv0t21;
        } else {
        }
      }
    }
  }
  return p;
}

static int parse_primary_expr(int tags, int starts, int ends, const char* source, int pos, int et, int ed1, int ed2, int ed3, int ed4, int pp, int sf_names, int allow_struct) {
  int _sv0t0 = ps_peek(tags, pos);
  int t = _sv0t0;
  int _sv0t1 = lit_from_tok(t);
  int lt = _sv0t1;
  int _sv0t2 = (0 - 1);
  if ((lt != _sv0t2)) {
    sv0_vec_push(et, 0);
    sv0_vec_push(ed1, lt);
    sv0_vec_push(ed2, pos);
    sv0_vec_push(ed3, 0);
    sv0_vec_push(ed4, 0);
    int _sv0t3 = (pos + 1);
    return _sv0t3;
  } else {
  }
  if ((t == 5)) {
    int _sv0t4 = sv0_vec_len(pp);
    int pps = _sv0t4;
    int _sv0t5 = parse_path(tags, pos, pp);
    int p1 = _sv0t5;
    if ((p1 < 0)) {
      int _sv0t6 = (0 - 1);
      return _sv0t6;
    } else {
    }
    int _sv0t7 = sv0_vec_len(pp);
    int ppc = (_sv0t7 - pps);
    if ((allow_struct == 1)) {
      int _sv0t8 = ps_peek(tags, p1);
      if ((_sv0t8 == 8)) {
        int _sv0t9 = sv0_vec_len(sf_names);
        int fc_before = _sv0t9;
        int _sv0t10 = sv0_vec_len(et);
        int first_val = _sv0t10;
        int _sv0t11 = (p1 + 1);
        int _sv0t12 = parse_struct_fields(tags, starts, ends, source, _sv0t11, et, ed1, ed2, ed3, ed4, pp, sf_names);
        int pf = _sv0t12;
        if ((pf < 0)) {
          int _sv0t13 = (0 - 1);
          return _sv0t13;
        } else {
        }
        int _sv0t14 = sv0_vec_len(sf_names);
        int fc = (_sv0t14 - fc_before);
        sv0_vec_push(et, 24);
        sv0_vec_push(ed1, pps);
        sv0_vec_push(ed2, ppc);
        sv0_vec_push(ed3, fc);
        sv0_vec_push(ed4, 0);
        return pf;
      } else {
      }
    } else {
    }
    sv0_vec_push(et, 1);
    sv0_vec_push(ed1, pps);
    sv0_vec_push(ed2, ppc);
    sv0_vec_push(ed3, 0);
    sv0_vec_push(ed4, 0);
    return p1;
  } else {
  }
  if ((t == 6)) {
    int _sv0t15 = (pos + 1);
    int _sv0t16 = ps_peek(tags, _sv0t15);
    int nt = _sv0t16;
    if ((nt == 7)) {
      sv0_vec_push(et, 0);
      sv0_vec_push(ed1, 5);
      sv0_vec_push(ed2, pos);
      sv0_vec_push(ed3, 0);
      sv0_vec_push(ed4, 0);
      int _sv0t17 = (pos + 2);
      return _sv0t17;
    } else {
    }
    int _sv0t18 = (pos + 1);
    int _sv0t19 = parse_expr(tags, starts, ends, source, _sv0t18, et, ed1, ed2, ed3, ed4, pp, 1);
    int p1 = _sv0t19;
    if ((p1 < 0)) {
      int _sv0t20 = (0 - 1);
      return _sv0t20;
    } else {
    }
    int _sv0t21 = ps_peek(tags, p1);
    if ((_sv0t21 == 7)) {
      int _sv0t22 = (p1 + 1);
      return _sv0t22;
    } else {
    }
    int _sv0t23 = ps_peek(tags, p1);
    if ((_sv0t23 == 12)) {
      int _sv0t24 = sv0_vec_len(et);
      int first_idx = (_sv0t24 - 1);
      int count = 1;
      int p = (p1 + 1);
      int td = 0;
      while ((td != 1)) {
        int _sv0t25 = ps_peek(tags, p);
        if ((_sv0t25 == 7)) {
          sv0_vec_push(et, 25);
          sv0_vec_push(ed1, first_idx);
          sv0_vec_push(ed2, count);
          sv0_vec_push(ed3, 0);
          sv0_vec_push(ed4, 0);
          int _sv0t26 = (p + 1);
          return _sv0t26;
        } else {
        }
        int _sv0t27 = parse_expr(tags, starts, ends, source, p, et, ed1, ed2, ed3, ed4, pp, 1);
        int pa = _sv0t27;
        if ((pa < 0)) {
          int _sv0t28 = (0 - 1);
          return _sv0t28;
        } else {
        }
        count = (count + 1);
        p = pa;
        int _sv0t29 = ps_peek(tags, p);
        if ((_sv0t29 == 12)) {
          p = (p + 1);
        } else {
          int _sv0t30 = ps_peek(tags, p);
          if ((_sv0t30 == 7)) {
            sv0_vec_push(et, 25);
            sv0_vec_push(ed1, first_idx);
            sv0_vec_push(ed2, count);
            sv0_vec_push(ed3, 0);
            sv0_vec_push(ed4, 0);
            int _sv0t31 = (p + 1);
            return _sv0t31;
          } else {
          }
          int _sv0t32 = (0 - 1);
          return _sv0t32;
        }
      }
    } else {
    }
    int _sv0t33 = (0 - 1);
    return _sv0t33;
  } else {
  }
  if ((t == 10)) {
    int _sv0t34 = (pos + 1);
    int _sv0t35 = ps_peek(tags, _sv0t34);
    if ((_sv0t35 == 11)) {
      sv0_vec_push(et, 26);
      sv0_vec_push(ed1, 0);
      sv0_vec_push(ed2, 0);
      sv0_vec_push(ed3, 0);
      sv0_vec_push(ed4, 0);
      int _sv0t36 = (pos + 2);
      return _sv0t36;
    } else {
    }
    int _sv0t37 = sv0_vec_len(et);
    int first_idx = _sv0t37;
    int _sv0t38 = (pos + 1);
    int _sv0t39 = parse_expr(tags, starts, ends, source, _sv0t38, et, ed1, ed2, ed3, ed4, pp, 1);
    int p1 = _sv0t39;
    if ((p1 < 0)) {
      int _sv0t40 = (0 - 1);
      return _sv0t40;
    } else {
    }
    int _sv0t41 = ps_peek(tags, p1);
    if ((_sv0t41 == 13)) {
      int _sv0t42 = (p1 + 1);
      int _sv0t43 = parse_expr(tags, starts, ends, source, _sv0t42, et, ed1, ed2, ed3, ed4, pp, 1);
      int p2 = _sv0t43;
      if ((p2 < 0)) {
        int _sv0t44 = (0 - 1);
        return _sv0t44;
      } else {
      }
      int _sv0t45 = ps_peek(tags, p2);
      if ((_sv0t45 != 11)) {
        int _sv0t46 = (0 - 1);
        return _sv0t46;
      } else {
      }
      sv0_vec_push(et, 26);
      sv0_vec_push(ed1, first_idx);
      sv0_vec_push(ed2, 2);
      sv0_vec_push(ed3, 0);
      sv0_vec_push(ed4, 0);
      int _sv0t47 = (p2 + 1);
      return _sv0t47;
    } else {
    }
    int _sv0t48 = ps_peek(tags, p1);
    if ((_sv0t48 == 11)) {
      sv0_vec_push(et, 26);
      sv0_vec_push(ed1, first_idx);
      sv0_vec_push(ed2, 1);
      sv0_vec_push(ed3, 0);
      sv0_vec_push(ed4, 0);
      int _sv0t49 = (p1 + 1);
      return _sv0t49;
    } else {
    }
    int _sv0t50 = ps_peek(tags, p1);
    if ((_sv0t50 == 12)) {
      int count = 1;
      int p = (p1 + 1);
      int ad = 0;
      while ((ad != 1)) {
        int _sv0t51 = ps_peek(tags, p);
        if ((_sv0t51 == 11)) {
          sv0_vec_push(et, 26);
          sv0_vec_push(ed1, first_idx);
          sv0_vec_push(ed2, count);
          sv0_vec_push(ed3, 0);
          sv0_vec_push(ed4, 0);
          int _sv0t52 = (p + 1);
          return _sv0t52;
        } else {
        }
        int _sv0t53 = parse_expr(tags, starts, ends, source, p, et, ed1, ed2, ed3, ed4, pp, 1);
        int pa = _sv0t53;
        if ((pa < 0)) {
          int _sv0t54 = (0 - 1);
          return _sv0t54;
        } else {
        }
        count = (count + 1);
        p = pa;
        int _sv0t55 = ps_peek(tags, p);
        if ((_sv0t55 == 12)) {
          p = (p + 1);
        } else {
          int _sv0t56 = ps_peek(tags, p);
          if ((_sv0t56 == 11)) {
            sv0_vec_push(et, 26);
            sv0_vec_push(ed1, first_idx);
            sv0_vec_push(ed2, count);
            sv0_vec_push(ed3, 0);
            sv0_vec_push(ed4, 0);
            int _sv0t57 = (p + 1);
            return _sv0t57;
          } else {
          }
          int _sv0t58 = (0 - 1);
          return _sv0t58;
        }
      }
    } else {
    }
    int _sv0t59 = (0 - 1);
    return _sv0t59;
  } else {
  }
  int _sv0t60 = (0 - 1);
  return _sv0t60;
}

static int parse_postfix_expr(int tags, int starts, int ends, const char* source, int pos, int et, int ed1, int ed2, int ed3, int ed4, int pp, int sf_names, int allow_struct) {
  int _sv0t0 = parse_primary_expr(tags, starts, ends, source, pos, et, ed1, ed2, ed3, ed4, pp, sf_names, allow_struct);
  int p1 = _sv0t0;
  if ((p1 < 0)) {
    int _sv0t1 = (0 - 1);
    return _sv0t1;
  } else {
  }
  int _sv0t2 = sv0_vec_len(et);
  int cur = (_sv0t2 - 1);
  int p = p1;
  int done = 0;
  while ((done != 1)) {
    int _sv0t3 = ps_peek(tags, p);
    int t = _sv0t3;
    if ((t == 6)) {
      int _sv0t4 = sv0_vec_len(et);
      int args_before = _sv0t4;
      int _sv0t5 = (p + 1);
      int _sv0t6 = parse_arg_list(tags, starts, ends, source, _sv0t5, et, ed1, ed2, ed3, ed4, pp);
      int pa = _sv0t6;
      if ((pa < 0)) {
        int _sv0t7 = (0 - 1);
        return _sv0t7;
      } else {
      }
      int _sv0t8 = sv0_vec_len(et);
      int argc = (_sv0t8 - args_before);
      int _sv0t9 = ps_peek(tags, pa);
      if ((_sv0t9 != 7)) {
        int _sv0t10 = (0 - 1);
        return _sv0t10;
      } else {
      }
      sv0_vec_push(et, 4);
      sv0_vec_push(ed1, cur);
      sv0_vec_push(ed2, args_before);
      sv0_vec_push(ed3, argc);
      sv0_vec_push(ed4, 0);
      int _sv0t11 = sv0_vec_len(et);
      cur = (_sv0t11 - 1);
      p = (pa + 1);
    } else {
      if ((t == 16)) {
        int _sv0t12 = (p + 1);
        int _sv0t13 = ps_peek(tags, _sv0t12);
        int nt = _sv0t13;
        if ((nt == 5)) {
          int _sv0t14 = (p + 2);
          int _sv0t15 = ps_peek(tags, _sv0t14);
          int n2 = _sv0t15;
          if ((n2 == 6)) {
            int method_tok = (p + 1);
            int _sv0t16 = sv0_vec_len(et);
            int args_before = _sv0t16;
            int _sv0t17 = (p + 3);
            int _sv0t18 = parse_arg_list(tags, starts, ends, source, _sv0t17, et, ed1, ed2, ed3, ed4, pp);
            int pa = _sv0t18;
            if ((pa < 0)) {
              int _sv0t19 = (0 - 1);
              return _sv0t19;
            } else {
            }
            int _sv0t20 = sv0_vec_len(et);
            int argc = (_sv0t20 - args_before);
            int _sv0t21 = ps_peek(tags, pa);
            if ((_sv0t21 != 7)) {
              int _sv0t22 = (0 - 1);
              return _sv0t22;
            } else {
            }
            sv0_vec_push(et, 5);
            sv0_vec_push(ed1, cur);
            sv0_vec_push(ed2, method_tok);
            sv0_vec_push(ed3, args_before);
            sv0_vec_push(ed4, argc);
            int _sv0t23 = sv0_vec_len(et);
            cur = (_sv0t23 - 1);
            p = (pa + 1);
          } else {
            sv0_vec_push(et, 6);
            sv0_vec_push(ed1, cur);
            int _sv0t24 = (p + 1);
            sv0_vec_push(ed2, _sv0t24);
            sv0_vec_push(ed3, 0);
            sv0_vec_push(ed4, 0);
            int _sv0t25 = sv0_vec_len(et);
            cur = (_sv0t25 - 1);
            p = (p + 2);
          }
        } else {
          if ((nt == 0)) {
            sv0_vec_push(et, 7);
            sv0_vec_push(ed1, cur);
            int _sv0t26 = (p + 1);
            sv0_vec_push(ed2, _sv0t26);
            sv0_vec_push(ed3, 0);
            sv0_vec_push(ed4, 0);
            int _sv0t27 = sv0_vec_len(et);
            cur = (_sv0t27 - 1);
            p = (p + 2);
          } else {
            done = 1;
          }
        }
      } else {
        if ((t == 10)) {
          int _sv0t28 = (p + 1);
          int _sv0t29 = parse_expr(tags, starts, ends, source, _sv0t28, et, ed1, ed2, ed3, ed4, pp, 1);
          int ix_p = _sv0t29;
          if ((ix_p < 0)) {
            int _sv0t30 = (0 - 1);
            return _sv0t30;
          } else {
          }
          int _sv0t31 = sv0_vec_len(et);
          int ix_idx = (_sv0t31 - 1);
          int _sv0t32 = ps_peek(tags, ix_p);
          if ((_sv0t32 != 11)) {
            int _sv0t33 = (0 - 1);
            return _sv0t33;
          } else {
          }
          sv0_vec_push(et, 8);
          sv0_vec_push(ed1, cur);
          sv0_vec_push(ed2, ix_idx);
          sv0_vec_push(ed3, 0);
          sv0_vec_push(ed4, 0);
          int _sv0t34 = sv0_vec_len(et);
          cur = (_sv0t34 - 1);
          p = (ix_p + 1);
        } else {
          done = 1;
        }
      }
    }
  }
  return p;
}

static int parse_unary_expr(int tags, int starts, int ends, const char* source, int pos, int et, int ed1, int ed2, int ed3, int ed4, int pp, int sf_names, int allow_struct) {
  int _sv0t0 = ps_peek(tags, pos);
  int t = _sv0t0;
  if ((t == 23)) {
    int _sv0t1 = (pos + 1);
    int _sv0t2 = parse_unary_expr(tags, starts, ends, source, _sv0t1, et, ed1, ed2, ed3, ed4, pp, sf_names, allow_struct);
    int p1 = _sv0t2;
    if ((p1 < 0)) {
      int _sv0t3 = (0 - 1);
      return _sv0t3;
    } else {
    }
    int _sv0t4 = sv0_vec_len(et);
    int oi = (_sv0t4 - 1);
    sv0_vec_push(et, 2);
    sv0_vec_push(ed1, 0);
    sv0_vec_push(ed2, oi);
    sv0_vec_push(ed3, 0);
    sv0_vec_push(ed4, 0);
    return p1;
  } else {
  }
  if ((t == 33)) {
    int _sv0t5 = (pos + 1);
    int _sv0t6 = parse_unary_expr(tags, starts, ends, source, _sv0t5, et, ed1, ed2, ed3, ed4, pp, sf_names, allow_struct);
    int p1 = _sv0t6;
    if ((p1 < 0)) {
      int _sv0t7 = (0 - 1);
      return _sv0t7;
    } else {
    }
    int _sv0t8 = sv0_vec_len(et);
    int oi = (_sv0t8 - 1);
    sv0_vec_push(et, 2);
    sv0_vec_push(ed1, 1);
    sv0_vec_push(ed2, oi);
    sv0_vec_push(ed3, 0);
    sv0_vec_push(ed4, 0);
    return p1;
  } else {
  }
  if ((t == 24)) {
    int _sv0t9 = (pos + 1);
    int _sv0t10 = parse_unary_expr(tags, starts, ends, source, _sv0t9, et, ed1, ed2, ed3, ed4, pp, sf_names, allow_struct);
    int p1 = _sv0t10;
    if ((p1 < 0)) {
      int _sv0t11 = (0 - 1);
      return _sv0t11;
    } else {
    }
    int _sv0t12 = sv0_vec_len(et);
    int oi = (_sv0t12 - 1);
    sv0_vec_push(et, 2);
    sv0_vec_push(ed1, 3);
    sv0_vec_push(ed2, oi);
    sv0_vec_push(ed3, 0);
    sv0_vec_push(ed4, 0);
    return p1;
  } else {
  }
  if ((t == 27)) {
    int _sv0t13 = (pos + 1);
    int _sv0t14 = ps_peek(tags, _sv0t13);
    if ((_sv0t14 == 77)) {
      int _sv0t15 = (pos + 2);
      int _sv0t16 = parse_unary_expr(tags, starts, ends, source, _sv0t15, et, ed1, ed2, ed3, ed4, pp, sf_names, allow_struct);
      int p1 = _sv0t16;
      if ((p1 < 0)) {
        int _sv0t17 = (0 - 1);
        return _sv0t17;
      } else {
      }
      int _sv0t18 = sv0_vec_len(et);
      int oi = (_sv0t18 - 1);
      sv0_vec_push(et, 2);
      sv0_vec_push(ed1, 5);
      sv0_vec_push(ed2, oi);
      sv0_vec_push(ed3, 0);
      sv0_vec_push(ed4, 0);
      return p1;
    } else {
    }
    int _sv0t19 = (pos + 1);
    int _sv0t20 = parse_unary_expr(tags, starts, ends, source, _sv0t19, et, ed1, ed2, ed3, ed4, pp, sf_names, allow_struct);
    int p1 = _sv0t20;
    if ((p1 < 0)) {
      int _sv0t21 = (0 - 1);
      return _sv0t21;
    } else {
    }
    int _sv0t22 = sv0_vec_len(et);
    int oi = (_sv0t22 - 1);
    sv0_vec_push(et, 2);
    sv0_vec_push(ed1, 4);
    sv0_vec_push(ed2, oi);
    sv0_vec_push(ed3, 0);
    sv0_vec_push(ed4, 0);
    return p1;
  } else {
  }
  int _sv0t23 = parse_postfix_expr(tags, starts, ends, source, pos, et, ed1, ed2, ed3, ed4, pp, sf_names, allow_struct);
  return _sv0t23;
}

static int parse_cast_expr(int tags, int starts, int ends, const char* source, int pos, int et, int ed1, int ed2, int ed3, int ed4, int pp, int sf_names, int ty_tags, int ty_d1, int ty_d2, int ty_d3, int allow_struct) {
  int _sv0t0 = parse_unary_expr(tags, starts, ends, source, pos, et, ed1, ed2, ed3, ed4, pp, sf_names, allow_struct);
  int p1 = _sv0t0;
  if ((p1 < 0)) {
    int _sv0t1 = (0 - 1);
    return _sv0t1;
  } else {
  }
  int _sv0t2 = sv0_vec_len(et);
  int cur = (_sv0t2 - 1);
  int p = p1;
  int done = 0;
  while ((done != 1)) {
    int _sv0t3 = ps_peek(tags, p);
    if ((_sv0t3 == 54)) {
      int _sv0t4 = sv0_vec_len(ty_tags);
      int ty_idx = _sv0t4;
      int _sv0t5 = (p + 1);
      int _sv0t6 = parse_type(tags, starts, ends, source, _sv0t5, ty_tags, ty_d1, ty_d2, ty_d3, pp);
      int pt = _sv0t6;
      if ((pt < 0)) {
        int _sv0t7 = (0 - 1);
        return _sv0t7;
      } else {
      }
      sv0_vec_push(et, 20);
      sv0_vec_push(ed1, cur);
      sv0_vec_push(ed2, ty_idx);
      sv0_vec_push(ed3, 0);
      sv0_vec_push(ed4, 0);
      int _sv0t8 = sv0_vec_len(et);
      cur = (_sv0t8 - 1);
      p = pt;
    } else {
      done = 1;
    }
  }
  return p;
}

static int parse_mul_expr(int tags, int starts, int ends, const char* source, int pos, int et, int ed1, int ed2, int ed3, int ed4, int pp, int sf_names, int ty_tags, int ty_d1, int ty_d2, int ty_d3, int allow_struct) {
  int _sv0t0 = parse_cast_expr(tags, starts, ends, source, pos, et, ed1, ed2, ed3, ed4, pp, sf_names, ty_tags, ty_d1, ty_d2, ty_d3, allow_struct);
  int p1 = _sv0t0;
  if ((p1 < 0)) {
    int _sv0t1 = (0 - 1);
    return _sv0t1;
  } else {
  }
  int _sv0t2 = sv0_vec_len(et);
  int left = (_sv0t2 - 1);
  int p = p1;
  int done = 0;
  while ((done != 1)) {
    int _sv0t3 = ps_peek(tags, p);
    int t = _sv0t3;
    int _sv0t4 = is_multiplicative_token(t);
    if ((_sv0t4 != 1)) {
      done = 1;
    } else {
      int _sv0t5 = binop_of(t);
      int bop = _sv0t5;
      int _sv0t6 = (p + 1);
      int _sv0t7 = parse_cast_expr(tags, starts, ends, source, _sv0t6, et, ed1, ed2, ed3, ed4, pp, sf_names, ty_tags, ty_d1, ty_d2, ty_d3, allow_struct);
      int pa = _sv0t7;
      if ((pa < 0)) {
        int _sv0t8 = (0 - 1);
        return _sv0t8;
      } else {
      }
      int _sv0t9 = sv0_vec_len(et);
      int ri = (_sv0t9 - 1);
      sv0_vec_push(et, 3);
      sv0_vec_push(ed1, bop);
      sv0_vec_push(ed2, left);
      sv0_vec_push(ed3, ri);
      sv0_vec_push(ed4, 0);
      int _sv0t10 = sv0_vec_len(et);
      left = (_sv0t10 - 1);
      p = pa;
    }
  }
  return p;
}

static int parse_add_expr(int tags, int starts, int ends, const char* source, int pos, int et, int ed1, int ed2, int ed3, int ed4, int pp, int sf_names, int ty_tags, int ty_d1, int ty_d2, int ty_d3, int allow_struct) {
  int _sv0t0 = parse_mul_expr(tags, starts, ends, source, pos, et, ed1, ed2, ed3, ed4, pp, sf_names, ty_tags, ty_d1, ty_d2, ty_d3, allow_struct);
  int p1 = _sv0t0;
  if ((p1 < 0)) {
    int _sv0t1 = (0 - 1);
    return _sv0t1;
  } else {
  }
  int _sv0t2 = sv0_vec_len(et);
  int left = (_sv0t2 - 1);
  int p = p1;
  int done = 0;
  while ((done != 1)) {
    int _sv0t3 = ps_peek(tags, p);
    int t = _sv0t3;
    int _sv0t4 = is_additive_token(t);
    if ((_sv0t4 != 1)) {
      done = 1;
    } else {
      int _sv0t5 = binop_of(t);
      int bop = _sv0t5;
      int _sv0t6 = (p + 1);
      int _sv0t7 = parse_mul_expr(tags, starts, ends, source, _sv0t6, et, ed1, ed2, ed3, ed4, pp, sf_names, ty_tags, ty_d1, ty_d2, ty_d3, allow_struct);
      int pa = _sv0t7;
      if ((pa < 0)) {
        int _sv0t8 = (0 - 1);
        return _sv0t8;
      } else {
      }
      int _sv0t9 = sv0_vec_len(et);
      int ri = (_sv0t9 - 1);
      sv0_vec_push(et, 3);
      sv0_vec_push(ed1, bop);
      sv0_vec_push(ed2, left);
      sv0_vec_push(ed3, ri);
      sv0_vec_push(ed4, 0);
      int _sv0t10 = sv0_vec_len(et);
      left = (_sv0t10 - 1);
      p = pa;
    }
  }
  return p;
}

static int parse_shift_expr(int tags, int starts, int ends, const char* source, int pos, int et, int ed1, int ed2, int ed3, int ed4, int pp, int sf_names, int ty_tags, int ty_d1, int ty_d2, int ty_d3, int allow_struct) {
  int _sv0t0 = parse_add_expr(tags, starts, ends, source, pos, et, ed1, ed2, ed3, ed4, pp, sf_names, ty_tags, ty_d1, ty_d2, ty_d3, allow_struct);
  int p1 = _sv0t0;
  if ((p1 < 0)) {
    int _sv0t1 = (0 - 1);
    return _sv0t1;
  } else {
  }
  int _sv0t2 = sv0_vec_len(et);
  int left = (_sv0t2 - 1);
  int p = p1;
  int done = 0;
  while ((done != 1)) {
    int _sv0t3 = ps_peek(tags, p);
    int t = _sv0t3;
    int _sv0t4 = is_shift_token(t);
    if ((_sv0t4 != 1)) {
      done = 1;
    } else {
      int _sv0t5 = binop_of(t);
      int bop = _sv0t5;
      int _sv0t6 = (p + 1);
      int _sv0t7 = parse_add_expr(tags, starts, ends, source, _sv0t6, et, ed1, ed2, ed3, ed4, pp, sf_names, ty_tags, ty_d1, ty_d2, ty_d3, allow_struct);
      int pa = _sv0t7;
      if ((pa < 0)) {
        int _sv0t8 = (0 - 1);
        return _sv0t8;
      } else {
      }
      int _sv0t9 = sv0_vec_len(et);
      int ri = (_sv0t9 - 1);
      sv0_vec_push(et, 3);
      sv0_vec_push(ed1, bop);
      sv0_vec_push(ed2, left);
      sv0_vec_push(ed3, ri);
      sv0_vec_push(ed4, 0);
      int _sv0t10 = sv0_vec_len(et);
      left = (_sv0t10 - 1);
      p = pa;
    }
  }
  return p;
}

static int parse_bit_and_expr(int tags, int starts, int ends, const char* source, int pos, int et, int ed1, int ed2, int ed3, int ed4, int pp, int sf_names, int ty_tags, int ty_d1, int ty_d2, int ty_d3, int allow_struct) {
  int _sv0t0 = parse_shift_expr(tags, starts, ends, source, pos, et, ed1, ed2, ed3, ed4, pp, sf_names, ty_tags, ty_d1, ty_d2, ty_d3, allow_struct);
  int p1 = _sv0t0;
  if ((p1 < 0)) {
    int _sv0t1 = (0 - 1);
    return _sv0t1;
  } else {
  }
  int _sv0t2 = sv0_vec_len(et);
  int left = (_sv0t2 - 1);
  int p = p1;
  int done = 0;
  while ((done != 1)) {
    int _sv0t3 = ps_peek(tags, p);
    int t = _sv0t3;
    if ((t != 27)) {
      done = 1;
    } else {
      int _sv0t4 = binop_of(t);
      int bop = _sv0t4;
      int _sv0t5 = (p + 1);
      int _sv0t6 = parse_shift_expr(tags, starts, ends, source, _sv0t5, et, ed1, ed2, ed3, ed4, pp, sf_names, ty_tags, ty_d1, ty_d2, ty_d3, allow_struct);
      int pa = _sv0t6;
      if ((pa < 0)) {
        int _sv0t7 = (0 - 1);
        return _sv0t7;
      } else {
      }
      int _sv0t8 = sv0_vec_len(et);
      int ri = (_sv0t8 - 1);
      sv0_vec_push(et, 3);
      sv0_vec_push(ed1, bop);
      sv0_vec_push(ed2, left);
      sv0_vec_push(ed3, ri);
      sv0_vec_push(ed4, 0);
      int _sv0t9 = sv0_vec_len(et);
      left = (_sv0t9 - 1);
      p = pa;
    }
  }
  return p;
}

static int parse_bit_xor_expr(int tags, int starts, int ends, const char* source, int pos, int et, int ed1, int ed2, int ed3, int ed4, int pp, int sf_names, int ty_tags, int ty_d1, int ty_d2, int ty_d3, int allow_struct) {
  int _sv0t0 = parse_bit_and_expr(tags, starts, ends, source, pos, et, ed1, ed2, ed3, ed4, pp, sf_names, ty_tags, ty_d1, ty_d2, ty_d3, allow_struct);
  int p1 = _sv0t0;
  if ((p1 < 0)) {
    int _sv0t1 = (0 - 1);
    return _sv0t1;
  } else {
  }
  int _sv0t2 = sv0_vec_len(et);
  int left = (_sv0t2 - 1);
  int p = p1;
  int done = 0;
  while ((done != 1)) {
    int _sv0t3 = ps_peek(tags, p);
    int t = _sv0t3;
    if ((t != 31)) {
      done = 1;
    } else {
      int _sv0t4 = binop_of(t);
      int bop = _sv0t4;
      int _sv0t5 = (p + 1);
      int _sv0t6 = parse_bit_and_expr(tags, starts, ends, source, _sv0t5, et, ed1, ed2, ed3, ed4, pp, sf_names, ty_tags, ty_d1, ty_d2, ty_d3, allow_struct);
      int pa = _sv0t6;
      if ((pa < 0)) {
        int _sv0t7 = (0 - 1);
        return _sv0t7;
      } else {
      }
      int _sv0t8 = sv0_vec_len(et);
      int ri = (_sv0t8 - 1);
      sv0_vec_push(et, 3);
      sv0_vec_push(ed1, bop);
      sv0_vec_push(ed2, left);
      sv0_vec_push(ed3, ri);
      sv0_vec_push(ed4, 0);
      int _sv0t9 = sv0_vec_len(et);
      left = (_sv0t9 - 1);
      p = pa;
    }
  }
  return p;
}

static int parse_bit_or_expr(int tags, int starts, int ends, const char* source, int pos, int et, int ed1, int ed2, int ed3, int ed4, int pp, int sf_names, int ty_tags, int ty_d1, int ty_d2, int ty_d3, int allow_struct) {
  int _sv0t0 = parse_bit_xor_expr(tags, starts, ends, source, pos, et, ed1, ed2, ed3, ed4, pp, sf_names, ty_tags, ty_d1, ty_d2, ty_d3, allow_struct);
  int p1 = _sv0t0;
  if ((p1 < 0)) {
    int _sv0t1 = (0 - 1);
    return _sv0t1;
  } else {
  }
  int _sv0t2 = sv0_vec_len(et);
  int left = (_sv0t2 - 1);
  int p = p1;
  int done = 0;
  while ((done != 1)) {
    int _sv0t3 = ps_peek(tags, p);
    int t = _sv0t3;
    if ((t != 29)) {
      done = 1;
    } else {
      int _sv0t4 = binop_of(t);
      int bop = _sv0t4;
      int _sv0t5 = (p + 1);
      int _sv0t6 = parse_bit_xor_expr(tags, starts, ends, source, _sv0t5, et, ed1, ed2, ed3, ed4, pp, sf_names, ty_tags, ty_d1, ty_d2, ty_d3, allow_struct);
      int pa = _sv0t6;
      if ((pa < 0)) {
        int _sv0t7 = (0 - 1);
        return _sv0t7;
      } else {
      }
      int _sv0t8 = sv0_vec_len(et);
      int ri = (_sv0t8 - 1);
      sv0_vec_push(et, 3);
      sv0_vec_push(ed1, bop);
      sv0_vec_push(ed2, left);
      sv0_vec_push(ed3, ri);
      sv0_vec_push(ed4, 0);
      int _sv0t9 = sv0_vec_len(et);
      left = (_sv0t9 - 1);
      p = pa;
    }
  }
  return p;
}

static int parse_cmp_expr(int tags, int starts, int ends, const char* source, int pos, int et, int ed1, int ed2, int ed3, int ed4, int pp, int sf_names, int ty_tags, int ty_d1, int ty_d2, int ty_d3, int allow_struct) {
  int _sv0t0 = parse_bit_or_expr(tags, starts, ends, source, pos, et, ed1, ed2, ed3, ed4, pp, sf_names, ty_tags, ty_d1, ty_d2, ty_d3, allow_struct);
  int p1 = _sv0t0;
  if ((p1 < 0)) {
    int _sv0t1 = (0 - 1);
    return _sv0t1;
  } else {
  }
  int _sv0t2 = ps_peek(tags, p1);
  int t = _sv0t2;
  int _sv0t3 = is_cmp_token(t);
  if ((_sv0t3 == 1)) {
    int _sv0t4 = binop_of(t);
    int bop = _sv0t4;
    int _sv0t5 = sv0_vec_len(et);
    int left = (_sv0t5 - 1);
    int _sv0t6 = (p1 + 1);
    int _sv0t7 = parse_bit_or_expr(tags, starts, ends, source, _sv0t6, et, ed1, ed2, ed3, ed4, pp, sf_names, ty_tags, ty_d1, ty_d2, ty_d3, allow_struct);
    int pa = _sv0t7;
    if ((pa < 0)) {
      int _sv0t8 = (0 - 1);
      return _sv0t8;
    } else {
    }
    int _sv0t9 = sv0_vec_len(et);
    int ri = (_sv0t9 - 1);
    sv0_vec_push(et, 3);
    sv0_vec_push(ed1, bop);
    sv0_vec_push(ed2, left);
    sv0_vec_push(ed3, ri);
    sv0_vec_push(ed4, 0);
    return pa;
  } else {
  }
  return p1;
}

static int parse_and_expr(int tags, int starts, int ends, const char* source, int pos, int et, int ed1, int ed2, int ed3, int ed4, int pp, int sf_names, int ty_tags, int ty_d1, int ty_d2, int ty_d3, int allow_struct) {
  int _sv0t0 = parse_cmp_expr(tags, starts, ends, source, pos, et, ed1, ed2, ed3, ed4, pp, sf_names, ty_tags, ty_d1, ty_d2, ty_d3, allow_struct);
  int p1 = _sv0t0;
  if ((p1 < 0)) {
    int _sv0t1 = (0 - 1);
    return _sv0t1;
  } else {
  }
  int _sv0t2 = sv0_vec_len(et);
  int left = (_sv0t2 - 1);
  int p = p1;
  int done = 0;
  while ((done != 1)) {
    int _sv0t3 = ps_peek(tags, p);
    int t = _sv0t3;
    if ((t != 28)) {
      done = 1;
    } else {
      int _sv0t4 = (p + 1);
      int _sv0t5 = parse_cmp_expr(tags, starts, ends, source, _sv0t4, et, ed1, ed2, ed3, ed4, pp, sf_names, ty_tags, ty_d1, ty_d2, ty_d3, allow_struct);
      int pa = _sv0t5;
      if ((pa < 0)) {
        int _sv0t6 = (0 - 1);
        return _sv0t6;
      } else {
      }
      int _sv0t7 = sv0_vec_len(et);
      int ri = (_sv0t7 - 1);
      sv0_vec_push(et, 3);
      sv0_vec_push(ed1, 10);
      sv0_vec_push(ed2, left);
      sv0_vec_push(ed3, ri);
      sv0_vec_push(ed4, 0);
      int _sv0t8 = sv0_vec_len(et);
      left = (_sv0t8 - 1);
      p = pa;
    }
  }
  return p;
}

static int parse_or_expr(int tags, int starts, int ends, const char* source, int pos, int et, int ed1, int ed2, int ed3, int ed4, int pp, int sf_names, int ty_tags, int ty_d1, int ty_d2, int ty_d3, int allow_struct) {
  int _sv0t0 = parse_and_expr(tags, starts, ends, source, pos, et, ed1, ed2, ed3, ed4, pp, sf_names, ty_tags, ty_d1, ty_d2, ty_d3, allow_struct);
  int p1 = _sv0t0;
  if ((p1 < 0)) {
    int _sv0t1 = (0 - 1);
    return _sv0t1;
  } else {
  }
  int _sv0t2 = sv0_vec_len(et);
  int left = (_sv0t2 - 1);
  int p = p1;
  int done = 0;
  while ((done != 1)) {
    int _sv0t3 = ps_peek(tags, p);
    int t = _sv0t3;
    if ((t != 30)) {
      done = 1;
    } else {
      int _sv0t4 = (p + 1);
      int _sv0t5 = parse_and_expr(tags, starts, ends, source, _sv0t4, et, ed1, ed2, ed3, ed4, pp, sf_names, ty_tags, ty_d1, ty_d2, ty_d3, allow_struct);
      int pa = _sv0t5;
      if ((pa < 0)) {
        int _sv0t6 = (0 - 1);
        return _sv0t6;
      } else {
      }
      int _sv0t7 = sv0_vec_len(et);
      int ri = (_sv0t7 - 1);
      sv0_vec_push(et, 3);
      sv0_vec_push(ed1, 11);
      sv0_vec_push(ed2, left);
      sv0_vec_push(ed3, ri);
      sv0_vec_push(ed4, 0);
      int _sv0t8 = sv0_vec_len(et);
      left = (_sv0t8 - 1);
      p = pa;
    }
  }
  return p;
}

static int parse_range_expr(int tags, int starts, int ends, const char* source, int pos, int et, int ed1, int ed2, int ed3, int ed4, int pp, int sf_names, int ty_tags, int ty_d1, int ty_d2, int ty_d3, int allow_struct) {
  int _sv0t0 = parse_or_expr(tags, starts, ends, source, pos, et, ed1, ed2, ed3, ed4, pp, sf_names, ty_tags, ty_d1, ty_d2, ty_d3, allow_struct);
  int p1 = _sv0t0;
  if ((p1 < 0)) {
    int _sv0t1 = (0 - 1);
    return _sv0t1;
  } else {
  }
  int _sv0t2 = ps_peek(tags, p1);
  int t = _sv0t2;
  if ((t == 17)) {
    int _sv0t3 = sv0_vec_len(et);
    int left_idx = (_sv0t3 - 1);
    int _sv0t4 = (p1 + 1);
    int _sv0t5 = parse_or_expr(tags, starts, ends, source, _sv0t4, et, ed1, ed2, ed3, ed4, pp, sf_names, ty_tags, ty_d1, ty_d2, ty_d3, allow_struct);
    int pa = _sv0t5;
    if ((pa < 0)) {
      int _sv0t6 = (0 - 1);
      return _sv0t6;
    } else {
    }
    int _sv0t7 = sv0_vec_len(et);
    int ri = (_sv0t7 - 1);
    sv0_vec_push(et, 21);
    sv0_vec_push(ed1, left_idx);
    sv0_vec_push(ed2, ri);
    sv0_vec_push(ed3, 0);
    sv0_vec_push(ed4, 0);
    return pa;
  } else {
  }
  return p1;
}

static int parse_expr(int tags, int starts, int ends, const char* source, int pos, int et, int ed1, int ed2, int ed3, int ed4, int pp, int allow_struct) {
  int _sv0t0 = sv0_vec_new();
  int sf = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int tt = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int td1 = _sv0t2;
  int _sv0t3 = sv0_vec_new();
  int td2 = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int td3 = _sv0t4;
  int _sv0t5 = parse_range_expr(tags, starts, ends, source, pos, et, ed1, ed2, ed3, ed4, pp, sf, tt, td1, td2, td3, allow_struct);
  return _sv0t5;
}

static int test_binop_of(void) {
  int _sv0t0 = binop_of(22);
  if ((_sv0t0 != 0)) {
    return 1;
  } else {
  }
  int _sv0t1 = binop_of(23);
  if ((_sv0t1 != 1)) {
    return 2;
  } else {
  }
  int _sv0t2 = binop_of(24);
  if ((_sv0t2 != 2)) {
    return 3;
  } else {
  }
  int _sv0t3 = binop_of(25);
  if ((_sv0t3 != 3)) {
    return 4;
  } else {
  }
  int _sv0t4 = binop_of(26);
  if ((_sv0t4 != 4)) {
    return 5;
  } else {
  }
  int _sv0t5 = binop_of(27);
  if ((_sv0t5 != 13)) {
    return 6;
  } else {
  }
  int _sv0t6 = binop_of(29);
  if ((_sv0t6 != 14)) {
    return 7;
  } else {
  }
  int _sv0t7 = binop_of(31);
  if ((_sv0t7 != 15)) {
    return 8;
  } else {
  }
  int _sv0t8 = binop_of(42);
  if ((_sv0t8 != 16)) {
    return 9;
  } else {
  }
  int _sv0t9 = binop_of(43);
  if ((_sv0t9 != 17)) {
    return 10;
  } else {
  }
  int _sv0t10 = binop_of(28);
  if ((_sv0t10 != 11)) {
    return 11;
  } else {
  }
  int _sv0t11 = binop_of(30);
  if ((_sv0t11 != 12)) {
    return 12;
  } else {
  }
  int _sv0t12 = binop_of(36);
  if ((_sv0t12 != 5)) {
    return 13;
  } else {
  }
  int _sv0t13 = binop_of(37);
  if ((_sv0t13 != 6)) {
    return 14;
  } else {
  }
  int _sv0t14 = binop_of(38);
  if ((_sv0t14 != 7)) {
    return 15;
  } else {
  }
  int _sv0t15 = binop_of(39);
  if ((_sv0t15 != 8)) {
    return 16;
  } else {
  }
  int _sv0t16 = binop_of(40);
  if ((_sv0t16 != 9)) {
    return 17;
  } else {
  }
  int _sv0t17 = binop_of(41);
  if ((_sv0t17 != 10)) {
    return 18;
  } else {
  }
  int _sv0t18 = binop_of(0);
  int _sv0t19 = (0 - 1);
  if ((_sv0t18 != _sv0t19)) {
    return 19;
  } else {
  }
  int _sv0t20 = has_binop(22);
  if ((_sv0t20 != 1)) {
    return 20;
  } else {
  }
  int _sv0t21 = has_binop(0);
  if ((_sv0t21 != 0)) {
    return 21;
  } else {
  }
  return 0;
}

static int test_assign_binop(void) {
  int _sv0t0 = assign_binop(44);
  if ((_sv0t0 != 0)) {
    return 1;
  } else {
  }
  int _sv0t1 = assign_binop(45);
  if ((_sv0t1 != 1)) {
    return 2;
  } else {
  }
  int _sv0t2 = assign_binop(46);
  if ((_sv0t2 != 2)) {
    return 3;
  } else {
  }
  int _sv0t3 = assign_binop(47);
  if ((_sv0t3 != 3)) {
    return 4;
  } else {
  }
  int _sv0t4 = assign_binop(48);
  if ((_sv0t4 != 4)) {
    return 5;
  } else {
  }
  int _sv0t5 = assign_binop(49);
  if ((_sv0t5 != 13)) {
    return 6;
  } else {
  }
  int _sv0t6 = assign_binop(50);
  if ((_sv0t6 != 14)) {
    return 7;
  } else {
  }
  int _sv0t7 = assign_binop(51);
  if ((_sv0t7 != 15)) {
    return 8;
  } else {
  }
  int _sv0t8 = assign_binop(52);
  if ((_sv0t8 != 16)) {
    return 9;
  } else {
  }
  int _sv0t9 = assign_binop(53);
  if ((_sv0t9 != 17)) {
    return 10;
  } else {
  }
  int _sv0t10 = assign_binop(0);
  int _sv0t11 = (0 - 1);
  if ((_sv0t10 != _sv0t11)) {
    return 11;
  } else {
  }
  int _sv0t12 = has_assign_binop(44);
  if ((_sv0t12 != 1)) {
    return 12;
  } else {
  }
  int _sv0t13 = has_assign_binop(0);
  if ((_sv0t13 != 0)) {
    return 13;
  } else {
  }
  int _sv0t14 = is_compound_assign(44);
  if ((_sv0t14 != 1)) {
    return 14;
  } else {
  }
  int _sv0t15 = is_compound_assign(53);
  if ((_sv0t15 != 1)) {
    return 15;
  } else {
  }
  int _sv0t16 = is_compound_assign(22);
  if ((_sv0t16 != 0)) {
    return 16;
  } else {
  }
  return 0;
}

static int test_is_assign_tok(void) {
  int _sv0t0 = is_assign_tok(35);
  if ((_sv0t0 != 1)) {
    return 1;
  } else {
  }
  int _sv0t1 = is_assign_tok(44);
  if ((_sv0t1 != 1)) {
    return 2;
  } else {
  }
  int _sv0t2 = is_assign_tok(53);
  if ((_sv0t2 != 1)) {
    return 3;
  } else {
  }
  int _sv0t3 = is_assign_tok(22);
  if ((_sv0t3 != 0)) {
    return 4;
  } else {
  }
  int _sv0t4 = is_assign_tok(0);
  if ((_sv0t4 != 0)) {
    return 5;
  } else {
  }
  int _sv0t5 = is_assign_tok(97);
  if ((_sv0t5 != 0)) {
    return 6;
  } else {
  }
  return 0;
}

static int test_path_of_ty(void) {
  int _sv0t0 = path_of_ty_valid(0);
  if ((_sv0t0 != 1)) {
    return 1;
  } else {
  }
  int _sv0t1 = path_of_ty_valid(1);
  if ((_sv0t1 != 0)) {
    return 2;
  } else {
  }
  int _sv0t2 = path_of_ty_valid(5);
  if ((_sv0t2 != 0)) {
    return 3;
  } else {
  }
  return 0;
}

static int test_lit_from_tok(void) {
  int _sv0t0 = lit_from_tok(0);
  if ((_sv0t0 != 0)) {
    return 1;
  } else {
  }
  int _sv0t1 = lit_from_tok(1);
  if ((_sv0t1 != 1)) {
    return 2;
  } else {
  }
  int _sv0t2 = lit_from_tok(2);
  if ((_sv0t2 != 3)) {
    return 3;
  } else {
  }
  int _sv0t3 = lit_from_tok(3);
  if ((_sv0t3 != 4)) {
    return 4;
  } else {
  }
  int _sv0t4 = lit_from_tok(91);
  if ((_sv0t4 != 5)) {
    return 5;
  } else {
  }
  int _sv0t5 = lit_from_tok(64);
  if ((_sv0t5 != 5)) {
    return 6;
  } else {
  }
  int _sv0t6 = lit_from_tok(5);
  int _sv0t7 = (0 - 1);
  if ((_sv0t6 != _sv0t7)) {
    return 7;
  } else {
  }
  int _sv0t8 = is_literal_tok(0);
  if ((_sv0t8 != 1)) {
    return 8;
  } else {
  }
  int _sv0t9 = is_literal_tok(5);
  if ((_sv0t9 != 0)) {
    return 9;
  } else {
  }
  return 0;
}

static int test_expr_span(void) {
  int _sv0t0 = expr_has_span(0);
  if ((_sv0t0 != 1)) {
    return 1;
  } else {
  }
  int _sv0t1 = expr_has_span(27);
  if ((_sv0t1 != 1)) {
    return 2;
  } else {
  }
  int _sv0t2 = expr_has_span(28);
  if ((_sv0t2 != 0)) {
    return 3;
  } else {
  }
  int _sv0t3 = (0 - 1);
  int _sv0t4 = expr_has_span(_sv0t3);
  if ((_sv0t4 != 0)) {
    return 4;
  } else {
  }
  return 0;
}

static int test_skip_helpers(void) {
  int _sv0t0 = is_pub_token(82);
  if ((_sv0t0 != 1)) {
    return 1;
  } else {
  }
  int _sv0t1 = is_pub_token(0);
  if ((_sv0t1 != 0)) {
    return 2;
  } else {
  }
  int _sv0t2 = is_unsafe_token(93);
  if ((_sv0t2 != 1)) {
    return 3;
  } else {
  }
  int _sv0t3 = is_where_token(95);
  if ((_sv0t3 != 1)) {
    return 4;
  } else {
  }
  int _sv0t4 = is_hash_token(21);
  if ((_sv0t4 != 1)) {
    return 5;
  } else {
  }
  int _sv0t5 = is_lt_token(38);
  if ((_sv0t5 != 1)) {
    return 6;
  } else {
  }
  int _sv0t6 = is_gt_token(39);
  if ((_sv0t6 != 1)) {
    return 7;
  } else {
  }
  int _sv0t7 = generic_params_update_depth(38, 1);
  if ((_sv0t7 != 2)) {
    return 8;
  } else {
  }
  int _sv0t8 = generic_params_update_depth(39, 2);
  if ((_sv0t8 != 1)) {
    return 9;
  } else {
  }
  int _sv0t9 = generic_params_update_depth(0, 1);
  if ((_sv0t9 != 1)) {
    return 10;
  } else {
  }
  int _sv0t10 = generic_params_done(0);
  if ((_sv0t10 != 1)) {
    return 11;
  } else {
  }
  int _sv0t11 = generic_params_done(1);
  if ((_sv0t11 != 0)) {
    return 12;
  } else {
  }
  int _sv0t12 = where_clause_stop(8);
  if ((_sv0t12 != 1)) {
    return 13;
  } else {
  }
  int _sv0t13 = where_clause_stop(65);
  if ((_sv0t13 != 1)) {
    return 14;
  } else {
  }
  int _sv0t14 = where_clause_stop(13);
  if ((_sv0t14 != 1)) {
    return 15;
  } else {
  }
  int _sv0t15 = where_clause_stop(97);
  if ((_sv0t15 != 1)) {
    return 16;
  } else {
  }
  int _sv0t16 = where_clause_stop(0);
  if ((_sv0t16 != 0)) {
    return 17;
  } else {
  }
  return 0;
}

static int test_span_merge(void) {
  int _sv0t0 = span_min(3, 5);
  if ((_sv0t0 != 3)) {
    return 1;
  } else {
  }
  int _sv0t1 = span_min(5, 3);
  if ((_sv0t1 != 3)) {
    return 2;
  } else {
  }
  int _sv0t2 = span_max(3, 5);
  if ((_sv0t2 != 5)) {
    return 3;
  } else {
  }
  int _sv0t3 = span_max(5, 3);
  if ((_sv0t3 != 5)) {
    return 4;
  } else {
  }
  int _sv0t4 = span_merge_start(10, 5);
  if ((_sv0t4 != 5)) {
    return 5;
  } else {
  }
  int _sv0t5 = span_merge_end(10, 20);
  if ((_sv0t5 != 20)) {
    return 6;
  } else {
  }
  return 0;
}

static int test_precedence(void) {
  int _sv0t0 = is_cmp_token(36);
  if ((_sv0t0 != 1)) {
    return 1;
  } else {
  }
  int _sv0t1 = is_cmp_token(41);
  if ((_sv0t1 != 1)) {
    return 2;
  } else {
  }
  int _sv0t2 = is_cmp_token(22);
  if ((_sv0t2 != 0)) {
    return 3;
  } else {
  }
  int _sv0t3 = is_additive_token(22);
  if ((_sv0t3 != 1)) {
    return 4;
  } else {
  }
  int _sv0t4 = is_additive_token(23);
  if ((_sv0t4 != 1)) {
    return 5;
  } else {
  }
  int _sv0t5 = is_additive_token(24);
  if ((_sv0t5 != 0)) {
    return 6;
  } else {
  }
  int _sv0t6 = is_multiplicative_token(24);
  if ((_sv0t6 != 1)) {
    return 7;
  } else {
  }
  int _sv0t7 = is_multiplicative_token(25);
  if ((_sv0t7 != 1)) {
    return 8;
  } else {
  }
  int _sv0t8 = is_multiplicative_token(26);
  if ((_sv0t8 != 1)) {
    return 9;
  } else {
  }
  int _sv0t9 = is_multiplicative_token(22);
  if ((_sv0t9 != 0)) {
    return 10;
  } else {
  }
  int _sv0t10 = is_shift_token(42);
  if ((_sv0t10 != 1)) {
    return 11;
  } else {
  }
  int _sv0t11 = is_shift_token(43);
  if ((_sv0t11 != 1)) {
    return 12;
  } else {
  }
  int _sv0t12 = is_shift_token(22);
  if ((_sv0t12 != 0)) {
    return 13;
  } else {
  }
  int _sv0t13 = is_unary_prefix(23);
  if ((_sv0t13 != 1)) {
    return 14;
  } else {
  }
  int _sv0t14 = is_unary_prefix(33);
  if ((_sv0t14 != 1)) {
    return 15;
  } else {
  }
  int _sv0t15 = is_unary_prefix(27);
  if ((_sv0t15 != 1)) {
    return 16;
  } else {
  }
  int _sv0t16 = is_unary_prefix(0);
  if ((_sv0t16 != 0)) {
    return 17;
  } else {
  }
  return 0;
}

static int test_ty_name(void) {
  int _sv0t0 = ty_is_name(0);
  if ((_sv0t0 != 1)) {
    return 1;
  } else {
  }
  int _sv0t1 = ty_is_name(1);
  if ((_sv0t1 != 0)) {
    return 2;
  } else {
  }
  return 0;
}

static int test_pratt_prec(void) {
  int _sv0t0 = prec_of_binop(12);
  if ((_sv0t0 != 1)) {
    return 1;
  } else {
  }
  int _sv0t1 = prec_of_binop(11);
  if ((_sv0t1 != 2)) {
    return 2;
  } else {
  }
  int _sv0t2 = prec_of_binop(5);
  if ((_sv0t2 != 3)) {
    return 3;
  } else {
  }
  int _sv0t3 = prec_of_binop(10);
  if ((_sv0t3 != 3)) {
    return 4;
  } else {
  }
  int _sv0t4 = prec_of_binop(0);
  if ((_sv0t4 != 8)) {
    return 5;
  } else {
  }
  int _sv0t5 = prec_of_binop(2);
  if ((_sv0t5 != 9)) {
    return 6;
  } else {
  }
  int _sv0t6 = prec_of_binop(16);
  if ((_sv0t6 != 7)) {
    return 7;
  } else {
  }
  int _sv0t7 = is_right_assoc(0);
  if ((_sv0t7 != 0)) {
    return 8;
  } else {
  }
  int _sv0t8 = next_prec(3, 0);
  if ((_sv0t8 != 4)) {
    return 9;
  } else {
  }
  int _sv0t9 = next_prec(3, 1);
  if ((_sv0t9 != 3)) {
    return 10;
  } else {
  }
  int _sv0t10 = binop_is_comparison(5);
  if ((_sv0t10 != 1)) {
    return 11;
  } else {
  }
  int _sv0t11 = binop_is_comparison(0);
  if ((_sv0t11 != 0)) {
    return 12;
  } else {
  }
  int _sv0t12 = binop_is_arithmetic(0);
  if ((_sv0t12 != 1)) {
    return 13;
  } else {
  }
  int _sv0t13 = binop_is_arithmetic(2);
  if ((_sv0t13 != 1)) {
    return 14;
  } else {
  }
  int _sv0t14 = binop_is_arithmetic(5);
  if ((_sv0t14 != 0)) {
    return 15;
  } else {
  }
  int _sv0t15 = binop_is_logical(11);
  if ((_sv0t15 != 1)) {
    return 16;
  } else {
  }
  int _sv0t16 = binop_is_logical(12);
  if ((_sv0t16 != 1)) {
    return 17;
  } else {
  }
  int _sv0t17 = binop_is_logical(0);
  if ((_sv0t17 != 0)) {
    return 18;
  } else {
  }
  return 0;
}

static int test_attr_bracket(void) {
  int _sv0t0 = attr_bracket_update(10, 0);
  if ((_sv0t0 != 1)) {
    return 1;
  } else {
  }
  int _sv0t1 = attr_bracket_update(11, 2);
  if ((_sv0t1 != 1)) {
    return 2;
  } else {
  }
  int _sv0t2 = attr_bracket_update(5, 1);
  if ((_sv0t2 != 1)) {
    return 3;
  } else {
  }
  return 0;
}

static int test_is_builtin_type_name(void) {
  const char* src;
  src = "i32 bool Foo string u8";
  int _sv0t0 = sv0_vec_new();
  int starts = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int ends = _sv0t1;
  sv0_vec_push(starts, 0);
  sv0_vec_push(ends, 3);
  sv0_vec_push(starts, 4);
  sv0_vec_push(ends, 8);
  sv0_vec_push(starts, 9);
  sv0_vec_push(ends, 12);
  sv0_vec_push(starts, 13);
  sv0_vec_push(ends, 19);
  sv0_vec_push(starts, 20);
  sv0_vec_push(ends, 22);
  int _sv0t2 = is_builtin_type_name(src, starts, ends, 0);
  if ((_sv0t2 != 1)) {
    return 1;
  } else {
  }
  int _sv0t3 = is_builtin_type_name(src, starts, ends, 1);
  if ((_sv0t3 != 1)) {
    return 2;
  } else {
  }
  int _sv0t4 = is_builtin_type_name(src, starts, ends, 2);
  if ((_sv0t4 != 0)) {
    return 3;
  } else {
  }
  int _sv0t5 = is_builtin_type_name(src, starts, ends, 3);
  if ((_sv0t5 != 1)) {
    return 4;
  } else {
  }
  int _sv0t6 = is_builtin_type_name(src, starts, ends, 4);
  if ((_sv0t6 != 1)) {
    return 5;
  } else {
  }
  return 0;
}

static int test_parse_type_unit(void) {
  const char* src;
  src = "()";
  int _sv0t0 = sv0_vec_new();
  int tags = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int starts = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int ends = _sv0t2;
  sv0_vec_push(tags, 6);
  sv0_vec_push(starts, 0);
  sv0_vec_push(ends, 1);
  sv0_vec_push(tags, 7);
  sv0_vec_push(starts, 1);
  sv0_vec_push(ends, 2);
  sv0_vec_push(tags, 97);
  sv0_vec_push(starts, 2);
  sv0_vec_push(ends, 2);
  int _sv0t3 = sv0_vec_new();
  int ty_tags = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int ty_d1 = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int ty_d2 = _sv0t5;
  int _sv0t6 = sv0_vec_new();
  int ty_d3 = _sv0t6;
  int _sv0t7 = sv0_vec_new();
  int pp = _sv0t7;
  int _sv0t8 = parse_type(tags, starts, ends, src, 0, ty_tags, ty_d1, ty_d2, ty_d3, pp);
  int p = _sv0t8;
  if ((p != 2)) {
    return 1;
  } else {
  }
  int _sv0t9 = sv0_vec_len(ty_tags);
  if ((_sv0t9 != 1)) {
    return 2;
  } else {
  }
  int _sv0t10 = sv0_vec_get(ty_tags, 0);
  if ((_sv0t10 != 6)) {
    return 3;
  } else {
  }
  return 0;
}

static int test_parse_type_builtin(void) {
  const char* src;
  src = "i32 end";
  int _sv0t0 = sv0_vec_new();
  int tags = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int starts = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int ends = _sv0t2;
  sv0_vec_push(tags, 5);
  sv0_vec_push(starts, 0);
  sv0_vec_push(ends, 3);
  sv0_vec_push(tags, 97);
  sv0_vec_push(starts, 4);
  sv0_vec_push(ends, 7);
  int _sv0t3 = sv0_vec_new();
  int ty_tags = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int ty_d1 = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int ty_d2 = _sv0t5;
  int _sv0t6 = sv0_vec_new();
  int ty_d3 = _sv0t6;
  int _sv0t7 = sv0_vec_new();
  int pp = _sv0t7;
  int _sv0t8 = parse_type(tags, starts, ends, src, 0, ty_tags, ty_d1, ty_d2, ty_d3, pp);
  int p = _sv0t8;
  if ((p != 1)) {
    return 1;
  } else {
  }
  int _sv0t9 = sv0_vec_len(ty_tags);
  if ((_sv0t9 != 1)) {
    return 2;
  } else {
  }
  int _sv0t10 = sv0_vec_get(ty_tags, 0);
  if ((_sv0t10 != 0)) {
    return 3;
  } else {
  }
  int _sv0t11 = sv0_vec_get(ty_d2, 0);
  if ((_sv0t11 != 1)) {
    return 4;
  } else {
  }
  int _sv0t12 = sv0_vec_get(ty_d3, 0);
  if ((_sv0t12 != 0)) {
    return 5;
  } else {
  }
  int _sv0t13 = sv0_vec_len(pp);
  if ((_sv0t13 != 1)) {
    return 6;
  } else {
  }
  int _sv0t14 = sv0_vec_get(pp, 0);
  if ((_sv0t14 != 0)) {
    return 7;
  } else {
  }
  return 0;
}

static int test_parse_type_ref(void) {
  const char* src;
  src = "&i32";
  int _sv0t0 = sv0_vec_new();
  int tags = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int starts = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int ends = _sv0t2;
  sv0_vec_push(tags, 27);
  sv0_vec_push(starts, 0);
  sv0_vec_push(ends, 1);
  sv0_vec_push(tags, 5);
  sv0_vec_push(starts, 1);
  sv0_vec_push(ends, 4);
  sv0_vec_push(tags, 97);
  sv0_vec_push(starts, 4);
  sv0_vec_push(ends, 4);
  int _sv0t3 = sv0_vec_new();
  int ty_tags = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int ty_d1 = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int ty_d2 = _sv0t5;
  int _sv0t6 = sv0_vec_new();
  int ty_d3 = _sv0t6;
  int _sv0t7 = sv0_vec_new();
  int pp = _sv0t7;
  int _sv0t8 = parse_type(tags, starts, ends, src, 0, ty_tags, ty_d1, ty_d2, ty_d3, pp);
  int p = _sv0t8;
  if ((p != 2)) {
    return 1;
  } else {
  }
  int _sv0t9 = sv0_vec_len(ty_tags);
  if ((_sv0t9 != 2)) {
    return 2;
  } else {
  }
  int _sv0t10 = sv0_vec_get(ty_tags, 0);
  if ((_sv0t10 != 0)) {
    return 3;
  } else {
  }
  int _sv0t11 = sv0_vec_get(ty_tags, 1);
  if ((_sv0t11 != 1)) {
    return 4;
  } else {
  }
  int _sv0t12 = sv0_vec_get(ty_d1, 1);
  if ((_sv0t12 != 0)) {
    return 5;
  } else {
  }
  return 0;
}

static int test_parse_type_ref_mut(void) {
  const char* src;
  src = "&mut i32";
  int _sv0t0 = sv0_vec_new();
  int tags = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int starts = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int ends = _sv0t2;
  sv0_vec_push(tags, 27);
  sv0_vec_push(starts, 0);
  sv0_vec_push(ends, 1);
  sv0_vec_push(tags, 77);
  sv0_vec_push(starts, 1);
  sv0_vec_push(ends, 4);
  sv0_vec_push(tags, 5);
  sv0_vec_push(starts, 5);
  sv0_vec_push(ends, 8);
  sv0_vec_push(tags, 97);
  sv0_vec_push(starts, 8);
  sv0_vec_push(ends, 8);
  int _sv0t3 = sv0_vec_new();
  int ty_tags = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int ty_d1 = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int ty_d2 = _sv0t5;
  int _sv0t6 = sv0_vec_new();
  int ty_d3 = _sv0t6;
  int _sv0t7 = sv0_vec_new();
  int pp = _sv0t7;
  int _sv0t8 = parse_type(tags, starts, ends, src, 0, ty_tags, ty_d1, ty_d2, ty_d3, pp);
  int p = _sv0t8;
  if ((p != 3)) {
    return 1;
  } else {
  }
  int _sv0t9 = sv0_vec_len(ty_tags);
  if ((_sv0t9 != 2)) {
    return 2;
  } else {
  }
  int _sv0t10 = sv0_vec_get(ty_tags, 0);
  if ((_sv0t10 != 0)) {
    return 3;
  } else {
  }
  int _sv0t11 = sv0_vec_get(ty_tags, 1);
  if ((_sv0t11 != 2)) {
    return 4;
  } else {
  }
  int _sv0t12 = sv0_vec_get(ty_d1, 1);
  if ((_sv0t12 != 0)) {
    return 5;
  } else {
  }
  return 0;
}

static int test_parse_type_slice(void) {
  const char* src;
  src = "[i32]";
  int _sv0t0 = sv0_vec_new();
  int tags = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int starts = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int ends = _sv0t2;
  sv0_vec_push(tags, 10);
  sv0_vec_push(starts, 0);
  sv0_vec_push(ends, 1);
  sv0_vec_push(tags, 5);
  sv0_vec_push(starts, 1);
  sv0_vec_push(ends, 4);
  sv0_vec_push(tags, 11);
  sv0_vec_push(starts, 4);
  sv0_vec_push(ends, 5);
  sv0_vec_push(tags, 97);
  sv0_vec_push(starts, 5);
  sv0_vec_push(ends, 5);
  int _sv0t3 = sv0_vec_new();
  int ty_tags = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int ty_d1 = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int ty_d2 = _sv0t5;
  int _sv0t6 = sv0_vec_new();
  int ty_d3 = _sv0t6;
  int _sv0t7 = sv0_vec_new();
  int pp = _sv0t7;
  int _sv0t8 = parse_type(tags, starts, ends, src, 0, ty_tags, ty_d1, ty_d2, ty_d3, pp);
  int p = _sv0t8;
  if ((p != 3)) {
    return 1;
  } else {
  }
  int _sv0t9 = sv0_vec_len(ty_tags);
  if ((_sv0t9 != 2)) {
    return 2;
  } else {
  }
  int _sv0t10 = sv0_vec_get(ty_tags, 0);
  if ((_sv0t10 != 0)) {
    return 3;
  } else {
  }
  int _sv0t11 = sv0_vec_get(ty_tags, 1);
  if ((_sv0t11 != 4)) {
    return 4;
  } else {
  }
  int _sv0t12 = sv0_vec_get(ty_d1, 1);
  if ((_sv0t12 != 0)) {
    return 5;
  } else {
  }
  return 0;
}

static int test_parse_type_array(void) {
  const char* src;
  src = "[i32; 5]";
  int _sv0t0 = sv0_vec_new();
  int tags = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int starts = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int ends = _sv0t2;
  sv0_vec_push(tags, 10);
  sv0_vec_push(starts, 0);
  sv0_vec_push(ends, 1);
  sv0_vec_push(tags, 5);
  sv0_vec_push(starts, 1);
  sv0_vec_push(ends, 4);
  sv0_vec_push(tags, 13);
  sv0_vec_push(starts, 4);
  sv0_vec_push(ends, 5);
  sv0_vec_push(tags, 0);
  sv0_vec_push(starts, 6);
  sv0_vec_push(ends, 7);
  sv0_vec_push(tags, 11);
  sv0_vec_push(starts, 7);
  sv0_vec_push(ends, 8);
  sv0_vec_push(tags, 97);
  sv0_vec_push(starts, 8);
  sv0_vec_push(ends, 8);
  int _sv0t3 = sv0_vec_new();
  int ty_tags = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int ty_d1 = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int ty_d2 = _sv0t5;
  int _sv0t6 = sv0_vec_new();
  int ty_d3 = _sv0t6;
  int _sv0t7 = sv0_vec_new();
  int pp = _sv0t7;
  int _sv0t8 = parse_type(tags, starts, ends, src, 0, ty_tags, ty_d1, ty_d2, ty_d3, pp);
  int p = _sv0t8;
  if ((p != 5)) {
    return 1;
  } else {
  }
  int _sv0t9 = sv0_vec_len(ty_tags);
  if ((_sv0t9 != 2)) {
    return 2;
  } else {
  }
  int _sv0t10 = sv0_vec_get(ty_tags, 0);
  if ((_sv0t10 != 0)) {
    return 3;
  } else {
  }
  int _sv0t11 = sv0_vec_get(ty_tags, 1);
  if ((_sv0t11 != 3)) {
    return 4;
  } else {
  }
  int _sv0t12 = sv0_vec_get(ty_d1, 1);
  if ((_sv0t12 != 0)) {
    return 5;
  } else {
  }
  int _sv0t13 = sv0_vec_get(ty_d2, 1);
  if ((_sv0t13 != 3)) {
    return 6;
  } else {
  }
  return 0;
}

static int test_parse_type_generic(void) {
  const char* src;
  src = "Vec<i32>";
  int _sv0t0 = sv0_vec_new();
  int tags = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int starts = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int ends = _sv0t2;
  sv0_vec_push(tags, 5);
  sv0_vec_push(starts, 0);
  sv0_vec_push(ends, 3);
  sv0_vec_push(tags, 38);
  sv0_vec_push(starts, 3);
  sv0_vec_push(ends, 4);
  sv0_vec_push(tags, 5);
  sv0_vec_push(starts, 4);
  sv0_vec_push(ends, 7);
  sv0_vec_push(tags, 39);
  sv0_vec_push(starts, 7);
  sv0_vec_push(ends, 8);
  sv0_vec_push(tags, 97);
  sv0_vec_push(starts, 8);
  sv0_vec_push(ends, 8);
  int _sv0t3 = sv0_vec_new();
  int ty_tags = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int ty_d1 = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int ty_d2 = _sv0t5;
  int _sv0t6 = sv0_vec_new();
  int ty_d3 = _sv0t6;
  int _sv0t7 = sv0_vec_new();
  int pp = _sv0t7;
  int _sv0t8 = parse_type(tags, starts, ends, src, 0, ty_tags, ty_d1, ty_d2, ty_d3, pp);
  int p = _sv0t8;
  if ((p != 4)) {
    return 1;
  } else {
  }
  int _sv0t9 = sv0_vec_len(ty_tags);
  if ((_sv0t9 != 2)) {
    return 2;
  } else {
  }
  int _sv0t10 = sv0_vec_get(ty_tags, 0);
  if ((_sv0t10 != 0)) {
    return 3;
  } else {
  }
  int _sv0t11 = sv0_vec_get(ty_d3, 0);
  if ((_sv0t11 != 0)) {
    return 4;
  } else {
  }
  int _sv0t12 = sv0_vec_get(ty_tags, 1);
  if ((_sv0t12 != 0)) {
    return 5;
  } else {
  }
  int _sv0t13 = sv0_vec_get(ty_d2, 1);
  if ((_sv0t13 != 1)) {
    return 6;
  } else {
  }
  int _sv0t14 = sv0_vec_get(ty_d3, 1);
  if ((_sv0t14 != 1)) {
    return 7;
  } else {
  }
  return 0;
}

static int test_parse_type_paren(void) {
  const char* src;
  src = "(i32)";
  int _sv0t0 = sv0_vec_new();
  int tags = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int starts = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int ends = _sv0t2;
  sv0_vec_push(tags, 6);
  sv0_vec_push(starts, 0);
  sv0_vec_push(ends, 1);
  sv0_vec_push(tags, 5);
  sv0_vec_push(starts, 1);
  sv0_vec_push(ends, 4);
  sv0_vec_push(tags, 7);
  sv0_vec_push(starts, 4);
  sv0_vec_push(ends, 5);
  sv0_vec_push(tags, 97);
  sv0_vec_push(starts, 5);
  sv0_vec_push(ends, 5);
  int _sv0t3 = sv0_vec_new();
  int ty_tags = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int ty_d1 = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int ty_d2 = _sv0t5;
  int _sv0t6 = sv0_vec_new();
  int ty_d3 = _sv0t6;
  int _sv0t7 = sv0_vec_new();
  int pp = _sv0t7;
  int _sv0t8 = parse_type(tags, starts, ends, src, 0, ty_tags, ty_d1, ty_d2, ty_d3, pp);
  int p = _sv0t8;
  if ((p != 3)) {
    return 1;
  } else {
  }
  int _sv0t9 = sv0_vec_len(ty_tags);
  if ((_sv0t9 != 1)) {
    return 2;
  } else {
  }
  int _sv0t10 = sv0_vec_get(ty_tags, 0);
  if ((_sv0t10 != 0)) {
    return 3;
  } else {
  }
  return 0;
}

static int test_parse_pat_wild(void) {
  const char* src;
  src = "_";
  int _sv0t0 = sv0_vec_new();
  int tags = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int starts = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int ends = _sv0t2;
  sv0_vec_push(tags, 5);
  sv0_vec_push(starts, 0);
  sv0_vec_push(ends, 1);
  sv0_vec_push(tags, 97);
  sv0_vec_push(starts, 1);
  sv0_vec_push(ends, 1);
  int _sv0t3 = sv0_vec_new();
  int pt = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int pd1 = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int pd2 = _sv0t5;
  int _sv0t6 = sv0_vec_new();
  int pd3 = _sv0t6;
  int _sv0t7 = sv0_vec_new();
  int pp = _sv0t7;
  int _sv0t8 = parse_pat(tags, starts, ends, src, 0, pt, pd1, pd2, pd3, pp);
  int p = _sv0t8;
  if ((p != 1)) {
    return 1;
  } else {
  }
  int _sv0t9 = sv0_vec_len(pt);
  if ((_sv0t9 != 1)) {
    return 2;
  } else {
  }
  int _sv0t10 = sv0_vec_get(pt, 0);
  if ((_sv0t10 != 0)) {
    return 3;
  } else {
  }
  return 0;
}

static int test_parse_pat_bind(void) {
  const char* src;
  src = "x";
  int _sv0t0 = sv0_vec_new();
  int tags = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int starts = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int ends = _sv0t2;
  sv0_vec_push(tags, 5);
  sv0_vec_push(starts, 0);
  sv0_vec_push(ends, 1);
  sv0_vec_push(tags, 97);
  sv0_vec_push(starts, 1);
  sv0_vec_push(ends, 1);
  int _sv0t3 = sv0_vec_new();
  int pt = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int pd1 = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int pd2 = _sv0t5;
  int _sv0t6 = sv0_vec_new();
  int pd3 = _sv0t6;
  int _sv0t7 = sv0_vec_new();
  int pp = _sv0t7;
  int _sv0t8 = parse_pat(tags, starts, ends, src, 0, pt, pd1, pd2, pd3, pp);
  int p = _sv0t8;
  if ((p != 1)) {
    return 1;
  } else {
  }
  int _sv0t9 = sv0_vec_len(pt);
  if ((_sv0t9 != 1)) {
    return 2;
  } else {
  }
  int _sv0t10 = sv0_vec_get(pt, 0);
  if ((_sv0t10 != 1)) {
    return 3;
  } else {
  }
  int _sv0t11 = sv0_vec_get(pd1, 0);
  if ((_sv0t11 != 0)) {
    return 4;
  } else {
  }
  int _sv0t12 = sv0_vec_get(pd2, 0);
  if ((_sv0t12 != 0)) {
    return 5;
  } else {
  }
  return 0;
}

static int test_parse_pat_bind_mut(void) {
  const char* src;
  src = "mut x";
  int _sv0t0 = sv0_vec_new();
  int tags = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int starts = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int ends = _sv0t2;
  sv0_vec_push(tags, 77);
  sv0_vec_push(starts, 0);
  sv0_vec_push(ends, 3);
  sv0_vec_push(tags, 5);
  sv0_vec_push(starts, 4);
  sv0_vec_push(ends, 5);
  sv0_vec_push(tags, 97);
  sv0_vec_push(starts, 5);
  sv0_vec_push(ends, 5);
  int _sv0t3 = sv0_vec_new();
  int pt = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int pd1 = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int pd2 = _sv0t5;
  int _sv0t6 = sv0_vec_new();
  int pd3 = _sv0t6;
  int _sv0t7 = sv0_vec_new();
  int pp = _sv0t7;
  int _sv0t8 = parse_pat(tags, starts, ends, src, 0, pt, pd1, pd2, pd3, pp);
  int p = _sv0t8;
  if ((p != 2)) {
    return 1;
  } else {
  }
  int _sv0t9 = sv0_vec_len(pt);
  if ((_sv0t9 != 1)) {
    return 2;
  } else {
  }
  int _sv0t10 = sv0_vec_get(pt, 0);
  if ((_sv0t10 != 1)) {
    return 3;
  } else {
  }
  int _sv0t11 = sv0_vec_get(pd1, 0);
  if ((_sv0t11 != 1)) {
    return 4;
  } else {
  }
  int _sv0t12 = sv0_vec_get(pd2, 0);
  if ((_sv0t12 != 1)) {
    return 5;
  } else {
  }
  return 0;
}

static int test_parse_pat_lit(void) {
  const char* src;
  src = "42";
  int _sv0t0 = sv0_vec_new();
  int tags = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int starts = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int ends = _sv0t2;
  sv0_vec_push(tags, 0);
  sv0_vec_push(starts, 0);
  sv0_vec_push(ends, 2);
  sv0_vec_push(tags, 97);
  sv0_vec_push(starts, 2);
  sv0_vec_push(ends, 2);
  int _sv0t3 = sv0_vec_new();
  int pt = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int pd1 = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int pd2 = _sv0t5;
  int _sv0t6 = sv0_vec_new();
  int pd3 = _sv0t6;
  int _sv0t7 = sv0_vec_new();
  int pp = _sv0t7;
  int _sv0t8 = parse_pat(tags, starts, ends, src, 0, pt, pd1, pd2, pd3, pp);
  int p = _sv0t8;
  if ((p != 1)) {
    return 1;
  } else {
  }
  int _sv0t9 = sv0_vec_len(pt);
  if ((_sv0t9 != 1)) {
    return 2;
  } else {
  }
  int _sv0t10 = sv0_vec_get(pt, 0);
  if ((_sv0t10 != 2)) {
    return 3;
  } else {
  }
  int _sv0t11 = sv0_vec_get(pd1, 0);
  if ((_sv0t11 != 0)) {
    return 4;
  } else {
  }
  int _sv0t12 = sv0_vec_get(pd3, 0);
  if ((_sv0t12 != 0)) {
    return 5;
  } else {
  }
  return 0;
}

static int test_parse_pat_neg_lit(void) {
  const char* src;
  src = "-42";
  int _sv0t0 = sv0_vec_new();
  int tags = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int starts = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int ends = _sv0t2;
  sv0_vec_push(tags, 23);
  sv0_vec_push(starts, 0);
  sv0_vec_push(ends, 1);
  sv0_vec_push(tags, 0);
  sv0_vec_push(starts, 1);
  sv0_vec_push(ends, 3);
  sv0_vec_push(tags, 97);
  sv0_vec_push(starts, 3);
  sv0_vec_push(ends, 3);
  int _sv0t3 = sv0_vec_new();
  int pt = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int pd1 = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int pd2 = _sv0t5;
  int _sv0t6 = sv0_vec_new();
  int pd3 = _sv0t6;
  int _sv0t7 = sv0_vec_new();
  int pp = _sv0t7;
  int _sv0t8 = parse_pat(tags, starts, ends, src, 0, pt, pd1, pd2, pd3, pp);
  int p = _sv0t8;
  if ((p != 2)) {
    return 1;
  } else {
  }
  int _sv0t9 = sv0_vec_len(pt);
  if ((_sv0t9 != 1)) {
    return 2;
  } else {
  }
  int _sv0t10 = sv0_vec_get(pt, 0);
  if ((_sv0t10 != 2)) {
    return 3;
  } else {
  }
  int _sv0t11 = sv0_vec_get(pd1, 0);
  if ((_sv0t11 != 0)) {
    return 4;
  } else {
  }
  int _sv0t12 = sv0_vec_get(pd3, 0);
  if ((_sv0t12 != 1)) {
    return 5;
  } else {
  }
  return 0;
}

static int test_parse_pat_unit(void) {
  const char* src;
  src = "()";
  int _sv0t0 = sv0_vec_new();
  int tags = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int starts = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int ends = _sv0t2;
  sv0_vec_push(tags, 6);
  sv0_vec_push(starts, 0);
  sv0_vec_push(ends, 1);
  sv0_vec_push(tags, 7);
  sv0_vec_push(starts, 1);
  sv0_vec_push(ends, 2);
  sv0_vec_push(tags, 97);
  sv0_vec_push(starts, 2);
  sv0_vec_push(ends, 2);
  int _sv0t3 = sv0_vec_new();
  int pt = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int pd1 = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int pd2 = _sv0t5;
  int _sv0t6 = sv0_vec_new();
  int pd3 = _sv0t6;
  int _sv0t7 = sv0_vec_new();
  int pp = _sv0t7;
  int _sv0t8 = parse_pat(tags, starts, ends, src, 0, pt, pd1, pd2, pd3, pp);
  int p = _sv0t8;
  if ((p != 2)) {
    return 1;
  } else {
  }
  int _sv0t9 = sv0_vec_len(pt);
  if ((_sv0t9 != 1)) {
    return 2;
  } else {
  }
  int _sv0t10 = sv0_vec_get(pt, 0);
  if ((_sv0t10 != 3)) {
    return 3;
  } else {
  }
  int _sv0t11 = sv0_vec_get(pd2, 0);
  if ((_sv0t11 != 0)) {
    return 4;
  } else {
  }
  return 0;
}

static int test_parse_pat_tuple(void) {
  const char* src;
  src = "(x, y)";
  int _sv0t0 = sv0_vec_new();
  int tags = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int starts = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int ends = _sv0t2;
  sv0_vec_push(tags, 6);
  sv0_vec_push(starts, 0);
  sv0_vec_push(ends, 1);
  sv0_vec_push(tags, 5);
  sv0_vec_push(starts, 1);
  sv0_vec_push(ends, 2);
  sv0_vec_push(tags, 12);
  sv0_vec_push(starts, 2);
  sv0_vec_push(ends, 3);
  sv0_vec_push(tags, 5);
  sv0_vec_push(starts, 4);
  sv0_vec_push(ends, 5);
  sv0_vec_push(tags, 7);
  sv0_vec_push(starts, 5);
  sv0_vec_push(ends, 6);
  sv0_vec_push(tags, 97);
  sv0_vec_push(starts, 6);
  sv0_vec_push(ends, 6);
  int _sv0t3 = sv0_vec_new();
  int pt = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int pd1 = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int pd2 = _sv0t5;
  int _sv0t6 = sv0_vec_new();
  int pd3 = _sv0t6;
  int _sv0t7 = sv0_vec_new();
  int pp = _sv0t7;
  int _sv0t8 = parse_pat(tags, starts, ends, src, 0, pt, pd1, pd2, pd3, pp);
  int p = _sv0t8;
  if ((p != 5)) {
    return 1;
  } else {
  }
  int _sv0t9 = sv0_vec_len(pt);
  if ((_sv0t9 != 3)) {
    return 2;
  } else {
  }
  int _sv0t10 = sv0_vec_get(pt, 0);
  if ((_sv0t10 != 1)) {
    return 3;
  } else {
  }
  int _sv0t11 = sv0_vec_get(pt, 1);
  if ((_sv0t11 != 1)) {
    return 4;
  } else {
  }
  int _sv0t12 = sv0_vec_get(pt, 2);
  if ((_sv0t12 != 3)) {
    return 5;
  } else {
  }
  int _sv0t13 = sv0_vec_get(pd2, 2);
  if ((_sv0t13 != 2)) {
    return 6;
  } else {
  }
  return 0;
}

static int test_parse_pat_enum(void) {
  const char* src;
  src = "Foo::Bar(x)";
  int _sv0t0 = sv0_vec_new();
  int tags = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int starts = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int ends = _sv0t2;
  sv0_vec_push(tags, 5);
  sv0_vec_push(starts, 0);
  sv0_vec_push(ends, 3);
  sv0_vec_push(tags, 15);
  sv0_vec_push(starts, 3);
  sv0_vec_push(ends, 5);
  sv0_vec_push(tags, 5);
  sv0_vec_push(starts, 5);
  sv0_vec_push(ends, 8);
  sv0_vec_push(tags, 6);
  sv0_vec_push(starts, 8);
  sv0_vec_push(ends, 9);
  sv0_vec_push(tags, 5);
  sv0_vec_push(starts, 9);
  sv0_vec_push(ends, 10);
  sv0_vec_push(tags, 7);
  sv0_vec_push(starts, 10);
  sv0_vec_push(ends, 11);
  sv0_vec_push(tags, 97);
  sv0_vec_push(starts, 11);
  sv0_vec_push(ends, 11);
  int _sv0t3 = sv0_vec_new();
  int pt = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int pd1 = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int pd2 = _sv0t5;
  int _sv0t6 = sv0_vec_new();
  int pd3 = _sv0t6;
  int _sv0t7 = sv0_vec_new();
  int pp = _sv0t7;
  int _sv0t8 = parse_pat(tags, starts, ends, src, 0, pt, pd1, pd2, pd3, pp);
  int p = _sv0t8;
  if ((p != 6)) {
    return 1;
  } else {
  }
  int _sv0t9 = sv0_vec_get(pt, 0);
  if ((_sv0t9 != 1)) {
    return 2;
  } else {
  }
  int _sv0t10 = sv0_vec_get(pt, 1);
  if ((_sv0t10 != 5)) {
    return 3;
  } else {
  }
  int _sv0t11 = sv0_vec_get(pd3, 1);
  if ((_sv0t11 != 1)) {
    return 4;
  } else {
  }
  int _sv0t12 = sv0_vec_len(pp);
  if ((_sv0t12 != 2)) {
    return 5;
  } else {
  }
  return 0;
}

static int test_parse_pat_or(void) {
  const char* src;
  src = "x | y";
  int _sv0t0 = sv0_vec_new();
  int tags = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int starts = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int ends = _sv0t2;
  sv0_vec_push(tags, 5);
  sv0_vec_push(starts, 0);
  sv0_vec_push(ends, 1);
  sv0_vec_push(tags, 29);
  sv0_vec_push(starts, 2);
  sv0_vec_push(ends, 3);
  sv0_vec_push(tags, 5);
  sv0_vec_push(starts, 4);
  sv0_vec_push(ends, 5);
  sv0_vec_push(tags, 97);
  sv0_vec_push(starts, 5);
  sv0_vec_push(ends, 5);
  int _sv0t3 = sv0_vec_new();
  int pt = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int pd1 = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int pd2 = _sv0t5;
  int _sv0t6 = sv0_vec_new();
  int pd3 = _sv0t6;
  int _sv0t7 = sv0_vec_new();
  int pp = _sv0t7;
  int _sv0t8 = parse_pat(tags, starts, ends, src, 0, pt, pd1, pd2, pd3, pp);
  int p = _sv0t8;
  if ((p != 3)) {
    return 1;
  } else {
  }
  int _sv0t9 = sv0_vec_len(pt);
  if ((_sv0t9 != 3)) {
    return 2;
  } else {
  }
  int _sv0t10 = sv0_vec_get(pt, 0);
  if ((_sv0t10 != 1)) {
    return 3;
  } else {
  }
  int _sv0t11 = sv0_vec_get(pt, 1);
  if ((_sv0t11 != 1)) {
    return 4;
  } else {
  }
  int _sv0t12 = sv0_vec_get(pt, 2);
  if ((_sv0t12 != 6)) {
    return 5;
  } else {
  }
  int _sv0t13 = sv0_vec_get(pd1, 2);
  if ((_sv0t13 != 0)) {
    return 6;
  } else {
  }
  int _sv0t14 = sv0_vec_get(pd2, 2);
  if ((_sv0t14 != 2)) {
    return 7;
  } else {
  }
  return 0;
}

static int test_parse_pat_paren(void) {
  const char* src;
  src = "(x)";
  int _sv0t0 = sv0_vec_new();
  int tags = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int starts = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int ends = _sv0t2;
  sv0_vec_push(tags, 6);
  sv0_vec_push(starts, 0);
  sv0_vec_push(ends, 1);
  sv0_vec_push(tags, 5);
  sv0_vec_push(starts, 1);
  sv0_vec_push(ends, 2);
  sv0_vec_push(tags, 7);
  sv0_vec_push(starts, 2);
  sv0_vec_push(ends, 3);
  sv0_vec_push(tags, 97);
  sv0_vec_push(starts, 3);
  sv0_vec_push(ends, 3);
  int _sv0t3 = sv0_vec_new();
  int pt = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int pd1 = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int pd2 = _sv0t5;
  int _sv0t6 = sv0_vec_new();
  int pd3 = _sv0t6;
  int _sv0t7 = sv0_vec_new();
  int pp = _sv0t7;
  int _sv0t8 = parse_pat(tags, starts, ends, src, 0, pt, pd1, pd2, pd3, pp);
  int p = _sv0t8;
  if ((p != 3)) {
    return 1;
  } else {
  }
  int _sv0t9 = sv0_vec_len(pt);
  if ((_sv0t9 != 1)) {
    return 2;
  } else {
  }
  int _sv0t10 = sv0_vec_get(pt, 0);
  if ((_sv0t10 != 1)) {
    return 3;
  } else {
  }
  return 0;
}

static int test_parse_pat_bare_enum(void) {
  const char* src;
  src = "Foo::Bar";
  int _sv0t0 = sv0_vec_new();
  int tags = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int starts = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int ends = _sv0t2;
  sv0_vec_push(tags, 5);
  sv0_vec_push(starts, 0);
  sv0_vec_push(ends, 3);
  sv0_vec_push(tags, 15);
  sv0_vec_push(starts, 3);
  sv0_vec_push(ends, 5);
  sv0_vec_push(tags, 5);
  sv0_vec_push(starts, 5);
  sv0_vec_push(ends, 8);
  sv0_vec_push(tags, 97);
  sv0_vec_push(starts, 8);
  sv0_vec_push(ends, 8);
  int _sv0t3 = sv0_vec_new();
  int pt = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int pd1 = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int pd2 = _sv0t5;
  int _sv0t6 = sv0_vec_new();
  int pd3 = _sv0t6;
  int _sv0t7 = sv0_vec_new();
  int pp = _sv0t7;
  int _sv0t8 = parse_pat(tags, starts, ends, src, 0, pt, pd1, pd2, pd3, pp);
  int p = _sv0t8;
  if ((p != 3)) {
    return 1;
  } else {
  }
  int _sv0t9 = sv0_vec_len(pt);
  if ((_sv0t9 != 1)) {
    return 2;
  } else {
  }
  int _sv0t10 = sv0_vec_get(pt, 0);
  if ((_sv0t10 != 5)) {
    return 3;
  } else {
  }
  int _sv0t11 = sv0_vec_get(pd3, 0);
  if ((_sv0t11 != 0)) {
    return 4;
  } else {
  }
  int _sv0t12 = sv0_vec_len(pp);
  if ((_sv0t12 != 2)) {
    return 5;
  } else {
  }
  return 0;
}

static int test_ps_nav(void) {
  int _sv0t0 = sv0_vec_new();
  int tags = _sv0t0;
  sv0_vec_push(tags, 5);
  sv0_vec_push(tags, 22);
  sv0_vec_push(tags, 5);
  sv0_vec_push(tags, 97);
  int _sv0t1 = ps_peek(tags, 0);
  if ((_sv0t1 != 5)) {
    return 1;
  } else {
  }
  int _sv0t2 = ps_peek(tags, 1);
  if ((_sv0t2 != 22)) {
    return 2;
  } else {
  }
  int _sv0t3 = ps_peek(tags, 3);
  if ((_sv0t3 != 97)) {
    return 3;
  } else {
  }
  int _sv0t4 = ps_peek(tags, 10);
  if ((_sv0t4 != 97)) {
    return 4;
  } else {
  }
  int _sv0t5 = ps_at_end(tags, 3);
  if ((_sv0t5 != 1)) {
    return 5;
  } else {
  }
  int _sv0t6 = ps_at_end(tags, 0);
  if ((_sv0t6 != 0)) {
    return 6;
  } else {
  }
  int _sv0t7 = ps_advance(0);
  if ((_sv0t7 != 1)) {
    return 7;
  } else {
  }
  int _sv0t8 = ps_expect(tags, 0, 5);
  if ((_sv0t8 != 1)) {
    return 8;
  } else {
  }
  int _sv0t9 = ps_expect(tags, 0, 22);
  int _sv0t10 = (0 - 1);
  if ((_sv0t9 != _sv0t10)) {
    return 9;
  } else {
  }
  int _sv0t11 = ps_match(tags, 0, 5);
  if ((_sv0t11 != 1)) {
    return 10;
  } else {
  }
  int _sv0t12 = ps_match(tags, 0, 22);
  if ((_sv0t12 != 0)) {
    return 11;
  } else {
  }
  int _sv0t13 = sv0_vec_new();
  int starts = _sv0t13;
  int _sv0t14 = sv0_vec_new();
  int ends = _sv0t14;
  sv0_vec_push(starts, 0);
  sv0_vec_push(starts, 3);
  sv0_vec_push(ends, 3);
  sv0_vec_push(ends, 4);
  int _sv0t15 = ps_start(starts, 0);
  if ((_sv0t15 != 0)) {
    return 12;
  } else {
  }
  int _sv0t16 = ps_end(ends, 0);
  if ((_sv0t16 != 3)) {
    return 13;
  } else {
  }
  const char* src;
  src = "foo+bar";
  const char* _sv0t17 = ps_tok_text(src, starts, ends, 0);
  const char* txt;
  txt = _sv0t17;
  int _sv0t18 = sv0_string_len(txt);
  if ((_sv0t18 != 3)) {
    return 14;
  } else {
  }
  return 0;
}

static int test_parse_path(void) {
  int _sv0t0 = sv0_vec_new();
  int tags = _sv0t0;
  sv0_vec_push(tags, 5);
  sv0_vec_push(tags, 15);
  sv0_vec_push(tags, 5);
  sv0_vec_push(tags, 15);
  sv0_vec_push(tags, 5);
  sv0_vec_push(tags, 22);
  sv0_vec_push(tags, 97);
  int _sv0t1 = sv0_vec_new();
  int path = _sv0t1;
  int _sv0t2 = parse_path(tags, 0, path);
  int end_pos = _sv0t2;
  int _sv0t3 = sv0_vec_len(path);
  if ((_sv0t3 != 3)) {
    return 1;
  } else {
  }
  int _sv0t4 = sv0_vec_get(path, 0);
  if ((_sv0t4 != 0)) {
    return 2;
  } else {
  }
  int _sv0t5 = sv0_vec_get(path, 1);
  if ((_sv0t5 != 2)) {
    return 3;
  } else {
  }
  int _sv0t6 = sv0_vec_get(path, 2);
  if ((_sv0t6 != 4)) {
    return 4;
  } else {
  }
  if ((end_pos != 5)) {
    return 5;
  } else {
  }
  int _sv0t7 = sv0_vec_new();
  int path2 = _sv0t7;
  int _sv0t8 = parse_path(tags, 0, path2);
  int end2 = _sv0t8;
  int _sv0t9 = sv0_vec_len(path2);
  if ((_sv0t9 != 3)) {
    return 6;
  } else {
  }
  int _sv0t10 = sv0_vec_new();
  int single = _sv0t10;
  int _sv0t11 = sv0_vec_new();
  int tags2 = _sv0t11;
  sv0_vec_push(tags2, 5);
  sv0_vec_push(tags2, 22);
  sv0_vec_push(tags2, 97);
  int _sv0t12 = parse_path(tags2, 0, single);
  int end3 = _sv0t12;
  int _sv0t13 = sv0_vec_len(single);
  if ((_sv0t13 != 1)) {
    return 7;
  } else {
  }
  if ((end3 != 1)) {
    return 8;
  } else {
  }
  int _sv0t14 = sv0_vec_new();
  int empty = _sv0t14;
  int _sv0t15 = parse_path(tags2, 1, empty);
  int fail = _sv0t15;
  int _sv0t16 = (0 - 1);
  if ((fail != _sv0t16)) {
    return 9;
  } else {
  }
  return 0;
}

static int test_ps_skip(void) {
  int _sv0t0 = sv0_vec_new();
  int tags = _sv0t0;
  sv0_vec_push(tags, 82);
  sv0_vec_push(tags, 65);
  sv0_vec_push(tags, 97);
  int _sv0t1 = ps_skip_optional_pub(tags, 0);
  if ((_sv0t1 != 1)) {
    return 1;
  } else {
  }
  int _sv0t2 = ps_skip_optional_pub(tags, 1);
  if ((_sv0t2 != 1)) {
    return 2;
  } else {
  }
  int _sv0t3 = sv0_vec_new();
  int tags2 = _sv0t3;
  sv0_vec_push(tags2, 82);
  sv0_vec_push(tags2, 6);
  sv0_vec_push(tags2, 81);
  sv0_vec_push(tags2, 7);
  sv0_vec_push(tags2, 65);
  sv0_vec_push(tags2, 97);
  int _sv0t4 = ps_skip_optional_pub(tags2, 0);
  if ((_sv0t4 != 4)) {
    return 3;
  } else {
  }
  int _sv0t5 = sv0_vec_new();
  int tags3 = _sv0t5;
  sv0_vec_push(tags3, 93);
  sv0_vec_push(tags3, 65);
  sv0_vec_push(tags3, 97);
  int _sv0t6 = ps_skip_optional_unsafe(tags3, 0);
  if ((_sv0t6 != 1)) {
    return 4;
  } else {
  }
  int _sv0t7 = ps_skip_optional_unsafe(tags3, 1);
  if ((_sv0t7 != 1)) {
    return 5;
  } else {
  }
  int _sv0t8 = sv0_vec_new();
  int tags4 = _sv0t8;
  sv0_vec_push(tags4, 38);
  sv0_vec_push(tags4, 5);
  sv0_vec_push(tags4, 12);
  sv0_vec_push(tags4, 5);
  sv0_vec_push(tags4, 39);
  sv0_vec_push(tags4, 8);
  sv0_vec_push(tags4, 97);
  int _sv0t9 = ps_skip_generic_params(tags4, 0);
  if ((_sv0t9 != 5)) {
    return 6;
  } else {
  }
  int _sv0t10 = ps_skip_generic_params(tags4, 5);
  if ((_sv0t10 != 5)) {
    return 7;
  } else {
  }
  int _sv0t11 = sv0_vec_new();
  int tags5 = _sv0t11;
  sv0_vec_push(tags5, 95);
  sv0_vec_push(tags5, 5);
  sv0_vec_push(tags5, 14);
  sv0_vec_push(tags5, 5);
  sv0_vec_push(tags5, 8);
  sv0_vec_push(tags5, 97);
  int _sv0t12 = ps_skip_where_clause(tags5, 0);
  if ((_sv0t12 != 4)) {
    return 8;
  } else {
  }
  int _sv0t13 = ps_skip_where_clause(tags5, 4);
  if ((_sv0t13 != 4)) {
    return 9;
  } else {
  }
  int _sv0t14 = sv0_vec_new();
  int tags6 = _sv0t14;
  sv0_vec_push(tags6, 21);
  sv0_vec_push(tags6, 10);
  sv0_vec_push(tags6, 5);
  sv0_vec_push(tags6, 11);
  sv0_vec_push(tags6, 65);
  sv0_vec_push(tags6, 97);
  int _sv0t15 = ps_skip_attributes(tags6, 0);
  if ((_sv0t15 != 4)) {
    return 10;
  } else {
  }
  int _sv0t16 = ps_skip_attributes(tags6, 4);
  if ((_sv0t16 != 4)) {
    return 11;
  } else {
  }
  return 0;
}

static int test_parse_generic_params(void) {
  int _sv0t0 = sv0_vec_new();
  int tags = _sv0t0;
  sv0_vec_push(tags, 38);
  sv0_vec_push(tags, 5);
  sv0_vec_push(tags, 12);
  sv0_vec_push(tags, 5);
  sv0_vec_push(tags, 12);
  sv0_vec_push(tags, 5);
  sv0_vec_push(tags, 39);
  sv0_vec_push(tags, 8);
  sv0_vec_push(tags, 97);
  int _sv0t1 = sv0_vec_new();
  int names = _sv0t1;
  int _sv0t2 = parse_generic_params(tags, 0, names);
  int end_pos = _sv0t2;
  int _sv0t3 = sv0_vec_len(names);
  if ((_sv0t3 != 3)) {
    return 1;
  } else {
  }
  int _sv0t4 = sv0_vec_get(names, 0);
  if ((_sv0t4 != 1)) {
    return 2;
  } else {
  }
  int _sv0t5 = sv0_vec_get(names, 1);
  if ((_sv0t5 != 3)) {
    return 3;
  } else {
  }
  int _sv0t6 = sv0_vec_get(names, 2);
  if ((_sv0t6 != 5)) {
    return 4;
  } else {
  }
  if ((end_pos != 7)) {
    return 5;
  } else {
  }
  int _sv0t7 = sv0_vec_new();
  int names2 = _sv0t7;
  int _sv0t8 = parse_generic_params(tags, 7, names2);
  int no_gen = _sv0t8;
  int _sv0t9 = sv0_vec_len(names2);
  if ((_sv0t9 != 0)) {
    return 6;
  } else {
  }
  if ((no_gen != 7)) {
    return 7;
  } else {
  }
  int _sv0t10 = sv0_vec_new();
  int tags2 = _sv0t10;
  sv0_vec_push(tags2, 38);
  sv0_vec_push(tags2, 5);
  sv0_vec_push(tags2, 14);
  sv0_vec_push(tags2, 5);
  sv0_vec_push(tags2, 39);
  sv0_vec_push(tags2, 97);
  int _sv0t11 = sv0_vec_new();
  int bounded = _sv0t11;
  int _sv0t12 = parse_generic_params(tags2, 0, bounded);
  int end2 = _sv0t12;
  int _sv0t13 = sv0_vec_len(bounded);
  if ((_sv0t13 != 1)) {
    return 8;
  } else {
  }
  int _sv0t14 = sv0_vec_get(bounded, 0);
  if ((_sv0t14 != 1)) {
    return 9;
  } else {
  }
  if ((end2 != 5)) {
    return 10;
  } else {
  }
  return 0;
}

static int test_parse_use_item(void) {
  int _sv0t0 = sv0_vec_new();
  int tags = _sv0t0;
  sv0_vec_push(tags, 94);
  sv0_vec_push(tags, 5);
  sv0_vec_push(tags, 15);
  sv0_vec_push(tags, 5);
  sv0_vec_push(tags, 13);
  sv0_vec_push(tags, 97);
  int _sv0t1 = sv0_vec_new();
  int path = _sv0t1;
  int _sv0t2 = parse_use_item(tags, 0, path);
  int end_pos = _sv0t2;
  if ((end_pos != 5)) {
    return 1;
  } else {
  }
  int _sv0t3 = sv0_vec_len(path);
  if ((_sv0t3 != 2)) {
    return 2;
  } else {
  }
  int _sv0t4 = sv0_vec_get(path, 0);
  if ((_sv0t4 != 1)) {
    return 3;
  } else {
  }
  int _sv0t5 = sv0_vec_get(path, 1);
  if ((_sv0t5 != 3)) {
    return 4;
  } else {
  }
  int _sv0t6 = sv0_vec_new();
  int path2 = _sv0t6;
  int _sv0t7 = parse_use_item(tags, 1, path2);
  int fail = _sv0t7;
  int _sv0t8 = (0 - 1);
  if ((fail != _sv0t8)) {
    return 5;
  } else {
  }
  int _sv0t9 = sv0_vec_new();
  int tags2 = _sv0t9;
  sv0_vec_push(tags2, 94);
  sv0_vec_push(tags2, 5);
  sv0_vec_push(tags2, 97);
  int _sv0t10 = sv0_vec_new();
  int path3 = _sv0t10;
  int _sv0t11 = parse_use_item(tags2, 0, path3);
  int fail2 = _sv0t11;
  int _sv0t12 = (0 - 1);
  if ((fail2 != _sv0t12)) {
    return 6;
  } else {
  }
  return 0;
}

static int test_parse_module_decl(void) {
  int _sv0t0 = sv0_vec_new();
  int tags = _sv0t0;
  sv0_vec_push(tags, 75);
  sv0_vec_push(tags, 5);
  sv0_vec_push(tags, 13);
  sv0_vec_push(tags, 97);
  int _sv0t1 = sv0_vec_new();
  int path = _sv0t1;
  int _sv0t2 = parse_module_decl(tags, 0, path);
  int end_pos = _sv0t2;
  if ((end_pos != 3)) {
    return 1;
  } else {
  }
  int _sv0t3 = sv0_vec_len(path);
  if ((_sv0t3 != 1)) {
    return 2;
  } else {
  }
  int _sv0t4 = sv0_vec_get(path, 0);
  if ((_sv0t4 != 1)) {
    return 3;
  } else {
  }
  int _sv0t5 = sv0_vec_new();
  int path2 = _sv0t5;
  int _sv0t6 = parse_module_decl(tags, 3, path2);
  int same = _sv0t6;
  if ((same != 3)) {
    return 4;
  } else {
  }
  int _sv0t7 = sv0_vec_len(path2);
  if ((_sv0t7 != 0)) {
    return 5;
  } else {
  }
  int _sv0t8 = sv0_vec_new();
  int tags2 = _sv0t8;
  sv0_vec_push(tags2, 5);
  sv0_vec_push(tags2, 97);
  int _sv0t9 = sv0_vec_new();
  int path3 = _sv0t9;
  int _sv0t10 = parse_module_decl(tags2, 0, path3);
  int noop = _sv0t10;
  if ((noop != 0)) {
    return 6;
  } else {
  }
  return 0;
}

static int test_parse_expr_lit(void) {
  int _sv0t0 = sv0_vec_new();
  int tags = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int starts = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int ends = _sv0t2;
  sv0_vec_push(tags, 0);
  sv0_vec_push(starts, 0);
  sv0_vec_push(ends, 2);
  sv0_vec_push(tags, 97);
  sv0_vec_push(starts, 2);
  sv0_vec_push(ends, 2);
  int _sv0t3 = sv0_vec_new();
  int et = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int ed1 = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int ed2 = _sv0t5;
  int _sv0t6 = sv0_vec_new();
  int ed3 = _sv0t6;
  int _sv0t7 = sv0_vec_new();
  int ed4 = _sv0t7;
  int _sv0t8 = sv0_vec_new();
  int pp = _sv0t8;
  int _sv0t9 = parse_expr(tags, starts, ends, "42", 0, et, ed1, ed2, ed3, ed4, pp, 1);
  int p = _sv0t9;
  if ((p != 1)) {
    return 1;
  } else {
  }
  int _sv0t10 = sv0_vec_len(et);
  if ((_sv0t10 != 1)) {
    return 2;
  } else {
  }
  int _sv0t11 = sv0_vec_get(et, 0);
  if ((_sv0t11 != 0)) {
    return 3;
  } else {
  }
  int _sv0t12 = sv0_vec_get(ed1, 0);
  if ((_sv0t12 != 0)) {
    return 4;
  } else {
  }
  int _sv0t13 = sv0_vec_new();
  int tags2 = _sv0t13;
  int _sv0t14 = sv0_vec_new();
  int starts2 = _sv0t14;
  int _sv0t15 = sv0_vec_new();
  int ends2 = _sv0t15;
  sv0_vec_push(tags2, 91);
  sv0_vec_push(starts2, 0);
  sv0_vec_push(ends2, 4);
  sv0_vec_push(tags2, 97);
  sv0_vec_push(starts2, 4);
  sv0_vec_push(ends2, 4);
  int _sv0t16 = sv0_vec_new();
  int et2 = _sv0t16;
  int _sv0t17 = sv0_vec_new();
  int ed12 = _sv0t17;
  int _sv0t18 = sv0_vec_new();
  int ed22 = _sv0t18;
  int _sv0t19 = sv0_vec_new();
  int ed32 = _sv0t19;
  int _sv0t20 = sv0_vec_new();
  int ed42 = _sv0t20;
  int _sv0t21 = sv0_vec_new();
  int pp2 = _sv0t21;
  int _sv0t22 = parse_expr(tags2, starts2, ends2, "true", 0, et2, ed12, ed22, ed32, ed42, pp2, 1);
  int p2 = _sv0t22;
  if ((p2 != 1)) {
    return 5;
  } else {
  }
  int _sv0t23 = sv0_vec_get(et2, 0);
  if ((_sv0t23 != 0)) {
    return 6;
  } else {
  }
  int _sv0t24 = sv0_vec_get(ed12, 0);
  if ((_sv0t24 != 5)) {
    return 7;
  } else {
  }
  return 0;
}

static int test_parse_expr_path(void) {
  int _sv0t0 = sv0_vec_new();
  int tags = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int starts = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int ends = _sv0t2;
  sv0_vec_push(tags, 5);
  sv0_vec_push(starts, 0);
  sv0_vec_push(ends, 3);
  sv0_vec_push(tags, 97);
  sv0_vec_push(starts, 3);
  sv0_vec_push(ends, 3);
  int _sv0t3 = sv0_vec_new();
  int et = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int ed1 = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int ed2 = _sv0t5;
  int _sv0t6 = sv0_vec_new();
  int ed3 = _sv0t6;
  int _sv0t7 = sv0_vec_new();
  int ed4 = _sv0t7;
  int _sv0t8 = sv0_vec_new();
  int pp = _sv0t8;
  int _sv0t9 = parse_expr(tags, starts, ends, "foo", 0, et, ed1, ed2, ed3, ed4, pp, 1);
  int p = _sv0t9;
  if ((p != 1)) {
    return 1;
  } else {
  }
  int _sv0t10 = sv0_vec_len(et);
  if ((_sv0t10 != 1)) {
    return 2;
  } else {
  }
  int _sv0t11 = sv0_vec_get(et, 0);
  if ((_sv0t11 != 1)) {
    return 3;
  } else {
  }
  int _sv0t12 = sv0_vec_get(ed2, 0);
  if ((_sv0t12 != 1)) {
    return 4;
  } else {
  }
  int _sv0t13 = sv0_vec_len(pp);
  if ((_sv0t13 != 1)) {
    return 5;
  } else {
  }
  return 0;
}

static int test_parse_expr_binop(void) {
  int _sv0t0 = sv0_vec_new();
  int tags = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int starts = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int ends = _sv0t2;
  sv0_vec_push(tags, 0);
  sv0_vec_push(starts, 0);
  sv0_vec_push(ends, 1);
  sv0_vec_push(tags, 22);
  sv0_vec_push(starts, 2);
  sv0_vec_push(ends, 3);
  sv0_vec_push(tags, 0);
  sv0_vec_push(starts, 4);
  sv0_vec_push(ends, 5);
  sv0_vec_push(tags, 97);
  sv0_vec_push(starts, 5);
  sv0_vec_push(ends, 5);
  int _sv0t3 = sv0_vec_new();
  int et = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int ed1 = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int ed2 = _sv0t5;
  int _sv0t6 = sv0_vec_new();
  int ed3 = _sv0t6;
  int _sv0t7 = sv0_vec_new();
  int ed4 = _sv0t7;
  int _sv0t8 = sv0_vec_new();
  int pp = _sv0t8;
  int _sv0t9 = parse_expr(tags, starts, ends, "1 + 2", 0, et, ed1, ed2, ed3, ed4, pp, 1);
  int p = _sv0t9;
  if ((p != 3)) {
    return 1;
  } else {
  }
  int _sv0t10 = sv0_vec_len(et);
  if ((_sv0t10 != 3)) {
    return 2;
  } else {
  }
  int _sv0t11 = sv0_vec_get(et, 0);
  if ((_sv0t11 != 0)) {
    return 3;
  } else {
  }
  int _sv0t12 = sv0_vec_get(et, 1);
  if ((_sv0t12 != 0)) {
    return 4;
  } else {
  }
  int _sv0t13 = sv0_vec_get(et, 2);
  if ((_sv0t13 != 3)) {
    return 5;
  } else {
  }
  int _sv0t14 = sv0_vec_get(ed1, 2);
  if ((_sv0t14 != 0)) {
    return 6;
  } else {
  }
  int _sv0t15 = sv0_vec_get(ed2, 2);
  if ((_sv0t15 != 0)) {
    return 7;
  } else {
  }
  int _sv0t16 = sv0_vec_get(ed3, 2);
  if ((_sv0t16 != 1)) {
    return 8;
  } else {
  }
  return 0;
}

static int test_parse_expr_unary(void) {
  int _sv0t0 = sv0_vec_new();
  int tags = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int starts = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int ends = _sv0t2;
  sv0_vec_push(tags, 23);
  sv0_vec_push(starts, 0);
  sv0_vec_push(ends, 1);
  sv0_vec_push(tags, 0);
  sv0_vec_push(starts, 1);
  sv0_vec_push(ends, 2);
  sv0_vec_push(tags, 97);
  sv0_vec_push(starts, 2);
  sv0_vec_push(ends, 2);
  int _sv0t3 = sv0_vec_new();
  int et = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int ed1 = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int ed2 = _sv0t5;
  int _sv0t6 = sv0_vec_new();
  int ed3 = _sv0t6;
  int _sv0t7 = sv0_vec_new();
  int ed4 = _sv0t7;
  int _sv0t8 = sv0_vec_new();
  int pp = _sv0t8;
  int _sv0t9 = parse_expr(tags, starts, ends, "-x", 0, et, ed1, ed2, ed3, ed4, pp, 1);
  int p = _sv0t9;
  if ((p != 2)) {
    return 1;
  } else {
  }
  int _sv0t10 = sv0_vec_len(et);
  if ((_sv0t10 != 2)) {
    return 2;
  } else {
  }
  int _sv0t11 = sv0_vec_get(et, 0);
  if ((_sv0t11 != 0)) {
    return 3;
  } else {
  }
  int _sv0t12 = sv0_vec_get(et, 1);
  if ((_sv0t12 != 2)) {
    return 4;
  } else {
  }
  int _sv0t13 = sv0_vec_get(ed1, 1);
  if ((_sv0t13 != 0)) {
    return 5;
  } else {
  }
  int _sv0t14 = sv0_vec_get(ed2, 1);
  if ((_sv0t14 != 0)) {
    return 6;
  } else {
  }
  int _sv0t15 = sv0_vec_new();
  int tags2 = _sv0t15;
  int _sv0t16 = sv0_vec_new();
  int starts2 = _sv0t16;
  int _sv0t17 = sv0_vec_new();
  int ends2 = _sv0t17;
  sv0_vec_push(tags2, 33);
  sv0_vec_push(starts2, 0);
  sv0_vec_push(ends2, 1);
  sv0_vec_push(tags2, 5);
  sv0_vec_push(starts2, 1);
  sv0_vec_push(ends2, 2);
  sv0_vec_push(tags2, 97);
  sv0_vec_push(starts2, 2);
  sv0_vec_push(ends2, 2);
  int _sv0t18 = sv0_vec_new();
  int et2 = _sv0t18;
  int _sv0t19 = sv0_vec_new();
  int ed12 = _sv0t19;
  int _sv0t20 = sv0_vec_new();
  int ed22 = _sv0t20;
  int _sv0t21 = sv0_vec_new();
  int ed32 = _sv0t21;
  int _sv0t22 = sv0_vec_new();
  int ed42 = _sv0t22;
  int _sv0t23 = sv0_vec_new();
  int pp2 = _sv0t23;
  int _sv0t24 = parse_expr(tags2, starts2, ends2, "!x", 0, et2, ed12, ed22, ed32, ed42, pp2, 1);
  int p2 = _sv0t24;
  if ((p2 != 2)) {
    return 7;
  } else {
  }
  int _sv0t25 = sv0_vec_get(et2, 1);
  if ((_sv0t25 != 2)) {
    return 8;
  } else {
  }
  int _sv0t26 = sv0_vec_get(ed12, 1);
  if ((_sv0t26 != 1)) {
    return 9;
  } else {
  }
  return 0;
}

static int test_parse_expr_call(void) {
  int _sv0t0 = sv0_vec_new();
  int tags = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int starts = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int ends = _sv0t2;
  sv0_vec_push(tags, 5);
  sv0_vec_push(starts, 0);
  sv0_vec_push(ends, 3);
  sv0_vec_push(tags, 6);
  sv0_vec_push(starts, 3);
  sv0_vec_push(ends, 4);
  sv0_vec_push(tags, 0);
  sv0_vec_push(starts, 4);
  sv0_vec_push(ends, 5);
  sv0_vec_push(tags, 12);
  sv0_vec_push(starts, 5);
  sv0_vec_push(ends, 6);
  sv0_vec_push(tags, 0);
  sv0_vec_push(starts, 7);
  sv0_vec_push(ends, 8);
  sv0_vec_push(tags, 7);
  sv0_vec_push(starts, 8);
  sv0_vec_push(ends, 9);
  sv0_vec_push(tags, 97);
  sv0_vec_push(starts, 9);
  sv0_vec_push(ends, 9);
  int _sv0t3 = sv0_vec_new();
  int et = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int ed1 = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int ed2 = _sv0t5;
  int _sv0t6 = sv0_vec_new();
  int ed3 = _sv0t6;
  int _sv0t7 = sv0_vec_new();
  int ed4 = _sv0t7;
  int _sv0t8 = sv0_vec_new();
  int pp = _sv0t8;
  int _sv0t9 = parse_expr(tags, starts, ends, "foo(1, 2)", 0, et, ed1, ed2, ed3, ed4, pp, 1);
  int p = _sv0t9;
  if ((p != 6)) {
    return 1;
  } else {
  }
  int _sv0t10 = sv0_vec_len(et);
  if ((_sv0t10 != 4)) {
    return 2;
  } else {
  }
  int _sv0t11 = sv0_vec_get(et, 0);
  if ((_sv0t11 != 1)) {
    return 3;
  } else {
  }
  int _sv0t12 = sv0_vec_get(et, 1);
  if ((_sv0t12 != 0)) {
    return 4;
  } else {
  }
  int _sv0t13 = sv0_vec_get(et, 2);
  if ((_sv0t13 != 0)) {
    return 5;
  } else {
  }
  int _sv0t14 = sv0_vec_get(et, 3);
  if ((_sv0t14 != 4)) {
    return 6;
  } else {
  }
  int _sv0t15 = sv0_vec_get(ed1, 3);
  if ((_sv0t15 != 0)) {
    return 7;
  } else {
  }
  int _sv0t16 = sv0_vec_get(ed2, 3);
  if ((_sv0t16 != 1)) {
    return 8;
  } else {
  }
  int _sv0t17 = sv0_vec_get(ed3, 3);
  if ((_sv0t17 != 2)) {
    return 9;
  } else {
  }
  return 0;
}

static int test_parse_expr_field(void) {
  int _sv0t0 = sv0_vec_new();
  int tags = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int starts = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int ends = _sv0t2;
  sv0_vec_push(tags, 5);
  sv0_vec_push(starts, 0);
  sv0_vec_push(ends, 1);
  sv0_vec_push(tags, 16);
  sv0_vec_push(starts, 1);
  sv0_vec_push(ends, 2);
  sv0_vec_push(tags, 5);
  sv0_vec_push(starts, 2);
  sv0_vec_push(ends, 3);
  sv0_vec_push(tags, 97);
  sv0_vec_push(starts, 3);
  sv0_vec_push(ends, 3);
  int _sv0t3 = sv0_vec_new();
  int et = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int ed1 = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int ed2 = _sv0t5;
  int _sv0t6 = sv0_vec_new();
  int ed3 = _sv0t6;
  int _sv0t7 = sv0_vec_new();
  int ed4 = _sv0t7;
  int _sv0t8 = sv0_vec_new();
  int pp = _sv0t8;
  int _sv0t9 = parse_expr(tags, starts, ends, "x.y", 0, et, ed1, ed2, ed3, ed4, pp, 1);
  int p = _sv0t9;
  if ((p != 3)) {
    return 1;
  } else {
  }
  int _sv0t10 = sv0_vec_len(et);
  if ((_sv0t10 != 2)) {
    return 2;
  } else {
  }
  int _sv0t11 = sv0_vec_get(et, 0);
  if ((_sv0t11 != 1)) {
    return 3;
  } else {
  }
  int _sv0t12 = sv0_vec_get(et, 1);
  if ((_sv0t12 != 6)) {
    return 4;
  } else {
  }
  int _sv0t13 = sv0_vec_get(ed1, 1);
  if ((_sv0t13 != 0)) {
    return 5;
  } else {
  }
  int _sv0t14 = sv0_vec_get(ed2, 1);
  if ((_sv0t14 != 2)) {
    return 6;
  } else {
  }
  int _sv0t15 = sv0_vec_new();
  int tags2 = _sv0t15;
  int _sv0t16 = sv0_vec_new();
  int starts2 = _sv0t16;
  int _sv0t17 = sv0_vec_new();
  int ends2 = _sv0t17;
  sv0_vec_push(tags2, 5);
  sv0_vec_push(starts2, 0);
  sv0_vec_push(ends2, 1);
  sv0_vec_push(tags2, 16);
  sv0_vec_push(starts2, 1);
  sv0_vec_push(ends2, 2);
  sv0_vec_push(tags2, 0);
  sv0_vec_push(starts2, 2);
  sv0_vec_push(ends2, 3);
  sv0_vec_push(tags2, 97);
  sv0_vec_push(starts2, 3);
  sv0_vec_push(ends2, 3);
  int _sv0t18 = sv0_vec_new();
  int et2 = _sv0t18;
  int _sv0t19 = sv0_vec_new();
  int ed12 = _sv0t19;
  int _sv0t20 = sv0_vec_new();
  int ed22 = _sv0t20;
  int _sv0t21 = sv0_vec_new();
  int ed32 = _sv0t21;
  int _sv0t22 = sv0_vec_new();
  int ed42 = _sv0t22;
  int _sv0t23 = sv0_vec_new();
  int pp2 = _sv0t23;
  int _sv0t24 = parse_expr(tags2, starts2, ends2, "x.0", 0, et2, ed12, ed22, ed32, ed42, pp2, 1);
  int p2 = _sv0t24;
  if ((p2 != 3)) {
    return 7;
  } else {
  }
  int _sv0t25 = sv0_vec_get(et2, 1);
  if ((_sv0t25 != 7)) {
    return 8;
  } else {
  }
  int _sv0t26 = sv0_vec_get(ed12, 1);
  if ((_sv0t26 != 0)) {
    return 9;
  } else {
  }
  return 0;
}

static int test_parse_expr_tuple(void) {
  int _sv0t0 = sv0_vec_new();
  int tags = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int starts = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int ends = _sv0t2;
  sv0_vec_push(tags, 6);
  sv0_vec_push(starts, 0);
  sv0_vec_push(ends, 1);
  sv0_vec_push(tags, 0);
  sv0_vec_push(starts, 1);
  sv0_vec_push(ends, 2);
  sv0_vec_push(tags, 12);
  sv0_vec_push(starts, 2);
  sv0_vec_push(ends, 3);
  sv0_vec_push(tags, 0);
  sv0_vec_push(starts, 4);
  sv0_vec_push(ends, 5);
  sv0_vec_push(tags, 7);
  sv0_vec_push(starts, 5);
  sv0_vec_push(ends, 6);
  sv0_vec_push(tags, 97);
  sv0_vec_push(starts, 6);
  sv0_vec_push(ends, 6);
  int _sv0t3 = sv0_vec_new();
  int et = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int ed1 = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int ed2 = _sv0t5;
  int _sv0t6 = sv0_vec_new();
  int ed3 = _sv0t6;
  int _sv0t7 = sv0_vec_new();
  int ed4 = _sv0t7;
  int _sv0t8 = sv0_vec_new();
  int pp = _sv0t8;
  int _sv0t9 = parse_expr(tags, starts, ends, "(1, 2)", 0, et, ed1, ed2, ed3, ed4, pp, 1);
  int p = _sv0t9;
  if ((p != 5)) {
    return 1;
  } else {
  }
  int _sv0t10 = sv0_vec_len(et);
  if ((_sv0t10 != 3)) {
    return 2;
  } else {
  }
  int _sv0t11 = sv0_vec_get(et, 0);
  if ((_sv0t11 != 0)) {
    return 3;
  } else {
  }
  int _sv0t12 = sv0_vec_get(et, 1);
  if ((_sv0t12 != 0)) {
    return 4;
  } else {
  }
  int _sv0t13 = sv0_vec_get(et, 2);
  if ((_sv0t13 != 25)) {
    return 5;
  } else {
  }
  int _sv0t14 = sv0_vec_get(ed1, 2);
  if ((_sv0t14 != 0)) {
    return 6;
  } else {
  }
  int _sv0t15 = sv0_vec_get(ed2, 2);
  if ((_sv0t15 != 2)) {
    return 7;
  } else {
  }
  int _sv0t16 = sv0_vec_new();
  int tags3 = _sv0t16;
  int _sv0t17 = sv0_vec_new();
  int starts3 = _sv0t17;
  int _sv0t18 = sv0_vec_new();
  int ends3 = _sv0t18;
  sv0_vec_push(tags3, 6);
  sv0_vec_push(starts3, 0);
  sv0_vec_push(ends3, 1);
  sv0_vec_push(tags3, 7);
  sv0_vec_push(starts3, 1);
  sv0_vec_push(ends3, 2);
  sv0_vec_push(tags3, 97);
  sv0_vec_push(starts3, 2);
  sv0_vec_push(ends3, 2);
  int _sv0t19 = sv0_vec_new();
  int et3 = _sv0t19;
  int _sv0t20 = sv0_vec_new();
  int ed13 = _sv0t20;
  int _sv0t21 = sv0_vec_new();
  int ed23 = _sv0t21;
  int _sv0t22 = sv0_vec_new();
  int ed33 = _sv0t22;
  int _sv0t23 = sv0_vec_new();
  int ed43 = _sv0t23;
  int _sv0t24 = sv0_vec_new();
  int pp3 = _sv0t24;
  int _sv0t25 = parse_expr(tags3, starts3, ends3, "()", 0, et3, ed13, ed23, ed33, ed43, pp3, 1);
  int p3 = _sv0t25;
  if ((p3 != 2)) {
    return 8;
  } else {
  }
  int _sv0t26 = sv0_vec_get(et3, 0);
  if ((_sv0t26 != 0)) {
    return 9;
  } else {
  }
  int _sv0t27 = sv0_vec_get(ed13, 0);
  if ((_sv0t27 != 5)) {
    return 10;
  } else {
  }
  return 0;
}

static int test_parse_expr_array(void) {
  int _sv0t0 = sv0_vec_new();
  int tags = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int starts = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int ends = _sv0t2;
  sv0_vec_push(tags, 10);
  sv0_vec_push(starts, 0);
  sv0_vec_push(ends, 1);
  sv0_vec_push(tags, 0);
  sv0_vec_push(starts, 1);
  sv0_vec_push(ends, 2);
  sv0_vec_push(tags, 12);
  sv0_vec_push(starts, 2);
  sv0_vec_push(ends, 3);
  sv0_vec_push(tags, 0);
  sv0_vec_push(starts, 4);
  sv0_vec_push(ends, 5);
  sv0_vec_push(tags, 11);
  sv0_vec_push(starts, 5);
  sv0_vec_push(ends, 6);
  sv0_vec_push(tags, 97);
  sv0_vec_push(starts, 6);
  sv0_vec_push(ends, 6);
  int _sv0t3 = sv0_vec_new();
  int et = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int ed1 = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int ed2 = _sv0t5;
  int _sv0t6 = sv0_vec_new();
  int ed3 = _sv0t6;
  int _sv0t7 = sv0_vec_new();
  int ed4 = _sv0t7;
  int _sv0t8 = sv0_vec_new();
  int pp = _sv0t8;
  int _sv0t9 = parse_expr(tags, starts, ends, "[1, 2]", 0, et, ed1, ed2, ed3, ed4, pp, 1);
  int p = _sv0t9;
  if ((p != 5)) {
    return 1;
  } else {
  }
  int _sv0t10 = sv0_vec_len(et);
  if ((_sv0t10 != 3)) {
    return 2;
  } else {
  }
  int _sv0t11 = sv0_vec_get(et, 0);
  if ((_sv0t11 != 0)) {
    return 3;
  } else {
  }
  int _sv0t12 = sv0_vec_get(et, 1);
  if ((_sv0t12 != 0)) {
    return 4;
  } else {
  }
  int _sv0t13 = sv0_vec_get(et, 2);
  if ((_sv0t13 != 26)) {
    return 5;
  } else {
  }
  int _sv0t14 = sv0_vec_get(ed1, 2);
  if ((_sv0t14 != 0)) {
    return 6;
  } else {
  }
  int _sv0t15 = sv0_vec_get(ed2, 2);
  if ((_sv0t15 != 2)) {
    return 7;
  } else {
  }
  int _sv0t16 = sv0_vec_new();
  int tags2 = _sv0t16;
  int _sv0t17 = sv0_vec_new();
  int starts2 = _sv0t17;
  int _sv0t18 = sv0_vec_new();
  int ends2 = _sv0t18;
  sv0_vec_push(tags2, 10);
  sv0_vec_push(starts2, 0);
  sv0_vec_push(ends2, 1);
  sv0_vec_push(tags2, 11);
  sv0_vec_push(starts2, 1);
  sv0_vec_push(ends2, 2);
  sv0_vec_push(tags2, 97);
  sv0_vec_push(starts2, 2);
  sv0_vec_push(ends2, 2);
  int _sv0t19 = sv0_vec_new();
  int et2 = _sv0t19;
  int _sv0t20 = sv0_vec_new();
  int ed12 = _sv0t20;
  int _sv0t21 = sv0_vec_new();
  int ed22 = _sv0t21;
  int _sv0t22 = sv0_vec_new();
  int ed32 = _sv0t22;
  int _sv0t23 = sv0_vec_new();
  int ed42 = _sv0t23;
  int _sv0t24 = sv0_vec_new();
  int pp2 = _sv0t24;
  int _sv0t25 = parse_expr(tags2, starts2, ends2, "[]", 0, et2, ed12, ed22, ed32, ed42, pp2, 1);
  int p2 = _sv0t25;
  if ((p2 != 2)) {
    return 8;
  } else {
  }
  int _sv0t26 = sv0_vec_get(et2, 0);
  if ((_sv0t26 != 26)) {
    return 9;
  } else {
  }
  int _sv0t27 = sv0_vec_get(ed12, 0);
  if ((_sv0t27 != 0)) {
    return 10;
  } else {
  }
  int _sv0t28 = sv0_vec_get(ed22, 0);
  if ((_sv0t28 != 0)) {
    return 11;
  } else {
  }
  return 0;
}

static int test_parse_expr_cast(void) {
  int _sv0t0 = sv0_vec_new();
  int tags = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int starts = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int ends = _sv0t2;
  sv0_vec_push(tags, 0);
  sv0_vec_push(starts, 0);
  sv0_vec_push(ends, 2);
  sv0_vec_push(tags, 54);
  sv0_vec_push(starts, 3);
  sv0_vec_push(ends, 5);
  sv0_vec_push(tags, 5);
  sv0_vec_push(starts, 6);
  sv0_vec_push(ends, 9);
  sv0_vec_push(tags, 97);
  sv0_vec_push(starts, 9);
  sv0_vec_push(ends, 9);
  int _sv0t3 = sv0_vec_new();
  int et = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int ed1 = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int ed2 = _sv0t5;
  int _sv0t6 = sv0_vec_new();
  int ed3 = _sv0t6;
  int _sv0t7 = sv0_vec_new();
  int ed4 = _sv0t7;
  int _sv0t8 = sv0_vec_new();
  int pp = _sv0t8;
  int _sv0t9 = parse_expr(tags, starts, ends, "42 as i32", 0, et, ed1, ed2, ed3, ed4, pp, 1);
  int p = _sv0t9;
  if ((p != 3)) {
    return 1;
  } else {
  }
  int _sv0t10 = sv0_vec_len(et);
  if ((_sv0t10 != 2)) {
    return 2;
  } else {
  }
  int _sv0t11 = sv0_vec_get(et, 0);
  if ((_sv0t11 != 0)) {
    return 3;
  } else {
  }
  int _sv0t12 = sv0_vec_get(et, 1);
  if ((_sv0t12 != 20)) {
    return 4;
  } else {
  }
  int _sv0t13 = sv0_vec_get(ed1, 1);
  if ((_sv0t13 != 0)) {
    return 5;
  } else {
  }
  return 0;
}

static int test_parse_expr_precedence(void) {
  int _sv0t0 = sv0_vec_new();
  int tags = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int starts = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int ends = _sv0t2;
  sv0_vec_push(tags, 0);
  sv0_vec_push(starts, 0);
  sv0_vec_push(ends, 1);
  sv0_vec_push(tags, 22);
  sv0_vec_push(starts, 2);
  sv0_vec_push(ends, 3);
  sv0_vec_push(tags, 0);
  sv0_vec_push(starts, 4);
  sv0_vec_push(ends, 5);
  sv0_vec_push(tags, 24);
  sv0_vec_push(starts, 6);
  sv0_vec_push(ends, 7);
  sv0_vec_push(tags, 0);
  sv0_vec_push(starts, 8);
  sv0_vec_push(ends, 9);
  sv0_vec_push(tags, 97);
  sv0_vec_push(starts, 9);
  sv0_vec_push(ends, 9);
  int _sv0t3 = sv0_vec_new();
  int et = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int ed1 = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int ed2 = _sv0t5;
  int _sv0t6 = sv0_vec_new();
  int ed3 = _sv0t6;
  int _sv0t7 = sv0_vec_new();
  int ed4 = _sv0t7;
  int _sv0t8 = sv0_vec_new();
  int pp = _sv0t8;
  int _sv0t9 = parse_expr(tags, starts, ends, "1 + 2 * 3", 0, et, ed1, ed2, ed3, ed4, pp, 1);
  int p = _sv0t9;
  if ((p != 5)) {
    return 1;
  } else {
  }
  int _sv0t10 = sv0_vec_len(et);
  if ((_sv0t10 != 5)) {
    return 2;
  } else {
  }
  int _sv0t11 = sv0_vec_get(et, 0);
  if ((_sv0t11 != 0)) {
    return 3;
  } else {
  }
  int _sv0t12 = sv0_vec_get(et, 1);
  if ((_sv0t12 != 0)) {
    return 4;
  } else {
  }
  int _sv0t13 = sv0_vec_get(et, 2);
  if ((_sv0t13 != 0)) {
    return 5;
  } else {
  }
  int _sv0t14 = sv0_vec_get(et, 3);
  if ((_sv0t14 != 3)) {
    return 6;
  } else {
  }
  int _sv0t15 = sv0_vec_get(ed1, 3);
  if ((_sv0t15 != 2)) {
    return 7;
  } else {
  }
  int _sv0t16 = sv0_vec_get(ed2, 3);
  if ((_sv0t16 != 1)) {
    return 8;
  } else {
  }
  int _sv0t17 = sv0_vec_get(ed3, 3);
  if ((_sv0t17 != 2)) {
    return 9;
  } else {
  }
  int _sv0t18 = sv0_vec_get(et, 4);
  if ((_sv0t18 != 3)) {
    return 10;
  } else {
  }
  int _sv0t19 = sv0_vec_get(ed1, 4);
  if ((_sv0t19 != 0)) {
    return 11;
  } else {
  }
  int _sv0t20 = sv0_vec_get(ed2, 4);
  if ((_sv0t20 != 0)) {
    return 12;
  } else {
  }
  int _sv0t21 = sv0_vec_get(ed3, 4);
  if ((_sv0t21 != 3)) {
    return 13;
  } else {
  }
  return 0;
}

static int test_parse_expr_index(void) {
  int _sv0t0 = sv0_vec_new();
  int tags = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int starts = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int ends = _sv0t2;
  sv0_vec_push(tags, 5);
  sv0_vec_push(starts, 0);
  sv0_vec_push(ends, 1);
  sv0_vec_push(tags, 10);
  sv0_vec_push(starts, 1);
  sv0_vec_push(ends, 2);
  sv0_vec_push(tags, 0);
  sv0_vec_push(starts, 2);
  sv0_vec_push(ends, 3);
  sv0_vec_push(tags, 11);
  sv0_vec_push(starts, 3);
  sv0_vec_push(ends, 4);
  sv0_vec_push(tags, 97);
  sv0_vec_push(starts, 4);
  sv0_vec_push(ends, 4);
  int _sv0t3 = sv0_vec_new();
  int et = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int ed1 = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int ed2 = _sv0t5;
  int _sv0t6 = sv0_vec_new();
  int ed3 = _sv0t6;
  int _sv0t7 = sv0_vec_new();
  int ed4 = _sv0t7;
  int _sv0t8 = sv0_vec_new();
  int pp = _sv0t8;
  int _sv0t9 = parse_expr(tags, starts, ends, "a[0]", 0, et, ed1, ed2, ed3, ed4, pp, 1);
  int p = _sv0t9;
  if ((p != 4)) {
    return 1;
  } else {
  }
  int _sv0t10 = sv0_vec_len(et);
  if ((_sv0t10 != 3)) {
    return 2;
  } else {
  }
  int _sv0t11 = sv0_vec_get(et, 0);
  if ((_sv0t11 != 1)) {
    return 3;
  } else {
  }
  int _sv0t12 = sv0_vec_get(et, 1);
  if ((_sv0t12 != 0)) {
    return 4;
  } else {
  }
  int _sv0t13 = sv0_vec_get(et, 2);
  if ((_sv0t13 != 8)) {
    return 5;
  } else {
  }
  int _sv0t14 = sv0_vec_get(ed1, 2);
  if ((_sv0t14 != 0)) {
    return 6;
  } else {
  }
  int _sv0t15 = sv0_vec_get(ed2, 2);
  if ((_sv0t15 != 1)) {
    return 7;
  } else {
  }
  return 0;
}

int main(void) {
  int _sv0t0 = test_binop_of();
  int r1 = _sv0t0;
  if ((r1 != 0)) {
    return r1;
  } else {
  }
  int _sv0t1 = test_assign_binop();
  int r2 = _sv0t1;
  if ((r2 != 0)) {
    int _sv0t2 = (30 + r2);
    return _sv0t2;
  } else {
  }
  int _sv0t3 = test_lit_from_tok();
  int r3 = _sv0t3;
  if ((r3 != 0)) {
    int _sv0t4 = (50 + r3);
    return _sv0t4;
  } else {
  }
  int _sv0t5 = test_expr_span();
  int r4 = _sv0t5;
  if ((r4 != 0)) {
    int _sv0t6 = (60 + r4);
    return _sv0t6;
  } else {
  }
  int _sv0t7 = test_skip_helpers();
  int r5 = _sv0t7;
  if ((r5 != 0)) {
    int _sv0t8 = (70 + r5);
    return _sv0t8;
  } else {
  }
  int _sv0t9 = test_span_merge();
  int r6 = _sv0t9;
  if ((r6 != 0)) {
    int _sv0t10 = (90 + r6);
    return _sv0t10;
  } else {
  }
  int _sv0t11 = test_precedence();
  int r7 = _sv0t11;
  if ((r7 != 0)) {
    int _sv0t12 = (100 + r7);
    return _sv0t12;
  } else {
  }
  int _sv0t13 = test_ty_name();
  int r8 = _sv0t13;
  if ((r8 != 0)) {
    int _sv0t14 = (120 + r8);
    return _sv0t14;
  } else {
  }
  int _sv0t15 = test_attr_bracket();
  int r9 = _sv0t15;
  if ((r9 != 0)) {
    int _sv0t16 = (130 + r9);
    return _sv0t16;
  } else {
  }
  int _sv0t17 = test_pratt_prec();
  int r10 = _sv0t17;
  if ((r10 != 0)) {
    int _sv0t18 = (140 + r10);
    return _sv0t18;
  } else {
  }
  int _sv0t19 = test_ps_nav();
  int r11 = _sv0t19;
  if ((r11 != 0)) {
    int _sv0t20 = (160 + r11);
    return _sv0t20;
  } else {
  }
  int _sv0t21 = test_parse_path();
  int r12 = _sv0t21;
  if ((r12 != 0)) {
    int _sv0t22 = (180 + r12);
    return _sv0t22;
  } else {
  }
  int _sv0t23 = test_ps_skip();
  int r13 = _sv0t23;
  if ((r13 != 0)) {
    int _sv0t24 = (200 + r13);
    return _sv0t24;
  } else {
  }
  int _sv0t25 = test_parse_generic_params();
  int r14 = _sv0t25;
  if ((r14 != 0)) {
    int _sv0t26 = (220 + r14);
    return _sv0t26;
  } else {
  }
  int _sv0t27 = test_parse_use_item();
  int r15 = _sv0t27;
  if ((r15 != 0)) {
    int _sv0t28 = (240 + r15);
    return _sv0t28;
  } else {
  }
  int _sv0t29 = test_parse_module_decl();
  int r16 = _sv0t29;
  if ((r16 != 0)) {
    int _sv0t30 = (260 + r16);
    return _sv0t30;
  } else {
  }
  int _sv0t31 = test_is_assign_tok();
  int r17 = _sv0t31;
  if ((r17 != 0)) {
    int _sv0t32 = (280 + r17);
    return _sv0t32;
  } else {
  }
  int _sv0t33 = test_path_of_ty();
  int r18 = _sv0t33;
  if ((r18 != 0)) {
    int _sv0t34 = (290 + r18);
    return _sv0t34;
  } else {
  }
  int _sv0t35 = test_is_builtin_type_name();
  int r19 = _sv0t35;
  if ((r19 != 0)) {
    int _sv0t36 = (300 + r19);
    return _sv0t36;
  } else {
  }
  int _sv0t37 = test_parse_type_unit();
  int r20 = _sv0t37;
  if ((r20 != 0)) {
    int _sv0t38 = (310 + r20);
    return _sv0t38;
  } else {
  }
  int _sv0t39 = test_parse_type_builtin();
  int r21 = _sv0t39;
  if ((r21 != 0)) {
    int _sv0t40 = (320 + r21);
    return _sv0t40;
  } else {
  }
  int _sv0t41 = test_parse_type_ref();
  int r22 = _sv0t41;
  if ((r22 != 0)) {
    int _sv0t42 = (330 + r22);
    return _sv0t42;
  } else {
  }
  int _sv0t43 = test_parse_type_ref_mut();
  int r23 = _sv0t43;
  if ((r23 != 0)) {
    int _sv0t44 = (340 + r23);
    return _sv0t44;
  } else {
  }
  int _sv0t45 = test_parse_type_slice();
  int r24 = _sv0t45;
  if ((r24 != 0)) {
    int _sv0t46 = (350 + r24);
    return _sv0t46;
  } else {
  }
  int _sv0t47 = test_parse_type_array();
  int r25 = _sv0t47;
  if ((r25 != 0)) {
    int _sv0t48 = (360 + r25);
    return _sv0t48;
  } else {
  }
  int _sv0t49 = test_parse_type_generic();
  int r26 = _sv0t49;
  if ((r26 != 0)) {
    int _sv0t50 = (370 + r26);
    return _sv0t50;
  } else {
  }
  int _sv0t51 = test_parse_type_paren();
  int r27 = _sv0t51;
  if ((r27 != 0)) {
    int _sv0t52 = (380 + r27);
    return _sv0t52;
  } else {
  }
  int _sv0t53 = test_parse_pat_wild();
  int r28 = _sv0t53;
  if ((r28 != 0)) {
    int _sv0t54 = (390 + r28);
    return _sv0t54;
  } else {
  }
  int _sv0t55 = test_parse_pat_bind();
  int r29 = _sv0t55;
  if ((r29 != 0)) {
    int _sv0t56 = (400 + r29);
    return _sv0t56;
  } else {
  }
  int _sv0t57 = test_parse_pat_bind_mut();
  int r30 = _sv0t57;
  if ((r30 != 0)) {
    int _sv0t58 = (410 + r30);
    return _sv0t58;
  } else {
  }
  int _sv0t59 = test_parse_pat_lit();
  int r31 = _sv0t59;
  if ((r31 != 0)) {
    int _sv0t60 = (420 + r31);
    return _sv0t60;
  } else {
  }
  int _sv0t61 = test_parse_pat_neg_lit();
  int r32 = _sv0t61;
  if ((r32 != 0)) {
    int _sv0t62 = (430 + r32);
    return _sv0t62;
  } else {
  }
  int _sv0t63 = test_parse_pat_unit();
  int r33 = _sv0t63;
  if ((r33 != 0)) {
    int _sv0t64 = (440 + r33);
    return _sv0t64;
  } else {
  }
  int _sv0t65 = test_parse_pat_tuple();
  int r34 = _sv0t65;
  if ((r34 != 0)) {
    int _sv0t66 = (450 + r34);
    return _sv0t66;
  } else {
  }
  int _sv0t67 = test_parse_pat_enum();
  int r35 = _sv0t67;
  if ((r35 != 0)) {
    int _sv0t68 = (460 + r35);
    return _sv0t68;
  } else {
  }
  int _sv0t69 = test_parse_pat_or();
  int r36 = _sv0t69;
  if ((r36 != 0)) {
    int _sv0t70 = (470 + r36);
    return _sv0t70;
  } else {
  }
  int _sv0t71 = test_parse_pat_paren();
  int r37 = _sv0t71;
  if ((r37 != 0)) {
    int _sv0t72 = (480 + r37);
    return _sv0t72;
  } else {
  }
  int _sv0t73 = test_parse_pat_bare_enum();
  int r38 = _sv0t73;
  if ((r38 != 0)) {
    int _sv0t74 = (490 + r38);
    return _sv0t74;
  } else {
  }
  int _sv0t75 = test_parse_expr_lit();
  int r39 = _sv0t75;
  if ((r39 != 0)) {
    int _sv0t76 = (500 + r39);
    return _sv0t76;
  } else {
  }
  int _sv0t77 = test_parse_expr_path();
  int r40 = _sv0t77;
  if ((r40 != 0)) {
    int _sv0t78 = (510 + r40);
    return _sv0t78;
  } else {
  }
  int _sv0t79 = test_parse_expr_binop();
  int r41 = _sv0t79;
  if ((r41 != 0)) {
    int _sv0t80 = (520 + r41);
    return _sv0t80;
  } else {
  }
  int _sv0t81 = test_parse_expr_unary();
  int r42 = _sv0t81;
  if ((r42 != 0)) {
    int _sv0t82 = (530 + r42);
    return _sv0t82;
  } else {
  }
  int _sv0t83 = test_parse_expr_call();
  int r43 = _sv0t83;
  if ((r43 != 0)) {
    int _sv0t84 = (540 + r43);
    return _sv0t84;
  } else {
  }
  int _sv0t85 = test_parse_expr_field();
  int r44 = _sv0t85;
  if ((r44 != 0)) {
    int _sv0t86 = (550 + r44);
    return _sv0t86;
  } else {
  }
  int _sv0t87 = test_parse_expr_tuple();
  int r45 = _sv0t87;
  if ((r45 != 0)) {
    int _sv0t88 = (560 + r45);
    return _sv0t88;
  } else {
  }
  int _sv0t89 = test_parse_expr_array();
  int r46 = _sv0t89;
  if ((r46 != 0)) {
    int _sv0t90 = (570 + r46);
    return _sv0t90;
  } else {
  }
  int _sv0t91 = test_parse_expr_cast();
  int r47 = _sv0t91;
  if ((r47 != 0)) {
    int _sv0t92 = (580 + r47);
    return _sv0t92;
  } else {
  }
  int _sv0t93 = test_parse_expr_precedence();
  int r48 = _sv0t93;
  if ((r48 != 0)) {
    int _sv0t94 = (590 + r48);
    return _sv0t94;
  } else {
  }
  int _sv0t95 = test_parse_expr_index();
  int r49 = _sv0t95;
  if ((r49 != 0)) {
    int _sv0t96 = (600 + r49);
    return _sv0t96;
  } else {
  }
  return 0;
}


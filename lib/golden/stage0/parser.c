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
static int test_binop_of(void);
static int test_assign_binop(void);
static int test_lit_from_tok(void);
static int test_expr_span(void);
static int test_skip_helpers(void);
static int test_span_merge(void);
static int test_precedence(void);
static int test_ty_name(void);
static int test_pratt_prec(void);
static int test_attr_bracket(void);

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
  return 79;
}

static int TK_PROJECT(void) {
  return 79;
}

static int TK_UNSAFE(void) {
  return 90;
}

static int TK_FN(void) {
  return 65;
}

static int TK_WHERE(void) {
  return 91;
}

static int TK_TRUE(void) {
  return 89;
}

static int TK_FALSE(void) {
  return 64;
}

static int TK_EOF(void) {
  return 96;
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
  if ((tok_tag == 89)) {
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
  int _sv0t0 = (tok_tag == 79);
  return _sv0t0;
}

static int is_unsafe_token(int tok_tag) {
  int _sv0t0 = (tok_tag == 90);
  return _sv0t0;
}

static int is_where_token(int tok_tag) {
  int _sv0t0 = (tok_tag == 91);
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
  if ((tok_tag == 96)) {
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
  int _sv0t4 = lit_from_tok(89);
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
  int _sv0t0 = is_pub_token(79);
  if ((_sv0t0 != 1)) {
    return 1;
  } else {
  }
  int _sv0t1 = is_pub_token(0);
  if ((_sv0t1 != 0)) {
    return 2;
  } else {
  }
  int _sv0t2 = is_unsafe_token(90);
  if ((_sv0t2 != 1)) {
    return 3;
  } else {
  }
  int _sv0t3 = is_where_token(91);
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
  int _sv0t15 = where_clause_stop(96);
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
  return 0;
}


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
static int TK_ASSERT(void);
static int TK_EXISTS(void);
static int TK_FORALL(void);
static int TK_LOOP_INVARIANT(void);
static int TK_OLD(void);
static int TK_NO_ALIAS(void);
static int TK_RESULT(void);
static int TK_SELF(void);
static int TK_REQUIRES(void);
static int TK_ENSURES(void);
static int TK_MODULE(void);
static int TK_NEWTYPE(void);
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
static int parse_arg_list(int tags, int starts, int ends, const char* source, int pos, int et, int ed1, int ed2, int ed3, int ed4, int pp, int sf_names, int arg_roots);
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
static int parse_expr_sf(int tags, int starts, int ends, const char* source, int pos, int et, int ed1, int ed2, int ed3, int ed4, int pp, int sf_names, int allow_struct);
static int expr_ends_with_braced_block(int et, int idx);
static int parse_return_expr(int tags, int starts, int ends, const char* source, int pos, int et, int ed1, int ed2, int ed3, int ed4, int pp, int sf_names);
static int parse_let_stmt(int tags, int starts, int ends, const char* source, int pos, int et, int ed1, int ed2, int ed3, int ed4, int pp, int sf_names);
static int parse_assign_target_op_pos(int tags, int pos, int out);
static int try_assign_stmt(int tags, int starts, int ends, const char* source, int pos, int et, int ed1, int ed2, int ed3, int ed4, int pp, int sf_names);
static int try_stmt(int tags, int starts, int ends, const char* source, int pos, int et, int ed1, int ed2, int ed3, int ed4, int pp, int sf_names);
static int block_stmt_sidecar_push(int pp, int stmt_ids);
static int parse_block(int tags, int starts, int ends, const char* source, int pos, int et, int ed1, int ed2, int ed3, int ed4, int pp, int sf_names);
static int parse_if_expr(int tags, int starts, int ends, const char* source, int pos, int et, int ed1, int ed2, int ed3, int ed4, int pp, int sf_names);
static int parse_while_expr(int tags, int starts, int ends, const char* source, int pos, int et, int ed1, int ed2, int ed3, int ed4, int pp, int sf_names);
static int parse_for_expr(int tags, int starts, int ends, const char* source, int pos, int et, int ed1, int ed2, int ed3, int ed4, int pp, int sf_names);
static int parse_loop_expr(int tags, int starts, int ends, const char* source, int pos, int et, int ed1, int ed2, int ed3, int ed4, int pp, int sf_names);
static int pat_leftmost_subtree_rows(int pat_tags, int pat_d1, int pat_d2, int pat_d3, int s);
static int parse_match_expr(int tags, int starts, int ends, const char* source, int pos, int et, int ed1, int ed2, int ed3, int ed4, int pp, int sf_names);
static int parse_quant(int tags, int starts, int ends, const char* source, int pos, int is_forall, int et, int ed1, int ed2, int ed3, int ed4, int pp);
static int parse_param(int tags, int starts, int ends, const char* source, int pos, int pp, int fn_param_name_toks, int pty_tt, int pty_td1, int pty_td2, int pty_td3, int fn_param_ty_root);
static int parse_param_list(int tags, int starts, int ends, const char* source, int pos, int pp, int count_out, int fn_param_name_toks, int pty_tt, int pty_td1, int pty_td2, int pty_td3, int fn_param_ty_root);
static int parse_contract(int tags, int starts, int ends, const char* source, int pos, int pp, int et, int ed1, int ed2, int ed3, int ed4, int contract_root);
static int parse_contracts(int tags, int starts, int ends, const char* source, int pos, int pp, int et, int ed1, int ed2, int ed3, int ed4, int contract_root, int count_out);
static int parse_fn_body(int tags, int starts, int ends, const char* source, int pos, int it, int id1, int id2, int id3, int id4, int id5, int pp, int fn_param_name_toks, int fn_param_ty_root, int fn_ret_ty_root_by_item, int fn_contract_base_by_item, int fn_contract_root, int enum_variant_payload_base_by_item, int enum_variant_payload_count_by_item, int pty_tt, int pty_td1, int pty_td2, int pty_td3, int body_et, int body_ed1, int body_ed2, int body_ed3, int body_ed4, int body_sf);
static int parse_struct_item(int tags, int starts, int ends, const char* source, int pos, int it, int id1, int id2, int id3, int id4, int id5, int pp, int struct_field_name_toks, int fn_ret_ty_root_by_item, int fn_contract_base_by_item, int enum_variant_payload_base_by_item, int enum_variant_payload_count_by_item, int pty_tt, int pty_td1, int pty_td2, int pty_td3, int struct_field_ty_root);
static int parse_enum_item(int tags, int starts, int ends, const char* source, int pos, int it, int id1, int id2, int id3, int id4, int id5, int pp, int enum_variant_name_toks, int fn_ret_ty_root_by_item, int fn_contract_base_by_item, int enum_variant_payload_ty_root, int enum_variant_payload_base_by_item, int enum_variant_payload_count_by_item, int pty_tt, int pty_td1, int pty_td2, int pty_td3);
static int parse_type_alias_item(int tags, int starts, int ends, const char* source, int pos, int it, int id1, int id2, int id3, int id4, int id5, int pp, int fn_ret_ty_root_by_item, int fn_contract_base_by_item, int enum_variant_payload_base_by_item, int enum_variant_payload_count_by_item, int pty_tt, int pty_td1, int pty_td2, int pty_td3);
static int parse_trait_item(int tags, int starts, int ends, const char* source, int pos, int it, int id1, int id2, int id3, int id4, int id5, int pp, int fn_param_name_toks, int fn_param_ty_root, int fn_ret_ty_root_by_item, int fn_contract_base_by_item, int fn_contract_root, int enum_variant_payload_base_by_item, int enum_variant_payload_count_by_item, int pty_tt, int pty_td1, int pty_td2, int pty_td3, int body_et, int body_ed1, int body_ed2, int body_ed3, int body_ed4, int body_sf);
static int parse_impl_item(int tags, int starts, int ends, const char* source, int pos, int it, int id1, int id2, int id3, int id4, int id5, int pp, int fn_param_name_toks, int fn_param_ty_root, int fn_ret_ty_root_by_item, int fn_contract_base_by_item, int fn_contract_root, int enum_variant_payload_base_by_item, int enum_variant_payload_count_by_item, int pty_tt, int pty_td1, int pty_td2, int pty_td3, int body_et, int body_ed1, int body_ed2, int body_ed3, int body_ed4, int body_sf);
static int parse_item(int tags, int starts, int ends, const char* source, int pos, int it, int id1, int id2, int id3, int id4, int id5, int pp, int enum_variant_name_toks, int struct_field_name_toks, int fn_param_name_toks, int fn_param_ty_root, int fn_ret_ty_root_by_item, int fn_contract_base_by_item, int fn_contract_root, int enum_variant_payload_ty_root, int enum_variant_payload_base_by_item, int enum_variant_payload_count_by_item, int pty_tt, int pty_td1, int pty_td2, int pty_td3, int struct_field_ty_root, int body_et, int body_ed1, int body_ed2, int body_ed3, int body_ed4, int body_sf);
static int parse_program(int tags, int starts, int ends, const char* source, int pos, int it, int id1, int id2, int id3, int id4, int id5, int pp, int enum_variant_name_toks, int struct_field_name_toks, int fn_param_name_toks, int fn_param_ty_root, int fn_ret_ty_root_by_item, int fn_contract_base_by_item, int fn_contract_root, int enum_variant_payload_ty_root, int enum_variant_payload_base_by_item, int enum_variant_payload_count_by_item, int pty_tt, int pty_td1, int pty_td2, int pty_td3, int struct_field_ty_root, int body_et, int body_ed1, int body_ed2, int body_ed3, int body_ed4, int body_sf);
static int item_fn_has_ret_packed(int item_id2);
static int item_fn_contract_count_packed(int item_id2);
static int test_item_fn_id2_pack_roundtrip(void);
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
static int test_parse_block_empty(void);
static int test_parse_block_tail(void);
static int test_parse_block_semi(void);
static int test_parse_block_let(void);
static int test_parse_if_simple(void);
static int test_parse_if_else(void);
static int test_parse_while(void);
static int test_parse_loop(void);
static int test_parse_return(void);
static int test_parse_assign(void);
static int test_parse_match_simple(void);
static int test_parse_match_multi(void);
static int test_parse_match_guard(void);
static int test_parse_match_enum_tuple_arm(void);
static int test_parse_match_pat_or(void);
static int test_parse_quant(void);
static int test_parse_old(void);
static int test_parse_no_alias(void);
static int test_parse_while_inv(void);
static int test_parse_fn_simple(void);
static int test_parse_fn_ret(void);
static int test_parse_struct(void);
static int test_parse_enum_mixed(void);
static int test_parse_impl(void);
static int test_parse_fn_contract(void);
static int test_parse_program(void);

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

static int TK_ASSERT(void) {
  return 55;
}

static int TK_EXISTS(void) {
  return 63;
}

static int TK_FORALL(void) {
  return 67;
}

static int TK_LOOP_INVARIANT(void) {
  return 73;
}

static int TK_OLD(void) {
  return 80;
}

static int TK_NO_ALIAS(void) {
  return 79;
}

static int TK_RESULT(void) {
  return 84;
}

static int TK_SELF(void) {
  return 86;
}

static int TK_REQUIRES(void) {
  return 83;
}

static int TK_ENSURES(void) {
  return 61;
}

static int TK_MODULE(void) {
  return 75;
}

static int TK_NEWTYPE(void) {
  return 78;
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
        int _sv0t10 = sv0_vec_len(pat_tags);
        int last_pat = (_sv0t10 - 1);
        sv0_vec_set(pat_data3, last_pat, fname_pos);
        p = pv;
      } else {
        sv0_vec_push(pat_tags, 1);
        sv0_vec_push(pat_data1, fname_pos);
        sv0_vec_push(pat_data2, 0);
        sv0_vec_push(pat_data3, 0);
      }
      field_count = (field_count + 1);
      int _sv0t11 = ps_peek(tags, p);
      if ((_sv0t11 == 12)) {
        p = (p + 1);
      } else {
      }
    }
    int _sv0t12 = (0 - 1);
    return _sv0t12;
  } else {
  }
  if ((t == 6)) {
    int p = (pos + 1);
    int _sv0t13 = ps_peek(tags, p);
    if ((_sv0t13 == 7)) {
      sv0_vec_push(pat_tags, 5);
      sv0_vec_push(pat_data1, pp_start);
      sv0_vec_push(pat_data2, pp_count);
      sv0_vec_push(pat_data3, 0);
      int _sv0t14 = (p + 1);
      return _sv0t14;
    } else {
    }
    int arg_count = 0;
    int done = 0;
    while ((done != 1)) {
      int _sv0t15 = parse_pat(tags, starts, ends, source, p, pat_tags, pat_data1, pat_data2, pat_data3, path_pool);
      int pa = _sv0t15;
      if ((pa < 0)) {
        int _sv0t16 = (0 - 1);
        return _sv0t16;
      } else {
      }
      arg_count = (arg_count + 1);
      p = pa;
      int _sv0t17 = ps_peek(tags, p);
      if ((_sv0t17 == 12)) {
        p = (p + 1);
      } else {
        int _sv0t18 = ps_peek(tags, p);
        if ((_sv0t18 == 7)) {
          done = 1;
        } else {
          int _sv0t19 = (0 - 1);
          return _sv0t19;
        }
      }
    }
    sv0_vec_push(pat_tags, 5);
    sv0_vec_push(pat_data1, pp_start);
    sv0_vec_push(pat_data2, pp_count);
    sv0_vec_push(pat_data3, arg_count);
    int _sv0t20 = (p + 1);
    return _sv0t20;
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

static int parse_arg_list(int tags, int starts, int ends, const char* source, int pos, int et, int ed1, int ed2, int ed3, int ed4, int pp, int sf_names, int arg_roots) {
  int _sv0t0 = ps_peek(tags, pos);
  if ((_sv0t0 == 7)) {
    return pos;
  } else {
  }
  int _sv0t1 = parse_expr_sf(tags, starts, ends, source, pos, et, ed1, ed2, ed3, ed4, pp, sf_names, 1);
  int p1 = _sv0t1;
  if ((p1 < 0)) {
    int _sv0t2 = (0 - 1);
    return _sv0t2;
  } else {
  }
  int _sv0t3 = sv0_vec_len(et);
  int _sv0t4 = (_sv0t3 - 1);
  sv0_vec_push(arg_roots, _sv0t4);
  int p = p1;
  int done = 0;
  while ((done != 1)) {
    int _sv0t5 = ps_peek(tags, p);
    int t = _sv0t5;
    if ((t == 12)) {
      int _sv0t6 = (p + 1);
      int _sv0t7 = ps_peek(tags, _sv0t6);
      int nt = _sv0t7;
      if ((nt == 7)) {
        done = 1;
      } else {
        p = (p + 1);
        int _sv0t8 = parse_expr_sf(tags, starts, ends, source, p, et, ed1, ed2, ed3, ed4, pp, sf_names, 1);
        int pa = _sv0t8;
        if ((pa < 0)) {
          int _sv0t9 = (0 - 1);
          return _sv0t9;
        } else {
        }
        int _sv0t10 = sv0_vec_len(et);
        int _sv0t11 = (_sv0t10 - 1);
        sv0_vec_push(arg_roots, _sv0t11);
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
            int _sv0t9 = parse_expr_sf(tags, starts, ends, source, _sv0t8, et, ed1, ed2, ed3, ed4, pp, sf_names, 1);
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
            int _sv0t16 = parse_expr_sf(tags, starts, ends, source, _sv0t15, et, ed1, ed2, ed3, ed4, pp, sf_names, 1);
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
        sv0_vec_push(ed4, fc_before);
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
    int _sv0t19 = parse_expr_sf(tags, starts, ends, source, _sv0t18, et, ed1, ed2, ed3, ed4, pp, sf_names, 1);
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
        int _sv0t27 = parse_expr_sf(tags, starts, ends, source, p, et, ed1, ed2, ed3, ed4, pp, sf_names, 1);
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
    int _sv0t39 = parse_expr_sf(tags, starts, ends, source, _sv0t38, et, ed1, ed2, ed3, ed4, pp, sf_names, 1);
    int p1 = _sv0t39;
    if ((p1 < 0)) {
      int _sv0t40 = (0 - 1);
      return _sv0t40;
    } else {
    }
    int _sv0t41 = ps_peek(tags, p1);
    if ((_sv0t41 == 13)) {
      int _sv0t42 = (p1 + 1);
      int _sv0t43 = parse_expr_sf(tags, starts, ends, source, _sv0t42, et, ed1, ed2, ed3, ed4, pp, sf_names, 1);
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
        int _sv0t53 = parse_expr_sf(tags, starts, ends, source, p, et, ed1, ed2, ed3, ed4, pp, sf_names, 1);
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
  if ((t == 8)) {
    int _sv0t60 = parse_block(tags, starts, ends, source, pos, et, ed1, ed2, ed3, ed4, pp, sf_names);
    return _sv0t60;
  } else {
  }
  if ((t == 68)) {
    int _sv0t61 = (pos + 1);
    int _sv0t62 = parse_if_expr(tags, starts, ends, source, _sv0t61, et, ed1, ed2, ed3, ed4, pp, sf_names);
    return _sv0t62;
  } else {
  }
  if ((t == 96)) {
    int _sv0t63 = (pos + 1);
    int _sv0t64 = parse_while_expr(tags, starts, ends, source, _sv0t63, et, ed1, ed2, ed3, ed4, pp, sf_names);
    return _sv0t64;
  } else {
  }
  if ((t == 66)) {
    int _sv0t65 = (pos + 1);
    int _sv0t66 = parse_for_expr(tags, starts, ends, source, _sv0t65, et, ed1, ed2, ed3, ed4, pp, sf_names);
    return _sv0t66;
  } else {
  }
  if ((t == 72)) {
    int _sv0t67 = (pos + 1);
    int _sv0t68 = parse_loop_expr(tags, starts, ends, source, _sv0t67, et, ed1, ed2, ed3, ed4, pp, sf_names);
    return _sv0t68;
  } else {
  }
  if ((t == 85)) {
    int _sv0t69 = (pos + 1);
    int _sv0t70 = parse_return_expr(tags, starts, ends, source, _sv0t69, et, ed1, ed2, ed3, ed4, pp, sf_names);
    return _sv0t70;
  } else {
  }
  if ((t == 93)) {
    int _sv0t71 = (pos + 1);
    int _sv0t72 = parse_block(tags, starts, ends, source, _sv0t71, et, ed1, ed2, ed3, ed4, pp, sf_names);
    return _sv0t72;
  } else {
  }
  if ((t == 55)) {
    int _sv0t73 = (pos + 1);
    int _sv0t74 = parse_expr_sf(tags, starts, ends, source, _sv0t73, et, ed1, ed2, ed3, ed4, pp, sf_names, 1);
    int pa = _sv0t74;
    if ((pa < 0)) {
      int _sv0t75 = (0 - 1);
      return _sv0t75;
    } else {
    }
    int _sv0t76 = sv0_vec_len(et);
    int ei = (_sv0t76 - 1);
    sv0_vec_push(et, 23);
    sv0_vec_push(ed1, ei);
    sv0_vec_push(ed2, 0);
    sv0_vec_push(ed3, 0);
    sv0_vec_push(ed4, 0);
    return pa;
  } else {
  }
  if ((t == 74)) {
    int _sv0t77 = (pos + 1);
    int _sv0t78 = parse_match_expr(tags, starts, ends, source, _sv0t77, et, ed1, ed2, ed3, ed4, pp, sf_names);
    return _sv0t78;
  } else {
  }
  if ((t == 67)) {
    int _sv0t79 = parse_quant(tags, starts, ends, source, pos, 1, et, ed1, ed2, ed3, ed4, pp);
    return _sv0t79;
  } else {
  }
  if ((t == 63)) {
    int _sv0t80 = parse_quant(tags, starts, ends, source, pos, 0, et, ed1, ed2, ed3, ed4, pp);
    return _sv0t80;
  } else {
  }
  if ((t == 80)) {
    int _sv0t81 = (pos + 1);
    int _sv0t82 = ps_peek(tags, _sv0t81);
    if ((_sv0t82 != 6)) {
      int _sv0t83 = (0 - 1);
      return _sv0t83;
    } else {
    }
    int _sv0t84 = sv0_vec_len(et);
    int first_idx = _sv0t84;
    int _sv0t85 = (pos + 2);
    int _sv0t86 = parse_expr_sf(tags, starts, ends, source, _sv0t85, et, ed1, ed2, ed3, ed4, pp, sf_names, 1);
    int pe = _sv0t86;
    if ((pe < 0)) {
      int _sv0t87 = (0 - 1);
      return _sv0t87;
    } else {
    }
    int _sv0t88 = ps_peek(tags, pe);
    if ((_sv0t88 != 7)) {
      int _sv0t89 = (0 - 1);
      return _sv0t89;
    } else {
    }
    int _sv0t90 = sv0_vec_len(pp);
    int pps = _sv0t90;
    sv0_vec_push(pp, pos);
    sv0_vec_push(et, 1);
    sv0_vec_push(ed1, pps);
    sv0_vec_push(ed2, 1);
    sv0_vec_push(ed3, 0);
    sv0_vec_push(ed4, 0);
    int _sv0t91 = sv0_vec_len(et);
    int callee_idx = (_sv0t91 - 1);
    sv0_vec_push(et, 4);
    sv0_vec_push(ed1, callee_idx);
    sv0_vec_push(ed2, first_idx);
    sv0_vec_push(ed3, 1);
    sv0_vec_push(ed4, 0);
    int _sv0t92 = (pe + 1);
    return _sv0t92;
  } else {
  }
  if ((t == 79)) {
    int _sv0t93 = (pos + 1);
    int _sv0t94 = ps_peek(tags, _sv0t93);
    if ((_sv0t94 != 6)) {
      int _sv0t95 = (0 - 1);
      return _sv0t95;
    } else {
    }
    int _sv0t96 = sv0_vec_len(et);
    int first_idx = _sv0t96;
    int _sv0t97 = (pos + 2);
    int _sv0t98 = parse_expr_sf(tags, starts, ends, source, _sv0t97, et, ed1, ed2, ed3, ed4, pp, sf_names, 1);
    int pa = _sv0t98;
    if ((pa < 0)) {
      int _sv0t99 = (0 - 1);
      return _sv0t99;
    } else {
    }
    int _sv0t100 = ps_peek(tags, pa);
    if ((_sv0t100 != 12)) {
      int _sv0t101 = (0 - 1);
      return _sv0t101;
    } else {
    }
    int _sv0t102 = (pa + 1);
    int _sv0t103 = parse_expr_sf(tags, starts, ends, source, _sv0t102, et, ed1, ed2, ed3, ed4, pp, sf_names, 1);
    int pb = _sv0t103;
    if ((pb < 0)) {
      int _sv0t104 = (0 - 1);
      return _sv0t104;
    } else {
    }
    int _sv0t105 = ps_peek(tags, pb);
    if ((_sv0t105 != 7)) {
      int _sv0t106 = (0 - 1);
      return _sv0t106;
    } else {
    }
    int _sv0t107 = sv0_vec_len(pp);
    int pps = _sv0t107;
    sv0_vec_push(pp, pos);
    sv0_vec_push(et, 1);
    sv0_vec_push(ed1, pps);
    sv0_vec_push(ed2, 1);
    sv0_vec_push(ed3, 0);
    sv0_vec_push(ed4, 0);
    int _sv0t108 = sv0_vec_len(et);
    int callee_idx = (_sv0t108 - 1);
    sv0_vec_push(et, 4);
    sv0_vec_push(ed1, callee_idx);
    sv0_vec_push(ed2, first_idx);
    sv0_vec_push(ed3, 2);
    sv0_vec_push(ed4, 0);
    int _sv0t109 = (pb + 1);
    return _sv0t109;
  } else {
  }
  int _sv0t110 = (0 - 1);
  return _sv0t110;
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
      int _sv0t5 = sv0_vec_new();
      int arg_roots = _sv0t5;
      int _sv0t6 = (p + 1);
      int _sv0t7 = parse_arg_list(tags, starts, ends, source, _sv0t6, et, ed1, ed2, ed3, ed4, pp, sf_names, arg_roots);
      int pa = _sv0t7;
      if ((pa < 0)) {
        int _sv0t8 = (0 - 1);
        return _sv0t8;
      } else {
      }
      int _sv0t9 = sv0_vec_len(arg_roots);
      int argc = _sv0t9;
      int _sv0t10 = ps_peek(tags, pa);
      if ((_sv0t10 != 7)) {
        int _sv0t11 = (0 - 1);
        return _sv0t11;
      } else {
      }
      sv0_vec_push(et, 4);
      sv0_vec_push(ed1, cur);
      sv0_vec_push(ed2, args_before);
      sv0_vec_push(ed3, argc);
      int _sv0t12 = block_stmt_sidecar_push(pp, arg_roots);
      sv0_vec_push(ed4, _sv0t12);
      int _sv0t13 = sv0_vec_len(et);
      cur = (_sv0t13 - 1);
      p = (pa + 1);
    } else {
      if ((t == 16)) {
        int _sv0t14 = (p + 1);
        int _sv0t15 = ps_peek(tags, _sv0t14);
        int nt = _sv0t15;
        if ((nt == 5)) {
          int _sv0t16 = (p + 2);
          int _sv0t17 = ps_peek(tags, _sv0t16);
          int n2 = _sv0t17;
          if ((n2 == 6)) {
            int method_tok = (p + 1);
            int _sv0t18 = sv0_vec_len(et);
            int args_before = _sv0t18;
            int _sv0t19 = sv0_vec_new();
            int arg_roots_m = _sv0t19;
            int _sv0t20 = (p + 3);
            int _sv0t21 = parse_arg_list(tags, starts, ends, source, _sv0t20, et, ed1, ed2, ed3, ed4, pp, sf_names, arg_roots_m);
            int pa = _sv0t21;
            if ((pa < 0)) {
              int _sv0t22 = (0 - 1);
              return _sv0t22;
            } else {
            }
            int _sv0t23 = sv0_vec_len(arg_roots_m);
            int argc = _sv0t23;
            int _sv0t24 = ps_peek(tags, pa);
            if ((_sv0t24 != 7)) {
              int _sv0t25 = (0 - 1);
              return _sv0t25;
            } else {
            }
            sv0_vec_push(et, 5);
            sv0_vec_push(ed1, cur);
            sv0_vec_push(ed2, method_tok);
            sv0_vec_push(ed3, args_before);
            sv0_vec_push(ed4, argc);
            int _sv0t26 = sv0_vec_len(et);
            cur = (_sv0t26 - 1);
            p = (pa + 1);
          } else {
            sv0_vec_push(et, 6);
            sv0_vec_push(ed1, cur);
            int _sv0t27 = (p + 1);
            sv0_vec_push(ed2, _sv0t27);
            sv0_vec_push(ed3, 0);
            sv0_vec_push(ed4, 0);
            int _sv0t28 = sv0_vec_len(et);
            cur = (_sv0t28 - 1);
            p = (p + 2);
          }
        } else {
          if ((nt == 0)) {
            sv0_vec_push(et, 7);
            sv0_vec_push(ed1, cur);
            int _sv0t29 = (p + 1);
            sv0_vec_push(ed2, _sv0t29);
            sv0_vec_push(ed3, 0);
            sv0_vec_push(ed4, 0);
            int _sv0t30 = sv0_vec_len(et);
            cur = (_sv0t30 - 1);
            p = (p + 2);
          } else {
            done = 1;
          }
        }
      } else {
        if ((t == 10)) {
          int _sv0t31 = (p + 1);
          int _sv0t32 = parse_expr_sf(tags, starts, ends, source, _sv0t31, et, ed1, ed2, ed3, ed4, pp, sf_names, 1);
          int ix_p = _sv0t32;
          if ((ix_p < 0)) {
            int _sv0t33 = (0 - 1);
            return _sv0t33;
          } else {
          }
          int _sv0t34 = sv0_vec_len(et);
          int ix_idx = (_sv0t34 - 1);
          int _sv0t35 = ps_peek(tags, ix_p);
          if ((_sv0t35 != 11)) {
            int _sv0t36 = (0 - 1);
            return _sv0t36;
          } else {
          }
          sv0_vec_push(et, 8);
          sv0_vec_push(ed1, cur);
          sv0_vec_push(ed2, ix_idx);
          sv0_vec_push(ed3, 0);
          sv0_vec_push(ed4, 0);
          int _sv0t37 = sv0_vec_len(et);
          cur = (_sv0t37 - 1);
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
      sv0_vec_push(ed1, 12);
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
  int _sv0t1 = parse_expr_sf(tags, starts, ends, source, pos, et, ed1, ed2, ed3, ed4, pp, sf, allow_struct);
  return _sv0t1;
}

static int parse_expr_sf(int tags, int starts, int ends, const char* source, int pos, int et, int ed1, int ed2, int ed3, int ed4, int pp, int sf_names, int allow_struct) {
  int _sv0t0 = sv0_vec_new();
  int tt = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int td1 = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int td2 = _sv0t2;
  int _sv0t3 = sv0_vec_new();
  int td3 = _sv0t3;
  int _sv0t4 = parse_range_expr(tags, starts, ends, source, pos, et, ed1, ed2, ed3, ed4, pp, sf_names, tt, td1, td2, td3, allow_struct);
  return _sv0t4;
}

static int expr_ends_with_braced_block(int et, int idx) {
  int _sv0t0 = sv0_vec_get(et, idx);
  int tag = _sv0t0;
  if ((tag == 9)) {
    return 1;
  } else {
  }
  if ((tag == 10)) {
    return 1;
  } else {
  }
  if ((tag == 11)) {
    return 1;
  } else {
  }
  if ((tag == 12)) {
    return 1;
  } else {
  }
  if ((tag == 13)) {
    return 1;
  } else {
  }
  if ((tag == 14)) {
    return 1;
  } else {
  }
  return 0;
}

static int parse_return_expr(int tags, int starts, int ends, const char* source, int pos, int et, int ed1, int ed2, int ed3, int ed4, int pp, int sf_names) {
  int _sv0t0 = ps_peek(tags, pos);
  int t = _sv0t0;
  if ((t == 13)) {
    sv0_vec_push(et, 15);
    int _sv0t1 = (0 - 1);
    sv0_vec_push(ed1, _sv0t1);
    sv0_vec_push(ed2, 0);
    sv0_vec_push(ed3, 0);
    sv0_vec_push(ed4, 0);
    return pos;
  } else {
  }
  if ((t == 9)) {
    sv0_vec_push(et, 15);
    int _sv0t2 = (0 - 1);
    sv0_vec_push(ed1, _sv0t2);
    sv0_vec_push(ed2, 0);
    sv0_vec_push(ed3, 0);
    sv0_vec_push(ed4, 0);
    return pos;
  } else {
  }
  int _sv0t3 = sv0_vec_new();
  int tt = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int td1 = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int td2 = _sv0t5;
  int _sv0t6 = sv0_vec_new();
  int td3 = _sv0t6;
  int _sv0t7 = parse_range_expr(tags, starts, ends, source, pos, et, ed1, ed2, ed3, ed4, pp, sf_names, tt, td1, td2, td3, 1);
  int p1 = _sv0t7;
  if ((p1 < 0)) {
    int _sv0t8 = (0 - 1);
    return _sv0t8;
  } else {
  }
  int _sv0t9 = sv0_vec_len(et);
  int vi = (_sv0t9 - 1);
  sv0_vec_push(et, 15);
  sv0_vec_push(ed1, vi);
  sv0_vec_push(ed2, 0);
  sv0_vec_push(ed3, 0);
  sv0_vec_push(ed4, 0);
  return p1;
}

static int parse_let_stmt(int tags, int starts, int ends, const char* source, int pos, int et, int ed1, int ed2, int ed3, int ed4, int pp, int sf_names) {
  int is_mut = 0;
  int p = pos;
  int _sv0t0 = ps_peek(tags, p);
  if ((_sv0t0 == 77)) {
    is_mut = 1;
    p = (p + 1);
  } else {
  }
  int _sv0t1 = sv0_vec_new();
  int pt = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int pd1v = _sv0t2;
  int _sv0t3 = sv0_vec_new();
  int pd2v = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int pd3v = _sv0t4;
  int _sv0t5 = parse_pat(tags, starts, ends, source, p, pt, pd1v, pd2v, pd3v, pp);
  int p2 = _sv0t5;
  if ((p2 < 0)) {
    int _sv0t6 = (0 - 1);
    return _sv0t6;
  } else {
  }
  int _sv0t7 = sv0_vec_get(pd1v, 0);
  int name_tok = _sv0t7;
  int type_flag = (0 - 1);
  int p3 = p2;
  int _sv0t8 = ps_peek(tags, p3);
  if ((_sv0t8 == 14)) {
    int _sv0t9 = sv0_vec_new();
    int tytt = _sv0t9;
    int _sv0t10 = sv0_vec_new();
    int tyd1 = _sv0t10;
    int _sv0t11 = sv0_vec_new();
    int tyd2 = _sv0t11;
    int _sv0t12 = sv0_vec_new();
    int tyd3 = _sv0t12;
    int _sv0t13 = (p3 + 1);
    int _sv0t14 = parse_type(tags, starts, ends, source, _sv0t13, tytt, tyd1, tyd2, tyd3, pp);
    int pt2 = _sv0t14;
    if ((pt2 < 0)) {
      int _sv0t15 = (0 - 1);
      return _sv0t15;
    } else {
    }
    type_flag = 0;
    p3 = pt2;
  } else {
  }
  int init_idx = (0 - 1);
  int p4 = p3;
  int _sv0t16 = ps_peek(tags, p4);
  if ((_sv0t16 == 35)) {
    int _sv0t17 = (p4 + 1);
    int _sv0t18 = parse_expr_sf(tags, starts, ends, source, _sv0t17, et, ed1, ed2, ed3, ed4, pp, sf_names, 1);
    int pi = _sv0t18;
    if ((pi < 0)) {
      int _sv0t19 = (0 - 1);
      return _sv0t19;
    } else {
    }
    int _sv0t20 = sv0_vec_len(et);
    init_idx = (_sv0t20 - 1);
    p4 = pi;
  } else {
  }
  int _sv0t21 = ps_peek(tags, p4);
  if ((_sv0t21 != 13)) {
    int _sv0t22 = (0 - 1);
    return _sv0t22;
  } else {
  }
  sv0_vec_push(et, 27);
  sv0_vec_push(ed1, name_tok);
  sv0_vec_push(ed2, type_flag);
  sv0_vec_push(ed3, init_idx);
  sv0_vec_push(ed4, is_mut);
  int _sv0t23 = (p4 + 1);
  return _sv0t23;
}

static int parse_assign_target_op_pos(int tags, int pos, int out) {
  int _sv0t0 = ps_peek(tags, pos);
  if ((_sv0t0 != 5)) {
    int _sv0t1 = (0 - 2);
    return _sv0t1;
  } else {
  }
  int field_tok = (0 - 1);
  int op_pos = (pos + 1);
  int _sv0t2 = (pos + 1);
  int _sv0t3 = ps_peek(tags, _sv0t2);
  if ((_sv0t3 == 15)) {
    int _sv0t4 = (pos + 2);
    int _sv0t5 = ps_peek(tags, _sv0t4);
    if ((_sv0t5 != 5)) {
      int _sv0t6 = (0 - 2);
      return _sv0t6;
    } else {
    }
    field_tok = (pos + 2);
    op_pos = (pos + 3);
  } else {
  }
  sv0_vec_push(out, field_tok);
  sv0_vec_push(out, op_pos);
  return op_pos;
}

static int try_assign_stmt(int tags, int starts, int ends, const char* source, int pos, int et, int ed1, int ed2, int ed3, int ed4, int pp, int sf_names) {
  int _sv0t0 = sv0_vec_new();
  int tmp = _sv0t0;
  int _sv0t1 = parse_assign_target_op_pos(tags, pos, tmp);
  int op_pos = _sv0t1;
  if ((op_pos < 0)) {
    return op_pos;
  } else {
  }
  int _sv0t2 = sv0_vec_get(tmp, 0);
  int lhs_field_tok = _sv0t2;
  int rhs_pos = (op_pos + 1);
  int _sv0t3 = ps_peek(tags, op_pos);
  int nt = _sv0t3;
  if ((nt == 35)) {
    int _sv0t4 = sv0_vec_len(pp);
    int pps = _sv0t4;
    sv0_vec_push(pp, pos);
    sv0_vec_push(et, 1);
    sv0_vec_push(ed1, pps);
    sv0_vec_push(ed2, 1);
    sv0_vec_push(ed3, 0);
    sv0_vec_push(ed4, 0);
    int _sv0t5 = sv0_vec_len(et);
    int lhs_idx = (_sv0t5 - 1);
    if ((lhs_field_tok >= 0)) {
      sv0_vec_push(et, 6);
      sv0_vec_push(ed1, lhs_idx);
      sv0_vec_push(ed2, lhs_field_tok);
      sv0_vec_push(ed3, 0);
      sv0_vec_push(ed4, 0);
      int _sv0t6 = sv0_vec_len(et);
      lhs_idx = (_sv0t6 - 1);
    } else {
    }
    int _sv0t7 = parse_expr_sf(tags, starts, ends, source, rhs_pos, et, ed1, ed2, ed3, ed4, pp, sf_names, 1);
    int pe = _sv0t7;
    if ((pe < 0)) {
      int _sv0t8 = (0 - 1);
      return _sv0t8;
    } else {
    }
    int _sv0t9 = sv0_vec_len(et);
    int rhs_idx = (_sv0t9 - 1);
    int _sv0t10 = ps_peek(tags, pe);
    if ((_sv0t10 != 13)) {
      int _sv0t11 = (0 - 1);
      return _sv0t11;
    } else {
    }
    sv0_vec_push(et, 18);
    sv0_vec_push(ed1, lhs_idx);
    sv0_vec_push(ed2, rhs_idx);
    sv0_vec_push(ed3, 0);
    sv0_vec_push(ed4, 0);
    int _sv0t12 = sv0_vec_len(et);
    int ai = (_sv0t12 - 1);
    sv0_vec_push(et, 28);
    sv0_vec_push(ed1, ai);
    sv0_vec_push(ed2, 0);
    sv0_vec_push(ed3, 0);
    sv0_vec_push(ed4, 0);
    int _sv0t13 = (pe + 1);
    return _sv0t13;
  } else {
  }
  int _sv0t14 = has_assign_binop(nt);
  if ((_sv0t14 == 1)) {
    int _sv0t15 = assign_binop(nt);
    int bop = _sv0t15;
    int _sv0t16 = sv0_vec_len(pp);
    int pps = _sv0t16;
    sv0_vec_push(pp, pos);
    sv0_vec_push(et, 1);
    sv0_vec_push(ed1, pps);
    sv0_vec_push(ed2, 1);
    sv0_vec_push(ed3, 0);
    sv0_vec_push(ed4, 0);
    int _sv0t17 = sv0_vec_len(et);
    int lhs_idx = (_sv0t17 - 1);
    if ((lhs_field_tok >= 0)) {
      sv0_vec_push(et, 6);
      sv0_vec_push(ed1, lhs_idx);
      sv0_vec_push(ed2, lhs_field_tok);
      sv0_vec_push(ed3, 0);
      sv0_vec_push(ed4, 0);
      int _sv0t18 = sv0_vec_len(et);
      lhs_idx = (_sv0t18 - 1);
    } else {
    }
    int _sv0t19 = parse_expr_sf(tags, starts, ends, source, rhs_pos, et, ed1, ed2, ed3, ed4, pp, sf_names, 1);
    int pe = _sv0t19;
    if ((pe < 0)) {
      int _sv0t20 = (0 - 1);
      return _sv0t20;
    } else {
    }
    int _sv0t21 = sv0_vec_len(et);
    int rhs_idx = (_sv0t21 - 1);
    int _sv0t22 = ps_peek(tags, pe);
    if ((_sv0t22 != 13)) {
      int _sv0t23 = (0 - 1);
      return _sv0t23;
    } else {
    }
    sv0_vec_push(et, 19);
    sv0_vec_push(ed1, bop);
    sv0_vec_push(ed2, lhs_idx);
    sv0_vec_push(ed3, rhs_idx);
    sv0_vec_push(ed4, 0);
    int _sv0t24 = sv0_vec_len(et);
    int ai = (_sv0t24 - 1);
    sv0_vec_push(et, 28);
    sv0_vec_push(ed1, ai);
    sv0_vec_push(ed2, 0);
    sv0_vec_push(ed3, 0);
    sv0_vec_push(ed4, 0);
    int _sv0t25 = (pe + 1);
    return _sv0t25;
  } else {
  }
  int _sv0t26 = (0 - 2);
  return _sv0t26;
}

static int try_stmt(int tags, int starts, int ends, const char* source, int pos, int et, int ed1, int ed2, int ed3, int ed4, int pp, int sf_names) {
  int _sv0t0 = ps_peek(tags, pos);
  int t = _sv0t0;
  if ((t == 71)) {
    int _sv0t1 = (pos + 1);
    int _sv0t2 = parse_let_stmt(tags, starts, ends, source, _sv0t1, et, ed1, ed2, ed3, ed4, pp, sf_names);
    return _sv0t2;
  } else {
  }
  if ((t == 57)) {
    int _sv0t3 = (pos + 1);
    int _sv0t4 = ps_peek(tags, _sv0t3);
    if ((_sv0t4 == 13)) {
      sv0_vec_push(et, 16);
      int _sv0t5 = (0 - 1);
      sv0_vec_push(ed1, _sv0t5);
      sv0_vec_push(ed2, 0);
      sv0_vec_push(ed3, 0);
      sv0_vec_push(ed4, 0);
      int _sv0t6 = sv0_vec_len(et);
      int bi = (_sv0t6 - 1);
      sv0_vec_push(et, 28);
      sv0_vec_push(ed1, bi);
      sv0_vec_push(ed2, 0);
      sv0_vec_push(ed3, 0);
      sv0_vec_push(ed4, 0);
      int _sv0t7 = (pos + 2);
      return _sv0t7;
    } else {
    }
    int _sv0t8 = (pos + 1);
    int _sv0t9 = parse_expr_sf(tags, starts, ends, source, _sv0t8, et, ed1, ed2, ed3, ed4, pp, sf_names, 1);
    int pe = _sv0t9;
    if ((pe >= 0)) {
      int _sv0t10 = ps_peek(tags, pe);
      if ((_sv0t10 == 13)) {
        int _sv0t11 = sv0_vec_len(et);
        int vi = (_sv0t11 - 1);
        sv0_vec_push(et, 16);
        sv0_vec_push(ed1, vi);
        sv0_vec_push(ed2, 0);
        sv0_vec_push(ed3, 0);
        sv0_vec_push(ed4, 0);
        int _sv0t12 = sv0_vec_len(et);
        int bi = (_sv0t12 - 1);
        sv0_vec_push(et, 28);
        sv0_vec_push(ed1, bi);
        sv0_vec_push(ed2, 0);
        sv0_vec_push(ed3, 0);
        sv0_vec_push(ed4, 0);
        int _sv0t13 = (pe + 1);
        return _sv0t13;
      } else {
      }
    } else {
    }
    int _sv0t14 = (0 - 2);
    return _sv0t14;
  } else {
  }
  if ((t == 59)) {
    int _sv0t15 = (pos + 1);
    int _sv0t16 = ps_peek(tags, _sv0t15);
    if ((_sv0t16 == 13)) {
      sv0_vec_push(et, 17);
      sv0_vec_push(ed1, 0);
      sv0_vec_push(ed2, 0);
      sv0_vec_push(ed3, 0);
      sv0_vec_push(ed4, 0);
      int _sv0t17 = sv0_vec_len(et);
      int ci = (_sv0t17 - 1);
      sv0_vec_push(et, 28);
      sv0_vec_push(ed1, ci);
      sv0_vec_push(ed2, 0);
      sv0_vec_push(ed3, 0);
      sv0_vec_push(ed4, 0);
      int _sv0t18 = (pos + 2);
      return _sv0t18;
    } else {
    }
    int _sv0t19 = (0 - 2);
    return _sv0t19;
  } else {
  }
  if ((t == 55)) {
    int _sv0t20 = (pos + 1);
    int _sv0t21 = parse_expr_sf(tags, starts, ends, source, _sv0t20, et, ed1, ed2, ed3, ed4, pp, sf_names, 1);
    int pe = _sv0t21;
    if ((pe >= 0)) {
      int _sv0t22 = ps_peek(tags, pe);
      if ((_sv0t22 == 13)) {
        int _sv0t23 = sv0_vec_len(et);
        int ei = (_sv0t23 - 1);
        sv0_vec_push(et, 23);
        sv0_vec_push(ed1, ei);
        sv0_vec_push(ed2, 0);
        sv0_vec_push(ed3, 0);
        sv0_vec_push(ed4, 0);
        int _sv0t24 = sv0_vec_len(et);
        int ai = (_sv0t24 - 1);
        sv0_vec_push(et, 28);
        sv0_vec_push(ed1, ai);
        sv0_vec_push(ed2, 0);
        sv0_vec_push(ed3, 0);
        sv0_vec_push(ed4, 0);
        int _sv0t25 = (pe + 1);
        return _sv0t25;
      } else {
      }
    } else {
    }
    int _sv0t26 = (0 - 2);
    return _sv0t26;
  } else {
  }
  int _sv0t27 = try_assign_stmt(tags, starts, ends, source, pos, et, ed1, ed2, ed3, ed4, pp, sf_names);
  return _sv0t27;
}

static int block_stmt_sidecar_push(int pp, int stmt_ids) {
  int _sv0t0 = sv0_vec_len(pp);
  int off = _sv0t0;
  int k = 0;
  while (1) {
    int _sv0t1 = sv0_vec_len(stmt_ids);
    int _sv0t3 = (k < _sv0t1);
    if ((!_sv0t3)) {
      break;
    } else {
    }
    int _sv0t2 = sv0_vec_get(stmt_ids, k);
    sv0_vec_push(pp, _sv0t2);
    k = (k + 1);
  }
  int _sv0t4 = (off + 1);
  return _sv0t4;
}

static int parse_block(int tags, int starts, int ends, const char* source, int pos, int et, int ed1, int ed2, int ed3, int ed4, int pp, int sf_names) {
  int _sv0t0 = ps_peek(tags, pos);
  if ((_sv0t0 != 8)) {
    int _sv0t1 = (0 - 1);
    return _sv0t1;
  } else {
  }
  int stmts_first = (0 - 1);
  int stmt_count = 0;
  int _sv0t2 = sv0_vec_new();
  int stmt_ids = _sv0t2;
  int p = (pos + 1);
  int blk_done = 0;
  while ((blk_done != 1)) {
    int _sv0t3 = ps_peek(tags, p);
    if ((_sv0t3 == 9)) {
      int sf2 = stmts_first;
      if ((sf2 < 0)) {
        int _sv0t4 = sv0_vec_len(et);
        sf2 = _sv0t4;
      } else {
      }
      sv0_vec_push(et, 9);
      sv0_vec_push(ed1, sf2);
      sv0_vec_push(ed2, stmt_count);
      int _sv0t5 = (0 - 1);
      sv0_vec_push(ed3, _sv0t5);
      int _sv0t6 = block_stmt_sidecar_push(pp, stmt_ids);
      sv0_vec_push(ed4, _sv0t6);
      int _sv0t7 = (p + 1);
      return _sv0t7;
    } else {
    }
    int _sv0t8 = try_stmt(tags, starts, ends, source, p, et, ed1, ed2, ed3, ed4, pp, sf_names);
    int s = _sv0t8;
    if ((s >= 0)) {
      if ((stmts_first < 0)) {
        int _sv0t9 = sv0_vec_len(et);
        stmts_first = (_sv0t9 - 1);
      } else {
      }
      int _sv0t10 = sv0_vec_len(et);
      int _sv0t11 = (_sv0t10 - 1);
      sv0_vec_push(stmt_ids, _sv0t11);
      stmt_count = (stmt_count + 1);
      p = s;
    } else {
      int _sv0t12 = parse_expr_sf(tags, starts, ends, source, p, et, ed1, ed2, ed3, ed4, pp, sf_names, 1);
      int pe = _sv0t12;
      if ((pe < 0)) {
        int _sv0t13 = (0 - 1);
        return _sv0t13;
      } else {
      }
      int _sv0t14 = ps_peek(tags, pe);
      if ((_sv0t14 == 9)) {
        int _sv0t15 = sv0_vec_len(et);
        int tail_idx = (_sv0t15 - 1);
        int sf2 = stmts_first;
        if ((sf2 < 0)) {
          sf2 = tail_idx;
        } else {
        }
        sv0_vec_push(et, 9);
        sv0_vec_push(ed1, sf2);
        sv0_vec_push(ed2, stmt_count);
        sv0_vec_push(ed3, tail_idx);
        int _sv0t16 = block_stmt_sidecar_push(pp, stmt_ids);
        sv0_vec_push(ed4, _sv0t16);
        int _sv0t17 = (pe + 1);
        return _sv0t17;
      } else {
      }
      int _sv0t18 = ps_peek(tags, pe);
      if ((_sv0t18 == 13)) {
        int _sv0t19 = sv0_vec_len(et);
        int ei = (_sv0t19 - 1);
        sv0_vec_push(et, 28);
        sv0_vec_push(ed1, ei);
        sv0_vec_push(ed2, 0);
        sv0_vec_push(ed3, 0);
        sv0_vec_push(ed4, 0);
        if ((stmts_first < 0)) {
          int _sv0t20 = sv0_vec_len(et);
          stmts_first = (_sv0t20 - 1);
        } else {
        }
        int _sv0t21 = sv0_vec_len(et);
        int _sv0t22 = (_sv0t21 - 1);
        sv0_vec_push(stmt_ids, _sv0t22);
        stmt_count = (stmt_count + 1);
        p = (pe + 1);
      } else {
        int _sv0t23 = sv0_vec_len(et);
        int _sv0t24 = (_sv0t23 - 1);
        int _sv0t25 = expr_ends_with_braced_block(et, _sv0t24);
        if ((_sv0t25 == 1)) {
          int _sv0t26 = sv0_vec_len(et);
          int ei = (_sv0t26 - 1);
          sv0_vec_push(et, 28);
          sv0_vec_push(ed1, ei);
          sv0_vec_push(ed2, 0);
          sv0_vec_push(ed3, 0);
          sv0_vec_push(ed4, 0);
          if ((stmts_first < 0)) {
            int _sv0t27 = sv0_vec_len(et);
            stmts_first = (_sv0t27 - 1);
          } else {
          }
          int _sv0t28 = sv0_vec_len(et);
          int _sv0t29 = (_sv0t28 - 1);
          sv0_vec_push(stmt_ids, _sv0t29);
          stmt_count = (stmt_count + 1);
          p = pe;
        } else {
          int _sv0t30 = (0 - 1);
          return _sv0t30;
        }
      }
    }
  }
  int _sv0t31 = (0 - 1);
  return _sv0t31;
}

static int parse_if_expr(int tags, int starts, int ends, const char* source, int pos, int et, int ed1, int ed2, int ed3, int ed4, int pp, int sf_names) {
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
  int _sv0t5 = parse_range_expr(tags, starts, ends, source, pos, et, ed1, ed2, ed3, ed4, pp, sf, tt, td1, td2, td3, 0);
  int p1 = _sv0t5;
  if ((p1 < 0)) {
    int _sv0t6 = (0 - 1);
    return _sv0t6;
  } else {
  }
  int _sv0t7 = sv0_vec_len(et);
  int cond_idx = (_sv0t7 - 1);
  int _sv0t8 = parse_block(tags, starts, ends, source, p1, et, ed1, ed2, ed3, ed4, pp, sf_names);
  int p2 = _sv0t8;
  if ((p2 < 0)) {
    int _sv0t9 = (0 - 1);
    return _sv0t9;
  } else {
  }
  int _sv0t10 = sv0_vec_len(et);
  int then_idx = (_sv0t10 - 1);
  int _sv0t11 = ps_peek(tags, p2);
  if ((_sv0t11 == 60)) {
    int _sv0t12 = (p2 + 1);
    int _sv0t13 = ps_peek(tags, _sv0t12);
    if ((_sv0t13 == 68)) {
      int _sv0t14 = (p2 + 2);
      int _sv0t15 = parse_if_expr(tags, starts, ends, source, _sv0t14, et, ed1, ed2, ed3, ed4, pp, sf_names);
      int p3 = _sv0t15;
      if ((p3 < 0)) {
        int _sv0t16 = (0 - 1);
        return _sv0t16;
      } else {
      }
      int _sv0t17 = sv0_vec_len(et);
      int else_idx = (_sv0t17 - 1);
      sv0_vec_push(et, 10);
      sv0_vec_push(ed1, cond_idx);
      sv0_vec_push(ed2, then_idx);
      sv0_vec_push(ed3, else_idx);
      sv0_vec_push(ed4, 0);
      return p3;
    } else {
    }
    int _sv0t18 = (p2 + 1);
    int _sv0t19 = parse_block(tags, starts, ends, source, _sv0t18, et, ed1, ed2, ed3, ed4, pp, sf_names);
    int p3 = _sv0t19;
    if ((p3 < 0)) {
      int _sv0t20 = (0 - 1);
      return _sv0t20;
    } else {
    }
    int _sv0t21 = sv0_vec_len(et);
    int else_idx = (_sv0t21 - 1);
    sv0_vec_push(et, 10);
    sv0_vec_push(ed1, cond_idx);
    sv0_vec_push(ed2, then_idx);
    sv0_vec_push(ed3, else_idx);
    sv0_vec_push(ed4, 0);
    return p3;
  } else {
  }
  sv0_vec_push(et, 10);
  sv0_vec_push(ed1, cond_idx);
  sv0_vec_push(ed2, then_idx);
  int _sv0t22 = (0 - 1);
  sv0_vec_push(ed3, _sv0t22);
  sv0_vec_push(ed4, 0);
  return p2;
}

static int parse_while_expr(int tags, int starts, int ends, const char* source, int pos, int et, int ed1, int ed2, int ed3, int ed4, int pp, int sf_names) {
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
  int _sv0t5 = parse_range_expr(tags, starts, ends, source, pos, et, ed1, ed2, ed3, ed4, pp, sf, tt, td1, td2, td3, 0);
  int p1 = _sv0t5;
  if ((p1 < 0)) {
    int _sv0t6 = (0 - 1);
    return _sv0t6;
  } else {
  }
  int _sv0t7 = sv0_vec_len(et);
  int cond_idx = (_sv0t7 - 1);
  int _sv0t8 = sv0_vec_len(et);
  int inv_mark = _sv0t8;
  int inv_count = 0;
  int pi = p1;
  int inv_done = 0;
  while ((inv_done != 1)) {
    int _sv0t9 = ps_peek(tags, pi);
    if ((_sv0t9 != 73)) {
      inv_done = 1;
    } else {
      int _sv0t10 = (pi + 1);
      int _sv0t11 = ps_peek(tags, _sv0t10);
      if ((_sv0t11 != 6)) {
        int _sv0t12 = (0 - 1);
        return _sv0t12;
      } else {
      }
      int _sv0t13 = (pi + 2);
      int _sv0t14 = parse_expr_sf(tags, starts, ends, source, _sv0t13, et, ed1, ed2, ed3, ed4, pp, sf_names, 1);
      int pe = _sv0t14;
      if ((pe < 0)) {
        int _sv0t15 = (0 - 1);
        return _sv0t15;
      } else {
      }
      int _sv0t16 = ps_peek(tags, pe);
      if ((_sv0t16 != 7)) {
        int _sv0t17 = (0 - 1);
        return _sv0t17;
      } else {
      }
      inv_count = (inv_count + 1);
      pi = (pe + 1);
    }
  }
  int _sv0t18 = parse_block(tags, starts, ends, source, pi, et, ed1, ed2, ed3, ed4, pp, sf_names);
  int p2 = _sv0t18;
  if ((p2 < 0)) {
    int _sv0t19 = (0 - 1);
    return _sv0t19;
  } else {
  }
  int _sv0t20 = sv0_vec_len(et);
  int body_idx = (_sv0t20 - 1);
  int inv_first = (0 - 1);
  if ((inv_count > 0)) {
    inv_first = inv_mark;
  } else {
  }
  sv0_vec_push(et, 12);
  sv0_vec_push(ed1, cond_idx);
  sv0_vec_push(ed2, body_idx);
  sv0_vec_push(ed3, inv_first);
  sv0_vec_push(ed4, inv_count);
  return p2;
}

static int parse_for_expr(int tags, int starts, int ends, const char* source, int pos, int et, int ed1, int ed2, int ed3, int ed4, int pp, int sf_names) {
  int _sv0t0 = sv0_vec_new();
  int pt = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int pd1v = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int pd2v = _sv0t2;
  int _sv0t3 = sv0_vec_new();
  int pd3v = _sv0t3;
  int _sv0t4 = parse_pat(tags, starts, ends, source, pos, pt, pd1v, pd2v, pd3v, pp);
  int p1 = _sv0t4;
  if ((p1 < 0)) {
    int _sv0t5 = (0 - 1);
    return _sv0t5;
  } else {
  }
  int _sv0t6 = sv0_vec_get(pd1v, 0);
  int pat_tok = _sv0t6;
  int _sv0t7 = ps_peek(tags, p1);
  if ((_sv0t7 != 70)) {
    int _sv0t8 = (0 - 1);
    return _sv0t8;
  } else {
  }
  int _sv0t9 = sv0_vec_new();
  int sf = _sv0t9;
  int _sv0t10 = sv0_vec_new();
  int tt = _sv0t10;
  int _sv0t11 = sv0_vec_new();
  int td1 = _sv0t11;
  int _sv0t12 = sv0_vec_new();
  int td2 = _sv0t12;
  int _sv0t13 = sv0_vec_new();
  int td3 = _sv0t13;
  int _sv0t14 = (p1 + 1);
  int _sv0t15 = parse_range_expr(tags, starts, ends, source, _sv0t14, et, ed1, ed2, ed3, ed4, pp, sf, tt, td1, td2, td3, 0);
  int p2 = _sv0t15;
  if ((p2 < 0)) {
    int _sv0t16 = (0 - 1);
    return _sv0t16;
  } else {
  }
  int _sv0t17 = sv0_vec_len(et);
  int iter_idx = (_sv0t17 - 1);
  int _sv0t18 = parse_block(tags, starts, ends, source, p2, et, ed1, ed2, ed3, ed4, pp, sf_names);
  int p3 = _sv0t18;
  if ((p3 < 0)) {
    int _sv0t19 = (0 - 1);
    return _sv0t19;
  } else {
  }
  int _sv0t20 = sv0_vec_len(et);
  int body_idx = (_sv0t20 - 1);
  sv0_vec_push(et, 13);
  sv0_vec_push(ed1, pat_tok);
  sv0_vec_push(ed2, iter_idx);
  sv0_vec_push(ed3, body_idx);
  sv0_vec_push(ed4, 0);
  return p3;
}

static int parse_loop_expr(int tags, int starts, int ends, const char* source, int pos, int et, int ed1, int ed2, int ed3, int ed4, int pp, int sf_names) {
  int _sv0t0 = parse_block(tags, starts, ends, source, pos, et, ed1, ed2, ed3, ed4, pp, sf_names);
  int pb = _sv0t0;
  if ((pb < 0)) {
    int _sv0t1 = (0 - 1);
    return _sv0t1;
  } else {
  }
  int _sv0t2 = sv0_vec_len(et);
  int body_idx = (_sv0t2 - 1);
  sv0_vec_push(et, 14);
  sv0_vec_push(ed1, body_idx);
  sv0_vec_push(ed2, 0);
  sv0_vec_push(ed3, 0);
  sv0_vec_push(ed4, 0);
  return pb;
}

static int pat_leftmost_subtree_rows(int pat_tags, int pat_d1, int pat_d2, int pat_d3, int s) {
  int _sv0t0 = sv0_vec_get(pat_tags, s);
  int t = _sv0t0;
  if ((t == 0)) {
    return 1;
  } else {
  }
  if ((t == 1)) {
    return 1;
  } else {
  }
  if ((t == 2)) {
    return 1;
  } else {
  }
  if ((t == 3)) {
    int _sv0t1 = sv0_vec_get(pat_d1, s);
    int ef = _sv0t1;
    int _sv0t2 = sv0_vec_get(pat_d2, s);
    int ec = _sv0t2;
    int cur = ef;
    int tot = 0;
    int j = 0;
    while ((j < ec)) {
      int _sv0t3 = pat_leftmost_subtree_rows(pat_tags, pat_d1, pat_d2, pat_d3, cur);
      int part = _sv0t3;
      tot = (tot + part);
      cur = (cur + part);
      j = (j + 1);
    }
    int one = 1;
    int _sv0t4 = (tot + one);
    return _sv0t4;
  } else {
  }
  if ((t == 4)) {
    int _sv0t5 = sv0_vec_get(pat_d3, s);
    int fc = _sv0t5;
    int first_f = (s - fc);
    int cur2 = first_f;
    int tot2 = 0;
    int j2 = 0;
    while ((j2 < fc)) {
      int _sv0t6 = pat_leftmost_subtree_rows(pat_tags, pat_d1, pat_d2, pat_d3, cur2);
      int part2 = _sv0t6;
      tot2 = (tot2 + part2);
      cur2 = (cur2 + part2);
      j2 = (j2 + 1);
    }
    int one2 = 1;
    int _sv0t7 = (tot2 + one2);
    return _sv0t7;
  } else {
  }
  if ((t == 5)) {
    int _sv0t8 = sv0_vec_get(pat_d3, s);
    int ac = _sv0t8;
    int first_a = (s - ac);
    int cur3 = first_a;
    int tot3 = 0;
    int j3 = 0;
    while ((j3 < ac)) {
      int _sv0t9 = pat_leftmost_subtree_rows(pat_tags, pat_d1, pat_d2, pat_d3, cur3);
      int part3 = _sv0t9;
      tot3 = (tot3 + part3);
      cur3 = (cur3 + part3);
      j3 = (j3 + 1);
    }
    int one3 = 1;
    int _sv0t10 = (tot3 + one3);
    return _sv0t10;
  } else {
  }
  if ((t == 6)) {
    int _sv0t11 = sv0_vec_get(pat_d1, s);
    int fo = _sv0t11;
    int _sv0t12 = (s - fo);
    int span = (_sv0t12 + 1);
    return span;
  } else {
  }
  return 1;
}

static int parse_match_expr(int tags, int starts, int ends, const char* source, int pos, int et, int ed1, int ed2, int ed3, int ed4, int pp, int sf_names) {
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
  int _sv0t5 = parse_range_expr(tags, starts, ends, source, pos, et, ed1, ed2, ed3, ed4, pp, sf, tt, td1, td2, td3, 0);
  int p1 = _sv0t5;
  if ((p1 < 0)) {
    int _sv0t6 = (0 - 1);
    return _sv0t6;
  } else {
  }
  int _sv0t7 = sv0_vec_len(et);
  int scrut_idx = (_sv0t7 - 1);
  int _sv0t8 = ps_peek(tags, p1);
  if ((_sv0t8 != 8)) {
    int _sv0t9 = (0 - 1);
    return _sv0t9;
  } else {
  }
  int arms_first = (0 - 1);
  int arm_count = 0;
  int _sv0t10 = sv0_vec_new();
  int arm_ids = _sv0t10;
  int p = (p1 + 1);
  int arms_done = 0;
  while ((arms_done != 1)) {
    int _sv0t11 = ps_peek(tags, p);
    if ((_sv0t11 == 9)) {
      arms_done = 1;
    } else {
      int _sv0t12 = sv0_vec_new();
      int pt = _sv0t12;
      int _sv0t13 = sv0_vec_new();
      int pd1v = _sv0t13;
      int _sv0t14 = sv0_vec_new();
      int pd2v = _sv0t14;
      int _sv0t15 = sv0_vec_new();
      int pd3v = _sv0t15;
      int _sv0t16 = parse_pat(tags, starts, ends, source, p, pt, pd1v, pd2v, pd3v, pp);
      int pa = _sv0t16;
      if ((pa < 0)) {
        int _sv0t17 = (0 - 1);
        return _sv0t17;
      } else {
      }
      int _sv0t18 = sv0_vec_len(pt);
      int pr = (_sv0t18 - 1);
      if ((pr < 0)) {
        int _sv0t19 = (0 - 1);
        return _sv0t19;
      } else {
      }
      int _sv0t20 = sv0_vec_get(pt, pr);
      int pat_tag = _sv0t20;
      int _sv0t21 = sv0_vec_get(pd1v, pr);
      int pat_d1 = _sv0t21;
      int guard_idx = (0 - 1);
      int p2 = pa;
      int _sv0t22 = ps_peek(tags, p2);
      if ((_sv0t22 == 68)) {
        int _sv0t23 = (p2 + 1);
        int _sv0t24 = parse_expr_sf(tags, starts, ends, source, _sv0t23, et, ed1, ed2, ed3, ed4, pp, sf_names, 1);
        int pg = _sv0t24;
        if ((pg < 0)) {
          int _sv0t25 = (0 - 1);
          return _sv0t25;
        } else {
        }
        int _sv0t26 = sv0_vec_len(et);
        guard_idx = (_sv0t26 - 1);
        p2 = pg;
      } else {
      }
      int _sv0t27 = ps_peek(tags, p2);
      if ((_sv0t27 != 20)) {
        int _sv0t28 = (0 - 1);
        return _sv0t28;
      } else {
      }
      int p3 = (p2 + 1);
      int body_pos = 0;
      int _sv0t29 = ps_peek(tags, p3);
      if ((_sv0t29 == 8)) {
        int _sv0t30 = parse_block(tags, starts, ends, source, p3, et, ed1, ed2, ed3, ed4, pp, sf_names);
        body_pos = _sv0t30;
      } else {
        int _sv0t31 = parse_expr_sf(tags, starts, ends, source, p3, et, ed1, ed2, ed3, ed4, pp, sf_names, 1);
        body_pos = _sv0t31;
      }
      if ((body_pos < 0)) {
        int _sv0t32 = (0 - 1);
        return _sv0t32;
      } else {
      }
      int _sv0t33 = sv0_vec_len(et);
      int body_idx = (_sv0t33 - 1);
      int _sv0t34 = sv0_vec_new();
      int alt_roots = _sv0t34;
      if ((pat_tag == 6)) {
        int _sv0t35 = sv0_vec_get(pd1v, pr);
        int pof = _sv0t35;
        int _sv0t36 = sv0_vec_get(pd2v, pr);
        int poc = _sv0t36;
        int crs = pof;
        int aix = 0;
        while ((aix < poc)) {
          int _sv0t37 = pat_leftmost_subtree_rows(pt, pd1v, pd2v, pd3v, crs);
          int spro = _sv0t37;
          int _sv0t38 = (crs + spro);
          int aroot = (_sv0t38 - 1);
          sv0_vec_push(alt_roots, aroot);
          crs = (crs + spro);
          aix = (aix + 1);
        }
      } else {
        sv0_vec_push(alt_roots, pr);
      }
      int _sv0t39 = sv0_vec_len(alt_roots);
      int naltarms = _sv0t39;
      int am = 0;
      while ((am < naltarms)) {
        int _sv0t40 = sv0_vec_get(alt_roots, am);
        int pr_u = _sv0t40;
        int _sv0t41 = sv0_vec_get(pt, pr_u);
        int pat_tag_u = _sv0t41;
        int _sv0t42 = sv0_vec_get(pd1v, pr_u);
        int pat_d1_u = _sv0t42;
        int arm_ed2 = pat_d1_u;
        int _sv0t43 = (pat_tag_u == 2);
        int _sv0t44 = (pat_tag_u == 4);
        int _sv0t45 = (_sv0t43 || _sv0t44);
        int _sv0t46 = (pat_tag_u == 5);
        if ((_sv0t45 || _sv0t46)) {
          if ((pat_tag_u == 4)) {
            int _sv0t47 = sv0_vec_get(pd3v, pr_u);
            int fc_u = _sv0t47;
            int _sv0t48 = sv0_vec_len(et);
            int first_embed = _sv0t48;
            int fj = 0;
            while ((fj < fc_u)) {
              int _sv0t49 = (pr_u - fc_u);
              int row = (_sv0t49 + fj);
              sv0_vec_push(et, 31);
              int _sv0t50 = sv0_vec_get(pd1v, row);
              sv0_vec_push(ed1, _sv0t50);
              int _sv0t51 = sv0_vec_get(pd2v, row);
              sv0_vec_push(ed2, _sv0t51);
              int _sv0t52 = sv0_vec_get(pd3v, row);
              sv0_vec_push(ed3, _sv0t52);
              int _sv0t53 = sv0_vec_get(pt, row);
              sv0_vec_push(ed4, _sv0t53);
              fj = (fj + 1);
            }
            sv0_vec_push(et, 32);
            sv0_vec_push(ed1, fc_u);
            if ((fc_u > 0)) {
              sv0_vec_push(ed2, first_embed);
            } else {
              int _sv0t54 = (0 - 1);
              sv0_vec_push(ed2, _sv0t54);
            }
            sv0_vec_push(ed3, 0);
            sv0_vec_push(ed4, 0);
          } else {
          }
          if ((pat_tag_u == 5)) {
            int _sv0t55 = sv0_vec_get(pd3v, pr_u);
            int ac_u = _sv0t55;
            if ((ac_u > 0)) {
              int _sv0t56 = sv0_vec_len(et);
              int first_emb_e = _sv0t56;
              int aj = 0;
              while ((aj < ac_u)) {
                int _sv0t57 = (pr_u - ac_u);
                int row_e = (_sv0t57 + aj);
                sv0_vec_push(et, 31);
                int _sv0t58 = sv0_vec_get(pd1v, row_e);
                sv0_vec_push(ed1, _sv0t58);
                sv0_vec_push(ed2, aj);
                int _sv0t59 = sv0_vec_get(pd3v, row_e);
                sv0_vec_push(ed3, _sv0t59);
                int _sv0t60 = sv0_vec_get(pt, row_e);
                sv0_vec_push(ed4, _sv0t60);
                aj = (aj + 1);
              }
              sv0_vec_push(et, 33);
              sv0_vec_push(ed1, ac_u);
              sv0_vec_push(ed2, first_emb_e);
              sv0_vec_push(ed3, 0);
              sv0_vec_push(ed4, 0);
            } else {
            }
          } else {
          }
          sv0_vec_push(et, 30);
          int _sv0t61 = sv0_vec_get(pd1v, pr_u);
          sv0_vec_push(ed1, _sv0t61);
          int _sv0t62 = sv0_vec_get(pd2v, pr_u);
          sv0_vec_push(ed2, _sv0t62);
          int _sv0t63 = sv0_vec_get(pd3v, pr_u);
          sv0_vec_push(ed3, _sv0t63);
          sv0_vec_push(ed4, pat_tag_u);
          int _sv0t64 = sv0_vec_len(et);
          arm_ed2 = (_sv0t64 - 1);
        } else {
        }
        sv0_vec_push(et, 29);
        sv0_vec_push(ed1, pat_tag_u);
        sv0_vec_push(ed2, arm_ed2);
        sv0_vec_push(ed3, guard_idx);
        sv0_vec_push(ed4, body_idx);
        if ((arms_first < 0)) {
          int _sv0t65 = sv0_vec_len(et);
          arms_first = (_sv0t65 - 1);
        } else {
        }
        int _sv0t66 = sv0_vec_len(et);
        int _sv0t67 = (_sv0t66 - 1);
        sv0_vec_push(arm_ids, _sv0t67);
        arm_count = (arm_count + 1);
        am = (am + 1);
      }
      int _sv0t68 = ps_peek(tags, body_pos);
      if ((_sv0t68 == 12)) {
        p = (body_pos + 1);
      } else {
        p = body_pos;
      }
    }
  }
  sv0_vec_push(et, 11);
  sv0_vec_push(ed1, scrut_idx);
  sv0_vec_push(ed2, arms_first);
  sv0_vec_push(ed3, arm_count);
  int _sv0t69 = block_stmt_sidecar_push(pp, arm_ids);
  sv0_vec_push(ed4, _sv0t69);
  int _sv0t70 = (p + 1);
  return _sv0t70;
}

static int parse_quant(int tags, int starts, int ends, const char* source, int pos, int is_forall, int et, int ed1, int ed2, int ed3, int ed4, int pp) {
  int _sv0t0 = (pos + 1);
  int _sv0t1 = ps_peek(tags, _sv0t0);
  if ((_sv0t1 != 6)) {
    int _sv0t2 = (0 - 1);
    return _sv0t2;
  } else {
  }
  int _sv0t3 = (pos + 2);
  int _sv0t4 = ps_peek(tags, _sv0t3);
  if ((_sv0t4 != 5)) {
    int _sv0t5 = (0 - 1);
    return _sv0t5;
  } else {
  }
  int id_pos = (pos + 2);
  int _sv0t6 = (pos + 3);
  int _sv0t7 = ps_peek(tags, _sv0t6);
  if ((_sv0t7 != 70)) {
    int _sv0t8 = (0 - 1);
    return _sv0t8;
  } else {
  }
  int _sv0t9 = sv0_vec_len(et);
  int first_idx = _sv0t9;
  int _sv0t10 = sv0_vec_len(pp);
  int pps_id = _sv0t10;
  sv0_vec_push(pp, id_pos);
  sv0_vec_push(et, 1);
  sv0_vec_push(ed1, pps_id);
  sv0_vec_push(ed2, 1);
  sv0_vec_push(ed3, 0);
  sv0_vec_push(ed4, 0);
  int _sv0t11 = (pos + 4);
  int _sv0t12 = parse_expr(tags, starts, ends, source, _sv0t11, et, ed1, ed2, ed3, ed4, pp, 1);
  int pd = _sv0t12;
  if ((pd < 0)) {
    int _sv0t13 = (0 - 1);
    return _sv0t13;
  } else {
  }
  int _sv0t14 = ps_peek(tags, pd);
  if ((_sv0t14 != 12)) {
    int _sv0t15 = (0 - 1);
    return _sv0t15;
  } else {
  }
  int _sv0t16 = (pd + 1);
  int _sv0t17 = parse_expr(tags, starts, ends, source, _sv0t16, et, ed1, ed2, ed3, ed4, pp, 1);
  int pb = _sv0t17;
  if ((pb < 0)) {
    int _sv0t18 = (0 - 1);
    return _sv0t18;
  } else {
  }
  int _sv0t19 = ps_peek(tags, pb);
  if ((_sv0t19 != 7)) {
    int _sv0t20 = (0 - 1);
    return _sv0t20;
  } else {
  }
  int _sv0t21 = sv0_vec_len(pp);
  int pps_q = _sv0t21;
  sv0_vec_push(pp, pos);
  sv0_vec_push(et, 1);
  sv0_vec_push(ed1, pps_q);
  sv0_vec_push(ed2, 1);
  sv0_vec_push(ed3, 0);
  sv0_vec_push(ed4, 0);
  int _sv0t22 = sv0_vec_len(et);
  int callee_idx = (_sv0t22 - 1);
  sv0_vec_push(et, 4);
  sv0_vec_push(ed1, callee_idx);
  sv0_vec_push(ed2, first_idx);
  sv0_vec_push(ed3, 3);
  sv0_vec_push(ed4, 0);
  int _sv0t23 = (pb + 1);
  return _sv0t23;
}

static int parse_param(int tags, int starts, int ends, const char* source, int pos, int pp, int fn_param_name_toks, int pty_tt, int pty_td1, int pty_td2, int pty_td3, int fn_param_ty_root) {
  int p = pos;
  int _sv0t0 = ps_peek(tags, p);
  if ((_sv0t0 == 77)) {
    p = (p + 1);
  } else {
  }
  int _sv0t1 = ps_peek(tags, p);
  int t = _sv0t1;
  if ((t == 5)) {
    sv0_vec_push(fn_param_name_toks, p);
  } else {
    if ((t != 86)) {
      int _sv0t2 = (0 - 1);
      return _sv0t2;
    } else {
    }
    int _sv0t3 = (0 - 1);
    sv0_vec_push(fn_param_name_toks, _sv0t3);
  }
  p = (p + 1);
  int _sv0t4 = ps_peek(tags, p);
  if ((_sv0t4 != 14)) {
    int _sv0t5 = (0 - 1);
    return _sv0t5;
  } else {
  }
  int _sv0t6 = (p + 1);
  int _sv0t7 = parse_type(tags, starts, ends, source, _sv0t6, pty_tt, pty_td1, pty_td2, pty_td3, pp);
  int pt = _sv0t7;
  if ((pt < 0)) {
    int _sv0t8 = (0 - 1);
    return _sv0t8;
  } else {
  }
  int _sv0t9 = sv0_vec_len(pty_tt);
  int _sv0t10 = (_sv0t9 - 1);
  sv0_vec_push(fn_param_ty_root, _sv0t10);
  return pt;
}

static int parse_param_list(int tags, int starts, int ends, const char* source, int pos, int pp, int count_out, int fn_param_name_toks, int pty_tt, int pty_td1, int pty_td2, int pty_td3, int fn_param_ty_root) {
  int _sv0t0 = ps_peek(tags, pos);
  if ((_sv0t0 == 7)) {
    sv0_vec_push(count_out, 0);
    return pos;
  } else {
  }
  int _sv0t1 = parse_param(tags, starts, ends, source, pos, pp, fn_param_name_toks, pty_tt, pty_td1, pty_td2, pty_td3, fn_param_ty_root);
  int p1 = _sv0t1;
  if ((p1 < 0)) {
    int _sv0t2 = (0 - 1);
    return _sv0t2;
  } else {
  }
  int count = 1;
  int p = p1;
  int done = 0;
  while ((done != 1)) {
    int _sv0t3 = ps_peek(tags, p);
    if ((_sv0t3 == 12)) {
      int _sv0t4 = (p + 1);
      int _sv0t5 = ps_peek(tags, _sv0t4);
      if ((_sv0t5 == 7)) {
        p = (p + 1);
        done = 1;
      } else {
        int _sv0t6 = (p + 1);
        int _sv0t7 = parse_param(tags, starts, ends, source, _sv0t6, pp, fn_param_name_toks, pty_tt, pty_td1, pty_td2, pty_td3, fn_param_ty_root);
        int pn = _sv0t7;
        if ((pn < 0)) {
          int _sv0t8 = (0 - 1);
          return _sv0t8;
        } else {
        }
        count = (count + 1);
        p = pn;
      }
    } else {
      done = 1;
    }
  }
  sv0_vec_push(count_out, count);
  return p;
}

static int parse_contract(int tags, int starts, int ends, const char* source, int pos, int pp, int et, int ed1, int ed2, int ed3, int ed4, int contract_root) {
  int _sv0t0 = ps_peek(tags, pos);
  int t = _sv0t0;
  if ((t != 83)) {
    if ((t != 61)) {
      if ((t != 73)) {
        int _sv0t1 = (0 - 2);
        return _sv0t1;
      } else {
      }
    } else {
    }
  } else {
  }
  int _sv0t2 = (pos + 1);
  int _sv0t3 = ps_peek(tags, _sv0t2);
  if ((_sv0t3 != 6)) {
    int _sv0t4 = (0 - 1);
    return _sv0t4;
  } else {
  }
  int _sv0t5 = (pos + 2);
  int _sv0t6 = parse_expr(tags, starts, ends, source, _sv0t5, et, ed1, ed2, ed3, ed4, pp, 1);
  int pe = _sv0t6;
  if ((pe < 0)) {
    int _sv0t7 = (0 - 1);
    return _sv0t7;
  } else {
  }
  int _sv0t8 = ps_peek(tags, pe);
  if ((_sv0t8 != 7)) {
    int _sv0t9 = (0 - 1);
    return _sv0t9;
  } else {
  }
  int _sv0t10 = sv0_vec_len(et);
  int _sv0t11 = (_sv0t10 - 1);
  sv0_vec_push(contract_root, _sv0t11);
  int _sv0t12 = (pe + 1);
  return _sv0t12;
}

static int parse_contracts(int tags, int starts, int ends, const char* source, int pos, int pp, int et, int ed1, int ed2, int ed3, int ed4, int contract_root, int count_out) {
  int p = pos;
  int count = 0;
  int done = 0;
  while ((done != 1)) {
    int _sv0t0 = parse_contract(tags, starts, ends, source, p, pp, et, ed1, ed2, ed3, ed4, contract_root);
    int pc = _sv0t0;
    int _sv0t1 = (0 - 2);
    if ((pc == _sv0t1)) {
      done = 1;
    } else {
      if ((pc < 0)) {
        int _sv0t2 = (0 - 1);
        return _sv0t2;
      } else {
      }
      count = (count + 1);
      p = pc;
    }
  }
  sv0_vec_push(count_out, count);
  return p;
}

static int parse_fn_body(int tags, int starts, int ends, const char* source, int pos, int it, int id1, int id2, int id3, int id4, int id5, int pp, int fn_param_name_toks, int fn_param_ty_root, int fn_ret_ty_root_by_item, int fn_contract_base_by_item, int fn_contract_root, int enum_variant_payload_base_by_item, int enum_variant_payload_count_by_item, int pty_tt, int pty_td1, int pty_td2, int pty_td3, int body_et, int body_ed1, int body_ed2, int body_ed3, int body_ed4, int body_sf) {
  int _sv0t0 = ps_skip_optional_unsafe(tags, pos);
  int p0 = _sv0t0;
  int _sv0t1 = ps_peek(tags, p0);
  if ((_sv0t1 != 65)) {
    int _sv0t2 = (0 - 1);
    return _sv0t2;
  } else {
  }
  int _sv0t3 = (p0 + 1);
  int _sv0t4 = ps_peek(tags, _sv0t3);
  if ((_sv0t4 != 5)) {
    int _sv0t5 = (0 - 1);
    return _sv0t5;
  } else {
  }
  int name_pos = (p0 + 1);
  int _sv0t6 = sv0_vec_new();
  int gp = _sv0t6;
  int _sv0t7 = (p0 + 2);
  int _sv0t8 = parse_generic_params(tags, _sv0t7, gp);
  int p1 = _sv0t8;
  int _sv0t9 = ps_peek(tags, p1);
  if ((_sv0t9 != 6)) {
    int _sv0t10 = (0 - 1);
    return _sv0t10;
  } else {
  }
  int _sv0t11 = sv0_vec_len(fn_param_name_toks);
  int pbase = _sv0t11;
  int _sv0t12 = sv0_vec_new();
  int pc = _sv0t12;
  int _sv0t13 = (p1 + 1);
  int _sv0t14 = parse_param_list(tags, starts, ends, source, _sv0t13, pp, pc, fn_param_name_toks, pty_tt, pty_td1, pty_td2, pty_td3, fn_param_ty_root);
  int p2 = _sv0t14;
  if ((p2 < 0)) {
    int _sv0t15 = (0 - 1);
    return _sv0t15;
  } else {
  }
  int _sv0t16 = ps_peek(tags, p2);
  if ((_sv0t16 != 7)) {
    int _sv0t17 = (0 - 1);
    return _sv0t17;
  } else {
  }
  int _sv0t18 = sv0_vec_get(pc, 0);
  int param_count = _sv0t18;
  int has_ret = 0;
  int ret_ty_root = (0 - 1);
  int p3 = (p2 + 1);
  int _sv0t19 = ps_peek(tags, p3);
  if ((_sv0t19 == 19)) {
    int _sv0t20 = (p3 + 1);
    int _sv0t21 = parse_type(tags, starts, ends, source, _sv0t20, pty_tt, pty_td1, pty_td2, pty_td3, pp);
    int pt = _sv0t21;
    if ((pt < 0)) {
      int _sv0t22 = (0 - 1);
      return _sv0t22;
    } else {
    }
    has_ret = 1;
    int _sv0t23 = sv0_vec_len(pty_tt);
    ret_ty_root = (_sv0t23 - 1);
    p3 = pt;
  } else {
  }
  int _sv0t24 = ps_skip_where_clause(tags, p3);
  int p4 = _sv0t24;
  int _sv0t25 = sv0_vec_len(fn_contract_root);
  int contract_base = _sv0t25;
  int _sv0t26 = sv0_vec_new();
  int cc = _sv0t26;
  int _sv0t27 = parse_contracts(tags, starts, ends, source, p4, pp, body_et, body_ed1, body_ed2, body_ed3, body_ed4, fn_contract_root, cc);
  int p5 = _sv0t27;
  if ((p5 < 0)) {
    int _sv0t28 = (0 - 1);
    return _sv0t28;
  } else {
  }
  int _sv0t29 = sv0_vec_get(cc, 0);
  int contract_count = _sv0t29;
  int _sv0t30 = parse_block(tags, starts, ends, source, p5, body_et, body_ed1, body_ed2, body_ed3, body_ed4, pp, body_sf);
  int p6 = _sv0t30;
  if ((p6 < 0)) {
    int _sv0t31 = (0 - 1);
    return _sv0t31;
  } else {
  }
  int _sv0t32 = sv0_vec_len(body_et);
  int body_root = (_sv0t32 - 1);
  sv0_vec_push(it, 0);
  sv0_vec_push(id1, name_pos);
  int _sv0t33 = (contract_count * 2);
  int _sv0t34 = (has_ret + _sv0t33);
  sv0_vec_push(id2, _sv0t34);
  sv0_vec_push(id3, param_count);
  sv0_vec_push(id4, body_root);
  sv0_vec_push(id5, pbase);
  sv0_vec_push(fn_ret_ty_root_by_item, ret_ty_root);
  sv0_vec_push(fn_contract_base_by_item, contract_base);
  int _sv0t35 = (0 - 1);
  sv0_vec_push(enum_variant_payload_base_by_item, _sv0t35);
  sv0_vec_push(enum_variant_payload_count_by_item, 0);
  return p6;
}

static int parse_struct_item(int tags, int starts, int ends, const char* source, int pos, int it, int id1, int id2, int id3, int id4, int id5, int pp, int struct_field_name_toks, int fn_ret_ty_root_by_item, int fn_contract_base_by_item, int enum_variant_payload_base_by_item, int enum_variant_payload_count_by_item, int pty_tt, int pty_td1, int pty_td2, int pty_td3, int struct_field_ty_root) {
  int _sv0t0 = ps_peek(tags, pos);
  if ((_sv0t0 != 89)) {
    int _sv0t1 = (0 - 1);
    return _sv0t1;
  } else {
  }
  int _sv0t2 = (pos + 1);
  int _sv0t3 = ps_peek(tags, _sv0t2);
  if ((_sv0t3 != 5)) {
    int _sv0t4 = (0 - 1);
    return _sv0t4;
  } else {
  }
  int name_pos = (pos + 1);
  int _sv0t5 = sv0_vec_new();
  int gp = _sv0t5;
  int _sv0t6 = (pos + 2);
  int _sv0t7 = parse_generic_params(tags, _sv0t6, gp);
  int p1 = _sv0t7;
  int _sv0t8 = ps_peek(tags, p1);
  if ((_sv0t8 != 8)) {
    int _sv0t9 = (0 - 1);
    return _sv0t9;
  } else {
  }
  int p = (p1 + 1);
  int field_count = 0;
  int _sv0t10 = sv0_vec_len(struct_field_name_toks);
  int sf_base = _sv0t10;
  int done = 0;
  while ((done != 1)) {
    int _sv0t11 = ps_peek(tags, p);
    if ((_sv0t11 == 9)) {
      done = 1;
    } else {
      int _sv0t12 = ps_skip_optional_pub(tags, p);
      int fp = _sv0t12;
      int _sv0t13 = ps_peek(tags, fp);
      if ((_sv0t13 != 5)) {
        int _sv0t14 = (0 - 1);
        return _sv0t14;
      } else {
      }
      sv0_vec_push(struct_field_name_toks, fp);
      int _sv0t15 = (fp + 1);
      int _sv0t16 = ps_peek(tags, _sv0t15);
      if ((_sv0t16 != 14)) {
        int _sv0t17 = (0 - 1);
        return _sv0t17;
      } else {
      }
      int _sv0t18 = (fp + 2);
      int _sv0t19 = parse_type(tags, starts, ends, source, _sv0t18, pty_tt, pty_td1, pty_td2, pty_td3, pp);
      int pt = _sv0t19;
      if ((pt < 0)) {
        int _sv0t20 = (0 - 1);
        return _sv0t20;
      } else {
      }
      int _sv0t21 = sv0_vec_len(pty_tt);
      int _sv0t22 = (_sv0t21 - 1);
      sv0_vec_push(struct_field_ty_root, _sv0t22);
      field_count = (field_count + 1);
      int _sv0t23 = ps_peek(tags, pt);
      if ((_sv0t23 == 12)) {
        p = (pt + 1);
      } else {
        int _sv0t24 = ps_peek(tags, pt);
        if ((_sv0t24 == 9)) {
          p = pt;
        } else {
          int _sv0t25 = (0 - 1);
          return _sv0t25;
        }
      }
    }
  }
  sv0_vec_push(it, 1);
  sv0_vec_push(id1, name_pos);
  sv0_vec_push(id2, field_count);
  sv0_vec_push(id3, sf_base);
  sv0_vec_push(id4, field_count);
  int _sv0t26 = (0 - 1);
  sv0_vec_push(id5, _sv0t26);
  int _sv0t27 = (0 - 1);
  sv0_vec_push(fn_ret_ty_root_by_item, _sv0t27);
  int _sv0t28 = (0 - 1);
  sv0_vec_push(fn_contract_base_by_item, _sv0t28);
  int _sv0t29 = (0 - 1);
  sv0_vec_push(enum_variant_payload_base_by_item, _sv0t29);
  sv0_vec_push(enum_variant_payload_count_by_item, 0);
  int _sv0t30 = (p + 1);
  return _sv0t30;
}

static int parse_enum_item(int tags, int starts, int ends, const char* source, int pos, int it, int id1, int id2, int id3, int id4, int id5, int pp, int enum_variant_name_toks, int fn_ret_ty_root_by_item, int fn_contract_base_by_item, int enum_variant_payload_ty_root, int enum_variant_payload_base_by_item, int enum_variant_payload_count_by_item, int pty_tt, int pty_td1, int pty_td2, int pty_td3) {
  int _sv0t0 = ps_peek(tags, pos);
  if ((_sv0t0 != 62)) {
    int _sv0t1 = (0 - 1);
    return _sv0t1;
  } else {
  }
  int _sv0t2 = (pos + 1);
  int _sv0t3 = ps_peek(tags, _sv0t2);
  if ((_sv0t3 != 5)) {
    int _sv0t4 = (0 - 1);
    return _sv0t4;
  } else {
  }
  int name_pos = (pos + 1);
  int _sv0t5 = sv0_vec_new();
  int gp = _sv0t5;
  int _sv0t6 = (pos + 2);
  int _sv0t7 = parse_generic_params(tags, _sv0t6, gp);
  int p1 = _sv0t7;
  int _sv0t8 = ps_peek(tags, p1);
  if ((_sv0t8 != 8)) {
    int _sv0t9 = (0 - 1);
    return _sv0t9;
  } else {
  }
  int p = (p1 + 1);
  int variant_count = 0;
  int _sv0t10 = sv0_vec_len(enum_variant_name_toks);
  int ev_base = _sv0t10;
  int _sv0t11 = sv0_vec_len(enum_variant_payload_ty_root);
  int payload_base = _sv0t11;
  int payload_count = 0;
  int done = 0;
  while ((done != 1)) {
    int _sv0t12 = ps_peek(tags, p);
    if ((_sv0t12 == 9)) {
      done = 1;
    } else {
      int _sv0t13 = ps_peek(tags, p);
      if ((_sv0t13 != 5)) {
        int _sv0t14 = (0 - 1);
        return _sv0t14;
      } else {
      }
      sv0_vec_push(enum_variant_name_toks, p);
      int vp = (p + 1);
      int _sv0t15 = ps_peek(tags, vp);
      if ((_sv0t15 == 6)) {
        int tp = (vp + 1);
        int _sv0t16 = ps_peek(tags, tp);
        if ((_sv0t16 == 7)) {
          tp = (tp + 1);
        } else {
          int td = 0;
          while ((td != 1)) {
            int _sv0t17 = parse_type(tags, starts, ends, source, tp, pty_tt, pty_td1, pty_td2, pty_td3, pp);
            int pt = _sv0t17;
            if ((pt < 0)) {
              int _sv0t18 = (0 - 1);
              return _sv0t18;
            } else {
            }
            int _sv0t19 = sv0_vec_len(pty_tt);
            int _sv0t20 = (_sv0t19 - 1);
            sv0_vec_push(enum_variant_payload_ty_root, _sv0t20);
            payload_count = (payload_count + 1);
            int _sv0t21 = ps_peek(tags, pt);
            if ((_sv0t21 == 12)) {
              tp = (pt + 1);
            } else {
              int _sv0t22 = ps_peek(tags, pt);
              if ((_sv0t22 == 7)) {
                tp = (pt + 1);
                td = 1;
              } else {
                int _sv0t23 = (0 - 1);
                return _sv0t23;
              }
            }
          }
        }
        variant_count = (variant_count + 1);
        int _sv0t24 = ps_peek(tags, tp);
        if ((_sv0t24 == 12)) {
          p = (tp + 1);
        } else {
          int _sv0t25 = ps_peek(tags, tp);
          if ((_sv0t25 == 9)) {
            p = tp;
          } else {
            int _sv0t26 = (0 - 1);
            return _sv0t26;
          }
        }
      } else {
        int _sv0t27 = ps_peek(tags, vp);
        if ((_sv0t27 == 8)) {
          int sp = (vp + 1);
          int sd = 0;
          while ((sd != 1)) {
            int _sv0t28 = ps_peek(tags, sp);
            if ((_sv0t28 == 9)) {
              sp = (sp + 1);
              sd = 1;
            } else {
              int _sv0t29 = ps_peek(tags, sp);
              if ((_sv0t29 != 5)) {
                int _sv0t30 = (0 - 1);
                return _sv0t30;
              } else {
              }
              int _sv0t31 = (sp + 1);
              int _sv0t32 = ps_peek(tags, _sv0t31);
              if ((_sv0t32 != 14)) {
                int _sv0t33 = (0 - 1);
                return _sv0t33;
              } else {
              }
              int _sv0t34 = (sp + 2);
              int _sv0t35 = parse_type(tags, starts, ends, source, _sv0t34, pty_tt, pty_td1, pty_td2, pty_td3, pp);
              int pt = _sv0t35;
              if ((pt < 0)) {
                int _sv0t36 = (0 - 1);
                return _sv0t36;
              } else {
              }
              int _sv0t37 = sv0_vec_len(pty_tt);
              int _sv0t38 = (_sv0t37 - 1);
              sv0_vec_push(enum_variant_payload_ty_root, _sv0t38);
              payload_count = (payload_count + 1);
              int _sv0t39 = ps_peek(tags, pt);
              if ((_sv0t39 == 12)) {
                sp = (pt + 1);
              } else {
                int _sv0t40 = ps_peek(tags, pt);
                if ((_sv0t40 == 9)) {
                  sp = pt;
                } else {
                  int _sv0t41 = (0 - 1);
                  return _sv0t41;
                }
              }
            }
          }
          variant_count = (variant_count + 1);
          int _sv0t42 = ps_peek(tags, sp);
          if ((_sv0t42 == 12)) {
            p = (sp + 1);
          } else {
            int _sv0t43 = ps_peek(tags, sp);
            if ((_sv0t43 == 9)) {
              p = sp;
            } else {
              int _sv0t44 = (0 - 1);
              return _sv0t44;
            }
          }
        } else {
          variant_count = (variant_count + 1);
          int _sv0t45 = ps_peek(tags, vp);
          if ((_sv0t45 == 12)) {
            p = (vp + 1);
          } else {
            int _sv0t46 = ps_peek(tags, vp);
            if ((_sv0t46 == 9)) {
              p = vp;
            } else {
              int _sv0t47 = (0 - 1);
              return _sv0t47;
            }
          }
        }
      }
    }
  }
  sv0_vec_push(it, 2);
  sv0_vec_push(id1, name_pos);
  sv0_vec_push(id2, variant_count);
  sv0_vec_push(id3, ev_base);
  sv0_vec_push(id4, variant_count);
  int _sv0t48 = (0 - 1);
  sv0_vec_push(id5, _sv0t48);
  int _sv0t49 = (0 - 1);
  sv0_vec_push(fn_ret_ty_root_by_item, _sv0t49);
  int _sv0t50 = (0 - 1);
  sv0_vec_push(fn_contract_base_by_item, _sv0t50);
  sv0_vec_push(enum_variant_payload_base_by_item, payload_base);
  sv0_vec_push(enum_variant_payload_count_by_item, payload_count);
  int _sv0t51 = (p + 1);
  return _sv0t51;
}

static int parse_type_alias_item(int tags, int starts, int ends, const char* source, int pos, int it, int id1, int id2, int id3, int id4, int id5, int pp, int fn_ret_ty_root_by_item, int fn_contract_base_by_item, int enum_variant_payload_base_by_item, int enum_variant_payload_count_by_item, int pty_tt, int pty_td1, int pty_td2, int pty_td3) {
  int _sv0t0 = ps_peek(tags, pos);
  if ((_sv0t0 != 92)) {
    int _sv0t1 = (0 - 1);
    return _sv0t1;
  } else {
  }
  int _sv0t2 = (pos + 1);
  int _sv0t3 = ps_peek(tags, _sv0t2);
  if ((_sv0t3 != 5)) {
    int _sv0t4 = (0 - 1);
    return _sv0t4;
  } else {
  }
  int name_pos = (pos + 1);
  int _sv0t5 = (pos + 2);
  int _sv0t6 = ps_skip_generic_params(tags, _sv0t5);
  int p1 = _sv0t6;
  int _sv0t7 = ps_peek(tags, p1);
  if ((_sv0t7 != 35)) {
    int _sv0t8 = (0 - 1);
    return _sv0t8;
  } else {
  }
  int _sv0t9 = (p1 + 1);
  int _sv0t10 = parse_type(tags, starts, ends, source, _sv0t9, pty_tt, pty_td1, pty_td2, pty_td3, pp);
  int pt = _sv0t10;
  if ((pt < 0)) {
    int _sv0t11 = (0 - 1);
    return _sv0t11;
  } else {
  }
  int _sv0t12 = ps_peek(tags, pt);
  if ((_sv0t12 != 13)) {
    int _sv0t13 = (0 - 1);
    return _sv0t13;
  } else {
  }
  int _sv0t14 = sv0_vec_len(pty_tt);
  int alias_ty_root = (_sv0t14 - 1);
  sv0_vec_push(it, 7);
  sv0_vec_push(id1, name_pos);
  sv0_vec_push(id2, alias_ty_root);
  sv0_vec_push(id3, 0);
  sv0_vec_push(id4, 0);
  int _sv0t15 = (0 - 1);
  sv0_vec_push(id5, _sv0t15);
  int _sv0t16 = (0 - 1);
  sv0_vec_push(fn_ret_ty_root_by_item, _sv0t16);
  int _sv0t17 = (0 - 1);
  sv0_vec_push(fn_contract_base_by_item, _sv0t17);
  int _sv0t18 = (0 - 1);
  sv0_vec_push(enum_variant_payload_base_by_item, _sv0t18);
  sv0_vec_push(enum_variant_payload_count_by_item, 0);
  int _sv0t19 = (pt + 1);
  return _sv0t19;
}

static int parse_trait_item(int tags, int starts, int ends, const char* source, int pos, int it, int id1, int id2, int id3, int id4, int id5, int pp, int fn_param_name_toks, int fn_param_ty_root, int fn_ret_ty_root_by_item, int fn_contract_base_by_item, int fn_contract_root, int enum_variant_payload_base_by_item, int enum_variant_payload_count_by_item, int pty_tt, int pty_td1, int pty_td2, int pty_td3, int body_et, int body_ed1, int body_ed2, int body_ed3, int body_ed4, int body_sf) {
  int _sv0t0 = ps_peek(tags, pos);
  if ((_sv0t0 != 90)) {
    int _sv0t1 = (0 - 1);
    return _sv0t1;
  } else {
  }
  int _sv0t2 = (pos + 1);
  int _sv0t3 = ps_peek(tags, _sv0t2);
  if ((_sv0t3 != 5)) {
    int _sv0t4 = (0 - 1);
    return _sv0t4;
  } else {
  }
  int name_pos = (pos + 1);
  int _sv0t5 = (pos + 2);
  int _sv0t6 = ps_skip_generic_params(tags, _sv0t5);
  int p1 = _sv0t6;
  int _sv0t7 = ps_peek(tags, p1);
  if ((_sv0t7 != 8)) {
    int _sv0t8 = (0 - 1);
    return _sv0t8;
  } else {
  }
  int p = (p1 + 1);
  int method_count = 0;
  int done = 0;
  while ((done != 1)) {
    int _sv0t9 = ps_peek(tags, p);
    if ((_sv0t9 == 9)) {
      done = 1;
    } else {
      int _sv0t10 = parse_fn_body(tags, starts, ends, source, p, it, id1, id2, id3, id4, id5, pp, fn_param_name_toks, fn_param_ty_root, fn_ret_ty_root_by_item, fn_contract_base_by_item, fn_contract_root, enum_variant_payload_base_by_item, enum_variant_payload_count_by_item, pty_tt, pty_td1, pty_td2, pty_td3, body_et, body_ed1, body_ed2, body_ed3, body_ed4, body_sf);
      int pm = _sv0t10;
      if ((pm < 0)) {
        int _sv0t11 = (0 - 1);
        return _sv0t11;
      } else {
      }
      method_count = (method_count + 1);
      p = pm;
    }
  }
  sv0_vec_push(it, 3);
  sv0_vec_push(id1, name_pos);
  sv0_vec_push(id2, method_count);
  sv0_vec_push(id3, 0);
  int _sv0t12 = (0 - 1);
  sv0_vec_push(id4, _sv0t12);
  int _sv0t13 = (0 - 1);
  sv0_vec_push(id5, _sv0t13);
  int _sv0t14 = (0 - 1);
  sv0_vec_push(fn_ret_ty_root_by_item, _sv0t14);
  int _sv0t15 = (0 - 1);
  sv0_vec_push(fn_contract_base_by_item, _sv0t15);
  int _sv0t16 = (0 - 1);
  sv0_vec_push(enum_variant_payload_base_by_item, _sv0t16);
  sv0_vec_push(enum_variant_payload_count_by_item, 0);
  int _sv0t17 = (p + 1);
  return _sv0t17;
}

static int parse_impl_item(int tags, int starts, int ends, const char* source, int pos, int it, int id1, int id2, int id3, int id4, int id5, int pp, int fn_param_name_toks, int fn_param_ty_root, int fn_ret_ty_root_by_item, int fn_contract_base_by_item, int fn_contract_root, int enum_variant_payload_base_by_item, int enum_variant_payload_count_by_item, int pty_tt, int pty_td1, int pty_td2, int pty_td3, int body_et, int body_ed1, int body_ed2, int body_ed3, int body_ed4, int body_sf) {
  int _sv0t0 = ps_peek(tags, pos);
  if ((_sv0t0 != 69)) {
    int _sv0t1 = (0 - 1);
    return _sv0t1;
  } else {
  }
  int _sv0t2 = (pos + 1);
  int _sv0t3 = ps_skip_generic_params(tags, _sv0t2);
  int p0 = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int tt = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int td1v = _sv0t5;
  int _sv0t6 = sv0_vec_new();
  int td2v = _sv0t6;
  int _sv0t7 = sv0_vec_new();
  int td3v = _sv0t7;
  int _sv0t8 = parse_type(tags, starts, ends, source, p0, tt, td1v, td2v, td3v, pp);
  int p1 = _sv0t8;
  if ((p1 < 0)) {
    int _sv0t9 = (0 - 1);
    return _sv0t9;
  } else {
  }
  int has_trait = 0;
  int p2 = p1;
  int _sv0t10 = ps_peek(tags, p1);
  if ((_sv0t10 == 66)) {
    has_trait = 1;
    int _sv0t11 = sv0_vec_new();
    int tt2 = _sv0t11;
    int _sv0t12 = sv0_vec_new();
    int td1b = _sv0t12;
    int _sv0t13 = sv0_vec_new();
    int td2b = _sv0t13;
    int _sv0t14 = sv0_vec_new();
    int td3b = _sv0t14;
    int _sv0t15 = (p1 + 1);
    int _sv0t16 = parse_type(tags, starts, ends, source, _sv0t15, tt2, td1b, td2b, td3b, pp);
    int p3 = _sv0t16;
    if ((p3 < 0)) {
      int _sv0t17 = (0 - 1);
      return _sv0t17;
    } else {
    }
    p2 = p3;
  } else {
  }
  int _sv0t18 = ps_skip_where_clause(tags, p2);
  int p4 = _sv0t18;
  int _sv0t19 = ps_peek(tags, p4);
  if ((_sv0t19 != 8)) {
    int _sv0t20 = (0 - 1);
    return _sv0t20;
  } else {
  }
  int p = (p4 + 1);
  int items_count = 0;
  int done = 0;
  while ((done != 1)) {
    int _sv0t21 = ps_peek(tags, p);
    if ((_sv0t21 == 9)) {
      done = 1;
    } else {
      int _sv0t22 = parse_fn_body(tags, starts, ends, source, p, it, id1, id2, id3, id4, id5, pp, fn_param_name_toks, fn_param_ty_root, fn_ret_ty_root_by_item, fn_contract_base_by_item, fn_contract_root, enum_variant_payload_base_by_item, enum_variant_payload_count_by_item, pty_tt, pty_td1, pty_td2, pty_td3, body_et, body_ed1, body_ed2, body_ed3, body_ed4, body_sf);
      int pm = _sv0t22;
      if ((pm < 0)) {
        int _sv0t23 = (0 - 1);
        return _sv0t23;
      } else {
      }
      items_count = (items_count + 1);
      p = pm;
    }
  }
  sv0_vec_push(it, 4);
  sv0_vec_push(id1, has_trait);
  sv0_vec_push(id2, items_count);
  sv0_vec_push(id3, 0);
  int _sv0t24 = (0 - 1);
  sv0_vec_push(id4, _sv0t24);
  int _sv0t25 = (0 - 1);
  sv0_vec_push(id5, _sv0t25);
  int _sv0t26 = (0 - 1);
  sv0_vec_push(fn_ret_ty_root_by_item, _sv0t26);
  int _sv0t27 = (0 - 1);
  sv0_vec_push(fn_contract_base_by_item, _sv0t27);
  int _sv0t28 = (0 - 1);
  sv0_vec_push(enum_variant_payload_base_by_item, _sv0t28);
  sv0_vec_push(enum_variant_payload_count_by_item, 0);
  int _sv0t29 = (p + 1);
  return _sv0t29;
}

static int parse_item(int tags, int starts, int ends, const char* source, int pos, int it, int id1, int id2, int id3, int id4, int id5, int pp, int enum_variant_name_toks, int struct_field_name_toks, int fn_param_name_toks, int fn_param_ty_root, int fn_ret_ty_root_by_item, int fn_contract_base_by_item, int fn_contract_root, int enum_variant_payload_ty_root, int enum_variant_payload_base_by_item, int enum_variant_payload_count_by_item, int pty_tt, int pty_td1, int pty_td2, int pty_td3, int struct_field_ty_root, int body_et, int body_ed1, int body_ed2, int body_ed3, int body_ed4, int body_sf) {
  int _sv0t0 = ps_skip_attributes(tags, pos);
  int _sv0t1 = ps_skip_optional_pub(tags, _sv0t0);
  int p0 = _sv0t1;
  int _sv0t2 = ps_peek(tags, p0);
  int t = _sv0t2;
  if ((t == 75)) {
    int _sv0t3 = sv0_vec_len(pp);
    int pps = _sv0t3;
    int _sv0t4 = parse_module_decl(tags, p0, pp);
    int pm = _sv0t4;
    if ((pm == p0)) {
      int _sv0t5 = (0 - 1);
      return _sv0t5;
    } else {
    }
    int _sv0t6 = sv0_vec_len(pp);
    int ppc = (_sv0t6 - pps);
    sv0_vec_push(it, 6);
    sv0_vec_push(id1, pps);
    sv0_vec_push(id2, ppc);
    sv0_vec_push(id3, 0);
    int _sv0t7 = (0 - 1);
    sv0_vec_push(id4, _sv0t7);
    int _sv0t8 = (0 - 1);
    sv0_vec_push(id5, _sv0t8);
    int _sv0t9 = (0 - 1);
    sv0_vec_push(fn_ret_ty_root_by_item, _sv0t9);
    int _sv0t10 = (0 - 1);
    sv0_vec_push(fn_contract_base_by_item, _sv0t10);
    int _sv0t11 = (0 - 1);
    sv0_vec_push(enum_variant_payload_base_by_item, _sv0t11);
    sv0_vec_push(enum_variant_payload_count_by_item, 0);
    return pm;
  } else {
  }
  if ((t == 94)) {
    int _sv0t12 = sv0_vec_len(pp);
    int pps = _sv0t12;
    int _sv0t13 = parse_use_item(tags, p0, pp);
    int pu = _sv0t13;
    if ((pu < 0)) {
      int _sv0t14 = (0 - 1);
      return _sv0t14;
    } else {
    }
    int _sv0t15 = sv0_vec_len(pp);
    int ppc = (_sv0t15 - pps);
    sv0_vec_push(it, 5);
    sv0_vec_push(id1, pps);
    sv0_vec_push(id2, ppc);
    sv0_vec_push(id3, 0);
    int _sv0t16 = (0 - 1);
    sv0_vec_push(id4, _sv0t16);
    int _sv0t17 = (0 - 1);
    sv0_vec_push(id5, _sv0t17);
    int _sv0t18 = (0 - 1);
    sv0_vec_push(fn_ret_ty_root_by_item, _sv0t18);
    int _sv0t19 = (0 - 1);
    sv0_vec_push(fn_contract_base_by_item, _sv0t19);
    int _sv0t20 = (0 - 1);
    sv0_vec_push(enum_variant_payload_base_by_item, _sv0t20);
    sv0_vec_push(enum_variant_payload_count_by_item, 0);
    return pu;
  } else {
  }
  if ((t == 65)) {
    int _sv0t21 = parse_fn_body(tags, starts, ends, source, p0, it, id1, id2, id3, id4, id5, pp, fn_param_name_toks, fn_param_ty_root, fn_ret_ty_root_by_item, fn_contract_base_by_item, fn_contract_root, enum_variant_payload_base_by_item, enum_variant_payload_count_by_item, pty_tt, pty_td1, pty_td2, pty_td3, body_et, body_ed1, body_ed2, body_ed3, body_ed4, body_sf);
    return _sv0t21;
  } else {
  }
  if ((t == 93)) {
    int _sv0t22 = parse_fn_body(tags, starts, ends, source, p0, it, id1, id2, id3, id4, id5, pp, fn_param_name_toks, fn_param_ty_root, fn_ret_ty_root_by_item, fn_contract_base_by_item, fn_contract_root, enum_variant_payload_base_by_item, enum_variant_payload_count_by_item, pty_tt, pty_td1, pty_td2, pty_td3, body_et, body_ed1, body_ed2, body_ed3, body_ed4, body_sf);
    return _sv0t22;
  } else {
  }
  if ((t == 89)) {
    int _sv0t23 = parse_struct_item(tags, starts, ends, source, p0, it, id1, id2, id3, id4, id5, pp, struct_field_name_toks, fn_ret_ty_root_by_item, fn_contract_base_by_item, enum_variant_payload_base_by_item, enum_variant_payload_count_by_item, pty_tt, pty_td1, pty_td2, pty_td3, struct_field_ty_root);
    return _sv0t23;
  } else {
  }
  if ((t == 62)) {
    int _sv0t24 = parse_enum_item(tags, starts, ends, source, p0, it, id1, id2, id3, id4, id5, pp, enum_variant_name_toks, fn_ret_ty_root_by_item, fn_contract_base_by_item, enum_variant_payload_ty_root, enum_variant_payload_base_by_item, enum_variant_payload_count_by_item, pty_tt, pty_td1, pty_td2, pty_td3);
    return _sv0t24;
  } else {
  }
  if ((t == 90)) {
    int _sv0t25 = parse_trait_item(tags, starts, ends, source, p0, it, id1, id2, id3, id4, id5, pp, fn_param_name_toks, fn_param_ty_root, fn_ret_ty_root_by_item, fn_contract_base_by_item, fn_contract_root, enum_variant_payload_base_by_item, enum_variant_payload_count_by_item, pty_tt, pty_td1, pty_td2, pty_td3, body_et, body_ed1, body_ed2, body_ed3, body_ed4, body_sf);
    return _sv0t25;
  } else {
  }
  if ((t == 69)) {
    int _sv0t26 = parse_impl_item(tags, starts, ends, source, p0, it, id1, id2, id3, id4, id5, pp, fn_param_name_toks, fn_param_ty_root, fn_ret_ty_root_by_item, fn_contract_base_by_item, fn_contract_root, enum_variant_payload_base_by_item, enum_variant_payload_count_by_item, pty_tt, pty_td1, pty_td2, pty_td3, body_et, body_ed1, body_ed2, body_ed3, body_ed4, body_sf);
    return _sv0t26;
  } else {
  }
  if ((t == 92)) {
    int _sv0t27 = parse_type_alias_item(tags, starts, ends, source, p0, it, id1, id2, id3, id4, id5, pp, fn_ret_ty_root_by_item, fn_contract_base_by_item, enum_variant_payload_base_by_item, enum_variant_payload_count_by_item, pty_tt, pty_td1, pty_td2, pty_td3);
    return _sv0t27;
  } else {
  }
  int _sv0t28 = (0 - 1);
  return _sv0t28;
}

static int parse_program(int tags, int starts, int ends, const char* source, int pos, int it, int id1, int id2, int id3, int id4, int id5, int pp, int enum_variant_name_toks, int struct_field_name_toks, int fn_param_name_toks, int fn_param_ty_root, int fn_ret_ty_root_by_item, int fn_contract_base_by_item, int fn_contract_root, int enum_variant_payload_ty_root, int enum_variant_payload_base_by_item, int enum_variant_payload_count_by_item, int pty_tt, int pty_td1, int pty_td2, int pty_td3, int struct_field_ty_root, int body_et, int body_ed1, int body_ed2, int body_ed3, int body_ed4, int body_sf) {
  int p = pos;
  int done = 0;
  while ((done != 1)) {
    int _sv0t0 = ps_skip_attributes(tags, p);
    int p0 = _sv0t0;
    int _sv0t1 = ps_peek(tags, p0);
    if ((_sv0t1 == 97)) {
      done = 1;
    } else {
      int _sv0t2 = parse_item(tags, starts, ends, source, p0, it, id1, id2, id3, id4, id5, pp, enum_variant_name_toks, struct_field_name_toks, fn_param_name_toks, fn_param_ty_root, fn_ret_ty_root_by_item, fn_contract_base_by_item, fn_contract_root, enum_variant_payload_ty_root, enum_variant_payload_base_by_item, enum_variant_payload_count_by_item, pty_tt, pty_td1, pty_td2, pty_td3, struct_field_ty_root, body_et, body_ed1, body_ed2, body_ed3, body_ed4, body_sf);
      int pi = _sv0t2;
      if ((pi < 0)) {
        int _sv0t3 = (0 - 1);
        return _sv0t3;
      } else {
      }
      p = pi;
    }
  }
  return p;
}

static int item_fn_has_ret_packed(int item_id2) {
  int m = (item_id2 % 2);
  return m;
}

static int item_fn_contract_count_packed(int item_id2) {
  int _sv0t0 = (item_id2 / 2);
  return _sv0t0;
}

static int test_item_fn_id2_pack_roundtrip(void) {
  int _sv0t0 = item_fn_has_ret_packed(7);
  int x = _sv0t0;
  if ((x != 1)) {
    return 1;
  } else {
  }
  int _sv0t1 = item_fn_contract_count_packed(7);
  int c = _sv0t1;
  if ((c != 3)) {
    return 2;
  } else {
  }
  int _sv0t2 = item_fn_has_ret_packed(6);
  int x2 = _sv0t2;
  if ((x2 != 0)) {
    return 3;
  } else {
  }
  int _sv0t3 = item_fn_contract_count_packed(6);
  int c2 = _sv0t3;
  if ((c2 != 3)) {
    return 4;
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

static int test_parse_block_empty(void) {
  int _sv0t0 = sv0_vec_new();
  int tags = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int starts = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int ends = _sv0t2;
  sv0_vec_push(tags, 8);
  sv0_vec_push(starts, 0);
  sv0_vec_push(ends, 1);
  sv0_vec_push(tags, 9);
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
  int _sv0t9 = parse_expr(tags, starts, ends, "{}", 0, et, ed1, ed2, ed3, ed4, pp, 1);
  int p = _sv0t9;
  if ((p != 2)) {
    return 1;
  } else {
  }
  int _sv0t10 = sv0_vec_len(et);
  if ((_sv0t10 != 1)) {
    return 2;
  } else {
  }
  int _sv0t11 = sv0_vec_get(et, 0);
  if ((_sv0t11 != 9)) {
    return 3;
  } else {
  }
  int _sv0t12 = sv0_vec_get(ed2, 0);
  if ((_sv0t12 != 0)) {
    return 4;
  } else {
  }
  int _sv0t13 = sv0_vec_get(ed3, 0);
  int _sv0t14 = (0 - 1);
  if ((_sv0t13 != _sv0t14)) {
    return 5;
  } else {
  }
  return 0;
}

static int test_parse_block_tail(void) {
  int _sv0t0 = sv0_vec_new();
  int tags = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int starts = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int ends = _sv0t2;
  sv0_vec_push(tags, 8);
  sv0_vec_push(starts, 0);
  sv0_vec_push(ends, 1);
  sv0_vec_push(tags, 0);
  sv0_vec_push(starts, 1);
  sv0_vec_push(ends, 3);
  sv0_vec_push(tags, 9);
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
  int _sv0t9 = parse_expr(tags, starts, ends, "{ 42 }", 0, et, ed1, ed2, ed3, ed4, pp, 1);
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
  if ((_sv0t12 != 9)) {
    return 4;
  } else {
  }
  int _sv0t13 = sv0_vec_get(ed2, 1);
  if ((_sv0t13 != 0)) {
    return 5;
  } else {
  }
  int _sv0t14 = sv0_vec_get(ed3, 1);
  if ((_sv0t14 != 0)) {
    return 6;
  } else {
  }
  return 0;
}

static int test_parse_block_semi(void) {
  int _sv0t0 = sv0_vec_new();
  int tags = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int starts = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int ends = _sv0t2;
  sv0_vec_push(tags, 8);
  sv0_vec_push(starts, 0);
  sv0_vec_push(ends, 1);
  sv0_vec_push(tags, 0);
  sv0_vec_push(starts, 1);
  sv0_vec_push(ends, 3);
  sv0_vec_push(tags, 13);
  sv0_vec_push(starts, 3);
  sv0_vec_push(ends, 4);
  sv0_vec_push(tags, 9);
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
  int _sv0t9 = parse_expr(tags, starts, ends, "{ 42; }", 0, et, ed1, ed2, ed3, ed4, pp, 1);
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
  if ((_sv0t11 != 0)) {
    return 3;
  } else {
  }
  int _sv0t12 = sv0_vec_get(et, 1);
  if ((_sv0t12 != 28)) {
    return 4;
  } else {
  }
  int _sv0t13 = sv0_vec_get(ed1, 1);
  if ((_sv0t13 != 0)) {
    return 5;
  } else {
  }
  int _sv0t14 = sv0_vec_get(et, 2);
  if ((_sv0t14 != 9)) {
    return 6;
  } else {
  }
  int _sv0t15 = sv0_vec_get(ed2, 2);
  if ((_sv0t15 != 1)) {
    return 7;
  } else {
  }
  int _sv0t16 = sv0_vec_get(ed3, 2);
  int _sv0t17 = (0 - 1);
  if ((_sv0t16 != _sv0t17)) {
    return 8;
  } else {
  }
  return 0;
}

static int test_parse_block_let(void) {
  int _sv0t0 = sv0_vec_new();
  int tags = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int starts = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int ends = _sv0t2;
  sv0_vec_push(tags, 8);
  sv0_vec_push(starts, 0);
  sv0_vec_push(ends, 1);
  sv0_vec_push(tags, 71);
  sv0_vec_push(starts, 1);
  sv0_vec_push(ends, 4);
  sv0_vec_push(tags, 5);
  sv0_vec_push(starts, 5);
  sv0_vec_push(ends, 6);
  sv0_vec_push(tags, 14);
  sv0_vec_push(starts, 6);
  sv0_vec_push(ends, 7);
  sv0_vec_push(tags, 5);
  sv0_vec_push(starts, 8);
  sv0_vec_push(ends, 11);
  sv0_vec_push(tags, 35);
  sv0_vec_push(starts, 12);
  sv0_vec_push(ends, 13);
  sv0_vec_push(tags, 0);
  sv0_vec_push(starts, 14);
  sv0_vec_push(ends, 15);
  sv0_vec_push(tags, 13);
  sv0_vec_push(starts, 15);
  sv0_vec_push(ends, 16);
  sv0_vec_push(tags, 5);
  sv0_vec_push(starts, 17);
  sv0_vec_push(ends, 18);
  sv0_vec_push(tags, 9);
  sv0_vec_push(starts, 18);
  sv0_vec_push(ends, 19);
  sv0_vec_push(tags, 97);
  sv0_vec_push(starts, 19);
  sv0_vec_push(ends, 19);
  int _sv0t3 = sv0_vec_new();
  int et = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int ed1v = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int ed2v = _sv0t5;
  int _sv0t6 = sv0_vec_new();
  int ed3v = _sv0t6;
  int _sv0t7 = sv0_vec_new();
  int ed4v = _sv0t7;
  int _sv0t8 = sv0_vec_new();
  int pp = _sv0t8;
  int _sv0t9 = parse_expr(tags, starts, ends, "{ let x: i32 = 1; x }", 0, et, ed1v, ed2v, ed3v, ed4v, pp, 1);
  int p = _sv0t9;
  if ((p != 10)) {
    return 1;
  } else {
  }
  int _sv0t10 = sv0_vec_get(et, 0);
  if ((_sv0t10 != 0)) {
    return 2;
  } else {
  }
  int _sv0t11 = sv0_vec_get(et, 1);
  if ((_sv0t11 != 27)) {
    return 3;
  } else {
  }
  int _sv0t12 = sv0_vec_get(ed1v, 1);
  if ((_sv0t12 != 2)) {
    return 4;
  } else {
  }
  int _sv0t13 = sv0_vec_get(ed2v, 1);
  if ((_sv0t13 != 0)) {
    return 5;
  } else {
  }
  int _sv0t14 = sv0_vec_get(ed3v, 1);
  if ((_sv0t14 != 0)) {
    return 6;
  } else {
  }
  int _sv0t15 = sv0_vec_get(ed4v, 1);
  if ((_sv0t15 != 0)) {
    return 7;
  } else {
  }
  int _sv0t16 = sv0_vec_get(et, 2);
  if ((_sv0t16 != 1)) {
    return 8;
  } else {
  }
  int _sv0t17 = sv0_vec_get(et, 3);
  if ((_sv0t17 != 9)) {
    return 9;
  } else {
  }
  int _sv0t18 = sv0_vec_get(ed2v, 3);
  if ((_sv0t18 != 1)) {
    return 10;
  } else {
  }
  int _sv0t19 = sv0_vec_get(ed3v, 3);
  if ((_sv0t19 != 2)) {
    return 11;
  } else {
  }
  return 0;
}

static int test_parse_if_simple(void) {
  int _sv0t0 = sv0_vec_new();
  int tags = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int starts = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int ends = _sv0t2;
  sv0_vec_push(tags, 68);
  sv0_vec_push(starts, 0);
  sv0_vec_push(ends, 2);
  sv0_vec_push(tags, 91);
  sv0_vec_push(starts, 3);
  sv0_vec_push(ends, 7);
  sv0_vec_push(tags, 8);
  sv0_vec_push(starts, 8);
  sv0_vec_push(ends, 9);
  sv0_vec_push(tags, 0);
  sv0_vec_push(starts, 10);
  sv0_vec_push(ends, 11);
  sv0_vec_push(tags, 9);
  sv0_vec_push(starts, 12);
  sv0_vec_push(ends, 13);
  sv0_vec_push(tags, 97);
  sv0_vec_push(starts, 13);
  sv0_vec_push(ends, 13);
  int _sv0t3 = sv0_vec_new();
  int et = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int ed1v = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int ed2v = _sv0t5;
  int _sv0t6 = sv0_vec_new();
  int ed3v = _sv0t6;
  int _sv0t7 = sv0_vec_new();
  int ed4v = _sv0t7;
  int _sv0t8 = sv0_vec_new();
  int pp = _sv0t8;
  int _sv0t9 = parse_expr(tags, starts, ends, "if true { 1 }", 0, et, ed1v, ed2v, ed3v, ed4v, pp, 1);
  int p = _sv0t9;
  if ((p != 5)) {
    return 1;
  } else {
  }
  int _sv0t10 = sv0_vec_get(et, 0);
  if ((_sv0t10 != 0)) {
    return 2;
  } else {
  }
  int _sv0t11 = sv0_vec_get(et, 1);
  if ((_sv0t11 != 0)) {
    return 3;
  } else {
  }
  int _sv0t12 = sv0_vec_get(et, 2);
  if ((_sv0t12 != 9)) {
    return 4;
  } else {
  }
  int _sv0t13 = sv0_vec_get(et, 3);
  if ((_sv0t13 != 10)) {
    return 5;
  } else {
  }
  int _sv0t14 = sv0_vec_get(ed1v, 3);
  if ((_sv0t14 != 0)) {
    return 6;
  } else {
  }
  int _sv0t15 = sv0_vec_get(ed2v, 3);
  if ((_sv0t15 != 2)) {
    return 7;
  } else {
  }
  int _sv0t16 = sv0_vec_get(ed3v, 3);
  int _sv0t17 = (0 - 1);
  if ((_sv0t16 != _sv0t17)) {
    return 8;
  } else {
  }
  return 0;
}

static int test_parse_if_else(void) {
  int _sv0t0 = sv0_vec_new();
  int tags = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int starts = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int ends = _sv0t2;
  sv0_vec_push(tags, 68);
  sv0_vec_push(starts, 0);
  sv0_vec_push(ends, 2);
  sv0_vec_push(tags, 91);
  sv0_vec_push(starts, 3);
  sv0_vec_push(ends, 7);
  sv0_vec_push(tags, 8);
  sv0_vec_push(starts, 8);
  sv0_vec_push(ends, 9);
  sv0_vec_push(tags, 0);
  sv0_vec_push(starts, 10);
  sv0_vec_push(ends, 11);
  sv0_vec_push(tags, 9);
  sv0_vec_push(starts, 12);
  sv0_vec_push(ends, 13);
  sv0_vec_push(tags, 60);
  sv0_vec_push(starts, 14);
  sv0_vec_push(ends, 18);
  sv0_vec_push(tags, 8);
  sv0_vec_push(starts, 19);
  sv0_vec_push(ends, 20);
  sv0_vec_push(tags, 0);
  sv0_vec_push(starts, 21);
  sv0_vec_push(ends, 22);
  sv0_vec_push(tags, 9);
  sv0_vec_push(starts, 23);
  sv0_vec_push(ends, 24);
  sv0_vec_push(tags, 97);
  sv0_vec_push(starts, 24);
  sv0_vec_push(ends, 24);
  int _sv0t3 = sv0_vec_new();
  int et = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int ed1v = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int ed2v = _sv0t5;
  int _sv0t6 = sv0_vec_new();
  int ed3v = _sv0t6;
  int _sv0t7 = sv0_vec_new();
  int ed4v = _sv0t7;
  int _sv0t8 = sv0_vec_new();
  int pp = _sv0t8;
  int _sv0t9 = parse_expr(tags, starts, ends, "if true { 1 } else { 2 }", 0, et, ed1v, ed2v, ed3v, ed4v, pp, 1);
  int p = _sv0t9;
  if ((p != 9)) {
    return 1;
  } else {
  }
  int _sv0t10 = sv0_vec_get(et, 0);
  if ((_sv0t10 != 0)) {
    return 2;
  } else {
  }
  int _sv0t11 = sv0_vec_get(et, 1);
  if ((_sv0t11 != 0)) {
    return 3;
  } else {
  }
  int _sv0t12 = sv0_vec_get(et, 2);
  if ((_sv0t12 != 9)) {
    return 4;
  } else {
  }
  int _sv0t13 = sv0_vec_get(et, 3);
  if ((_sv0t13 != 0)) {
    return 5;
  } else {
  }
  int _sv0t14 = sv0_vec_get(et, 4);
  if ((_sv0t14 != 9)) {
    return 6;
  } else {
  }
  int _sv0t15 = sv0_vec_get(et, 5);
  if ((_sv0t15 != 10)) {
    return 7;
  } else {
  }
  int _sv0t16 = sv0_vec_get(ed1v, 5);
  if ((_sv0t16 != 0)) {
    return 8;
  } else {
  }
  int _sv0t17 = sv0_vec_get(ed2v, 5);
  if ((_sv0t17 != 2)) {
    return 9;
  } else {
  }
  int _sv0t18 = sv0_vec_get(ed3v, 5);
  if ((_sv0t18 != 4)) {
    return 10;
  } else {
  }
  return 0;
}

static int test_parse_while(void) {
  int _sv0t0 = sv0_vec_new();
  int tags = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int starts = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int ends = _sv0t2;
  sv0_vec_push(tags, 96);
  sv0_vec_push(starts, 0);
  sv0_vec_push(ends, 5);
  sv0_vec_push(tags, 91);
  sv0_vec_push(starts, 6);
  sv0_vec_push(ends, 10);
  sv0_vec_push(tags, 8);
  sv0_vec_push(starts, 11);
  sv0_vec_push(ends, 12);
  sv0_vec_push(tags, 0);
  sv0_vec_push(starts, 13);
  sv0_vec_push(ends, 14);
  sv0_vec_push(tags, 13);
  sv0_vec_push(starts, 14);
  sv0_vec_push(ends, 15);
  sv0_vec_push(tags, 9);
  sv0_vec_push(starts, 16);
  sv0_vec_push(ends, 17);
  sv0_vec_push(tags, 97);
  sv0_vec_push(starts, 17);
  sv0_vec_push(ends, 17);
  int _sv0t3 = sv0_vec_new();
  int et = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int ed1v = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int ed2v = _sv0t5;
  int _sv0t6 = sv0_vec_new();
  int ed3v = _sv0t6;
  int _sv0t7 = sv0_vec_new();
  int ed4v = _sv0t7;
  int _sv0t8 = sv0_vec_new();
  int pp = _sv0t8;
  int _sv0t9 = parse_expr(tags, starts, ends, "while true { 1; }", 0, et, ed1v, ed2v, ed3v, ed4v, pp, 1);
  int p = _sv0t9;
  if ((p != 6)) {
    return 1;
  } else {
  }
  int _sv0t10 = sv0_vec_get(et, 0);
  if ((_sv0t10 != 0)) {
    return 2;
  } else {
  }
  int _sv0t11 = sv0_vec_get(et, 1);
  if ((_sv0t11 != 0)) {
    return 3;
  } else {
  }
  int _sv0t12 = sv0_vec_get(et, 2);
  if ((_sv0t12 != 28)) {
    return 4;
  } else {
  }
  int _sv0t13 = sv0_vec_get(et, 3);
  if ((_sv0t13 != 9)) {
    return 5;
  } else {
  }
  int _sv0t14 = sv0_vec_get(et, 4);
  if ((_sv0t14 != 12)) {
    return 6;
  } else {
  }
  int _sv0t15 = sv0_vec_get(ed1v, 4);
  if ((_sv0t15 != 0)) {
    return 7;
  } else {
  }
  int _sv0t16 = sv0_vec_get(ed2v, 4);
  if ((_sv0t16 != 3)) {
    return 8;
  } else {
  }
  return 0;
}

static int test_parse_loop(void) {
  int _sv0t0 = sv0_vec_new();
  int tags = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int starts = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int ends = _sv0t2;
  sv0_vec_push(tags, 72);
  sv0_vec_push(starts, 0);
  sv0_vec_push(ends, 4);
  sv0_vec_push(tags, 8);
  sv0_vec_push(starts, 5);
  sv0_vec_push(ends, 6);
  sv0_vec_push(tags, 0);
  sv0_vec_push(starts, 7);
  sv0_vec_push(ends, 8);
  sv0_vec_push(tags, 13);
  sv0_vec_push(starts, 8);
  sv0_vec_push(ends, 9);
  sv0_vec_push(tags, 9);
  sv0_vec_push(starts, 10);
  sv0_vec_push(ends, 11);
  sv0_vec_push(tags, 97);
  sv0_vec_push(starts, 11);
  sv0_vec_push(ends, 11);
  int _sv0t3 = sv0_vec_new();
  int et = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int ed1v = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int ed2v = _sv0t5;
  int _sv0t6 = sv0_vec_new();
  int ed3v = _sv0t6;
  int _sv0t7 = sv0_vec_new();
  int ed4v = _sv0t7;
  int _sv0t8 = sv0_vec_new();
  int pp = _sv0t8;
  int _sv0t9 = parse_expr(tags, starts, ends, "loop { 1; }", 0, et, ed1v, ed2v, ed3v, ed4v, pp, 1);
  int p = _sv0t9;
  if ((p != 5)) {
    return 1;
  } else {
  }
  int _sv0t10 = sv0_vec_get(et, 0);
  if ((_sv0t10 != 0)) {
    return 2;
  } else {
  }
  int _sv0t11 = sv0_vec_get(et, 1);
  if ((_sv0t11 != 28)) {
    return 3;
  } else {
  }
  int _sv0t12 = sv0_vec_get(et, 2);
  if ((_sv0t12 != 9)) {
    return 4;
  } else {
  }
  int _sv0t13 = sv0_vec_get(et, 3);
  if ((_sv0t13 != 14)) {
    return 5;
  } else {
  }
  int _sv0t14 = sv0_vec_get(ed1v, 3);
  if ((_sv0t14 != 2)) {
    return 6;
  } else {
  }
  return 0;
}

static int test_parse_return(void) {
  int _sv0t0 = sv0_vec_new();
  int tags = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int starts = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int ends = _sv0t2;
  sv0_vec_push(tags, 85);
  sv0_vec_push(starts, 0);
  sv0_vec_push(ends, 6);
  sv0_vec_push(tags, 0);
  sv0_vec_push(starts, 7);
  sv0_vec_push(ends, 9);
  sv0_vec_push(tags, 97);
  sv0_vec_push(starts, 9);
  sv0_vec_push(ends, 9);
  int _sv0t3 = sv0_vec_new();
  int et = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int ed1v = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int ed2v = _sv0t5;
  int _sv0t6 = sv0_vec_new();
  int ed3v = _sv0t6;
  int _sv0t7 = sv0_vec_new();
  int ed4v = _sv0t7;
  int _sv0t8 = sv0_vec_new();
  int pp = _sv0t8;
  int _sv0t9 = parse_expr(tags, starts, ends, "return 42", 0, et, ed1v, ed2v, ed3v, ed4v, pp, 1);
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
  if ((_sv0t12 != 15)) {
    return 4;
  } else {
  }
  int _sv0t13 = sv0_vec_get(ed1v, 1);
  if ((_sv0t13 != 0)) {
    return 5;
  } else {
  }
  int _sv0t14 = sv0_vec_new();
  int tags2 = _sv0t14;
  int _sv0t15 = sv0_vec_new();
  int starts2 = _sv0t15;
  int _sv0t16 = sv0_vec_new();
  int ends2 = _sv0t16;
  sv0_vec_push(tags2, 85);
  sv0_vec_push(starts2, 0);
  sv0_vec_push(ends2, 6);
  sv0_vec_push(tags2, 13);
  sv0_vec_push(starts2, 6);
  sv0_vec_push(ends2, 7);
  sv0_vec_push(tags2, 97);
  sv0_vec_push(starts2, 7);
  sv0_vec_push(ends2, 7);
  int _sv0t17 = sv0_vec_new();
  int et2 = _sv0t17;
  int _sv0t18 = sv0_vec_new();
  int ed1b = _sv0t18;
  int _sv0t19 = sv0_vec_new();
  int ed2b = _sv0t19;
  int _sv0t20 = sv0_vec_new();
  int ed3b = _sv0t20;
  int _sv0t21 = sv0_vec_new();
  int ed4b = _sv0t21;
  int _sv0t22 = sv0_vec_new();
  int pp2 = _sv0t22;
  int _sv0t23 = parse_expr(tags2, starts2, ends2, "return;", 0, et2, ed1b, ed2b, ed3b, ed4b, pp2, 1);
  int p2 = _sv0t23;
  if ((p2 != 1)) {
    return 6;
  } else {
  }
  int _sv0t24 = sv0_vec_get(et2, 0);
  if ((_sv0t24 != 15)) {
    return 7;
  } else {
  }
  int _sv0t25 = sv0_vec_get(ed1b, 0);
  int _sv0t26 = (0 - 1);
  if ((_sv0t25 != _sv0t26)) {
    return 8;
  } else {
  }
  return 0;
}

static int test_parse_assign(void) {
  int _sv0t0 = sv0_vec_new();
  int tags = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int starts = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int ends = _sv0t2;
  sv0_vec_push(tags, 8);
  sv0_vec_push(starts, 0);
  sv0_vec_push(ends, 1);
  sv0_vec_push(tags, 5);
  sv0_vec_push(starts, 2);
  sv0_vec_push(ends, 3);
  sv0_vec_push(tags, 35);
  sv0_vec_push(starts, 4);
  sv0_vec_push(ends, 5);
  sv0_vec_push(tags, 0);
  sv0_vec_push(starts, 6);
  sv0_vec_push(ends, 7);
  sv0_vec_push(tags, 13);
  sv0_vec_push(starts, 7);
  sv0_vec_push(ends, 8);
  sv0_vec_push(tags, 9);
  sv0_vec_push(starts, 9);
  sv0_vec_push(ends, 10);
  sv0_vec_push(tags, 97);
  sv0_vec_push(starts, 10);
  sv0_vec_push(ends, 10);
  int _sv0t3 = sv0_vec_new();
  int et = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int ed1v = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int ed2v = _sv0t5;
  int _sv0t6 = sv0_vec_new();
  int ed3v = _sv0t6;
  int _sv0t7 = sv0_vec_new();
  int ed4v = _sv0t7;
  int _sv0t8 = sv0_vec_new();
  int pp = _sv0t8;
  int _sv0t9 = parse_expr(tags, starts, ends, "{ x = 1; }", 0, et, ed1v, ed2v, ed3v, ed4v, pp, 1);
  int p = _sv0t9;
  if ((p != 6)) {
    return 1;
  } else {
  }
  int _sv0t10 = sv0_vec_get(et, 0);
  if ((_sv0t10 != 1)) {
    return 2;
  } else {
  }
  int _sv0t11 = sv0_vec_get(et, 1);
  if ((_sv0t11 != 0)) {
    return 3;
  } else {
  }
  int _sv0t12 = sv0_vec_get(et, 2);
  if ((_sv0t12 != 18)) {
    return 4;
  } else {
  }
  int _sv0t13 = sv0_vec_get(ed1v, 2);
  if ((_sv0t13 != 0)) {
    return 5;
  } else {
  }
  int _sv0t14 = sv0_vec_get(ed2v, 2);
  if ((_sv0t14 != 1)) {
    return 6;
  } else {
  }
  int _sv0t15 = sv0_vec_get(et, 3);
  if ((_sv0t15 != 28)) {
    return 7;
  } else {
  }
  int _sv0t16 = sv0_vec_get(et, 4);
  if ((_sv0t16 != 9)) {
    return 8;
  } else {
  }
  int _sv0t17 = sv0_vec_new();
  int tags2 = _sv0t17;
  int _sv0t18 = sv0_vec_new();
  int starts2 = _sv0t18;
  int _sv0t19 = sv0_vec_new();
  int ends2 = _sv0t19;
  sv0_vec_push(tags2, 8);
  sv0_vec_push(starts2, 0);
  sv0_vec_push(ends2, 1);
  sv0_vec_push(tags2, 5);
  sv0_vec_push(starts2, 2);
  sv0_vec_push(ends2, 3);
  sv0_vec_push(tags2, 15);
  sv0_vec_push(starts2, 3);
  sv0_vec_push(ends2, 4);
  sv0_vec_push(tags2, 5);
  sv0_vec_push(starts2, 4);
  sv0_vec_push(ends2, 5);
  sv0_vec_push(tags2, 35);
  sv0_vec_push(starts2, 6);
  sv0_vec_push(ends2, 7);
  sv0_vec_push(tags2, 0);
  sv0_vec_push(starts2, 8);
  sv0_vec_push(ends2, 9);
  sv0_vec_push(tags2, 13);
  sv0_vec_push(starts2, 9);
  sv0_vec_push(ends2, 10);
  sv0_vec_push(tags2, 9);
  sv0_vec_push(starts2, 11);
  sv0_vec_push(ends2, 12);
  sv0_vec_push(tags2, 97);
  sv0_vec_push(starts2, 12);
  sv0_vec_push(ends2, 12);
  int _sv0t20 = sv0_vec_new();
  int et2 = _sv0t20;
  int _sv0t21 = sv0_vec_new();
  int ed1b = _sv0t21;
  int _sv0t22 = sv0_vec_new();
  int ed2b = _sv0t22;
  int _sv0t23 = sv0_vec_new();
  int ed3b = _sv0t23;
  int _sv0t24 = sv0_vec_new();
  int ed4b = _sv0t24;
  int _sv0t25 = sv0_vec_new();
  int pp2 = _sv0t25;
  int _sv0t26 = parse_expr(tags2, starts2, ends2, "{ x.y = 1; }", 0, et2, ed1b, ed2b, ed3b, ed4b, pp2, 1);
  int p2 = _sv0t26;
  if ((p2 != 8)) {
    return 9;
  } else {
  }
  int _sv0t27 = sv0_vec_get(et2, 1);
  if ((_sv0t27 != 6)) {
    return 10;
  } else {
  }
  int _sv0t28 = sv0_vec_get(et2, 3);
  if ((_sv0t28 != 18)) {
    return 11;
  } else {
  }
  int _sv0t29 = sv0_vec_get(ed1b, 3);
  if ((_sv0t29 != 1)) {
    return 12;
  } else {
  }
  int _sv0t30 = sv0_vec_new();
  int tags3 = _sv0t30;
  int _sv0t31 = sv0_vec_new();
  int starts3 = _sv0t31;
  int _sv0t32 = sv0_vec_new();
  int ends3 = _sv0t32;
  sv0_vec_push(tags3, 8);
  sv0_vec_push(starts3, 0);
  sv0_vec_push(ends3, 1);
  sv0_vec_push(tags3, 5);
  sv0_vec_push(starts3, 2);
  sv0_vec_push(ends3, 3);
  sv0_vec_push(tags3, 15);
  sv0_vec_push(starts3, 3);
  sv0_vec_push(ends3, 4);
  sv0_vec_push(tags3, 5);
  sv0_vec_push(starts3, 4);
  sv0_vec_push(ends3, 5);
  sv0_vec_push(tags3, 44);
  sv0_vec_push(starts3, 6);
  sv0_vec_push(ends3, 8);
  sv0_vec_push(tags3, 0);
  sv0_vec_push(starts3, 8);
  sv0_vec_push(ends3, 9);
  sv0_vec_push(tags3, 13);
  sv0_vec_push(starts3, 9);
  sv0_vec_push(ends3, 10);
  sv0_vec_push(tags3, 9);
  sv0_vec_push(starts3, 11);
  sv0_vec_push(ends3, 12);
  sv0_vec_push(tags3, 97);
  sv0_vec_push(starts3, 12);
  sv0_vec_push(ends3, 12);
  int _sv0t33 = sv0_vec_new();
  int et3 = _sv0t33;
  int _sv0t34 = sv0_vec_new();
  int ed1c = _sv0t34;
  int _sv0t35 = sv0_vec_new();
  int ed2c = _sv0t35;
  int _sv0t36 = sv0_vec_new();
  int ed3c = _sv0t36;
  int _sv0t37 = sv0_vec_new();
  int ed4c = _sv0t37;
  int _sv0t38 = sv0_vec_new();
  int pp3 = _sv0t38;
  int _sv0t39 = parse_expr(tags3, starts3, ends3, "{ x.y += 1; }", 0, et3, ed1c, ed2c, ed3c, ed4c, pp3, 1);
  int p3 = _sv0t39;
  if ((p3 != 8)) {
    return 13;
  } else {
  }
  int _sv0t40 = sv0_vec_get(et3, 3);
  if ((_sv0t40 != 19)) {
    return 14;
  } else {
  }
  int _sv0t41 = sv0_vec_get(ed1c, 3);
  if ((_sv0t41 != 0)) {
    return 15;
  } else {
  }
  int _sv0t42 = sv0_vec_get(ed2c, 3);
  if ((_sv0t42 != 1)) {
    return 16;
  } else {
  }
  int _sv0t43 = sv0_vec_get(ed3c, 3);
  if ((_sv0t43 != 2)) {
    return 17;
  } else {
  }
  return 0;
}

static int test_parse_match_simple(void) {
  int _sv0t0 = sv0_vec_new();
  int tags = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int starts = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int ends = _sv0t2;
  sv0_vec_push(tags, 74);
  sv0_vec_push(starts, 0);
  sv0_vec_push(ends, 5);
  sv0_vec_push(tags, 5);
  sv0_vec_push(starts, 6);
  sv0_vec_push(ends, 7);
  sv0_vec_push(tags, 8);
  sv0_vec_push(starts, 8);
  sv0_vec_push(ends, 9);
  sv0_vec_push(tags, 0);
  sv0_vec_push(starts, 10);
  sv0_vec_push(ends, 11);
  sv0_vec_push(tags, 20);
  sv0_vec_push(starts, 12);
  sv0_vec_push(ends, 14);
  sv0_vec_push(tags, 0);
  sv0_vec_push(starts, 15);
  sv0_vec_push(ends, 16);
  sv0_vec_push(tags, 9);
  sv0_vec_push(starts, 17);
  sv0_vec_push(ends, 18);
  sv0_vec_push(tags, 97);
  sv0_vec_push(starts, 18);
  sv0_vec_push(ends, 18);
  int _sv0t3 = sv0_vec_new();
  int et = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int ed1v = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int ed2v = _sv0t5;
  int _sv0t6 = sv0_vec_new();
  int ed3v = _sv0t6;
  int _sv0t7 = sv0_vec_new();
  int ed4v = _sv0t7;
  int _sv0t8 = sv0_vec_new();
  int pp = _sv0t8;
  int _sv0t9 = parse_expr(tags, starts, ends, "match x { 1 => 2 }", 0, et, ed1v, ed2v, ed3v, ed4v, pp, 1);
  int p = _sv0t9;
  if ((p != 7)) {
    return 1;
  } else {
  }
  int _sv0t10 = sv0_vec_get(et, 0);
  if ((_sv0t10 != 1)) {
    return 2;
  } else {
  }
  int _sv0t11 = sv0_vec_get(et, 1);
  if ((_sv0t11 != 0)) {
    return 3;
  } else {
  }
  int _sv0t12 = sv0_vec_get(et, 2);
  if ((_sv0t12 != 30)) {
    return 4;
  } else {
  }
  int _sv0t13 = sv0_vec_get(ed4v, 2);
  if ((_sv0t13 != 2)) {
    return 11;
  } else {
  }
  int _sv0t14 = sv0_vec_get(et, 3);
  if ((_sv0t14 != 29)) {
    return 12;
  } else {
  }
  int _sv0t15 = sv0_vec_get(ed1v, 3);
  if ((_sv0t15 != 2)) {
    return 5;
  } else {
  }
  int _sv0t16 = sv0_vec_get(ed2v, 3);
  if ((_sv0t16 != 2)) {
    return 13;
  } else {
  }
  int _sv0t17 = sv0_vec_get(ed3v, 3);
  int _sv0t18 = (0 - 1);
  if ((_sv0t17 != _sv0t18)) {
    return 6;
  } else {
  }
  int _sv0t19 = sv0_vec_get(ed4v, 3);
  if ((_sv0t19 != 1)) {
    return 7;
  } else {
  }
  int _sv0t20 = sv0_vec_get(et, 4);
  if ((_sv0t20 != 11)) {
    return 8;
  } else {
  }
  int _sv0t21 = sv0_vec_get(ed1v, 4);
  if ((_sv0t21 != 0)) {
    return 9;
  } else {
  }
  int _sv0t22 = sv0_vec_get(ed3v, 4);
  if ((_sv0t22 != 1)) {
    return 10;
  } else {
  }
  return 0;
}

static int test_parse_match_multi(void) {
  int _sv0t0 = sv0_vec_new();
  int tags = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int starts = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int ends = _sv0t2;
  sv0_vec_push(tags, 74);
  sv0_vec_push(starts, 0);
  sv0_vec_push(ends, 5);
  sv0_vec_push(tags, 5);
  sv0_vec_push(starts, 6);
  sv0_vec_push(ends, 7);
  sv0_vec_push(tags, 8);
  sv0_vec_push(starts, 8);
  sv0_vec_push(ends, 9);
  sv0_vec_push(tags, 0);
  sv0_vec_push(starts, 10);
  sv0_vec_push(ends, 11);
  sv0_vec_push(tags, 20);
  sv0_vec_push(starts, 12);
  sv0_vec_push(ends, 14);
  sv0_vec_push(tags, 0);
  sv0_vec_push(starts, 15);
  sv0_vec_push(ends, 16);
  sv0_vec_push(tags, 12);
  sv0_vec_push(starts, 16);
  sv0_vec_push(ends, 17);
  sv0_vec_push(tags, 0);
  sv0_vec_push(starts, 18);
  sv0_vec_push(ends, 19);
  sv0_vec_push(tags, 20);
  sv0_vec_push(starts, 20);
  sv0_vec_push(ends, 22);
  sv0_vec_push(tags, 0);
  sv0_vec_push(starts, 23);
  sv0_vec_push(ends, 24);
  sv0_vec_push(tags, 9);
  sv0_vec_push(starts, 25);
  sv0_vec_push(ends, 26);
  sv0_vec_push(tags, 97);
  sv0_vec_push(starts, 26);
  sv0_vec_push(ends, 26);
  int _sv0t3 = sv0_vec_new();
  int et = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int ed1v = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int ed2v = _sv0t5;
  int _sv0t6 = sv0_vec_new();
  int ed3v = _sv0t6;
  int _sv0t7 = sv0_vec_new();
  int ed4v = _sv0t7;
  int _sv0t8 = sv0_vec_new();
  int pp = _sv0t8;
  int _sv0t9 = parse_expr(tags, starts, ends, "match x { 1 => 2, 3 => 4 }", 0, et, ed1v, ed2v, ed3v, ed4v, pp, 1);
  int p = _sv0t9;
  if ((p != 11)) {
    return 1;
  } else {
  }
  int _sv0t10 = sv0_vec_get(et, 0);
  if ((_sv0t10 != 1)) {
    return 2;
  } else {
  }
  int _sv0t11 = sv0_vec_get(et, 1);
  if ((_sv0t11 != 0)) {
    return 3;
  } else {
  }
  int _sv0t12 = sv0_vec_get(et, 2);
  if ((_sv0t12 != 30)) {
    return 4;
  } else {
  }
  int _sv0t13 = sv0_vec_get(et, 3);
  if ((_sv0t13 != 29)) {
    return 14;
  } else {
  }
  int _sv0t14 = sv0_vec_get(et, 4);
  if ((_sv0t14 != 0)) {
    return 5;
  } else {
  }
  int _sv0t15 = sv0_vec_get(et, 5);
  if ((_sv0t15 != 30)) {
    return 15;
  } else {
  }
  int _sv0t16 = sv0_vec_get(et, 6);
  if ((_sv0t16 != 29)) {
    return 6;
  } else {
  }
  int _sv0t17 = sv0_vec_get(et, 7);
  if ((_sv0t17 != 11)) {
    return 7;
  } else {
  }
  int _sv0t18 = sv0_vec_get(ed3v, 7);
  if ((_sv0t18 != 2)) {
    return 8;
  } else {
  }
  return 0;
}

static int test_parse_match_guard(void) {
  int _sv0t0 = sv0_vec_new();
  int tags = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int starts = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int ends = _sv0t2;
  sv0_vec_push(tags, 74);
  sv0_vec_push(starts, 0);
  sv0_vec_push(ends, 5);
  sv0_vec_push(tags, 5);
  sv0_vec_push(starts, 6);
  sv0_vec_push(ends, 7);
  sv0_vec_push(tags, 8);
  sv0_vec_push(starts, 8);
  sv0_vec_push(ends, 9);
  sv0_vec_push(tags, 0);
  sv0_vec_push(starts, 10);
  sv0_vec_push(ends, 11);
  sv0_vec_push(tags, 68);
  sv0_vec_push(starts, 12);
  sv0_vec_push(ends, 14);
  sv0_vec_push(tags, 5);
  sv0_vec_push(starts, 15);
  sv0_vec_push(ends, 16);
  sv0_vec_push(tags, 20);
  sv0_vec_push(starts, 17);
  sv0_vec_push(ends, 19);
  sv0_vec_push(tags, 0);
  sv0_vec_push(starts, 20);
  sv0_vec_push(ends, 21);
  sv0_vec_push(tags, 9);
  sv0_vec_push(starts, 22);
  sv0_vec_push(ends, 23);
  sv0_vec_push(tags, 97);
  sv0_vec_push(starts, 23);
  sv0_vec_push(ends, 23);
  int _sv0t3 = sv0_vec_new();
  int et = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int ed1v = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int ed2v = _sv0t5;
  int _sv0t6 = sv0_vec_new();
  int ed3v = _sv0t6;
  int _sv0t7 = sv0_vec_new();
  int ed4v = _sv0t7;
  int _sv0t8 = sv0_vec_new();
  int pp = _sv0t8;
  int _sv0t9 = parse_expr(tags, starts, ends, "match x { 1 if y => 2 }", 0, et, ed1v, ed2v, ed3v, ed4v, pp, 1);
  int p = _sv0t9;
  if ((p != 9)) {
    return 1;
  } else {
  }
  int _sv0t10 = sv0_vec_get(et, 0);
  if ((_sv0t10 != 1)) {
    return 2;
  } else {
  }
  int _sv0t11 = sv0_vec_get(et, 1);
  if ((_sv0t11 != 1)) {
    return 3;
  } else {
  }
  int _sv0t12 = sv0_vec_get(et, 2);
  if ((_sv0t12 != 0)) {
    return 4;
  } else {
  }
  int _sv0t13 = sv0_vec_get(et, 3);
  if ((_sv0t13 != 30)) {
    return 5;
  } else {
  }
  int _sv0t14 = sv0_vec_get(ed4v, 3);
  if ((_sv0t14 != 2)) {
    return 16;
  } else {
  }
  int _sv0t15 = sv0_vec_get(et, 4);
  if ((_sv0t15 != 29)) {
    return 17;
  } else {
  }
  int _sv0t16 = sv0_vec_get(ed2v, 4);
  if ((_sv0t16 != 3)) {
    return 18;
  } else {
  }
  int _sv0t17 = sv0_vec_get(ed3v, 4);
  if ((_sv0t17 != 1)) {
    return 6;
  } else {
  }
  int _sv0t18 = sv0_vec_get(ed4v, 4);
  if ((_sv0t18 != 2)) {
    return 7;
  } else {
  }
  int _sv0t19 = sv0_vec_get(et, 5);
  if ((_sv0t19 != 11)) {
    return 8;
  } else {
  }
  return 0;
}

static int test_parse_match_enum_tuple_arm(void) {
  const char* src;
  src = "match x { Foo::Bar(y) => y }";
  int _sv0t0 = sv0_vec_new();
  int tags = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int starts = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int ends = _sv0t2;
  sv0_vec_push(tags, 74);
  sv0_vec_push(starts, 0);
  sv0_vec_push(ends, 5);
  sv0_vec_push(tags, 5);
  sv0_vec_push(starts, 6);
  sv0_vec_push(ends, 7);
  sv0_vec_push(tags, 8);
  sv0_vec_push(starts, 8);
  sv0_vec_push(ends, 9);
  sv0_vec_push(tags, 5);
  sv0_vec_push(starts, 10);
  sv0_vec_push(ends, 12);
  sv0_vec_push(tags, 15);
  sv0_vec_push(starts, 13);
  sv0_vec_push(ends, 15);
  sv0_vec_push(tags, 5);
  sv0_vec_push(starts, 16);
  sv0_vec_push(ends, 18);
  sv0_vec_push(tags, 6);
  sv0_vec_push(starts, 19);
  sv0_vec_push(ends, 20);
  sv0_vec_push(tags, 5);
  sv0_vec_push(starts, 21);
  sv0_vec_push(ends, 22);
  sv0_vec_push(tags, 7);
  sv0_vec_push(starts, 23);
  sv0_vec_push(ends, 24);
  sv0_vec_push(tags, 20);
  sv0_vec_push(starts, 25);
  sv0_vec_push(ends, 27);
  sv0_vec_push(tags, 5);
  sv0_vec_push(starts, 28);
  sv0_vec_push(ends, 29);
  sv0_vec_push(tags, 9);
  sv0_vec_push(starts, 30);
  sv0_vec_push(ends, 31);
  sv0_vec_push(tags, 97);
  sv0_vec_push(starts, 32);
  sv0_vec_push(ends, 32);
  int _sv0t3 = sv0_vec_new();
  int et = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int ed1v = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int ed2v = _sv0t5;
  int _sv0t6 = sv0_vec_new();
  int ed3v = _sv0t6;
  int _sv0t7 = sv0_vec_new();
  int ed4v = _sv0t7;
  int _sv0t8 = sv0_vec_new();
  int pp = _sv0t8;
  int _sv0t9 = parse_expr(tags, starts, ends, src, 0, et, ed1v, ed2v, ed3v, ed4v, pp, 1);
  int p = _sv0t9;
  if ((p != 12)) {
    return 1;
  } else {
  }
  int _sv0t10 = sv0_vec_len(et);
  int n = _sv0t10;
  int found33 = 0;
  int found30enum = 0;
  int i = 0;
  while ((i < n)) {
    int _sv0t11 = sv0_vec_get(et, i);
    if ((_sv0t11 == 33)) {
      found33 = 1;
    } else {
    }
    int _sv0t12 = sv0_vec_get(et, i);
    if ((_sv0t12 == 30)) {
      int _sv0t13 = sv0_vec_get(ed4v, i);
      if ((_sv0t13 == 5)) {
        int _sv0t14 = sv0_vec_get(ed3v, i);
        if ((_sv0t14 == 1)) {
          found30enum = 1;
        } else {
        }
      } else {
      }
    } else {
    }
    i = (i + 1);
  }
  if ((found33 != 1)) {
    return 2;
  } else {
  }
  if ((found30enum != 1)) {
    return 3;
  } else {
  }
  return 0;
}

static int test_parse_match_pat_or(void) {
  int _sv0t0 = sv0_vec_new();
  int tags = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int starts = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int ends = _sv0t2;
  sv0_vec_push(tags, 74);
  sv0_vec_push(starts, 0);
  sv0_vec_push(ends, 5);
  sv0_vec_push(tags, 5);
  sv0_vec_push(starts, 6);
  sv0_vec_push(ends, 7);
  sv0_vec_push(tags, 8);
  sv0_vec_push(starts, 8);
  sv0_vec_push(ends, 9);
  sv0_vec_push(tags, 0);
  sv0_vec_push(starts, 10);
  sv0_vec_push(ends, 11);
  sv0_vec_push(tags, 29);
  sv0_vec_push(starts, 12);
  sv0_vec_push(ends, 13);
  sv0_vec_push(tags, 0);
  sv0_vec_push(starts, 14);
  sv0_vec_push(ends, 15);
  sv0_vec_push(tags, 20);
  sv0_vec_push(starts, 16);
  sv0_vec_push(ends, 18);
  sv0_vec_push(tags, 0);
  sv0_vec_push(starts, 19);
  sv0_vec_push(ends, 20);
  sv0_vec_push(tags, 9);
  sv0_vec_push(starts, 21);
  sv0_vec_push(ends, 22);
  sv0_vec_push(tags, 97);
  sv0_vec_push(starts, 22);
  sv0_vec_push(ends, 22);
  int _sv0t3 = sv0_vec_new();
  int et = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int ed1v = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int ed2v = _sv0t5;
  int _sv0t6 = sv0_vec_new();
  int ed3v = _sv0t6;
  int _sv0t7 = sv0_vec_new();
  int ed4v = _sv0t7;
  int _sv0t8 = sv0_vec_new();
  int pp = _sv0t8;
  int _sv0t9 = parse_expr(tags, starts, ends, "match x { 1 | 2 => 3 }", 0, et, ed1v, ed2v, ed3v, ed4v, pp, 1);
  int p = _sv0t9;
  if ((p != 9)) {
    return 1;
  } else {
  }
  int _sv0t10 = sv0_vec_len(et);
  int n = _sv0t10;
  int arm29 = 0;
  int i = 0;
  while ((i < n)) {
    int _sv0t11 = sv0_vec_get(et, i);
    if ((_sv0t11 == 29)) {
      arm29 = (arm29 + 1);
    } else {
    }
    i = (i + 1);
  }
  if ((arm29 != 2)) {
    return 2;
  } else {
  }
  int _sv0t12 = (n - 1);
  int _sv0t13 = sv0_vec_get(et, _sv0t12);
  if ((_sv0t13 != 11)) {
    return 3;
  } else {
  }
  int _sv0t14 = (n - 1);
  int _sv0t15 = sv0_vec_get(ed3v, _sv0t14);
  if ((_sv0t15 != 2)) {
    return 4;
  } else {
  }
  return 0;
}

static int test_parse_quant(void) {
  int _sv0t0 = sv0_vec_new();
  int tags = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int starts = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int ends = _sv0t2;
  sv0_vec_push(tags, 67);
  sv0_vec_push(starts, 0);
  sv0_vec_push(ends, 6);
  sv0_vec_push(tags, 6);
  sv0_vec_push(starts, 6);
  sv0_vec_push(ends, 7);
  sv0_vec_push(tags, 5);
  sv0_vec_push(starts, 7);
  sv0_vec_push(ends, 8);
  sv0_vec_push(tags, 70);
  sv0_vec_push(starts, 9);
  sv0_vec_push(ends, 11);
  sv0_vec_push(tags, 5);
  sv0_vec_push(starts, 12);
  sv0_vec_push(ends, 14);
  sv0_vec_push(tags, 12);
  sv0_vec_push(starts, 14);
  sv0_vec_push(ends, 15);
  sv0_vec_push(tags, 5);
  sv0_vec_push(starts, 16);
  sv0_vec_push(ends, 20);
  sv0_vec_push(tags, 7);
  sv0_vec_push(starts, 20);
  sv0_vec_push(ends, 21);
  sv0_vec_push(tags, 97);
  sv0_vec_push(starts, 21);
  sv0_vec_push(ends, 21);
  int _sv0t3 = sv0_vec_new();
  int et = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int ed1v = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int ed2v = _sv0t5;
  int _sv0t6 = sv0_vec_new();
  int ed3v = _sv0t6;
  int _sv0t7 = sv0_vec_new();
  int ed4v = _sv0t7;
  int _sv0t8 = sv0_vec_new();
  int pp = _sv0t8;
  int _sv0t9 = parse_expr(tags, starts, ends, "forall(i in xs, body)", 0, et, ed1v, ed2v, ed3v, ed4v, pp, 1);
  int p = _sv0t9;
  if ((p != 8)) {
    return 1;
  } else {
  }
  int _sv0t10 = sv0_vec_get(et, 0);
  if ((_sv0t10 != 1)) {
    return 2;
  } else {
  }
  int _sv0t11 = sv0_vec_get(et, 1);
  if ((_sv0t11 != 1)) {
    return 3;
  } else {
  }
  int _sv0t12 = sv0_vec_get(et, 2);
  if ((_sv0t12 != 1)) {
    return 4;
  } else {
  }
  int _sv0t13 = sv0_vec_get(et, 3);
  if ((_sv0t13 != 1)) {
    return 5;
  } else {
  }
  int _sv0t14 = sv0_vec_get(et, 4);
  if ((_sv0t14 != 4)) {
    return 6;
  } else {
  }
  int _sv0t15 = sv0_vec_get(ed3v, 4);
  if ((_sv0t15 != 3)) {
    return 7;
  } else {
  }
  return 0;
}

static int test_parse_old(void) {
  int _sv0t0 = sv0_vec_new();
  int tags = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int starts = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int ends = _sv0t2;
  sv0_vec_push(tags, 80);
  sv0_vec_push(starts, 0);
  sv0_vec_push(ends, 3);
  sv0_vec_push(tags, 6);
  sv0_vec_push(starts, 3);
  sv0_vec_push(ends, 4);
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
  int et = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int ed1v = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int ed2v = _sv0t5;
  int _sv0t6 = sv0_vec_new();
  int ed3v = _sv0t6;
  int _sv0t7 = sv0_vec_new();
  int ed4v = _sv0t7;
  int _sv0t8 = sv0_vec_new();
  int pp = _sv0t8;
  int _sv0t9 = parse_expr(tags, starts, ends, "old(x)", 0, et, ed1v, ed2v, ed3v, ed4v, pp, 1);
  int p = _sv0t9;
  if ((p != 4)) {
    return 1;
  } else {
  }
  int _sv0t10 = sv0_vec_get(et, 0);
  if ((_sv0t10 != 1)) {
    return 2;
  } else {
  }
  int _sv0t11 = sv0_vec_get(et, 1);
  if ((_sv0t11 != 1)) {
    return 3;
  } else {
  }
  int _sv0t12 = sv0_vec_get(et, 2);
  if ((_sv0t12 != 4)) {
    return 4;
  } else {
  }
  int _sv0t13 = sv0_vec_get(ed3v, 2);
  if ((_sv0t13 != 1)) {
    return 5;
  } else {
  }
  return 0;
}

static int test_parse_no_alias(void) {
  int _sv0t0 = sv0_vec_new();
  int tags = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int starts = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int ends = _sv0t2;
  sv0_vec_push(tags, 79);
  sv0_vec_push(starts, 0);
  sv0_vec_push(ends, 8);
  sv0_vec_push(tags, 6);
  sv0_vec_push(starts, 8);
  sv0_vec_push(ends, 9);
  sv0_vec_push(tags, 5);
  sv0_vec_push(starts, 9);
  sv0_vec_push(ends, 10);
  sv0_vec_push(tags, 12);
  sv0_vec_push(starts, 10);
  sv0_vec_push(ends, 11);
  sv0_vec_push(tags, 5);
  sv0_vec_push(starts, 12);
  sv0_vec_push(ends, 13);
  sv0_vec_push(tags, 7);
  sv0_vec_push(starts, 13);
  sv0_vec_push(ends, 14);
  sv0_vec_push(tags, 97);
  sv0_vec_push(starts, 14);
  sv0_vec_push(ends, 14);
  int _sv0t3 = sv0_vec_new();
  int et = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int ed1v = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int ed2v = _sv0t5;
  int _sv0t6 = sv0_vec_new();
  int ed3v = _sv0t6;
  int _sv0t7 = sv0_vec_new();
  int ed4v = _sv0t7;
  int _sv0t8 = sv0_vec_new();
  int pp = _sv0t8;
  int _sv0t9 = parse_expr(tags, starts, ends, "no_alias(a, b)", 0, et, ed1v, ed2v, ed3v, ed4v, pp, 1);
  int p = _sv0t9;
  if ((p != 6)) {
    return 1;
  } else {
  }
  int _sv0t10 = sv0_vec_get(et, 0);
  if ((_sv0t10 != 1)) {
    return 2;
  } else {
  }
  int _sv0t11 = sv0_vec_get(et, 1);
  if ((_sv0t11 != 1)) {
    return 3;
  } else {
  }
  int _sv0t12 = sv0_vec_get(et, 2);
  if ((_sv0t12 != 1)) {
    return 4;
  } else {
  }
  int _sv0t13 = sv0_vec_get(et, 3);
  if ((_sv0t13 != 4)) {
    return 5;
  } else {
  }
  int _sv0t14 = sv0_vec_get(ed3v, 3);
  if ((_sv0t14 != 2)) {
    return 6;
  } else {
  }
  return 0;
}

static int test_parse_while_inv(void) {
  int _sv0t0 = sv0_vec_new();
  int tags = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int starts = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int ends = _sv0t2;
  sv0_vec_push(tags, 96);
  sv0_vec_push(starts, 0);
  sv0_vec_push(ends, 5);
  sv0_vec_push(tags, 5);
  sv0_vec_push(starts, 6);
  sv0_vec_push(ends, 7);
  sv0_vec_push(tags, 73);
  sv0_vec_push(starts, 8);
  sv0_vec_push(ends, 22);
  sv0_vec_push(tags, 6);
  sv0_vec_push(starts, 22);
  sv0_vec_push(ends, 23);
  sv0_vec_push(tags, 5);
  sv0_vec_push(starts, 23);
  sv0_vec_push(ends, 24);
  sv0_vec_push(tags, 7);
  sv0_vec_push(starts, 24);
  sv0_vec_push(ends, 25);
  sv0_vec_push(tags, 8);
  sv0_vec_push(starts, 26);
  sv0_vec_push(ends, 27);
  sv0_vec_push(tags, 9);
  sv0_vec_push(starts, 28);
  sv0_vec_push(ends, 29);
  sv0_vec_push(tags, 97);
  sv0_vec_push(starts, 29);
  sv0_vec_push(ends, 29);
  int _sv0t3 = sv0_vec_new();
  int et = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int ed1v = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int ed2v = _sv0t5;
  int _sv0t6 = sv0_vec_new();
  int ed3v = _sv0t6;
  int _sv0t7 = sv0_vec_new();
  int ed4v = _sv0t7;
  int _sv0t8 = sv0_vec_new();
  int pp = _sv0t8;
  int _sv0t9 = parse_expr(tags, starts, ends, "while c loop_invariant(x) { }", 0, et, ed1v, ed2v, ed3v, ed4v, pp, 1);
  int p = _sv0t9;
  if ((p != 8)) {
    return 1;
  } else {
  }
  int _sv0t10 = sv0_vec_get(et, 0);
  if ((_sv0t10 != 1)) {
    return 2;
  } else {
  }
  int _sv0t11 = sv0_vec_get(et, 1);
  if ((_sv0t11 != 1)) {
    return 3;
  } else {
  }
  int _sv0t12 = sv0_vec_get(et, 2);
  if ((_sv0t12 != 9)) {
    return 4;
  } else {
  }
  int _sv0t13 = sv0_vec_get(et, 3);
  if ((_sv0t13 != 12)) {
    return 5;
  } else {
  }
  int _sv0t14 = sv0_vec_get(ed4v, 3);
  if ((_sv0t14 != 1)) {
    return 6;
  } else {
  }
  return 0;
}

static int test_parse_fn_simple(void) {
  int _sv0t0 = sv0_vec_new();
  int tags = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int starts = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int ends = _sv0t2;
  const char* source;
  source = "fn f() { }";
  sv0_vec_push(tags, 65);
  sv0_vec_push(starts, 0);
  sv0_vec_push(ends, 2);
  sv0_vec_push(tags, 5);
  sv0_vec_push(starts, 3);
  sv0_vec_push(ends, 4);
  sv0_vec_push(tags, 6);
  sv0_vec_push(starts, 4);
  sv0_vec_push(ends, 5);
  sv0_vec_push(tags, 7);
  sv0_vec_push(starts, 5);
  sv0_vec_push(ends, 6);
  sv0_vec_push(tags, 8);
  sv0_vec_push(starts, 7);
  sv0_vec_push(ends, 8);
  sv0_vec_push(tags, 9);
  sv0_vec_push(starts, 9);
  sv0_vec_push(ends, 10);
  sv0_vec_push(tags, 97);
  sv0_vec_push(starts, 10);
  sv0_vec_push(ends, 10);
  int _sv0t3 = sv0_vec_new();
  int it = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int id1 = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int id2 = _sv0t5;
  int _sv0t6 = sv0_vec_new();
  int id3 = _sv0t6;
  int _sv0t7 = sv0_vec_new();
  int id4 = _sv0t7;
  int _sv0t8 = sv0_vec_new();
  int pp = _sv0t8;
  int _sv0t9 = sv0_vec_new();
  int bet = _sv0t9;
  int _sv0t10 = sv0_vec_new();
  int be1 = _sv0t10;
  int _sv0t11 = sv0_vec_new();
  int be2 = _sv0t11;
  int _sv0t12 = sv0_vec_new();
  int be3 = _sv0t12;
  int _sv0t13 = sv0_vec_new();
  int be4 = _sv0t13;
  int _sv0t14 = sv0_vec_new();
  int bsf = _sv0t14;
  int _sv0t15 = sv0_vec_new();
  int ev_ntoks = _sv0t15;
  int _sv0t16 = sv0_vec_new();
  int sf_ntoks = _sv0t16;
  int _sv0t17 = sv0_vec_new();
  int id5 = _sv0t17;
  int _sv0t18 = sv0_vec_new();
  int fn_ptoks = _sv0t18;
  int _sv0t19 = sv0_vec_new();
  int pty_tt = _sv0t19;
  int _sv0t20 = sv0_vec_new();
  int pty_td1 = _sv0t20;
  int _sv0t21 = sv0_vec_new();
  int pty_td2 = _sv0t21;
  int _sv0t22 = sv0_vec_new();
  int pty_td3 = _sv0t22;
  int _sv0t23 = sv0_vec_new();
  int sf_ty_root = _sv0t23;
  int _sv0t24 = sv0_vec_new();
  int _sv0t25 = sv0_vec_new();
  int _sv0t26 = sv0_vec_new();
  int _sv0t27 = sv0_vec_new();
  int _sv0t28 = sv0_vec_new();
  int _sv0t29 = sv0_vec_new();
  int _sv0t30 = sv0_vec_new();
  int _sv0t31 = parse_item(tags, starts, ends, source, 0, it, id1, id2, id3, id4, id5, pp, ev_ntoks, sf_ntoks, fn_ptoks, _sv0t24, _sv0t25, _sv0t26, _sv0t27, _sv0t28, _sv0t29, _sv0t30, pty_tt, pty_td1, pty_td2, pty_td3, sf_ty_root, bet, be1, be2, be3, be4, bsf);
  int r = _sv0t31;
  if ((r != 6)) {
    return 1;
  } else {
  }
  int _sv0t32 = sv0_vec_get(it, 0);
  if ((_sv0t32 != 0)) {
    return 2;
  } else {
  }
  int _sv0t33 = sv0_vec_get(id1, 0);
  if ((_sv0t33 != 1)) {
    return 3;
  } else {
  }
  int _sv0t34 = sv0_vec_get(id2, 0);
  if ((_sv0t34 != 0)) {
    return 4;
  } else {
  }
  int _sv0t35 = sv0_vec_get(id3, 0);
  if ((_sv0t35 != 0)) {
    return 5;
  } else {
  }
  int _sv0t36 = sv0_vec_get(id4, 0);
  if ((_sv0t36 < 0)) {
    return 6;
  } else {
  }
  int _sv0t37 = sv0_vec_get(id5, 0);
  if ((_sv0t37 != 0)) {
    return 7;
  } else {
  }
  int _sv0t38 = sv0_vec_len(fn_ptoks);
  if ((_sv0t38 != 0)) {
    return 8;
  } else {
  }
  return 0;
}

static int test_parse_fn_ret(void) {
  int _sv0t0 = sv0_vec_new();
  int tags = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int starts = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int ends = _sv0t2;
  const char* source;
  source = "fn f(x: i32) -> i32 { 0 }";
  sv0_vec_push(tags, 65);
  sv0_vec_push(starts, 0);
  sv0_vec_push(ends, 2);
  sv0_vec_push(tags, 5);
  sv0_vec_push(starts, 3);
  sv0_vec_push(ends, 4);
  sv0_vec_push(tags, 6);
  sv0_vec_push(starts, 4);
  sv0_vec_push(ends, 5);
  sv0_vec_push(tags, 5);
  sv0_vec_push(starts, 5);
  sv0_vec_push(ends, 6);
  sv0_vec_push(tags, 14);
  sv0_vec_push(starts, 6);
  sv0_vec_push(ends, 7);
  sv0_vec_push(tags, 5);
  sv0_vec_push(starts, 8);
  sv0_vec_push(ends, 11);
  sv0_vec_push(tags, 7);
  sv0_vec_push(starts, 11);
  sv0_vec_push(ends, 12);
  sv0_vec_push(tags, 19);
  sv0_vec_push(starts, 13);
  sv0_vec_push(ends, 15);
  sv0_vec_push(tags, 5);
  sv0_vec_push(starts, 16);
  sv0_vec_push(ends, 19);
  sv0_vec_push(tags, 8);
  sv0_vec_push(starts, 20);
  sv0_vec_push(ends, 21);
  sv0_vec_push(tags, 1);
  sv0_vec_push(starts, 22);
  sv0_vec_push(ends, 23);
  sv0_vec_push(tags, 9);
  sv0_vec_push(starts, 24);
  sv0_vec_push(ends, 25);
  sv0_vec_push(tags, 97);
  sv0_vec_push(starts, 25);
  sv0_vec_push(ends, 25);
  int _sv0t3 = sv0_vec_new();
  int it = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int id1 = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int id2 = _sv0t5;
  int _sv0t6 = sv0_vec_new();
  int id3 = _sv0t6;
  int _sv0t7 = sv0_vec_new();
  int id4 = _sv0t7;
  int _sv0t8 = sv0_vec_new();
  int pp = _sv0t8;
  int _sv0t9 = sv0_vec_new();
  int bet = _sv0t9;
  int _sv0t10 = sv0_vec_new();
  int be1 = _sv0t10;
  int _sv0t11 = sv0_vec_new();
  int be2 = _sv0t11;
  int _sv0t12 = sv0_vec_new();
  int be3 = _sv0t12;
  int _sv0t13 = sv0_vec_new();
  int be4 = _sv0t13;
  int _sv0t14 = sv0_vec_new();
  int bsf = _sv0t14;
  int _sv0t15 = sv0_vec_new();
  int ev_ntoks = _sv0t15;
  int _sv0t16 = sv0_vec_new();
  int sf_ntoks = _sv0t16;
  int _sv0t17 = sv0_vec_new();
  int id5 = _sv0t17;
  int _sv0t18 = sv0_vec_new();
  int fn_ptoks = _sv0t18;
  int _sv0t19 = sv0_vec_new();
  int pty_tt = _sv0t19;
  int _sv0t20 = sv0_vec_new();
  int pty_td1 = _sv0t20;
  int _sv0t21 = sv0_vec_new();
  int pty_td2 = _sv0t21;
  int _sv0t22 = sv0_vec_new();
  int pty_td3 = _sv0t22;
  int _sv0t23 = sv0_vec_new();
  int sf_ty_root = _sv0t23;
  int _sv0t24 = sv0_vec_new();
  int _sv0t25 = sv0_vec_new();
  int _sv0t26 = sv0_vec_new();
  int _sv0t27 = sv0_vec_new();
  int _sv0t28 = sv0_vec_new();
  int _sv0t29 = sv0_vec_new();
  int _sv0t30 = sv0_vec_new();
  int _sv0t31 = parse_item(tags, starts, ends, source, 0, it, id1, id2, id3, id4, id5, pp, ev_ntoks, sf_ntoks, fn_ptoks, _sv0t24, _sv0t25, _sv0t26, _sv0t27, _sv0t28, _sv0t29, _sv0t30, pty_tt, pty_td1, pty_td2, pty_td3, sf_ty_root, bet, be1, be2, be3, be4, bsf);
  int r = _sv0t31;
  if ((r != 12)) {
    return 1;
  } else {
  }
  int _sv0t32 = sv0_vec_get(it, 0);
  if ((_sv0t32 != 0)) {
    return 2;
  } else {
  }
  int _sv0t33 = sv0_vec_get(id1, 0);
  if ((_sv0t33 != 1)) {
    return 3;
  } else {
  }
  int _sv0t34 = sv0_vec_get(id2, 0);
  if ((_sv0t34 != 1)) {
    return 4;
  } else {
  }
  int _sv0t35 = sv0_vec_get(id3, 0);
  if ((_sv0t35 != 1)) {
    return 5;
  } else {
  }
  int _sv0t36 = sv0_vec_get(id4, 0);
  if ((_sv0t36 < 0)) {
    return 6;
  } else {
  }
  int _sv0t37 = sv0_vec_get(id5, 0);
  if ((_sv0t37 != 0)) {
    return 7;
  } else {
  }
  int _sv0t38 = sv0_vec_len(fn_ptoks);
  if ((_sv0t38 != 1)) {
    return 8;
  } else {
  }
  int _sv0t39 = sv0_vec_get(fn_ptoks, 0);
  if ((_sv0t39 != 3)) {
    return 9;
  } else {
  }
  return 0;
}

static int test_parse_struct(void) {
  int _sv0t0 = sv0_vec_new();
  int tags = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int starts = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int ends = _sv0t2;
  const char* source;
  source = "struct S { x: i32, y: bool }";
  sv0_vec_push(tags, 89);
  sv0_vec_push(starts, 0);
  sv0_vec_push(ends, 6);
  sv0_vec_push(tags, 5);
  sv0_vec_push(starts, 7);
  sv0_vec_push(ends, 8);
  sv0_vec_push(tags, 8);
  sv0_vec_push(starts, 9);
  sv0_vec_push(ends, 10);
  sv0_vec_push(tags, 5);
  sv0_vec_push(starts, 11);
  sv0_vec_push(ends, 12);
  sv0_vec_push(tags, 14);
  sv0_vec_push(starts, 12);
  sv0_vec_push(ends, 13);
  sv0_vec_push(tags, 5);
  sv0_vec_push(starts, 14);
  sv0_vec_push(ends, 17);
  sv0_vec_push(tags, 12);
  sv0_vec_push(starts, 17);
  sv0_vec_push(ends, 18);
  sv0_vec_push(tags, 5);
  sv0_vec_push(starts, 19);
  sv0_vec_push(ends, 20);
  sv0_vec_push(tags, 14);
  sv0_vec_push(starts, 20);
  sv0_vec_push(ends, 21);
  sv0_vec_push(tags, 5);
  sv0_vec_push(starts, 22);
  sv0_vec_push(ends, 26);
  sv0_vec_push(tags, 9);
  sv0_vec_push(starts, 27);
  sv0_vec_push(ends, 28);
  sv0_vec_push(tags, 97);
  sv0_vec_push(starts, 28);
  sv0_vec_push(ends, 28);
  int _sv0t3 = sv0_vec_new();
  int it = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int id1 = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int id2 = _sv0t5;
  int _sv0t6 = sv0_vec_new();
  int id3 = _sv0t6;
  int _sv0t7 = sv0_vec_new();
  int id4 = _sv0t7;
  int _sv0t8 = sv0_vec_new();
  int pp = _sv0t8;
  int _sv0t9 = sv0_vec_new();
  int bet = _sv0t9;
  int _sv0t10 = sv0_vec_new();
  int be1 = _sv0t10;
  int _sv0t11 = sv0_vec_new();
  int be2 = _sv0t11;
  int _sv0t12 = sv0_vec_new();
  int be3 = _sv0t12;
  int _sv0t13 = sv0_vec_new();
  int be4 = _sv0t13;
  int _sv0t14 = sv0_vec_new();
  int bsf = _sv0t14;
  int _sv0t15 = sv0_vec_new();
  int ev_ntoks = _sv0t15;
  int _sv0t16 = sv0_vec_new();
  int sf_ntoks = _sv0t16;
  int _sv0t17 = sv0_vec_new();
  int id5 = _sv0t17;
  int _sv0t18 = sv0_vec_new();
  int fn_ptoks = _sv0t18;
  int _sv0t19 = sv0_vec_new();
  int pty_tt = _sv0t19;
  int _sv0t20 = sv0_vec_new();
  int pty_td1 = _sv0t20;
  int _sv0t21 = sv0_vec_new();
  int pty_td2 = _sv0t21;
  int _sv0t22 = sv0_vec_new();
  int pty_td3 = _sv0t22;
  int _sv0t23 = sv0_vec_new();
  int sf_ty_root = _sv0t23;
  int _sv0t24 = sv0_vec_new();
  int _sv0t25 = sv0_vec_new();
  int _sv0t26 = sv0_vec_new();
  int _sv0t27 = sv0_vec_new();
  int _sv0t28 = sv0_vec_new();
  int _sv0t29 = sv0_vec_new();
  int _sv0t30 = sv0_vec_new();
  int _sv0t31 = parse_item(tags, starts, ends, source, 0, it, id1, id2, id3, id4, id5, pp, ev_ntoks, sf_ntoks, fn_ptoks, _sv0t24, _sv0t25, _sv0t26, _sv0t27, _sv0t28, _sv0t29, _sv0t30, pty_tt, pty_td1, pty_td2, pty_td3, sf_ty_root, bet, be1, be2, be3, be4, bsf);
  int r = _sv0t31;
  if ((r != 11)) {
    return 1;
  } else {
  }
  int _sv0t32 = sv0_vec_get(it, 0);
  if ((_sv0t32 != 1)) {
    return 2;
  } else {
  }
  int _sv0t33 = sv0_vec_get(id1, 0);
  if ((_sv0t33 != 1)) {
    return 3;
  } else {
  }
  int _sv0t34 = sv0_vec_get(id2, 0);
  if ((_sv0t34 != 2)) {
    return 4;
  } else {
  }
  int _sv0t35 = sv0_vec_get(id3, 0);
  if ((_sv0t35 != 0)) {
    return 5;
  } else {
  }
  int _sv0t36 = sv0_vec_get(id4, 0);
  if ((_sv0t36 != 2)) {
    return 6;
  } else {
  }
  int _sv0t37 = sv0_vec_len(sf_ntoks);
  if ((_sv0t37 != 2)) {
    return 7;
  } else {
  }
  int _sv0t38 = sv0_vec_get(sf_ntoks, 0);
  if ((_sv0t38 != 3)) {
    return 8;
  } else {
  }
  int _sv0t39 = sv0_vec_get(sf_ntoks, 1);
  if ((_sv0t39 != 7)) {
    return 9;
  } else {
  }
  int _sv0t40 = sv0_vec_len(sf_ty_root);
  if ((_sv0t40 != 2)) {
    return 10;
  } else {
  }
  return 0;
}

static int test_parse_enum_mixed(void) {
  int _sv0t0 = sv0_vec_new();
  int tags = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int starts = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int ends = _sv0t2;
  const char* source;
  source = "enum E { A, B(i32) }";
  sv0_vec_push(tags, 62);
  sv0_vec_push(starts, 0);
  sv0_vec_push(ends, 4);
  sv0_vec_push(tags, 5);
  sv0_vec_push(starts, 5);
  sv0_vec_push(ends, 6);
  sv0_vec_push(tags, 8);
  sv0_vec_push(starts, 7);
  sv0_vec_push(ends, 8);
  sv0_vec_push(tags, 5);
  sv0_vec_push(starts, 9);
  sv0_vec_push(ends, 10);
  sv0_vec_push(tags, 12);
  sv0_vec_push(starts, 10);
  sv0_vec_push(ends, 11);
  sv0_vec_push(tags, 5);
  sv0_vec_push(starts, 12);
  sv0_vec_push(ends, 13);
  sv0_vec_push(tags, 6);
  sv0_vec_push(starts, 13);
  sv0_vec_push(ends, 14);
  sv0_vec_push(tags, 5);
  sv0_vec_push(starts, 14);
  sv0_vec_push(ends, 17);
  sv0_vec_push(tags, 7);
  sv0_vec_push(starts, 17);
  sv0_vec_push(ends, 18);
  sv0_vec_push(tags, 9);
  sv0_vec_push(starts, 19);
  sv0_vec_push(ends, 20);
  sv0_vec_push(tags, 97);
  sv0_vec_push(starts, 20);
  sv0_vec_push(ends, 20);
  int _sv0t3 = sv0_vec_new();
  int it = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int id1 = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int id2 = _sv0t5;
  int _sv0t6 = sv0_vec_new();
  int id3 = _sv0t6;
  int _sv0t7 = sv0_vec_new();
  int id4 = _sv0t7;
  int _sv0t8 = sv0_vec_new();
  int pp = _sv0t8;
  int _sv0t9 = sv0_vec_new();
  int bet = _sv0t9;
  int _sv0t10 = sv0_vec_new();
  int be1 = _sv0t10;
  int _sv0t11 = sv0_vec_new();
  int be2 = _sv0t11;
  int _sv0t12 = sv0_vec_new();
  int be3 = _sv0t12;
  int _sv0t13 = sv0_vec_new();
  int be4 = _sv0t13;
  int _sv0t14 = sv0_vec_new();
  int bsf = _sv0t14;
  int _sv0t15 = sv0_vec_new();
  int ev_ntoks = _sv0t15;
  int _sv0t16 = sv0_vec_new();
  int sf_ntoks = _sv0t16;
  int _sv0t17 = sv0_vec_new();
  int id5 = _sv0t17;
  int _sv0t18 = sv0_vec_new();
  int fn_ptoks = _sv0t18;
  int _sv0t19 = sv0_vec_new();
  int pty_tt = _sv0t19;
  int _sv0t20 = sv0_vec_new();
  int pty_td1 = _sv0t20;
  int _sv0t21 = sv0_vec_new();
  int pty_td2 = _sv0t21;
  int _sv0t22 = sv0_vec_new();
  int pty_td3 = _sv0t22;
  int _sv0t23 = sv0_vec_new();
  int sf_ty_root = _sv0t23;
  int _sv0t24 = sv0_vec_new();
  int _sv0t25 = sv0_vec_new();
  int _sv0t26 = sv0_vec_new();
  int _sv0t27 = sv0_vec_new();
  int _sv0t28 = sv0_vec_new();
  int _sv0t29 = sv0_vec_new();
  int _sv0t30 = sv0_vec_new();
  int _sv0t31 = parse_item(tags, starts, ends, source, 0, it, id1, id2, id3, id4, id5, pp, ev_ntoks, sf_ntoks, fn_ptoks, _sv0t24, _sv0t25, _sv0t26, _sv0t27, _sv0t28, _sv0t29, _sv0t30, pty_tt, pty_td1, pty_td2, pty_td3, sf_ty_root, bet, be1, be2, be3, be4, bsf);
  int r = _sv0t31;
  if ((r != 10)) {
    return 1;
  } else {
  }
  int _sv0t32 = sv0_vec_get(it, 0);
  if ((_sv0t32 != 2)) {
    return 2;
  } else {
  }
  int _sv0t33 = sv0_vec_get(id1, 0);
  if ((_sv0t33 != 1)) {
    return 3;
  } else {
  }
  int _sv0t34 = sv0_vec_get(id2, 0);
  if ((_sv0t34 != 2)) {
    return 4;
  } else {
  }
  int _sv0t35 = sv0_vec_get(id3, 0);
  if ((_sv0t35 != 0)) {
    return 5;
  } else {
  }
  int _sv0t36 = sv0_vec_get(id4, 0);
  if ((_sv0t36 != 2)) {
    return 6;
  } else {
  }
  int _sv0t37 = sv0_vec_len(ev_ntoks);
  if ((_sv0t37 != 2)) {
    return 7;
  } else {
  }
  int _sv0t38 = sv0_vec_get(ev_ntoks, 0);
  if ((_sv0t38 != 3)) {
    return 8;
  } else {
  }
  int _sv0t39 = sv0_vec_get(ev_ntoks, 1);
  if ((_sv0t39 != 5)) {
    return 9;
  } else {
  }
  return 0;
}

static int test_parse_impl(void) {
  int _sv0t0 = sv0_vec_new();
  int tags = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int starts = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int ends = _sv0t2;
  const char* source;
  source = "impl Foo { fn bar() { } }";
  sv0_vec_push(tags, 69);
  sv0_vec_push(starts, 0);
  sv0_vec_push(ends, 4);
  sv0_vec_push(tags, 5);
  sv0_vec_push(starts, 5);
  sv0_vec_push(ends, 8);
  sv0_vec_push(tags, 8);
  sv0_vec_push(starts, 9);
  sv0_vec_push(ends, 10);
  sv0_vec_push(tags, 65);
  sv0_vec_push(starts, 11);
  sv0_vec_push(ends, 13);
  sv0_vec_push(tags, 5);
  sv0_vec_push(starts, 14);
  sv0_vec_push(ends, 17);
  sv0_vec_push(tags, 6);
  sv0_vec_push(starts, 17);
  sv0_vec_push(ends, 18);
  sv0_vec_push(tags, 7);
  sv0_vec_push(starts, 18);
  sv0_vec_push(ends, 19);
  sv0_vec_push(tags, 8);
  sv0_vec_push(starts, 20);
  sv0_vec_push(ends, 21);
  sv0_vec_push(tags, 9);
  sv0_vec_push(starts, 22);
  sv0_vec_push(ends, 23);
  sv0_vec_push(tags, 9);
  sv0_vec_push(starts, 24);
  sv0_vec_push(ends, 25);
  sv0_vec_push(tags, 97);
  sv0_vec_push(starts, 25);
  sv0_vec_push(ends, 25);
  int _sv0t3 = sv0_vec_new();
  int it = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int id1 = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int id2 = _sv0t5;
  int _sv0t6 = sv0_vec_new();
  int id3 = _sv0t6;
  int _sv0t7 = sv0_vec_new();
  int id4 = _sv0t7;
  int _sv0t8 = sv0_vec_new();
  int pp = _sv0t8;
  int _sv0t9 = sv0_vec_new();
  int bet = _sv0t9;
  int _sv0t10 = sv0_vec_new();
  int be1 = _sv0t10;
  int _sv0t11 = sv0_vec_new();
  int be2 = _sv0t11;
  int _sv0t12 = sv0_vec_new();
  int be3 = _sv0t12;
  int _sv0t13 = sv0_vec_new();
  int be4 = _sv0t13;
  int _sv0t14 = sv0_vec_new();
  int bsf = _sv0t14;
  int _sv0t15 = sv0_vec_new();
  int ev_ntoks = _sv0t15;
  int _sv0t16 = sv0_vec_new();
  int sf_ntoks = _sv0t16;
  int _sv0t17 = sv0_vec_new();
  int id5 = _sv0t17;
  int _sv0t18 = sv0_vec_new();
  int fn_ptoks = _sv0t18;
  int _sv0t19 = sv0_vec_new();
  int pty_tt = _sv0t19;
  int _sv0t20 = sv0_vec_new();
  int pty_td1 = _sv0t20;
  int _sv0t21 = sv0_vec_new();
  int pty_td2 = _sv0t21;
  int _sv0t22 = sv0_vec_new();
  int pty_td3 = _sv0t22;
  int _sv0t23 = sv0_vec_new();
  int sf_ty_root = _sv0t23;
  int _sv0t24 = sv0_vec_new();
  int _sv0t25 = sv0_vec_new();
  int _sv0t26 = sv0_vec_new();
  int _sv0t27 = sv0_vec_new();
  int _sv0t28 = sv0_vec_new();
  int _sv0t29 = sv0_vec_new();
  int _sv0t30 = sv0_vec_new();
  int _sv0t31 = parse_item(tags, starts, ends, source, 0, it, id1, id2, id3, id4, id5, pp, ev_ntoks, sf_ntoks, fn_ptoks, _sv0t24, _sv0t25, _sv0t26, _sv0t27, _sv0t28, _sv0t29, _sv0t30, pty_tt, pty_td1, pty_td2, pty_td3, sf_ty_root, bet, be1, be2, be3, be4, bsf);
  int r = _sv0t31;
  if ((r != 10)) {
    return 1;
  } else {
  }
  int _sv0t32 = sv0_vec_len(it);
  if ((_sv0t32 != 2)) {
    return 2;
  } else {
  }
  int _sv0t33 = sv0_vec_get(it, 0);
  if ((_sv0t33 != 0)) {
    return 3;
  } else {
  }
  int _sv0t34 = sv0_vec_get(id1, 0);
  if ((_sv0t34 != 4)) {
    return 4;
  } else {
  }
  int _sv0t35 = sv0_vec_get(it, 1);
  if ((_sv0t35 != 4)) {
    return 5;
  } else {
  }
  int _sv0t36 = sv0_vec_get(id1, 1);
  if ((_sv0t36 != 0)) {
    return 6;
  } else {
  }
  int _sv0t37 = sv0_vec_get(id2, 1);
  if ((_sv0t37 != 1)) {
    return 7;
  } else {
  }
  return 0;
}

static int test_parse_fn_contract(void) {
  int _sv0t0 = sv0_vec_new();
  int tags = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int starts = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int ends = _sv0t2;
  const char* source;
  source = "fn f() requires(true) { }";
  sv0_vec_push(tags, 65);
  sv0_vec_push(starts, 0);
  sv0_vec_push(ends, 2);
  sv0_vec_push(tags, 5);
  sv0_vec_push(starts, 3);
  sv0_vec_push(ends, 4);
  sv0_vec_push(tags, 6);
  sv0_vec_push(starts, 4);
  sv0_vec_push(ends, 5);
  sv0_vec_push(tags, 7);
  sv0_vec_push(starts, 5);
  sv0_vec_push(ends, 6);
  sv0_vec_push(tags, 83);
  sv0_vec_push(starts, 7);
  sv0_vec_push(ends, 15);
  sv0_vec_push(tags, 6);
  sv0_vec_push(starts, 15);
  sv0_vec_push(ends, 16);
  sv0_vec_push(tags, 91);
  sv0_vec_push(starts, 16);
  sv0_vec_push(ends, 20);
  sv0_vec_push(tags, 7);
  sv0_vec_push(starts, 20);
  sv0_vec_push(ends, 21);
  sv0_vec_push(tags, 8);
  sv0_vec_push(starts, 22);
  sv0_vec_push(ends, 23);
  sv0_vec_push(tags, 9);
  sv0_vec_push(starts, 24);
  sv0_vec_push(ends, 25);
  sv0_vec_push(tags, 97);
  sv0_vec_push(starts, 25);
  sv0_vec_push(ends, 25);
  int _sv0t3 = sv0_vec_new();
  int it = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int id1 = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int id2 = _sv0t5;
  int _sv0t6 = sv0_vec_new();
  int id3 = _sv0t6;
  int _sv0t7 = sv0_vec_new();
  int id4 = _sv0t7;
  int _sv0t8 = sv0_vec_new();
  int pp = _sv0t8;
  int _sv0t9 = sv0_vec_new();
  int bet = _sv0t9;
  int _sv0t10 = sv0_vec_new();
  int be1 = _sv0t10;
  int _sv0t11 = sv0_vec_new();
  int be2 = _sv0t11;
  int _sv0t12 = sv0_vec_new();
  int be3 = _sv0t12;
  int _sv0t13 = sv0_vec_new();
  int be4 = _sv0t13;
  int _sv0t14 = sv0_vec_new();
  int bsf = _sv0t14;
  int _sv0t15 = sv0_vec_new();
  int ev_ntoks = _sv0t15;
  int _sv0t16 = sv0_vec_new();
  int sf_ntoks = _sv0t16;
  int _sv0t17 = sv0_vec_new();
  int id5 = _sv0t17;
  int _sv0t18 = sv0_vec_new();
  int fn_ptoks = _sv0t18;
  int _sv0t19 = sv0_vec_new();
  int pty_tt = _sv0t19;
  int _sv0t20 = sv0_vec_new();
  int pty_td1 = _sv0t20;
  int _sv0t21 = sv0_vec_new();
  int pty_td2 = _sv0t21;
  int _sv0t22 = sv0_vec_new();
  int pty_td3 = _sv0t22;
  int _sv0t23 = sv0_vec_new();
  int sf_ty_root = _sv0t23;
  int _sv0t24 = sv0_vec_new();
  int _sv0t25 = sv0_vec_new();
  int _sv0t26 = sv0_vec_new();
  int _sv0t27 = sv0_vec_new();
  int _sv0t28 = sv0_vec_new();
  int _sv0t29 = sv0_vec_new();
  int _sv0t30 = sv0_vec_new();
  int _sv0t31 = parse_item(tags, starts, ends, source, 0, it, id1, id2, id3, id4, id5, pp, ev_ntoks, sf_ntoks, fn_ptoks, _sv0t24, _sv0t25, _sv0t26, _sv0t27, _sv0t28, _sv0t29, _sv0t30, pty_tt, pty_td1, pty_td2, pty_td3, sf_ty_root, bet, be1, be2, be3, be4, bsf);
  int r = _sv0t31;
  if ((r != 10)) {
    return 1;
  } else {
  }
  int _sv0t32 = sv0_vec_get(it, 0);
  if ((_sv0t32 != 0)) {
    return 2;
  } else {
  }
  int _sv0t33 = sv0_vec_get(id1, 0);
  if ((_sv0t33 != 1)) {
    return 3;
  } else {
  }
  int _sv0t34 = sv0_vec_get(id3, 0);
  if ((_sv0t34 != 0)) {
    return 4;
  } else {
  }
  int _sv0t35 = sv0_vec_get(id4, 0);
  if ((_sv0t35 < 0)) {
    return 5;
  } else {
  }
  return 0;
}

static int test_parse_program(void) {
  int _sv0t0 = sv0_vec_new();
  int tags = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int starts = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int ends = _sv0t2;
  const char* source;
  source = "fn a() { } fn b() { }";
  sv0_vec_push(tags, 65);
  sv0_vec_push(starts, 0);
  sv0_vec_push(ends, 2);
  sv0_vec_push(tags, 5);
  sv0_vec_push(starts, 3);
  sv0_vec_push(ends, 4);
  sv0_vec_push(tags, 6);
  sv0_vec_push(starts, 4);
  sv0_vec_push(ends, 5);
  sv0_vec_push(tags, 7);
  sv0_vec_push(starts, 5);
  sv0_vec_push(ends, 6);
  sv0_vec_push(tags, 8);
  sv0_vec_push(starts, 7);
  sv0_vec_push(ends, 8);
  sv0_vec_push(tags, 9);
  sv0_vec_push(starts, 9);
  sv0_vec_push(ends, 10);
  sv0_vec_push(tags, 65);
  sv0_vec_push(starts, 11);
  sv0_vec_push(ends, 13);
  sv0_vec_push(tags, 5);
  sv0_vec_push(starts, 14);
  sv0_vec_push(ends, 15);
  sv0_vec_push(tags, 6);
  sv0_vec_push(starts, 15);
  sv0_vec_push(ends, 16);
  sv0_vec_push(tags, 7);
  sv0_vec_push(starts, 16);
  sv0_vec_push(ends, 17);
  sv0_vec_push(tags, 8);
  sv0_vec_push(starts, 18);
  sv0_vec_push(ends, 19);
  sv0_vec_push(tags, 9);
  sv0_vec_push(starts, 20);
  sv0_vec_push(ends, 21);
  sv0_vec_push(tags, 97);
  sv0_vec_push(starts, 21);
  sv0_vec_push(ends, 21);
  int _sv0t3 = sv0_vec_new();
  int it = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int id1 = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int id2 = _sv0t5;
  int _sv0t6 = sv0_vec_new();
  int id3 = _sv0t6;
  int _sv0t7 = sv0_vec_new();
  int id4 = _sv0t7;
  int _sv0t8 = sv0_vec_new();
  int pp = _sv0t8;
  int _sv0t9 = sv0_vec_new();
  int bet = _sv0t9;
  int _sv0t10 = sv0_vec_new();
  int be1 = _sv0t10;
  int _sv0t11 = sv0_vec_new();
  int be2 = _sv0t11;
  int _sv0t12 = sv0_vec_new();
  int be3 = _sv0t12;
  int _sv0t13 = sv0_vec_new();
  int be4 = _sv0t13;
  int _sv0t14 = sv0_vec_new();
  int bsf = _sv0t14;
  int _sv0t15 = sv0_vec_new();
  int ev_ntoks = _sv0t15;
  int _sv0t16 = sv0_vec_new();
  int sf_ntoks = _sv0t16;
  int _sv0t17 = sv0_vec_new();
  int id5 = _sv0t17;
  int _sv0t18 = sv0_vec_new();
  int fn_ptoks = _sv0t18;
  int _sv0t19 = sv0_vec_new();
  int pty_tt = _sv0t19;
  int _sv0t20 = sv0_vec_new();
  int pty_td1 = _sv0t20;
  int _sv0t21 = sv0_vec_new();
  int pty_td2 = _sv0t21;
  int _sv0t22 = sv0_vec_new();
  int pty_td3 = _sv0t22;
  int _sv0t23 = sv0_vec_new();
  int sf_ty_root = _sv0t23;
  int _sv0t24 = sv0_vec_new();
  int _sv0t25 = sv0_vec_new();
  int _sv0t26 = sv0_vec_new();
  int _sv0t27 = sv0_vec_new();
  int _sv0t28 = sv0_vec_new();
  int _sv0t29 = sv0_vec_new();
  int _sv0t30 = sv0_vec_new();
  int _sv0t31 = parse_program(tags, starts, ends, source, 0, it, id1, id2, id3, id4, id5, pp, ev_ntoks, sf_ntoks, fn_ptoks, _sv0t24, _sv0t25, _sv0t26, _sv0t27, _sv0t28, _sv0t29, _sv0t30, pty_tt, pty_td1, pty_td2, pty_td3, sf_ty_root, bet, be1, be2, be3, be4, bsf);
  int r = _sv0t31;
  if ((r != 12)) {
    return 1;
  } else {
  }
  int _sv0t32 = sv0_vec_len(it);
  if ((_sv0t32 != 2)) {
    return 2;
  } else {
  }
  int _sv0t33 = sv0_vec_get(it, 0);
  if ((_sv0t33 != 0)) {
    return 3;
  } else {
  }
  int _sv0t34 = sv0_vec_get(id1, 0);
  if ((_sv0t34 != 1)) {
    return 4;
  } else {
  }
  int _sv0t35 = sv0_vec_get(it, 1);
  if ((_sv0t35 != 0)) {
    return 5;
  } else {
  }
  int _sv0t36 = sv0_vec_get(id1, 1);
  if ((_sv0t36 != 7)) {
    return 6;
  } else {
  }
  return 0;
}

int main(void) {
  int _sv0t0 = test_item_fn_id2_pack_roundtrip();
  int r0 = _sv0t0;
  if ((r0 != 0)) {
    int _sv0t1 = (2000 + r0);
    return _sv0t1;
  } else {
  }
  int _sv0t2 = test_binop_of();
  int r1 = _sv0t2;
  if ((r1 != 0)) {
    return r1;
  } else {
  }
  int _sv0t3 = test_assign_binop();
  int r2 = _sv0t3;
  if ((r2 != 0)) {
    int _sv0t4 = (30 + r2);
    return _sv0t4;
  } else {
  }
  int _sv0t5 = test_lit_from_tok();
  int r3 = _sv0t5;
  if ((r3 != 0)) {
    int _sv0t6 = (50 + r3);
    return _sv0t6;
  } else {
  }
  int _sv0t7 = test_expr_span();
  int r4 = _sv0t7;
  if ((r4 != 0)) {
    int _sv0t8 = (60 + r4);
    return _sv0t8;
  } else {
  }
  int _sv0t9 = test_skip_helpers();
  int r5 = _sv0t9;
  if ((r5 != 0)) {
    int _sv0t10 = (70 + r5);
    return _sv0t10;
  } else {
  }
  int _sv0t11 = test_span_merge();
  int r6 = _sv0t11;
  if ((r6 != 0)) {
    int _sv0t12 = (90 + r6);
    return _sv0t12;
  } else {
  }
  int _sv0t13 = test_precedence();
  int r7 = _sv0t13;
  if ((r7 != 0)) {
    int _sv0t14 = (100 + r7);
    return _sv0t14;
  } else {
  }
  int _sv0t15 = test_ty_name();
  int r8 = _sv0t15;
  if ((r8 != 0)) {
    int _sv0t16 = (120 + r8);
    return _sv0t16;
  } else {
  }
  int _sv0t17 = test_attr_bracket();
  int r9 = _sv0t17;
  if ((r9 != 0)) {
    int _sv0t18 = (130 + r9);
    return _sv0t18;
  } else {
  }
  int _sv0t19 = test_pratt_prec();
  int r10 = _sv0t19;
  if ((r10 != 0)) {
    int _sv0t20 = (140 + r10);
    return _sv0t20;
  } else {
  }
  int _sv0t21 = test_ps_nav();
  int r11 = _sv0t21;
  if ((r11 != 0)) {
    int _sv0t22 = (160 + r11);
    return _sv0t22;
  } else {
  }
  int _sv0t23 = test_parse_path();
  int r12 = _sv0t23;
  if ((r12 != 0)) {
    int _sv0t24 = (180 + r12);
    return _sv0t24;
  } else {
  }
  int _sv0t25 = test_ps_skip();
  int r13 = _sv0t25;
  if ((r13 != 0)) {
    int _sv0t26 = (200 + r13);
    return _sv0t26;
  } else {
  }
  int _sv0t27 = test_parse_generic_params();
  int r14 = _sv0t27;
  if ((r14 != 0)) {
    int _sv0t28 = (220 + r14);
    return _sv0t28;
  } else {
  }
  int _sv0t29 = test_parse_use_item();
  int r15 = _sv0t29;
  if ((r15 != 0)) {
    int _sv0t30 = (240 + r15);
    return _sv0t30;
  } else {
  }
  int _sv0t31 = test_parse_module_decl();
  int r16 = _sv0t31;
  if ((r16 != 0)) {
    int _sv0t32 = (260 + r16);
    return _sv0t32;
  } else {
  }
  int _sv0t33 = test_is_assign_tok();
  int r17 = _sv0t33;
  if ((r17 != 0)) {
    int _sv0t34 = (280 + r17);
    return _sv0t34;
  } else {
  }
  int _sv0t35 = test_path_of_ty();
  int r18 = _sv0t35;
  if ((r18 != 0)) {
    int _sv0t36 = (290 + r18);
    return _sv0t36;
  } else {
  }
  int _sv0t37 = test_is_builtin_type_name();
  int r19 = _sv0t37;
  if ((r19 != 0)) {
    int _sv0t38 = (300 + r19);
    return _sv0t38;
  } else {
  }
  int _sv0t39 = test_parse_type_unit();
  int r20 = _sv0t39;
  if ((r20 != 0)) {
    int _sv0t40 = (310 + r20);
    return _sv0t40;
  } else {
  }
  int _sv0t41 = test_parse_type_builtin();
  int r21 = _sv0t41;
  if ((r21 != 0)) {
    int _sv0t42 = (320 + r21);
    return _sv0t42;
  } else {
  }
  int _sv0t43 = test_parse_type_ref();
  int r22 = _sv0t43;
  if ((r22 != 0)) {
    int _sv0t44 = (330 + r22);
    return _sv0t44;
  } else {
  }
  int _sv0t45 = test_parse_type_ref_mut();
  int r23 = _sv0t45;
  if ((r23 != 0)) {
    int _sv0t46 = (340 + r23);
    return _sv0t46;
  } else {
  }
  int _sv0t47 = test_parse_type_slice();
  int r24 = _sv0t47;
  if ((r24 != 0)) {
    int _sv0t48 = (350 + r24);
    return _sv0t48;
  } else {
  }
  int _sv0t49 = test_parse_type_array();
  int r25 = _sv0t49;
  if ((r25 != 0)) {
    int _sv0t50 = (360 + r25);
    return _sv0t50;
  } else {
  }
  int _sv0t51 = test_parse_type_generic();
  int r26 = _sv0t51;
  if ((r26 != 0)) {
    int _sv0t52 = (370 + r26);
    return _sv0t52;
  } else {
  }
  int _sv0t53 = test_parse_type_paren();
  int r27 = _sv0t53;
  if ((r27 != 0)) {
    int _sv0t54 = (380 + r27);
    return _sv0t54;
  } else {
  }
  int _sv0t55 = test_parse_pat_wild();
  int r28 = _sv0t55;
  if ((r28 != 0)) {
    int _sv0t56 = (390 + r28);
    return _sv0t56;
  } else {
  }
  int _sv0t57 = test_parse_pat_bind();
  int r29 = _sv0t57;
  if ((r29 != 0)) {
    int _sv0t58 = (400 + r29);
    return _sv0t58;
  } else {
  }
  int _sv0t59 = test_parse_pat_bind_mut();
  int r30 = _sv0t59;
  if ((r30 != 0)) {
    int _sv0t60 = (410 + r30);
    return _sv0t60;
  } else {
  }
  int _sv0t61 = test_parse_pat_lit();
  int r31 = _sv0t61;
  if ((r31 != 0)) {
    int _sv0t62 = (420 + r31);
    return _sv0t62;
  } else {
  }
  int _sv0t63 = test_parse_pat_neg_lit();
  int r32 = _sv0t63;
  if ((r32 != 0)) {
    int _sv0t64 = (430 + r32);
    return _sv0t64;
  } else {
  }
  int _sv0t65 = test_parse_pat_unit();
  int r33 = _sv0t65;
  if ((r33 != 0)) {
    int _sv0t66 = (440 + r33);
    return _sv0t66;
  } else {
  }
  int _sv0t67 = test_parse_pat_tuple();
  int r34 = _sv0t67;
  if ((r34 != 0)) {
    int _sv0t68 = (450 + r34);
    return _sv0t68;
  } else {
  }
  int _sv0t69 = test_parse_pat_enum();
  int r35 = _sv0t69;
  if ((r35 != 0)) {
    int _sv0t70 = (460 + r35);
    return _sv0t70;
  } else {
  }
  int _sv0t71 = test_parse_pat_or();
  int r36 = _sv0t71;
  if ((r36 != 0)) {
    int _sv0t72 = (470 + r36);
    return _sv0t72;
  } else {
  }
  int _sv0t73 = test_parse_pat_paren();
  int r37 = _sv0t73;
  if ((r37 != 0)) {
    int _sv0t74 = (480 + r37);
    return _sv0t74;
  } else {
  }
  int _sv0t75 = test_parse_pat_bare_enum();
  int r38 = _sv0t75;
  if ((r38 != 0)) {
    int _sv0t76 = (490 + r38);
    return _sv0t76;
  } else {
  }
  int _sv0t77 = test_parse_expr_lit();
  int r39 = _sv0t77;
  if ((r39 != 0)) {
    int _sv0t78 = (500 + r39);
    return _sv0t78;
  } else {
  }
  int _sv0t79 = test_parse_expr_path();
  int r40 = _sv0t79;
  if ((r40 != 0)) {
    int _sv0t80 = (510 + r40);
    return _sv0t80;
  } else {
  }
  int _sv0t81 = test_parse_expr_binop();
  int r41 = _sv0t81;
  if ((r41 != 0)) {
    int _sv0t82 = (520 + r41);
    return _sv0t82;
  } else {
  }
  int _sv0t83 = test_parse_expr_unary();
  int r42 = _sv0t83;
  if ((r42 != 0)) {
    int _sv0t84 = (530 + r42);
    return _sv0t84;
  } else {
  }
  int _sv0t85 = test_parse_expr_call();
  int r43 = _sv0t85;
  if ((r43 != 0)) {
    int _sv0t86 = (540 + r43);
    return _sv0t86;
  } else {
  }
  int _sv0t87 = test_parse_expr_field();
  int r44 = _sv0t87;
  if ((r44 != 0)) {
    int _sv0t88 = (550 + r44);
    return _sv0t88;
  } else {
  }
  int _sv0t89 = test_parse_expr_tuple();
  int r45 = _sv0t89;
  if ((r45 != 0)) {
    int _sv0t90 = (560 + r45);
    return _sv0t90;
  } else {
  }
  int _sv0t91 = test_parse_expr_array();
  int r46 = _sv0t91;
  if ((r46 != 0)) {
    int _sv0t92 = (570 + r46);
    return _sv0t92;
  } else {
  }
  int _sv0t93 = test_parse_expr_cast();
  int r47 = _sv0t93;
  if ((r47 != 0)) {
    int _sv0t94 = (580 + r47);
    return _sv0t94;
  } else {
  }
  int _sv0t95 = test_parse_expr_precedence();
  int r48 = _sv0t95;
  if ((r48 != 0)) {
    int _sv0t96 = (590 + r48);
    return _sv0t96;
  } else {
  }
  int _sv0t97 = test_parse_expr_index();
  int r49 = _sv0t97;
  if ((r49 != 0)) {
    int _sv0t98 = (600 + r49);
    return _sv0t98;
  } else {
  }
  int _sv0t99 = test_parse_block_empty();
  int r50 = _sv0t99;
  if ((r50 != 0)) {
    int _sv0t100 = (610 + r50);
    return _sv0t100;
  } else {
  }
  int _sv0t101 = test_parse_block_tail();
  int r51 = _sv0t101;
  if ((r51 != 0)) {
    int _sv0t102 = (620 + r51);
    return _sv0t102;
  } else {
  }
  int _sv0t103 = test_parse_block_semi();
  int r52 = _sv0t103;
  if ((r52 != 0)) {
    int _sv0t104 = (630 + r52);
    return _sv0t104;
  } else {
  }
  int _sv0t105 = test_parse_block_let();
  int r53 = _sv0t105;
  if ((r53 != 0)) {
    int _sv0t106 = (640 + r53);
    return _sv0t106;
  } else {
  }
  int _sv0t107 = test_parse_if_simple();
  int r54 = _sv0t107;
  if ((r54 != 0)) {
    int _sv0t108 = (650 + r54);
    return _sv0t108;
  } else {
  }
  int _sv0t109 = test_parse_if_else();
  int r55 = _sv0t109;
  if ((r55 != 0)) {
    int _sv0t110 = (660 + r55);
    return _sv0t110;
  } else {
  }
  int _sv0t111 = test_parse_while();
  int r56 = _sv0t111;
  if ((r56 != 0)) {
    int _sv0t112 = (670 + r56);
    return _sv0t112;
  } else {
  }
  int _sv0t113 = test_parse_loop();
  int r57 = _sv0t113;
  if ((r57 != 0)) {
    int _sv0t114 = (680 + r57);
    return _sv0t114;
  } else {
  }
  int _sv0t115 = test_parse_return();
  int r58 = _sv0t115;
  if ((r58 != 0)) {
    int _sv0t116 = (690 + r58);
    return _sv0t116;
  } else {
  }
  int _sv0t117 = test_parse_assign();
  int r59 = _sv0t117;
  if ((r59 != 0)) {
    int _sv0t118 = (700 + r59);
    return _sv0t118;
  } else {
  }
  int _sv0t119 = test_parse_match_simple();
  int r60 = _sv0t119;
  if ((r60 != 0)) {
    int _sv0t120 = (710 + r60);
    return _sv0t120;
  } else {
  }
  int _sv0t121 = test_parse_match_multi();
  int r61 = _sv0t121;
  if ((r61 != 0)) {
    int _sv0t122 = (720 + r61);
    return _sv0t122;
  } else {
  }
  int _sv0t123 = test_parse_match_guard();
  int r62 = _sv0t123;
  if ((r62 != 0)) {
    int _sv0t124 = (730 + r62);
    return _sv0t124;
  } else {
  }
  int _sv0t125 = test_parse_match_enum_tuple_arm();
  int r62b = _sv0t125;
  int _sv0t126 = test_parse_match_pat_or();
  int r62c = _sv0t126;
  if ((r62b != 0)) {
    int _sv0t127 = (735 + r62b);
    return _sv0t127;
  } else {
  }
  if ((r62c != 0)) {
    int _sv0t128 = (736 + r62c);
    return _sv0t128;
  } else {
  }
  int _sv0t129 = test_parse_quant();
  int r63 = _sv0t129;
  if ((r63 != 0)) {
    int _sv0t130 = (740 + r63);
    return _sv0t130;
  } else {
  }
  int _sv0t131 = test_parse_old();
  int r64 = _sv0t131;
  if ((r64 != 0)) {
    int _sv0t132 = (750 + r64);
    return _sv0t132;
  } else {
  }
  int _sv0t133 = test_parse_no_alias();
  int r65 = _sv0t133;
  if ((r65 != 0)) {
    int _sv0t134 = (760 + r65);
    return _sv0t134;
  } else {
  }
  int _sv0t135 = test_parse_while_inv();
  int r66 = _sv0t135;
  if ((r66 != 0)) {
    int _sv0t136 = (770 + r66);
    return _sv0t136;
  } else {
  }
  int _sv0t137 = test_parse_fn_simple();
  int r67 = _sv0t137;
  if ((r67 != 0)) {
    int _sv0t138 = (780 + r67);
    return _sv0t138;
  } else {
  }
  int _sv0t139 = test_parse_fn_ret();
  int r68 = _sv0t139;
  if ((r68 != 0)) {
    int _sv0t140 = (790 + r68);
    return _sv0t140;
  } else {
  }
  int _sv0t141 = test_parse_struct();
  int r69 = _sv0t141;
  if ((r69 != 0)) {
    int _sv0t142 = (800 + r69);
    return _sv0t142;
  } else {
  }
  int _sv0t143 = test_parse_enum_mixed();
  int r70 = _sv0t143;
  if ((r70 != 0)) {
    int _sv0t144 = (810 + r70);
    return _sv0t144;
  } else {
  }
  int _sv0t145 = test_parse_impl();
  int r71 = _sv0t145;
  if ((r71 != 0)) {
    int _sv0t146 = (820 + r71);
    return _sv0t146;
  } else {
  }
  int _sv0t147 = test_parse_fn_contract();
  int r72 = _sv0t147;
  if ((r72 != 0)) {
    int _sv0t148 = (830 + r72);
    return _sv0t148;
  } else {
  }
  int _sv0t149 = test_parse_program();
  int r73 = _sv0t149;
  if ((r73 != 0)) {
    int _sv0t150 = (840 + r73);
    return _sv0t150;
  } else {
  }
  return 0;
}


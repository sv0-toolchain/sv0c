#include "sv0_runtime.h"

typedef struct {
  int tag;
  int p0;
  int p1;
} Value;
typedef struct {
  int tag;
  int p0;
  int p1;
  int p2;
} Expr;
typedef struct {
  int tag;
  int p0;
  int p1;
  int p2;
  int p3;
} Instr;

static int ir_value_tag(Value v);
static int ir_expr_tag(Expr e);
static int ir_instr_tag(Instr ins);
static Value lower_lit_to_ir_value(int lit_tag, int tok_pos, int tok_tags, const char* source, int starts, int ends);
static int parse_decimal_i32_from_tok(const char* source, int starts, int ends, int tok_pos);
static Value lower_tag_call(int et, int ed1, int ed2, int ed3, int ed4, int pp, int tok_tags, int idx, int ctr, int out_instrs, int enum_names, int enum_tag_offsets, int enum_tag_counts, int enum_tags_flat, int fn_ctx, int builtin_map, int lit_sf_names, const char* source, int starts, int ends, int item_tags, int item_names, int item_d2, int item_d3, int item_d4, int item_field_counts, int enum_vnames_flat, int enum_max_payloads, int item_fn_row, int match_env_inherit, int import_aliases);
static int block_stmt_index(int pp, int stmts_first, int sidecar, int si);
static Value lower_tag_block(int et, int ed1, int ed2, int ed3, int ed4, int pp, int tok_tags, int idx, int ctr, int out_instrs, int enum_names, int enum_tag_offsets, int enum_tag_counts, int enum_tags_flat, int fn_ctx, int builtin_map, int lit_sf_names, const char* source, int starts, int ends, int item_tags, int item_names, int item_d2, int item_d3, int item_d4, int item_field_counts, int enum_vnames_flat, int enum_max_payloads, int item_fn_row, int match_env_inherit, int import_aliases, int for_effect, int into_dest);
static Value lower_tag_if(int et, int ed1, int ed2, int ed3, int ed4, int pp, int tok_tags, int idx, int ctr, int out_instrs, int enum_names, int enum_tag_offsets, int enum_tag_counts, int enum_tags_flat, int fn_ctx, int builtin_map, int lit_sf_names, const char* source, int starts, int ends, int item_tags, int item_names, int item_d2, int item_d3, int item_d4, int item_field_counts, int enum_vnames_flat, int enum_max_payloads, int item_fn_row, int match_env_inherit, int import_aliases);
static Value lower_tag_for(int et, int ed1, int ed2, int ed3, int ed4, int pp, int tok_tags, int idx, int ctr, int out_instrs, int enum_names, int enum_tag_offsets, int enum_tag_counts, int enum_tags_flat, int fn_ctx, int builtin_map, int lit_sf_names, const char* source, int starts, int ends, int item_tags, int item_names, int item_d2, int item_d3, int item_d4, int item_field_counts, int enum_vnames_flat, int enum_max_payloads, int item_fn_row, int match_env_inherit, int import_aliases);
static Value lower_tag_try(int et, int ed1, int ed2, int ed3, int ed4, int pp, int tok_tags, int idx, int ctr, int out_instrs, int enum_names, int enum_tag_offsets, int enum_tag_counts, int enum_tags_flat, int fn_ctx, int builtin_map, int lit_sf_names, const char* source, int starts, int ends, int item_tags, int item_names, int item_d2, int item_d3, int item_d4, int item_field_counts, int enum_vnames_flat, int enum_max_payloads, int item_fn_row, int match_env_inherit, int import_aliases);
static Expr lower_cond_expr(int et, int ed1, int ed2, int ed3, int ed4, int pp, int tok_tags, int idx, int ctr, int cond_is, int enum_names, int enum_tag_offsets, int enum_tag_counts, int enum_tags_flat, int fn_ctx, int builtin_map, int lit_sf_names, const char* source, int starts, int ends, int item_tags, int item_names, int item_d2, int item_d3, int item_d4, int item_field_counts, int enum_vnames_flat, int enum_max_payloads, int item_fn_row, int match_env_inherit, int import_aliases);
static Value lower_expr_to_value(int et, int ed1, int ed2, int ed3, int ed4, int pp, int tok_tags, int idx, int ctr, int out_instrs, int enum_names, int enum_tag_offsets, int enum_tag_counts, int enum_tags_flat, int fn_ctx, int builtin_map, int lit_sf_names, const char* source, int starts, int ends, int item_tags, int item_names, int item_d2, int item_d3, int item_d4, int item_field_counts, int enum_vnames_flat, int enum_max_payloads, int item_fn_row, int match_env_inherit, int import_aliases);
static Expr value_to_expr(Value v);
static Expr lower_expr_with_instrs(int et, int ed1, int ed2, int ed3, int ed4, int pp, int tok_tags, int idx, int ctr, int out_instrs, int enum_names, int enum_tag_offsets, int enum_tag_counts, int enum_tags_flat, int fn_ctx, int builtin_map, int lit_sf_names, const char* source, int starts, int ends, int item_tags, int item_names, int item_d2, int item_d3, int item_d4, int item_field_counts, int enum_vnames_flat, int enum_max_payloads, int item_fn_row, int match_env_inherit, int import_aliases);
static int lower_for_effect(int et, int ed1, int ed2, int ed3, int ed4, int pp, int tok_tags, int idx, int ctr, int out_instrs, int enum_names, int enum_tag_offsets, int enum_tag_counts, int enum_tags_flat, int fn_ctx, int builtin_map, int lit_sf_names, const char* source, int starts, int ends, int item_tags, int item_names, int item_d2, int item_d3, int item_d4, int item_field_counts, int enum_vnames_flat, int enum_max_payloads, int item_fn_row, int match_env_inherit, int import_aliases);
static int lower_into_var_instrs(int et, int ed1, int ed2, int ed3, int ed4, int pp, int tok_tags, int body_idx, int dest, int ctr, int out_instrs, int enum_names, int enum_tag_offsets, int enum_tag_counts, int enum_tags_flat, int fn_ctx, int builtin_map, int lit_sf_names, const char* source, int starts, int ends, int item_tags, int item_names, int item_d2, int item_d3, int item_d4, int item_field_counts, int enum_vnames_flat, int enum_max_payloads, int item_fn_row, int match_env_inherit, int import_aliases);
static int bind_scrut_var(int et, int ed1, int ed2, int ed3, int ed4, int pp, int tok_tags, int scrut_idx, int ctr, int out_instrs, int enum_names, int enum_tag_offsets, int enum_tag_counts, int enum_tags_flat, int fn_ctx, int builtin_map, int lit_sf_names, int item_tags, int item_names, int item_d2, int item_d3, int item_d4, int item_field_counts, int enum_vnames_flat, int enum_max_payloads, int item_fn_row, int match_env_ctx, int scrut_cty, const char* source, int starts, int ends, int import_aliases);
static int lower_match_arms(int et, int ed1, int ed2, int ed3, int ed4, int pp, int tok_tags, int arms_first, int arm_count, int arms_sidecar, int out_var, int scrut_var, int scrut_cty, int ctr, int out_instrs, int enum_names, int enum_tag_offsets, int enum_tag_counts, int enum_tags_flat, int fn_ctx, int builtin_map, int lit_sf_names, int item_tags, int item_names, int item_d2, int item_d3, int item_d4, int item_field_counts, int enum_vnames_flat, int enum_max_payloads, int item_fn_row, int match_env, const char* source, int starts, int ends, int import_aliases);
static int lower_match_ps_peek(int tags, int pos);
static int lower_match_skip_generic_params(int tags, int pos);
static int lower_match_skip_bracket_group(int tags, int pos);
static int lower_skip_paren_group_with_generics(int tags, int open_pos);
static int lower_scan_fn_ret_ty_tok(int tok_tags, const char* source, int starts, int ends, int name_tok);
static int lower_scan_item_fn_param_names_ty_toks(int tok_tags, int name_tok_pos, int param_count, int out_param_name_tok, int out_param_ty_head_tok);
static int lower_match_env_hdr_len(void);
static int lower_match_env_magic_legacy(void);
static int lower_match_env_magic_v0(void);
static int lower_match_env_version_v0(void);
static int lower_match_env_is_legacy_path(int env);
static int lower_match_env_is_packed_v0(int env);
static int lower_match_env_alloc_v0_empty_header(void);
static int lower_match_env_append_top_fn_match_tables(int env, int item_tags, int item_names, int item_d2, const char* source, int starts, int ends, int tok_tags);
static int lower_match_env_unpack_fn_tables_for_match(int env, int fn_names_out, int fn_ret_out);
static int lower_match_env_append_item_fn_param_rows(int env, int item_tags, int item_names, int item_d3, int tok_tags, int item_fn_row);
static int lower_match_env_clone(int env);
static int lower_match_env_merge_parent_scrut(int dst, int parent);
static int lower_match_env_append_scrut_pair(int env, int name_h, int ty_h);
static int lower_match_env_scrut_ty_lookup(int env, int limit, int name_h);
static int lower_top_fn_tables_for_match(int item_tags, int item_names, int item_d2, const char* source, int starts, int ends, int tok_tags, int fn_names, int fn_ret);
static Value lower_tag_match(int et, int ed1, int ed2, int ed3, int ed4, int pp, int tok_tags, int idx, int ctr, int out_instrs, int enum_names, int enum_tag_offsets, int enum_tag_counts, int enum_tags_flat, int fn_ctx, int builtin_map, int lit_sf_names, const char* source, int starts, int ends, int item_tags, int item_names, int item_d2, int item_d3, int item_d4, int item_field_counts, int enum_vnames_flat, int enum_max_payloads, int item_fn_row, int match_env_inherit, int import_aliases);
static int lower_return(int et, int ed1, int ed2, int ed3, int ed4, int pp, int tok_tags, int idx, int ctr, int out_instrs, int enum_names, int enum_tag_offsets, int enum_tag_counts, int enum_tags_flat, int fn_ctx, int builtin_map, int lit_sf_names, const char* source, int starts, int ends, int item_tags, int item_names, int item_d2, int item_d3, int item_d4, int item_field_counts, int enum_vnames_flat, int enum_max_payloads, int item_fn_row, int match_env_inherit, int import_aliases);
static int lower_stmt(int et, int ed1, int ed2, int ed3, int ed4, int pp, int tok_tags, int s_idx, int ctr, int out_instrs, int enum_names, int enum_tag_offsets, int enum_tag_counts, int enum_tags_flat, int fn_ctx, int builtin_map, int lit_sf_names, const char* source, int starts, int ends, int item_tags, int item_names, int item_d2, int item_d3, int item_d4, int item_field_counts, int enum_vnames_flat, int enum_max_payloads, int item_fn_row, int match_env_inherit, int import_aliases, int str_locals);
static int lower_block(int et, int ed1, int ed2, int ed3, int ed4, int pp, int tok_tags, int stmts_first, int stmts_count, int tail_idx, int block_sidecar, int ctr, int out_instrs, int enum_names, int enum_tag_offsets, int enum_tag_counts, int enum_tags_flat, int fn_ctx, int builtin_map, int lit_sf_names, const char* source, int starts, int ends, int item_tags, int item_names, int item_d2, int item_d3, int item_d4, int item_field_counts, int enum_vnames_flat, int enum_max_payloads, int item_fn_row, int match_env_inherit, int import_aliases);
static int lower_body(int et, int ed1, int ed2, int ed3, int ed4, int pp, int tok_tags, int body_idx, int ctr, int out_instrs, int enum_names, int enum_tag_offsets, int enum_tag_counts, int enum_tags_flat, int fn_ctx, int builtin_map, int lit_sf_names, const char* source, int starts, int ends, int item_tags, int item_names, int item_d2, int item_d3, int item_d4, int item_field_counts, int enum_vnames_flat, int enum_max_payloads, int item_fn_row, int match_env_inherit, int import_aliases);
static int lower_fn(int et, int ed1, int ed2, int ed3, int ed4, int pp, int tok_tags, int body_idx, int fn_name_h, int req_roots, int ens_roots, int out_instrs, int enum_names, int enum_tag_offsets, int enum_tag_counts, int enum_tags_flat, int fn_ctx, int builtin_map, int lit_sf_names, const char* source, int starts, int ends, int item_tags, int item_names, int item_d2, int item_d3, int item_d4, int item_field_counts, int enum_vnames_flat, int enum_max_payloads, int item_fn_row, int match_env_inherit, int import_aliases);
static int build_builtin_map_classify(const char* s);
static int build_builtin_map(int tok_tags, const char* source, int starts, int ends);
static const char* lower(int item_tags, int item_names, int item_d2, int item_d3, int item_d4, int item_field_counts, int item_vmax, int struct_fnames_flat, int enum_vnames_flat, int sf_names, int sf_types, const char* source, int starts, int ends, int out_block_labels, int out_struct_names, int out_struct_offsets, int out_struct_counts, int out_struct_flat, int out_enum_names, int out_enum_tag_offsets, int out_enum_tag_counts, int out_enum_tag_flat, int out_enum_max, int body_et, int body_ed1, int body_ed2, int body_ed3, int body_ed4, int body_sf, int body_pp, int tok_tags, int out_blocks, int import_aliases);
static int lower_temp_base(void);
static int lower_string_cty(void);
static int lower_tok_in_bounds(int h, int starts, int ends);
static int lower_call_ret_is_string(int et, int ed1, int ed2, int pp, int call_idx, int item_tags, int item_names, int item_d2, int tok_tags, const char* source, int starts, int ends);
static int lower_init_ident_is_string_param(int et, int ed1, int ed2, int pp, int init_idx, int item_tags, int item_names, int item_d3, int tok_tags, const char* source, int starts, int ends, int item_fn_row);
static int lower_ident_in_str_locals(int et, int ed1, int ed2, int pp, int init_idx, int str_locals, const char* source, int starts, int ends);
static int lower_callee_is_void(int et, int ed1, int ed2, int pp, int call_idx, int item_tags, int item_names, int item_d2, int tok_tags, const char* source, int starts, int ends);
static int lower_callee_ret_ty_head(int et, int ed1, int ed2, int pp, int call_idx, int item_tags, int item_names, int item_d2, int tok_tags, const char* source, int starts, int ends);
static int lower_name_is_user_type(const char* name, int item_tags, int item_names, const char* source, int starts, int ends);
static int lower_call_ret_user_ty_tok(int et, int ed1, int ed2, int pp, int call_idx, int item_tags, int item_names, int item_d2, int tok_tags, const char* source, int starts, int ends);
static int lower_typed_box_deref_let(int et, int ed1, int ed2, int ed3, int ed4, int pp, int tok_tags, int init_idx, int name_tok, int ann_tok, int ctr, int out_instrs, int enum_names, int enum_tag_offsets, int enum_tag_counts, int enum_tags_flat, int fn_ctx, int builtin_map, int lit_sf_names, const char* source, int starts, int ends, int item_tags, int item_names, int item_d2, int item_d3, int item_d4, int item_field_counts, int enum_vnames_flat, int enum_max_payloads, int item_fn_row, int match_env_inherit, int import_aliases);
static int lower_vm_target(void);
static int lower_init_is_string(int et, int ed1, int ed2, int pp, int init_idx, const char* source, int starts, int ends);
static int lower_expr_yields_string(int et, int ed1, int ed2, int ed3, int pp, int idx, int item_tags, int item_names, int item_d2, int tok_tags, const char* source, int starts, int ends);
static int fresh_tmp_name(int counter);
static int next_tmp(int counter);
static const char* lower_digit(int d);
static const char* lower_int_to_str(int n);
static const char* fresh_tmp_str(int counter);
static int lower_alias_new(void);
static int lower_alias_add(int aliases, int from_h, int to_h);
static int lower_alias_lookup(int aliases, int name_h);
static int resolve_fn_callee(int aliases, int name_h);
static int lower_find_item_fn_name_by_text(int item_tags, int item_names, const char* source, int starts, int ends, const char* target_name);
static int lower_fill_import_aliases_from_use_items(int aliases, int item_tags, int item_names, int item_d2, int item_d3, int pp, const char* source, int starts, int ends);
static int resolve_enum_ctor_path(int ctor_from_enum, int ctor_from_var, int ctor_to_enum, int ctor_to_var, int enum_h, int variant_h);
static int resolved_ctor_enum(int ctor_to_enum, int idx, int default_enum);
static int resolved_ctor_variant(int ctor_to_var, int idx, int default_var);
static int find_double_colon(const char* s, int start);
static int split_qname_count(const char* s);
static int has_qualified_name(const char* s);
static const char* ast_binop_to_c(int tag);
static const char* ast_unop_to_c(int tag);
static int is_supported_unop(int tag);
static const char* ast_ty_to_c_string(const char* name);
static const char* ast_ty_to_c_string_with_user(const char* name, int name_handle, int struct_names, int enum_names);
static int is_void_cty(const char* name);
static int ast_ty_is_unit(const char* name);
static const char* old_slot_prefix(void);
static int old_slot_name_len(int name_len);
static const char* old_slot_name(const char* name);
static int param_name_tag(int pat_tag);
static int param_name(int pat_tag, int name_handle);
static int lower_field_index(int field_names, int target);
static int old_name_seen(int seen, int name);
static int old_names_add_unique(int seen, int name);
static int uniq_old_names(int names);
static int ret_syntax_is_unit(int has_ret, const char* ret_ty_name);
static int use_ret_slot(int is_unit, int ens_mentions_result);
static int append_instrs(int dst, int src);
static int store_value_to_slot(int slot, Value v, int out_instrs);
static int lower_ensures_clause(int et, int ed1, int ed2, int ed3, int ed4, int pp, int tok_tags, int expr_idx, int map_result, int fn_name_h, int ctr, int out_instrs, int enum_names, int enum_tag_offsets, int enum_tag_counts, int enum_tags_flat, int fn_ctx, int builtin_map, int lit_sf_names, const char* source, int starts, int ends, int item_tags, int item_names, int item_d2, int item_d3, int item_d4, int item_field_counts, int enum_vnames_flat, int enum_max_payloads, int item_fn_row, int match_env_inherit, int import_aliases);
static int inject_ensures_and_ret_slot(int is, int ens_instrs, int use_ret_slot_flag, int ret_slot);
static int lower_lit_supported(int lit_tag);
static int store_value_class(int is_var, int var_name_eq_slot);
static int low_classify_assign_lhs(int expr_tag);
static int enum_tag_lookup_str(int enum_names, int enum_tags_flat, int enum_tag_offsets, int enum_tag_counts, int enum_name, int variant_name, const char* source, int starts, int ends);
static int enum_tag_lookup(int enum_names, int enum_tags_flat, int enum_tag_offsets, int enum_tag_counts, int enum_name, int variant_name);
static int lower_scan_struct_field_names(int tok_tags, int name_tok_pos, int field_count, int out_field_names);
static int lower_pat_struct_field_payload_index(int tok_tags, const char* source, int starts, int ends, int vn_name_tok, int field_count, int field_name_tok);
static int struct_fields_lookup(int struct_names, int struct_field_offsets, int struct_field_counts, int struct_fields_flat, int name);
static int struct_field_count(int struct_names, int struct_field_counts, int name);
static int is_enum_cty(int enum_names, int name);
static int is_struct_cty(int struct_names, int name);
static int variant_slot_count(int shape_tag, int field_count);
static int enum_max_payload_lookup(int enum_names, int enum_max_payloads, int name);
static const char* try_variant_success(int has_ok, int has_some);
static const char* try_variant_failure(int has_err, int has_none);
static int is_try_enum(int has_ok, int has_err, int has_some, int has_none);
static int try_variant_names_lookup(int enum_names, int enum_tag_offsets, int enum_tag_counts, int enum_tags_flat, const char* source, int starts, int ends, int enum_name, int out_handles);
static int find_variant_in_items(int item_tags, int item_names, int item_field_counts, int variant_names_flat, int enum_name, int variant_name);
static int classify_literal(int lit_tag);
static int lower_lit_to_ir_tag(int lit_tag);
static const char* fn_ret_cty(int has_ret, const char* ret_cty);
static int value_to_expr_kind(int val_tag);
static int payload_field_name(int index);
static int enum_store_payload_count(int val_count);
static int enum_alloc_width(int max_payload);
static const char* ret_value_slot(void);
static int find_fn_index(int fn_names, int name_h);
static int callee_fn_index(int fn_names, int aliases, int name_h);
static int callee_fn_index_match_env(int env, int aliases, int name_h);
static int scrut_local_lookup(int scrut_names, int name_h);
static int param_ty_lookup(int param_names, int name_h);
static const char* handle_to_str(int h, const char* source, int starts, int ends);
static int lower_name_in_handle_list(const char* name, int list, const char* source, int starts, int ends);
static const char* emit_struct_td(int name_h, int fnames, int ftypes, int foff, int fcount, const char* source, int starts, int ends, int sn, int en);
static const char* emit_enum_td(int name_h, int max_payload, const char* source, int starts, int ends);
static int build_struct_order(int item_tags, int item_names, int item_field_counts, int struct_fnames_flat, int out_names, int out_offsets, int out_counts, int out_flat);
static int build_enum_variants_data(int item_tags, int item_names, int item_field_counts, int item_vmax, int enum_vnames_flat, int out_names, int out_tag_offsets, int out_tag_counts, int out_tags_flat, int out_max_payloads);
static const char* collect_typedefs_str(int item_tags, int item_names, int item_field_counts, int item_vmax, int sf_names, int sf_types, const char* source, int starts, int ends, int sn, int en);
static int mentions_result(int et, int ed1, int ed2, int ed3, int ed4, int idx, const char* source, int starts, int ends, int pp);
static int first_result_path_handle(int et, int ed1, int ed2, int ed3, int ed4, int idx, const char* source, int starts, int ends, int pp);
static int all_old_names_expr(int et, int ed1, int ed2, int ed3, int ed4, int idx, const char* source, int starts, int ends, int pp, int out);
static int expr_init_cty(int et, int ed1, int ed2, int ed3, int ed4, int idx, int pp, int fn_names, int fn_ret_ctys, int aliases);
static int scan_lets(int et, int ed1, int ed2, int ed3, int ed4, int block_idx, int pp, int fn_names, int fn_ret_ctys, int aliases, int out_names, int out_ctys);
static int scrut_locals_push(int scrut_names, int scrut_types, int extra_names, int extra_types);
static int scrut_local_lookup_bounded(int scrut_names, int limit, int name_h);
static int lower_alias_replace(int target, int from_handles, int to_handles);
static int match_scrut_cty(int et, int ed1, int ed2, int ed3, int ed4, int idx, int pp, int match_env, int aliases, int scrut_names, int scrut_ctys, int scrut_limit);
static int variant_shape_in_items(int item_tags, int item_names, int item_field_counts, int variant_shapes_flat, int variant_fcs_flat, int enum_name, int variant_idx, int out_count);
static int ctr_fresh(int ctr);
static int classify_assign_lhs_arena(int et, int ed1, int ed2, int pp, int idx, int out);
static int test_binop_to_c(void);
static int test_unop_to_c(void);
static int test_ty_to_c(void);
static int test_old_slot(void);
static int test_ret_syntax(void);
static int test_store_value_class(void);
static int test_store_value_to_slot(void);
static int test_inject_ensures_and_ret_slot(void);
static int test_assign_lhs(void);
static int test_enum_tag(void);
static int test_struct_fields(void);
static int test_type_detection(void);
static int test_variant_slots(void);
static int test_try_variants(void);
static int test_literals(void);
static int test_value_to_expr(void);
static int test_enum_alloc(void);
static int test_ret_value_slot(void);
static int test_use_ret_slot(void);
static int test_lit_supported(void);
static int test_param_name_tag(void);
static int test_uniq_old_names(void);
static int test_fresh_tmp_str(void);
static int test_lower_lit_ir(void);
static int test_fn_ret_cty(void);
static int test_lower_alias(void);
static int test_resolve_fn_callee(void);
static int test_resolve_ctor_path(void);
static int test_param_name(void);
static int test_lower_field_index(void);
static int test_ast_ty_c_string_user(void);
static int test_find_fn_index(void);
static int test_callee_fn_index(void);
static int test_scrut_local(void);
static int test_param_ty(void);
static int test_mentions_result(void);
static int test_first_result_path_handle(void);
static int test_all_old_names(void);
static int test_typedef_builders(void);
static int test_expr_init_cty(void);
static int test_try_variant_lookup(void);
static int test_find_variant_in_items(void);
static int test_scan_lets(void);
static int test_scrut_locals_push(void);
static int test_alias_replace(void);
static int test_fill_import_aliases_from_use_items(void);
static int test_fill_import_aliases_merges_with_preload(void);
static int test_match_scrut_cty(void);
static int test_variant_shape_in_items(void);
static int test_ir_types(void);
static int test_ctr_fresh(void);
static int test_classify_arena(void);
static int test_parse_decimal_i32_from_tok(void);
static int test_lower_lit_val(void);
static int test_lower_expr_val(void);
static int test_lower_return(void);
static int test_lower_if(void);
static int test_lower_block(void);
static int test_lower_assign(void);
static int test_lower_while(void);
static int test_lower_loop(void);
static int test_lower_cast(void);
static int test_lower_body_fn(void);
static int test_lower_fn_fn(void);
static int test_lower_fn_contract_glue(void);
static int test_lower_for_loop(void);
static int test_lower_expr_assert(void);
static int test_lower_expr_range(void);
static int test_lower_expr_tuple_field(void);
static int test_lower_expr_index_effect(void);
static int test_lower_expr_method_call_effect(void);
static int test_lower_expr_array_effect(void);
static int test_lower_expr_struct(void);
static int test_lower_expr_struct_qualified_path(void);
static int test_lower_program_fn(void);
static int test_lower_return_fn(void);
static int test_lower_stmt_fn(void);
static int test_lower_block_fn(void);
static int test_value_to_expr_fn(void);
static int test_lower_with_instrs(void);
static int test_lower_for_effect_fn(void);
static int test_lower_enum_ctor(void);
static int test_lower_try(void);
static int test_build_builtin_map(void);
static int test_lower_match_env_helpers(void);
static int test_lower_match_env_fn_pack_roundtrip(void);
static int test_lower_match_env_scrut_merge_roundtrip(void);
static int test_lower_match_env_clone_scrut_roundtrip(void);

static int ir_value_tag(Value v) {
  int _sv0t0;
  if ((v.tag == 0)) {
    return 0;
    _sv0t0 = 0;
  } else {
    if ((v.tag == 1)) {
      return 1;
      _sv0t0 = 0;
    } else {
      if ((v.tag == 2)) {
        return 2;
        _sv0t0 = 0;
      } else {
        if ((v.tag == 3)) {
          return 3;
          _sv0t0 = 0;
        } else {
          if ((v.tag == 4)) {
            return 4;
            _sv0t0 = 0;
          } else {
            if ((v.tag == 5)) {
              return 5;
              _sv0t0 = 0;
            } else {
              if ((v.tag == 6)) {
                return 6;
                _sv0t0 = 0;
              } else {
                if ((v.tag == 7)) {
                  return 7;
                  _sv0t0 = 0;
                } else {
                }
              }
            }
          }
        }
      }
    }
  }
  return _sv0t0;
}

static int ir_expr_tag(Expr e) {
  int _sv0t0;
  if ((e.tag == 0)) {
    return 0;
    _sv0t0 = 0;
  } else {
    if ((e.tag == 1)) {
      return 1;
      _sv0t0 = 0;
    } else {
      if ((e.tag == 2)) {
        return 2;
        _sv0t0 = 0;
      } else {
        if ((e.tag == 3)) {
          return 3;
          _sv0t0 = 0;
        } else {
          if ((e.tag == 4)) {
            return 4;
            _sv0t0 = 0;
          } else {
          }
        }
      }
    }
  }
  return _sv0t0;
}

static int ir_instr_tag(Instr ins) {
  int _sv0t0;
  if ((ins.tag == 0)) {
    return 0;
    _sv0t0 = 0;
  } else {
    if ((ins.tag == 1)) {
      return 1;
      _sv0t0 = 0;
    } else {
      if ((ins.tag == 2)) {
        return 2;
        _sv0t0 = 0;
      } else {
        if ((ins.tag == 3)) {
          return 3;
          _sv0t0 = 0;
        } else {
          if ((ins.tag == 4)) {
            return 4;
            _sv0t0 = 0;
          } else {
            if ((ins.tag == 5)) {
              return 5;
              _sv0t0 = 0;
            } else {
              if ((ins.tag == 6)) {
                return 6;
                _sv0t0 = 0;
              } else {
                if ((ins.tag == 7)) {
                  return 7;
                  _sv0t0 = 0;
                } else {
                  if ((ins.tag == 8)) {
                    return 8;
                    _sv0t0 = 0;
                  } else {
                    if ((ins.tag == 9)) {
                      return 9;
                      _sv0t0 = 0;
                    } else {
                      if ((ins.tag == 10)) {
                        return 10;
                        _sv0t0 = 0;
                      } else {
                        if ((ins.tag == 11)) {
                          return 11;
                          _sv0t0 = 0;
                        } else {
                          if ((ins.tag == 12)) {
                            return 12;
                            _sv0t0 = 0;
                          } else {
                            if ((ins.tag == 13)) {
                              return 13;
                              _sv0t0 = 0;
                            } else {
                              if ((ins.tag == 14)) {
                                return 14;
                                _sv0t0 = 0;
                              } else {
                                if ((ins.tag == 15)) {
                                  return 15;
                                  _sv0t0 = 0;
                                } else {
                                }
                              }
                            }
                          }
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  return _sv0t0;
}

static Value lower_lit_to_ir_value(int lit_tag, int tok_pos, int tok_tags, const char* source, int starts, int ends) {
  if ((lit_tag == 0)) {
    Value _sv0t0;
    int _sv0t1 = parse_decimal_i32_from_tok(source, starts, ends, tok_pos);
    _sv0t0.tag = 0;
    _sv0t0.p0 = _sv0t1;
    return _sv0t0;
  } else {
  }
  if ((lit_tag == 5)) {
    int _sv0t2 = sv0_vec_get(tok_tags, tok_pos);
    int tt = _sv0t2;
    if ((tt == 91)) {
      Value _sv0t3;
      _sv0t3.tag = 1;
      return _sv0t3;
    } else {
    }
    Value _sv0t4;
    _sv0t4.tag = 2;
    return _sv0t4;
  } else {
  }
  if ((lit_tag == 3)) {
    Value _sv0t5;
    _sv0t5.tag = 5;
    _sv0t5.p0 = tok_pos;
    return _sv0t5;
  } else {
  }
  Value _sv0t6;
  _sv0t6.tag = 4;
  return _sv0t6;
}

static int parse_decimal_i32_from_tok(const char* source, int starts, int ends, int tok_pos) {
  if ((tok_pos < 0)) {
    return 0;
  } else {
  }
  int _sv0t0 = sv0_vec_len(starts);
  if ((tok_pos >= _sv0t0)) {
    return 0;
  } else {
  }
  int _sv0t1 = sv0_vec_len(ends);
  if ((tok_pos >= _sv0t1)) {
    return 0;
  } else {
  }
  int _sv0t2 = sv0_vec_get(starts, tok_pos);
  int s = _sv0t2;
  int _sv0t3 = sv0_vec_get(ends, tok_pos);
  int e = _sv0t3;
  if ((s < 0)) {
    return 0;
  } else {
  }
  if ((e <= s)) {
    return 0;
  } else {
  }
  int acc = 0;
  int i = s;
  while ((i < e)) {
    int _sv0t4 = sv0_string_char_at(source, i);
    int c = _sv0t4;
    if ((c < 48)) {
      return 0;
    } else {
    }
    if ((c > 57)) {
      return 0;
    } else {
    }
    int d = (c - 48);
    int _sv0t5 = (acc * 10);
    acc = (_sv0t5 + d);
    i = (i + 1);
  }
  return acc;
}

static Value lower_tag_call(int et, int ed1, int ed2, int ed3, int ed4, int pp, int tok_tags, int idx, int ctr, int out_instrs, int enum_names, int enum_tag_offsets, int enum_tag_counts, int enum_tags_flat, int fn_ctx, int builtin_map, int lit_sf_names, const char* source, int starts, int ends, int item_tags, int item_names, int item_d2, int item_d3, int item_d4, int item_field_counts, int enum_vnames_flat, int enum_max_payloads, int item_fn_row, int match_env_inherit, int import_aliases) {
  int _sv0t0 = sv0_vec_get(ed1, idx);
  int callee_idx = _sv0t0;
  int _sv0t1 = sv0_vec_get(ed2, idx);
  int args_first = _sv0t1;
  int _sv0t2 = sv0_vec_get(ed3, idx);
  int args_count = _sv0t2;
  int _sv0t3 = sv0_vec_get(ed4, idx);
  int args_sidecar = _sv0t3;
  int _sv0t4 = sv0_vec_get(et, callee_idx);
  int ct_pre = _sv0t4;
  if ((ct_pre == 1)) {
    int _sv0t5 = sv0_vec_get(ed1, callee_idx);
    int cpps_pre = _sv0t5;
    int _sv0t6 = sv0_vec_get(ed2, callee_idx);
    int cppc_pre = _sv0t6;
    if ((cppc_pre == 1)) {
      int _sv0t7 = sv0_vec_get(pp, cpps_pre);
      int name_h_pre = _sv0t7;
      int _sv0t8 = (name_h_pre >= 0);
      int _sv0t9 = sv0_vec_len(starts);
      int _sv0t10 = (name_h_pre < _sv0t9);
      int _sv0t11 = (_sv0t8 && _sv0t10);
      int _sv0t12 = sv0_vec_len(ends);
      int _sv0t13 = (name_h_pre < _sv0t12);
      if ((_sv0t11 && _sv0t13)) {
        const char* _sv0t14 = handle_to_str(name_h_pre, source, starts, ends);
        const char* callee_nm_pre;
        callee_nm_pre = _sv0t14;
        int qwant_pre = 0;
        int _sv0t15 = sv0_string_eq(callee_nm_pre, "forall");
        if (_sv0t15) {
          qwant_pre = 1;
        } else {
        }
        int _sv0t16 = sv0_string_eq(callee_nm_pre, "exists");
        if (_sv0t16) {
          qwant_pre = 2;
        } else {
        }
        int _sv0t17 = (qwant_pre >= 1);
        int _sv0t18 = (args_count == 3);
        if ((_sv0t17 && _sv0t18)) {
          int arg_iter_pre = args_first;
          int arg_rng_pre = (args_first + 1);
          int arg_body_pre = (args_first + 2);
          int _sv0t19 = sv0_vec_get(et, arg_iter_pre);
          int _sv0t20 = (_sv0t19 == 1);
          int _sv0t21 = sv0_vec_get(ed2, arg_iter_pre);
          int _sv0t22 = (_sv0t21 == 1);
          int _sv0t23 = (_sv0t20 && _sv0t22);
          int _sv0t24 = sv0_vec_get(et, arg_rng_pre);
          int _sv0t25 = (_sv0t24 == 21);
          if ((_sv0t23 && _sv0t25)) {
            int _sv0t26 = sv0_vec_get(ed1, arg_rng_pre);
            int lo_ix_pre = _sv0t26;
            int _sv0t27 = sv0_vec_get(ed2, arg_rng_pre);
            int hi_ix_pre = _sv0t27;
            int _sv0t28 = sv0_vec_get(ed1, arg_iter_pre);
            int _sv0t29 = sv0_vec_get(pp, _sv0t28);
            int iter_tok_pre = _sv0t29;
            int _sv0t30 = ctr_fresh(ctr);
            int hi_t_q = _sv0t30;
            int _sv0t31 = ctr_fresh(ctr);
            int cnt_t_q = _sv0t31;
            int _sv0t32 = ctr_fresh(ctr);
            int acc_t_q = _sv0t32;
            Value _sv0t33 = lower_expr_to_value(et, ed1, ed2, ed3, ed4, pp, tok_tags, hi_ix_pre, ctr, out_instrs, enum_names, enum_tag_offsets, enum_tag_counts, enum_tags_flat, fn_ctx, builtin_map, lit_sf_names, source, starts, ends, item_tags, item_names, item_d2, item_d3, item_d4, item_field_counts, enum_vnames_flat, enum_max_payloads, item_fn_row, match_env_inherit, import_aliases);
            Value v_hi_q;
            v_hi_q = _sv0t33;
            Expr _sv0t34 = value_to_expr(v_hi_q);
            Expr e_hi_q;
            e_hi_q = _sv0t34;
            Instr a_hi_q;
            int _sv0t35 = sv0_box_alloc(4);
            sv0_box_store(_sv0t35, 0, e_hi_q.tag);
            sv0_box_store(_sv0t35, 1, e_hi_q.p0);
            sv0_box_store(_sv0t35, 2, e_hi_q.p1);
            sv0_box_store(_sv0t35, 3, e_hi_q.p2);
            a_hi_q.tag = 3;
            a_hi_q.p0 = hi_t_q;
            a_hi_q.p1 = _sv0t35;
            int _sv0t36 = sv0_box_alloc(5);
            sv0_box_store(_sv0t36, 0, a_hi_q.tag);
            sv0_box_store(_sv0t36, 1, a_hi_q.p0);
            sv0_box_store(_sv0t36, 2, a_hi_q.p1);
            sv0_box_store(_sv0t36, 3, a_hi_q.p2);
            sv0_box_store(_sv0t36, 4, a_hi_q.p3);
            sv0_vec_push(out_instrs, _sv0t36);
            Value _sv0t37 = lower_expr_to_value(et, ed1, ed2, ed3, ed4, pp, tok_tags, lo_ix_pre, ctr, out_instrs, enum_names, enum_tag_offsets, enum_tag_counts, enum_tags_flat, fn_ctx, builtin_map, lit_sf_names, source, starts, ends, item_tags, item_names, item_d2, item_d3, item_d4, item_field_counts, enum_vnames_flat, enum_max_payloads, item_fn_row, match_env_inherit, import_aliases);
            Value v_lo_q;
            v_lo_q = _sv0t37;
            Expr _sv0t38 = value_to_expr(v_lo_q);
            Expr e_lo_q;
            e_lo_q = _sv0t38;
            Instr a_lo_q;
            int _sv0t39 = sv0_box_alloc(4);
            sv0_box_store(_sv0t39, 0, e_lo_q.tag);
            sv0_box_store(_sv0t39, 1, e_lo_q.p0);
            sv0_box_store(_sv0t39, 2, e_lo_q.p1);
            sv0_box_store(_sv0t39, 3, e_lo_q.p2);
            a_lo_q.tag = 3;
            a_lo_q.p0 = cnt_t_q;
            a_lo_q.p1 = _sv0t39;
            int _sv0t40 = sv0_box_alloc(5);
            sv0_box_store(_sv0t40, 0, a_lo_q.tag);
            sv0_box_store(_sv0t40, 1, a_lo_q.p0);
            sv0_box_store(_sv0t40, 2, a_lo_q.p1);
            sv0_box_store(_sv0t40, 3, a_lo_q.p2);
            sv0_box_store(_sv0t40, 4, a_lo_q.p3);
            sv0_vec_push(out_instrs, _sv0t40);
            if ((qwant_pre == 1)) {
              Instr ia_q;
              Expr _sv0t41;
              Value _sv0t42;
              _sv0t42.tag = 1;
              int _sv0t43 = sv0_box_alloc(3);
              sv0_box_store(_sv0t43, 0, _sv0t42.tag);
              sv0_box_store(_sv0t43, 1, _sv0t42.p0);
              sv0_box_store(_sv0t43, 2, _sv0t42.p1);
              _sv0t41.tag = 0;
              _sv0t41.p0 = _sv0t43;
              int _sv0t44 = sv0_box_alloc(4);
              sv0_box_store(_sv0t44, 0, _sv0t41.tag);
              sv0_box_store(_sv0t44, 1, _sv0t41.p0);
              sv0_box_store(_sv0t44, 2, _sv0t41.p1);
              sv0_box_store(_sv0t44, 3, _sv0t41.p2);
              ia_q.tag = 3;
              ia_q.p0 = acc_t_q;
              ia_q.p1 = _sv0t44;
              int _sv0t45 = sv0_box_alloc(5);
              sv0_box_store(_sv0t45, 0, ia_q.tag);
              sv0_box_store(_sv0t45, 1, ia_q.p0);
              sv0_box_store(_sv0t45, 2, ia_q.p1);
              sv0_box_store(_sv0t45, 3, ia_q.p2);
              sv0_box_store(_sv0t45, 4, ia_q.p3);
              sv0_vec_push(out_instrs, _sv0t45);
            } else {
              Instr ia2_q;
              Expr _sv0t46;
              Value _sv0t47;
              _sv0t47.tag = 2;
              int _sv0t48 = sv0_box_alloc(3);
              sv0_box_store(_sv0t48, 0, _sv0t47.tag);
              sv0_box_store(_sv0t48, 1, _sv0t47.p0);
              sv0_box_store(_sv0t48, 2, _sv0t47.p1);
              _sv0t46.tag = 0;
              _sv0t46.p0 = _sv0t48;
              int _sv0t49 = sv0_box_alloc(4);
              sv0_box_store(_sv0t49, 0, _sv0t46.tag);
              sv0_box_store(_sv0t49, 1, _sv0t46.p0);
              sv0_box_store(_sv0t49, 2, _sv0t46.p1);
              sv0_box_store(_sv0t49, 3, _sv0t46.p2);
              ia2_q.tag = 3;
              ia2_q.p0 = acc_t_q;
              ia2_q.p1 = _sv0t49;
              int _sv0t50 = sv0_box_alloc(5);
              sv0_box_store(_sv0t50, 0, ia2_q.tag);
              sv0_box_store(_sv0t50, 1, ia2_q.p0);
              sv0_box_store(_sv0t50, 2, ia2_q.p1);
              sv0_box_store(_sv0t50, 3, ia2_q.p2);
              sv0_box_store(_sv0t50, 4, ia2_q.p3);
              sv0_vec_push(out_instrs, _sv0t50);
            }
            Expr cond_e_q;
            Value _sv0t51;
            _sv0t51.tag = 3;
            _sv0t51.p0 = cnt_t_q;
            int _sv0t52 = sv0_box_alloc(3);
            sv0_box_store(_sv0t52, 0, _sv0t51.tag);
            sv0_box_store(_sv0t52, 1, _sv0t51.p0);
            sv0_box_store(_sv0t52, 2, _sv0t51.p1);
            Value _sv0t53;
            _sv0t53.tag = 3;
            _sv0t53.p0 = hi_t_q;
            int _sv0t54 = sv0_box_alloc(3);
            sv0_box_store(_sv0t54, 0, _sv0t53.tag);
            sv0_box_store(_sv0t54, 1, _sv0t53.p0);
            sv0_box_store(_sv0t54, 2, _sv0t53.p1);
            cond_e_q.tag = 2;
            cond_e_q.p0 = 7;
            cond_e_q.p1 = _sv0t52;
            cond_e_q.p2 = _sv0t54;
            int _sv0t55 = ctr_fresh(ctr);
            int p_t_q = _sv0t55;
            int _sv0t56 = sv0_vec_new();
            int inner_q = _sv0t56;
            Instr it_q;
            Expr _sv0t57;
            _sv0t57.tag = 1;
            _sv0t57.p0 = cnt_t_q;
            int _sv0t58 = sv0_box_alloc(4);
            sv0_box_store(_sv0t58, 0, _sv0t57.tag);
            sv0_box_store(_sv0t58, 1, _sv0t57.p0);
            sv0_box_store(_sv0t58, 2, _sv0t57.p1);
            sv0_box_store(_sv0t58, 3, _sv0t57.p2);
            it_q.tag = 3;
            it_q.p0 = iter_tok_pre;
            it_q.p1 = _sv0t58;
            int _sv0t59 = sv0_box_alloc(5);
            sv0_box_store(_sv0t59, 0, it_q.tag);
            sv0_box_store(_sv0t59, 1, it_q.p0);
            sv0_box_store(_sv0t59, 2, it_q.p1);
            sv0_box_store(_sv0t59, 3, it_q.p2);
            sv0_box_store(_sv0t59, 4, it_q.p3);
            sv0_vec_push(inner_q, _sv0t59);
            Expr _sv0t60 = lower_expr_with_instrs(et, ed1, ed2, ed3, ed4, pp, tok_tags, arg_body_pre, ctr, inner_q, enum_names, enum_tag_offsets, enum_tag_counts, enum_tags_flat, fn_ctx, builtin_map, lit_sf_names, source, starts, ends, item_tags, item_names, item_d2, item_d3, item_d4, item_field_counts, enum_vnames_flat, enum_max_payloads, item_fn_row, match_env_inherit, import_aliases);
            Expr ep_q;
            ep_q = _sv0t60;
            Instr dv_q;
            dv_q.tag = 1;
            dv_q.p0 = p_t_q;
            int _sv0t61 = sv0_box_alloc(5);
            sv0_box_store(_sv0t61, 0, dv_q.tag);
            sv0_box_store(_sv0t61, 1, dv_q.p0);
            sv0_box_store(_sv0t61, 2, dv_q.p1);
            sv0_box_store(_sv0t61, 3, dv_q.p2);
            sv0_box_store(_sv0t61, 4, dv_q.p3);
            sv0_vec_push(inner_q, _sv0t61);
            Instr st_q;
            int _sv0t62 = sv0_box_alloc(4);
            sv0_box_store(_sv0t62, 0, ep_q.tag);
            sv0_box_store(_sv0t62, 1, ep_q.p0);
            sv0_box_store(_sv0t62, 2, ep_q.p1);
            sv0_box_store(_sv0t62, 3, ep_q.p2);
            st_q.tag = 4;
            st_q.p0 = p_t_q;
            st_q.p1 = _sv0t62;
            int _sv0t63 = sv0_box_alloc(5);
            sv0_box_store(_sv0t63, 0, st_q.tag);
            sv0_box_store(_sv0t63, 1, st_q.p0);
            sv0_box_store(_sv0t63, 2, st_q.p1);
            sv0_box_store(_sv0t63, 3, st_q.p2);
            sv0_box_store(_sv0t63, 4, st_q.p3);
            sv0_vec_push(inner_q, _sv0t63);
            int _sv0t64 = sv0_vec_new();
            int else_q = _sv0t64;
            if ((qwant_pre == 1)) {
              Expr cond_pf_q;
              Value _sv0t65;
              _sv0t65.tag = 3;
              _sv0t65.p0 = p_t_q;
              int _sv0t66 = sv0_box_alloc(3);
              sv0_box_store(_sv0t66, 0, _sv0t65.tag);
              sv0_box_store(_sv0t66, 1, _sv0t65.p0);
              sv0_box_store(_sv0t66, 2, _sv0t65.p1);
              Value _sv0t67;
              _sv0t67.tag = 0;
              _sv0t67.p0 = 0;
              int _sv0t68 = sv0_box_alloc(3);
              sv0_box_store(_sv0t68, 0, _sv0t67.tag);
              sv0_box_store(_sv0t68, 1, _sv0t67.p0);
              sv0_box_store(_sv0t68, 2, _sv0t67.p1);
              cond_pf_q.tag = 2;
              cond_pf_q.p0 = 5;
              cond_pf_q.p1 = _sv0t66;
              cond_pf_q.p2 = _sv0t68;
              int _sv0t69 = sv0_vec_new();
              int then_q = _sv0t69;
              Instr stf_q;
              Expr _sv0t70;
              Value _sv0t71;
              _sv0t71.tag = 2;
              int _sv0t72 = sv0_box_alloc(3);
              sv0_box_store(_sv0t72, 0, _sv0t71.tag);
              sv0_box_store(_sv0t72, 1, _sv0t71.p0);
              sv0_box_store(_sv0t72, 2, _sv0t71.p1);
              _sv0t70.tag = 0;
              _sv0t70.p0 = _sv0t72;
              int _sv0t73 = sv0_box_alloc(4);
              sv0_box_store(_sv0t73, 0, _sv0t70.tag);
              sv0_box_store(_sv0t73, 1, _sv0t70.p0);
              sv0_box_store(_sv0t73, 2, _sv0t70.p1);
              sv0_box_store(_sv0t73, 3, _sv0t70.p2);
              stf_q.tag = 4;
              stf_q.p0 = acc_t_q;
              stf_q.p1 = _sv0t73;
              int _sv0t74 = sv0_box_alloc(5);
              sv0_box_store(_sv0t74, 0, stf_q.tag);
              sv0_box_store(_sv0t74, 1, stf_q.p0);
              sv0_box_store(_sv0t74, 2, stf_q.p1);
              sv0_box_store(_sv0t74, 3, stf_q.p2);
              sv0_box_store(_sv0t74, 4, stf_q.p3);
              sv0_vec_push(then_q, _sv0t74);
              Instr _sv0t75;
              _sv0t75.tag = 9;
              int _sv0t76 = sv0_box_alloc(5);
              sv0_box_store(_sv0t76, 0, _sv0t75.tag);
              sv0_box_store(_sv0t76, 1, _sv0t75.p0);
              sv0_box_store(_sv0t76, 2, _sv0t75.p1);
              sv0_box_store(_sv0t76, 3, _sv0t75.p2);
              sv0_box_store(_sv0t76, 4, _sv0t75.p3);
              sv0_vec_push(then_q, _sv0t76);
              Instr if_q;
              int _sv0t77 = sv0_box_alloc(4);
              sv0_box_store(_sv0t77, 0, cond_pf_q.tag);
              sv0_box_store(_sv0t77, 1, cond_pf_q.p0);
              sv0_box_store(_sv0t77, 2, cond_pf_q.p1);
              sv0_box_store(_sv0t77, 3, cond_pf_q.p2);
              if_q.tag = 6;
              if_q.p0 = _sv0t77;
              if_q.p1 = then_q;
              if_q.p2 = else_q;
              int _sv0t78 = sv0_box_alloc(5);
              sv0_box_store(_sv0t78, 0, if_q.tag);
              sv0_box_store(_sv0t78, 1, if_q.p0);
              sv0_box_store(_sv0t78, 2, if_q.p1);
              sv0_box_store(_sv0t78, 3, if_q.p2);
              sv0_box_store(_sv0t78, 4, if_q.p3);
              sv0_vec_push(inner_q, _sv0t78);
            } else {
              Expr cond_pe_q;
              Value _sv0t79;
              _sv0t79.tag = 3;
              _sv0t79.p0 = p_t_q;
              int _sv0t80 = sv0_box_alloc(3);
              sv0_box_store(_sv0t80, 0, _sv0t79.tag);
              sv0_box_store(_sv0t80, 1, _sv0t79.p0);
              sv0_box_store(_sv0t80, 2, _sv0t79.p1);
              Value _sv0t81;
              _sv0t81.tag = 0;
              _sv0t81.p0 = 0;
              int _sv0t82 = sv0_box_alloc(3);
              sv0_box_store(_sv0t82, 0, _sv0t81.tag);
              sv0_box_store(_sv0t82, 1, _sv0t81.p0);
              sv0_box_store(_sv0t82, 2, _sv0t81.p1);
              cond_pe_q.tag = 2;
              cond_pe_q.p0 = 6;
              cond_pe_q.p1 = _sv0t80;
              cond_pe_q.p2 = _sv0t82;
              int _sv0t83 = sv0_vec_new();
              int then_e = _sv0t83;
              Instr ste_q;
              Expr _sv0t84;
              Value _sv0t85;
              _sv0t85.tag = 1;
              int _sv0t86 = sv0_box_alloc(3);
              sv0_box_store(_sv0t86, 0, _sv0t85.tag);
              sv0_box_store(_sv0t86, 1, _sv0t85.p0);
              sv0_box_store(_sv0t86, 2, _sv0t85.p1);
              _sv0t84.tag = 0;
              _sv0t84.p0 = _sv0t86;
              int _sv0t87 = sv0_box_alloc(4);
              sv0_box_store(_sv0t87, 0, _sv0t84.tag);
              sv0_box_store(_sv0t87, 1, _sv0t84.p0);
              sv0_box_store(_sv0t87, 2, _sv0t84.p1);
              sv0_box_store(_sv0t87, 3, _sv0t84.p2);
              ste_q.tag = 4;
              ste_q.p0 = acc_t_q;
              ste_q.p1 = _sv0t87;
              int _sv0t88 = sv0_box_alloc(5);
              sv0_box_store(_sv0t88, 0, ste_q.tag);
              sv0_box_store(_sv0t88, 1, ste_q.p0);
              sv0_box_store(_sv0t88, 2, ste_q.p1);
              sv0_box_store(_sv0t88, 3, ste_q.p2);
              sv0_box_store(_sv0t88, 4, ste_q.p3);
              sv0_vec_push(then_e, _sv0t88);
              Instr _sv0t89;
              _sv0t89.tag = 9;
              int _sv0t90 = sv0_box_alloc(5);
              sv0_box_store(_sv0t90, 0, _sv0t89.tag);
              sv0_box_store(_sv0t90, 1, _sv0t89.p0);
              sv0_box_store(_sv0t90, 2, _sv0t89.p1);
              sv0_box_store(_sv0t90, 3, _sv0t89.p2);
              sv0_box_store(_sv0t90, 4, _sv0t89.p3);
              sv0_vec_push(then_e, _sv0t90);
              Instr if_e;
              int _sv0t91 = sv0_box_alloc(4);
              sv0_box_store(_sv0t91, 0, cond_pe_q.tag);
              sv0_box_store(_sv0t91, 1, cond_pe_q.p0);
              sv0_box_store(_sv0t91, 2, cond_pe_q.p1);
              sv0_box_store(_sv0t91, 3, cond_pe_q.p2);
              if_e.tag = 6;
              if_e.p0 = _sv0t91;
              if_e.p1 = then_e;
              if_e.p2 = else_q;
              int _sv0t92 = sv0_box_alloc(5);
              sv0_box_store(_sv0t92, 0, if_e.tag);
              sv0_box_store(_sv0t92, 1, if_e.p0);
              sv0_box_store(_sv0t92, 2, if_e.p1);
              sv0_box_store(_sv0t92, 3, if_e.p2);
              sv0_box_store(_sv0t92, 4, if_e.p3);
              sv0_vec_push(inner_q, _sv0t92);
            }
            Value one_q;
            one_q.tag = 0;
            one_q.p0 = 1;
            Expr incr_eq;
            Value _sv0t93;
            _sv0t93.tag = 3;
            _sv0t93.p0 = cnt_t_q;
            int _sv0t94 = sv0_box_alloc(3);
            sv0_box_store(_sv0t94, 0, _sv0t93.tag);
            sv0_box_store(_sv0t94, 1, _sv0t93.p0);
            sv0_box_store(_sv0t94, 2, _sv0t93.p1);
            int _sv0t95 = sv0_box_alloc(3);
            sv0_box_store(_sv0t95, 0, one_q.tag);
            sv0_box_store(_sv0t95, 1, one_q.p0);
            sv0_box_store(_sv0t95, 2, one_q.p1);
            incr_eq.tag = 2;
            incr_eq.p0 = 0;
            incr_eq.p1 = _sv0t94;
            incr_eq.p2 = _sv0t95;
            Instr incr_iq;
            int _sv0t96 = sv0_box_alloc(4);
            sv0_box_store(_sv0t96, 0, incr_eq.tag);
            sv0_box_store(_sv0t96, 1, incr_eq.p0);
            sv0_box_store(_sv0t96, 2, incr_eq.p1);
            sv0_box_store(_sv0t96, 3, incr_eq.p2);
            incr_iq.tag = 4;
            incr_iq.p0 = cnt_t_q;
            incr_iq.p1 = _sv0t96;
            int _sv0t97 = sv0_box_alloc(5);
            sv0_box_store(_sv0t97, 0, incr_iq.tag);
            sv0_box_store(_sv0t97, 1, incr_iq.p0);
            sv0_box_store(_sv0t97, 2, incr_iq.p1);
            sv0_box_store(_sv0t97, 3, incr_iq.p2);
            sv0_box_store(_sv0t97, 4, incr_iq.p3);
            sv0_vec_push(inner_q, _sv0t97);
            Instr wl_q;
            int _sv0t98 = sv0_box_alloc(4);
            sv0_box_store(_sv0t98, 0, cond_e_q.tag);
            sv0_box_store(_sv0t98, 1, cond_e_q.p0);
            sv0_box_store(_sv0t98, 2, cond_e_q.p1);
            sv0_box_store(_sv0t98, 3, cond_e_q.p2);
            wl_q.tag = 7;
            wl_q.p0 = _sv0t98;
            wl_q.p1 = inner_q;
            int _sv0t99 = sv0_box_alloc(5);
            sv0_box_store(_sv0t99, 0, wl_q.tag);
            sv0_box_store(_sv0t99, 1, wl_q.p0);
            sv0_box_store(_sv0t99, 2, wl_q.p1);
            sv0_box_store(_sv0t99, 3, wl_q.p2);
            sv0_box_store(_sv0t99, 4, wl_q.p3);
            sv0_vec_push(out_instrs, _sv0t99);
            Value _sv0t100;
            _sv0t100.tag = 3;
            _sv0t100.p0 = acc_t_q;
            return _sv0t100;
          } else {
          }
        } else {
        }
      } else {
      }
    } else {
    }
  } else {
  }
  int ec_t = (0 - 1);
  int _sv0t101 = sv0_vec_get(et, callee_idx);
  if ((_sv0t101 == 1)) {
    int _sv0t102 = sv0_vec_get(ed2, callee_idx);
    if ((_sv0t102 == 2)) {
      int _sv0t103 = sv0_vec_get(ed1, callee_idx);
      int _sv0t104 = sv0_vec_get(pp, _sv0t103);
      int en_tok_pre = _sv0t104;
      int _sv0t105 = ctr_fresh(ctr);
      ec_t = _sv0t105;
      Instr _sv0t106;
      _sv0t106.tag = 2;
      _sv0t106.p0 = en_tok_pre;
      _sv0t106.p1 = ec_t;
      int _sv0t107 = sv0_box_alloc(5);
      sv0_box_store(_sv0t107, 0, _sv0t106.tag);
      sv0_box_store(_sv0t107, 1, _sv0t106.p0);
      sv0_box_store(_sv0t107, 2, _sv0t106.p1);
      sv0_box_store(_sv0t107, 3, _sv0t106.p2);
      sv0_box_store(_sv0t107, 4, _sv0t106.p3);
      sv0_vec_push(out_instrs, _sv0t107);
    } else {
    }
  } else {
  }
  int _sv0t108 = sv0_vec_new();
  int arg_vals = _sv0t108;
  int ai = 0;
  while (1) {
    if ((ai >= args_count)) {
      break;
    } else {
    }
    int _sv0t109 = block_stmt_index(pp, args_first, args_sidecar, ai);
    int a_idx = _sv0t109;
    Value _sv0t110 = lower_expr_to_value(et, ed1, ed2, ed3, ed4, pp, tok_tags, a_idx, ctr, out_instrs, enum_names, enum_tag_offsets, enum_tag_counts, enum_tags_flat, fn_ctx, builtin_map, lit_sf_names, source, starts, ends, item_tags, item_names, item_d2, item_d3, item_d4, item_field_counts, enum_vnames_flat, enum_max_payloads, item_fn_row, match_env_inherit, import_aliases);
    Value av;
    av = _sv0t110;
    int _sv0t111 = sv0_box_alloc(3);
    sv0_box_store(_sv0t111, 0, av.tag);
    sv0_box_store(_sv0t111, 1, av.p0);
    sv0_box_store(_sv0t111, 2, av.p1);
    sv0_vec_push(arg_vals, _sv0t111);
    ai = (ai + 1);
  }
  int _sv0t112 = sv0_vec_get(et, callee_idx);
  int ct = _sv0t112;
  if ((ct == 1)) {
    int _sv0t113 = sv0_vec_get(ed1, callee_idx);
    int cpps = _sv0t113;
    int _sv0t114 = sv0_vec_get(ed2, callee_idx);
    int cppc = _sv0t114;
    if ((cppc == 1)) {
      int _sv0t115 = sv0_vec_get(pp, cpps);
      int callee_tok = _sv0t115;
      int _sv0t116 = ctr_fresh(ctr);
      int t = _sv0t116;
      int bid = 0;
      if ((callee_tok >= 0)) {
        int _sv0t117 = sv0_vec_len(builtin_map);
        if ((callee_tok < _sv0t117)) {
          int _sv0t118 = sv0_vec_get(builtin_map, callee_tok);
          bid = _sv0t118;
        } else {
        }
      } else {
      }
      if ((bid == 0)) {
        int _sv0t119 = lower_callee_is_void(et, ed1, ed2, pp, idx, item_tags, item_names, item_d2, tok_tags, source, starts, ends);
        if (_sv0t119) {
          Instr void_call_instr;
          void_call_instr.tag = 11;
          void_call_instr.p0 = 0;
          void_call_instr.p1 = callee_tok;
          void_call_instr.p2 = arg_vals;
          void_call_instr.p3 = 0;
          int _sv0t120 = sv0_box_alloc(5);
          sv0_box_store(_sv0t120, 0, void_call_instr.tag);
          sv0_box_store(_sv0t120, 1, void_call_instr.p0);
          sv0_box_store(_sv0t120, 2, void_call_instr.p1);
          sv0_box_store(_sv0t120, 3, void_call_instr.p2);
          sv0_box_store(_sv0t120, 4, void_call_instr.p3);
          sv0_vec_push(out_instrs, _sv0t120);
          Value _sv0t121;
          _sv0t121.tag = 4;
          return _sv0t121;
        } else {
        }
        int rt_u = 0;
        int _sv0t122 = lower_call_ret_is_string(et, ed1, ed2, pp, idx, item_tags, item_names, item_d2, tok_tags, source, starts, ends);
        if (_sv0t122) {
          int _sv0t123 = lower_string_cty();
          rt_u = _sv0t123;
        } else {
          int _sv0t124 = lower_call_ret_user_ty_tok(et, ed1, ed2, pp, idx, item_tags, item_names, item_d2, tok_tags, source, starts, ends);
          int uty_c = _sv0t124;
          if ((uty_c >= 0)) {
            rt_u = uty_c;
          } else {
          }
        }
        Instr call_instr;
        call_instr.tag = 11;
        call_instr.p0 = t;
        call_instr.p1 = callee_tok;
        call_instr.p2 = arg_vals;
        call_instr.p3 = rt_u;
        int _sv0t125 = sv0_box_alloc(5);
        sv0_box_store(_sv0t125, 0, call_instr.tag);
        sv0_box_store(_sv0t125, 1, call_instr.p0);
        sv0_box_store(_sv0t125, 2, call_instr.p1);
        sv0_box_store(_sv0t125, 3, call_instr.p2);
        sv0_box_store(_sv0t125, 4, call_instr.p3);
        sv0_vec_push(out_instrs, _sv0t125);
        Value _sv0t126;
        _sv0t126.tag = 3;
        _sv0t126.p0 = t;
        return _sv0t126;
      } else {
      }
      if ((bid == 14)) {
        if ((args_count >= 1)) {
          int _sv0t127 = sv0_vec_get(arg_vals, 0);
          Value _sv0t128;
          int _sv0t129 = sv0_box_load(_sv0t127, 0);
          _sv0t128.tag = _sv0t129;
          int _sv0t130 = sv0_box_load(_sv0t127, 1);
          _sv0t128.p0 = _sv0t130;
          int _sv0t131 = sv0_box_load(_sv0t127, 2);
          _sv0t128.p1 = _sv0t131;
          Value na_val;
          na_val.tag = (_sv0t128).tag;
          na_val.p0 = (_sv0t128).p0;
          na_val.p1 = (_sv0t128).p1;
          return na_val;
        } else {
        }
        Value _sv0t132;
        _sv0t132.tag = 4;
        return _sv0t132;
      } else {
      }
      int is_void = 0;
      if ((bid == 1)) {
        is_void = 1;
      } else {
      }
      if ((bid == 8)) {
        is_void = 1;
      } else {
      }
      if ((bid == 11)) {
        is_void = 1;
      } else {
      }
      if ((bid == 16)) {
        is_void = 1;
      } else {
      }
      if ((bid == 18)) {
        is_void = 1;
      } else {
      }
      int fn_sentinel = (0 - bid);
      if ((is_void == 1)) {
        Instr vc;
        vc.tag = 11;
        vc.p0 = 0;
        vc.p1 = fn_sentinel;
        vc.p2 = arg_vals;
        vc.p3 = 0;
        int _sv0t133 = sv0_box_alloc(5);
        sv0_box_store(_sv0t133, 0, vc.tag);
        sv0_box_store(_sv0t133, 1, vc.p0);
        sv0_box_store(_sv0t133, 2, vc.p1);
        sv0_box_store(_sv0t133, 3, vc.p2);
        sv0_box_store(_sv0t133, 4, vc.p3);
        sv0_vec_push(out_instrs, _sv0t133);
        Value _sv0t134;
        _sv0t134.tag = 4;
        return _sv0t134;
      } else {
      }
      Instr bc;
      bc.tag = 11;
      bc.p0 = t;
      bc.p1 = fn_sentinel;
      bc.p2 = arg_vals;
      bc.p3 = 0;
      int _sv0t135 = sv0_box_alloc(5);
      sv0_box_store(_sv0t135, 0, bc.tag);
      sv0_box_store(_sv0t135, 1, bc.p0);
      sv0_box_store(_sv0t135, 2, bc.p1);
      sv0_box_store(_sv0t135, 3, bc.p2);
      sv0_box_store(_sv0t135, 4, bc.p3);
      sv0_vec_push(out_instrs, _sv0t135);
      Value _sv0t136;
      _sv0t136.tag = 3;
      _sv0t136.p0 = t;
      return _sv0t136;
    } else {
    }
    if ((cppc == 2)) {
      int _sv0t137 = sv0_vec_get(pp, cpps);
      int en_tok = _sv0t137;
      int _sv0t138 = (cpps + 1);
      int _sv0t139 = sv0_vec_get(pp, _sv0t138);
      int vn_tok = _sv0t139;
      int _sv0t140 = enum_tag_lookup_str(enum_names, enum_tags_flat, enum_tag_offsets, enum_tag_counts, en_tok, vn_tok, source, starts, ends);
      int k = _sv0t140;
      int t = ec_t;
      Expr tag_e;
      Value _sv0t141;
      _sv0t141.tag = 0;
      _sv0t141.p0 = k;
      int _sv0t142 = sv0_box_alloc(3);
      sv0_box_store(_sv0t142, 0, _sv0t141.tag);
      sv0_box_store(_sv0t142, 1, _sv0t141.p0);
      sv0_box_store(_sv0t142, 2, _sv0t141.p1);
      tag_e.tag = 0;
      tag_e.p0 = _sv0t142;
      Instr sf_tag;
      int _sv0t143 = (0 - 1);
      int _sv0t144 = sv0_box_alloc(4);
      sv0_box_store(_sv0t144, 0, tag_e.tag);
      sv0_box_store(_sv0t144, 1, tag_e.p0);
      sv0_box_store(_sv0t144, 2, tag_e.p1);
      sv0_box_store(_sv0t144, 3, tag_e.p2);
      sf_tag.tag = 5;
      sf_tag.p0 = t;
      sf_tag.p1 = _sv0t143;
      sf_tag.p2 = _sv0t144;
      int _sv0t145 = sv0_box_alloc(5);
      sv0_box_store(_sv0t145, 0, sf_tag.tag);
      sv0_box_store(_sv0t145, 1, sf_tag.p0);
      sv0_box_store(_sv0t145, 2, sf_tag.p1);
      sv0_box_store(_sv0t145, 3, sf_tag.p2);
      sv0_box_store(_sv0t145, 4, sf_tag.p3);
      sv0_vec_push(out_instrs, _sv0t145);
      int pi = 0;
      while (1) {
        if ((pi >= args_count)) {
          break;
        } else {
        }
        int _sv0t146 = sv0_vec_get(arg_vals, pi);
        Value _sv0t147;
        int _sv0t148 = sv0_box_load(_sv0t146, 0);
        _sv0t147.tag = _sv0t148;
        int _sv0t149 = sv0_box_load(_sv0t146, 1);
        _sv0t147.p0 = _sv0t149;
        int _sv0t150 = sv0_box_load(_sv0t146, 2);
        _sv0t147.p1 = _sv0t150;
        Value pv;
        pv.tag = (_sv0t147).tag;
        pv.p0 = (_sv0t147).p0;
        pv.p1 = (_sv0t147).p1;
        Expr _sv0t151 = value_to_expr(pv);
        Expr pe;
        pe = _sv0t151;
        int _sv0t152 = (0 - 10);
        int field_sentinel = (_sv0t152 - pi);
        Instr sf_p;
        int _sv0t153 = sv0_box_alloc(4);
        sv0_box_store(_sv0t153, 0, pe.tag);
        sv0_box_store(_sv0t153, 1, pe.p0);
        sv0_box_store(_sv0t153, 2, pe.p1);
        sv0_box_store(_sv0t153, 3, pe.p2);
        sf_p.tag = 5;
        sf_p.p0 = t;
        sf_p.p1 = field_sentinel;
        sf_p.p2 = _sv0t153;
        int _sv0t154 = sv0_box_alloc(5);
        sv0_box_store(_sv0t154, 0, sf_p.tag);
        sv0_box_store(_sv0t154, 1, sf_p.p0);
        sv0_box_store(_sv0t154, 2, sf_p.p1);
        sv0_box_store(_sv0t154, 3, sf_p.p2);
        sv0_box_store(_sv0t154, 4, sf_p.p3);
        sv0_vec_push(out_instrs, _sv0t154);
        pi = (pi + 1);
      }
      Value _sv0t155;
      _sv0t155.tag = 3;
      _sv0t155.p0 = t;
      return _sv0t155;
    } else {
    }
  } else {
  }
  Value _sv0t156;
  _sv0t156.tag = 4;
  return _sv0t156;
}

static int block_stmt_index(int pp, int stmts_first, int sidecar, int si) {
  if ((sidecar > 0)) {
    int _sv0t0 = (sidecar - 1);
    int _sv0t1 = (_sv0t0 + si);
    int _sv0t2 = sv0_vec_get(pp, _sv0t1);
    return _sv0t2;
  } else {
  }
  int _sv0t3 = (stmts_first + si);
  return _sv0t3;
}

static Value lower_tag_block(int et, int ed1, int ed2, int ed3, int ed4, int pp, int tok_tags, int idx, int ctr, int out_instrs, int enum_names, int enum_tag_offsets, int enum_tag_counts, int enum_tags_flat, int fn_ctx, int builtin_map, int lit_sf_names, const char* source, int starts, int ends, int item_tags, int item_names, int item_d2, int item_d3, int item_d4, int item_field_counts, int enum_vnames_flat, int enum_max_payloads, int item_fn_row, int match_env_inherit, int import_aliases, int for_effect, int into_dest) {
  int _sv0t0 = sv0_vec_get(ed1, idx);
  int stmts_first = _sv0t0;
  int _sv0t1 = sv0_vec_get(ed2, idx);
  int stmts_count = _sv0t1;
  int _sv0t2 = sv0_vec_get(ed3, idx);
  int tail_idx = _sv0t2;
  int _sv0t3 = sv0_vec_get(ed4, idx);
  int sidecar = _sv0t3;
  int si = 0;
  while (1) {
    if ((si >= stmts_count)) {
      break;
    } else {
    }
    int _sv0t4 = block_stmt_index(pp, stmts_first, sidecar, si);
    int s_idx = _sv0t4;
    int _sv0t5 = sv0_vec_get(et, s_idx);
    int s_tag = _sv0t5;
    if ((s_tag == 27)) {
      int _sv0t6 = sv0_vec_get(ed1, s_idx);
      int name_tok = _sv0t6;
      int _sv0t7 = sv0_vec_get(ed3, s_idx);
      int init_idx = _sv0t7;
      int let_declared = 0;
      int init_handled = 0;
      int deferred_ty = (0 - 1);
      int has_deferred_decl = 0;
      if ((init_idx >= 0)) {
        int _sv0t8 = sv0_vec_get(et, init_idx);
        if ((_sv0t8 == 24)) {
          int _sv0t9 = sv0_vec_get(ed2, init_idx);
          int sppc = _sv0t9;
          int _sv0t10 = sv0_vec_get(ed3, init_idx);
          int fc_lb = _sv0t10;
          if ((sppc >= 1)) {
            if ((fc_lb > 0)) {
              int _sv0t11 = sv0_vec_get(ed1, init_idx);
              int _sv0t12 = (_sv0t11 + sppc);
              int _sv0t13 = (_sv0t12 - 1);
              int _sv0t14 = sv0_vec_get(pp, _sv0t13);
              int sty_tok = _sv0t14;
              Instr _sv0t15;
              _sv0t15.tag = 2;
              _sv0t15.p0 = sty_tok;
              _sv0t15.p1 = name_tok;
              int _sv0t16 = sv0_box_alloc(5);
              sv0_box_store(_sv0t16, 0, _sv0t15.tag);
              sv0_box_store(_sv0t16, 1, _sv0t15.p0);
              sv0_box_store(_sv0t16, 2, _sv0t15.p1);
              sv0_box_store(_sv0t16, 3, _sv0t15.p2);
              sv0_box_store(_sv0t16, 4, _sv0t15.p3);
              sv0_vec_push(out_instrs, _sv0t16);
              let_declared = 1;
              int _sv0t17 = sv0_vec_get(ed4, init_idx);
              int sf0_lb = _sv0t17;
              int fi_lb = 0;
              while ((fi_lb < fc_lb)) {
                int _sv0t18 = (fi_lb * 2);
                int _sv0t19 = (sf0_lb + _sv0t18);
                int _sv0t20 = (_sv0t19 + 1);
                int _sv0t21 = sv0_vec_get(lit_sf_names, _sv0t20);
                int ch_lb = _sv0t21;
                Value _sv0t22 = lower_expr_to_value(et, ed1, ed2, ed3, ed4, pp, tok_tags, ch_lb, ctr, out_instrs, enum_names, enum_tag_offsets, enum_tag_counts, enum_tags_flat, fn_ctx, builtin_map, lit_sf_names, source, starts, ends, item_tags, item_names, item_d2, item_d3, item_d4, item_field_counts, enum_vnames_flat, enum_max_payloads, item_fn_row, match_env_inherit, import_aliases);
                Value fv_lb;
                fv_lb = _sv0t22;
                int _sv0t23 = (fi_lb * 2);
                int _sv0t24 = (sf0_lb + _sv0t23);
                int _sv0t25 = sv0_vec_get(lit_sf_names, _sv0t24);
                int fnm_lb = _sv0t25;
                Instr _sv0t26;
                Expr _sv0t27 = value_to_expr(fv_lb);
                int _sv0t28 = sv0_box_alloc(4);
                sv0_box_store(_sv0t28, 0, _sv0t27.tag);
                sv0_box_store(_sv0t28, 1, _sv0t27.p0);
                sv0_box_store(_sv0t28, 2, _sv0t27.p1);
                sv0_box_store(_sv0t28, 3, _sv0t27.p2);
                _sv0t26.tag = 5;
                _sv0t26.p0 = name_tok;
                _sv0t26.p1 = fnm_lb;
                _sv0t26.p2 = _sv0t28;
                int _sv0t29 = sv0_box_alloc(5);
                sv0_box_store(_sv0t29, 0, _sv0t26.tag);
                sv0_box_store(_sv0t29, 1, _sv0t26.p0);
                sv0_box_store(_sv0t29, 2, _sv0t26.p1);
                sv0_box_store(_sv0t29, 3, _sv0t26.p2);
                sv0_box_store(_sv0t29, 4, _sv0t26.p3);
                sv0_vec_push(out_instrs, _sv0t29);
                fi_lb = (fi_lb + 1);
              }
              init_handled = 1;
            } else {
            }
          } else {
          }
        } else {
        }
        int _sv0t30 = sv0_vec_get(et, init_idx);
        if ((_sv0t30 == 1)) {
          int _sv0t31 = sv0_vec_get(ed2, init_idx);
          if ((_sv0t31 == 2)) {
            int _sv0t32 = sv0_vec_get(ed1, init_idx);
            int _sv0t33 = sv0_vec_get(pp, _sv0t32);
            int ety_tok = _sv0t33;
            int _sv0t34 = sv0_vec_get(ed1, init_idx);
            int _sv0t35 = (_sv0t34 + 1);
            int _sv0t36 = sv0_vec_get(pp, _sv0t35);
            int vn_tok_lb = _sv0t36;
            Instr _sv0t37;
            _sv0t37.tag = 2;
            _sv0t37.p0 = ety_tok;
            _sv0t37.p1 = name_tok;
            int _sv0t38 = sv0_box_alloc(5);
            sv0_box_store(_sv0t38, 0, _sv0t37.tag);
            sv0_box_store(_sv0t38, 1, _sv0t37.p0);
            sv0_box_store(_sv0t38, 2, _sv0t37.p1);
            sv0_box_store(_sv0t38, 3, _sv0t37.p2);
            sv0_box_store(_sv0t38, 4, _sv0t37.p3);
            sv0_vec_push(out_instrs, _sv0t38);
            int _sv0t39 = enum_tag_lookup_str(enum_names, enum_tags_flat, enum_tag_offsets, enum_tag_counts, ety_tok, vn_tok_lb, source, starts, ends);
            int k_lb = _sv0t39;
            Expr tag_elb;
            Value _sv0t40;
            _sv0t40.tag = 0;
            _sv0t40.p0 = k_lb;
            int _sv0t41 = sv0_box_alloc(3);
            sv0_box_store(_sv0t41, 0, _sv0t40.tag);
            sv0_box_store(_sv0t41, 1, _sv0t40.p0);
            sv0_box_store(_sv0t41, 2, _sv0t40.p1);
            tag_elb.tag = 0;
            tag_elb.p0 = _sv0t41;
            Instr _sv0t42;
            int _sv0t43 = (0 - 1);
            int _sv0t44 = sv0_box_alloc(4);
            sv0_box_store(_sv0t44, 0, tag_elb.tag);
            sv0_box_store(_sv0t44, 1, tag_elb.p0);
            sv0_box_store(_sv0t44, 2, tag_elb.p1);
            sv0_box_store(_sv0t44, 3, tag_elb.p2);
            _sv0t42.tag = 5;
            _sv0t42.p0 = name_tok;
            _sv0t42.p1 = _sv0t43;
            _sv0t42.p2 = _sv0t44;
            int _sv0t45 = sv0_box_alloc(5);
            sv0_box_store(_sv0t45, 0, _sv0t42.tag);
            sv0_box_store(_sv0t45, 1, _sv0t42.p0);
            sv0_box_store(_sv0t45, 2, _sv0t42.p1);
            sv0_box_store(_sv0t45, 3, _sv0t42.p2);
            sv0_box_store(_sv0t45, 4, _sv0t42.p3);
            sv0_vec_push(out_instrs, _sv0t45);
            let_declared = 1;
            init_handled = 1;
          } else {
          }
        } else {
        }
        int _sv0t46 = sv0_vec_get(et, init_idx);
        if ((_sv0t46 == 4)) {
          int _sv0t47 = sv0_vec_get(ed1, init_idx);
          int callee_cb = _sv0t47;
          int _sv0t48 = sv0_vec_get(et, callee_cb);
          if ((_sv0t48 == 1)) {
            int _sv0t49 = sv0_vec_get(ed2, callee_cb);
            if ((_sv0t49 == 2)) {
              int _sv0t50 = sv0_vec_get(ed1, callee_cb);
              int _sv0t51 = sv0_vec_get(pp, _sv0t50);
              int en_tok_lb = _sv0t51;
              int _sv0t52 = sv0_vec_get(ed1, callee_cb);
              int _sv0t53 = (_sv0t52 + 1);
              int _sv0t54 = sv0_vec_get(pp, _sv0t53);
              int vnc_lb = _sv0t54;
              Instr _sv0t55;
              _sv0t55.tag = 2;
              _sv0t55.p0 = en_tok_lb;
              _sv0t55.p1 = name_tok;
              int _sv0t56 = sv0_box_alloc(5);
              sv0_box_store(_sv0t56, 0, _sv0t55.tag);
              sv0_box_store(_sv0t56, 1, _sv0t55.p0);
              sv0_box_store(_sv0t56, 2, _sv0t55.p1);
              sv0_box_store(_sv0t56, 3, _sv0t55.p2);
              sv0_box_store(_sv0t56, 4, _sv0t55.p3);
              sv0_vec_push(out_instrs, _sv0t56);
              int _sv0t57 = enum_tag_lookup_str(enum_names, enum_tags_flat, enum_tag_offsets, enum_tag_counts, en_tok_lb, vnc_lb, source, starts, ends);
              int kc_lb = _sv0t57;
              int _sv0t58 = sv0_vec_get(ed2, init_idx);
              int af_lb = _sv0t58;
              int _sv0t59 = sv0_vec_get(ed3, init_idx);
              int ac_lb = _sv0t59;
              int _sv0t60 = sv0_vec_get(ed4, init_idx);
              int asc_lb = _sv0t60;
              int _sv0t61 = sv0_vec_new();
              int avals_lb = _sv0t61;
              int aic_lb = 0;
              while ((aic_lb < ac_lb)) {
                int _sv0t62 = block_stmt_index(pp, af_lb, asc_lb, aic_lb);
                int aidx_lb = _sv0t62;
                Value _sv0t63 = lower_expr_to_value(et, ed1, ed2, ed3, ed4, pp, tok_tags, aidx_lb, ctr, out_instrs, enum_names, enum_tag_offsets, enum_tag_counts, enum_tags_flat, fn_ctx, builtin_map, lit_sf_names, source, starts, ends, item_tags, item_names, item_d2, item_d3, item_d4, item_field_counts, enum_vnames_flat, enum_max_payloads, item_fn_row, match_env_inherit, import_aliases);
                Value avc_lb;
                avc_lb = _sv0t63;
                int _sv0t64 = sv0_box_alloc(3);
                sv0_box_store(_sv0t64, 0, avc_lb.tag);
                sv0_box_store(_sv0t64, 1, avc_lb.p0);
                sv0_box_store(_sv0t64, 2, avc_lb.p1);
                sv0_vec_push(avals_lb, _sv0t64);
                aic_lb = (aic_lb + 1);
              }
              Expr tagc_lb;
              Value _sv0t65;
              _sv0t65.tag = 0;
              _sv0t65.p0 = kc_lb;
              int _sv0t66 = sv0_box_alloc(3);
              sv0_box_store(_sv0t66, 0, _sv0t65.tag);
              sv0_box_store(_sv0t66, 1, _sv0t65.p0);
              sv0_box_store(_sv0t66, 2, _sv0t65.p1);
              tagc_lb.tag = 0;
              tagc_lb.p0 = _sv0t66;
              Instr _sv0t67;
              int _sv0t68 = (0 - 1);
              int _sv0t69 = sv0_box_alloc(4);
              sv0_box_store(_sv0t69, 0, tagc_lb.tag);
              sv0_box_store(_sv0t69, 1, tagc_lb.p0);
              sv0_box_store(_sv0t69, 2, tagc_lb.p1);
              sv0_box_store(_sv0t69, 3, tagc_lb.p2);
              _sv0t67.tag = 5;
              _sv0t67.p0 = name_tok;
              _sv0t67.p1 = _sv0t68;
              _sv0t67.p2 = _sv0t69;
              int _sv0t70 = sv0_box_alloc(5);
              sv0_box_store(_sv0t70, 0, _sv0t67.tag);
              sv0_box_store(_sv0t70, 1, _sv0t67.p0);
              sv0_box_store(_sv0t70, 2, _sv0t67.p1);
              sv0_box_store(_sv0t70, 3, _sv0t67.p2);
              sv0_box_store(_sv0t70, 4, _sv0t67.p3);
              sv0_vec_push(out_instrs, _sv0t70);
              int pic_lb = 0;
              while ((pic_lb < ac_lb)) {
                int _sv0t71 = sv0_vec_get(avals_lb, pic_lb);
                Value _sv0t72;
                int _sv0t73 = sv0_box_load(_sv0t71, 0);
                _sv0t72.tag = _sv0t73;
                int _sv0t74 = sv0_box_load(_sv0t71, 1);
                _sv0t72.p0 = _sv0t74;
                int _sv0t75 = sv0_box_load(_sv0t71, 2);
                _sv0t72.p1 = _sv0t75;
                Value pvc_lb;
                pvc_lb.tag = (_sv0t72).tag;
                pvc_lb.p0 = (_sv0t72).p0;
                pvc_lb.p1 = (_sv0t72).p1;
                int _sv0t76 = (0 - 10);
                int fsc_lb = (_sv0t76 - pic_lb);
                Instr _sv0t77;
                Expr _sv0t78 = value_to_expr(pvc_lb);
                int _sv0t79 = sv0_box_alloc(4);
                sv0_box_store(_sv0t79, 0, _sv0t78.tag);
                sv0_box_store(_sv0t79, 1, _sv0t78.p0);
                sv0_box_store(_sv0t79, 2, _sv0t78.p1);
                sv0_box_store(_sv0t79, 3, _sv0t78.p2);
                _sv0t77.tag = 5;
                _sv0t77.p0 = name_tok;
                _sv0t77.p1 = fsc_lb;
                _sv0t77.p2 = _sv0t79;
                int _sv0t80 = sv0_box_alloc(5);
                sv0_box_store(_sv0t80, 0, _sv0t77.tag);
                sv0_box_store(_sv0t80, 1, _sv0t77.p0);
                sv0_box_store(_sv0t80, 2, _sv0t77.p1);
                sv0_box_store(_sv0t80, 3, _sv0t77.p2);
                sv0_box_store(_sv0t80, 4, _sv0t77.p3);
                sv0_vec_push(out_instrs, _sv0t80);
                pic_lb = (pic_lb + 1);
              }
              let_declared = 1;
              init_handled = 1;
            } else {
            }
          } else {
          }
        } else {
        }
        int _sv0t81 = lower_expr_yields_string(et, ed1, ed2, ed3, pp, init_idx, item_tags, item_names, item_d2, tok_tags, source, starts, ends);
        if (_sv0t81) {
          int _sv0t82 = lower_string_cty();
          deferred_ty = _sv0t82;
          has_deferred_decl = 1;
          let_declared = 1;
        } else {
        }
        if ((let_declared == 0)) {
          int _sv0t83 = lower_call_ret_is_string(et, ed1, ed2, pp, init_idx, item_tags, item_names, item_d2, tok_tags, source, starts, ends);
          if (_sv0t83) {
            Instr decl_scb;
            int _sv0t84 = lower_string_cty();
            decl_scb.tag = 2;
            decl_scb.p0 = _sv0t84;
            decl_scb.p1 = name_tok;
            int _sv0t85 = sv0_box_alloc(5);
            sv0_box_store(_sv0t85, 0, decl_scb.tag);
            sv0_box_store(_sv0t85, 1, decl_scb.p0);
            sv0_box_store(_sv0t85, 2, decl_scb.p1);
            sv0_box_store(_sv0t85, 3, decl_scb.p2);
            sv0_box_store(_sv0t85, 4, decl_scb.p3);
            sv0_vec_push(out_instrs, _sv0t85);
            let_declared = 1;
          } else {
          }
        } else {
        }
        if ((let_declared == 0)) {
          int _sv0t86 = lower_init_ident_is_string_param(et, ed1, ed2, pp, init_idx, item_tags, item_names, item_d3, tok_tags, source, starts, ends, item_fn_row);
          if (_sv0t86) {
            Instr decl_spb;
            int _sv0t87 = lower_string_cty();
            decl_spb.tag = 2;
            decl_spb.p0 = _sv0t87;
            decl_spb.p1 = name_tok;
            int _sv0t88 = sv0_box_alloc(5);
            sv0_box_store(_sv0t88, 0, decl_spb.tag);
            sv0_box_store(_sv0t88, 1, decl_spb.p0);
            sv0_box_store(_sv0t88, 2, decl_spb.p1);
            sv0_box_store(_sv0t88, 3, decl_spb.p2);
            sv0_box_store(_sv0t88, 4, decl_spb.p3);
            sv0_vec_push(out_instrs, _sv0t88);
            let_declared = 1;
          } else {
          }
        } else {
        }
        if ((let_declared == 0)) {
          int _sv0t89 = lower_call_ret_user_ty_tok(et, ed1, ed2, pp, init_idx, item_tags, item_names, item_d2, tok_tags, source, starts, ends);
          int uty_lb = _sv0t89;
          if ((uty_lb >= 0)) {
            deferred_ty = uty_lb;
            has_deferred_decl = 1;
            let_declared = 1;
          } else {
          }
        } else {
        }
        if ((let_declared == 0)) {
          int _sv0t90 = sv0_vec_get(ed2, s_idx);
          int _sv0t91 = lower_typed_box_deref_let(et, ed1, ed2, ed3, ed4, pp, tok_tags, init_idx, name_tok, _sv0t90, ctr, out_instrs, enum_names, enum_tag_offsets, enum_tag_counts, enum_tags_flat, fn_ctx, builtin_map, lit_sf_names, source, starts, ends, item_tags, item_names, item_d2, item_d3, item_d4, item_field_counts, enum_vnames_flat, enum_max_payloads, item_fn_row, match_env_inherit, import_aliases);
          int bdb = _sv0t91;
          if ((bdb == 1)) {
            let_declared = 1;
            init_handled = 1;
          } else {
          }
        } else {
        }
      } else {
      }
      if ((init_idx >= 0)) {
        if ((init_handled == 0)) {
          Expr _sv0t92 = lower_expr_with_instrs(et, ed1, ed2, ed3, ed4, pp, tok_tags, init_idx, ctr, out_instrs, enum_names, enum_tag_offsets, enum_tag_counts, enum_tags_flat, fn_ctx, builtin_map, lit_sf_names, source, starts, ends, item_tags, item_names, item_d2, item_d3, item_d4, item_field_counts, enum_vnames_flat, enum_max_payloads, item_fn_row, match_env_inherit, import_aliases);
          Expr e;
          e = _sv0t92;
          if ((has_deferred_decl == 1)) {
            Instr _sv0t93;
            _sv0t93.tag = 2;
            _sv0t93.p0 = deferred_ty;
            _sv0t93.p1 = name_tok;
            int _sv0t94 = sv0_box_alloc(5);
            sv0_box_store(_sv0t94, 0, _sv0t93.tag);
            sv0_box_store(_sv0t94, 1, _sv0t93.p0);
            sv0_box_store(_sv0t94, 2, _sv0t93.p1);
            sv0_box_store(_sv0t94, 3, _sv0t93.p2);
            sv0_box_store(_sv0t94, 4, _sv0t93.p3);
            sv0_vec_push(out_instrs, _sv0t94);
          } else {
            if ((let_declared == 0)) {
              Instr _sv0t95;
              _sv0t95.tag = 1;
              _sv0t95.p0 = name_tok;
              int _sv0t96 = sv0_box_alloc(5);
              sv0_box_store(_sv0t96, 0, _sv0t95.tag);
              sv0_box_store(_sv0t96, 1, _sv0t95.p0);
              sv0_box_store(_sv0t96, 2, _sv0t95.p1);
              sv0_box_store(_sv0t96, 3, _sv0t95.p2);
              sv0_box_store(_sv0t96, 4, _sv0t95.p3);
              sv0_vec_push(out_instrs, _sv0t96);
            } else {
            }
          }
          Instr asgn;
          int _sv0t97 = sv0_box_alloc(4);
          sv0_box_store(_sv0t97, 0, e.tag);
          sv0_box_store(_sv0t97, 1, e.p0);
          sv0_box_store(_sv0t97, 2, e.p1);
          sv0_box_store(_sv0t97, 3, e.p2);
          asgn.tag = 4;
          asgn.p0 = name_tok;
          asgn.p1 = _sv0t97;
          int _sv0t98 = sv0_box_alloc(5);
          sv0_box_store(_sv0t98, 0, asgn.tag);
          sv0_box_store(_sv0t98, 1, asgn.p0);
          sv0_box_store(_sv0t98, 2, asgn.p1);
          sv0_box_store(_sv0t98, 3, asgn.p2);
          sv0_box_store(_sv0t98, 4, asgn.p3);
          sv0_vec_push(out_instrs, _sv0t98);
        } else {
        }
      } else {
        if ((let_declared == 0)) {
          Instr _sv0t99;
          _sv0t99.tag = 1;
          _sv0t99.p0 = name_tok;
          int _sv0t100 = sv0_box_alloc(5);
          sv0_box_store(_sv0t100, 0, _sv0t99.tag);
          sv0_box_store(_sv0t100, 1, _sv0t99.p0);
          sv0_box_store(_sv0t100, 2, _sv0t99.p1);
          sv0_box_store(_sv0t100, 3, _sv0t99.p2);
          sv0_box_store(_sv0t100, 4, _sv0t99.p3);
          sv0_vec_push(out_instrs, _sv0t100);
        } else {
        }
      }
    } else {
    }
    if ((s_tag == 28)) {
      int _sv0t101 = sv0_vec_get(ed1, s_idx);
      int semi_idx = _sv0t101;
      int _sv0t102 = lower_for_effect(et, ed1, ed2, ed3, ed4, pp, tok_tags, semi_idx, ctr, out_instrs, enum_names, enum_tag_offsets, enum_tag_counts, enum_tags_flat, fn_ctx, builtin_map, lit_sf_names, source, starts, ends, item_tags, item_names, item_d2, item_d3, item_d4, item_field_counts, enum_vnames_flat, enum_max_payloads, item_fn_row, match_env_inherit, import_aliases);
      int de1 = _sv0t102;
    } else {
    }
    si = (si + 1);
  }
  if ((into_dest >= 0)) {
    if ((tail_idx >= 0)) {
      int _sv0t103 = lower_into_var_instrs(et, ed1, ed2, ed3, ed4, pp, tok_tags, tail_idx, into_dest, ctr, out_instrs, enum_names, enum_tag_offsets, enum_tag_counts, enum_tags_flat, fn_ctx, builtin_map, lit_sf_names, source, starts, ends, item_tags, item_names, item_d2, item_d3, item_d4, item_field_counts, enum_vnames_flat, enum_max_payloads, item_fn_row, match_env_inherit, import_aliases);
    } else {
      Instr _sv0t104;
      Expr _sv0t105;
      Value _sv0t106;
      _sv0t106.tag = 4;
      int _sv0t107 = sv0_box_alloc(3);
      sv0_box_store(_sv0t107, 0, _sv0t106.tag);
      sv0_box_store(_sv0t107, 1, _sv0t106.p0);
      sv0_box_store(_sv0t107, 2, _sv0t106.p1);
      _sv0t105.tag = 0;
      _sv0t105.p0 = _sv0t107;
      int _sv0t108 = sv0_box_alloc(4);
      sv0_box_store(_sv0t108, 0, _sv0t105.tag);
      sv0_box_store(_sv0t108, 1, _sv0t105.p0);
      sv0_box_store(_sv0t108, 2, _sv0t105.p1);
      sv0_box_store(_sv0t108, 3, _sv0t105.p2);
      _sv0t104.tag = 4;
      _sv0t104.p0 = into_dest;
      _sv0t104.p1 = _sv0t108;
      int _sv0t109 = sv0_box_alloc(5);
      sv0_box_store(_sv0t109, 0, _sv0t104.tag);
      sv0_box_store(_sv0t109, 1, _sv0t104.p0);
      sv0_box_store(_sv0t109, 2, _sv0t104.p1);
      sv0_box_store(_sv0t109, 3, _sv0t104.p2);
      sv0_box_store(_sv0t109, 4, _sv0t104.p3);
      sv0_vec_push(out_instrs, _sv0t109);
    }
    Value _sv0t110;
    _sv0t110.tag = 4;
    return _sv0t110;
  } else {
  }
  if ((tail_idx >= 0)) {
    if ((for_effect == 1)) {
      int _sv0t111 = lower_for_effect(et, ed1, ed2, ed3, ed4, pp, tok_tags, tail_idx, ctr, out_instrs, enum_names, enum_tag_offsets, enum_tag_counts, enum_tags_flat, fn_ctx, builtin_map, lit_sf_names, source, starts, ends, item_tags, item_names, item_d2, item_d3, item_d4, item_field_counts, enum_vnames_flat, enum_max_payloads, item_fn_row, match_env_inherit, import_aliases);
      Value _sv0t112;
      _sv0t112.tag = 4;
      return _sv0t112;
    } else {
    }
    Value _sv0t113 = lower_expr_to_value(et, ed1, ed2, ed3, ed4, pp, tok_tags, tail_idx, ctr, out_instrs, enum_names, enum_tag_offsets, enum_tag_counts, enum_tags_flat, fn_ctx, builtin_map, lit_sf_names, source, starts, ends, item_tags, item_names, item_d2, item_d3, item_d4, item_field_counts, enum_vnames_flat, enum_max_payloads, item_fn_row, match_env_inherit, import_aliases);
    return _sv0t113;
  } else {
  }
  Value _sv0t114;
  _sv0t114.tag = 4;
  return _sv0t114;
}

static Value lower_tag_if(int et, int ed1, int ed2, int ed3, int ed4, int pp, int tok_tags, int idx, int ctr, int out_instrs, int enum_names, int enum_tag_offsets, int enum_tag_counts, int enum_tags_flat, int fn_ctx, int builtin_map, int lit_sf_names, const char* source, int starts, int ends, int item_tags, int item_names, int item_d2, int item_d3, int item_d4, int item_field_counts, int enum_vnames_flat, int enum_max_payloads, int item_fn_row, int match_env_inherit, int import_aliases) {
  int _sv0t0 = sv0_vec_get(ed1, idx);
  int cond_idx = _sv0t0;
  int _sv0t1 = sv0_vec_get(ed2, idx);
  int then_idx = _sv0t1;
  int _sv0t2 = sv0_vec_get(ed3, idx);
  int else_idx = _sv0t2;
  Expr _sv0t3 = lower_cond_expr(et, ed1, ed2, ed3, ed4, pp, tok_tags, cond_idx, ctr, out_instrs, enum_names, enum_tag_offsets, enum_tag_counts, enum_tags_flat, fn_ctx, builtin_map, lit_sf_names, source, starts, ends, item_tags, item_names, item_d2, item_d3, item_d4, item_field_counts, enum_vnames_flat, enum_max_payloads, item_fn_row, match_env_inherit, import_aliases);
  Expr cond_expr;
  cond_expr = _sv0t3;
  if ((else_idx >= 0)) {
    int _sv0t4 = ctr_fresh(ctr);
    int t = _sv0t4;
    int _sv0t5 = ctr_fresh(ctr);
    int u = _sv0t5;
    int _sv0t6 = sv0_vec_new();
    int then_instrs = _sv0t6;
    Value _sv0t7 = lower_expr_to_value(et, ed1, ed2, ed3, ed4, pp, tok_tags, then_idx, ctr, then_instrs, enum_names, enum_tag_offsets, enum_tag_counts, enum_tags_flat, fn_ctx, builtin_map, lit_sf_names, source, starts, ends, item_tags, item_names, item_d2, item_d3, item_d4, item_field_counts, enum_vnames_flat, enum_max_payloads, item_fn_row, match_env_inherit, import_aliases);
    Value vt;
    vt = _sv0t7;
    Expr t_lit;
    int _sv0t8 = sv0_box_alloc(3);
    sv0_box_store(_sv0t8, 0, vt.tag);
    sv0_box_store(_sv0t8, 1, vt.p0);
    sv0_box_store(_sv0t8, 2, vt.p1);
    t_lit.tag = 0;
    t_lit.p0 = _sv0t8;
    Instr t_store;
    int _sv0t9 = sv0_box_alloc(4);
    sv0_box_store(_sv0t9, 0, t_lit.tag);
    sv0_box_store(_sv0t9, 1, t_lit.p0);
    sv0_box_store(_sv0t9, 2, t_lit.p1);
    sv0_box_store(_sv0t9, 3, t_lit.p2);
    t_store.tag = 4;
    t_store.p0 = u;
    t_store.p1 = _sv0t9;
    int _sv0t10 = sv0_box_alloc(5);
    sv0_box_store(_sv0t10, 0, t_store.tag);
    sv0_box_store(_sv0t10, 1, t_store.p0);
    sv0_box_store(_sv0t10, 2, t_store.p1);
    sv0_box_store(_sv0t10, 3, t_store.p2);
    sv0_box_store(_sv0t10, 4, t_store.p3);
    sv0_vec_push(then_instrs, _sv0t10);
    int _sv0t11 = sv0_vec_new();
    int else_instrs = _sv0t11;
    Value _sv0t12 = lower_expr_to_value(et, ed1, ed2, ed3, ed4, pp, tok_tags, else_idx, ctr, else_instrs, enum_names, enum_tag_offsets, enum_tag_counts, enum_tags_flat, fn_ctx, builtin_map, lit_sf_names, source, starts, ends, item_tags, item_names, item_d2, item_d3, item_d4, item_field_counts, enum_vnames_flat, enum_max_payloads, item_fn_row, match_env_inherit, import_aliases);
    Value ve;
    ve = _sv0t12;
    Expr e_lit;
    int _sv0t13 = sv0_box_alloc(3);
    sv0_box_store(_sv0t13, 0, ve.tag);
    sv0_box_store(_sv0t13, 1, ve.p0);
    sv0_box_store(_sv0t13, 2, ve.p1);
    e_lit.tag = 0;
    e_lit.p0 = _sv0t13;
    Instr e_store;
    int _sv0t14 = sv0_box_alloc(4);
    sv0_box_store(_sv0t14, 0, e_lit.tag);
    sv0_box_store(_sv0t14, 1, e_lit.p0);
    sv0_box_store(_sv0t14, 2, e_lit.p1);
    sv0_box_store(_sv0t14, 3, e_lit.p2);
    e_store.tag = 4;
    e_store.p0 = u;
    e_store.p1 = _sv0t14;
    int _sv0t15 = sv0_box_alloc(5);
    sv0_box_store(_sv0t15, 0, e_store.tag);
    sv0_box_store(_sv0t15, 1, e_store.p0);
    sv0_box_store(_sv0t15, 2, e_store.p1);
    sv0_box_store(_sv0t15, 3, e_store.p2);
    sv0_box_store(_sv0t15, 4, e_store.p3);
    sv0_vec_push(else_instrs, _sv0t15);
    int if_is_str = 0;
    int _sv0t16 = lower_expr_yields_string(et, ed1, ed2, ed3, pp, then_idx, item_tags, item_names, item_d2, tok_tags, source, starts, ends);
    if (_sv0t16) {
      if_is_str = 1;
    } else {
    }
    int _sv0t17 = lower_expr_yields_string(et, ed1, ed2, ed3, pp, else_idx, item_tags, item_names, item_d2, tok_tags, source, starts, ends);
    if (_sv0t17) {
      if_is_str = 1;
    } else {
    }
    if ((if_is_str == 1)) {
      Instr _sv0t18;
      int _sv0t19 = lower_string_cty();
      _sv0t18.tag = 2;
      _sv0t18.p0 = _sv0t19;
      _sv0t18.p1 = t;
      int _sv0t20 = sv0_box_alloc(5);
      sv0_box_store(_sv0t20, 0, _sv0t18.tag);
      sv0_box_store(_sv0t20, 1, _sv0t18.p0);
      sv0_box_store(_sv0t20, 2, _sv0t18.p1);
      sv0_box_store(_sv0t20, 3, _sv0t18.p2);
      sv0_box_store(_sv0t20, 4, _sv0t18.p3);
      sv0_vec_push(out_instrs, _sv0t20);
      Instr _sv0t21;
      int _sv0t22 = lower_string_cty();
      _sv0t21.tag = 2;
      _sv0t21.p0 = _sv0t22;
      _sv0t21.p1 = u;
      int _sv0t23 = sv0_box_alloc(5);
      sv0_box_store(_sv0t23, 0, _sv0t21.tag);
      sv0_box_store(_sv0t23, 1, _sv0t21.p0);
      sv0_box_store(_sv0t23, 2, _sv0t21.p1);
      sv0_box_store(_sv0t23, 3, _sv0t21.p2);
      sv0_box_store(_sv0t23, 4, _sv0t21.p3);
      sv0_vec_push(out_instrs, _sv0t23);
    } else {
      Instr _sv0t24;
      _sv0t24.tag = 1;
      _sv0t24.p0 = t;
      int _sv0t25 = sv0_box_alloc(5);
      sv0_box_store(_sv0t25, 0, _sv0t24.tag);
      sv0_box_store(_sv0t25, 1, _sv0t24.p0);
      sv0_box_store(_sv0t25, 2, _sv0t24.p1);
      sv0_box_store(_sv0t25, 3, _sv0t24.p2);
      sv0_box_store(_sv0t25, 4, _sv0t24.p3);
      sv0_vec_push(out_instrs, _sv0t25);
      Instr _sv0t26;
      _sv0t26.tag = 1;
      _sv0t26.p0 = u;
      int _sv0t27 = sv0_box_alloc(5);
      sv0_box_store(_sv0t27, 0, _sv0t26.tag);
      sv0_box_store(_sv0t27, 1, _sv0t26.p0);
      sv0_box_store(_sv0t27, 2, _sv0t26.p1);
      sv0_box_store(_sv0t27, 3, _sv0t26.p2);
      sv0_box_store(_sv0t27, 4, _sv0t26.p3);
      sv0_vec_push(out_instrs, _sv0t27);
    }
    Instr if_instr;
    int _sv0t28 = sv0_box_alloc(4);
    sv0_box_store(_sv0t28, 0, cond_expr.tag);
    sv0_box_store(_sv0t28, 1, cond_expr.p0);
    sv0_box_store(_sv0t28, 2, cond_expr.p1);
    sv0_box_store(_sv0t28, 3, cond_expr.p2);
    if_instr.tag = 6;
    if_instr.p0 = _sv0t28;
    if_instr.p1 = then_instrs;
    if_instr.p2 = else_instrs;
    int _sv0t29 = sv0_box_alloc(5);
    sv0_box_store(_sv0t29, 0, if_instr.tag);
    sv0_box_store(_sv0t29, 1, if_instr.p0);
    sv0_box_store(_sv0t29, 2, if_instr.p1);
    sv0_box_store(_sv0t29, 3, if_instr.p2);
    sv0_box_store(_sv0t29, 4, if_instr.p3);
    sv0_vec_push(out_instrs, _sv0t29);
    Expr load_u;
    load_u.tag = 1;
    load_u.p0 = u;
    Instr final_store;
    int _sv0t30 = sv0_box_alloc(4);
    sv0_box_store(_sv0t30, 0, load_u.tag);
    sv0_box_store(_sv0t30, 1, load_u.p0);
    sv0_box_store(_sv0t30, 2, load_u.p1);
    sv0_box_store(_sv0t30, 3, load_u.p2);
    final_store.tag = 4;
    final_store.p0 = t;
    final_store.p1 = _sv0t30;
    int _sv0t31 = sv0_box_alloc(5);
    sv0_box_store(_sv0t31, 0, final_store.tag);
    sv0_box_store(_sv0t31, 1, final_store.p0);
    sv0_box_store(_sv0t31, 2, final_store.p1);
    sv0_box_store(_sv0t31, 3, final_store.p2);
    sv0_box_store(_sv0t31, 4, final_store.p3);
    sv0_vec_push(out_instrs, _sv0t31);
    Value _sv0t32;
    _sv0t32.tag = 3;
    _sv0t32.p0 = t;
    return _sv0t32;
  } else {
  }
  int _sv0t33 = sv0_vec_new();
  int then_only = _sv0t33;
  Value _sv0t34 = lower_expr_to_value(et, ed1, ed2, ed3, ed4, pp, tok_tags, then_idx, ctr, then_only, enum_names, enum_tag_offsets, enum_tag_counts, enum_tags_flat, fn_ctx, builtin_map, lit_sf_names, source, starts, ends, item_tags, item_names, item_d2, item_d3, item_d4, item_field_counts, enum_vnames_flat, enum_max_payloads, item_fn_row, match_env_inherit, import_aliases);
  Value discard2;
  discard2 = _sv0t34;
  int _sv0t35 = sv0_vec_new();
  int no_else = _sv0t35;
  Instr if_no_el;
  int _sv0t36 = sv0_box_alloc(4);
  sv0_box_store(_sv0t36, 0, cond_expr.tag);
  sv0_box_store(_sv0t36, 1, cond_expr.p0);
  sv0_box_store(_sv0t36, 2, cond_expr.p1);
  sv0_box_store(_sv0t36, 3, cond_expr.p2);
  if_no_el.tag = 6;
  if_no_el.p0 = _sv0t36;
  if_no_el.p1 = then_only;
  if_no_el.p2 = no_else;
  int _sv0t37 = sv0_box_alloc(5);
  sv0_box_store(_sv0t37, 0, if_no_el.tag);
  sv0_box_store(_sv0t37, 1, if_no_el.p0);
  sv0_box_store(_sv0t37, 2, if_no_el.p1);
  sv0_box_store(_sv0t37, 3, if_no_el.p2);
  sv0_box_store(_sv0t37, 4, if_no_el.p3);
  sv0_vec_push(out_instrs, _sv0t37);
  Value _sv0t38;
  _sv0t38.tag = 4;
  return _sv0t38;
}

static Value lower_tag_for(int et, int ed1, int ed2, int ed3, int ed4, int pp, int tok_tags, int idx, int ctr, int out_instrs, int enum_names, int enum_tag_offsets, int enum_tag_counts, int enum_tags_flat, int fn_ctx, int builtin_map, int lit_sf_names, const char* source, int starts, int ends, int item_tags, int item_names, int item_d2, int item_d3, int item_d4, int item_field_counts, int enum_vnames_flat, int enum_max_payloads, int item_fn_row, int match_env_inherit, int import_aliases) {
  int _sv0t0 = sv0_vec_get(ed1, idx);
  int pat_tok = _sv0t0;
  int _sv0t1 = sv0_vec_get(ed2, idx);
  int range_idx = _sv0t1;
  int _sv0t2 = sv0_vec_get(ed3, idx);
  int for_body = _sv0t2;
  int _sv0t3 = sv0_vec_get(et, range_idx);
  int range_tag = _sv0t3;
  if ((range_tag == 21)) {
    int _sv0t4 = sv0_vec_get(ed1, range_idx);
    int lo_idx = _sv0t4;
    int _sv0t5 = sv0_vec_get(ed2, range_idx);
    int hi_idx = _sv0t5;
    int _sv0t6 = ctr_fresh(ctr);
    int hi_t = _sv0t6;
    int _sv0t7 = ctr_fresh(ctr);
    int i_t = _sv0t7;
    Value _sv0t8 = lower_expr_to_value(et, ed1, ed2, ed3, ed4, pp, tok_tags, hi_idx, ctr, out_instrs, enum_names, enum_tag_offsets, enum_tag_counts, enum_tags_flat, fn_ctx, builtin_map, lit_sf_names, source, starts, ends, item_tags, item_names, item_d2, item_d3, item_d4, item_field_counts, enum_vnames_flat, enum_max_payloads, item_fn_row, match_env_inherit, import_aliases);
    Value v_hi;
    v_hi = _sv0t8;
    Expr _sv0t9 = value_to_expr(v_hi);
    Expr e_hi;
    e_hi = _sv0t9;
    Instr assign_hi;
    int _sv0t10 = sv0_box_alloc(4);
    sv0_box_store(_sv0t10, 0, e_hi.tag);
    sv0_box_store(_sv0t10, 1, e_hi.p0);
    sv0_box_store(_sv0t10, 2, e_hi.p1);
    sv0_box_store(_sv0t10, 3, e_hi.p2);
    assign_hi.tag = 3;
    assign_hi.p0 = hi_t;
    assign_hi.p1 = _sv0t10;
    int _sv0t11 = sv0_box_alloc(5);
    sv0_box_store(_sv0t11, 0, assign_hi.tag);
    sv0_box_store(_sv0t11, 1, assign_hi.p0);
    sv0_box_store(_sv0t11, 2, assign_hi.p1);
    sv0_box_store(_sv0t11, 3, assign_hi.p2);
    sv0_box_store(_sv0t11, 4, assign_hi.p3);
    sv0_vec_push(out_instrs, _sv0t11);
    Value _sv0t12 = lower_expr_to_value(et, ed1, ed2, ed3, ed4, pp, tok_tags, lo_idx, ctr, out_instrs, enum_names, enum_tag_offsets, enum_tag_counts, enum_tags_flat, fn_ctx, builtin_map, lit_sf_names, source, starts, ends, item_tags, item_names, item_d2, item_d3, item_d4, item_field_counts, enum_vnames_flat, enum_max_payloads, item_fn_row, match_env_inherit, import_aliases);
    Value v_lo;
    v_lo = _sv0t12;
    Expr _sv0t13 = value_to_expr(v_lo);
    Expr e_lo;
    e_lo = _sv0t13;
    Instr assign_it;
    int _sv0t14 = sv0_box_alloc(4);
    sv0_box_store(_sv0t14, 0, e_lo.tag);
    sv0_box_store(_sv0t14, 1, e_lo.p0);
    sv0_box_store(_sv0t14, 2, e_lo.p1);
    sv0_box_store(_sv0t14, 3, e_lo.p2);
    assign_it.tag = 3;
    assign_it.p0 = i_t;
    assign_it.p1 = _sv0t14;
    int _sv0t15 = sv0_box_alloc(5);
    sv0_box_store(_sv0t15, 0, assign_it.tag);
    sv0_box_store(_sv0t15, 1, assign_it.p0);
    sv0_box_store(_sv0t15, 2, assign_it.p1);
    sv0_box_store(_sv0t15, 3, assign_it.p2);
    sv0_box_store(_sv0t15, 4, assign_it.p3);
    sv0_vec_push(out_instrs, _sv0t15);
    Expr cond_e;
    Value _sv0t16;
    _sv0t16.tag = 3;
    _sv0t16.p0 = i_t;
    int _sv0t17 = sv0_box_alloc(3);
    sv0_box_store(_sv0t17, 0, _sv0t16.tag);
    sv0_box_store(_sv0t17, 1, _sv0t16.p0);
    sv0_box_store(_sv0t17, 2, _sv0t16.p1);
    Value _sv0t18;
    _sv0t18.tag = 3;
    _sv0t18.p0 = hi_t;
    int _sv0t19 = sv0_box_alloc(3);
    sv0_box_store(_sv0t19, 0, _sv0t18.tag);
    sv0_box_store(_sv0t19, 1, _sv0t18.p0);
    sv0_box_store(_sv0t19, 2, _sv0t18.p1);
    cond_e.tag = 2;
    cond_e.p0 = 7;
    cond_e.p1 = _sv0t17;
    cond_e.p2 = _sv0t19;
    Instr assign_x;
    Expr _sv0t20;
    _sv0t20.tag = 1;
    _sv0t20.p0 = i_t;
    int _sv0t21 = sv0_box_alloc(4);
    sv0_box_store(_sv0t21, 0, _sv0t20.tag);
    sv0_box_store(_sv0t21, 1, _sv0t20.p0);
    sv0_box_store(_sv0t21, 2, _sv0t20.p1);
    sv0_box_store(_sv0t21, 3, _sv0t20.p2);
    assign_x.tag = 3;
    assign_x.p0 = pat_tok;
    assign_x.p1 = _sv0t21;
    Value one_v;
    one_v.tag = 0;
    one_v.p0 = 1;
    Expr incr_e;
    Value _sv0t22;
    _sv0t22.tag = 3;
    _sv0t22.p0 = i_t;
    int _sv0t23 = sv0_box_alloc(3);
    sv0_box_store(_sv0t23, 0, _sv0t22.tag);
    sv0_box_store(_sv0t23, 1, _sv0t22.p0);
    sv0_box_store(_sv0t23, 2, _sv0t22.p1);
    int _sv0t24 = sv0_box_alloc(3);
    sv0_box_store(_sv0t24, 0, one_v.tag);
    sv0_box_store(_sv0t24, 1, one_v.p0);
    sv0_box_store(_sv0t24, 2, one_v.p1);
    incr_e.tag = 2;
    incr_e.p0 = 0;
    incr_e.p1 = _sv0t23;
    incr_e.p2 = _sv0t24;
    Instr incr_i;
    int _sv0t25 = sv0_box_alloc(4);
    sv0_box_store(_sv0t25, 0, incr_e.tag);
    sv0_box_store(_sv0t25, 1, incr_e.p0);
    sv0_box_store(_sv0t25, 2, incr_e.p1);
    sv0_box_store(_sv0t25, 3, incr_e.p2);
    incr_i.tag = 4;
    incr_i.p0 = i_t;
    incr_i.p1 = _sv0t25;
    int _sv0t26 = sv0_vec_new();
    int loop_body = _sv0t26;
    int _sv0t27 = sv0_box_alloc(5);
    sv0_box_store(_sv0t27, 0, assign_x.tag);
    sv0_box_store(_sv0t27, 1, assign_x.p0);
    sv0_box_store(_sv0t27, 2, assign_x.p1);
    sv0_box_store(_sv0t27, 3, assign_x.p2);
    sv0_box_store(_sv0t27, 4, assign_x.p3);
    sv0_vec_push(loop_body, _sv0t27);
    int _sv0t28 = sv0_box_alloc(5);
    sv0_box_store(_sv0t28, 0, incr_i.tag);
    sv0_box_store(_sv0t28, 1, incr_i.p0);
    sv0_box_store(_sv0t28, 2, incr_i.p1);
    sv0_box_store(_sv0t28, 3, incr_i.p2);
    sv0_box_store(_sv0t28, 4, incr_i.p3);
    sv0_vec_push(loop_body, _sv0t28);
    int _sv0t29 = lower_for_effect(et, ed1, ed2, ed3, ed4, pp, tok_tags, for_body, ctr, loop_body, enum_names, enum_tag_offsets, enum_tag_counts, enum_tags_flat, fn_ctx, builtin_map, lit_sf_names, source, starts, ends, item_tags, item_names, item_d2, item_d3, item_d4, item_field_counts, enum_vnames_flat, enum_max_payloads, item_fn_row, match_env_inherit, import_aliases);
    int discard_for = _sv0t29;
    Instr wl;
    int _sv0t30 = sv0_box_alloc(4);
    sv0_box_store(_sv0t30, 0, cond_e.tag);
    sv0_box_store(_sv0t30, 1, cond_e.p0);
    sv0_box_store(_sv0t30, 2, cond_e.p1);
    sv0_box_store(_sv0t30, 3, cond_e.p2);
    wl.tag = 7;
    wl.p0 = _sv0t30;
    wl.p1 = loop_body;
    int _sv0t31 = sv0_box_alloc(5);
    sv0_box_store(_sv0t31, 0, wl.tag);
    sv0_box_store(_sv0t31, 1, wl.p0);
    sv0_box_store(_sv0t31, 2, wl.p1);
    sv0_box_store(_sv0t31, 3, wl.p2);
    sv0_box_store(_sv0t31, 4, wl.p3);
    sv0_vec_push(out_instrs, _sv0t31);
  } else {
  }
  Value _sv0t32;
  _sv0t32.tag = 4;
  return _sv0t32;
}

static Value lower_tag_try(int et, int ed1, int ed2, int ed3, int ed4, int pp, int tok_tags, int idx, int ctr, int out_instrs, int enum_names, int enum_tag_offsets, int enum_tag_counts, int enum_tags_flat, int fn_ctx, int builtin_map, int lit_sf_names, const char* source, int starts, int ends, int item_tags, int item_names, int item_d2, int item_d3, int item_d4, int item_field_counts, int enum_vnames_flat, int enum_max_payloads, int item_fn_row, int match_env_inherit, int import_aliases) {
  int _sv0t0 = sv0_vec_get(ed1, idx);
  int inner = _sv0t0;
  int _sv0t1 = sv0_vec_len(fn_ctx);
  int fc_len = _sv0t1;
  int fn_count = 0;
  if ((fc_len > 0)) {
    int _sv0t2 = sv0_vec_get(fn_ctx, 0);
    fn_count = _sv0t2;
  } else {
  }
  int _sv0t3 = sv0_vec_get(et, inner);
  int inner_tag = _sv0t3;
  int enum_name = (0 - 1);
  if ((inner_tag == 4)) {
    int _sv0t4 = sv0_vec_get(ed1, inner);
    int callee_idx = _sv0t4;
    int _sv0t5 = sv0_vec_get(et, callee_idx);
    int callee_tag = _sv0t5;
    if ((callee_tag == 1)) {
      int _sv0t6 = sv0_vec_get(ed1, callee_idx);
      int cpps = _sv0t6;
      int _sv0t7 = sv0_vec_get(ed2, callee_idx);
      int cppc = _sv0t7;
      if ((cppc == 1)) {
        int _sv0t8 = sv0_vec_get(pp, cpps);
        int callee_tok = _sv0t8;
        int fi = 0;
        while (1) {
          if ((fi >= fn_count)) {
            break;
          } else {
          }
          int _sv0t9 = (1 + fi);
          int _sv0t10 = sv0_vec_get(fn_ctx, _sv0t9);
          int fn_name_h = _sv0t10;
          if ((fn_name_h == callee_tok)) {
            int _sv0t11 = (1 + fn_count);
            int _sv0t12 = (_sv0t11 + fi);
            int _sv0t13 = sv0_vec_get(fn_ctx, _sv0t12);
            enum_name = _sv0t13;
          } else {
          }
          fi = (fi + 1);
        }
      } else {
      }
      if ((cppc == 2)) {
        int _sv0t14 = sv0_vec_get(ed1, callee_idx);
        int cpps2 = _sv0t14;
        int _sv0t15 = sv0_vec_get(pp, cpps2);
        enum_name = _sv0t15;
      } else {
      }
    } else {
    }
  } else {
  }
  if ((inner_tag == 1)) {
    int _sv0t16 = sv0_vec_get(ed1, inner);
    int pps = _sv0t16;
    int _sv0t17 = sv0_vec_get(ed2, inner);
    int ppc = _sv0t17;
    if ((ppc == 2)) {
      int _sv0t18 = sv0_vec_get(pp, pps);
      enum_name = _sv0t18;
    } else {
    }
  } else {
  }
  if ((enum_name < 0)) {
    Value _sv0t19;
    _sv0t19.tag = 4;
    return _sv0t19;
  } else {
  }
  int failK = 1;
  Value _sv0t20 = lower_expr_to_value(et, ed1, ed2, ed3, ed4, pp, tok_tags, inner, ctr, out_instrs, enum_names, enum_tag_offsets, enum_tag_counts, enum_tags_flat, fn_ctx, builtin_map, lit_sf_names, source, starts, ends, item_tags, item_names, item_d2, item_d3, item_d4, item_field_counts, enum_vnames_flat, enum_max_payloads, item_fn_row, match_env_inherit, import_aliases);
  Value inner_v;
  inner_v = _sv0t20;
  int _sv0t21 = ctr_fresh(ctr);
  int u = _sv0t21;
  Instr decl_u;
  decl_u.tag = 2;
  decl_u.p0 = enum_name;
  decl_u.p1 = u;
  int _sv0t22 = sv0_box_alloc(5);
  sv0_box_store(_sv0t22, 0, decl_u.tag);
  sv0_box_store(_sv0t22, 1, decl_u.p0);
  sv0_box_store(_sv0t22, 2, decl_u.p1);
  sv0_box_store(_sv0t22, 3, decl_u.p2);
  sv0_box_store(_sv0t22, 4, decl_u.p3);
  sv0_vec_push(out_instrs, _sv0t22);
  Expr _sv0t23 = value_to_expr(inner_v);
  Expr inner_e;
  inner_e = _sv0t23;
  Instr store_u;
  int _sv0t24 = sv0_box_alloc(4);
  sv0_box_store(_sv0t24, 0, inner_e.tag);
  sv0_box_store(_sv0t24, 1, inner_e.p0);
  sv0_box_store(_sv0t24, 2, inner_e.p1);
  sv0_box_store(_sv0t24, 3, inner_e.p2);
  store_u.tag = 4;
  store_u.p0 = u;
  store_u.p1 = _sv0t24;
  int _sv0t25 = sv0_box_alloc(5);
  sv0_box_store(_sv0t25, 0, store_u.tag);
  sv0_box_store(_sv0t25, 1, store_u.p0);
  sv0_box_store(_sv0t25, 2, store_u.p1);
  sv0_box_store(_sv0t25, 3, store_u.p2);
  sv0_box_store(_sv0t25, 4, store_u.p3);
  sv0_vec_push(out_instrs, _sv0t25);
  int _sv0t26 = ctr_fresh(ctr);
  int out_t = _sv0t26;
  Instr decl_out;
  decl_out.tag = 1;
  decl_out.p0 = out_t;
  int _sv0t27 = sv0_box_alloc(5);
  sv0_box_store(_sv0t27, 0, decl_out.tag);
  sv0_box_store(_sv0t27, 1, decl_out.p0);
  sv0_box_store(_sv0t27, 2, decl_out.p1);
  sv0_box_store(_sv0t27, 3, decl_out.p2);
  sv0_box_store(_sv0t27, 4, decl_out.p3);
  sv0_vec_push(out_instrs, _sv0t27);
  Value tag_member;
  Value _sv0t28;
  _sv0t28.tag = 3;
  _sv0t28.p0 = u;
  int _sv0t29 = sv0_box_alloc(3);
  sv0_box_store(_sv0t29, 0, _sv0t28.tag);
  sv0_box_store(_sv0t29, 1, _sv0t28.p0);
  sv0_box_store(_sv0t29, 2, _sv0t28.p1);
  int _sv0t30 = (0 - 1);
  tag_member.tag = 6;
  tag_member.p0 = _sv0t29;
  tag_member.p1 = _sv0t30;
  Value fail_v;
  fail_v.tag = 0;
  fail_v.p0 = failK;
  Expr cond_e;
  int _sv0t31 = sv0_box_alloc(3);
  sv0_box_store(_sv0t31, 0, tag_member.tag);
  sv0_box_store(_sv0t31, 1, tag_member.p0);
  sv0_box_store(_sv0t31, 2, tag_member.p1);
  int _sv0t32 = sv0_box_alloc(3);
  sv0_box_store(_sv0t32, 0, fail_v.tag);
  sv0_box_store(_sv0t32, 1, fail_v.p0);
  sv0_box_store(_sv0t32, 2, fail_v.p1);
  cond_e.tag = 2;
  cond_e.p0 = 11;
  cond_e.p1 = _sv0t31;
  cond_e.p2 = _sv0t32;
  int _sv0t33 = sv0_vec_new();
  int then_is = _sv0t33;
  Instr ret_fail;
  Value _sv0t34;
  _sv0t34.tag = 3;
  _sv0t34.p0 = u;
  int _sv0t35 = sv0_box_alloc(3);
  sv0_box_store(_sv0t35, 0, _sv0t34.tag);
  sv0_box_store(_sv0t35, 1, _sv0t34.p0);
  sv0_box_store(_sv0t35, 2, _sv0t34.p1);
  ret_fail.tag = 15;
  ret_fail.p0 = _sv0t35;
  int _sv0t36 = sv0_box_alloc(5);
  sv0_box_store(_sv0t36, 0, ret_fail.tag);
  sv0_box_store(_sv0t36, 1, ret_fail.p0);
  sv0_box_store(_sv0t36, 2, ret_fail.p1);
  sv0_box_store(_sv0t36, 3, ret_fail.p2);
  sv0_box_store(_sv0t36, 4, ret_fail.p3);
  sv0_vec_push(then_is, _sv0t36);
  int _sv0t37 = sv0_vec_new();
  int else_is = _sv0t37;
  Value p0_access;
  p0_access.tag = 3;
  p0_access.p0 = u;
  Expr p0_e;
  int _sv0t38 = sv0_box_alloc(3);
  sv0_box_store(_sv0t38, 0, p0_access.tag);
  sv0_box_store(_sv0t38, 1, p0_access.p0);
  sv0_box_store(_sv0t38, 2, p0_access.p1);
  int _sv0t39 = (0 - 10);
  p0_e.tag = 4;
  p0_e.p0 = _sv0t38;
  p0_e.p1 = _sv0t39;
  Instr store_out;
  int _sv0t40 = sv0_box_alloc(4);
  sv0_box_store(_sv0t40, 0, p0_e.tag);
  sv0_box_store(_sv0t40, 1, p0_e.p0);
  sv0_box_store(_sv0t40, 2, p0_e.p1);
  sv0_box_store(_sv0t40, 3, p0_e.p2);
  store_out.tag = 4;
  store_out.p0 = out_t;
  store_out.p1 = _sv0t40;
  int _sv0t41 = sv0_box_alloc(5);
  sv0_box_store(_sv0t41, 0, store_out.tag);
  sv0_box_store(_sv0t41, 1, store_out.p0);
  sv0_box_store(_sv0t41, 2, store_out.p1);
  sv0_box_store(_sv0t41, 3, store_out.p2);
  sv0_box_store(_sv0t41, 4, store_out.p3);
  sv0_vec_push(else_is, _sv0t41);
  Instr if_instr;
  int _sv0t42 = sv0_box_alloc(4);
  sv0_box_store(_sv0t42, 0, cond_e.tag);
  sv0_box_store(_sv0t42, 1, cond_e.p0);
  sv0_box_store(_sv0t42, 2, cond_e.p1);
  sv0_box_store(_sv0t42, 3, cond_e.p2);
  if_instr.tag = 6;
  if_instr.p0 = _sv0t42;
  if_instr.p1 = then_is;
  if_instr.p2 = else_is;
  int _sv0t43 = sv0_box_alloc(5);
  sv0_box_store(_sv0t43, 0, if_instr.tag);
  sv0_box_store(_sv0t43, 1, if_instr.p0);
  sv0_box_store(_sv0t43, 2, if_instr.p1);
  sv0_box_store(_sv0t43, 3, if_instr.p2);
  sv0_box_store(_sv0t43, 4, if_instr.p3);
  sv0_vec_push(out_instrs, _sv0t43);
  Value _sv0t44;
  _sv0t44.tag = 3;
  _sv0t44.p0 = out_t;
  return _sv0t44;
}

static Expr lower_cond_expr(int et, int ed1, int ed2, int ed3, int ed4, int pp, int tok_tags, int idx, int ctr, int cond_is, int enum_names, int enum_tag_offsets, int enum_tag_counts, int enum_tags_flat, int fn_ctx, int builtin_map, int lit_sf_names, const char* source, int starts, int ends, int item_tags, int item_names, int item_d2, int item_d3, int item_d4, int item_field_counts, int enum_vnames_flat, int enum_max_payloads, int item_fn_row, int match_env_inherit, int import_aliases) {
  int _sv0t0 = sv0_vec_get(et, idx);
  int ctag = _sv0t0;
  if ((ctag == 3)) {
    int _sv0t1 = sv0_vec_get(ed1, idx);
    int op = _sv0t1;
    int _sv0t2 = sv0_vec_get(ed2, idx);
    int lhs_i = _sv0t2;
    int _sv0t3 = sv0_vec_get(ed3, idx);
    int rhs_i = _sv0t3;
    Value _sv0t4 = lower_expr_to_value(et, ed1, ed2, ed3, ed4, pp, tok_tags, lhs_i, ctr, cond_is, enum_names, enum_tag_offsets, enum_tag_counts, enum_tags_flat, fn_ctx, builtin_map, lit_sf_names, source, starts, ends, item_tags, item_names, item_d2, item_d3, item_d4, item_field_counts, enum_vnames_flat, enum_max_payloads, item_fn_row, match_env_inherit, import_aliases);
    Value vl;
    vl = _sv0t4;
    Value _sv0t5 = lower_expr_to_value(et, ed1, ed2, ed3, ed4, pp, tok_tags, rhs_i, ctr, cond_is, enum_names, enum_tag_offsets, enum_tag_counts, enum_tags_flat, fn_ctx, builtin_map, lit_sf_names, source, starts, ends, item_tags, item_names, item_d2, item_d3, item_d4, item_field_counts, enum_vnames_flat, enum_max_payloads, item_fn_row, match_env_inherit, import_aliases);
    Value vr;
    vr = _sv0t5;
    Expr _sv0t6;
    int _sv0t7 = sv0_box_alloc(3);
    sv0_box_store(_sv0t7, 0, vl.tag);
    sv0_box_store(_sv0t7, 1, vl.p0);
    sv0_box_store(_sv0t7, 2, vl.p1);
    int _sv0t8 = sv0_box_alloc(3);
    sv0_box_store(_sv0t8, 0, vr.tag);
    sv0_box_store(_sv0t8, 1, vr.p0);
    sv0_box_store(_sv0t8, 2, vr.p1);
    _sv0t6.tag = 2;
    _sv0t6.p0 = op;
    _sv0t6.p1 = _sv0t7;
    _sv0t6.p2 = _sv0t8;
    return _sv0t6;
  } else {
  }
  if ((ctag == 2)) {
    int _sv0t9 = sv0_vec_get(ed1, idx);
    int uop = _sv0t9;
    if ((uop < 3)) {
      int _sv0t10 = sv0_vec_get(ed2, idx);
      int operand_i = _sv0t10;
      Value _sv0t11 = lower_expr_to_value(et, ed1, ed2, ed3, ed4, pp, tok_tags, operand_i, ctr, cond_is, enum_names, enum_tag_offsets, enum_tag_counts, enum_tags_flat, fn_ctx, builtin_map, lit_sf_names, source, starts, ends, item_tags, item_names, item_d2, item_d3, item_d4, item_field_counts, enum_vnames_flat, enum_max_payloads, item_fn_row, match_env_inherit, import_aliases);
      Value uv;
      uv = _sv0t11;
      Expr _sv0t12;
      int _sv0t13 = sv0_box_alloc(3);
      sv0_box_store(_sv0t13, 0, uv.tag);
      sv0_box_store(_sv0t13, 1, uv.p0);
      sv0_box_store(_sv0t13, 2, uv.p1);
      _sv0t12.tag = 3;
      _sv0t12.p0 = uop;
      _sv0t12.p1 = _sv0t13;
      return _sv0t12;
    } else {
    }
  } else {
  }
  Value _sv0t14 = lower_expr_to_value(et, ed1, ed2, ed3, ed4, pp, tok_tags, idx, ctr, cond_is, enum_names, enum_tag_offsets, enum_tag_counts, enum_tags_flat, fn_ctx, builtin_map, lit_sf_names, source, starts, ends, item_tags, item_names, item_d2, item_d3, item_d4, item_field_counts, enum_vnames_flat, enum_max_payloads, item_fn_row, match_env_inherit, import_aliases);
  Value gv;
  gv = _sv0t14;
  Expr _sv0t15 = value_to_expr(gv);
  return _sv0t15;
}

static Value lower_expr_to_value(int et, int ed1, int ed2, int ed3, int ed4, int pp, int tok_tags, int idx, int ctr, int out_instrs, int enum_names, int enum_tag_offsets, int enum_tag_counts, int enum_tags_flat, int fn_ctx, int builtin_map, int lit_sf_names, const char* source, int starts, int ends, int item_tags, int item_names, int item_d2, int item_d3, int item_d4, int item_field_counts, int enum_vnames_flat, int enum_max_payloads, int item_fn_row, int match_env_inherit, int import_aliases) {
  int _sv0t0 = sv0_vec_get(et, idx);
  int tag = _sv0t0;
  if ((tag == 0)) {
    int _sv0t1 = sv0_vec_get(ed1, idx);
    int lit_tag = _sv0t1;
    int _sv0t2 = sv0_vec_get(ed2, idx);
    int tok_pos = _sv0t2;
    Value _sv0t3 = lower_lit_to_ir_value(lit_tag, tok_pos, tok_tags, source, starts, ends);
    return _sv0t3;
  } else {
  }
  if ((tag == 1)) {
    int _sv0t4 = sv0_vec_get(ed1, idx);
    int pps = _sv0t4;
    int _sv0t5 = sv0_vec_get(ed2, idx);
    int ppc = _sv0t5;
    if ((ppc == 1)) {
      int _sv0t6 = sv0_vec_get(pp, pps);
      int name_tok = _sv0t6;
      Value _sv0t7;
      _sv0t7.tag = 3;
      _sv0t7.p0 = name_tok;
      return _sv0t7;
    } else {
    }
    if ((ppc == 2)) {
      int _sv0t8 = sv0_vec_get(pp, pps);
      int en_tok = _sv0t8;
      int _sv0t9 = (pps + 1);
      int _sv0t10 = sv0_vec_get(pp, _sv0t9);
      int vn_tok = _sv0t10;
      int _sv0t11 = enum_tag_lookup_str(enum_names, enum_tags_flat, enum_tag_offsets, enum_tag_counts, en_tok, vn_tok, source, starts, ends);
      int k = _sv0t11;
      int _sv0t12 = ctr_fresh(ctr);
      int t = _sv0t12;
      Instr decl;
      decl.tag = 2;
      decl.p0 = en_tok;
      decl.p1 = t;
      int _sv0t13 = sv0_box_alloc(5);
      sv0_box_store(_sv0t13, 0, decl.tag);
      sv0_box_store(_sv0t13, 1, decl.p0);
      sv0_box_store(_sv0t13, 2, decl.p1);
      sv0_box_store(_sv0t13, 3, decl.p2);
      sv0_box_store(_sv0t13, 4, decl.p3);
      sv0_vec_push(out_instrs, _sv0t13);
      Expr tag_e;
      Value _sv0t14;
      _sv0t14.tag = 0;
      _sv0t14.p0 = k;
      int _sv0t15 = sv0_box_alloc(3);
      sv0_box_store(_sv0t15, 0, _sv0t14.tag);
      sv0_box_store(_sv0t15, 1, _sv0t14.p0);
      sv0_box_store(_sv0t15, 2, _sv0t14.p1);
      tag_e.tag = 0;
      tag_e.p0 = _sv0t15;
      Instr sf_tag;
      int _sv0t16 = (0 - 1);
      int _sv0t17 = sv0_box_alloc(4);
      sv0_box_store(_sv0t17, 0, tag_e.tag);
      sv0_box_store(_sv0t17, 1, tag_e.p0);
      sv0_box_store(_sv0t17, 2, tag_e.p1);
      sv0_box_store(_sv0t17, 3, tag_e.p2);
      sf_tag.tag = 5;
      sf_tag.p0 = t;
      sf_tag.p1 = _sv0t16;
      sf_tag.p2 = _sv0t17;
      int _sv0t18 = sv0_box_alloc(5);
      sv0_box_store(_sv0t18, 0, sf_tag.tag);
      sv0_box_store(_sv0t18, 1, sf_tag.p0);
      sv0_box_store(_sv0t18, 2, sf_tag.p1);
      sv0_box_store(_sv0t18, 3, sf_tag.p2);
      sv0_box_store(_sv0t18, 4, sf_tag.p3);
      sv0_vec_push(out_instrs, _sv0t18);
      Value _sv0t19;
      _sv0t19.tag = 3;
      _sv0t19.p0 = t;
      return _sv0t19;
    } else {
    }
    Value _sv0t20;
    _sv0t20.tag = 4;
    return _sv0t20;
  } else {
  }
  if ((tag == 2)) {
    int _sv0t21 = sv0_vec_get(ed1, idx);
    int unop_tag = _sv0t21;
    int _sv0t22 = sv0_vec_get(ed2, idx);
    int operand_idx = _sv0t22;
    if ((unop_tag == 4)) {
      int _sv0t23 = sv0_vec_get(et, operand_idx);
      int ot = _sv0t23;
      if ((ot == 1)) {
        int _sv0t24 = sv0_vec_get(ed1, operand_idx);
        int opps = _sv0t24;
        int _sv0t25 = sv0_vec_get(ed2, operand_idx);
        int oppc = _sv0t25;
        if ((oppc == 1)) {
          Value _sv0t26;
          int _sv0t27 = sv0_vec_get(pp, opps);
          _sv0t26.tag = 7;
          _sv0t26.p0 = _sv0t27;
          return _sv0t26;
        } else {
        }
      } else {
      }
    } else {
    }
    if ((unop_tag == 5)) {
      int _sv0t28 = sv0_vec_get(et, operand_idx);
      int ot2 = _sv0t28;
      if ((ot2 == 1)) {
        int _sv0t29 = sv0_vec_get(ed1, operand_idx);
        int opps2 = _sv0t29;
        int _sv0t30 = sv0_vec_get(ed2, operand_idx);
        int oppc2 = _sv0t30;
        if ((oppc2 == 1)) {
          Value _sv0t31;
          int _sv0t32 = sv0_vec_get(pp, opps2);
          _sv0t31.tag = 7;
          _sv0t31.p0 = _sv0t32;
          return _sv0t31;
        } else {
        }
      } else {
      }
    } else {
    }
    Value _sv0t33 = lower_expr_to_value(et, ed1, ed2, ed3, ed4, pp, tok_tags, operand_idx, ctr, out_instrs, enum_names, enum_tag_offsets, enum_tag_counts, enum_tags_flat, fn_ctx, builtin_map, lit_sf_names, source, starts, ends, item_tags, item_names, item_d2, item_d3, item_d4, item_field_counts, enum_vnames_flat, enum_max_payloads, item_fn_row, match_env_inherit, import_aliases);
    Value v;
    v = _sv0t33;
    int _sv0t34 = ctr_fresh(ctr);
    int t = _sv0t34;
    Expr expr;
    int _sv0t35 = sv0_box_alloc(3);
    sv0_box_store(_sv0t35, 0, v.tag);
    sv0_box_store(_sv0t35, 1, v.p0);
    sv0_box_store(_sv0t35, 2, v.p1);
    expr.tag = 3;
    expr.p0 = unop_tag;
    expr.p1 = _sv0t35;
    Instr instr;
    int _sv0t36 = sv0_box_alloc(4);
    sv0_box_store(_sv0t36, 0, expr.tag);
    sv0_box_store(_sv0t36, 1, expr.p0);
    sv0_box_store(_sv0t36, 2, expr.p1);
    sv0_box_store(_sv0t36, 3, expr.p2);
    instr.tag = 3;
    instr.p0 = t;
    instr.p1 = _sv0t36;
    int _sv0t37 = sv0_box_alloc(5);
    sv0_box_store(_sv0t37, 0, instr.tag);
    sv0_box_store(_sv0t37, 1, instr.p0);
    sv0_box_store(_sv0t37, 2, instr.p1);
    sv0_box_store(_sv0t37, 3, instr.p2);
    sv0_box_store(_sv0t37, 4, instr.p3);
    sv0_vec_push(out_instrs, _sv0t37);
    Value _sv0t38;
    _sv0t38.tag = 3;
    _sv0t38.p0 = t;
    return _sv0t38;
  } else {
  }
  if ((tag == 3)) {
    int _sv0t39 = sv0_vec_get(ed1, idx);
    int binop_tag = _sv0t39;
    int _sv0t40 = sv0_vec_get(ed2, idx);
    int lhs_idx = _sv0t40;
    int _sv0t41 = sv0_vec_get(ed3, idx);
    int rhs_idx = _sv0t41;
    Value _sv0t42 = lower_expr_to_value(et, ed1, ed2, ed3, ed4, pp, tok_tags, lhs_idx, ctr, out_instrs, enum_names, enum_tag_offsets, enum_tag_counts, enum_tags_flat, fn_ctx, builtin_map, lit_sf_names, source, starts, ends, item_tags, item_names, item_d2, item_d3, item_d4, item_field_counts, enum_vnames_flat, enum_max_payloads, item_fn_row, match_env_inherit, import_aliases);
    Value vl;
    vl = _sv0t42;
    Value _sv0t43 = lower_expr_to_value(et, ed1, ed2, ed3, ed4, pp, tok_tags, rhs_idx, ctr, out_instrs, enum_names, enum_tag_offsets, enum_tag_counts, enum_tags_flat, fn_ctx, builtin_map, lit_sf_names, source, starts, ends, item_tags, item_names, item_d2, item_d3, item_d4, item_field_counts, enum_vnames_flat, enum_max_payloads, item_fn_row, match_env_inherit, import_aliases);
    Value vr;
    vr = _sv0t43;
    int _sv0t44 = ctr_fresh(ctr);
    int t = _sv0t44;
    Expr expr;
    int _sv0t45 = sv0_box_alloc(3);
    sv0_box_store(_sv0t45, 0, vl.tag);
    sv0_box_store(_sv0t45, 1, vl.p0);
    sv0_box_store(_sv0t45, 2, vl.p1);
    int _sv0t46 = sv0_box_alloc(3);
    sv0_box_store(_sv0t46, 0, vr.tag);
    sv0_box_store(_sv0t46, 1, vr.p0);
    sv0_box_store(_sv0t46, 2, vr.p1);
    expr.tag = 2;
    expr.p0 = binop_tag;
    expr.p1 = _sv0t45;
    expr.p2 = _sv0t46;
    Instr instr;
    int _sv0t47 = sv0_box_alloc(4);
    sv0_box_store(_sv0t47, 0, expr.tag);
    sv0_box_store(_sv0t47, 1, expr.p0);
    sv0_box_store(_sv0t47, 2, expr.p1);
    sv0_box_store(_sv0t47, 3, expr.p2);
    instr.tag = 3;
    instr.p0 = t;
    instr.p1 = _sv0t47;
    int _sv0t48 = sv0_box_alloc(5);
    sv0_box_store(_sv0t48, 0, instr.tag);
    sv0_box_store(_sv0t48, 1, instr.p0);
    sv0_box_store(_sv0t48, 2, instr.p1);
    sv0_box_store(_sv0t48, 3, instr.p2);
    sv0_box_store(_sv0t48, 4, instr.p3);
    sv0_vec_push(out_instrs, _sv0t48);
    Value _sv0t49;
    _sv0t49.tag = 3;
    _sv0t49.p0 = t;
    return _sv0t49;
  } else {
  }
  if ((tag == 4)) {
    Value _sv0t50 = lower_tag_call(et, ed1, ed2, ed3, ed4, pp, tok_tags, idx, ctr, out_instrs, enum_names, enum_tag_offsets, enum_tag_counts, enum_tags_flat, fn_ctx, builtin_map, lit_sf_names, source, starts, ends, item_tags, item_names, item_d2, item_d3, item_d4, item_field_counts, enum_vnames_flat, enum_max_payloads, item_fn_row, match_env_inherit, import_aliases);
    return _sv0t50;
  } else {
  }
  if ((tag == 6)) {
    int _sv0t51 = sv0_vec_get(ed1, idx);
    int obj_idx = _sv0t51;
    int _sv0t52 = sv0_vec_get(ed2, idx);
    int field_tok = _sv0t52;
    Value _sv0t53 = lower_expr_to_value(et, ed1, ed2, ed3, ed4, pp, tok_tags, obj_idx, ctr, out_instrs, enum_names, enum_tag_offsets, enum_tag_counts, enum_tags_flat, fn_ctx, builtin_map, lit_sf_names, source, starts, ends, item_tags, item_names, item_d2, item_d3, item_d4, item_field_counts, enum_vnames_flat, enum_max_payloads, item_fn_row, match_env_inherit, import_aliases);
    Value v1;
    v1 = _sv0t53;
    Value _sv0t54;
    int _sv0t55 = sv0_box_alloc(3);
    sv0_box_store(_sv0t55, 0, v1.tag);
    sv0_box_store(_sv0t55, 1, v1.p0);
    sv0_box_store(_sv0t55, 2, v1.p1);
    _sv0t54.tag = 6;
    _sv0t54.p0 = _sv0t55;
    _sv0t54.p1 = field_tok;
    return _sv0t54;
  } else {
  }
  if ((tag == 7)) {
    int _sv0t56 = sv0_vec_get(ed1, idx);
    int obj_idx7 = _sv0t56;
    int _sv0t57 = sv0_vec_get(ed2, idx);
    int field_num = _sv0t57;
    Value _sv0t58 = lower_expr_to_value(et, ed1, ed2, ed3, ed4, pp, tok_tags, obj_idx7, ctr, out_instrs, enum_names, enum_tag_offsets, enum_tag_counts, enum_tags_flat, fn_ctx, builtin_map, lit_sf_names, source, starts, ends, item_tags, item_names, item_d2, item_d3, item_d4, item_field_counts, enum_vnames_flat, enum_max_payloads, item_fn_row, match_env_inherit, import_aliases);
    Value vobj;
    vobj = _sv0t58;
    int fh = (500 + field_num);
    Value _sv0t59;
    int _sv0t60 = sv0_box_alloc(3);
    sv0_box_store(_sv0t60, 0, vobj.tag);
    sv0_box_store(_sv0t60, 1, vobj.p0);
    sv0_box_store(_sv0t60, 2, vobj.p1);
    _sv0t59.tag = 6;
    _sv0t59.p0 = _sv0t60;
    _sv0t59.p1 = fh;
    return _sv0t59;
  } else {
  }
  if ((tag == 5)) {
    int _sv0t61 = sv0_vec_get(ed1, idx);
    int rcv_mc = _sv0t61;
    int _sv0t62 = sv0_vec_get(ed2, idx);
    int method_tok = _sv0t62;
    int _sv0t63 = sv0_vec_get(ed3, idx);
    int args_first_mc = _sv0t63;
    int _sv0t64 = sv0_vec_get(ed4, idx);
    int args_count_mc = _sv0t64;
    Value _sv0t65 = lower_expr_to_value(et, ed1, ed2, ed3, ed4, pp, tok_tags, rcv_mc, ctr, out_instrs, enum_names, enum_tag_offsets, enum_tag_counts, enum_tags_flat, fn_ctx, builtin_map, lit_sf_names, source, starts, ends, item_tags, item_names, item_d2, item_d3, item_d4, item_field_counts, enum_vnames_flat, enum_max_payloads, item_fn_row, match_env_inherit, import_aliases);
    Value v_rcv;
    v_rcv = _sv0t65;
    int _sv0t66 = sv0_vec_new();
    int arg_vals_mc = _sv0t66;
    int _sv0t67 = sv0_box_alloc(3);
    sv0_box_store(_sv0t67, 0, v_rcv.tag);
    sv0_box_store(_sv0t67, 1, v_rcv.p0);
    sv0_box_store(_sv0t67, 2, v_rcv.p1);
    sv0_vec_push(arg_vals_mc, _sv0t67);
    int aj_mc = 0;
    while (1) {
      if ((aj_mc >= args_count_mc)) {
        break;
      } else {
      }
      int ax_mc = (args_first_mc + aj_mc);
      Value _sv0t68 = lower_expr_to_value(et, ed1, ed2, ed3, ed4, pp, tok_tags, ax_mc, ctr, out_instrs, enum_names, enum_tag_offsets, enum_tag_counts, enum_tags_flat, fn_ctx, builtin_map, lit_sf_names, source, starts, ends, item_tags, item_names, item_d2, item_d3, item_d4, item_field_counts, enum_vnames_flat, enum_max_payloads, item_fn_row, match_env_inherit, import_aliases);
      Value av_mc;
      av_mc = _sv0t68;
      int _sv0t69 = sv0_box_alloc(3);
      sv0_box_store(_sv0t69, 0, av_mc.tag);
      sv0_box_store(_sv0t69, 1, av_mc.p0);
      sv0_box_store(_sv0t69, 2, av_mc.p1);
      sv0_vec_push(arg_vals_mc, _sv0t69);
      aj_mc = (aj_mc + 1);
    }
    int _sv0t70 = ctr_fresh(ctr);
    int t_mc = _sv0t70;
    int bid_mc = 0;
    if ((method_tok >= 0)) {
      int _sv0t71 = sv0_vec_len(builtin_map);
      if ((method_tok < _sv0t71)) {
        int _sv0t72 = sv0_vec_get(builtin_map, method_tok);
        bid_mc = _sv0t72;
      } else {
      }
    } else {
    }
    if ((bid_mc == 0)) {
      Instr call_mc;
      call_mc.tag = 11;
      call_mc.p0 = t_mc;
      call_mc.p1 = method_tok;
      call_mc.p2 = arg_vals_mc;
      call_mc.p3 = 0;
      int _sv0t73 = sv0_box_alloc(5);
      sv0_box_store(_sv0t73, 0, call_mc.tag);
      sv0_box_store(_sv0t73, 1, call_mc.p0);
      sv0_box_store(_sv0t73, 2, call_mc.p1);
      sv0_box_store(_sv0t73, 3, call_mc.p2);
      sv0_box_store(_sv0t73, 4, call_mc.p3);
      sv0_vec_push(out_instrs, _sv0t73);
      Value _sv0t74;
      _sv0t74.tag = 3;
      _sv0t74.p0 = t_mc;
      return _sv0t74;
    } else {
    }
    if ((bid_mc == 14)) {
      int _sv0t75 = sv0_vec_len(arg_vals_mc);
      if ((_sv0t75 >= 2)) {
        int _sv0t76 = sv0_vec_get(arg_vals_mc, 1);
        Value _sv0t77;
        int _sv0t78 = sv0_box_load(_sv0t76, 0);
        _sv0t77.tag = _sv0t78;
        int _sv0t79 = sv0_box_load(_sv0t76, 1);
        _sv0t77.p0 = _sv0t79;
        int _sv0t80 = sv0_box_load(_sv0t76, 2);
        _sv0t77.p1 = _sv0t80;
        Value na_mv;
        na_mv.tag = (_sv0t77).tag;
        na_mv.p0 = (_sv0t77).p0;
        na_mv.p1 = (_sv0t77).p1;
        return na_mv;
      } else {
      }
      Value _sv0t81;
      _sv0t81.tag = 4;
      return _sv0t81;
    } else {
    }
    int is_void_mc = 0;
    if ((bid_mc == 1)) {
      is_void_mc = 1;
    } else {
    }
    if ((bid_mc == 8)) {
      is_void_mc = 1;
    } else {
    }
    if ((bid_mc == 11)) {
      is_void_mc = 1;
    } else {
    }
    int fn_sent_mc = (0 - bid_mc);
    if ((is_void_mc == 1)) {
      Instr vc_mc;
      vc_mc.tag = 11;
      vc_mc.p0 = 0;
      vc_mc.p1 = fn_sent_mc;
      vc_mc.p2 = arg_vals_mc;
      vc_mc.p3 = 0;
      int _sv0t82 = sv0_box_alloc(5);
      sv0_box_store(_sv0t82, 0, vc_mc.tag);
      sv0_box_store(_sv0t82, 1, vc_mc.p0);
      sv0_box_store(_sv0t82, 2, vc_mc.p1);
      sv0_box_store(_sv0t82, 3, vc_mc.p2);
      sv0_box_store(_sv0t82, 4, vc_mc.p3);
      sv0_vec_push(out_instrs, _sv0t82);
      Value _sv0t83;
      _sv0t83.tag = 4;
      return _sv0t83;
    } else {
    }
    Instr bc_mc;
    bc_mc.tag = 11;
    bc_mc.p0 = t_mc;
    bc_mc.p1 = fn_sent_mc;
    bc_mc.p2 = arg_vals_mc;
    bc_mc.p3 = 0;
    int _sv0t84 = sv0_box_alloc(5);
    sv0_box_store(_sv0t84, 0, bc_mc.tag);
    sv0_box_store(_sv0t84, 1, bc_mc.p0);
    sv0_box_store(_sv0t84, 2, bc_mc.p1);
    sv0_box_store(_sv0t84, 3, bc_mc.p2);
    sv0_box_store(_sv0t84, 4, bc_mc.p3);
    sv0_vec_push(out_instrs, _sv0t84);
    Value _sv0t85;
    _sv0t85.tag = 3;
    _sv0t85.p0 = t_mc;
    return _sv0t85;
  } else {
  }
  if ((tag == 8)) {
    int _sv0t86 = sv0_vec_get(ed1, idx);
    int ob = _sv0t86;
    int _sv0t87 = sv0_vec_get(ed2, idx);
    int ix = _sv0t87;
    Value _sv0t88 = lower_expr_to_value(et, ed1, ed2, ed3, ed4, pp, tok_tags, ob, ctr, out_instrs, enum_names, enum_tag_offsets, enum_tag_counts, enum_tags_flat, fn_ctx, builtin_map, lit_sf_names, source, starts, ends, item_tags, item_names, item_d2, item_d3, item_d4, item_field_counts, enum_vnames_flat, enum_max_payloads, item_fn_row, match_env_inherit, import_aliases);
    Value v_o;
    v_o = _sv0t88;
    Value _sv0t89 = lower_expr_to_value(et, ed1, ed2, ed3, ed4, pp, tok_tags, ix, ctr, out_instrs, enum_names, enum_tag_offsets, enum_tag_counts, enum_tags_flat, fn_ctx, builtin_map, lit_sf_names, source, starts, ends, item_tags, item_names, item_d2, item_d3, item_d4, item_field_counts, enum_vnames_flat, enum_max_payloads, item_fn_row, match_env_inherit, import_aliases);
    Value v_i;
    v_i = _sv0t89;
    int _sv0t90 = ctr_fresh(ctr);
    int t_ix = _sv0t90;
    int _sv0t91 = sv0_vec_new();
    int arg_ix = _sv0t91;
    int _sv0t92 = sv0_box_alloc(3);
    sv0_box_store(_sv0t92, 0, v_o.tag);
    sv0_box_store(_sv0t92, 1, v_o.p0);
    sv0_box_store(_sv0t92, 2, v_o.p1);
    sv0_vec_push(arg_ix, _sv0t92);
    int _sv0t93 = sv0_box_alloc(3);
    sv0_box_store(_sv0t93, 0, v_i.tag);
    sv0_box_store(_sv0t93, 1, v_i.p0);
    sv0_box_store(_sv0t93, 2, v_i.p1);
    sv0_vec_push(arg_ix, _sv0t93);
    int vec_get_fn_h = (0 - 10);
    Instr call_ix;
    call_ix.tag = 11;
    call_ix.p0 = t_ix;
    call_ix.p1 = vec_get_fn_h;
    call_ix.p2 = arg_ix;
    call_ix.p3 = 0;
    int _sv0t94 = sv0_box_alloc(5);
    sv0_box_store(_sv0t94, 0, call_ix.tag);
    sv0_box_store(_sv0t94, 1, call_ix.p0);
    sv0_box_store(_sv0t94, 2, call_ix.p1);
    sv0_box_store(_sv0t94, 3, call_ix.p2);
    sv0_box_store(_sv0t94, 4, call_ix.p3);
    sv0_vec_push(out_instrs, _sv0t94);
    Value _sv0t95;
    _sv0t95.tag = 3;
    _sv0t95.p0 = t_ix;
    return _sv0t95;
  } else {
  }
  if ((tag == 26)) {
    int _sv0t96 = sv0_vec_get(ed2, idx);
    int elem_count_ar = _sv0t96;
    int _sv0t97 = ctr_fresh(ctr);
    int t_ar = _sv0t97;
    int _sv0t98 = sv0_vec_new();
    int args_new = _sv0t98;
    int bid_new = 7;
    int fn_new = (0 - bid_new);
    Instr ins_new;
    ins_new.tag = 11;
    ins_new.p0 = t_ar;
    ins_new.p1 = fn_new;
    ins_new.p2 = args_new;
    ins_new.p3 = 0;
    int _sv0t99 = sv0_box_alloc(5);
    sv0_box_store(_sv0t99, 0, ins_new.tag);
    sv0_box_store(_sv0t99, 1, ins_new.p0);
    sv0_box_store(_sv0t99, 2, ins_new.p1);
    sv0_box_store(_sv0t99, 3, ins_new.p2);
    sv0_box_store(_sv0t99, 4, ins_new.p3);
    sv0_vec_push(out_instrs, _sv0t99);
    int ak_ar = 0;
    while (1) {
      if ((ak_ar >= elem_count_ar)) {
        break;
      } else {
      }
      int _sv0t100 = (idx - elem_count_ar);
      int ek_ar = (_sv0t100 + ak_ar);
      Value _sv0t101 = lower_expr_to_value(et, ed1, ed2, ed3, ed4, pp, tok_tags, ek_ar, ctr, out_instrs, enum_names, enum_tag_offsets, enum_tag_counts, enum_tags_flat, fn_ctx, builtin_map, lit_sf_names, source, starts, ends, item_tags, item_names, item_d2, item_d3, item_d4, item_field_counts, enum_vnames_flat, enum_max_payloads, item_fn_row, match_env_inherit, import_aliases);
      Value v_el;
      v_el = _sv0t101;
      int _sv0t102 = sv0_vec_new();
      int args_push = _sv0t102;
      Value _sv0t103;
      _sv0t103.tag = 3;
      _sv0t103.p0 = t_ar;
      int _sv0t104 = sv0_box_alloc(3);
      sv0_box_store(_sv0t104, 0, _sv0t103.tag);
      sv0_box_store(_sv0t104, 1, _sv0t103.p0);
      sv0_box_store(_sv0t104, 2, _sv0t103.p1);
      sv0_vec_push(args_push, _sv0t104);
      int _sv0t105 = sv0_box_alloc(3);
      sv0_box_store(_sv0t105, 0, v_el.tag);
      sv0_box_store(_sv0t105, 1, v_el.p0);
      sv0_box_store(_sv0t105, 2, v_el.p1);
      sv0_vec_push(args_push, _sv0t105);
      int bid_push = 8;
      int fn_push = (0 - bid_push);
      Instr ins_push;
      ins_push.tag = 11;
      ins_push.p0 = 0;
      ins_push.p1 = fn_push;
      ins_push.p2 = args_push;
      ins_push.p3 = 0;
      int _sv0t106 = sv0_box_alloc(5);
      sv0_box_store(_sv0t106, 0, ins_push.tag);
      sv0_box_store(_sv0t106, 1, ins_push.p0);
      sv0_box_store(_sv0t106, 2, ins_push.p1);
      sv0_box_store(_sv0t106, 3, ins_push.p2);
      sv0_box_store(_sv0t106, 4, ins_push.p3);
      sv0_vec_push(out_instrs, _sv0t106);
      ak_ar = (ak_ar + 1);
    }
    Value _sv0t107;
    _sv0t107.tag = 3;
    _sv0t107.p0 = t_ar;
    return _sv0t107;
  } else {
  }
  if ((tag == 21)) {
    int _sv0t108 = sv0_vec_get(ed1, idx);
    int lo_ix = _sv0t108;
    int _sv0t109 = sv0_vec_get(ed2, idx);
    int hi_ix = _sv0t109;
    if ((lo_ix >= 0)) {
      int _sv0t110 = lower_for_effect(et, ed1, ed2, ed3, ed4, pp, tok_tags, lo_ix, ctr, out_instrs, enum_names, enum_tag_offsets, enum_tag_counts, enum_tags_flat, fn_ctx, builtin_map, lit_sf_names, source, starts, ends, item_tags, item_names, item_d2, item_d3, item_d4, item_field_counts, enum_vnames_flat, enum_max_payloads, item_fn_row, match_env_inherit, import_aliases);
    } else {
    }
    if ((hi_ix >= 0)) {
      int _sv0t111 = lower_for_effect(et, ed1, ed2, ed3, ed4, pp, tok_tags, hi_ix, ctr, out_instrs, enum_names, enum_tag_offsets, enum_tag_counts, enum_tags_flat, fn_ctx, builtin_map, lit_sf_names, source, starts, ends, item_tags, item_names, item_d2, item_d3, item_d4, item_field_counts, enum_vnames_flat, enum_max_payloads, item_fn_row, match_env_inherit, import_aliases);
    } else {
    }
    Value _sv0t112;
    _sv0t112.tag = 4;
    return _sv0t112;
  } else {
  }
  if ((tag == 9)) {
    int _sv0t113 = (0 - 1);
    Value _sv0t114 = lower_tag_block(et, ed1, ed2, ed3, ed4, pp, tok_tags, idx, ctr, out_instrs, enum_names, enum_tag_offsets, enum_tag_counts, enum_tags_flat, fn_ctx, builtin_map, lit_sf_names, source, starts, ends, item_tags, item_names, item_d2, item_d3, item_d4, item_field_counts, enum_vnames_flat, enum_max_payloads, item_fn_row, match_env_inherit, import_aliases, 0, _sv0t113);
    return _sv0t114;
  } else {
  }
  if ((tag == 10)) {
    Value _sv0t115 = lower_tag_if(et, ed1, ed2, ed3, ed4, pp, tok_tags, idx, ctr, out_instrs, enum_names, enum_tag_offsets, enum_tag_counts, enum_tags_flat, fn_ctx, builtin_map, lit_sf_names, source, starts, ends, item_tags, item_names, item_d2, item_d3, item_d4, item_field_counts, enum_vnames_flat, enum_max_payloads, item_fn_row, match_env_inherit, import_aliases);
    return _sv0t115;
  } else {
  }
  if ((tag == 11)) {
    Value _sv0t116 = lower_tag_match(et, ed1, ed2, ed3, ed4, pp, tok_tags, idx, ctr, out_instrs, enum_names, enum_tag_offsets, enum_tag_counts, enum_tags_flat, fn_ctx, builtin_map, lit_sf_names, source, starts, ends, item_tags, item_names, item_d2, item_d3, item_d4, item_field_counts, enum_vnames_flat, enum_max_payloads, item_fn_row, match_env_inherit, import_aliases);
    return _sv0t116;
  } else {
  }
  if ((tag == 12)) {
    int _sv0t117 = sv0_vec_get(ed1, idx);
    int cond_idx2 = _sv0t117;
    int _sv0t118 = sv0_vec_get(ed2, idx);
    int body_idx2 = _sv0t118;
    int _sv0t119 = sv0_vec_new();
    int cond_is = _sv0t119;
    Expr _sv0t120 = lower_cond_expr(et, ed1, ed2, ed3, ed4, pp, tok_tags, cond_idx2, ctr, cond_is, enum_names, enum_tag_offsets, enum_tag_counts, enum_tags_flat, fn_ctx, builtin_map, lit_sf_names, source, starts, ends, item_tags, item_names, item_d2, item_d3, item_d4, item_field_counts, enum_vnames_flat, enum_max_payloads, item_fn_row, match_env_inherit, import_aliases);
    Expr ce2;
    ce2 = _sv0t120;
    int _sv0t121 = sv0_vec_new();
    int body_is = _sv0t121;
    int _sv0t122 = lower_for_effect(et, ed1, ed2, ed3, ed4, pp, tok_tags, body_idx2, ctr, body_is, enum_names, enum_tag_offsets, enum_tag_counts, enum_tags_flat, fn_ctx, builtin_map, lit_sf_names, source, starts, ends, item_tags, item_names, item_d2, item_d3, item_d4, item_field_counts, enum_vnames_flat, enum_max_payloads, item_fn_row, match_env_inherit, import_aliases);
    int discard3 = _sv0t122;
    int _sv0t123 = sv0_vec_len(cond_is);
    if ((_sv0t123 == 0)) {
      Instr wl;
      int _sv0t124 = sv0_box_alloc(4);
      sv0_box_store(_sv0t124, 0, ce2.tag);
      sv0_box_store(_sv0t124, 1, ce2.p0);
      sv0_box_store(_sv0t124, 2, ce2.p1);
      sv0_box_store(_sv0t124, 3, ce2.p2);
      wl.tag = 7;
      wl.p0 = _sv0t124;
      wl.p1 = body_is;
      int _sv0t125 = sv0_box_alloc(5);
      sv0_box_store(_sv0t125, 0, wl.tag);
      sv0_box_store(_sv0t125, 1, wl.p0);
      sv0_box_store(_sv0t125, 2, wl.p1);
      sv0_box_store(_sv0t125, 3, wl.p2);
      sv0_box_store(_sv0t125, 4, wl.p3);
      sv0_vec_push(out_instrs, _sv0t125);
    } else {
      int _sv0t126 = ctr_fresh(ctr);
      int ct_w = _sv0t126;
      int _sv0t127 = sv0_vec_new();
      int loop_body = _sv0t127;
      int ci = 0;
      while (1) {
        int _sv0t128 = sv0_vec_len(cond_is);
        int _sv0t130 = (ci < _sv0t128);
        if ((!_sv0t130)) {
          break;
        } else {
        }
        int _sv0t129 = sv0_vec_get(cond_is, ci);
        sv0_vec_push(loop_body, _sv0t129);
        ci = (ci + 1);
      }
      Instr asg_w;
      int _sv0t131 = sv0_box_alloc(4);
      sv0_box_store(_sv0t131, 0, ce2.tag);
      sv0_box_store(_sv0t131, 1, ce2.p0);
      sv0_box_store(_sv0t131, 2, ce2.p1);
      sv0_box_store(_sv0t131, 3, ce2.p2);
      asg_w.tag = 3;
      asg_w.p0 = ct_w;
      asg_w.p1 = _sv0t131;
      int _sv0t132 = sv0_box_alloc(5);
      sv0_box_store(_sv0t132, 0, asg_w.tag);
      sv0_box_store(_sv0t132, 1, asg_w.p0);
      sv0_box_store(_sv0t132, 2, asg_w.p1);
      sv0_box_store(_sv0t132, 3, asg_w.p2);
      sv0_box_store(_sv0t132, 4, asg_w.p3);
      sv0_vec_push(loop_body, _sv0t132);
      Expr neg_w;
      Value _sv0t133;
      _sv0t133.tag = 3;
      _sv0t133.p0 = ct_w;
      int _sv0t134 = sv0_box_alloc(3);
      sv0_box_store(_sv0t134, 0, _sv0t133.tag);
      sv0_box_store(_sv0t134, 1, _sv0t133.p0);
      sv0_box_store(_sv0t134, 2, _sv0t133.p1);
      neg_w.tag = 3;
      neg_w.p0 = 1;
      neg_w.p1 = _sv0t134;
      int _sv0t135 = sv0_vec_new();
      int brk_blk = _sv0t135;
      Instr _sv0t136;
      _sv0t136.tag = 9;
      int _sv0t137 = sv0_box_alloc(5);
      sv0_box_store(_sv0t137, 0, _sv0t136.tag);
      sv0_box_store(_sv0t137, 1, _sv0t136.p0);
      sv0_box_store(_sv0t137, 2, _sv0t136.p1);
      sv0_box_store(_sv0t137, 3, _sv0t136.p2);
      sv0_box_store(_sv0t137, 4, _sv0t136.p3);
      sv0_vec_push(brk_blk, _sv0t137);
      int _sv0t138 = sv0_vec_new();
      int empty_else = _sv0t138;
      Instr if_w;
      int _sv0t139 = sv0_box_alloc(4);
      sv0_box_store(_sv0t139, 0, neg_w.tag);
      sv0_box_store(_sv0t139, 1, neg_w.p0);
      sv0_box_store(_sv0t139, 2, neg_w.p1);
      sv0_box_store(_sv0t139, 3, neg_w.p2);
      if_w.tag = 6;
      if_w.p0 = _sv0t139;
      if_w.p1 = brk_blk;
      if_w.p2 = empty_else;
      int _sv0t140 = sv0_box_alloc(5);
      sv0_box_store(_sv0t140, 0, if_w.tag);
      sv0_box_store(_sv0t140, 1, if_w.p0);
      sv0_box_store(_sv0t140, 2, if_w.p1);
      sv0_box_store(_sv0t140, 3, if_w.p2);
      sv0_box_store(_sv0t140, 4, if_w.p3);
      sv0_vec_push(loop_body, _sv0t140);
      int bi = 0;
      while (1) {
        int _sv0t141 = sv0_vec_len(body_is);
        int _sv0t143 = (bi < _sv0t141);
        if ((!_sv0t143)) {
          break;
        } else {
        }
        int _sv0t142 = sv0_vec_get(body_is, bi);
        sv0_vec_push(loop_body, _sv0t142);
        bi = (bi + 1);
      }
      Instr wl;
      Expr _sv0t144;
      Value _sv0t145;
      _sv0t145.tag = 1;
      int _sv0t146 = sv0_box_alloc(3);
      sv0_box_store(_sv0t146, 0, _sv0t145.tag);
      sv0_box_store(_sv0t146, 1, _sv0t145.p0);
      sv0_box_store(_sv0t146, 2, _sv0t145.p1);
      _sv0t144.tag = 0;
      _sv0t144.p0 = _sv0t146;
      int _sv0t147 = sv0_box_alloc(4);
      sv0_box_store(_sv0t147, 0, _sv0t144.tag);
      sv0_box_store(_sv0t147, 1, _sv0t144.p0);
      sv0_box_store(_sv0t147, 2, _sv0t144.p1);
      sv0_box_store(_sv0t147, 3, _sv0t144.p2);
      wl.tag = 7;
      wl.p0 = _sv0t147;
      wl.p1 = loop_body;
      int _sv0t148 = sv0_box_alloc(5);
      sv0_box_store(_sv0t148, 0, wl.tag);
      sv0_box_store(_sv0t148, 1, wl.p0);
      sv0_box_store(_sv0t148, 2, wl.p1);
      sv0_box_store(_sv0t148, 3, wl.p2);
      sv0_box_store(_sv0t148, 4, wl.p3);
      sv0_vec_push(out_instrs, _sv0t148);
    }
    Value _sv0t149;
    _sv0t149.tag = 4;
    return _sv0t149;
  } else {
  }
  if ((tag == 13)) {
    Value _sv0t150 = lower_tag_for(et, ed1, ed2, ed3, ed4, pp, tok_tags, idx, ctr, out_instrs, enum_names, enum_tag_offsets, enum_tag_counts, enum_tags_flat, fn_ctx, builtin_map, lit_sf_names, source, starts, ends, item_tags, item_names, item_d2, item_d3, item_d4, item_field_counts, enum_vnames_flat, enum_max_payloads, item_fn_row, match_env_inherit, import_aliases);
    return _sv0t150;
  } else {
  }
  if ((tag == 14)) {
    int _sv0t151 = sv0_vec_get(ed1, idx);
    int loop_body = _sv0t151;
    Value true_v;
    true_v.tag = 1;
    Expr true_e;
    int _sv0t152 = sv0_box_alloc(3);
    sv0_box_store(_sv0t152, 0, true_v.tag);
    sv0_box_store(_sv0t152, 1, true_v.p0);
    sv0_box_store(_sv0t152, 2, true_v.p1);
    true_e.tag = 0;
    true_e.p0 = _sv0t152;
    int _sv0t153 = sv0_vec_new();
    int loop_is = _sv0t153;
    int _sv0t154 = lower_for_effect(et, ed1, ed2, ed3, ed4, pp, tok_tags, loop_body, ctr, loop_is, enum_names, enum_tag_offsets, enum_tag_counts, enum_tags_flat, fn_ctx, builtin_map, lit_sf_names, source, starts, ends, item_tags, item_names, item_d2, item_d3, item_d4, item_field_counts, enum_vnames_flat, enum_max_payloads, item_fn_row, match_env_inherit, import_aliases);
    int discard4 = _sv0t154;
    Instr ll;
    int _sv0t155 = sv0_box_alloc(4);
    sv0_box_store(_sv0t155, 0, true_e.tag);
    sv0_box_store(_sv0t155, 1, true_e.p0);
    sv0_box_store(_sv0t155, 2, true_e.p1);
    sv0_box_store(_sv0t155, 3, true_e.p2);
    ll.tag = 7;
    ll.p0 = _sv0t155;
    ll.p1 = loop_is;
    int _sv0t156 = sv0_box_alloc(5);
    sv0_box_store(_sv0t156, 0, ll.tag);
    sv0_box_store(_sv0t156, 1, ll.p0);
    sv0_box_store(_sv0t156, 2, ll.p1);
    sv0_box_store(_sv0t156, 3, ll.p2);
    sv0_box_store(_sv0t156, 4, ll.p3);
    sv0_vec_push(out_instrs, _sv0t156);
    Value _sv0t157;
    _sv0t157.tag = 4;
    return _sv0t157;
  } else {
  }
  if ((tag == 15)) {
    int _sv0t158 = sv0_vec_get(ed1, idx);
    int val_idx = _sv0t158;
    if ((val_idx >= 0)) {
      Value _sv0t159 = lower_expr_to_value(et, ed1, ed2, ed3, ed4, pp, tok_tags, val_idx, ctr, out_instrs, enum_names, enum_tag_offsets, enum_tag_counts, enum_tags_flat, fn_ctx, builtin_map, lit_sf_names, source, starts, ends, item_tags, item_names, item_d2, item_d3, item_d4, item_field_counts, enum_vnames_flat, enum_max_payloads, item_fn_row, match_env_inherit, import_aliases);
      Value rv;
      rv = _sv0t159;
      Instr ret_instr;
      int _sv0t160 = sv0_box_alloc(3);
      sv0_box_store(_sv0t160, 0, rv.tag);
      sv0_box_store(_sv0t160, 1, rv.p0);
      sv0_box_store(_sv0t160, 2, rv.p1);
      ret_instr.tag = 15;
      ret_instr.p0 = _sv0t160;
      int _sv0t161 = sv0_box_alloc(5);
      sv0_box_store(_sv0t161, 0, ret_instr.tag);
      sv0_box_store(_sv0t161, 1, ret_instr.p0);
      sv0_box_store(_sv0t161, 2, ret_instr.p1);
      sv0_box_store(_sv0t161, 3, ret_instr.p2);
      sv0_box_store(_sv0t161, 4, ret_instr.p3);
      sv0_vec_push(out_instrs, _sv0t161);
      Value _sv0t162;
      _sv0t162.tag = 4;
      return _sv0t162;
    } else {
    }
    Instr ret_void;
    ret_void.tag = 14;
    int _sv0t163 = sv0_box_alloc(5);
    sv0_box_store(_sv0t163, 0, ret_void.tag);
    sv0_box_store(_sv0t163, 1, ret_void.p0);
    sv0_box_store(_sv0t163, 2, ret_void.p1);
    sv0_box_store(_sv0t163, 3, ret_void.p2);
    sv0_box_store(_sv0t163, 4, ret_void.p3);
    sv0_vec_push(out_instrs, _sv0t163);
    Value _sv0t164;
    _sv0t164.tag = 4;
    return _sv0t164;
  } else {
  }
  if ((tag == 16)) {
    Instr brk;
    brk.tag = 9;
    int _sv0t165 = sv0_box_alloc(5);
    sv0_box_store(_sv0t165, 0, brk.tag);
    sv0_box_store(_sv0t165, 1, brk.p0);
    sv0_box_store(_sv0t165, 2, brk.p1);
    sv0_box_store(_sv0t165, 3, brk.p2);
    sv0_box_store(_sv0t165, 4, brk.p3);
    sv0_vec_push(out_instrs, _sv0t165);
    Value _sv0t166;
    _sv0t166.tag = 4;
    return _sv0t166;
  } else {
  }
  if ((tag == 17)) {
    Instr cnt;
    cnt.tag = 10;
    int _sv0t167 = sv0_box_alloc(5);
    sv0_box_store(_sv0t167, 0, cnt.tag);
    sv0_box_store(_sv0t167, 1, cnt.p0);
    sv0_box_store(_sv0t167, 2, cnt.p1);
    sv0_box_store(_sv0t167, 3, cnt.p2);
    sv0_box_store(_sv0t167, 4, cnt.p3);
    sv0_vec_push(out_instrs, _sv0t167);
    Value _sv0t168;
    _sv0t168.tag = 4;
    return _sv0t168;
  } else {
  }
  if ((tag == 18)) {
    int _sv0t169 = sv0_vec_get(ed1, idx);
    int lhs_idx = _sv0t169;
    int _sv0t170 = sv0_vec_get(ed2, idx);
    int rhs_idx = _sv0t170;
    Expr _sv0t171 = lower_expr_with_instrs(et, ed1, ed2, ed3, ed4, pp, tok_tags, rhs_idx, ctr, out_instrs, enum_names, enum_tag_offsets, enum_tag_counts, enum_tags_flat, fn_ctx, builtin_map, lit_sf_names, source, starts, ends, item_tags, item_names, item_d2, item_d3, item_d4, item_field_counts, enum_vnames_flat, enum_max_payloads, item_fn_row, match_env_inherit, import_aliases);
    Expr rhs_e;
    rhs_e = _sv0t171;
    int _sv0t172 = sv0_vec_new();
    int aout = _sv0t172;
    int _sv0t173 = classify_assign_lhs_arena(et, ed1, ed2, pp, lhs_idx, aout);
    int cls = _sv0t173;
    if ((cls == 0)) {
      int _sv0t174 = sv0_vec_get(aout, 0);
      int var_tok = _sv0t174;
      Instr store;
      int _sv0t175 = sv0_box_alloc(4);
      sv0_box_store(_sv0t175, 0, rhs_e.tag);
      sv0_box_store(_sv0t175, 1, rhs_e.p0);
      sv0_box_store(_sv0t175, 2, rhs_e.p1);
      sv0_box_store(_sv0t175, 3, rhs_e.p2);
      store.tag = 4;
      store.p0 = var_tok;
      store.p1 = _sv0t175;
      int _sv0t176 = sv0_box_alloc(5);
      sv0_box_store(_sv0t176, 0, store.tag);
      sv0_box_store(_sv0t176, 1, store.p0);
      sv0_box_store(_sv0t176, 2, store.p1);
      sv0_box_store(_sv0t176, 3, store.p2);
      sv0_box_store(_sv0t176, 4, store.p3);
      sv0_vec_push(out_instrs, _sv0t176);
    } else {
    }
    if ((cls == 1)) {
      int _sv0t177 = sv0_vec_get(aout, 0);
      int obj_tok = _sv0t177;
      int _sv0t178 = sv0_vec_get(aout, 1);
      int fld_tok = _sv0t178;
      Instr sf;
      int _sv0t179 = sv0_box_alloc(4);
      sv0_box_store(_sv0t179, 0, rhs_e.tag);
      sv0_box_store(_sv0t179, 1, rhs_e.p0);
      sv0_box_store(_sv0t179, 2, rhs_e.p1);
      sv0_box_store(_sv0t179, 3, rhs_e.p2);
      sf.tag = 5;
      sf.p0 = obj_tok;
      sf.p1 = fld_tok;
      sf.p2 = _sv0t179;
      int _sv0t180 = sv0_box_alloc(5);
      sv0_box_store(_sv0t180, 0, sf.tag);
      sv0_box_store(_sv0t180, 1, sf.p0);
      sv0_box_store(_sv0t180, 2, sf.p1);
      sv0_box_store(_sv0t180, 3, sf.p2);
      sv0_box_store(_sv0t180, 4, sf.p3);
      sv0_vec_push(out_instrs, _sv0t180);
    } else {
    }
    Value _sv0t181;
    _sv0t181.tag = 4;
    return _sv0t181;
  } else {
  }
  if ((tag == 19)) {
    int _sv0t182 = sv0_vec_get(ed1, idx);
    int op_tag = _sv0t182;
    int _sv0t183 = sv0_vec_get(ed2, idx);
    int lhs_idx2 = _sv0t183;
    int _sv0t184 = sv0_vec_get(ed3, idx);
    int rhs_idx2 = _sv0t184;
    int _sv0t185 = sv0_vec_new();
    int aout2 = _sv0t185;
    int _sv0t186 = classify_assign_lhs_arena(et, ed1, ed2, pp, lhs_idx2, aout2);
    int cls2 = _sv0t186;
    if ((cls2 == 0)) {
      int _sv0t187 = sv0_vec_get(aout2, 0);
      int var_tok2 = _sv0t187;
      Value cur;
      cur.tag = 3;
      cur.p0 = var_tok2;
      Value _sv0t188 = lower_expr_to_value(et, ed1, ed2, ed3, ed4, pp, tok_tags, rhs_idx2, ctr, out_instrs, enum_names, enum_tag_offsets, enum_tag_counts, enum_tags_flat, fn_ctx, builtin_map, lit_sf_names, source, starts, ends, item_tags, item_names, item_d2, item_d3, item_d4, item_field_counts, enum_vnames_flat, enum_max_payloads, item_fn_row, match_env_inherit, import_aliases);
      Value rv2;
      rv2 = _sv0t188;
      Expr be;
      int _sv0t189 = sv0_box_alloc(3);
      sv0_box_store(_sv0t189, 0, cur.tag);
      sv0_box_store(_sv0t189, 1, cur.p0);
      sv0_box_store(_sv0t189, 2, cur.p1);
      int _sv0t190 = sv0_box_alloc(3);
      sv0_box_store(_sv0t190, 0, rv2.tag);
      sv0_box_store(_sv0t190, 1, rv2.p0);
      sv0_box_store(_sv0t190, 2, rv2.p1);
      be.tag = 2;
      be.p0 = op_tag;
      be.p1 = _sv0t189;
      be.p2 = _sv0t190;
      Instr st2;
      int _sv0t191 = sv0_box_alloc(4);
      sv0_box_store(_sv0t191, 0, be.tag);
      sv0_box_store(_sv0t191, 1, be.p0);
      sv0_box_store(_sv0t191, 2, be.p1);
      sv0_box_store(_sv0t191, 3, be.p2);
      st2.tag = 4;
      st2.p0 = var_tok2;
      st2.p1 = _sv0t191;
      int _sv0t192 = sv0_box_alloc(5);
      sv0_box_store(_sv0t192, 0, st2.tag);
      sv0_box_store(_sv0t192, 1, st2.p0);
      sv0_box_store(_sv0t192, 2, st2.p1);
      sv0_box_store(_sv0t192, 3, st2.p2);
      sv0_box_store(_sv0t192, 4, st2.p3);
      sv0_vec_push(out_instrs, _sv0t192);
    } else {
    }
    Value _sv0t193;
    _sv0t193.tag = 4;
    return _sv0t193;
  } else {
  }
  if ((tag == 20)) {
    int _sv0t194 = sv0_vec_get(ed1, idx);
    int cast_src = _sv0t194;
    Value _sv0t195 = lower_expr_to_value(et, ed1, ed2, ed3, ed4, pp, tok_tags, cast_src, ctr, out_instrs, enum_names, enum_tag_offsets, enum_tag_counts, enum_tags_flat, fn_ctx, builtin_map, lit_sf_names, source, starts, ends, item_tags, item_names, item_d2, item_d3, item_d4, item_field_counts, enum_vnames_flat, enum_max_payloads, item_fn_row, match_env_inherit, import_aliases);
    Value cv;
    cv = _sv0t195;
    int _sv0t196 = ctr_fresh(ctr);
    int ct2 = _sv0t196;
    Expr cast_e;
    int _sv0t197 = sv0_box_alloc(3);
    sv0_box_store(_sv0t197, 0, cv.tag);
    sv0_box_store(_sv0t197, 1, cv.p0);
    sv0_box_store(_sv0t197, 2, cv.p1);
    cast_e.tag = 3;
    cast_e.p0 = 99;
    cast_e.p1 = _sv0t197;
    Instr cast_i;
    int _sv0t198 = sv0_box_alloc(4);
    sv0_box_store(_sv0t198, 0, cast_e.tag);
    sv0_box_store(_sv0t198, 1, cast_e.p0);
    sv0_box_store(_sv0t198, 2, cast_e.p1);
    sv0_box_store(_sv0t198, 3, cast_e.p2);
    cast_i.tag = 3;
    cast_i.p0 = ct2;
    cast_i.p1 = _sv0t198;
    int _sv0t199 = sv0_box_alloc(5);
    sv0_box_store(_sv0t199, 0, cast_i.tag);
    sv0_box_store(_sv0t199, 1, cast_i.p0);
    sv0_box_store(_sv0t199, 2, cast_i.p1);
    sv0_box_store(_sv0t199, 3, cast_i.p2);
    sv0_box_store(_sv0t199, 4, cast_i.p3);
    sv0_vec_push(out_instrs, _sv0t199);
    Value _sv0t200;
    _sv0t200.tag = 3;
    _sv0t200.p0 = ct2;
    return _sv0t200;
  } else {
  }
  if ((tag == 22)) {
    Value _sv0t201 = lower_tag_try(et, ed1, ed2, ed3, ed4, pp, tok_tags, idx, ctr, out_instrs, enum_names, enum_tag_offsets, enum_tag_counts, enum_tags_flat, fn_ctx, builtin_map, lit_sf_names, source, starts, ends, item_tags, item_names, item_d2, item_d3, item_d4, item_field_counts, enum_vnames_flat, enum_max_payloads, item_fn_row, match_env_inherit, import_aliases);
    return _sv0t201;
  } else {
  }
  if ((tag == 24)) {
    int _sv0t202 = sv0_vec_get(ed1, idx);
    int pps_s = _sv0t202;
    int _sv0t203 = sv0_vec_get(ed2, idx);
    int ppc_s = _sv0t203;
    int _sv0t204 = sv0_vec_get(ed3, idx);
    int fc_s = _sv0t204;
    int _sv0t205 = sv0_vec_get(ed4, idx);
    int sf0 = _sv0t205;
    if ((ppc_s < 1)) {
      Value _sv0t206;
      _sv0t206.tag = 4;
      return _sv0t206;
    } else {
    }
    if ((fc_s <= 0)) {
      Value _sv0t207;
      _sv0t207.tag = 4;
      return _sv0t207;
    } else {
    }
    int _sv0t208 = (pps_s + ppc_s);
    int sty_idx = (_sv0t208 - 1);
    int _sv0t209 = sv0_vec_get(pp, sty_idx);
    int sty_tok = _sv0t209;
    int _sv0t210 = ctr_fresh(ctr);
    int t_s = _sv0t210;
    Instr decl_s;
    decl_s.tag = 2;
    decl_s.p0 = sty_tok;
    decl_s.p1 = t_s;
    int _sv0t211 = sv0_box_alloc(5);
    sv0_box_store(_sv0t211, 0, decl_s.tag);
    sv0_box_store(_sv0t211, 1, decl_s.p0);
    sv0_box_store(_sv0t211, 2, decl_s.p1);
    sv0_box_store(_sv0t211, 3, decl_s.p2);
    sv0_box_store(_sv0t211, 4, decl_s.p3);
    sv0_vec_push(out_instrs, _sv0t211);
    int fi_s = 0;
    while ((fi_s < fc_s)) {
      int _sv0t212 = (fi_s * 2);
      int _sv0t213 = (sf0 + _sv0t212);
      int _sv0t214 = (_sv0t213 + 1);
      int _sv0t215 = sv0_vec_get(lit_sf_names, _sv0t214);
      int ch_s = _sv0t215;
      Value _sv0t216 = lower_expr_to_value(et, ed1, ed2, ed3, ed4, pp, tok_tags, ch_s, ctr, out_instrs, enum_names, enum_tag_offsets, enum_tag_counts, enum_tags_flat, fn_ctx, builtin_map, lit_sf_names, source, starts, ends, item_tags, item_names, item_d2, item_d3, item_d4, item_field_counts, enum_vnames_flat, enum_max_payloads, item_fn_row, match_env_inherit, import_aliases);
      Value fv_s;
      fv_s = _sv0t216;
      Expr _sv0t217 = value_to_expr(fv_s);
      Expr fe_s;
      fe_s = _sv0t217;
      int _sv0t218 = (fi_s * 2);
      int _sv0t219 = (sf0 + _sv0t218);
      int _sv0t220 = sv0_vec_get(lit_sf_names, _sv0t219);
      int fnm_s = _sv0t220;
      Instr stf_s;
      int _sv0t221 = sv0_box_alloc(4);
      sv0_box_store(_sv0t221, 0, fe_s.tag);
      sv0_box_store(_sv0t221, 1, fe_s.p0);
      sv0_box_store(_sv0t221, 2, fe_s.p1);
      sv0_box_store(_sv0t221, 3, fe_s.p2);
      stf_s.tag = 5;
      stf_s.p0 = t_s;
      stf_s.p1 = fnm_s;
      stf_s.p2 = _sv0t221;
      int _sv0t222 = sv0_box_alloc(5);
      sv0_box_store(_sv0t222, 0, stf_s.tag);
      sv0_box_store(_sv0t222, 1, stf_s.p0);
      sv0_box_store(_sv0t222, 2, stf_s.p1);
      sv0_box_store(_sv0t222, 3, stf_s.p2);
      sv0_box_store(_sv0t222, 4, stf_s.p3);
      sv0_vec_push(out_instrs, _sv0t222);
      fi_s = (fi_s + 1);
    }
    Value _sv0t223;
    _sv0t223.tag = 3;
    _sv0t223.p0 = t_s;
    return _sv0t223;
  } else {
  }
  if ((tag == 25)) {
    int _sv0t224 = sv0_vec_get(ed2, idx);
    int elem_count = _sv0t224;
    if ((elem_count == 1)) {
      int _sv0t225 = sv0_vec_get(ed1, idx);
      int first_idx = _sv0t225;
      Value _sv0t226 = lower_expr_to_value(et, ed1, ed2, ed3, ed4, pp, tok_tags, first_idx, ctr, out_instrs, enum_names, enum_tag_offsets, enum_tag_counts, enum_tags_flat, fn_ctx, builtin_map, lit_sf_names, source, starts, ends, item_tags, item_names, item_d2, item_d3, item_d4, item_field_counts, enum_vnames_flat, enum_max_payloads, item_fn_row, match_env_inherit, import_aliases);
      return _sv0t226;
    } else {
    }
  } else {
  }
  if ((tag == 23)) {
    int _sv0t227 = sv0_vec_get(ed1, idx);
    int assert_expr = _sv0t227;
    int _sv0t228 = lower_for_effect(et, ed1, ed2, ed3, ed4, pp, tok_tags, assert_expr, ctr, out_instrs, enum_names, enum_tag_offsets, enum_tag_counts, enum_tags_flat, fn_ctx, builtin_map, lit_sf_names, source, starts, ends, item_tags, item_names, item_d2, item_d3, item_d4, item_field_counts, enum_vnames_flat, enum_max_payloads, item_fn_row, match_env_inherit, import_aliases);
    Value _sv0t229;
    _sv0t229.tag = 4;
    return _sv0t229;
  } else {
  }
  if ((tag == 30)) {
    Value _sv0t230;
    _sv0t230.tag = 4;
    return _sv0t230;
  } else {
  }
  Value _sv0t231;
  _sv0t231.tag = 4;
  return _sv0t231;
}

static Expr value_to_expr(Value v) {
  int _sv0t0 = ir_value_tag(v);
  int t = _sv0t0;
  if ((t == 3)) {
    int _sv0t1;
    if ((v.tag == 3)) {
      int x = v.p0;
      Expr _sv0t4;
      _sv0t4.tag = 1;
      _sv0t4.p0 = x;
      return _sv0t4;
      _sv0t1 = 0;
    } else {
      if (1) {
        Expr _sv0t2;
        int _sv0t3 = sv0_box_alloc(3);
        sv0_box_store(_sv0t3, 0, v.tag);
        sv0_box_store(_sv0t3, 1, v.p0);
        sv0_box_store(_sv0t3, 2, v.p1);
        _sv0t2.tag = 0;
        _sv0t2.p0 = _sv0t3;
        return _sv0t2;
        _sv0t1 = 0;
      } else {
      }
    }
  } else {
  }
  Expr _sv0t5;
  int _sv0t6 = sv0_box_alloc(3);
  sv0_box_store(_sv0t6, 0, v.tag);
  sv0_box_store(_sv0t6, 1, v.p0);
  sv0_box_store(_sv0t6, 2, v.p1);
  _sv0t5.tag = 0;
  _sv0t5.p0 = _sv0t6;
  return _sv0t5;
}

static Expr lower_expr_with_instrs(int et, int ed1, int ed2, int ed3, int ed4, int pp, int tok_tags, int idx, int ctr, int out_instrs, int enum_names, int enum_tag_offsets, int enum_tag_counts, int enum_tags_flat, int fn_ctx, int builtin_map, int lit_sf_names, const char* source, int starts, int ends, int item_tags, int item_names, int item_d2, int item_d3, int item_d4, int item_field_counts, int enum_vnames_flat, int enum_max_payloads, int item_fn_row, int match_env_inherit, int import_aliases) {
  int _sv0t0 = sv0_vec_get(et, idx);
  int etag = _sv0t0;
  if ((etag == 3)) {
    int _sv0t1 = sv0_vec_get(ed1, idx);
    int binop_tag = _sv0t1;
    int _sv0t2 = sv0_vec_get(ed2, idx);
    int lhs_idx = _sv0t2;
    int _sv0t3 = sv0_vec_get(ed3, idx);
    int rhs_idx = _sv0t3;
    Value _sv0t4 = lower_expr_to_value(et, ed1, ed2, ed3, ed4, pp, tok_tags, lhs_idx, ctr, out_instrs, enum_names, enum_tag_offsets, enum_tag_counts, enum_tags_flat, fn_ctx, builtin_map, lit_sf_names, source, starts, ends, item_tags, item_names, item_d2, item_d3, item_d4, item_field_counts, enum_vnames_flat, enum_max_payloads, item_fn_row, match_env_inherit, import_aliases);
    Value vl;
    vl = _sv0t4;
    Value _sv0t5 = lower_expr_to_value(et, ed1, ed2, ed3, ed4, pp, tok_tags, rhs_idx, ctr, out_instrs, enum_names, enum_tag_offsets, enum_tag_counts, enum_tags_flat, fn_ctx, builtin_map, lit_sf_names, source, starts, ends, item_tags, item_names, item_d2, item_d3, item_d4, item_field_counts, enum_vnames_flat, enum_max_payloads, item_fn_row, match_env_inherit, import_aliases);
    Value vr;
    vr = _sv0t5;
    Expr _sv0t6;
    int _sv0t7 = sv0_box_alloc(3);
    sv0_box_store(_sv0t7, 0, vl.tag);
    sv0_box_store(_sv0t7, 1, vl.p0);
    sv0_box_store(_sv0t7, 2, vl.p1);
    int _sv0t8 = sv0_box_alloc(3);
    sv0_box_store(_sv0t8, 0, vr.tag);
    sv0_box_store(_sv0t8, 1, vr.p0);
    sv0_box_store(_sv0t8, 2, vr.p1);
    _sv0t6.tag = 2;
    _sv0t6.p0 = binop_tag;
    _sv0t6.p1 = _sv0t7;
    _sv0t6.p2 = _sv0t8;
    return _sv0t6;
  } else {
  }
  Value _sv0t9 = lower_expr_to_value(et, ed1, ed2, ed3, ed4, pp, tok_tags, idx, ctr, out_instrs, enum_names, enum_tag_offsets, enum_tag_counts, enum_tags_flat, fn_ctx, builtin_map, lit_sf_names, source, starts, ends, item_tags, item_names, item_d2, item_d3, item_d4, item_field_counts, enum_vnames_flat, enum_max_payloads, item_fn_row, match_env_inherit, import_aliases);
  Value v;
  v = _sv0t9;
  Expr _sv0t10 = value_to_expr(v);
  return _sv0t10;
}

static int lower_for_effect(int et, int ed1, int ed2, int ed3, int ed4, int pp, int tok_tags, int idx, int ctr, int out_instrs, int enum_names, int enum_tag_offsets, int enum_tag_counts, int enum_tags_flat, int fn_ctx, int builtin_map, int lit_sf_names, const char* source, int starts, int ends, int item_tags, int item_names, int item_d2, int item_d3, int item_d4, int item_field_counts, int enum_vnames_flat, int enum_max_payloads, int item_fn_row, int match_env_inherit, int import_aliases) {
  int _sv0t0 = sv0_vec_get(et, idx);
  int etag = _sv0t0;
  if ((etag == 9)) {
    int _sv0t1 = (0 - 1);
    Value _sv0t2 = lower_tag_block(et, ed1, ed2, ed3, ed4, pp, tok_tags, idx, ctr, out_instrs, enum_names, enum_tag_offsets, enum_tag_counts, enum_tags_flat, fn_ctx, builtin_map, lit_sf_names, source, starts, ends, item_tags, item_names, item_d2, item_d3, item_d4, item_field_counts, enum_vnames_flat, enum_max_payloads, item_fn_row, match_env_inherit, import_aliases, 1, _sv0t1);
    return 0;
  } else {
  }
  if ((etag == 10)) {
    int _sv0t3 = sv0_vec_get(ed1, idx);
    int cond_idx = _sv0t3;
    int _sv0t4 = sv0_vec_get(ed2, idx);
    int then_idx = _sv0t4;
    int _sv0t5 = sv0_vec_get(ed3, idx);
    int else_idx = _sv0t5;
    Expr _sv0t6 = lower_cond_expr(et, ed1, ed2, ed3, ed4, pp, tok_tags, cond_idx, ctr, out_instrs, enum_names, enum_tag_offsets, enum_tag_counts, enum_tags_flat, fn_ctx, builtin_map, lit_sf_names, source, starts, ends, item_tags, item_names, item_d2, item_d3, item_d4, item_field_counts, enum_vnames_flat, enum_max_payloads, item_fn_row, match_env_inherit, import_aliases);
    Expr cond_e;
    cond_e = _sv0t6;
    int _sv0t7 = sv0_vec_new();
    int then_is = _sv0t7;
    int _sv0t8 = lower_for_effect(et, ed1, ed2, ed3, ed4, pp, tok_tags, then_idx, ctr, then_is, enum_names, enum_tag_offsets, enum_tag_counts, enum_tags_flat, fn_ctx, builtin_map, lit_sf_names, source, starts, ends, item_tags, item_names, item_d2, item_d3, item_d4, item_field_counts, enum_vnames_flat, enum_max_payloads, item_fn_row, match_env_inherit, import_aliases);
    int rt = _sv0t8;
    int _sv0t9 = sv0_vec_new();
    int else_is = _sv0t9;
    if ((else_idx >= 0)) {
      int _sv0t10 = lower_for_effect(et, ed1, ed2, ed3, ed4, pp, tok_tags, else_idx, ctr, else_is, enum_names, enum_tag_offsets, enum_tag_counts, enum_tags_flat, fn_ctx, builtin_map, lit_sf_names, source, starts, ends, item_tags, item_names, item_d2, item_d3, item_d4, item_field_counts, enum_vnames_flat, enum_max_payloads, item_fn_row, match_env_inherit, import_aliases);
      int re = _sv0t10;
    } else {
    }
    Instr _sv0t11;
    int _sv0t12 = sv0_box_alloc(4);
    sv0_box_store(_sv0t12, 0, cond_e.tag);
    sv0_box_store(_sv0t12, 1, cond_e.p0);
    sv0_box_store(_sv0t12, 2, cond_e.p1);
    sv0_box_store(_sv0t12, 3, cond_e.p2);
    _sv0t11.tag = 6;
    _sv0t11.p0 = _sv0t12;
    _sv0t11.p1 = then_is;
    _sv0t11.p2 = else_is;
    int _sv0t13 = sv0_box_alloc(5);
    sv0_box_store(_sv0t13, 0, _sv0t11.tag);
    sv0_box_store(_sv0t13, 1, _sv0t11.p0);
    sv0_box_store(_sv0t13, 2, _sv0t11.p1);
    sv0_box_store(_sv0t13, 3, _sv0t11.p2);
    sv0_box_store(_sv0t13, 4, _sv0t11.p3);
    sv0_vec_push(out_instrs, _sv0t13);
    return 0;
  } else {
  }
  Value _sv0t14 = lower_expr_to_value(et, ed1, ed2, ed3, ed4, pp, tok_tags, idx, ctr, out_instrs, enum_names, enum_tag_offsets, enum_tag_counts, enum_tags_flat, fn_ctx, builtin_map, lit_sf_names, source, starts, ends, item_tags, item_names, item_d2, item_d3, item_d4, item_field_counts, enum_vnames_flat, enum_max_payloads, item_fn_row, match_env_inherit, import_aliases);
  Value discard5;
  discard5 = _sv0t14;
  return 0;
}

static int lower_into_var_instrs(int et, int ed1, int ed2, int ed3, int ed4, int pp, int tok_tags, int body_idx, int dest, int ctr, int out_instrs, int enum_names, int enum_tag_offsets, int enum_tag_counts, int enum_tags_flat, int fn_ctx, int builtin_map, int lit_sf_names, const char* source, int starts, int ends, int item_tags, int item_names, int item_d2, int item_d3, int item_d4, int item_field_counts, int enum_vnames_flat, int enum_max_payloads, int item_fn_row, int match_env_inherit, int import_aliases) {
  int _sv0t0 = sv0_vec_get(et, body_idx);
  int btag = _sv0t0;
  if ((btag == 9)) {
    Value _sv0t1 = lower_tag_block(et, ed1, ed2, ed3, ed4, pp, tok_tags, body_idx, ctr, out_instrs, enum_names, enum_tag_offsets, enum_tag_counts, enum_tags_flat, fn_ctx, builtin_map, lit_sf_names, source, starts, ends, item_tags, item_names, item_d2, item_d3, item_d4, item_field_counts, enum_vnames_flat, enum_max_payloads, item_fn_row, match_env_inherit, import_aliases, 0, dest);
    return 0;
  } else {
  }
  if ((btag == 10)) {
    int _sv0t2 = sv0_vec_get(ed3, body_idx);
    int else_idx_iv = _sv0t2;
    if ((else_idx_iv < 0)) {
      int _sv0t3 = sv0_vec_get(ed1, body_idx);
      int cond_idx_iv = _sv0t3;
      int _sv0t4 = sv0_vec_get(ed2, body_idx);
      int then_idx_iv = _sv0t4;
      Expr _sv0t5 = lower_cond_expr(et, ed1, ed2, ed3, ed4, pp, tok_tags, cond_idx_iv, ctr, out_instrs, enum_names, enum_tag_offsets, enum_tag_counts, enum_tags_flat, fn_ctx, builtin_map, lit_sf_names, source, starts, ends, item_tags, item_names, item_d2, item_d3, item_d4, item_field_counts, enum_vnames_flat, enum_max_payloads, item_fn_row, match_env_inherit, import_aliases);
      Expr cond_e_iv;
      cond_e_iv = _sv0t5;
      int _sv0t6 = sv0_vec_new();
      int then_is_iv = _sv0t6;
      int _sv0t7 = lower_into_var_instrs(et, ed1, ed2, ed3, ed4, pp, tok_tags, then_idx_iv, dest, ctr, then_is_iv, enum_names, enum_tag_offsets, enum_tag_counts, enum_tags_flat, fn_ctx, builtin_map, lit_sf_names, source, starts, ends, item_tags, item_names, item_d2, item_d3, item_d4, item_field_counts, enum_vnames_flat, enum_max_payloads, item_fn_row, match_env_inherit, import_aliases);
      int _sv0t8 = sv0_vec_new();
      int else_is_iv = _sv0t8;
      Instr _sv0t9;
      Expr _sv0t10;
      Value _sv0t11;
      _sv0t11.tag = 4;
      int _sv0t12 = sv0_box_alloc(3);
      sv0_box_store(_sv0t12, 0, _sv0t11.tag);
      sv0_box_store(_sv0t12, 1, _sv0t11.p0);
      sv0_box_store(_sv0t12, 2, _sv0t11.p1);
      _sv0t10.tag = 0;
      _sv0t10.p0 = _sv0t12;
      int _sv0t13 = sv0_box_alloc(4);
      sv0_box_store(_sv0t13, 0, _sv0t10.tag);
      sv0_box_store(_sv0t13, 1, _sv0t10.p0);
      sv0_box_store(_sv0t13, 2, _sv0t10.p1);
      sv0_box_store(_sv0t13, 3, _sv0t10.p2);
      _sv0t9.tag = 4;
      _sv0t9.p0 = dest;
      _sv0t9.p1 = _sv0t13;
      int _sv0t14 = sv0_box_alloc(5);
      sv0_box_store(_sv0t14, 0, _sv0t9.tag);
      sv0_box_store(_sv0t14, 1, _sv0t9.p0);
      sv0_box_store(_sv0t14, 2, _sv0t9.p1);
      sv0_box_store(_sv0t14, 3, _sv0t9.p2);
      sv0_box_store(_sv0t14, 4, _sv0t9.p3);
      sv0_vec_push(else_is_iv, _sv0t14);
      Instr _sv0t15;
      int _sv0t16 = sv0_box_alloc(4);
      sv0_box_store(_sv0t16, 0, cond_e_iv.tag);
      sv0_box_store(_sv0t16, 1, cond_e_iv.p0);
      sv0_box_store(_sv0t16, 2, cond_e_iv.p1);
      sv0_box_store(_sv0t16, 3, cond_e_iv.p2);
      _sv0t15.tag = 6;
      _sv0t15.p0 = _sv0t16;
      _sv0t15.p1 = then_is_iv;
      _sv0t15.p2 = else_is_iv;
      int _sv0t17 = sv0_box_alloc(5);
      sv0_box_store(_sv0t17, 0, _sv0t15.tag);
      sv0_box_store(_sv0t17, 1, _sv0t15.p0);
      sv0_box_store(_sv0t17, 2, _sv0t15.p1);
      sv0_box_store(_sv0t17, 3, _sv0t15.p2);
      sv0_box_store(_sv0t17, 4, _sv0t15.p3);
      sv0_vec_push(out_instrs, _sv0t17);
      return 0;
    } else {
      int _sv0t18 = sv0_vec_get(ed1, body_idx);
      int cond_idx_we = _sv0t18;
      int _sv0t19 = sv0_vec_get(ed2, body_idx);
      int then_idx_we = _sv0t19;
      int _sv0t20 = ctr_fresh(ctr);
      int u_we = _sv0t20;
      Expr _sv0t21 = lower_cond_expr(et, ed1, ed2, ed3, ed4, pp, tok_tags, cond_idx_we, ctr, out_instrs, enum_names, enum_tag_offsets, enum_tag_counts, enum_tags_flat, fn_ctx, builtin_map, lit_sf_names, source, starts, ends, item_tags, item_names, item_d2, item_d3, item_d4, item_field_counts, enum_vnames_flat, enum_max_payloads, item_fn_row, match_env_inherit, import_aliases);
      Expr cond_e_we;
      cond_e_we = _sv0t21;
      Instr _sv0t22;
      _sv0t22.tag = 1;
      _sv0t22.p0 = u_we;
      int _sv0t23 = sv0_box_alloc(5);
      sv0_box_store(_sv0t23, 0, _sv0t22.tag);
      sv0_box_store(_sv0t23, 1, _sv0t22.p0);
      sv0_box_store(_sv0t23, 2, _sv0t22.p1);
      sv0_box_store(_sv0t23, 3, _sv0t22.p2);
      sv0_box_store(_sv0t23, 4, _sv0t22.p3);
      sv0_vec_push(out_instrs, _sv0t23);
      int _sv0t24 = sv0_vec_new();
      int then_we = _sv0t24;
      int _sv0t25 = lower_into_var_instrs(et, ed1, ed2, ed3, ed4, pp, tok_tags, then_idx_we, u_we, ctr, then_we, enum_names, enum_tag_offsets, enum_tag_counts, enum_tags_flat, fn_ctx, builtin_map, lit_sf_names, source, starts, ends, item_tags, item_names, item_d2, item_d3, item_d4, item_field_counts, enum_vnames_flat, enum_max_payloads, item_fn_row, match_env_inherit, import_aliases);
      int _sv0t26 = sv0_vec_new();
      int else_we = _sv0t26;
      int _sv0t27 = lower_into_var_instrs(et, ed1, ed2, ed3, ed4, pp, tok_tags, else_idx_iv, u_we, ctr, else_we, enum_names, enum_tag_offsets, enum_tag_counts, enum_tags_flat, fn_ctx, builtin_map, lit_sf_names, source, starts, ends, item_tags, item_names, item_d2, item_d3, item_d4, item_field_counts, enum_vnames_flat, enum_max_payloads, item_fn_row, match_env_inherit, import_aliases);
      Instr _sv0t28;
      int _sv0t29 = sv0_box_alloc(4);
      sv0_box_store(_sv0t29, 0, cond_e_we.tag);
      sv0_box_store(_sv0t29, 1, cond_e_we.p0);
      sv0_box_store(_sv0t29, 2, cond_e_we.p1);
      sv0_box_store(_sv0t29, 3, cond_e_we.p2);
      _sv0t28.tag = 6;
      _sv0t28.p0 = _sv0t29;
      _sv0t28.p1 = then_we;
      _sv0t28.p2 = else_we;
      int _sv0t30 = sv0_box_alloc(5);
      sv0_box_store(_sv0t30, 0, _sv0t28.tag);
      sv0_box_store(_sv0t30, 1, _sv0t28.p0);
      sv0_box_store(_sv0t30, 2, _sv0t28.p1);
      sv0_box_store(_sv0t30, 3, _sv0t28.p2);
      sv0_box_store(_sv0t30, 4, _sv0t28.p3);
      sv0_vec_push(out_instrs, _sv0t30);
      Instr _sv0t31;
      Expr _sv0t32;
      _sv0t32.tag = 1;
      _sv0t32.p0 = u_we;
      int _sv0t33 = sv0_box_alloc(4);
      sv0_box_store(_sv0t33, 0, _sv0t32.tag);
      sv0_box_store(_sv0t33, 1, _sv0t32.p0);
      sv0_box_store(_sv0t33, 2, _sv0t32.p1);
      sv0_box_store(_sv0t33, 3, _sv0t32.p2);
      _sv0t31.tag = 4;
      _sv0t31.p0 = dest;
      _sv0t31.p1 = _sv0t33;
      int _sv0t34 = sv0_box_alloc(5);
      sv0_box_store(_sv0t34, 0, _sv0t31.tag);
      sv0_box_store(_sv0t34, 1, _sv0t31.p0);
      sv0_box_store(_sv0t34, 2, _sv0t31.p1);
      sv0_box_store(_sv0t34, 3, _sv0t31.p2);
      sv0_box_store(_sv0t34, 4, _sv0t31.p3);
      sv0_vec_push(out_instrs, _sv0t34);
      return 0;
    }
  } else {
  }
  Expr _sv0t35 = lower_expr_with_instrs(et, ed1, ed2, ed3, ed4, pp, tok_tags, body_idx, ctr, out_instrs, enum_names, enum_tag_offsets, enum_tag_counts, enum_tags_flat, fn_ctx, builtin_map, lit_sf_names, source, starts, ends, item_tags, item_names, item_d2, item_d3, item_d4, item_field_counts, enum_vnames_flat, enum_max_payloads, item_fn_row, match_env_inherit, import_aliases);
  Expr ee;
  ee = _sv0t35;
  Instr si;
  int _sv0t36 = sv0_box_alloc(4);
  sv0_box_store(_sv0t36, 0, ee.tag);
  sv0_box_store(_sv0t36, 1, ee.p0);
  sv0_box_store(_sv0t36, 2, ee.p1);
  sv0_box_store(_sv0t36, 3, ee.p2);
  si.tag = 4;
  si.p0 = dest;
  si.p1 = _sv0t36;
  int _sv0t37 = sv0_box_alloc(5);
  sv0_box_store(_sv0t37, 0, si.tag);
  sv0_box_store(_sv0t37, 1, si.p0);
  sv0_box_store(_sv0t37, 2, si.p1);
  sv0_box_store(_sv0t37, 3, si.p2);
  sv0_box_store(_sv0t37, 4, si.p3);
  sv0_vec_push(out_instrs, _sv0t37);
  return 0;
}

static int bind_scrut_var(int et, int ed1, int ed2, int ed3, int ed4, int pp, int tok_tags, int scrut_idx, int ctr, int out_instrs, int enum_names, int enum_tag_offsets, int enum_tag_counts, int enum_tags_flat, int fn_ctx, int builtin_map, int lit_sf_names, int item_tags, int item_names, int item_d2, int item_d3, int item_d4, int item_field_counts, int enum_vnames_flat, int enum_max_payloads, int item_fn_row, int match_env_ctx, int scrut_cty, const char* source, int starts, int ends, int import_aliases) {
  Value _sv0t0 = lower_expr_to_value(et, ed1, ed2, ed3, ed4, pp, tok_tags, scrut_idx, ctr, out_instrs, enum_names, enum_tag_offsets, enum_tag_counts, enum_tags_flat, fn_ctx, builtin_map, lit_sf_names, source, starts, ends, item_tags, item_names, item_d2, item_d3, item_d4, item_field_counts, enum_vnames_flat, enum_max_payloads, item_fn_row, match_env_ctx, import_aliases);
  Value v;
  v = _sv0t0;
  int _sv0t1 = ir_value_tag(v);
  int vtag = _sv0t1;
  if ((vtag == 3)) {
    int _sv0t2;
    if ((v.tag == 3)) {
      int x = v.p0;
      return x;
      _sv0t2 = 0;
    } else {
      if (1) {
        int _sv0t3 = (0 - 1);
        return _sv0t3;
        _sv0t2 = 0;
      } else {
      }
    }
  } else {
  }
  if ((vtag == 0)) {
    int _sv0t4 = ctr_fresh(ctr);
    int t = _sv0t4;
    Instr ai;
    Expr _sv0t5;
    int _sv0t6 = sv0_box_alloc(3);
    sv0_box_store(_sv0t6, 0, v.tag);
    sv0_box_store(_sv0t6, 1, v.p0);
    sv0_box_store(_sv0t6, 2, v.p1);
    _sv0t5.tag = 0;
    _sv0t5.p0 = _sv0t6;
    int _sv0t7 = sv0_box_alloc(4);
    sv0_box_store(_sv0t7, 0, _sv0t5.tag);
    sv0_box_store(_sv0t7, 1, _sv0t5.p0);
    sv0_box_store(_sv0t7, 2, _sv0t5.p1);
    sv0_box_store(_sv0t7, 3, _sv0t5.p2);
    ai.tag = 3;
    ai.p0 = t;
    ai.p1 = _sv0t7;
    int _sv0t8 = sv0_box_alloc(5);
    sv0_box_store(_sv0t8, 0, ai.tag);
    sv0_box_store(_sv0t8, 1, ai.p0);
    sv0_box_store(_sv0t8, 2, ai.p1);
    sv0_box_store(_sv0t8, 3, ai.p2);
    sv0_box_store(_sv0t8, 4, ai.p3);
    sv0_vec_push(out_instrs, _sv0t8);
    return t;
  } else {
  }
  if ((vtag == 1)) {
    int _sv0t9 = ctr_fresh(ctr);
    int t = _sv0t9;
    Instr ai;
    Expr _sv0t10;
    int _sv0t11 = sv0_box_alloc(3);
    sv0_box_store(_sv0t11, 0, v.tag);
    sv0_box_store(_sv0t11, 1, v.p0);
    sv0_box_store(_sv0t11, 2, v.p1);
    _sv0t10.tag = 0;
    _sv0t10.p0 = _sv0t11;
    int _sv0t12 = sv0_box_alloc(4);
    sv0_box_store(_sv0t12, 0, _sv0t10.tag);
    sv0_box_store(_sv0t12, 1, _sv0t10.p0);
    sv0_box_store(_sv0t12, 2, _sv0t10.p1);
    sv0_box_store(_sv0t12, 3, _sv0t10.p2);
    ai.tag = 3;
    ai.p0 = t;
    ai.p1 = _sv0t12;
    int _sv0t13 = sv0_box_alloc(5);
    sv0_box_store(_sv0t13, 0, ai.tag);
    sv0_box_store(_sv0t13, 1, ai.p0);
    sv0_box_store(_sv0t13, 2, ai.p1);
    sv0_box_store(_sv0t13, 3, ai.p2);
    sv0_box_store(_sv0t13, 4, ai.p3);
    sv0_vec_push(out_instrs, _sv0t13);
    return t;
  } else {
  }
  if ((vtag == 2)) {
    int _sv0t14 = ctr_fresh(ctr);
    int t = _sv0t14;
    Instr ai;
    Expr _sv0t15;
    int _sv0t16 = sv0_box_alloc(3);
    sv0_box_store(_sv0t16, 0, v.tag);
    sv0_box_store(_sv0t16, 1, v.p0);
    sv0_box_store(_sv0t16, 2, v.p1);
    _sv0t15.tag = 0;
    _sv0t15.p0 = _sv0t16;
    int _sv0t17 = sv0_box_alloc(4);
    sv0_box_store(_sv0t17, 0, _sv0t15.tag);
    sv0_box_store(_sv0t17, 1, _sv0t15.p0);
    sv0_box_store(_sv0t17, 2, _sv0t15.p1);
    sv0_box_store(_sv0t17, 3, _sv0t15.p2);
    ai.tag = 3;
    ai.p0 = t;
    ai.p1 = _sv0t17;
    int _sv0t18 = sv0_box_alloc(5);
    sv0_box_store(_sv0t18, 0, ai.tag);
    sv0_box_store(_sv0t18, 1, ai.p0);
    sv0_box_store(_sv0t18, 2, ai.p1);
    sv0_box_store(_sv0t18, 3, ai.p2);
    sv0_box_store(_sv0t18, 4, ai.p3);
    sv0_vec_push(out_instrs, _sv0t18);
    return t;
  } else {
  }
  int _sv0t19 = ctr_fresh(ctr);
  int t = _sv0t19;
  Expr _sv0t20 = value_to_expr(v);
  Expr ee;
  ee = _sv0t20;
  int is_scalar = 0;
  if ((scrut_cty < 0)) {
    is_scalar = 1;
  } else {
  }
  if ((is_scalar == 0)) {
    const char* _sv0t21 = handle_to_str(scrut_cty, source, starts, ends);
    const char* tname;
    tname = _sv0t21;
    int _sv0t22 = sv0_string_eq(tname, "i32");
    if (_sv0t22) {
      is_scalar = 1;
    } else {
    }
    int _sv0t23 = sv0_string_eq(tname, "bool");
    if (_sv0t23) {
      is_scalar = 1;
    } else {
    }
  } else {
  }
  if ((is_scalar == 1)) {
    Instr ai2;
    int _sv0t24 = sv0_box_alloc(4);
    sv0_box_store(_sv0t24, 0, ee.tag);
    sv0_box_store(_sv0t24, 1, ee.p0);
    sv0_box_store(_sv0t24, 2, ee.p1);
    sv0_box_store(_sv0t24, 3, ee.p2);
    ai2.tag = 3;
    ai2.p0 = t;
    ai2.p1 = _sv0t24;
    int _sv0t25 = sv0_box_alloc(5);
    sv0_box_store(_sv0t25, 0, ai2.tag);
    sv0_box_store(_sv0t25, 1, ai2.p0);
    sv0_box_store(_sv0t25, 2, ai2.p1);
    sv0_box_store(_sv0t25, 3, ai2.p2);
    sv0_box_store(_sv0t25, 4, ai2.p3);
    sv0_vec_push(out_instrs, _sv0t25);
  } else {
    Instr di;
    di.tag = 2;
    di.p0 = scrut_cty;
    di.p1 = t;
    Instr si;
    int _sv0t26 = sv0_box_alloc(4);
    sv0_box_store(_sv0t26, 0, ee.tag);
    sv0_box_store(_sv0t26, 1, ee.p0);
    sv0_box_store(_sv0t26, 2, ee.p1);
    sv0_box_store(_sv0t26, 3, ee.p2);
    si.tag = 4;
    si.p0 = t;
    si.p1 = _sv0t26;
    int _sv0t27 = sv0_box_alloc(5);
    sv0_box_store(_sv0t27, 0, di.tag);
    sv0_box_store(_sv0t27, 1, di.p0);
    sv0_box_store(_sv0t27, 2, di.p1);
    sv0_box_store(_sv0t27, 3, di.p2);
    sv0_box_store(_sv0t27, 4, di.p3);
    sv0_vec_push(out_instrs, _sv0t27);
    int _sv0t28 = sv0_box_alloc(5);
    sv0_box_store(_sv0t28, 0, si.tag);
    sv0_box_store(_sv0t28, 1, si.p0);
    sv0_box_store(_sv0t28, 2, si.p1);
    sv0_box_store(_sv0t28, 3, si.p2);
    sv0_box_store(_sv0t28, 4, si.p3);
    sv0_vec_push(out_instrs, _sv0t28);
  }
  return t;
}

static int lower_match_arms(int et, int ed1, int ed2, int ed3, int ed4, int pp, int tok_tags, int arms_first, int arm_count, int arms_sidecar, int out_var, int scrut_var, int scrut_cty, int ctr, int out_instrs, int enum_names, int enum_tag_offsets, int enum_tag_counts, int enum_tags_flat, int fn_ctx, int builtin_map, int lit_sf_names, int item_tags, int item_names, int item_d2, int item_d3, int item_d4, int item_field_counts, int enum_vnames_flat, int enum_max_payloads, int item_fn_row, int match_env, const char* source, int starts, int ends, int import_aliases) {
  int _sv0t0 = sv0_vec_new();
  int result_instrs = _sv0t0;
  int ai = (arm_count - 1);
  while ((ai >= 0)) {
    int _sv0t1 = block_stmt_index(pp, arms_first, arms_sidecar, ai);
    int arm_idx = _sv0t1;
    int _sv0t2 = sv0_vec_get(et, arm_idx);
    int arm_tag = _sv0t2;
    if ((arm_tag != 29)) {
      int _sv0t3 = (0 - 1);
      return _sv0t3;
    } else {
    }
    int _sv0t4 = sv0_vec_get(ed1, arm_idx);
    int pat_tag = _sv0t4;
    int _sv0t5 = sv0_vec_get(ed2, arm_idx);
    int pat_d1 = _sv0t5;
    int _sv0t6 = sv0_vec_get(ed3, arm_idx);
    int guard_idx = _sv0t6;
    int _sv0t7 = sv0_vec_get(ed4, arm_idx);
    int body_idx = _sv0t7;
    Expr cexp;
    Value _sv0t8;
    _sv0t8.tag = 1;
    int _sv0t9 = sv0_box_alloc(3);
    sv0_box_store(_sv0t9, 0, _sv0t8.tag);
    sv0_box_store(_sv0t9, 1, _sv0t8.p0);
    sv0_box_store(_sv0t9, 2, _sv0t8.p1);
    cexp.tag = 0;
    cexp.p0 = _sv0t9;
    int _sv0t10 = sv0_vec_new();
    int bind_instrs = _sv0t10;
    if ((pat_tag == 0)) {
      Expr _sv0t11;
      Value _sv0t12;
      _sv0t12.tag = 1;
      int _sv0t13 = sv0_box_alloc(3);
      sv0_box_store(_sv0t13, 0, _sv0t12.tag);
      sv0_box_store(_sv0t13, 1, _sv0t12.p0);
      sv0_box_store(_sv0t13, 2, _sv0t12.p1);
      _sv0t11.tag = 0;
      _sv0t11.p0 = _sv0t13;
      cexp = _sv0t11;
    } else {
    }
    if ((pat_tag == 1)) {
      Expr _sv0t14;
      Value _sv0t15;
      _sv0t15.tag = 1;
      int _sv0t16 = sv0_box_alloc(3);
      sv0_box_store(_sv0t16, 0, _sv0t15.tag);
      sv0_box_store(_sv0t16, 1, _sv0t15.p0);
      sv0_box_store(_sv0t16, 2, _sv0t15.p1);
      _sv0t14.tag = 0;
      _sv0t14.p0 = _sv0t16;
      cexp = _sv0t14;
      int is_scalar = 0;
      if ((scrut_cty < 0)) {
        is_scalar = 1;
      } else {
      }
      if ((is_scalar == 0)) {
        const char* _sv0t17 = handle_to_str(scrut_cty, source, starts, ends);
        const char* tname;
        tname = _sv0t17;
        int _sv0t18 = sv0_string_eq(tname, "i32");
        if (_sv0t18) {
          is_scalar = 1;
        } else {
        }
        int _sv0t19 = sv0_string_eq(tname, "bool");
        if (_sv0t19) {
          is_scalar = 1;
        } else {
        }
      } else {
      }
      if ((is_scalar == 1)) {
        Instr ba;
        Expr _sv0t20;
        _sv0t20.tag = 1;
        _sv0t20.p0 = scrut_var;
        int _sv0t21 = sv0_box_alloc(4);
        sv0_box_store(_sv0t21, 0, _sv0t20.tag);
        sv0_box_store(_sv0t21, 1, _sv0t20.p0);
        sv0_box_store(_sv0t21, 2, _sv0t20.p1);
        sv0_box_store(_sv0t21, 3, _sv0t20.p2);
        ba.tag = 3;
        ba.p0 = pat_d1;
        ba.p1 = _sv0t21;
        int _sv0t22 = sv0_box_alloc(5);
        sv0_box_store(_sv0t22, 0, ba.tag);
        sv0_box_store(_sv0t22, 1, ba.p0);
        sv0_box_store(_sv0t22, 2, ba.p1);
        sv0_box_store(_sv0t22, 3, ba.p2);
        sv0_box_store(_sv0t22, 4, ba.p3);
        sv0_vec_push(bind_instrs, _sv0t22);
      } else {
        Instr bd;
        bd.tag = 2;
        bd.p0 = scrut_cty;
        bd.p1 = pat_d1;
        Instr bs;
        Expr _sv0t23;
        _sv0t23.tag = 1;
        _sv0t23.p0 = scrut_var;
        int _sv0t24 = sv0_box_alloc(4);
        sv0_box_store(_sv0t24, 0, _sv0t23.tag);
        sv0_box_store(_sv0t24, 1, _sv0t23.p0);
        sv0_box_store(_sv0t24, 2, _sv0t23.p1);
        sv0_box_store(_sv0t24, 3, _sv0t23.p2);
        bs.tag = 4;
        bs.p0 = pat_d1;
        bs.p1 = _sv0t24;
        int _sv0t25 = sv0_box_alloc(5);
        sv0_box_store(_sv0t25, 0, bd.tag);
        sv0_box_store(_sv0t25, 1, bd.p0);
        sv0_box_store(_sv0t25, 2, bd.p1);
        sv0_box_store(_sv0t25, 3, bd.p2);
        sv0_box_store(_sv0t25, 4, bd.p3);
        sv0_vec_push(bind_instrs, _sv0t25);
        int _sv0t26 = sv0_box_alloc(5);
        sv0_box_store(_sv0t26, 0, bs.tag);
        sv0_box_store(_sv0t26, 1, bs.p0);
        sv0_box_store(_sv0t26, 2, bs.p1);
        sv0_box_store(_sv0t26, 3, bs.p2);
        sv0_box_store(_sv0t26, 4, bs.p3);
        sv0_vec_push(bind_instrs, _sv0t26);
      }
    } else {
    }
    if ((pat_tag == 2)) {
      int _sv0t27 = sv0_vec_get(ed2, arm_idx);
      int paux2 = _sv0t27;
      int _sv0t28 = sv0_vec_get(et, paux2);
      int pxt2 = _sv0t28;
      if ((pxt2 == 30)) {
        int _sv0t29 = sv0_vec_get(ed4, paux2);
        int k2 = _sv0t29;
        if ((k2 == 2)) {
          int _sv0t30 = sv0_vec_get(ed3, paux2);
          int is_neg2 = _sv0t30;
          int _sv0t31 = sv0_vec_get(ed1, paux2);
          int lt2 = _sv0t31;
          int _sv0t32 = sv0_vec_get(ed2, paux2);
          int tp2 = _sv0t32;
          if ((lt2 == 0)) {
            Value lv2;
            lv2.tag = 3;
            lv2.p0 = scrut_var;
            int _sv0t33 = sv0_string_len(source);
            if ((_sv0t33 == 0)) {
              if ((is_neg2 == 1)) {
                Expr _sv0t34;
                Value _sv0t35;
                _sv0t35.tag = 1;
                int _sv0t36 = sv0_box_alloc(3);
                sv0_box_store(_sv0t36, 0, _sv0t35.tag);
                sv0_box_store(_sv0t36, 1, _sv0t35.p0);
                sv0_box_store(_sv0t36, 2, _sv0t35.p1);
                _sv0t34.tag = 0;
                _sv0t34.p0 = _sv0t36;
                cexp = _sv0t34;
              } else {
                Value _sv0t37 = lower_lit_to_ir_value(0, tp2, tok_tags, source, starts, ends);
                Value rv0;
                rv0 = _sv0t37;
                Expr _sv0t38;
                int _sv0t39 = sv0_box_alloc(3);
                sv0_box_store(_sv0t39, 0, lv2.tag);
                sv0_box_store(_sv0t39, 1, lv2.p0);
                sv0_box_store(_sv0t39, 2, lv2.p1);
                int _sv0t40 = sv0_box_alloc(3);
                sv0_box_store(_sv0t40, 0, rv0.tag);
                sv0_box_store(_sv0t40, 1, rv0.p0);
                sv0_box_store(_sv0t40, 2, rv0.p1);
                _sv0t38.tag = 2;
                _sv0t38.p0 = 5;
                _sv0t38.p1 = _sv0t39;
                _sv0t38.p2 = _sv0t40;
                cexp = _sv0t38;
              }
            } else {
              int _sv0t41 = parse_decimal_i32_from_tok(source, starts, ends, tp2);
              int vi = _sv0t41;
              if ((is_neg2 == 1)) {
                vi = (0 - vi);
              } else {
              }
              Value rv2;
              rv2.tag = 0;
              rv2.p0 = vi;
              Expr _sv0t42;
              int _sv0t43 = sv0_box_alloc(3);
              sv0_box_store(_sv0t43, 0, lv2.tag);
              sv0_box_store(_sv0t43, 1, lv2.p0);
              sv0_box_store(_sv0t43, 2, lv2.p1);
              int _sv0t44 = sv0_box_alloc(3);
              sv0_box_store(_sv0t44, 0, rv2.tag);
              sv0_box_store(_sv0t44, 1, rv2.p0);
              sv0_box_store(_sv0t44, 2, rv2.p1);
              _sv0t42.tag = 2;
              _sv0t42.p0 = 5;
              _sv0t42.p1 = _sv0t43;
              _sv0t42.p2 = _sv0t44;
              cexp = _sv0t42;
            }
          } else {
            if ((lt2 == 5)) {
              int _sv0t45 = sv0_vec_get(tok_tags, tp2);
              int tt_b = _sv0t45;
              int nz = 0;
              if ((tt_b == 91)) {
                nz = 1;
              } else {
              }
              if ((is_neg2 == 1)) {
                if ((nz == 1)) {
                  nz = 0;
                } else {
                  nz = 1;
                }
              } else {
              }
              Value lv_b;
              lv_b.tag = 3;
              lv_b.p0 = scrut_var;
              Value zv;
              zv.tag = 0;
              zv.p0 = 0;
              if ((nz == 1)) {
                Expr _sv0t46;
                int _sv0t47 = sv0_box_alloc(3);
                sv0_box_store(_sv0t47, 0, lv_b.tag);
                sv0_box_store(_sv0t47, 1, lv_b.p0);
                sv0_box_store(_sv0t47, 2, lv_b.p1);
                int _sv0t48 = sv0_box_alloc(3);
                sv0_box_store(_sv0t48, 0, zv.tag);
                sv0_box_store(_sv0t48, 1, zv.p0);
                sv0_box_store(_sv0t48, 2, zv.p1);
                _sv0t46.tag = 2;
                _sv0t46.p0 = 6;
                _sv0t46.p1 = _sv0t47;
                _sv0t46.p2 = _sv0t48;
                cexp = _sv0t46;
              } else {
                Expr _sv0t49;
                int _sv0t50 = sv0_box_alloc(3);
                sv0_box_store(_sv0t50, 0, lv_b.tag);
                sv0_box_store(_sv0t50, 1, lv_b.p0);
                sv0_box_store(_sv0t50, 2, lv_b.p1);
                int _sv0t51 = sv0_box_alloc(3);
                sv0_box_store(_sv0t51, 0, zv.tag);
                sv0_box_store(_sv0t51, 1, zv.p0);
                sv0_box_store(_sv0t51, 2, zv.p1);
                _sv0t49.tag = 2;
                _sv0t49.p0 = 5;
                _sv0t49.p1 = _sv0t50;
                _sv0t49.p2 = _sv0t51;
                cexp = _sv0t49;
              }
            } else {
              if ((is_neg2 == 1)) {
                Value _sv0t52 = lower_lit_to_ir_value(lt2, tp2, tok_tags, source, starts, ends);
                Value rv_n;
                rv_n = _sv0t52;
                Value lv_n;
                lv_n.tag = 3;
                lv_n.p0 = scrut_var;
                Expr _sv0t53;
                int _sv0t54 = sv0_box_alloc(3);
                sv0_box_store(_sv0t54, 0, lv_n.tag);
                sv0_box_store(_sv0t54, 1, lv_n.p0);
                sv0_box_store(_sv0t54, 2, lv_n.p1);
                int _sv0t55 = sv0_box_alloc(3);
                sv0_box_store(_sv0t55, 0, rv_n.tag);
                sv0_box_store(_sv0t55, 1, rv_n.p0);
                sv0_box_store(_sv0t55, 2, rv_n.p1);
                _sv0t53.tag = 2;
                _sv0t53.p0 = 6;
                _sv0t53.p1 = _sv0t54;
                _sv0t53.p2 = _sv0t55;
                cexp = _sv0t53;
              } else {
                Value _sv0t56 = lower_lit_to_ir_value(lt2, tp2, tok_tags, source, starts, ends);
                Value rv3;
                rv3 = _sv0t56;
                Value lv3;
                lv3.tag = 3;
                lv3.p0 = scrut_var;
                Expr _sv0t57;
                int _sv0t58 = sv0_box_alloc(3);
                sv0_box_store(_sv0t58, 0, lv3.tag);
                sv0_box_store(_sv0t58, 1, lv3.p0);
                sv0_box_store(_sv0t58, 2, lv3.p1);
                int _sv0t59 = sv0_box_alloc(3);
                sv0_box_store(_sv0t59, 0, rv3.tag);
                sv0_box_store(_sv0t59, 1, rv3.p0);
                sv0_box_store(_sv0t59, 2, rv3.p1);
                _sv0t57.tag = 2;
                _sv0t57.p0 = 5;
                _sv0t57.p1 = _sv0t58;
                _sv0t57.p2 = _sv0t59;
                cexp = _sv0t57;
              }
            }
          }
        } else {
          Expr _sv0t60;
          Value _sv0t61;
          _sv0t61.tag = 2;
          int _sv0t62 = sv0_box_alloc(3);
          sv0_box_store(_sv0t62, 0, _sv0t61.tag);
          sv0_box_store(_sv0t62, 1, _sv0t61.p0);
          sv0_box_store(_sv0t62, 2, _sv0t61.p1);
          _sv0t60.tag = 0;
          _sv0t60.p0 = _sv0t62;
          cexp = _sv0t60;
        }
      } else {
        Expr _sv0t63;
        Value _sv0t64;
        _sv0t64.tag = 2;
        int _sv0t65 = sv0_box_alloc(3);
        sv0_box_store(_sv0t65, 0, _sv0t64.tag);
        sv0_box_store(_sv0t65, 1, _sv0t64.p0);
        sv0_box_store(_sv0t65, 2, _sv0t64.p1);
        _sv0t63.tag = 0;
        _sv0t63.p0 = _sv0t65;
        cexp = _sv0t63;
      }
    } else {
    }
    if ((pat_tag == 5)) {
      int _sv0t66 = sv0_vec_get(ed2, arm_idx);
      int paux5 = _sv0t66;
      int _sv0t67 = sv0_vec_get(et, paux5);
      int pxt5 = _sv0t67;
      if ((pxt5 == 30)) {
        int _sv0t68 = sv0_vec_get(ed4, paux5);
        int k5 = _sv0t68;
        if ((k5 == 5)) {
          int _sv0t69 = sv0_vec_get(ed1, paux5);
          int pp_s5 = _sv0t69;
          int _sv0t70 = sv0_vec_get(ed2, paux5);
          int pp_c5 = _sv0t70;
          if ((pp_c5 >= 2)) {
            int _sv0t71 = sv0_vec_get(pp, pp_s5);
            int en_h = _sv0t71;
            int _sv0t72 = (pp_s5 + 1);
            int _sv0t73 = sv0_vec_get(pp, _sv0t72);
            int vn_h = _sv0t73;
            int _sv0t74 = enum_tag_lookup_str(enum_names, enum_tags_flat, enum_tag_offsets, enum_tag_counts, en_h, vn_h, source, starts, ends);
            int kv = _sv0t74;
            if ((kv < 0)) {
              Expr _sv0t75;
              Value _sv0t76;
              _sv0t76.tag = 2;
              int _sv0t77 = sv0_box_alloc(3);
              sv0_box_store(_sv0t77, 0, _sv0t76.tag);
              sv0_box_store(_sv0t77, 1, _sv0t76.p0);
              sv0_box_store(_sv0t77, 2, _sv0t76.p1);
              _sv0t75.tag = 0;
              _sv0t75.p0 = _sv0t77;
              cexp = _sv0t75;
            } else {
              Value tag_m;
              Value _sv0t78;
              _sv0t78.tag = 3;
              _sv0t78.p0 = scrut_var;
              int _sv0t79 = sv0_box_alloc(3);
              sv0_box_store(_sv0t79, 0, _sv0t78.tag);
              sv0_box_store(_sv0t79, 1, _sv0t78.p0);
              sv0_box_store(_sv0t79, 2, _sv0t78.p1);
              int _sv0t80 = (0 - 1);
              tag_m.tag = 6;
              tag_m.p0 = _sv0t79;
              tag_m.p1 = _sv0t80;
              Value kv_v;
              kv_v.tag = 0;
              kv_v.p0 = kv;
              Expr _sv0t81;
              int _sv0t82 = sv0_box_alloc(3);
              sv0_box_store(_sv0t82, 0, tag_m.tag);
              sv0_box_store(_sv0t82, 1, tag_m.p0);
              sv0_box_store(_sv0t82, 2, tag_m.p1);
              int _sv0t83 = sv0_box_alloc(3);
              sv0_box_store(_sv0t83, 0, kv_v.tag);
              sv0_box_store(_sv0t83, 1, kv_v.p0);
              sv0_box_store(_sv0t83, 2, kv_v.p1);
              _sv0t81.tag = 2;
              _sv0t81.p0 = 5;
              _sv0t81.p1 = _sv0t82;
              _sv0t81.p2 = _sv0t83;
              cexp = _sv0t81;
              int _sv0t84 = sv0_vec_get(ed3, paux5);
              int argc5 = _sv0t84;
              if ((argc5 > 0)) {
                int meta_idx5 = (paux5 - 1);
                int _sv0t85 = sv0_vec_get(et, meta_idx5);
                int mtag5 = _sv0t85;
                if ((mtag5 == 33)) {
                  int _sv0t86 = sv0_vec_get(ed1, meta_idx5);
                  int ac5 = _sv0t86;
                  int _sv0t87 = sv0_vec_get(ed2, meta_idx5);
                  int first_e5 = _sv0t87;
                  int ej = 0;
                  while ((ej < ac5)) {
                    int eidx5 = (first_e5 + ej);
                    int _sv0t88 = sv0_vec_get(et, eidx5);
                    int etg5 = _sv0t88;
                    if ((etg5 == 31)) {
                      int _sv0t89 = sv0_vec_get(ed4, eidx5);
                      int ipt5 = _sv0t89;
                      if ((ipt5 == 1)) {
                        int _sv0t90 = sv0_vec_get(ed1, eidx5);
                        int bind_tok5 = _sv0t90;
                        int _sv0t91 = sv0_vec_get(ed2, eidx5);
                        int pi5 = _sv0t91;
                        if ((pi5 >= 0)) {
                          int _sv0t92 = (0 - 10);
                          int pay5 = (_sv0t92 - pi5);
                          Value rhsv5;
                          Value _sv0t93;
                          _sv0t93.tag = 3;
                          _sv0t93.p0 = scrut_var;
                          int _sv0t94 = sv0_box_alloc(3);
                          sv0_box_store(_sv0t94, 0, _sv0t93.tag);
                          sv0_box_store(_sv0t94, 1, _sv0t93.p0);
                          sv0_box_store(_sv0t94, 2, _sv0t93.p1);
                          rhsv5.tag = 6;
                          rhsv5.p0 = _sv0t94;
                          rhsv5.p1 = pay5;
                          Instr asgx5;
                          Expr _sv0t95;
                          int _sv0t96 = sv0_box_alloc(3);
                          sv0_box_store(_sv0t96, 0, rhsv5.tag);
                          sv0_box_store(_sv0t96, 1, rhsv5.p0);
                          sv0_box_store(_sv0t96, 2, rhsv5.p1);
                          _sv0t95.tag = 0;
                          _sv0t95.p0 = _sv0t96;
                          int _sv0t97 = sv0_box_alloc(4);
                          sv0_box_store(_sv0t97, 0, _sv0t95.tag);
                          sv0_box_store(_sv0t97, 1, _sv0t95.p0);
                          sv0_box_store(_sv0t97, 2, _sv0t95.p1);
                          sv0_box_store(_sv0t97, 3, _sv0t95.p2);
                          asgx5.tag = 3;
                          asgx5.p0 = bind_tok5;
                          asgx5.p1 = _sv0t97;
                          int _sv0t98 = sv0_box_alloc(5);
                          sv0_box_store(_sv0t98, 0, asgx5.tag);
                          sv0_box_store(_sv0t98, 1, asgx5.p0);
                          sv0_box_store(_sv0t98, 2, asgx5.p1);
                          sv0_box_store(_sv0t98, 3, asgx5.p2);
                          sv0_box_store(_sv0t98, 4, asgx5.p3);
                          sv0_vec_push(bind_instrs, _sv0t98);
                        } else {
                        }
                      } else {
                      }
                    } else {
                    }
                    ej = (ej + 1);
                  }
                } else {
                }
              } else {
              }
            }
          } else {
            Expr _sv0t99;
            Value _sv0t100;
            _sv0t100.tag = 1;
            int _sv0t101 = sv0_box_alloc(3);
            sv0_box_store(_sv0t101, 0, _sv0t100.tag);
            sv0_box_store(_sv0t101, 1, _sv0t100.p0);
            sv0_box_store(_sv0t101, 2, _sv0t100.p1);
            _sv0t99.tag = 0;
            _sv0t99.p0 = _sv0t101;
            cexp = _sv0t99;
          }
        } else {
          Expr _sv0t102;
          Value _sv0t103;
          _sv0t103.tag = 2;
          int _sv0t104 = sv0_box_alloc(3);
          sv0_box_store(_sv0t104, 0, _sv0t103.tag);
          sv0_box_store(_sv0t104, 1, _sv0t103.p0);
          sv0_box_store(_sv0t104, 2, _sv0t103.p1);
          _sv0t102.tag = 0;
          _sv0t102.p0 = _sv0t104;
          cexp = _sv0t102;
        }
      } else {
        Expr _sv0t105;
        Value _sv0t106;
        _sv0t106.tag = 2;
        int _sv0t107 = sv0_box_alloc(3);
        sv0_box_store(_sv0t107, 0, _sv0t106.tag);
        sv0_box_store(_sv0t107, 1, _sv0t106.p0);
        sv0_box_store(_sv0t107, 2, _sv0t106.p1);
        _sv0t105.tag = 0;
        _sv0t105.p0 = _sv0t107;
        cexp = _sv0t105;
      }
    } else {
    }
    if ((pat_tag == 4)) {
      int _sv0t108 = sv0_vec_get(ed2, arm_idx);
      int paux4 = _sv0t108;
      int _sv0t109 = sv0_vec_get(et, paux4);
      int pxt4 = _sv0t109;
      if ((pxt4 == 30)) {
        int _sv0t110 = sv0_vec_get(ed4, paux4);
        int k4 = _sv0t110;
        if ((k4 == 4)) {
          int _sv0t111 = sv0_vec_get(ed1, paux4);
          int pp_s4 = _sv0t111;
          int _sv0t112 = sv0_vec_get(ed2, paux4);
          int pp_c4 = _sv0t112;
          if ((pp_c4 >= 2)) {
            int _sv0t113 = sv0_vec_get(pp, pp_s4);
            int en_h4 = _sv0t113;
            int _sv0t114 = (pp_s4 + 1);
            int _sv0t115 = sv0_vec_get(pp, _sv0t114);
            int vn_h4 = _sv0t115;
            int _sv0t116 = enum_tag_lookup_str(enum_names, enum_tags_flat, enum_tag_offsets, enum_tag_counts, en_h4, vn_h4, source, starts, ends);
            int kv4 = _sv0t116;
            if ((kv4 < 0)) {
              Expr _sv0t117;
              Value _sv0t118;
              _sv0t118.tag = 2;
              int _sv0t119 = sv0_box_alloc(3);
              sv0_box_store(_sv0t119, 0, _sv0t118.tag);
              sv0_box_store(_sv0t119, 1, _sv0t118.p0);
              sv0_box_store(_sv0t119, 2, _sv0t118.p1);
              _sv0t117.tag = 0;
              _sv0t117.p0 = _sv0t119;
              cexp = _sv0t117;
            } else {
              Value tag_m4;
              Value _sv0t120;
              _sv0t120.tag = 3;
              _sv0t120.p0 = scrut_var;
              int _sv0t121 = sv0_box_alloc(3);
              sv0_box_store(_sv0t121, 0, _sv0t120.tag);
              sv0_box_store(_sv0t121, 1, _sv0t120.p0);
              sv0_box_store(_sv0t121, 2, _sv0t120.p1);
              int _sv0t122 = (0 - 1);
              tag_m4.tag = 6;
              tag_m4.p0 = _sv0t121;
              tag_m4.p1 = _sv0t122;
              Value kv_v4;
              kv_v4.tag = 0;
              kv_v4.p0 = kv4;
              Expr _sv0t123;
              int _sv0t124 = sv0_box_alloc(3);
              sv0_box_store(_sv0t124, 0, tag_m4.tag);
              sv0_box_store(_sv0t124, 1, tag_m4.p0);
              sv0_box_store(_sv0t124, 2, tag_m4.p1);
              int _sv0t125 = sv0_box_alloc(3);
              sv0_box_store(_sv0t125, 0, kv_v4.tag);
              sv0_box_store(_sv0t125, 1, kv_v4.p0);
              sv0_box_store(_sv0t125, 2, kv_v4.p1);
              _sv0t123.tag = 2;
              _sv0t123.p0 = 5;
              _sv0t123.p1 = _sv0t124;
              _sv0t123.p2 = _sv0t125;
              cexp = _sv0t123;
              int meta_idx = (paux4 - 1);
              int _sv0t126 = sv0_vec_get(et, meta_idx);
              int mtag = _sv0t126;
              if ((mtag == 32)) {
                int _sv0t127 = sv0_vec_get(ed1, meta_idx);
                int sfc = _sv0t127;
                int _sv0t128 = sv0_vec_get(ed2, meta_idx);
                int first_emb = _sv0t128;
                if ((sfc > 0)) {
                  int fj = 0;
                  while ((fj < sfc)) {
                    int eidx = (first_emb + fj);
                    int _sv0t129 = sv0_vec_get(et, eidx);
                    int etg = _sv0t129;
                    if ((etg == 31)) {
                      int _sv0t130 = sv0_vec_get(ed4, eidx);
                      int ipt = _sv0t130;
                      if ((ipt == 1)) {
                        int _sv0t131 = sv0_vec_get(ed1, eidx);
                        int bind_tok = _sv0t131;
                        int _sv0t132 = sv0_vec_get(ed3, eidx);
                        int fld_meta = _sv0t132;
                        int fnam_tok = bind_tok;
                        if ((fld_meta != 0)) {
                          fnam_tok = fld_meta;
                        } else {
                        }
                        int _sv0t133 = lower_pat_struct_field_payload_index(tok_tags, source, starts, ends, vn_h4, sfc, fnam_tok);
                        int pi = _sv0t133;
                        int pif = pi;
                        if ((pif < 0)) {
                          pif = fj;
                        } else {
                        }
                        if ((pif >= 0)) {
                          int _sv0t134 = (0 - 10);
                          int pay = (_sv0t134 - pif);
                          Value rhsv;
                          Value _sv0t135;
                          _sv0t135.tag = 3;
                          _sv0t135.p0 = scrut_var;
                          int _sv0t136 = sv0_box_alloc(3);
                          sv0_box_store(_sv0t136, 0, _sv0t135.tag);
                          sv0_box_store(_sv0t136, 1, _sv0t135.p0);
                          sv0_box_store(_sv0t136, 2, _sv0t135.p1);
                          rhsv.tag = 6;
                          rhsv.p0 = _sv0t136;
                          rhsv.p1 = pay;
                          Instr asgx;
                          Expr _sv0t137;
                          int _sv0t138 = sv0_box_alloc(3);
                          sv0_box_store(_sv0t138, 0, rhsv.tag);
                          sv0_box_store(_sv0t138, 1, rhsv.p0);
                          sv0_box_store(_sv0t138, 2, rhsv.p1);
                          _sv0t137.tag = 0;
                          _sv0t137.p0 = _sv0t138;
                          int _sv0t139 = sv0_box_alloc(4);
                          sv0_box_store(_sv0t139, 0, _sv0t137.tag);
                          sv0_box_store(_sv0t139, 1, _sv0t137.p0);
                          sv0_box_store(_sv0t139, 2, _sv0t137.p1);
                          sv0_box_store(_sv0t139, 3, _sv0t137.p2);
                          asgx.tag = 3;
                          asgx.p0 = bind_tok;
                          asgx.p1 = _sv0t139;
                          int _sv0t140 = sv0_box_alloc(5);
                          sv0_box_store(_sv0t140, 0, asgx.tag);
                          sv0_box_store(_sv0t140, 1, asgx.p0);
                          sv0_box_store(_sv0t140, 2, asgx.p1);
                          sv0_box_store(_sv0t140, 3, asgx.p2);
                          sv0_box_store(_sv0t140, 4, asgx.p3);
                          sv0_vec_push(bind_instrs, _sv0t140);
                        } else {
                        }
                      } else {
                      }
                    } else {
                    }
                    fj = (fj + 1);
                  }
                } else {
                }
              } else {
              }
            }
          } else {
            Expr _sv0t141;
            Value _sv0t142;
            _sv0t142.tag = 1;
            int _sv0t143 = sv0_box_alloc(3);
            sv0_box_store(_sv0t143, 0, _sv0t142.tag);
            sv0_box_store(_sv0t143, 1, _sv0t142.p0);
            sv0_box_store(_sv0t143, 2, _sv0t142.p1);
            _sv0t141.tag = 0;
            _sv0t141.p0 = _sv0t143;
            cexp = _sv0t141;
          }
        } else {
          Expr _sv0t144;
          Value _sv0t145;
          _sv0t145.tag = 2;
          int _sv0t146 = sv0_box_alloc(3);
          sv0_box_store(_sv0t146, 0, _sv0t145.tag);
          sv0_box_store(_sv0t146, 1, _sv0t145.p0);
          sv0_box_store(_sv0t146, 2, _sv0t145.p1);
          _sv0t144.tag = 0;
          _sv0t144.p0 = _sv0t146;
          cexp = _sv0t144;
        }
      } else {
        Expr _sv0t147;
        Value _sv0t148;
        _sv0t148.tag = 2;
        int _sv0t149 = sv0_box_alloc(3);
        sv0_box_store(_sv0t149, 0, _sv0t148.tag);
        sv0_box_store(_sv0t149, 1, _sv0t148.p0);
        sv0_box_store(_sv0t149, 2, _sv0t148.p1);
        _sv0t147.tag = 0;
        _sv0t147.p0 = _sv0t149;
        cexp = _sv0t147;
      }
    } else {
    }
    int _sv0t150 = lower_match_env_clone(match_env);
    int arm_menv = _sv0t150;
    if ((pat_tag == 1)) {
      int _sv0t151 = lower_match_env_append_scrut_pair(arm_menv, pat_d1, scrut_cty);
      int dap = _sv0t151;
    } else {
    }
    if ((pat_tag == 5)) {
      int _sv0t152 = sv0_vec_get(ed2, arm_idx);
      int paux5s = _sv0t152;
      int _sv0t153 = sv0_vec_get(et, paux5s);
      int pxt5s = _sv0t153;
      if ((pxt5s == 30)) {
        int _sv0t154 = sv0_vec_get(ed4, paux5s);
        int k5s = _sv0t154;
        if ((k5s == 5)) {
          int _sv0t155 = sv0_vec_get(ed2, paux5s);
          int pp_c5s = _sv0t155;
          if ((pp_c5s >= 2)) {
            int _sv0t156 = sv0_vec_get(ed1, paux5s);
            int pp_s5s = _sv0t156;
            int _sv0t157 = sv0_vec_get(pp, pp_s5s);
            int en_hs = _sv0t157;
            int _sv0t158 = (pp_s5s + 1);
            int _sv0t159 = sv0_vec_get(pp, _sv0t158);
            int vn_hs = _sv0t159;
            int _sv0t160 = enum_tag_lookup_str(enum_names, enum_tags_flat, enum_tag_offsets, enum_tag_counts, en_hs, vn_hs, source, starts, ends);
            int kvs = _sv0t160;
            if ((kvs >= 0)) {
              int _sv0t161 = sv0_vec_get(ed3, paux5s);
              int argc5s = _sv0t161;
              if ((argc5s > 0)) {
                int meta_idx5s = (paux5s - 1);
                int _sv0t162 = sv0_vec_get(et, meta_idx5s);
                if ((_sv0t162 == 33)) {
                  int _sv0t163 = sv0_vec_get(ed1, meta_idx5s);
                  int ac5s = _sv0t163;
                  int _sv0t164 = sv0_vec_get(ed2, meta_idx5s);
                  int first_e5s = _sv0t164;
                  int ejs = 0;
                  while ((ejs < ac5s)) {
                    int eidx5s = (first_e5s + ejs);
                    int _sv0t165 = sv0_vec_get(et, eidx5s);
                    int etg5s = _sv0t165;
                    if ((etg5s == 31)) {
                      int _sv0t166 = sv0_vec_get(ed4, eidx5s);
                      int ipt5s = _sv0t166;
                      if ((ipt5s == 1)) {
                        int _sv0t167 = sv0_vec_get(ed1, eidx5s);
                        int bind_tok5s = _sv0t167;
                        int _sv0t168 = sv0_vec_get(ed2, eidx5s);
                        int pi5s = _sv0t168;
                        if ((bind_tok5s >= 0)) {
                          if ((pi5s >= 0)) {
                            int _sv0t169 = lower_match_env_append_scrut_pair(arm_menv, bind_tok5s, scrut_cty);
                            int das = _sv0t169;
                          } else {
                          }
                        } else {
                        }
                      } else {
                      }
                    } else {
                    }
                    ejs = (ejs + 1);
                  }
                } else {
                }
              } else {
              }
            } else {
            }
          } else {
          }
        } else {
        }
      } else {
      }
    } else {
    }
    if ((pat_tag == 4)) {
      int _sv0t170 = sv0_vec_get(ed2, arm_idx);
      int paux4s = _sv0t170;
      int _sv0t171 = sv0_vec_get(et, paux4s);
      int pxt4s = _sv0t171;
      if ((pxt4s == 30)) {
        int _sv0t172 = sv0_vec_get(ed4, paux4s);
        int k4s = _sv0t172;
        if ((k4s == 4)) {
          int _sv0t173 = sv0_vec_get(ed2, paux4s);
          int pp_c4s = _sv0t173;
          if ((pp_c4s >= 2)) {
            int _sv0t174 = sv0_vec_get(ed1, paux4s);
            int pp_s4s = _sv0t174;
            int _sv0t175 = sv0_vec_get(pp, pp_s4s);
            int en_h4s = _sv0t175;
            int _sv0t176 = (pp_s4s + 1);
            int _sv0t177 = sv0_vec_get(pp, _sv0t176);
            int vn_h4s = _sv0t177;
            int _sv0t178 = enum_tag_lookup_str(enum_names, enum_tags_flat, enum_tag_offsets, enum_tag_counts, en_h4s, vn_h4s, source, starts, ends);
            int kv4s = _sv0t178;
            if ((kv4s >= 0)) {
              int meta_idxs = (paux4s - 1);
              int _sv0t179 = sv0_vec_get(et, meta_idxs);
              if ((_sv0t179 == 32)) {
                int _sv0t180 = sv0_vec_get(ed1, meta_idxs);
                int sfc4s = _sv0t180;
                int _sv0t181 = sv0_vec_get(ed2, meta_idxs);
                int first_embs = _sv0t181;
                if ((sfc4s > 0)) {
                  int fjs = 0;
                  while ((fjs < sfc4s)) {
                    int eidxs = (first_embs + fjs);
                    int _sv0t182 = sv0_vec_get(et, eidxs);
                    int etgs = _sv0t182;
                    if ((etgs == 31)) {
                      int _sv0t183 = sv0_vec_get(ed4, eidxs);
                      int ipts = _sv0t183;
                      if ((ipts == 1)) {
                        int _sv0t184 = sv0_vec_get(ed1, eidxs);
                        int bind_toks = _sv0t184;
                        int _sv0t185 = sv0_vec_get(ed3, eidxs);
                        int fld_metas = _sv0t185;
                        int fnam_toks = bind_toks;
                        if ((fld_metas != 0)) {
                          fnam_toks = fld_metas;
                        } else {
                        }
                        int _sv0t186 = lower_pat_struct_field_payload_index(tok_tags, source, starts, ends, vn_h4s, sfc4s, fnam_toks);
                        int pis = _sv0t186;
                        if ((bind_toks >= 0)) {
                          if ((pis >= 0)) {
                            int _sv0t187 = lower_match_env_append_scrut_pair(arm_menv, bind_toks, scrut_cty);
                            int dss = _sv0t187;
                          } else {
                          }
                        } else {
                        }
                      } else {
                      }
                    } else {
                    }
                    fjs = (fjs + 1);
                  }
                } else {
                }
              } else {
              }
            } else {
            }
          } else {
          }
        } else {
        }
      } else {
      }
    } else {
    }
    int _sv0t188 = sv0_vec_new();
    int body_is = _sv0t188;
    int _sv0t189 = sv0_vec_len(bind_instrs);
    int bli = _sv0t189;
    int bi = 0;
    while ((bi < bli)) {
      int _sv0t190 = sv0_vec_get(bind_instrs, bi);
      sv0_vec_push(body_is, _sv0t190);
      bi = (bi + 1);
    }
    int _sv0t191 = lower_into_var_instrs(et, ed1, ed2, ed3, ed4, pp, tok_tags, body_idx, out_var, ctr, body_is, enum_names, enum_tag_offsets, enum_tag_counts, enum_tags_flat, fn_ctx, builtin_map, lit_sf_names, source, starts, ends, item_tags, item_names, item_d2, item_d3, item_d4, item_field_counts, enum_vnames_flat, enum_max_payloads, item_fn_row, arm_menv, import_aliases);
    int discard_body = _sv0t191;
    int th = body_is;
    if ((guard_idx >= 0)) {
      int _sv0t192 = sv0_vec_new();
      int guard_is = _sv0t192;
      Expr _sv0t193 = lower_expr_with_instrs(et, ed1, ed2, ed3, ed4, pp, tok_tags, guard_idx, ctr, guard_is, enum_names, enum_tag_offsets, enum_tag_counts, enum_tags_flat, fn_ctx, builtin_map, lit_sf_names, source, starts, ends, item_tags, item_names, item_d2, item_d3, item_d4, item_field_counts, enum_vnames_flat, enum_max_payloads, item_fn_row, arm_menv, import_aliases);
      Expr ge;
      ge = _sv0t193;
      Instr guard_if;
      int _sv0t194 = sv0_box_alloc(4);
      sv0_box_store(_sv0t194, 0, ge.tag);
      sv0_box_store(_sv0t194, 1, ge.p0);
      sv0_box_store(_sv0t194, 2, ge.p1);
      sv0_box_store(_sv0t194, 3, ge.p2);
      guard_if.tag = 6;
      guard_if.p0 = _sv0t194;
      guard_if.p1 = body_is;
      guard_if.p2 = result_instrs;
      int _sv0t195 = sv0_box_alloc(5);
      sv0_box_store(_sv0t195, 0, guard_if.tag);
      sv0_box_store(_sv0t195, 1, guard_if.p0);
      sv0_box_store(_sv0t195, 2, guard_if.p1);
      sv0_box_store(_sv0t195, 3, guard_if.p2);
      sv0_box_store(_sv0t195, 4, guard_if.p3);
      sv0_vec_push(guard_is, _sv0t195);
      th = guard_is;
    } else {
    }
    Instr arm_if;
    int _sv0t196 = sv0_box_alloc(4);
    sv0_box_store(_sv0t196, 0, cexp.tag);
    sv0_box_store(_sv0t196, 1, cexp.p0);
    sv0_box_store(_sv0t196, 2, cexp.p1);
    sv0_box_store(_sv0t196, 3, cexp.p2);
    arm_if.tag = 6;
    arm_if.p0 = _sv0t196;
    arm_if.p1 = th;
    arm_if.p2 = result_instrs;
    int _sv0t197 = sv0_vec_new();
    int new_result = _sv0t197;
    int _sv0t198 = sv0_box_alloc(5);
    sv0_box_store(_sv0t198, 0, arm_if.tag);
    sv0_box_store(_sv0t198, 1, arm_if.p0);
    sv0_box_store(_sv0t198, 2, arm_if.p1);
    sv0_box_store(_sv0t198, 3, arm_if.p2);
    sv0_box_store(_sv0t198, 4, arm_if.p3);
    sv0_vec_push(new_result, _sv0t198);
    result_instrs = new_result;
    ai = (ai - 1);
  }
  int _sv0t199 = sv0_vec_len(result_instrs);
  int rlen = _sv0t199;
  int ri = 0;
  while ((ri < rlen)) {
    int _sv0t200 = sv0_vec_get(result_instrs, ri);
    sv0_vec_push(out_instrs, _sv0t200);
    ri = (ri + 1);
  }
  return 0;
}

static int lower_match_ps_peek(int tags, int pos) {
  int _sv0t0 = sv0_vec_len(tags);
  if ((pos >= _sv0t0)) {
    return 97;
  } else {
  }
  int _sv0t1 = sv0_vec_get(tags, pos);
  return _sv0t1;
}

static int lower_match_skip_generic_params(int tags, int pos) {
  int _sv0t0 = lower_match_ps_peek(tags, pos);
  if ((_sv0t0 != 38)) {
    return pos;
  } else {
  }
  int p = (pos + 1);
  int depth = 1;
  while ((depth > 0)) {
    int _sv0t1 = lower_match_ps_peek(tags, p);
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

static int lower_match_skip_bracket_group(int tags, int pos) {
  int _sv0t0 = lower_match_ps_peek(tags, pos);
  if ((_sv0t0 != 10)) {
    return pos;
  } else {
  }
  int p = (pos + 1);
  int depth = 1;
  while ((depth > 0)) {
    int _sv0t1 = lower_match_ps_peek(tags, p);
    int t = _sv0t1;
    if ((t == 97)) {
      int _sv0t2 = (0 - 1);
      return _sv0t2;
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
  return p;
}

static int lower_skip_paren_group_with_generics(int tags, int open_pos) {
  int _sv0t0 = lower_match_ps_peek(tags, open_pos);
  if ((_sv0t0 != 6)) {
    int _sv0t1 = (0 - 1);
    return _sv0t1;
  } else {
  }
  int p = (open_pos + 1);
  int depth = 1;
  while ((depth > 0)) {
    int _sv0t2 = sv0_vec_len(tags);
    if ((p >= _sv0t2)) {
      int _sv0t3 = (0 - 1);
      return _sv0t3;
    } else {
    }
    int _sv0t4 = lower_match_ps_peek(tags, p);
    int t = _sv0t4;
    if ((t == 97)) {
      int _sv0t5 = (0 - 1);
      return _sv0t5;
    } else {
    }
    if ((t == 38)) {
      int _sv0t6 = lower_match_skip_generic_params(tags, p);
      p = _sv0t6;
    } else {
      if ((t == 10)) {
        int _sv0t7 = lower_match_skip_bracket_group(tags, p);
        p = _sv0t7;
      } else {
        if ((t == 6)) {
          depth = (depth + 1);
        } else {
        }
        if ((t == 7)) {
          depth = (depth - 1);
        } else {
        }
        p = (p + 1);
      }
    }
  }
  return p;
}

static int lower_scan_fn_ret_ty_tok(int tok_tags, const char* source, int starts, int ends, int name_tok) {
  int _sv0t0 = sv0_vec_len(starts);
  int _sv0t1 = sv0_vec_len(ends);
  int _sv0t2 = (_sv0t0 + _sv0t1);
  int _sv0t3 = sv0_string_len(source);
  int bound_ok = (_sv0t2 + _sv0t3);
  if ((bound_ok < 0)) {
    int _sv0t4 = (0 - 1);
    return _sv0t4;
  } else {
  }
  int p = (name_tok + 1);
  int _sv0t5 = lower_match_skip_generic_params(tok_tags, p);
  p = _sv0t5;
  int _sv0t6 = lower_match_ps_peek(tok_tags, p);
  if ((_sv0t6 != 6)) {
    int _sv0t7 = (0 - 1);
    return _sv0t7;
  } else {
  }
  int _sv0t8 = lower_skip_paren_group_with_generics(tok_tags, p);
  int after_params = _sv0t8;
  if ((after_params < 0)) {
    int _sv0t9 = (0 - 1);
    return _sv0t9;
  } else {
  }
  int _sv0t10 = lower_match_ps_peek(tok_tags, after_params);
  if ((_sv0t10 != 19)) {
    int _sv0t11 = (0 - 1);
    return _sv0t11;
  } else {
  }
  int p3 = (after_params + 1);
  int _sv0t12 = lower_match_ps_peek(tok_tags, p3);
  int t0 = _sv0t12;
  if ((t0 != 5)) {
    if ((t0 != 6)) {
      if ((t0 != 27)) {
        if ((t0 != 10)) {
          int _sv0t13 = (0 - 1);
          return _sv0t13;
        } else {
        }
      } else {
      }
    } else {
    }
  } else {
  }
  return p3;
}

static int lower_scan_item_fn_param_names_ty_toks(int tok_tags, int name_tok_pos, int param_count, int out_param_name_tok, int out_param_ty_head_tok) {
  if ((param_count == 0)) {
    return 0;
  } else {
  }
  int p = (name_tok_pos + 1);
  int _sv0t0 = sv0_vec_get(tok_tags, p);
  if ((_sv0t0 == 38)) {
    int depth = 1;
    p = (p + 1);
    while ((depth > 0)) {
      int _sv0t1 = sv0_vec_get(tok_tags, p);
      int u = _sv0t1;
      if ((u == 38)) {
        depth = (depth + 1);
      } else {
      }
      if ((u == 39)) {
        depth = (depth - 1);
      } else {
      }
      p = (p + 1);
    }
  } else {
  }
  int _sv0t2 = sv0_vec_get(tok_tags, p);
  if ((_sv0t2 != 6)) {
    int _sv0t3 = (0 - 1);
    return _sv0t3;
  } else {
  }
  p = (p + 1);
  int found = 0;
  while ((found < param_count)) {
    int _sv0t4 = sv0_vec_get(tok_tags, p);
    if ((_sv0t4 == 77)) {
      p = (p + 1);
    } else {
    }
    int _sv0t5 = sv0_vec_get(tok_tags, p);
    int pt = _sv0t5;
    if ((pt != 5)) {
      if ((pt != 86)) {
        int _sv0t6 = (0 - 1);
        return _sv0t6;
      } else {
      }
    } else {
    }
    sv0_vec_push(out_param_name_tok, p);
    p = (p + 1);
    int _sv0t7 = sv0_vec_get(tok_tags, p);
    if ((_sv0t7 != 14)) {
      int _sv0t8 = (0 - 1);
      return _sv0t8;
    } else {
    }
    p = (p + 1);
    int ty_head = p;
    sv0_vec_push(out_param_ty_head_tok, ty_head);
    int tdepth = 0;
    int done = 0;
    while ((done != 1)) {
      int _sv0t9 = sv0_vec_get(tok_tags, p);
      int tt = _sv0t9;
      if ((tt == 38)) {
        tdepth = (tdepth + 1);
        p = (p + 1);
      } else {
        if ((tt == 39)) {
          tdepth = (tdepth - 1);
          p = (p + 1);
        } else {
          if ((tt == 6)) {
            tdepth = (tdepth + 1);
            p = (p + 1);
          } else {
            if ((tt == 7)) {
              if ((tdepth > 0)) {
                tdepth = (tdepth - 1);
                p = (p + 1);
              } else {
                done = 1;
              }
            } else {
              if ((tt == 12)) {
                if ((tdepth == 0)) {
                  done = 1;
                } else {
                  p = (p + 1);
                }
              } else {
                p = (p + 1);
              }
            }
          }
        }
      }
    }
    found = (found + 1);
    int _sv0t10 = sv0_vec_get(tok_tags, p);
    if ((_sv0t10 == 12)) {
      p = (p + 1);
    } else {
    }
  }
  return 0;
}

static int lower_match_env_hdr_len(void) {
  return 11;
}

static int lower_match_env_magic_legacy(void) {
  return 0;
}

static int lower_match_env_magic_v0(void) {
  return 1297235969;
}

static int lower_match_env_version_v0(void) {
  return 1;
}

static int lower_match_env_is_legacy_path(int env) {
  int _sv0t0 = sv0_vec_len(env);
  int n = _sv0t0;
  int _sv0t1 = lower_match_env_hdr_len();
  int h = _sv0t1;
  if ((n < h)) {
    return 1;
  } else {
  }
  int _sv0t2 = sv0_vec_get(env, 0);
  int _sv0t3 = lower_match_env_magic_legacy();
  if ((_sv0t2 == _sv0t3)) {
    return 1;
  } else {
  }
  return 0;
}

static int lower_match_env_is_packed_v0(int env) {
  int _sv0t0 = sv0_vec_len(env);
  int n = _sv0t0;
  int _sv0t1 = lower_match_env_hdr_len();
  int h = _sv0t1;
  if ((n < h)) {
    return 0;
  } else {
  }
  int _sv0t2 = sv0_vec_get(env, 0);
  int _sv0t3 = lower_match_env_magic_v0();
  if ((_sv0t2 != _sv0t3)) {
    return 0;
  } else {
  }
  int _sv0t4 = sv0_vec_get(env, 1);
  int _sv0t5 = lower_match_env_version_v0();
  if ((_sv0t4 != _sv0t5)) {
    return 0;
  } else {
  }
  return 1;
}

static int lower_match_env_alloc_v0_empty_header(void) {
  int _sv0t0 = sv0_vec_new();
  int e = _sv0t0;
  int _sv0t1 = lower_match_env_magic_v0();
  sv0_vec_push(e, _sv0t1);
  int _sv0t2 = lower_match_env_version_v0();
  sv0_vec_push(e, _sv0t2);
  int _sv0t3 = (0 - 1);
  sv0_vec_push(e, _sv0t3);
  int _sv0t4 = (0 - 1);
  sv0_vec_push(e, _sv0t4);
  sv0_vec_push(e, 0);
  int _sv0t5 = (0 - 1);
  sv0_vec_push(e, _sv0t5);
  int _sv0t6 = (0 - 1);
  sv0_vec_push(e, _sv0t6);
  sv0_vec_push(e, 0);
  int _sv0t7 = (0 - 1);
  sv0_vec_push(e, _sv0t7);
  int _sv0t8 = (0 - 1);
  sv0_vec_push(e, _sv0t8);
  sv0_vec_push(e, 0);
  return e;
}

static int lower_match_env_append_top_fn_match_tables(int env, int item_tags, int item_names, int item_d2, const char* source, int starts, int ends, int tok_tags) {
  int _sv0t0 = lower_match_env_is_packed_v0(env);
  if ((_sv0t0 != 1)) {
    int _sv0t1 = (0 - 1);
    return _sv0t1;
  } else {
  }
  int _sv0t2 = sv0_vec_new();
  int names_tmp = _sv0t2;
  int _sv0t3 = sv0_vec_new();
  int ret_tmp = _sv0t3;
  int _sv0t4 = lower_top_fn_tables_for_match(item_tags, item_names, item_d2, source, starts, ends, tok_tags, names_tmp, ret_tmp);
  int discard_tbl = _sv0t4;
  int _sv0t5 = sv0_vec_len(names_tmp);
  int fc = _sv0t5;
  int _sv0t6 = sv0_vec_len(ret_tmp);
  if ((fc != _sv0t6)) {
    int _sv0t7 = (0 - 2);
    return _sv0t7;
  } else {
  }
  if ((fc == 0)) {
    int _sv0t8 = (0 - 1);
    sv0_vec_set(env, 2, _sv0t8);
    int _sv0t9 = (0 - 1);
    sv0_vec_set(env, 3, _sv0t9);
    sv0_vec_set(env, 4, 0);
    return 0;
  } else {
  }
  int _sv0t10 = sv0_vec_len(env);
  int bfn = _sv0t10;
  int i = 0;
  while ((i < fc)) {
    int _sv0t11 = sv0_vec_get(names_tmp, i);
    sv0_vec_push(env, _sv0t11);
    i = (i + 1);
  }
  int _sv0t12 = sv0_vec_len(env);
  int brt = _sv0t12;
  i = 0;
  while ((i < fc)) {
    int _sv0t13 = sv0_vec_get(ret_tmp, i);
    sv0_vec_push(env, _sv0t13);
    i = (i + 1);
  }
  sv0_vec_set(env, 2, bfn);
  sv0_vec_set(env, 3, brt);
  sv0_vec_set(env, 4, fc);
  return 0;
}

static int lower_match_env_unpack_fn_tables_for_match(int env, int fn_names_out, int fn_ret_out) {
  int _sv0t0 = lower_match_env_is_packed_v0(env);
  if ((_sv0t0 != 1)) {
    int _sv0t1 = (0 - 1);
    return _sv0t1;
  } else {
  }
  int _sv0t2 = sv0_vec_get(env, 4);
  int fc = _sv0t2;
  if ((fc == 0)) {
    return 0;
  } else {
  }
  int _sv0t3 = sv0_vec_get(env, 2);
  int bfn = _sv0t3;
  int _sv0t4 = sv0_vec_get(env, 3);
  int brt = _sv0t4;
  if ((bfn < 0)) {
    int _sv0t5 = (0 - 2);
    return _sv0t5;
  } else {
  }
  if ((brt < 0)) {
    int _sv0t6 = (0 - 3);
    return _sv0t6;
  } else {
  }
  int j = 0;
  while ((j < fc)) {
    int _sv0t7 = (bfn + j);
    int _sv0t8 = sv0_vec_get(env, _sv0t7);
    sv0_vec_push(fn_names_out, _sv0t8);
    j = (j + 1);
  }
  j = 0;
  while ((j < fc)) {
    int _sv0t9 = (brt + j);
    int _sv0t10 = sv0_vec_get(env, _sv0t9);
    sv0_vec_push(fn_ret_out, _sv0t10);
    j = (j + 1);
  }
  return 0;
}

static int lower_match_env_append_item_fn_param_rows(int env, int item_tags, int item_names, int item_d3, int tok_tags, int item_fn_row) {
  int _sv0t0 = lower_match_env_is_packed_v0(env);
  if ((_sv0t0 != 1)) {
    int _sv0t1 = (0 - 1);
    return _sv0t1;
  } else {
  }
  if ((item_fn_row < 0)) {
    int _sv0t2 = (0 - 1);
    sv0_vec_set(env, 5, _sv0t2);
    int _sv0t3 = (0 - 1);
    sv0_vec_set(env, 6, _sv0t3);
    sv0_vec_set(env, 7, 0);
    return 0;
  } else {
  }
  int _sv0t4 = sv0_vec_len(item_tags);
  if ((item_fn_row >= _sv0t4)) {
    int _sv0t5 = (0 - 2);
    return _sv0t5;
  } else {
  }
  int _sv0t6 = sv0_vec_get(item_tags, item_fn_row);
  if ((_sv0t6 != 0)) {
    int _sv0t7 = (0 - 1);
    sv0_vec_set(env, 5, _sv0t7);
    int _sv0t8 = (0 - 1);
    sv0_vec_set(env, 6, _sv0t8);
    sv0_vec_set(env, 7, 0);
    return 0;
  } else {
  }
  int _sv0t9 = sv0_vec_get(item_d3, item_fn_row);
  int pc = _sv0t9;
  if ((pc == 0)) {
    int _sv0t10 = (0 - 1);
    sv0_vec_set(env, 5, _sv0t10);
    int _sv0t11 = (0 - 1);
    sv0_vec_set(env, 6, _sv0t11);
    sv0_vec_set(env, 7, 0);
    return 0;
  } else {
  }
  int _sv0t12 = sv0_vec_get(item_names, item_fn_row);
  int name_tok = _sv0t12;
  int _sv0t13 = sv0_vec_new();
  int pnames = _sv0t13;
  int _sv0t14 = sv0_vec_new();
  int pty = _sv0t14;
  int _sv0t15 = lower_scan_item_fn_param_names_ty_toks(tok_tags, name_tok, pc, pnames, pty);
  int rs = _sv0t15;
  if ((rs < 0)) {
    int _sv0t16 = (0 - 3);
    return _sv0t16;
  } else {
  }
  int _sv0t17 = sv0_vec_len(pnames);
  if ((_sv0t17 != pc)) {
    int _sv0t18 = (0 - 4);
    return _sv0t18;
  } else {
  }
  int _sv0t19 = sv0_vec_len(pty);
  if ((_sv0t19 != pc)) {
    int _sv0t20 = (0 - 5);
    return _sv0t20;
  } else {
  }
  int _sv0t21 = sv0_vec_len(env);
  int bn = _sv0t21;
  int j = 0;
  while ((j < pc)) {
    int _sv0t22 = sv0_vec_get(pnames, j);
    sv0_vec_push(env, _sv0t22);
    j = (j + 1);
  }
  int _sv0t23 = sv0_vec_len(env);
  int bt = _sv0t23;
  j = 0;
  while ((j < pc)) {
    int _sv0t24 = sv0_vec_get(pty, j);
    sv0_vec_push(env, _sv0t24);
    j = (j + 1);
  }
  sv0_vec_set(env, 5, bn);
  sv0_vec_set(env, 6, bt);
  sv0_vec_set(env, 7, pc);
  return 0;
}

static int lower_match_env_clone(int env) {
  int _sv0t0 = sv0_vec_len(env);
  int n = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int r = _sv0t1;
  int i = 0;
  while ((i < n)) {
    int _sv0t2 = sv0_vec_get(env, i);
    sv0_vec_push(r, _sv0t2);
    i = (i + 1);
  }
  return r;
}

static int lower_match_env_merge_parent_scrut(int dst, int parent) {
  int _sv0t0 = sv0_vec_len(parent);
  if ((_sv0t0 == 0)) {
    return 0;
  } else {
  }
  int _sv0t1 = lower_match_env_is_packed_v0(parent);
  if ((_sv0t1 != 1)) {
    return 0;
  } else {
  }
  int _sv0t2 = lower_match_env_is_packed_v0(dst);
  if ((_sv0t2 != 1)) {
    int _sv0t3 = (0 - 1);
    return _sv0t3;
  } else {
  }
  int _sv0t4 = sv0_vec_get(parent, 10);
  int sc = _sv0t4;
  if ((sc <= 0)) {
    return 0;
  } else {
  }
  int _sv0t5 = sv0_vec_get(parent, 8);
  int bn = _sv0t5;
  int _sv0t6 = sv0_vec_get(parent, 9);
  int bt = _sv0t6;
  if ((bn < 0)) {
    int _sv0t7 = (0 - 2);
    return _sv0t7;
  } else {
  }
  int _sv0t8 = sv0_vec_len(dst);
  int dst_bn = _sv0t8;
  int j = 0;
  if ((bt >= 0)) {
    while ((j < sc)) {
      int _sv0t9 = (bn + j);
      int _sv0t10 = sv0_vec_get(parent, _sv0t9);
      sv0_vec_push(dst, _sv0t10);
      int _sv0t11 = (bt + j);
      int _sv0t12 = sv0_vec_get(parent, _sv0t11);
      sv0_vec_push(dst, _sv0t12);
      j = (j + 1);
    }
  } else {
    while ((j < sc)) {
      int _sv0t13 = (j * 2);
      int _sv0t14 = (bn + _sv0t13);
      int _sv0t15 = sv0_vec_get(parent, _sv0t14);
      sv0_vec_push(dst, _sv0t15);
      int _sv0t16 = (j * 2);
      int _sv0t17 = (bn + _sv0t16);
      int _sv0t18 = (_sv0t17 + 1);
      int _sv0t19 = sv0_vec_get(parent, _sv0t18);
      sv0_vec_push(dst, _sv0t19);
      j = (j + 1);
    }
  }
  sv0_vec_set(dst, 8, dst_bn);
  int _sv0t20 = (0 - 1);
  sv0_vec_set(dst, 9, _sv0t20);
  sv0_vec_set(dst, 10, sc);
  return 0;
}

static int lower_match_env_append_scrut_pair(int env, int name_h, int ty_h) {
  int _sv0t0 = lower_match_env_is_packed_v0(env);
  if ((_sv0t0 != 1)) {
    int _sv0t1 = (0 - 1);
    return _sv0t1;
  } else {
  }
  if ((name_h < 0)) {
    return 0;
  } else {
  }
  int _sv0t2 = sv0_vec_get(env, 10);
  int sc0 = _sv0t2;
  if ((sc0 == 0)) {
    int _sv0t3 = sv0_vec_len(env);
    int bsn = _sv0t3;
    sv0_vec_push(env, name_h);
    sv0_vec_push(env, ty_h);
    sv0_vec_set(env, 8, bsn);
    int _sv0t4 = (0 - 1);
    sv0_vec_set(env, 9, _sv0t4);
    sv0_vec_set(env, 10, 1);
    return 0;
  } else {
  }
  sv0_vec_push(env, name_h);
  sv0_vec_push(env, ty_h);
  int _sv0t5 = (sc0 + 1);
  sv0_vec_set(env, 10, _sv0t5);
  return 0;
}

static int lower_match_env_scrut_ty_lookup(int env, int limit, int name_h) {
  int _sv0t0 = lower_match_env_is_packed_v0(env);
  if ((_sv0t0 != 1)) {
    int _sv0t1 = (0 - 2);
    return _sv0t1;
  } else {
  }
  int _sv0t2 = sv0_vec_get(env, 10);
  int sc = _sv0t2;
  if ((sc <= 0)) {
    int _sv0t3 = (0 - 2);
    return _sv0t3;
  } else {
  }
  int _sv0t4 = sv0_vec_get(env, 8);
  int bn = _sv0t4;
  if ((bn < 0)) {
    int _sv0t5 = (0 - 2);
    return _sv0t5;
  } else {
  }
  int lim = sc;
  if ((limit < lim)) {
    lim = limit;
  } else {
  }
  int i = (lim - 1);
  while ((i >= 0)) {
    int _sv0t6 = (i * 2);
    int _sv0t7 = (bn + _sv0t6);
    int _sv0t8 = sv0_vec_get(env, _sv0t7);
    if ((_sv0t8 == name_h)) {
      int _sv0t9 = (i * 2);
      int _sv0t10 = (bn + _sv0t9);
      int _sv0t11 = (_sv0t10 + 1);
      int _sv0t12 = sv0_vec_get(env, _sv0t11);
      return _sv0t12;
    } else {
    }
    i = (i - 1);
  }
  int _sv0t13 = (0 - 2);
  return _sv0t13;
}

static int lower_top_fn_tables_for_match(int item_tags, int item_names, int item_d2, const char* source, int starts, int ends, int tok_tags, int fn_names, int fn_ret) {
  int _sv0t0 = sv0_vec_len(item_tags);
  int n = _sv0t0;
  int i = 0;
  while ((i < n)) {
    int _sv0t1 = sv0_vec_get(item_tags, i);
    if ((_sv0t1 == 0)) {
      int _sv0t2 = sv0_vec_get(item_names, i);
      int nm = _sv0t2;
      int _sv0t3 = sv0_vec_get(item_d2, i);
      int packed = _sv0t3;
      int has_ret = (packed % 2);
      int rh = (0 - 1);
      if ((has_ret == 1)) {
        int _sv0t4 = lower_scan_fn_ret_ty_tok(tok_tags, source, starts, ends, nm);
        int got = _sv0t4;
        rh = got;
      } else {
      }
      sv0_vec_push(fn_names, nm);
      sv0_vec_push(fn_ret, rh);
    } else {
    }
    i = (i + 1);
  }
  return 0;
}

static Value lower_tag_match(int et, int ed1, int ed2, int ed3, int ed4, int pp, int tok_tags, int idx, int ctr, int out_instrs, int enum_names, int enum_tag_offsets, int enum_tag_counts, int enum_tags_flat, int fn_ctx, int builtin_map, int lit_sf_names, const char* source, int starts, int ends, int item_tags, int item_names, int item_d2, int item_d3, int item_d4, int item_field_counts, int enum_vnames_flat, int enum_max_payloads, int item_fn_row, int match_env_inherit, int import_aliases) {
  int _sv0t0 = sv0_vec_get(ed1, idx);
  int scrut_idx = _sv0t0;
  int _sv0t1 = sv0_vec_get(ed2, idx);
  int arms_first = _sv0t1;
  int _sv0t2 = sv0_vec_get(ed3, idx);
  int arm_count = _sv0t2;
  int _sv0t3 = sv0_vec_get(ed4, idx);
  int arms_sidecar = _sv0t3;
  int _sv0t4 = lower_match_env_alloc_v0_empty_header();
  int match_env = _sv0t4;
  int _sv0t5 = lower_match_env_append_top_fn_match_tables(match_env, item_tags, item_names, item_d2, source, starts, ends, tok_tags);
  int discard_ap = _sv0t5;
  int _sv0t6 = lower_match_env_append_item_fn_param_rows(match_env, item_tags, item_names, item_d3, tok_tags, item_fn_row);
  int discard_pr = _sv0t6;
  int _sv0t7 = lower_match_env_merge_parent_scrut(match_env, match_env_inherit);
  int discard_mg = _sv0t7;
  int _sv0t8 = sv0_vec_new();
  int scrut_names_mt = _sv0t8;
  int _sv0t9 = sv0_vec_new();
  int scrut_ctys_mt = _sv0t9;
  int _sv0t10 = match_scrut_cty(et, ed1, ed2, ed3, ed4, scrut_idx, pp, match_env, import_aliases, scrut_names_mt, scrut_ctys_mt, 0);
  int sct = _sv0t10;
  int _sv0t11 = bind_scrut_var(et, ed1, ed2, ed3, ed4, pp, tok_tags, scrut_idx, ctr, out_instrs, enum_names, enum_tag_offsets, enum_tag_counts, enum_tags_flat, fn_ctx, builtin_map, lit_sf_names, item_tags, item_names, item_d2, item_d3, item_d4, item_field_counts, enum_vnames_flat, enum_max_payloads, item_fn_row, match_env, sct, source, starts, ends, import_aliases);
  int scrut_var = _sv0t11;
  int _sv0t12 = sv0_vec_get(et, scrut_idx);
  int st_scr = _sv0t12;
  if ((st_scr == 1)) {
    int _sv0t13 = sv0_vec_get(ed1, scrut_idx);
    int pps_s2 = _sv0t13;
    int _sv0t14 = sv0_vec_get(ed2, scrut_idx);
    int ppc_s2 = _sv0t14;
    if ((ppc_s2 == 1)) {
      int _sv0t15 = sv0_vec_get(pp, pps_s2);
      int nh2 = _sv0t15;
      int _sv0t16 = lower_match_env_append_scrut_pair(match_env, nh2, sct);
      int dps2 = _sv0t16;
    } else {
    }
  } else {
  }
  int _sv0t17 = ctr_fresh(ctr);
  int out = _sv0t17;
  Instr dv;
  dv.tag = 1;
  dv.p0 = out;
  int _sv0t18 = sv0_box_alloc(5);
  sv0_box_store(_sv0t18, 0, dv.tag);
  sv0_box_store(_sv0t18, 1, dv.p0);
  sv0_box_store(_sv0t18, 2, dv.p1);
  sv0_box_store(_sv0t18, 3, dv.p2);
  sv0_box_store(_sv0t18, 4, dv.p3);
  sv0_vec_push(out_instrs, _sv0t18);
  int _sv0t19 = lower_match_arms(et, ed1, ed2, ed3, ed4, pp, tok_tags, arms_first, arm_count, arms_sidecar, out, scrut_var, sct, ctr, out_instrs, enum_names, enum_tag_offsets, enum_tag_counts, enum_tags_flat, fn_ctx, builtin_map, lit_sf_names, item_tags, item_names, item_d2, item_d3, item_d4, item_field_counts, enum_vnames_flat, enum_max_payloads, item_fn_row, match_env, source, starts, ends, import_aliases);
  int discard_ma = _sv0t19;
  Value _sv0t20;
  _sv0t20.tag = 3;
  _sv0t20.p0 = out;
  return _sv0t20;
}

static int lower_return(int et, int ed1, int ed2, int ed3, int ed4, int pp, int tok_tags, int idx, int ctr, int out_instrs, int enum_names, int enum_tag_offsets, int enum_tag_counts, int enum_tags_flat, int fn_ctx, int builtin_map, int lit_sf_names, const char* source, int starts, int ends, int item_tags, int item_names, int item_d2, int item_d3, int item_d4, int item_field_counts, int enum_vnames_flat, int enum_max_payloads, int item_fn_row, int match_env_inherit, int import_aliases) {
  if ((idx < 0)) {
    Instr rv;
    rv.tag = 14;
    int _sv0t0 = sv0_box_alloc(5);
    sv0_box_store(_sv0t0, 0, rv.tag);
    sv0_box_store(_sv0t0, 1, rv.p0);
    sv0_box_store(_sv0t0, 2, rv.p1);
    sv0_box_store(_sv0t0, 3, rv.p2);
    sv0_box_store(_sv0t0, 4, rv.p3);
    sv0_vec_push(out_instrs, _sv0t0);
    return 0;
  } else {
  }
  Value _sv0t1 = lower_expr_to_value(et, ed1, ed2, ed3, ed4, pp, tok_tags, idx, ctr, out_instrs, enum_names, enum_tag_offsets, enum_tag_counts, enum_tags_flat, fn_ctx, builtin_map, lit_sf_names, source, starts, ends, item_tags, item_names, item_d2, item_d3, item_d4, item_field_counts, enum_vnames_flat, enum_max_payloads, item_fn_row, match_env_inherit, import_aliases);
  Value v;
  v = _sv0t1;
  Instr ri;
  int _sv0t2 = sv0_box_alloc(3);
  sv0_box_store(_sv0t2, 0, v.tag);
  sv0_box_store(_sv0t2, 1, v.p0);
  sv0_box_store(_sv0t2, 2, v.p1);
  ri.tag = 15;
  ri.p0 = _sv0t2;
  int _sv0t3 = sv0_box_alloc(5);
  sv0_box_store(_sv0t3, 0, ri.tag);
  sv0_box_store(_sv0t3, 1, ri.p0);
  sv0_box_store(_sv0t3, 2, ri.p1);
  sv0_box_store(_sv0t3, 3, ri.p2);
  sv0_box_store(_sv0t3, 4, ri.p3);
  sv0_vec_push(out_instrs, _sv0t3);
  return 0;
}

static int lower_stmt(int et, int ed1, int ed2, int ed3, int ed4, int pp, int tok_tags, int s_idx, int ctr, int out_instrs, int enum_names, int enum_tag_offsets, int enum_tag_counts, int enum_tags_flat, int fn_ctx, int builtin_map, int lit_sf_names, const char* source, int starts, int ends, int item_tags, int item_names, int item_d2, int item_d3, int item_d4, int item_field_counts, int enum_vnames_flat, int enum_max_payloads, int item_fn_row, int match_env_inherit, int import_aliases, int str_locals) {
  int _sv0t0 = sv0_vec_get(et, s_idx);
  int s_tag = _sv0t0;
  if ((s_tag == 27)) {
    int _sv0t1 = sv0_vec_get(ed1, s_idx);
    int name_tok = _sv0t1;
    int _sv0t2 = sv0_vec_get(ed3, s_idx);
    int init_idx = _sv0t2;
    int let_declared = 0;
    int deferred_ty = (0 - 1);
    int has_deferred_decl = 0;
    int str_let = 0;
    int init_handled = 0;
    if ((init_idx >= 0)) {
      int _sv0t3 = sv0_vec_get(et, init_idx);
      if ((_sv0t3 == 24)) {
        int _sv0t4 = sv0_vec_get(ed2, init_idx);
        int sppc = _sv0t4;
        int _sv0t5 = sv0_vec_get(ed3, init_idx);
        int fc_ls = _sv0t5;
        if ((sppc >= 1)) {
          if ((fc_ls > 0)) {
            int _sv0t6 = sv0_vec_get(ed1, init_idx);
            int _sv0t7 = (_sv0t6 + sppc);
            int _sv0t8 = (_sv0t7 - 1);
            int _sv0t9 = sv0_vec_get(pp, _sv0t8);
            int sty_tok = _sv0t9;
            Instr decl_n;
            decl_n.tag = 2;
            decl_n.p0 = sty_tok;
            decl_n.p1 = name_tok;
            int _sv0t10 = sv0_box_alloc(5);
            sv0_box_store(_sv0t10, 0, decl_n.tag);
            sv0_box_store(_sv0t10, 1, decl_n.p0);
            sv0_box_store(_sv0t10, 2, decl_n.p1);
            sv0_box_store(_sv0t10, 3, decl_n.p2);
            sv0_box_store(_sv0t10, 4, decl_n.p3);
            sv0_vec_push(out_instrs, _sv0t10);
            let_declared = 1;
            int _sv0t11 = sv0_vec_get(ed4, init_idx);
            int sf0_ls = _sv0t11;
            int fi_ls = 0;
            while ((fi_ls < fc_ls)) {
              int _sv0t12 = (fi_ls * 2);
              int _sv0t13 = (sf0_ls + _sv0t12);
              int _sv0t14 = (_sv0t13 + 1);
              int _sv0t15 = sv0_vec_get(lit_sf_names, _sv0t14);
              int ch_ls = _sv0t15;
              Value _sv0t16 = lower_expr_to_value(et, ed1, ed2, ed3, ed4, pp, tok_tags, ch_ls, ctr, out_instrs, enum_names, enum_tag_offsets, enum_tag_counts, enum_tags_flat, fn_ctx, builtin_map, lit_sf_names, source, starts, ends, item_tags, item_names, item_d2, item_d3, item_d4, item_field_counts, enum_vnames_flat, enum_max_payloads, item_fn_row, match_env_inherit, import_aliases);
              Value fv_ls;
              fv_ls = _sv0t16;
              int _sv0t17 = (fi_ls * 2);
              int _sv0t18 = (sf0_ls + _sv0t17);
              int _sv0t19 = sv0_vec_get(lit_sf_names, _sv0t18);
              int fnm_ls = _sv0t19;
              Instr _sv0t20;
              Expr _sv0t21 = value_to_expr(fv_ls);
              int _sv0t22 = sv0_box_alloc(4);
              sv0_box_store(_sv0t22, 0, _sv0t21.tag);
              sv0_box_store(_sv0t22, 1, _sv0t21.p0);
              sv0_box_store(_sv0t22, 2, _sv0t21.p1);
              sv0_box_store(_sv0t22, 3, _sv0t21.p2);
              _sv0t20.tag = 5;
              _sv0t20.p0 = name_tok;
              _sv0t20.p1 = fnm_ls;
              _sv0t20.p2 = _sv0t22;
              int _sv0t23 = sv0_box_alloc(5);
              sv0_box_store(_sv0t23, 0, _sv0t20.tag);
              sv0_box_store(_sv0t23, 1, _sv0t20.p0);
              sv0_box_store(_sv0t23, 2, _sv0t20.p1);
              sv0_box_store(_sv0t23, 3, _sv0t20.p2);
              sv0_box_store(_sv0t23, 4, _sv0t20.p3);
              sv0_vec_push(out_instrs, _sv0t23);
              fi_ls = (fi_ls + 1);
            }
            init_handled = 1;
          } else {
          }
        } else {
        }
      } else {
      }
      int _sv0t24 = sv0_vec_get(et, init_idx);
      if ((_sv0t24 == 1)) {
        int _sv0t25 = sv0_vec_get(ed2, init_idx);
        if ((_sv0t25 == 2)) {
          int _sv0t26 = sv0_vec_get(ed1, init_idx);
          int _sv0t27 = sv0_vec_get(pp, _sv0t26);
          int ety_tok = _sv0t27;
          int _sv0t28 = sv0_vec_get(ed1, init_idx);
          int _sv0t29 = (_sv0t28 + 1);
          int _sv0t30 = sv0_vec_get(pp, _sv0t29);
          int vn_tok_n = _sv0t30;
          Instr _sv0t31;
          _sv0t31.tag = 2;
          _sv0t31.p0 = ety_tok;
          _sv0t31.p1 = name_tok;
          int _sv0t32 = sv0_box_alloc(5);
          sv0_box_store(_sv0t32, 0, _sv0t31.tag);
          sv0_box_store(_sv0t32, 1, _sv0t31.p0);
          sv0_box_store(_sv0t32, 2, _sv0t31.p1);
          sv0_box_store(_sv0t32, 3, _sv0t31.p2);
          sv0_box_store(_sv0t32, 4, _sv0t31.p3);
          sv0_vec_push(out_instrs, _sv0t32);
          int _sv0t33 = enum_tag_lookup_str(enum_names, enum_tags_flat, enum_tag_offsets, enum_tag_counts, ety_tok, vn_tok_n, source, starts, ends);
          int k_n = _sv0t33;
          Expr tag_en;
          Value _sv0t34;
          _sv0t34.tag = 0;
          _sv0t34.p0 = k_n;
          int _sv0t35 = sv0_box_alloc(3);
          sv0_box_store(_sv0t35, 0, _sv0t34.tag);
          sv0_box_store(_sv0t35, 1, _sv0t34.p0);
          sv0_box_store(_sv0t35, 2, _sv0t34.p1);
          tag_en.tag = 0;
          tag_en.p0 = _sv0t35;
          Instr _sv0t36;
          int _sv0t37 = (0 - 1);
          int _sv0t38 = sv0_box_alloc(4);
          sv0_box_store(_sv0t38, 0, tag_en.tag);
          sv0_box_store(_sv0t38, 1, tag_en.p0);
          sv0_box_store(_sv0t38, 2, tag_en.p1);
          sv0_box_store(_sv0t38, 3, tag_en.p2);
          _sv0t36.tag = 5;
          _sv0t36.p0 = name_tok;
          _sv0t36.p1 = _sv0t37;
          _sv0t36.p2 = _sv0t38;
          int _sv0t39 = sv0_box_alloc(5);
          sv0_box_store(_sv0t39, 0, _sv0t36.tag);
          sv0_box_store(_sv0t39, 1, _sv0t36.p0);
          sv0_box_store(_sv0t39, 2, _sv0t36.p1);
          sv0_box_store(_sv0t39, 3, _sv0t36.p2);
          sv0_box_store(_sv0t39, 4, _sv0t36.p3);
          sv0_vec_push(out_instrs, _sv0t39);
          let_declared = 1;
          init_handled = 1;
        } else {
        }
      } else {
      }
      int _sv0t40 = sv0_vec_get(et, init_idx);
      if ((_sv0t40 == 4)) {
        int _sv0t41 = sv0_vec_get(ed1, init_idx);
        int callee_ci = _sv0t41;
        int _sv0t42 = sv0_vec_get(et, callee_ci);
        if ((_sv0t42 == 1)) {
          int _sv0t43 = sv0_vec_get(ed2, callee_ci);
          if ((_sv0t43 == 2)) {
            int _sv0t44 = sv0_vec_get(ed1, callee_ci);
            int _sv0t45 = sv0_vec_get(pp, _sv0t44);
            int en_tok_c = _sv0t45;
            int _sv0t46 = sv0_vec_get(ed1, callee_ci);
            int _sv0t47 = (_sv0t46 + 1);
            int _sv0t48 = sv0_vec_get(pp, _sv0t47);
            int vn_tok_c = _sv0t48;
            Instr _sv0t49;
            _sv0t49.tag = 2;
            _sv0t49.p0 = en_tok_c;
            _sv0t49.p1 = name_tok;
            int _sv0t50 = sv0_box_alloc(5);
            sv0_box_store(_sv0t50, 0, _sv0t49.tag);
            sv0_box_store(_sv0t50, 1, _sv0t49.p0);
            sv0_box_store(_sv0t50, 2, _sv0t49.p1);
            sv0_box_store(_sv0t50, 3, _sv0t49.p2);
            sv0_box_store(_sv0t50, 4, _sv0t49.p3);
            sv0_vec_push(out_instrs, _sv0t50);
            int _sv0t51 = enum_tag_lookup_str(enum_names, enum_tags_flat, enum_tag_offsets, enum_tag_counts, en_tok_c, vn_tok_c, source, starts, ends);
            int k_c = _sv0t51;
            int _sv0t52 = sv0_vec_get(ed2, init_idx);
            int args_first_c = _sv0t52;
            int _sv0t53 = sv0_vec_get(ed3, init_idx);
            int args_count_c = _sv0t53;
            int _sv0t54 = sv0_vec_get(ed4, init_idx);
            int args_sidecar_c = _sv0t54;
            int _sv0t55 = sv0_vec_new();
            int arg_vals_c = _sv0t55;
            int ai_c = 0;
            while ((ai_c < args_count_c)) {
              int _sv0t56 = block_stmt_index(pp, args_first_c, args_sidecar_c, ai_c);
              int a_idx_c = _sv0t56;
              Value _sv0t57 = lower_expr_to_value(et, ed1, ed2, ed3, ed4, pp, tok_tags, a_idx_c, ctr, out_instrs, enum_names, enum_tag_offsets, enum_tag_counts, enum_tags_flat, fn_ctx, builtin_map, lit_sf_names, source, starts, ends, item_tags, item_names, item_d2, item_d3, item_d4, item_field_counts, enum_vnames_flat, enum_max_payloads, item_fn_row, match_env_inherit, import_aliases);
              Value av_c;
              av_c = _sv0t57;
              int _sv0t58 = sv0_box_alloc(3);
              sv0_box_store(_sv0t58, 0, av_c.tag);
              sv0_box_store(_sv0t58, 1, av_c.p0);
              sv0_box_store(_sv0t58, 2, av_c.p1);
              sv0_vec_push(arg_vals_c, _sv0t58);
              ai_c = (ai_c + 1);
            }
            Expr tag_ec;
            Value _sv0t59;
            _sv0t59.tag = 0;
            _sv0t59.p0 = k_c;
            int _sv0t60 = sv0_box_alloc(3);
            sv0_box_store(_sv0t60, 0, _sv0t59.tag);
            sv0_box_store(_sv0t60, 1, _sv0t59.p0);
            sv0_box_store(_sv0t60, 2, _sv0t59.p1);
            tag_ec.tag = 0;
            tag_ec.p0 = _sv0t60;
            Instr _sv0t61;
            int _sv0t62 = (0 - 1);
            int _sv0t63 = sv0_box_alloc(4);
            sv0_box_store(_sv0t63, 0, tag_ec.tag);
            sv0_box_store(_sv0t63, 1, tag_ec.p0);
            sv0_box_store(_sv0t63, 2, tag_ec.p1);
            sv0_box_store(_sv0t63, 3, tag_ec.p2);
            _sv0t61.tag = 5;
            _sv0t61.p0 = name_tok;
            _sv0t61.p1 = _sv0t62;
            _sv0t61.p2 = _sv0t63;
            int _sv0t64 = sv0_box_alloc(5);
            sv0_box_store(_sv0t64, 0, _sv0t61.tag);
            sv0_box_store(_sv0t64, 1, _sv0t61.p0);
            sv0_box_store(_sv0t64, 2, _sv0t61.p1);
            sv0_box_store(_sv0t64, 3, _sv0t61.p2);
            sv0_box_store(_sv0t64, 4, _sv0t61.p3);
            sv0_vec_push(out_instrs, _sv0t64);
            int pi_c = 0;
            while ((pi_c < args_count_c)) {
              int _sv0t65 = sv0_vec_get(arg_vals_c, pi_c);
              Value _sv0t66;
              int _sv0t67 = sv0_box_load(_sv0t65, 0);
              _sv0t66.tag = _sv0t67;
              int _sv0t68 = sv0_box_load(_sv0t65, 1);
              _sv0t66.p0 = _sv0t68;
              int _sv0t69 = sv0_box_load(_sv0t65, 2);
              _sv0t66.p1 = _sv0t69;
              Value pv_c;
              pv_c.tag = (_sv0t66).tag;
              pv_c.p0 = (_sv0t66).p0;
              pv_c.p1 = (_sv0t66).p1;
              int _sv0t70 = (0 - 10);
              int fs_c = (_sv0t70 - pi_c);
              Instr _sv0t71;
              Expr _sv0t72 = value_to_expr(pv_c);
              int _sv0t73 = sv0_box_alloc(4);
              sv0_box_store(_sv0t73, 0, _sv0t72.tag);
              sv0_box_store(_sv0t73, 1, _sv0t72.p0);
              sv0_box_store(_sv0t73, 2, _sv0t72.p1);
              sv0_box_store(_sv0t73, 3, _sv0t72.p2);
              _sv0t71.tag = 5;
              _sv0t71.p0 = name_tok;
              _sv0t71.p1 = fs_c;
              _sv0t71.p2 = _sv0t73;
              int _sv0t74 = sv0_box_alloc(5);
              sv0_box_store(_sv0t74, 0, _sv0t71.tag);
              sv0_box_store(_sv0t74, 1, _sv0t71.p0);
              sv0_box_store(_sv0t74, 2, _sv0t71.p1);
              sv0_box_store(_sv0t74, 3, _sv0t71.p2);
              sv0_box_store(_sv0t74, 4, _sv0t71.p3);
              sv0_vec_push(out_instrs, _sv0t74);
              pi_c = (pi_c + 1);
            }
            let_declared = 1;
            init_handled = 1;
          } else {
          }
        } else {
        }
      } else {
      }
      int _sv0t75 = lower_expr_yields_string(et, ed1, ed2, ed3, pp, init_idx, item_tags, item_names, item_d2, tok_tags, source, starts, ends);
      if (_sv0t75) {
        int _sv0t76 = lower_string_cty();
        deferred_ty = _sv0t76;
        has_deferred_decl = 1;
        let_declared = 1;
        str_let = 1;
      } else {
      }
      if ((let_declared == 0)) {
        int _sv0t77 = lower_call_ret_is_string(et, ed1, ed2, pp, init_idx, item_tags, item_names, item_d2, tok_tags, source, starts, ends);
        if (_sv0t77) {
          Instr decl_su;
          int _sv0t78 = lower_string_cty();
          decl_su.tag = 2;
          decl_su.p0 = _sv0t78;
          decl_su.p1 = name_tok;
          int _sv0t79 = sv0_box_alloc(5);
          sv0_box_store(_sv0t79, 0, decl_su.tag);
          sv0_box_store(_sv0t79, 1, decl_su.p0);
          sv0_box_store(_sv0t79, 2, decl_su.p1);
          sv0_box_store(_sv0t79, 3, decl_su.p2);
          sv0_box_store(_sv0t79, 4, decl_su.p3);
          sv0_vec_push(out_instrs, _sv0t79);
          let_declared = 1;
          str_let = 1;
        } else {
        }
      } else {
      }
      if ((let_declared == 0)) {
        int _sv0t80 = lower_init_ident_is_string_param(et, ed1, ed2, pp, init_idx, item_tags, item_names, item_d3, tok_tags, source, starts, ends, item_fn_row);
        if (_sv0t80) {
          Instr decl_sp;
          int _sv0t81 = lower_string_cty();
          decl_sp.tag = 2;
          decl_sp.p0 = _sv0t81;
          decl_sp.p1 = name_tok;
          int _sv0t82 = sv0_box_alloc(5);
          sv0_box_store(_sv0t82, 0, decl_sp.tag);
          sv0_box_store(_sv0t82, 1, decl_sp.p0);
          sv0_box_store(_sv0t82, 2, decl_sp.p1);
          sv0_box_store(_sv0t82, 3, decl_sp.p2);
          sv0_box_store(_sv0t82, 4, decl_sp.p3);
          sv0_vec_push(out_instrs, _sv0t82);
          let_declared = 1;
          str_let = 1;
        } else {
        }
      } else {
      }
      if ((let_declared == 0)) {
        int _sv0t83 = lower_ident_in_str_locals(et, ed1, ed2, pp, init_idx, str_locals, source, starts, ends);
        if (_sv0t83) {
          Instr decl_sl;
          int _sv0t84 = lower_string_cty();
          decl_sl.tag = 2;
          decl_sl.p0 = _sv0t84;
          decl_sl.p1 = name_tok;
          int _sv0t85 = sv0_box_alloc(5);
          sv0_box_store(_sv0t85, 0, decl_sl.tag);
          sv0_box_store(_sv0t85, 1, decl_sl.p0);
          sv0_box_store(_sv0t85, 2, decl_sl.p1);
          sv0_box_store(_sv0t85, 3, decl_sl.p2);
          sv0_box_store(_sv0t85, 4, decl_sl.p3);
          sv0_vec_push(out_instrs, _sv0t85);
          let_declared = 1;
          str_let = 1;
        } else {
        }
      } else {
      }
      if ((let_declared == 0)) {
        int _sv0t86 = lower_call_ret_user_ty_tok(et, ed1, ed2, pp, init_idx, item_tags, item_names, item_d2, tok_tags, source, starts, ends);
        int uty_ls = _sv0t86;
        if ((uty_ls >= 0)) {
          deferred_ty = uty_ls;
          has_deferred_decl = 1;
        } else {
        }
      } else {
      }
      if ((let_declared == 0)) {
        int _sv0t87 = sv0_vec_get(ed2, s_idx);
        int _sv0t88 = lower_typed_box_deref_let(et, ed1, ed2, ed3, ed4, pp, tok_tags, init_idx, name_tok, _sv0t87, ctr, out_instrs, enum_names, enum_tag_offsets, enum_tag_counts, enum_tags_flat, fn_ctx, builtin_map, lit_sf_names, source, starts, ends, item_tags, item_names, item_d2, item_d3, item_d4, item_field_counts, enum_vnames_flat, enum_max_payloads, item_fn_row, match_env_inherit, import_aliases);
        int bd = _sv0t88;
        if ((bd == 1)) {
          let_declared = 1;
          init_handled = 1;
        } else {
        }
      } else {
      }
    } else {
    }
    if ((str_let == 1)) {
      sv0_vec_push(str_locals, name_tok);
    } else {
    }
    if ((init_idx >= 0)) {
      if ((init_handled == 0)) {
        Expr _sv0t89 = lower_expr_with_instrs(et, ed1, ed2, ed3, ed4, pp, tok_tags, init_idx, ctr, out_instrs, enum_names, enum_tag_offsets, enum_tag_counts, enum_tags_flat, fn_ctx, builtin_map, lit_sf_names, source, starts, ends, item_tags, item_names, item_d2, item_d3, item_d4, item_field_counts, enum_vnames_flat, enum_max_payloads, item_fn_row, match_env_inherit, import_aliases);
        Expr e;
        e = _sv0t89;
        if ((has_deferred_decl == 1)) {
          Instr _sv0t90;
          _sv0t90.tag = 2;
          _sv0t90.p0 = deferred_ty;
          _sv0t90.p1 = name_tok;
          int _sv0t91 = sv0_box_alloc(5);
          sv0_box_store(_sv0t91, 0, _sv0t90.tag);
          sv0_box_store(_sv0t91, 1, _sv0t90.p0);
          sv0_box_store(_sv0t91, 2, _sv0t90.p1);
          sv0_box_store(_sv0t91, 3, _sv0t90.p2);
          sv0_box_store(_sv0t91, 4, _sv0t90.p3);
          sv0_vec_push(out_instrs, _sv0t91);
        } else {
          if ((let_declared == 0)) {
            Instr _sv0t92;
            _sv0t92.tag = 1;
            _sv0t92.p0 = name_tok;
            int _sv0t93 = sv0_box_alloc(5);
            sv0_box_store(_sv0t93, 0, _sv0t92.tag);
            sv0_box_store(_sv0t93, 1, _sv0t92.p0);
            sv0_box_store(_sv0t93, 2, _sv0t92.p1);
            sv0_box_store(_sv0t93, 3, _sv0t92.p2);
            sv0_box_store(_sv0t93, 4, _sv0t92.p3);
            sv0_vec_push(out_instrs, _sv0t93);
          } else {
          }
        }
        Instr si;
        int _sv0t94 = sv0_box_alloc(4);
        sv0_box_store(_sv0t94, 0, e.tag);
        sv0_box_store(_sv0t94, 1, e.p0);
        sv0_box_store(_sv0t94, 2, e.p1);
        sv0_box_store(_sv0t94, 3, e.p2);
        si.tag = 4;
        si.p0 = name_tok;
        si.p1 = _sv0t94;
        int _sv0t95 = sv0_box_alloc(5);
        sv0_box_store(_sv0t95, 0, si.tag);
        sv0_box_store(_sv0t95, 1, si.p0);
        sv0_box_store(_sv0t95, 2, si.p1);
        sv0_box_store(_sv0t95, 3, si.p2);
        sv0_box_store(_sv0t95, 4, si.p3);
        sv0_vec_push(out_instrs, _sv0t95);
      } else {
      }
    } else {
      if ((let_declared == 0)) {
        Instr _sv0t96;
        _sv0t96.tag = 1;
        _sv0t96.p0 = name_tok;
        int _sv0t97 = sv0_box_alloc(5);
        sv0_box_store(_sv0t97, 0, _sv0t96.tag);
        sv0_box_store(_sv0t97, 1, _sv0t96.p0);
        sv0_box_store(_sv0t97, 2, _sv0t96.p1);
        sv0_box_store(_sv0t97, 3, _sv0t96.p2);
        sv0_box_store(_sv0t97, 4, _sv0t96.p3);
        sv0_vec_push(out_instrs, _sv0t97);
      } else {
      }
    }
    return 0;
  } else {
  }
  if ((s_tag == 28)) {
    int _sv0t98 = sv0_vec_get(ed1, s_idx);
    int expr_idx = _sv0t98;
    int _sv0t99 = sv0_vec_get(et, expr_idx);
    if ((_sv0t99 == 15)) {
      int _sv0t100 = sv0_vec_get(ed1, expr_idx);
      int ret_val = _sv0t100;
      int _sv0t101 = lower_return(et, ed1, ed2, ed3, ed4, pp, tok_tags, ret_val, ctr, out_instrs, enum_names, enum_tag_offsets, enum_tag_counts, enum_tags_flat, fn_ctx, builtin_map, lit_sf_names, source, starts, ends, item_tags, item_names, item_d2, item_d3, item_d4, item_field_counts, enum_vnames_flat, enum_max_payloads, item_fn_row, match_env_inherit, import_aliases);
      return _sv0t101;
    } else {
    }
    int _sv0t102 = lower_for_effect(et, ed1, ed2, ed3, ed4, pp, tok_tags, expr_idx, ctr, out_instrs, enum_names, enum_tag_offsets, enum_tag_counts, enum_tags_flat, fn_ctx, builtin_map, lit_sf_names, source, starts, ends, item_tags, item_names, item_d2, item_d3, item_d4, item_field_counts, enum_vnames_flat, enum_max_payloads, item_fn_row, match_env_inherit, import_aliases);
    int rc = _sv0t102;
    return 0;
  } else {
  }
  return 0;
}

static int lower_block(int et, int ed1, int ed2, int ed3, int ed4, int pp, int tok_tags, int stmts_first, int stmts_count, int tail_idx, int block_sidecar, int ctr, int out_instrs, int enum_names, int enum_tag_offsets, int enum_tag_counts, int enum_tags_flat, int fn_ctx, int builtin_map, int lit_sf_names, const char* source, int starts, int ends, int item_tags, int item_names, int item_d2, int item_d3, int item_d4, int item_field_counts, int enum_vnames_flat, int enum_max_payloads, int item_fn_row, int match_env_inherit, int import_aliases) {
  int si = 0;
  int _sv0t0 = sv0_vec_new();
  int str_locals = _sv0t0;
  while (1) {
    if ((si >= stmts_count)) {
      break;
    } else {
    }
    int _sv0t1 = block_stmt_index(pp, stmts_first, block_sidecar, si);
    int s_idx = _sv0t1;
    int _sv0t2 = lower_stmt(et, ed1, ed2, ed3, ed4, pp, tok_tags, s_idx, ctr, out_instrs, enum_names, enum_tag_offsets, enum_tag_counts, enum_tags_flat, fn_ctx, builtin_map, lit_sf_names, source, starts, ends, item_tags, item_names, item_d2, item_d3, item_d4, item_field_counts, enum_vnames_flat, enum_max_payloads, item_fn_row, match_env_inherit, import_aliases, str_locals);
    int rc = _sv0t2;
    si = (si + 1);
  }
  if ((tail_idx >= 0)) {
    int _sv0t3 = lower_return(et, ed1, ed2, ed3, ed4, pp, tok_tags, tail_idx, ctr, out_instrs, enum_names, enum_tag_offsets, enum_tag_counts, enum_tags_flat, fn_ctx, builtin_map, lit_sf_names, source, starts, ends, item_tags, item_names, item_d2, item_d3, item_d4, item_field_counts, enum_vnames_flat, enum_max_payloads, item_fn_row, match_env_inherit, import_aliases);
    int rc2 = _sv0t3;
  } else {
  }
  return 0;
}

static int lower_body(int et, int ed1, int ed2, int ed3, int ed4, int pp, int tok_tags, int body_idx, int ctr, int out_instrs, int enum_names, int enum_tag_offsets, int enum_tag_counts, int enum_tags_flat, int fn_ctx, int builtin_map, int lit_sf_names, const char* source, int starts, int ends, int item_tags, int item_names, int item_d2, int item_d3, int item_d4, int item_field_counts, int enum_vnames_flat, int enum_max_payloads, int item_fn_row, int match_env_inherit, int import_aliases) {
  int _sv0t0 = sv0_vec_get(et, body_idx);
  int btag = _sv0t0;
  if ((btag == 9)) {
    int _sv0t1 = sv0_vec_get(ed1, body_idx);
    int sf = _sv0t1;
    int _sv0t2 = sv0_vec_get(ed2, body_idx);
    int sc = _sv0t2;
    int _sv0t3 = sv0_vec_get(ed3, body_idx);
    int ti = _sv0t3;
    int _sv0t4 = sv0_vec_get(ed4, body_idx);
    int sidecar = _sv0t4;
    int _sv0t5 = lower_block(et, ed1, ed2, ed3, ed4, pp, tok_tags, sf, sc, ti, sidecar, ctr, out_instrs, enum_names, enum_tag_offsets, enum_tag_counts, enum_tags_flat, fn_ctx, builtin_map, lit_sf_names, source, starts, ends, item_tags, item_names, item_d2, item_d3, item_d4, item_field_counts, enum_vnames_flat, enum_max_payloads, item_fn_row, match_env_inherit, import_aliases);
    return _sv0t5;
  } else {
  }
  int _sv0t6 = lower_return(et, ed1, ed2, ed3, ed4, pp, tok_tags, body_idx, ctr, out_instrs, enum_names, enum_tag_offsets, enum_tag_counts, enum_tags_flat, fn_ctx, builtin_map, lit_sf_names, source, starts, ends, item_tags, item_names, item_d2, item_d3, item_d4, item_field_counts, enum_vnames_flat, enum_max_payloads, item_fn_row, match_env_inherit, import_aliases);
  return _sv0t6;
}

static int lower_fn(int et, int ed1, int ed2, int ed3, int ed4, int pp, int tok_tags, int body_idx, int fn_name_h, int req_roots, int ens_roots, int out_instrs, int enum_names, int enum_tag_offsets, int enum_tag_counts, int enum_tags_flat, int fn_ctx, int builtin_map, int lit_sf_names, const char* source, int starts, int ends, int item_tags, int item_names, int item_d2, int item_d3, int item_d4, int item_field_counts, int enum_vnames_flat, int enum_max_payloads, int item_fn_row, int match_env_inherit, int import_aliases) {
  int _sv0t0 = sv0_vec_new();
  int ctr = _sv0t0;
  int _sv0t1 = lower_temp_base();
  sv0_vec_push(ctr, _sv0t1);
  int _sv0t2 = sv0_vec_new();
  int req_instrs = _sv0t2;
  int ri = 0;
  while (1) {
    int _sv0t3 = sv0_vec_len(req_roots);
    int _sv0t9 = (ri < _sv0t3);
    if ((!_sv0t9)) {
      break;
    } else {
    }
    int _sv0t4 = sv0_vec_get(req_roots, ri);
    int r_idx = _sv0t4;
    Expr _sv0t5 = lower_expr_with_instrs(et, ed1, ed2, ed3, ed4, pp, tok_tags, r_idx, ctr, req_instrs, enum_names, enum_tag_offsets, enum_tag_counts, enum_tags_flat, fn_ctx, builtin_map, lit_sf_names, source, starts, ends, item_tags, item_names, item_d2, item_d3, item_d4, item_field_counts, enum_vnames_flat, enum_max_payloads, item_fn_row, match_env_inherit, import_aliases);
    Expr re;
    re = _sv0t5;
    Instr _sv0t6;
    int _sv0t7 = sv0_box_alloc(4);
    sv0_box_store(_sv0t7, 0, re.tag);
    sv0_box_store(_sv0t7, 1, re.p0);
    sv0_box_store(_sv0t7, 2, re.p1);
    sv0_box_store(_sv0t7, 3, re.p2);
    _sv0t6.tag = 12;
    _sv0t6.p0 = _sv0t7;
    _sv0t6.p1 = fn_name_h;
    int _sv0t8 = sv0_box_alloc(5);
    sv0_box_store(_sv0t8, 0, _sv0t6.tag);
    sv0_box_store(_sv0t8, 1, _sv0t6.p0);
    sv0_box_store(_sv0t8, 2, _sv0t6.p1);
    sv0_box_store(_sv0t8, 3, _sv0t6.p2);
    sv0_box_store(_sv0t8, 4, _sv0t6.p3);
    sv0_vec_push(req_instrs, _sv0t8);
    ri = (ri + 1);
  }
  int _sv0t10 = sv0_vec_new();
  int body_instrs = _sv0t10;
  int _sv0t11 = lower_body(et, ed1, ed2, ed3, ed4, pp, tok_tags, body_idx, ctr, body_instrs, enum_names, enum_tag_offsets, enum_tag_counts, enum_tags_flat, fn_ctx, builtin_map, lit_sf_names, source, starts, ends, item_tags, item_names, item_d2, item_d3, item_d4, item_field_counts, enum_vnames_flat, enum_max_payloads, item_fn_row, match_env_inherit, import_aliases);
  int rc_body = _sv0t11;
  if ((rc_body != 0)) {
    return rc_body;
  } else {
  }
  int mentions_res = 0;
  int ei = 0;
  while (1) {
    int _sv0t12 = sv0_vec_len(ens_roots);
    int _sv0t15 = (ei < _sv0t12);
    if ((!_sv0t15)) {
      break;
    } else {
    }
    int _sv0t13 = sv0_vec_get(ens_roots, ei);
    int _sv0t14 = mentions_result(et, ed1, ed2, ed3, ed4, _sv0t13, source, starts, ends, pp);
    if (_sv0t14) {
      mentions_res = 1;
    } else {
    }
    ei = (ei + 1);
  }
  int _sv0t16 = use_ret_slot(0, mentions_res);
  int use_slot = _sv0t16;
  int ret_slot = (0 - 1);
  int _sv0t17 = sv0_vec_new();
  int decl_instrs = _sv0t17;
  if (use_slot) {
    int rh = (0 - 1);
    int rk = 0;
    while (1) {
      int _sv0t18 = sv0_vec_len(ens_roots);
      int _sv0t21 = (rk < _sv0t18);
      if ((!_sv0t21)) {
        break;
      } else {
      }
      int _sv0t19 = sv0_vec_get(ens_roots, rk);
      int _sv0t20 = first_result_path_handle(et, ed1, ed2, ed3, ed4, _sv0t19, source, starts, ends, pp);
      int c = _sv0t20;
      if ((c >= 0)) {
        rh = c;
      } else {
      }
      rk = (rk + 1);
    }
    if ((rh >= 0)) {
      ret_slot = rh;
    } else {
      int _sv0t22 = ctr_fresh(ctr);
      ret_slot = _sv0t22;
    }
    Instr _sv0t23;
    _sv0t23.tag = 1;
    _sv0t23.p0 = ret_slot;
    int _sv0t24 = sv0_box_alloc(5);
    sv0_box_store(_sv0t24, 0, _sv0t23.tag);
    sv0_box_store(_sv0t24, 1, _sv0t23.p0);
    sv0_box_store(_sv0t24, 2, _sv0t23.p1);
    sv0_box_store(_sv0t24, 3, _sv0t23.p2);
    sv0_box_store(_sv0t24, 4, _sv0t23.p3);
    sv0_vec_push(decl_instrs, _sv0t24);
  } else {
  }
  int _sv0t25 = sv0_vec_new();
  int ens_instrs = _sv0t25;
  int ej = 0;
  while (1) {
    int _sv0t26 = sv0_vec_len(ens_roots);
    int _sv0t29 = (ej < _sv0t26);
    if ((!_sv0t29)) {
      break;
    } else {
    }
    int _sv0t27 = sv0_vec_get(ens_roots, ej);
    int _sv0t28 = lower_ensures_clause(et, ed1, ed2, ed3, ed4, pp, tok_tags, _sv0t27, use_slot, fn_name_h, ctr, ens_instrs, enum_names, enum_tag_offsets, enum_tag_counts, enum_tags_flat, fn_ctx, builtin_map, lit_sf_names, source, starts, ends, item_tags, item_names, item_d2, item_d3, item_d4, item_field_counts, enum_vnames_flat, enum_max_payloads, item_fn_row, match_env_inherit, import_aliases);
    int discard_ens = _sv0t28;
    ej = (ej + 1);
  }
  int _sv0t30 = inject_ensures_and_ret_slot(body_instrs, ens_instrs, use_slot, ret_slot);
  int body_final = _sv0t30;
  int _sv0t31 = append_instrs(out_instrs, decl_instrs);
  int discard_append_decl = _sv0t31;
  int _sv0t32 = append_instrs(out_instrs, req_instrs);
  int discard_append_req = _sv0t32;
  int _sv0t33 = append_instrs(out_instrs, body_final);
  int discard_append_body = _sv0t33;
  return 0;
}

static int build_builtin_map_classify(const char* s) {
  int _sv0t0 = sv0_string_eq(s, "println");
  if (_sv0t0) {
    return 1;
  } else {
  }
  int _sv0t1 = sv0_string_eq(s, "string_len");
  if (_sv0t1) {
    return 2;
  } else {
  }
  int _sv0t2 = sv0_string_eq(s, "string_eq");
  if (_sv0t2) {
    return 3;
  } else {
  }
  int _sv0t3 = sv0_string_eq(s, "string_concat");
  if (_sv0t3) {
    return 4;
  } else {
  }
  int _sv0t4 = sv0_string_eq(s, "string_char_at");
  if (_sv0t4) {
    return 5;
  } else {
  }
  int _sv0t5 = sv0_string_eq(s, "string_substr");
  if (_sv0t5) {
    return 6;
  } else {
  }
  int _sv0t6 = sv0_string_eq(s, "vec_new");
  if (_sv0t6) {
    return 7;
  } else {
  }
  int _sv0t7 = sv0_string_eq(s, "vec_push");
  if (_sv0t7) {
    return 8;
  } else {
  }
  int _sv0t8 = sv0_string_eq(s, "vec_len");
  if (_sv0t8) {
    return 9;
  } else {
  }
  int _sv0t9 = sv0_string_eq(s, "vec_get");
  if (_sv0t9) {
    return 10;
  } else {
  }
  int _sv0t10 = sv0_string_eq(s, "vec_set");
  if (_sv0t10) {
    return 11;
  } else {
  }
  int _sv0t11 = sv0_string_eq(s, "box_new");
  if (_sv0t11) {
    return 12;
  } else {
  }
  int _sv0t12 = sv0_string_eq(s, "box_deref");
  if (_sv0t12) {
    return 13;
  } else {
  }
  int _sv0t13 = sv0_string_eq(s, "no_alias");
  if (_sv0t13) {
    return 14;
  } else {
  }
  int _sv0t14 = sv0_string_eq(s, "read_file");
  if (_sv0t14) {
    return 15;
  } else {
  }
  int _sv0t15 = sv0_string_eq(s, "write_file");
  if (_sv0t15) {
    return 16;
  } else {
  }
  int _sv0t16 = sv0_string_eq(s, "read_dir");
  if (_sv0t16) {
    return 17;
  } else {
  }
  int _sv0t17 = sv0_string_eq(s, "write_bytes");
  if (_sv0t17) {
    return 18;
  } else {
  }
  return 0;
}

static int build_builtin_map(int tok_tags, const char* source, int starts, int ends) {
  int _sv0t0 = sv0_vec_len(tok_tags);
  int n = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int bm = _sv0t1;
  int i = 0;
  while ((i < n)) {
    int bid = 0;
    int _sv0t2 = sv0_vec_get(tok_tags, i);
    if ((_sv0t2 == 5)) {
      const char* _sv0t3 = handle_to_str(i, source, starts, ends);
      const char* s;
      s = _sv0t3;
      int _sv0t4 = build_builtin_map_classify(s);
      bid = _sv0t4;
    } else {
    }
    sv0_vec_push(bm, bid);
    i = (i + 1);
  }
  return bm;
}

static const char* lower(int item_tags, int item_names, int item_d2, int item_d3, int item_d4, int item_field_counts, int item_vmax, int struct_fnames_flat, int enum_vnames_flat, int sf_names, int sf_types, const char* source, int starts, int ends, int out_block_labels, int out_struct_names, int out_struct_offsets, int out_struct_counts, int out_struct_flat, int out_enum_names, int out_enum_tag_offsets, int out_enum_tag_counts, int out_enum_tag_flat, int out_enum_max, int body_et, int body_ed1, int body_ed2, int body_ed3, int body_ed4, int body_sf, int body_pp, int tok_tags, int out_blocks, int import_aliases) {
  int _sv0t0 = lower_fill_import_aliases_from_use_items(import_aliases, item_tags, item_names, item_d2, item_d3, body_pp, source, starts, ends);
  int discard_fill_alias = _sv0t0;
  int _sv0t1 = build_struct_order(item_tags, item_names, item_field_counts, struct_fnames_flat, out_struct_names, out_struct_offsets, out_struct_counts, out_struct_flat);
  int discard_so = _sv0t1;
  int _sv0t2 = build_enum_variants_data(item_tags, item_names, item_field_counts, item_vmax, enum_vnames_flat, out_enum_names, out_enum_tag_offsets, out_enum_tag_counts, out_enum_tag_flat, out_enum_max);
  int discard_ev = _sv0t2;
  const char* _sv0t3 = collect_typedefs_str(item_tags, item_names, item_field_counts, item_vmax, sf_names, sf_types, source, starts, ends, out_struct_names, out_enum_names);
  const char* typedefs;
  typedefs = _sv0t3;
  int _sv0t4 = sv0_vec_len(body_et);
  int has_body_arenas = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int builtin_map = _sv0t5;
  if ((has_body_arenas > 0)) {
    int _sv0t6 = build_builtin_map(tok_tags, source, starts, ends);
    builtin_map = _sv0t6;
  } else {
  }
  int _sv0t7 = sv0_vec_new();
  int fn_ctx = _sv0t7;
  int _sv0t8 = sv0_vec_len(item_tags);
  int n = _sv0t8;
  int i = 0;
  while ((i < n)) {
    int _sv0t9 = sv0_vec_get(item_tags, i);
    if ((_sv0t9 == 0)) {
      int _sv0t10 = sv0_vec_get(item_names, i);
      int label_h = _sv0t10;
      sv0_vec_push(out_block_labels, label_h);
      int _sv0t11 = sv0_vec_get(item_d4, i);
      int body_root = _sv0t11;
      int _sv0t12 = sv0_vec_new();
      int instrs = _sv0t12;
      if ((has_body_arenas > 0)) {
        if ((body_root >= 0)) {
          int _sv0t13 = sv0_vec_new();
          int _sv0t14 = sv0_vec_new();
          int _sv0t15 = sv0_vec_new();
          int _sv0t16 = lower_fn(body_et, body_ed1, body_ed2, body_ed3, body_ed4, body_pp, tok_tags, body_root, label_h, _sv0t13, _sv0t14, instrs, out_enum_names, out_enum_tag_offsets, out_enum_tag_counts, out_enum_tag_flat, fn_ctx, builtin_map, body_sf, source, starts, ends, item_tags, item_names, item_d2, item_d3, item_d4, item_field_counts, enum_vnames_flat, out_enum_max, i, _sv0t15, import_aliases);
          int discard_lf = _sv0t16;
        } else {
        }
      } else {
      }
      int _sv0t17 = sv0_vec_new();
      int params = _sv0t17;
      int ret_h = 9;
      sv0_vec_push(out_blocks, label_h);
      sv0_vec_push(out_blocks, ret_h);
      int _sv0t18 = sv0_box_alloc(1);
      sv0_box_store(_sv0t18, 0, params);
      sv0_vec_push(out_blocks, _sv0t18);
      int _sv0t19 = sv0_box_alloc(1);
      sv0_box_store(_sv0t19, 0, instrs);
      sv0_vec_push(out_blocks, _sv0t19);
    } else {
    }
    i = (i + 1);
  }
  return typedefs;
}

static int lower_temp_base(void) {
  return 1000000;
}

static int lower_string_cty(void) {
  int _sv0t0 = (0 - 7);
  return _sv0t0;
}

static int lower_tok_in_bounds(int h, int starts, int ends) {
  if ((h < 0)) {
    return 0;
  } else {
  }
  int _sv0t0 = sv0_vec_len(starts);
  if ((h >= _sv0t0)) {
    return 0;
  } else {
  }
  int _sv0t1 = sv0_vec_len(ends);
  if ((h >= _sv0t1)) {
    return 0;
  } else {
  }
  return 1;
}

static int lower_call_ret_is_string(int et, int ed1, int ed2, int pp, int call_idx, int item_tags, int item_names, int item_d2, int tok_tags, const char* source, int starts, int ends) {
  if ((call_idx < 0)) {
    return 0;
  } else {
  }
  int _sv0t0 = sv0_vec_len(et);
  if ((call_idx >= _sv0t0)) {
    return 0;
  } else {
  }
  int _sv0t1 = sv0_vec_get(et, call_idx);
  if ((_sv0t1 != 4)) {
    return 0;
  } else {
  }
  int _sv0t2 = sv0_vec_get(ed1, call_idx);
  int callee = _sv0t2;
  if ((callee < 0)) {
    return 0;
  } else {
  }
  int _sv0t3 = sv0_vec_len(et);
  if ((callee >= _sv0t3)) {
    return 0;
  } else {
  }
  int _sv0t4 = sv0_vec_get(et, callee);
  if ((_sv0t4 != 1)) {
    return 0;
  } else {
  }
  int _sv0t5 = sv0_vec_get(ed2, callee);
  if ((_sv0t5 != 1)) {
    return 0;
  } else {
  }
  int _sv0t6 = sv0_vec_get(ed1, callee);
  int cpps = _sv0t6;
  if ((cpps < 0)) {
    return 0;
  } else {
  }
  int _sv0t7 = sv0_vec_len(pp);
  if ((cpps >= _sv0t7)) {
    return 0;
  } else {
  }
  int _sv0t8 = sv0_vec_get(pp, cpps);
  int ctok = _sv0t8;
  int _sv0t9 = lower_tok_in_bounds(ctok, starts, ends);
  if ((_sv0t9 != 1)) {
    return 0;
  } else {
  }
  const char* _sv0t10 = handle_to_str(ctok, source, starts, ends);
  const char* cname;
  cname = _sv0t10;
  int _sv0t11 = sv0_vec_len(item_tags);
  int n = _sv0t11;
  int _sv0t12 = sv0_vec_len(item_names);
  int nn = _sv0t12;
  int _sv0t13 = sv0_vec_len(item_d2);
  int nd = _sv0t13;
  int i = 0;
  while ((i < n)) {
    if ((i < nn)) {
      if ((i < nd)) {
        int _sv0t14 = sv0_vec_get(item_tags, i);
        if ((_sv0t14 == 0)) {
          int _sv0t15 = sv0_vec_get(item_names, i);
          int nm = _sv0t15;
          int _sv0t16 = lower_tok_in_bounds(nm, starts, ends);
          if (_sv0t16) {
            const char* _sv0t17 = handle_to_str(nm, source, starts, ends);
            const char* iname;
            iname = _sv0t17;
            int _sv0t18 = sv0_string_eq(iname, cname);
            if (_sv0t18) {
              int _sv0t19 = sv0_vec_get(item_d2, i);
              int packed = _sv0t19;
              int _sv0t20 = (packed % 2);
              if ((_sv0t20 == 1)) {
                int _sv0t21 = lower_scan_fn_ret_ty_tok(tok_tags, source, starts, ends, nm);
                int p3 = _sv0t21;
                int _sv0t22 = lower_tok_in_bounds(p3, starts, ends);
                if (_sv0t22) {
                  const char* _sv0t23 = handle_to_str(p3, source, starts, ends);
                  const char* rty;
                  rty = _sv0t23;
                  int _sv0t24 = sv0_string_eq(rty, "string");
                  if (_sv0t24) {
                    return 1;
                  } else {
                  }
                } else {
                }
              } else {
              }
              return 0;
            } else {
            }
          } else {
          }
        } else {
        }
      } else {
      }
    } else {
    }
    i = (i + 1);
  }
  return 0;
}

static int lower_init_ident_is_string_param(int et, int ed1, int ed2, int pp, int init_idx, int item_tags, int item_names, int item_d3, int tok_tags, const char* source, int starts, int ends, int item_fn_row) {
  if ((init_idx < 0)) {
    return 0;
  } else {
  }
  int _sv0t0 = sv0_vec_len(et);
  if ((init_idx >= _sv0t0)) {
    return 0;
  } else {
  }
  int _sv0t1 = sv0_vec_get(et, init_idx);
  if ((_sv0t1 != 1)) {
    return 0;
  } else {
  }
  int _sv0t2 = sv0_vec_get(ed2, init_idx);
  if ((_sv0t2 != 1)) {
    return 0;
  } else {
  }
  int _sv0t3 = sv0_vec_get(ed1, init_idx);
  int pps = _sv0t3;
  if ((pps < 0)) {
    return 0;
  } else {
  }
  int _sv0t4 = sv0_vec_len(pp);
  if ((pps >= _sv0t4)) {
    return 0;
  } else {
  }
  int _sv0t5 = sv0_vec_get(pp, pps);
  int itok = _sv0t5;
  int _sv0t6 = lower_tok_in_bounds(itok, starts, ends);
  if ((_sv0t6 != 1)) {
    return 0;
  } else {
  }
  if ((item_fn_row < 0)) {
    return 0;
  } else {
  }
  int _sv0t7 = sv0_vec_len(item_tags);
  if ((item_fn_row >= _sv0t7)) {
    return 0;
  } else {
  }
  int _sv0t8 = sv0_vec_len(item_names);
  if ((item_fn_row >= _sv0t8)) {
    return 0;
  } else {
  }
  int _sv0t9 = sv0_vec_len(item_d3);
  if ((item_fn_row >= _sv0t9)) {
    return 0;
  } else {
  }
  int _sv0t10 = sv0_vec_get(item_tags, item_fn_row);
  if ((_sv0t10 != 0)) {
    return 0;
  } else {
  }
  int _sv0t11 = sv0_vec_get(item_d3, item_fn_row);
  int pc = _sv0t11;
  if ((pc <= 0)) {
    return 0;
  } else {
  }
  int _sv0t12 = sv0_vec_get(item_names, item_fn_row);
  int name_tok = _sv0t12;
  if ((name_tok < 0)) {
    return 0;
  } else {
  }
  int _sv0t13 = (name_tok + 1);
  int _sv0t14 = sv0_vec_len(tok_tags);
  if ((_sv0t13 >= _sv0t14)) {
    return 0;
  } else {
  }
  int _sv0t15 = sv0_vec_new();
  int pnames = _sv0t15;
  int _sv0t16 = sv0_vec_new();
  int pty = _sv0t16;
  int _sv0t17 = lower_scan_item_fn_param_names_ty_toks(tok_tags, name_tok, pc, pnames, pty);
  int rs = _sv0t17;
  if ((rs < 0)) {
    return 0;
  } else {
  }
  int _sv0t18 = sv0_vec_len(pnames);
  if ((_sv0t18 != pc)) {
    return 0;
  } else {
  }
  int _sv0t19 = sv0_vec_len(pty);
  if ((_sv0t19 != pc)) {
    return 0;
  } else {
  }
  const char* _sv0t20 = handle_to_str(itok, source, starts, ends);
  const char* iname;
  iname = _sv0t20;
  int j = 0;
  while ((j < pc)) {
    int _sv0t21 = sv0_vec_get(pnames, j);
    int pn = _sv0t21;
    int _sv0t22 = sv0_vec_get(pty, j);
    int pt = _sv0t22;
    int _sv0t23 = lower_tok_in_bounds(pn, starts, ends);
    if (_sv0t23) {
      int _sv0t24 = lower_tok_in_bounds(pt, starts, ends);
      if (_sv0t24) {
        const char* _sv0t25 = handle_to_str(pn, source, starts, ends);
        int _sv0t26 = sv0_string_eq(_sv0t25, iname);
        if (_sv0t26) {
          const char* _sv0t27 = handle_to_str(pt, source, starts, ends);
          int _sv0t28 = sv0_string_eq(_sv0t27, "string");
          if (_sv0t28) {
            return 1;
          } else {
          }
          return 0;
        } else {
        }
      } else {
      }
    } else {
    }
    j = (j + 1);
  }
  return 0;
}

static int lower_ident_in_str_locals(int et, int ed1, int ed2, int pp, int init_idx, int str_locals, const char* source, int starts, int ends) {
  if ((init_idx < 0)) {
    return 0;
  } else {
  }
  int _sv0t0 = sv0_vec_len(et);
  if ((init_idx >= _sv0t0)) {
    return 0;
  } else {
  }
  int _sv0t1 = sv0_vec_get(et, init_idx);
  if ((_sv0t1 != 1)) {
    return 0;
  } else {
  }
  int _sv0t2 = sv0_vec_get(ed2, init_idx);
  if ((_sv0t2 != 1)) {
    return 0;
  } else {
  }
  int _sv0t3 = sv0_vec_get(ed1, init_idx);
  int pps = _sv0t3;
  if ((pps < 0)) {
    return 0;
  } else {
  }
  int _sv0t4 = sv0_vec_len(pp);
  if ((pps >= _sv0t4)) {
    return 0;
  } else {
  }
  int _sv0t5 = sv0_vec_get(pp, pps);
  int itok = _sv0t5;
  int _sv0t6 = lower_tok_in_bounds(itok, starts, ends);
  if ((_sv0t6 != 1)) {
    return 0;
  } else {
  }
  const char* _sv0t7 = handle_to_str(itok, source, starts, ends);
  const char* iname;
  iname = _sv0t7;
  int _sv0t8 = sv0_vec_len(str_locals);
  int n = _sv0t8;
  int i = 0;
  while ((i < n)) {
    int _sv0t9 = sv0_vec_get(str_locals, i);
    int s = _sv0t9;
    int _sv0t10 = lower_tok_in_bounds(s, starts, ends);
    if (_sv0t10) {
      const char* _sv0t11 = handle_to_str(s, source, starts, ends);
      int _sv0t12 = sv0_string_eq(_sv0t11, iname);
      if (_sv0t12) {
        return 1;
      } else {
      }
    } else {
    }
    i = (i + 1);
  }
  return 0;
}

static int lower_callee_is_void(int et, int ed1, int ed2, int pp, int call_idx, int item_tags, int item_names, int item_d2, int tok_tags, const char* source, int starts, int ends) {
  if ((call_idx < 0)) {
    return 0;
  } else {
  }
  int _sv0t0 = sv0_vec_len(et);
  if ((call_idx >= _sv0t0)) {
    return 0;
  } else {
  }
  int _sv0t1 = sv0_vec_get(et, call_idx);
  if ((_sv0t1 != 4)) {
    return 0;
  } else {
  }
  int _sv0t2 = sv0_vec_get(ed1, call_idx);
  int callee = _sv0t2;
  if ((callee < 0)) {
    return 0;
  } else {
  }
  int _sv0t3 = sv0_vec_len(et);
  if ((callee >= _sv0t3)) {
    return 0;
  } else {
  }
  int _sv0t4 = sv0_vec_get(et, callee);
  if ((_sv0t4 != 1)) {
    return 0;
  } else {
  }
  int _sv0t5 = sv0_vec_get(ed2, callee);
  if ((_sv0t5 != 1)) {
    return 0;
  } else {
  }
  int _sv0t6 = sv0_vec_get(ed1, callee);
  int cpps = _sv0t6;
  if ((cpps < 0)) {
    return 0;
  } else {
  }
  int _sv0t7 = sv0_vec_len(pp);
  if ((cpps >= _sv0t7)) {
    return 0;
  } else {
  }
  int _sv0t8 = sv0_vec_get(pp, cpps);
  int ctok = _sv0t8;
  int _sv0t9 = lower_tok_in_bounds(ctok, starts, ends);
  if ((_sv0t9 != 1)) {
    return 0;
  } else {
  }
  const char* _sv0t10 = handle_to_str(ctok, source, starts, ends);
  const char* cname;
  cname = _sv0t10;
  int _sv0t11 = sv0_vec_len(item_tags);
  int n = _sv0t11;
  int _sv0t12 = sv0_vec_len(item_names);
  int nn = _sv0t12;
  int _sv0t13 = sv0_vec_len(item_d2);
  int nd = _sv0t13;
  int i = 0;
  while ((i < n)) {
    if ((i < nn)) {
      if ((i < nd)) {
        int _sv0t14 = sv0_vec_get(item_tags, i);
        if ((_sv0t14 == 0)) {
          int _sv0t15 = sv0_vec_get(item_names, i);
          int nm = _sv0t15;
          int _sv0t16 = lower_tok_in_bounds(nm, starts, ends);
          if (_sv0t16) {
            const char* _sv0t17 = handle_to_str(nm, source, starts, ends);
            int _sv0t18 = sv0_string_eq(_sv0t17, cname);
            if (_sv0t18) {
              int _sv0t19 = sv0_vec_get(item_d2, i);
              int packed = _sv0t19;
              int _sv0t20 = (packed / 2);
              int _sv0t21 = (_sv0t20 * 2);
              int has_ret = (packed - _sv0t21);
              if ((has_ret == 0)) {
                return 1;
              } else {
              }
              int _sv0t22 = lower_scan_fn_ret_ty_tok(tok_tags, source, starts, ends, nm);
              int p3 = _sv0t22;
              int is_unit = 0;
              if ((p3 >= 0)) {
                int _sv0t23 = sv0_vec_len(tok_tags);
                if ((p3 < _sv0t23)) {
                  int _sv0t24 = (p3 + 1);
                  int _sv0t25 = sv0_vec_len(tok_tags);
                  if ((_sv0t24 < _sv0t25)) {
                    int _sv0t26 = sv0_vec_get(tok_tags, p3);
                    if ((_sv0t26 == 6)) {
                      int _sv0t27 = (p3 + 1);
                      int _sv0t28 = sv0_vec_get(tok_tags, _sv0t27);
                      if ((_sv0t28 == 7)) {
                        is_unit = 1;
                      } else {
                      }
                    } else {
                    }
                  } else {
                  }
                } else {
                }
              } else {
              }
              if ((is_unit == 1)) {
                return 1;
              } else {
              }
              return 0;
            } else {
            }
          } else {
          }
        } else {
        }
      } else {
      }
    } else {
    }
    i = (i + 1);
  }
  return 0;
}

static int lower_callee_ret_ty_head(int et, int ed1, int ed2, int pp, int call_idx, int item_tags, int item_names, int item_d2, int tok_tags, const char* source, int starts, int ends) {
  if ((call_idx < 0)) {
    int _sv0t0 = (0 - 1);
    return _sv0t0;
  } else {
  }
  int _sv0t1 = sv0_vec_len(et);
  if ((call_idx >= _sv0t1)) {
    int _sv0t2 = (0 - 1);
    return _sv0t2;
  } else {
  }
  int _sv0t3 = sv0_vec_get(et, call_idx);
  if ((_sv0t3 != 4)) {
    int _sv0t4 = (0 - 1);
    return _sv0t4;
  } else {
  }
  int _sv0t5 = sv0_vec_get(ed1, call_idx);
  int callee = _sv0t5;
  if ((callee < 0)) {
    int _sv0t6 = (0 - 1);
    return _sv0t6;
  } else {
  }
  int _sv0t7 = sv0_vec_len(et);
  if ((callee >= _sv0t7)) {
    int _sv0t8 = (0 - 1);
    return _sv0t8;
  } else {
  }
  int _sv0t9 = sv0_vec_get(et, callee);
  if ((_sv0t9 != 1)) {
    int _sv0t10 = (0 - 1);
    return _sv0t10;
  } else {
  }
  int _sv0t11 = sv0_vec_get(ed2, callee);
  if ((_sv0t11 != 1)) {
    int _sv0t12 = (0 - 1);
    return _sv0t12;
  } else {
  }
  int _sv0t13 = sv0_vec_get(ed1, callee);
  int cpps = _sv0t13;
  if ((cpps < 0)) {
    int _sv0t14 = (0 - 1);
    return _sv0t14;
  } else {
  }
  int _sv0t15 = sv0_vec_len(pp);
  if ((cpps >= _sv0t15)) {
    int _sv0t16 = (0 - 1);
    return _sv0t16;
  } else {
  }
  int _sv0t17 = sv0_vec_get(pp, cpps);
  int ctok = _sv0t17;
  int _sv0t18 = lower_tok_in_bounds(ctok, starts, ends);
  if ((_sv0t18 != 1)) {
    int _sv0t19 = (0 - 1);
    return _sv0t19;
  } else {
  }
  const char* _sv0t20 = handle_to_str(ctok, source, starts, ends);
  const char* cname;
  cname = _sv0t20;
  int _sv0t21 = sv0_vec_len(item_tags);
  int n = _sv0t21;
  int _sv0t22 = sv0_vec_len(item_names);
  int nn = _sv0t22;
  int _sv0t23 = sv0_vec_len(item_d2);
  int nd = _sv0t23;
  int i = 0;
  while ((i < n)) {
    if ((i < nn)) {
      if ((i < nd)) {
        int _sv0t24 = sv0_vec_get(item_tags, i);
        if ((_sv0t24 == 0)) {
          int _sv0t25 = sv0_vec_get(item_names, i);
          int nm = _sv0t25;
          int _sv0t26 = lower_tok_in_bounds(nm, starts, ends);
          if (_sv0t26) {
            const char* _sv0t27 = handle_to_str(nm, source, starts, ends);
            int _sv0t28 = sv0_string_eq(_sv0t27, cname);
            if (_sv0t28) {
              int _sv0t29 = sv0_vec_get(item_d2, i);
              int packed = _sv0t29;
              int _sv0t30 = (packed % 2);
              if ((_sv0t30 == 1)) {
                int _sv0t31 = lower_scan_fn_ret_ty_tok(tok_tags, source, starts, ends, nm);
                int p3 = _sv0t31;
                int _sv0t32 = lower_tok_in_bounds(p3, starts, ends);
                if (_sv0t32) {
                  return p3;
                } else {
                }
              } else {
              }
              int _sv0t33 = (0 - 1);
              return _sv0t33;
            } else {
            }
          } else {
          }
        } else {
        }
      } else {
      }
    } else {
    }
    i = (i + 1);
  }
  int _sv0t34 = (0 - 1);
  return _sv0t34;
}

static int lower_name_is_user_type(const char* name, int item_tags, int item_names, const char* source, int starts, int ends) {
  int _sv0t0 = sv0_vec_len(item_tags);
  int n = _sv0t0;
  int _sv0t1 = sv0_vec_len(item_names);
  int nn = _sv0t1;
  int i = 0;
  while ((i < n)) {
    if ((i < nn)) {
      int _sv0t2 = sv0_vec_get(item_tags, i);
      int t = _sv0t2;
      int is_ty = 0;
      if ((t == 1)) {
        is_ty = 1;
      } else {
      }
      if ((t == 2)) {
        is_ty = 1;
      } else {
      }
      if ((is_ty == 1)) {
        int _sv0t3 = sv0_vec_get(item_names, i);
        int nm = _sv0t3;
        int _sv0t4 = lower_tok_in_bounds(nm, starts, ends);
        if (_sv0t4) {
          const char* _sv0t5 = handle_to_str(nm, source, starts, ends);
          int _sv0t6 = sv0_string_eq(_sv0t5, name);
          if (_sv0t6) {
            return 1;
          } else {
          }
        } else {
        }
      } else {
      }
    } else {
    }
    i = (i + 1);
  }
  return 0;
}

static int lower_call_ret_user_ty_tok(int et, int ed1, int ed2, int pp, int call_idx, int item_tags, int item_names, int item_d2, int tok_tags, const char* source, int starts, int ends) {
  int _sv0t0 = lower_callee_ret_ty_head(et, ed1, ed2, pp, call_idx, item_tags, item_names, item_d2, tok_tags, source, starts, ends);
  int head = _sv0t0;
  if ((head < 0)) {
    int _sv0t1 = (0 - 1);
    return _sv0t1;
  } else {
  }
  const char* _sv0t2 = handle_to_str(head, source, starts, ends);
  const char* rty;
  rty = _sv0t2;
  int _sv0t3 = lower_name_is_user_type(rty, item_tags, item_names, source, starts, ends);
  if (_sv0t3) {
    return head;
  } else {
  }
  int _sv0t4 = (0 - 1);
  return _sv0t4;
}

static int lower_typed_box_deref_let(int et, int ed1, int ed2, int ed3, int ed4, int pp, int tok_tags, int init_idx, int name_tok, int ann_tok, int ctr, int out_instrs, int enum_names, int enum_tag_offsets, int enum_tag_counts, int enum_tags_flat, int fn_ctx, int builtin_map, int lit_sf_names, const char* source, int starts, int ends, int item_tags, int item_names, int item_d2, int item_d3, int item_d4, int item_field_counts, int enum_vnames_flat, int enum_max_payloads, int item_fn_row, int match_env_inherit, int import_aliases) {
  if ((ann_tok < 0)) {
    return 0;
  } else {
  }
  int _sv0t0 = lower_tok_in_bounds(ann_tok, starts, ends);
  if ((_sv0t0 != 1)) {
    return 0;
  } else {
  }
  const char* _sv0t1 = handle_to_str(ann_tok, source, starts, ends);
  const char* ann_name;
  ann_name = _sv0t1;
  int _sv0t2 = lower_name_is_user_type(ann_name, item_tags, item_names, source, starts, ends);
  if ((_sv0t2 != 1)) {
    return 0;
  } else {
  }
  if ((init_idx < 0)) {
    return 0;
  } else {
  }
  int _sv0t3 = sv0_vec_len(et);
  if ((init_idx >= _sv0t3)) {
    return 0;
  } else {
  }
  int _sv0t4 = sv0_vec_get(et, init_idx);
  if ((_sv0t4 != 4)) {
    return 0;
  } else {
  }
  int _sv0t5 = sv0_vec_get(ed1, init_idx);
  int callee = _sv0t5;
  if ((callee < 0)) {
    return 0;
  } else {
  }
  int _sv0t6 = sv0_vec_len(et);
  if ((callee >= _sv0t6)) {
    return 0;
  } else {
  }
  int _sv0t7 = sv0_vec_get(et, callee);
  if ((_sv0t7 != 1)) {
    return 0;
  } else {
  }
  int _sv0t8 = sv0_vec_get(ed2, callee);
  if ((_sv0t8 != 1)) {
    return 0;
  } else {
  }
  int _sv0t9 = sv0_vec_get(ed1, callee);
  int cpps = _sv0t9;
  if ((cpps < 0)) {
    return 0;
  } else {
  }
  int _sv0t10 = sv0_vec_len(pp);
  if ((cpps >= _sv0t10)) {
    return 0;
  } else {
  }
  int _sv0t11 = sv0_vec_get(pp, cpps);
  int ctok = _sv0t11;
  int _sv0t12 = lower_tok_in_bounds(ctok, starts, ends);
  if ((_sv0t12 != 1)) {
    return 0;
  } else {
  }
  const char* _sv0t13 = handle_to_str(ctok, source, starts, ends);
  int _sv0t14 = sv0_string_eq(_sv0t13, "box_deref");
  if ((_sv0t14 != 1)) {
    return 0;
  } else {
  }
  int _sv0t15 = sv0_vec_get(ed2, init_idx);
  int args_first = _sv0t15;
  int _sv0t16 = sv0_vec_get(ed3, init_idx);
  int args_count = _sv0t16;
  if ((args_count < 1)) {
    return 0;
  } else {
  }
  int _sv0t17 = sv0_vec_get(ed4, init_idx);
  int args_sidecar = _sv0t17;
  int _sv0t18 = block_stmt_index(pp, args_first, args_sidecar, 0);
  int arg_idx = _sv0t18;
  if ((arg_idx < 0)) {
    return 0;
  } else {
  }
  int _sv0t19 = sv0_vec_len(et);
  if ((arg_idx >= _sv0t19)) {
    return 0;
  } else {
  }
  Value _sv0t20 = lower_expr_to_value(et, ed1, ed2, ed3, ed4, pp, tok_tags, arg_idx, ctr, out_instrs, enum_names, enum_tag_offsets, enum_tag_counts, enum_tags_flat, fn_ctx, builtin_map, lit_sf_names, source, starts, ends, item_tags, item_names, item_d2, item_d3, item_d4, item_field_counts, enum_vnames_flat, enum_max_payloads, item_fn_row, match_env_inherit, import_aliases);
  Value av;
  av = _sv0t20;
  int box_h = (0 - 1);
  int _sv0t21;
  if ((av.tag == 3)) {
    int h = av.p0;
    box_h = h;
    _sv0t21 = 0;
  } else {
    if ((av.tag == 0)) {
      int n = av.p0;
      _sv0t21 = 0;
    } else {
      if ((av.tag == 1)) {
        _sv0t21 = 0;
      } else {
        if ((av.tag == 2)) {
          _sv0t21 = 0;
        } else {
          if ((av.tag == 5)) {
            int s = av.p0;
            _sv0t21 = 0;
          } else {
            if ((av.tag == 4)) {
              _sv0t21 = 0;
            } else {
              if ((av.tag == 6)) {
                int bv = av.p0;
                int f = av.p1;
                _sv0t21 = 0;
              } else {
                if ((av.tag == 7)) {
                  int h2 = av.p0;
                  _sv0t21 = 0;
                } else {
                }
              }
            }
          }
        }
      }
    }
  }
  int is_enum_bd = 0;
  int maxp_bd = 0;
  int _sv0t22 = sv0_vec_len(enum_names);
  int ne_bd = _sv0t22;
  int ei_bd = 0;
  while ((ei_bd < ne_bd)) {
    int _sv0t23 = sv0_vec_get(enum_names, ei_bd);
    int enh_bd = _sv0t23;
    int _sv0t24 = lower_tok_in_bounds(enh_bd, starts, ends);
    if (_sv0t24) {
      const char* _sv0t25 = handle_to_str(enh_bd, source, starts, ends);
      int _sv0t26 = sv0_string_eq(_sv0t25, ann_name);
      if (_sv0t26) {
        is_enum_bd = 1;
        int _sv0t27 = sv0_vec_len(enum_max_payloads);
        if ((ei_bd < _sv0t27)) {
          int _sv0t28 = sv0_vec_get(enum_max_payloads, ei_bd);
          maxp_bd = _sv0t28;
        } else {
        }
        ei_bd = ne_bd;
      } else {
      }
    } else {
    }
    ei_bd = (ei_bd + 1);
  }
  int _sv0t29 = lower_vm_target();
  if ((_sv0t29 == 1)) {
    if ((is_enum_bd == 1)) {
      if ((box_h >= 0)) {
        int _sv0t30 = ctr_fresh(ctr);
        int out_bd = _sv0t30;
        Instr _sv0t31;
        _sv0t31.tag = 2;
        _sv0t31.p0 = ann_tok;
        _sv0t31.p1 = out_bd;
        int _sv0t32 = sv0_box_alloc(5);
        sv0_box_store(_sv0t32, 0, _sv0t31.tag);
        sv0_box_store(_sv0t32, 1, _sv0t31.p0);
        sv0_box_store(_sv0t32, 2, _sv0t31.p1);
        sv0_box_store(_sv0t32, 3, _sv0t31.p2);
        sv0_box_store(_sv0t32, 4, _sv0t31.p3);
        sv0_vec_push(out_instrs, _sv0t32);
        int _sv0t33 = ctr_fresh(ctr);
        int t_tag = _sv0t33;
        int _sv0t34 = sv0_vec_new();
        int a_tag = _sv0t34;
        Value _sv0t35;
        _sv0t35.tag = 3;
        _sv0t35.p0 = box_h;
        int _sv0t36 = sv0_box_alloc(3);
        sv0_box_store(_sv0t36, 0, _sv0t35.tag);
        sv0_box_store(_sv0t36, 1, _sv0t35.p0);
        sv0_box_store(_sv0t36, 2, _sv0t35.p1);
        sv0_vec_push(a_tag, _sv0t36);
        Value _sv0t37;
        _sv0t37.tag = 0;
        _sv0t37.p0 = 0;
        int _sv0t38 = sv0_box_alloc(3);
        sv0_box_store(_sv0t38, 0, _sv0t37.tag);
        sv0_box_store(_sv0t38, 1, _sv0t37.p0);
        sv0_box_store(_sv0t38, 2, _sv0t37.p1);
        sv0_vec_push(a_tag, _sv0t38);
        Instr _sv0t39;
        int _sv0t40 = (0 - 21);
        _sv0t39.tag = 11;
        _sv0t39.p0 = t_tag;
        _sv0t39.p1 = _sv0t40;
        _sv0t39.p2 = a_tag;
        _sv0t39.p3 = 0;
        int _sv0t41 = sv0_box_alloc(5);
        sv0_box_store(_sv0t41, 0, _sv0t39.tag);
        sv0_box_store(_sv0t41, 1, _sv0t39.p0);
        sv0_box_store(_sv0t41, 2, _sv0t39.p1);
        sv0_box_store(_sv0t41, 3, _sv0t39.p2);
        sv0_box_store(_sv0t41, 4, _sv0t39.p3);
        sv0_vec_push(out_instrs, _sv0t41);
        Instr _sv0t42;
        int _sv0t43 = (0 - 1);
        Value _sv0t44;
        _sv0t44.tag = 3;
        _sv0t44.p0 = t_tag;
        Expr _sv0t45 = value_to_expr(_sv0t44);
        int _sv0t46 = sv0_box_alloc(4);
        sv0_box_store(_sv0t46, 0, _sv0t45.tag);
        sv0_box_store(_sv0t46, 1, _sv0t45.p0);
        sv0_box_store(_sv0t46, 2, _sv0t45.p1);
        sv0_box_store(_sv0t46, 3, _sv0t45.p2);
        _sv0t42.tag = 5;
        _sv0t42.p0 = out_bd;
        _sv0t42.p1 = _sv0t43;
        _sv0t42.p2 = _sv0t46;
        int _sv0t47 = sv0_box_alloc(5);
        sv0_box_store(_sv0t47, 0, _sv0t42.tag);
        sv0_box_store(_sv0t47, 1, _sv0t42.p0);
        sv0_box_store(_sv0t47, 2, _sv0t42.p1);
        sv0_box_store(_sv0t47, 3, _sv0t42.p2);
        sv0_box_store(_sv0t47, 4, _sv0t42.p3);
        sv0_vec_push(out_instrs, _sv0t47);
        int pi_bd = 0;
        while ((pi_bd < maxp_bd)) {
          int _sv0t48 = ctr_fresh(ctr);
          int t_p = _sv0t48;
          int _sv0t49 = sv0_vec_new();
          int a_p = _sv0t49;
          Value _sv0t50;
          _sv0t50.tag = 3;
          _sv0t50.p0 = box_h;
          int _sv0t51 = sv0_box_alloc(3);
          sv0_box_store(_sv0t51, 0, _sv0t50.tag);
          sv0_box_store(_sv0t51, 1, _sv0t50.p0);
          sv0_box_store(_sv0t51, 2, _sv0t50.p1);
          sv0_vec_push(a_p, _sv0t51);
          Value _sv0t52;
          int _sv0t53 = (pi_bd + 1);
          _sv0t52.tag = 0;
          _sv0t52.p0 = _sv0t53;
          int _sv0t54 = sv0_box_alloc(3);
          sv0_box_store(_sv0t54, 0, _sv0t52.tag);
          sv0_box_store(_sv0t54, 1, _sv0t52.p0);
          sv0_box_store(_sv0t54, 2, _sv0t52.p1);
          sv0_vec_push(a_p, _sv0t54);
          Instr _sv0t55;
          int _sv0t56 = (0 - 21);
          _sv0t55.tag = 11;
          _sv0t55.p0 = t_p;
          _sv0t55.p1 = _sv0t56;
          _sv0t55.p2 = a_p;
          _sv0t55.p3 = 0;
          int _sv0t57 = sv0_box_alloc(5);
          sv0_box_store(_sv0t57, 0, _sv0t55.tag);
          sv0_box_store(_sv0t57, 1, _sv0t55.p0);
          sv0_box_store(_sv0t57, 2, _sv0t55.p1);
          sv0_box_store(_sv0t57, 3, _sv0t55.p2);
          sv0_box_store(_sv0t57, 4, _sv0t55.p3);
          sv0_vec_push(out_instrs, _sv0t57);
          Instr _sv0t58;
          int _sv0t59 = (0 - 10);
          int _sv0t60 = (_sv0t59 - pi_bd);
          Value _sv0t61;
          _sv0t61.tag = 3;
          _sv0t61.p0 = t_p;
          Expr _sv0t62 = value_to_expr(_sv0t61);
          int _sv0t63 = sv0_box_alloc(4);
          sv0_box_store(_sv0t63, 0, _sv0t62.tag);
          sv0_box_store(_sv0t63, 1, _sv0t62.p0);
          sv0_box_store(_sv0t63, 2, _sv0t62.p1);
          sv0_box_store(_sv0t63, 3, _sv0t62.p2);
          _sv0t58.tag = 5;
          _sv0t58.p0 = out_bd;
          _sv0t58.p1 = _sv0t60;
          _sv0t58.p2 = _sv0t63;
          int _sv0t64 = sv0_box_alloc(5);
          sv0_box_store(_sv0t64, 0, _sv0t58.tag);
          sv0_box_store(_sv0t64, 1, _sv0t58.p0);
          sv0_box_store(_sv0t64, 2, _sv0t58.p1);
          sv0_box_store(_sv0t64, 3, _sv0t58.p2);
          sv0_box_store(_sv0t64, 4, _sv0t58.p3);
          sv0_vec_push(out_instrs, _sv0t64);
          pi_bd = (pi_bd + 1);
        }
        Instr _sv0t65;
        _sv0t65.tag = 2;
        _sv0t65.p0 = ann_tok;
        _sv0t65.p1 = name_tok;
        int _sv0t66 = sv0_box_alloc(5);
        sv0_box_store(_sv0t66, 0, _sv0t65.tag);
        sv0_box_store(_sv0t66, 1, _sv0t65.p0);
        sv0_box_store(_sv0t66, 2, _sv0t65.p1);
        sv0_box_store(_sv0t66, 3, _sv0t65.p2);
        sv0_box_store(_sv0t66, 4, _sv0t65.p3);
        sv0_vec_push(out_instrs, _sv0t66);
        Instr _sv0t67;
        int _sv0t68 = (0 - 1);
        Value _sv0t69;
        Value _sv0t70;
        _sv0t70.tag = 3;
        _sv0t70.p0 = out_bd;
        int _sv0t71 = sv0_box_alloc(3);
        sv0_box_store(_sv0t71, 0, _sv0t70.tag);
        sv0_box_store(_sv0t71, 1, _sv0t70.p0);
        sv0_box_store(_sv0t71, 2, _sv0t70.p1);
        int _sv0t72 = (0 - 1);
        _sv0t69.tag = 6;
        _sv0t69.p0 = _sv0t71;
        _sv0t69.p1 = _sv0t72;
        Expr _sv0t73 = value_to_expr(_sv0t69);
        int _sv0t74 = sv0_box_alloc(4);
        sv0_box_store(_sv0t74, 0, _sv0t73.tag);
        sv0_box_store(_sv0t74, 1, _sv0t73.p0);
        sv0_box_store(_sv0t74, 2, _sv0t73.p1);
        sv0_box_store(_sv0t74, 3, _sv0t73.p2);
        _sv0t67.tag = 5;
        _sv0t67.p0 = name_tok;
        _sv0t67.p1 = _sv0t68;
        _sv0t67.p2 = _sv0t74;
        int _sv0t75 = sv0_box_alloc(5);
        sv0_box_store(_sv0t75, 0, _sv0t67.tag);
        sv0_box_store(_sv0t75, 1, _sv0t67.p0);
        sv0_box_store(_sv0t75, 2, _sv0t67.p1);
        sv0_box_store(_sv0t75, 3, _sv0t67.p2);
        sv0_box_store(_sv0t75, 4, _sv0t67.p3);
        sv0_vec_push(out_instrs, _sv0t75);
        int ci_bd = 0;
        while ((ci_bd < maxp_bd)) {
          Instr _sv0t76;
          int _sv0t77 = (0 - 10);
          int _sv0t78 = (_sv0t77 - ci_bd);
          Value _sv0t79;
          Value _sv0t80;
          _sv0t80.tag = 3;
          _sv0t80.p0 = out_bd;
          int _sv0t81 = sv0_box_alloc(3);
          sv0_box_store(_sv0t81, 0, _sv0t80.tag);
          sv0_box_store(_sv0t81, 1, _sv0t80.p0);
          sv0_box_store(_sv0t81, 2, _sv0t80.p1);
          int _sv0t82 = (0 - 10);
          int _sv0t83 = (_sv0t82 - ci_bd);
          _sv0t79.tag = 6;
          _sv0t79.p0 = _sv0t81;
          _sv0t79.p1 = _sv0t83;
          Expr _sv0t84 = value_to_expr(_sv0t79);
          int _sv0t85 = sv0_box_alloc(4);
          sv0_box_store(_sv0t85, 0, _sv0t84.tag);
          sv0_box_store(_sv0t85, 1, _sv0t84.p0);
          sv0_box_store(_sv0t85, 2, _sv0t84.p1);
          sv0_box_store(_sv0t85, 3, _sv0t84.p2);
          _sv0t76.tag = 5;
          _sv0t76.p0 = name_tok;
          _sv0t76.p1 = _sv0t78;
          _sv0t76.p2 = _sv0t85;
          int _sv0t86 = sv0_box_alloc(5);
          sv0_box_store(_sv0t86, 0, _sv0t76.tag);
          sv0_box_store(_sv0t86, 1, _sv0t76.p0);
          sv0_box_store(_sv0t86, 2, _sv0t76.p1);
          sv0_box_store(_sv0t86, 3, _sv0t76.p2);
          sv0_box_store(_sv0t86, 4, _sv0t76.p3);
          sv0_vec_push(out_instrs, _sv0t86);
          ci_bd = (ci_bd + 1);
        }
        return 1;
      } else {
      }
    } else {
    }
  } else {
  }
  int _sv0t87 = sv0_vec_new();
  int args = _sv0t87;
  Value _sv0t88;
  _sv0t88.tag = 3;
  _sv0t88.p0 = box_h;
  int _sv0t89 = sv0_box_alloc(3);
  sv0_box_store(_sv0t89, 0, _sv0t88.tag);
  sv0_box_store(_sv0t89, 1, _sv0t88.p0);
  sv0_box_store(_sv0t89, 2, _sv0t88.p1);
  sv0_vec_push(args, _sv0t89);
  Instr call;
  int _sv0t90 = (0 - 13);
  call.tag = 11;
  call.p0 = name_tok;
  call.p1 = _sv0t90;
  call.p2 = args;
  call.p3 = ann_tok;
  int _sv0t91 = sv0_box_alloc(5);
  sv0_box_store(_sv0t91, 0, call.tag);
  sv0_box_store(_sv0t91, 1, call.p0);
  sv0_box_store(_sv0t91, 2, call.p1);
  sv0_box_store(_sv0t91, 3, call.p2);
  sv0_box_store(_sv0t91, 4, call.p3);
  sv0_vec_push(out_instrs, _sv0t91);
  return 1;
}

static int lower_vm_target(void) {
  return 0;
}

static int lower_init_is_string(int et, int ed1, int ed2, int pp, int init_idx, const char* source, int starts, int ends) {
  int _sv0t0 = sv0_vec_get(et, init_idx);
  int itag = _sv0t0;
  if ((itag == 0)) {
    int _sv0t1 = sv0_vec_get(ed1, init_idx);
    if ((_sv0t1 == 3)) {
      return 1;
    } else {
    }
    return 0;
  } else {
  }
  if ((itag == 4)) {
    int _sv0t2 = sv0_vec_get(ed1, init_idx);
    int callee = _sv0t2;
    int _sv0t3 = sv0_vec_get(et, callee);
    if ((_sv0t3 == 1)) {
      int _sv0t4 = sv0_vec_get(ed2, callee);
      if ((_sv0t4 == 1)) {
        int _sv0t5 = sv0_vec_get(ed1, callee);
        int _sv0t6 = sv0_vec_get(pp, _sv0t5);
        int nt = _sv0t6;
        const char* _sv0t7 = handle_to_str(nt, source, starts, ends);
        const char* nm;
        nm = _sv0t7;
        int _sv0t8 = sv0_string_eq(nm, "string_concat");
        if (_sv0t8) {
          return 1;
        } else {
        }
        int _sv0t9 = sv0_string_eq(nm, "string_substr");
        if (_sv0t9) {
          return 1;
        } else {
        }
        int _sv0t10 = sv0_string_eq(nm, "read_file");
        if (_sv0t10) {
          return 1;
        } else {
        }
        int _sv0t11 = sv0_string_eq(nm, "read_dir");
        if (_sv0t11) {
          return 1;
        } else {
        }
      } else {
      }
    } else {
    }
  } else {
  }
  return 0;
}

static int lower_expr_yields_string(int et, int ed1, int ed2, int ed3, int pp, int idx, int item_tags, int item_names, int item_d2, int tok_tags, const char* source, int starts, int ends) {
  if ((idx < 0)) {
    return 0;
  } else {
  }
  int _sv0t0 = sv0_vec_len(et);
  if ((idx >= _sv0t0)) {
    return 0;
  } else {
  }
  int _sv0t1 = lower_init_is_string(et, ed1, ed2, pp, idx, source, starts, ends);
  if (_sv0t1) {
    return 1;
  } else {
  }
  int _sv0t2 = lower_call_ret_is_string(et, ed1, ed2, pp, idx, item_tags, item_names, item_d2, tok_tags, source, starts, ends);
  if (_sv0t2) {
    return 1;
  } else {
  }
  int _sv0t3 = sv0_vec_get(et, idx);
  int tag = _sv0t3;
  if ((tag == 9)) {
    int _sv0t4 = sv0_vec_get(ed3, idx);
    int _sv0t5 = lower_expr_yields_string(et, ed1, ed2, ed3, pp, _sv0t4, item_tags, item_names, item_d2, tok_tags, source, starts, ends);
    return _sv0t5;
  } else {
  }
  if ((tag == 10)) {
    int _sv0t6 = sv0_vec_get(ed2, idx);
    int _sv0t7 = lower_expr_yields_string(et, ed1, ed2, ed3, pp, _sv0t6, item_tags, item_names, item_d2, tok_tags, source, starts, ends);
    return _sv0t7;
  } else {
  }
  return 0;
}

static int fresh_tmp_name(int counter) {
  return counter;
}

static int next_tmp(int counter) {
  int _sv0t0 = (counter + 1);
  return _sv0t0;
}

static const char* lower_digit(int d) {
  if ((d == 0)) {
    return "0";
  } else {
  }
  if ((d == 1)) {
    return "1";
  } else {
  }
  if ((d == 2)) {
    return "2";
  } else {
  }
  if ((d == 3)) {
    return "3";
  } else {
  }
  if ((d == 4)) {
    return "4";
  } else {
  }
  if ((d == 5)) {
    return "5";
  } else {
  }
  if ((d == 6)) {
    return "6";
  } else {
  }
  if ((d == 7)) {
    return "7";
  } else {
  }
  if ((d == 8)) {
    return "8";
  } else {
  }
  return "9";
}

static const char* lower_int_to_str(int n) {
  if ((n < 0)) {
    int _sv0t0 = (0 - n);
    const char* _sv0t1 = lower_int_to_str(_sv0t0);
    const char* _sv0t2 = sv0_string_concat("-", _sv0t1);
    return _sv0t2;
  } else {
  }
  if ((n < 10)) {
    const char* _sv0t3 = lower_digit(n);
    return _sv0t3;
  } else {
  }
  int _sv0t4 = (n / 10);
  const char* _sv0t5 = lower_int_to_str(_sv0t4);
  const char* rest;
  rest = _sv0t5;
  int _sv0t6 = (n / 10);
  int _sv0t7 = (_sv0t6 * 10);
  int _sv0t8 = (n - _sv0t7);
  const char* _sv0t9 = lower_digit(_sv0t8);
  const char* last;
  last = _sv0t9;
  const char* _sv0t10 = sv0_string_concat(rest, last);
  return _sv0t10;
}

static const char* fresh_tmp_str(int counter) {
  const char* _sv0t0 = lower_int_to_str(counter);
  const char* _sv0t1 = sv0_string_concat("_sv0t", _sv0t0);
  return _sv0t1;
}

static int lower_alias_new(void) {
  int _sv0t0 = sv0_vec_new();
  return _sv0t0;
}

static int lower_alias_add(int aliases, int from_h, int to_h) {
  sv0_vec_push(aliases, from_h);
  sv0_vec_push(aliases, to_h);
  int _sv0t0 = sv0_vec_len(aliases);
  int _sv0t1 = (_sv0t0 / 2);
  return _sv0t1;
}

static int lower_alias_lookup(int aliases, int name_h) {
  int _sv0t0 = sv0_vec_len(aliases);
  int n = _sv0t0;
  int i = 0;
  while ((i < n)) {
    int _sv0t1 = sv0_vec_get(aliases, i);
    if ((_sv0t1 == name_h)) {
      int _sv0t2 = (i + 1);
      int _sv0t3 = sv0_vec_get(aliases, _sv0t2);
      return _sv0t3;
    } else {
    }
    i = (i + 2);
  }
  int _sv0t4 = (0 - 1);
  return _sv0t4;
}

static int resolve_fn_callee(int aliases, int name_h) {
  int _sv0t0 = lower_alias_lookup(aliases, name_h);
  int target = _sv0t0;
  if ((target < 0)) {
    return name_h;
  } else {
  }
  return target;
}

static int lower_find_item_fn_name_by_text(int item_tags, int item_names, const char* source, int starts, int ends, const char* target_name) {
  int _sv0t0 = sv0_vec_len(item_tags);
  int n = _sv0t0;
  int i = 0;
  while ((i < n)) {
    int _sv0t1 = sv0_vec_get(item_tags, i);
    if ((_sv0t1 == 0)) {
      int _sv0t2 = sv0_vec_get(item_names, i);
      int name_h = _sv0t2;
      const char* _sv0t3 = handle_to_str(name_h, source, starts, ends);
      const char* name_s;
      name_s = _sv0t3;
      int _sv0t4 = sv0_string_eq(name_s, target_name);
      if (_sv0t4) {
        return name_h;
      } else {
      }
    } else {
    }
    i = (i + 1);
  }
  int _sv0t5 = (0 - 1);
  return _sv0t5;
}

static int lower_fill_import_aliases_from_use_items(int aliases, int item_tags, int item_names, int item_d2, int item_d3, int pp, const char* source, int starts, int ends) {
  int _sv0t0 = sv0_vec_len(item_tags);
  int n = _sv0t0;
  int i = 0;
  while ((i < n)) {
    int _sv0t1 = sv0_vec_get(item_tags, i);
    if ((_sv0t1 == 5)) {
      int _sv0t2 = sv0_vec_get(item_d2, i);
      int pps = _sv0t2;
      int _sv0t3 = sv0_vec_get(item_d3, i);
      int ppc = _sv0t3;
      if ((ppc == 2)) {
        int _sv0t4 = sv0_vec_get(pp, pps);
        int mod_h = _sv0t4;
        int _sv0t5 = (pps + 1);
        int _sv0t6 = sv0_vec_get(pp, _sv0t5);
        int short_h = _sv0t6;
        const char* _sv0t7 = handle_to_str(mod_h, source, starts, ends);
        const char* mod_s;
        mod_s = _sv0t7;
        const char* _sv0t8 = handle_to_str(short_h, source, starts, ends);
        const char* short_s;
        short_s = _sv0t8;
        const char* _sv0t9 = sv0_string_concat(mod_s, "__");
        const char* target;
        target = _sv0t9;
        const char* _sv0t10 = sv0_string_concat(target, short_s);
        target = _sv0t10;
        int _sv0t11 = lower_find_item_fn_name_by_text(item_tags, item_names, source, starts, ends, target);
        int to_h = _sv0t11;
        if ((to_h >= 0)) {
          int _sv0t12 = lower_alias_lookup(aliases, short_h);
          if ((_sv0t12 < 0)) {
            int _sv0t13 = lower_alias_add(aliases, short_h, to_h);
            int discard_add = _sv0t13;
          } else {
          }
        } else {
        }
      } else {
      }
    } else {
    }
    i = (i + 1);
  }
  int _sv0t14 = sv0_vec_len(aliases);
  int _sv0t15 = (_sv0t14 / 2);
  return _sv0t15;
}

static int resolve_enum_ctor_path(int ctor_from_enum, int ctor_from_var, int ctor_to_enum, int ctor_to_var, int enum_h, int variant_h) {
  int _sv0t0 = sv0_vec_len(ctor_from_enum);
  int n = _sv0t0;
  int i = 0;
  while ((i < n)) {
    int _sv0t1 = sv0_vec_get(ctor_from_enum, i);
    if ((_sv0t1 == enum_h)) {
      int _sv0t2 = sv0_vec_get(ctor_from_var, i);
      if ((_sv0t2 == variant_h)) {
        return i;
      } else {
      }
    } else {
    }
    i = (i + 1);
  }
  int _sv0t3 = (0 - 1);
  return _sv0t3;
}

static int resolved_ctor_enum(int ctor_to_enum, int idx, int default_enum) {
  if ((idx < 0)) {
    return default_enum;
  } else {
  }
  int _sv0t0 = sv0_vec_get(ctor_to_enum, idx);
  return _sv0t0;
}

static int resolved_ctor_variant(int ctor_to_var, int idx, int default_var) {
  if ((idx < 0)) {
    return default_var;
  } else {
  }
  int _sv0t0 = sv0_vec_get(ctor_to_var, idx);
  return _sv0t0;
}

static int find_double_colon(const char* s, int start) {
  int _sv0t0 = sv0_string_len(s);
  int len = _sv0t0;
  int limit = (len - 1);
  int i = start;
  while ((i < limit)) {
    int _sv0t1 = sv0_string_char_at(s, i);
    if ((_sv0t1 == 58)) {
      int _sv0t2 = (i + 1);
      int _sv0t3 = sv0_string_char_at(s, _sv0t2);
      if ((_sv0t3 == 58)) {
        return i;
      } else {
      }
    } else {
    }
    i = (i + 1);
  }
  int _sv0t4 = (0 - 1);
  return _sv0t4;
}

static int split_qname_count(const char* s) {
  int count = 1;
  int pos = 0;
  int _sv0t0 = find_double_colon(s, pos);
  int idx = _sv0t0;
  if ((idx < 0)) {
    return 1;
  } else {
  }
  count = (count + 1);
  pos = (idx + 2);
  int _sv0t1 = find_double_colon(s, pos);
  int idx2 = _sv0t1;
  if ((idx2 < 0)) {
    return count;
  } else {
  }
  count = (count + 1);
  pos = (idx2 + 2);
  int _sv0t2 = find_double_colon(s, pos);
  int idx3 = _sv0t2;
  if ((idx3 < 0)) {
    return count;
  } else {
  }
  int _sv0t3 = (count + 1);
  return _sv0t3;
}

static int has_qualified_name(const char* s) {
  int _sv0t0 = find_double_colon(s, 0);
  int _sv0t1 = (_sv0t0 >= 0);
  return _sv0t1;
}

static const char* ast_binop_to_c(int tag) {
  if ((tag == 0)) {
    return "+";
  } else {
  }
  if ((tag == 1)) {
    return "-";
  } else {
  }
  if ((tag == 2)) {
    return "*";
  } else {
  }
  if ((tag == 3)) {
    return "/";
  } else {
  }
  if ((tag == 4)) {
    return "%";
  } else {
  }
  if ((tag == 5)) {
    return "==";
  } else {
  }
  if ((tag == 6)) {
    return "!=";
  } else {
  }
  if ((tag == 7)) {
    return "<";
  } else {
  }
  if ((tag == 8)) {
    return ">";
  } else {
  }
  if ((tag == 9)) {
    return "<=";
  } else {
  }
  if ((tag == 10)) {
    return ">=";
  } else {
  }
  if ((tag == 11)) {
    return "&&";
  } else {
  }
  if ((tag == 12)) {
    return "||";
  } else {
  }
  if ((tag == 13)) {
    return "&";
  } else {
  }
  if ((tag == 14)) {
    return "|";
  } else {
  }
  if ((tag == 15)) {
    return "^";
  } else {
  }
  if ((tag == 16)) {
    return "<<";
  } else {
  }
  if ((tag == 17)) {
    return ">>";
  } else {
  }
  return "+";
}

static const char* ast_unop_to_c(int tag) {
  if ((tag == 0)) {
    return "-";
  } else {
  }
  if ((tag == 1)) {
    return "!";
  } else {
  }
  if ((tag == 2)) {
    return "~";
  } else {
  }
  if ((tag == 3)) {
    return "*";
  } else {
  }
  if ((tag == 4)) {
    return "&";
  } else {
  }
  if ((tag == 5)) {
    return "&";
  } else {
  }
  return "?";
}

static int is_supported_unop(int tag) {
  if ((tag == 0)) {
    return 1;
  } else {
  }
  if ((tag == 1)) {
    return 1;
  } else {
  }
  if ((tag == 2)) {
    return 1;
  } else {
  }
  if ((tag == 4)) {
    return 1;
  } else {
  }
  if ((tag == 5)) {
    return 1;
  } else {
  }
  return 0;
}

static const char* ast_ty_to_c_string(const char* name) {
  int _sv0t0 = sv0_string_eq(name, "unit");
  if (_sv0t0) {
    return "void";
  } else {
  }
  int _sv0t1 = sv0_string_eq(name, "i32");
  if (_sv0t1) {
    return "int";
  } else {
  }
  int _sv0t2 = sv0_string_eq(name, "bool");
  if (_sv0t2) {
    return "int";
  } else {
  }
  int _sv0t3 = sv0_string_eq(name, "i8");
  if (_sv0t3) {
    return "int8_t";
  } else {
  }
  int _sv0t4 = sv0_string_eq(name, "u8");
  if (_sv0t4) {
    return "uint8_t";
  } else {
  }
  int _sv0t5 = sv0_string_eq(name, "i16");
  if (_sv0t5) {
    return "int16_t";
  } else {
  }
  int _sv0t6 = sv0_string_eq(name, "u16");
  if (_sv0t6) {
    return "uint16_t";
  } else {
  }
  int _sv0t7 = sv0_string_eq(name, "i64");
  if (_sv0t7) {
    return "int64_t";
  } else {
  }
  int _sv0t8 = sv0_string_eq(name, "u64");
  if (_sv0t8) {
    return "uint64_t";
  } else {
  }
  int _sv0t9 = sv0_string_eq(name, "isize");
  if (_sv0t9) {
    return "intptr_t";
  } else {
  }
  int _sv0t10 = sv0_string_eq(name, "usize");
  if (_sv0t10) {
    return "uintptr_t";
  } else {
  }
  int _sv0t11 = sv0_string_eq(name, "string");
  if (_sv0t11) {
    return "const char*";
  } else {
  }
  int _sv0t12 = sv0_string_eq(name, "str");
  if (_sv0t12) {
    return "const char*";
  } else {
  }
  int _sv0t13 = sv0_string_eq(name, "String");
  if (_sv0t13) {
    return "const char*";
  } else {
  }
  int _sv0t14 = sv0_string_eq(name, "Vec");
  if (_sv0t14) {
    return "int";
  } else {
  }
  int _sv0t15 = sv0_string_eq(name, "Box");
  if (_sv0t15) {
    return "int";
  } else {
  }
  return "int";
}

static const char* ast_ty_to_c_string_with_user(const char* name, int name_handle, int struct_names, int enum_names) {
  const char* _sv0t0 = ast_ty_to_c_string(name);
  const char* prim;
  prim = _sv0t0;
  int _sv0t1 = sv0_string_eq(prim, "int");
  if ((_sv0t1 != 1)) {
    return prim;
  } else {
  }
  int _sv0t2 = is_struct_cty(struct_names, name_handle);
  if (_sv0t2) {
    return name;
  } else {
  }
  int _sv0t3 = is_enum_cty(enum_names, name_handle);
  if (_sv0t3) {
    return name;
  } else {
  }
  return "int";
}

static int is_void_cty(const char* name) {
  int _sv0t0 = sv0_string_eq(name, "void");
  if (_sv0t0) {
    return 1;
  } else {
  }
  int _sv0t1 = sv0_string_eq(name, "unit");
  if (_sv0t1) {
    return 1;
  } else {
  }
  return 0;
}

static int ast_ty_is_unit(const char* name) {
  int _sv0t0 = sv0_string_eq(name, "unit");
  return _sv0t0;
}

static const char* old_slot_prefix(void) {
  return "_sv0old_";
}

static int old_slot_name_len(int name_len) {
  int _sv0t0 = (8 + name_len);
  return _sv0t0;
}

static const char* old_slot_name(const char* name) {
  const char* _sv0t0 = sv0_string_concat("_sv0old_", name);
  return _sv0t0;
}

static int param_name_tag(int pat_tag) {
  int _sv0t0 = (pat_tag == 1);
  return _sv0t0;
}

static int param_name(int pat_tag, int name_handle) {
  if ((pat_tag == 1)) {
    return name_handle;
  } else {
  }
  int _sv0t0 = (0 - 1);
  return _sv0t0;
}

static int lower_field_index(int field_names, int target) {
  int _sv0t0 = sv0_vec_len(field_names);
  int n = _sv0t0;
  int i = 0;
  while ((i < n)) {
    int _sv0t1 = sv0_vec_get(field_names, i);
    if ((_sv0t1 == target)) {
      return i;
    } else {
    }
    i = (i + 1);
  }
  int _sv0t2 = (0 - 1);
  return _sv0t2;
}

static int old_name_seen(int seen, int name) {
  int _sv0t0 = sv0_vec_len(seen);
  int n = _sv0t0;
  int i = 0;
  while ((i < n)) {
    int _sv0t1 = sv0_vec_get(seen, i);
    if ((_sv0t1 == name)) {
      return 1;
    } else {
    }
    i = (i + 1);
  }
  return 0;
}

static int old_names_add_unique(int seen, int name) {
  int _sv0t0 = old_name_seen(seen, name);
  if (_sv0t0) {
    return 0;
  } else {
  }
  sv0_vec_push(seen, name);
  return 1;
}

static int uniq_old_names(int names) {
  int _sv0t0 = sv0_vec_new();
  int out = _sv0t0;
  int _sv0t1 = sv0_vec_len(names);
  int n = _sv0t1;
  int i = 0;
  while ((i < n)) {
    int _sv0t2 = sv0_vec_get(names, i);
    int name = _sv0t2;
    int _sv0t3 = old_names_add_unique(out, name);
    i = (i + 1);
  }
  return out;
}

static int ret_syntax_is_unit(int has_ret, const char* ret_ty_name) {
  if ((has_ret == 0)) {
    return 1;
  } else {
  }
  int _sv0t0 = sv0_string_eq(ret_ty_name, "unit");
  if (_sv0t0) {
    return 1;
  } else {
  }
  return 0;
}

static int use_ret_slot(int is_unit, int ens_mentions_result) {
  if (is_unit) {
    return 0;
  } else {
  }
  return ens_mentions_result;
}

static int append_instrs(int dst, int src) {
  int _sv0t0 = sv0_vec_len(src);
  int n = _sv0t0;
  int i = 0;
  while ((i < n)) {
    int _sv0t1 = sv0_vec_get(src, i);
    sv0_vec_push(dst, _sv0t1);
    i = (i + 1);
  }
  return 0;
}

static int store_value_to_slot(int slot, Value v, int out_instrs) {
  int _sv0t0 = ir_value_tag(v);
  int vtag = _sv0t0;
  if ((vtag == 3)) {
    int _sv0t1;
    if ((v.tag == 3)) {
      int x = v.p0;
      if ((x != slot)) {
        Instr si0;
        Expr _sv0t6;
        _sv0t6.tag = 1;
        _sv0t6.p0 = x;
        int _sv0t7 = sv0_box_alloc(4);
        sv0_box_store(_sv0t7, 0, _sv0t6.tag);
        sv0_box_store(_sv0t7, 1, _sv0t6.p0);
        sv0_box_store(_sv0t7, 2, _sv0t6.p1);
        sv0_box_store(_sv0t7, 3, _sv0t6.p2);
        si0.tag = 4;
        si0.p0 = slot;
        si0.p1 = _sv0t7;
        int _sv0t8 = sv0_box_alloc(5);
        sv0_box_store(_sv0t8, 0, si0.tag);
        sv0_box_store(_sv0t8, 1, si0.p0);
        sv0_box_store(_sv0t8, 2, si0.p1);
        sv0_box_store(_sv0t8, 3, si0.p2);
        sv0_box_store(_sv0t8, 4, si0.p3);
        sv0_vec_push(out_instrs, _sv0t8);
      } else {
      }
      return 0;
      _sv0t1 = 0;
    } else {
      if (1) {
        Instr si1;
        Expr _sv0t2;
        int _sv0t3 = sv0_box_alloc(3);
        sv0_box_store(_sv0t3, 0, v.tag);
        sv0_box_store(_sv0t3, 1, v.p0);
        sv0_box_store(_sv0t3, 2, v.p1);
        _sv0t2.tag = 0;
        _sv0t2.p0 = _sv0t3;
        int _sv0t4 = sv0_box_alloc(4);
        sv0_box_store(_sv0t4, 0, _sv0t2.tag);
        sv0_box_store(_sv0t4, 1, _sv0t2.p0);
        sv0_box_store(_sv0t4, 2, _sv0t2.p1);
        sv0_box_store(_sv0t4, 3, _sv0t2.p2);
        si1.tag = 4;
        si1.p0 = slot;
        si1.p1 = _sv0t4;
        int _sv0t5 = sv0_box_alloc(5);
        sv0_box_store(_sv0t5, 0, si1.tag);
        sv0_box_store(_sv0t5, 1, si1.p0);
        sv0_box_store(_sv0t5, 2, si1.p1);
        sv0_box_store(_sv0t5, 3, si1.p2);
        sv0_box_store(_sv0t5, 4, si1.p3);
        sv0_vec_push(out_instrs, _sv0t5);
        return 0;
        _sv0t1 = 0;
      } else {
      }
    }
  } else {
  }
  Instr si2;
  Expr _sv0t9;
  int _sv0t10 = sv0_box_alloc(3);
  sv0_box_store(_sv0t10, 0, v.tag);
  sv0_box_store(_sv0t10, 1, v.p0);
  sv0_box_store(_sv0t10, 2, v.p1);
  _sv0t9.tag = 0;
  _sv0t9.p0 = _sv0t10;
  int _sv0t11 = sv0_box_alloc(4);
  sv0_box_store(_sv0t11, 0, _sv0t9.tag);
  sv0_box_store(_sv0t11, 1, _sv0t9.p0);
  sv0_box_store(_sv0t11, 2, _sv0t9.p1);
  sv0_box_store(_sv0t11, 3, _sv0t9.p2);
  si2.tag = 4;
  si2.p0 = slot;
  si2.p1 = _sv0t11;
  int _sv0t12 = sv0_box_alloc(5);
  sv0_box_store(_sv0t12, 0, si2.tag);
  sv0_box_store(_sv0t12, 1, si2.p0);
  sv0_box_store(_sv0t12, 2, si2.p1);
  sv0_box_store(_sv0t12, 3, si2.p2);
  sv0_box_store(_sv0t12, 4, si2.p3);
  sv0_vec_push(out_instrs, _sv0t12);
  return 0;
}

static int lower_ensures_clause(int et, int ed1, int ed2, int ed3, int ed4, int pp, int tok_tags, int expr_idx, int map_result, int fn_name_h, int ctr, int out_instrs, int enum_names, int enum_tag_offsets, int enum_tag_counts, int enum_tags_flat, int fn_ctx, int builtin_map, int lit_sf_names, const char* source, int starts, int ends, int item_tags, int item_names, int item_d2, int item_d3, int item_d4, int item_field_counts, int enum_vnames_flat, int enum_max_payloads, int item_fn_row, int match_env_inherit, int import_aliases) {
  Expr _sv0t0 = lower_expr_with_instrs(et, ed1, ed2, ed3, ed4, pp, tok_tags, expr_idx, ctr, out_instrs, enum_names, enum_tag_offsets, enum_tag_counts, enum_tags_flat, fn_ctx, builtin_map, lit_sf_names, source, starts, ends, item_tags, item_names, item_d2, item_d3, item_d4, item_field_counts, enum_vnames_flat, enum_max_payloads, item_fn_row, match_env_inherit, import_aliases);
  Expr ee;
  ee = _sv0t0;
  if ((map_result == 1)) {
    int keep_shape = 0;
  } else {
  }
  Instr ei;
  int _sv0t1 = sv0_box_alloc(4);
  sv0_box_store(_sv0t1, 0, ee.tag);
  sv0_box_store(_sv0t1, 1, ee.p0);
  sv0_box_store(_sv0t1, 2, ee.p1);
  sv0_box_store(_sv0t1, 3, ee.p2);
  ei.tag = 13;
  ei.p0 = _sv0t1;
  ei.p1 = fn_name_h;
  int _sv0t2 = sv0_box_alloc(5);
  sv0_box_store(_sv0t2, 0, ei.tag);
  sv0_box_store(_sv0t2, 1, ei.p0);
  sv0_box_store(_sv0t2, 2, ei.p1);
  sv0_box_store(_sv0t2, 3, ei.p2);
  sv0_box_store(_sv0t2, 4, ei.p3);
  sv0_vec_push(out_instrs, _sv0t2);
  return 0;
}

static int inject_ensures_and_ret_slot(int is, int ens_instrs, int use_ret_slot_flag, int ret_slot) {
  int _sv0t0 = sv0_vec_len(ens_instrs);
  if ((_sv0t0 == 0)) {
    return is;
  } else {
  }
  int _sv0t1 = sv0_vec_new();
  int out = _sv0t1;
  int _sv0t2 = sv0_vec_len(is);
  int n = _sv0t2;
  int i = 0;
  while ((i < n)) {
    int _sv0t3 = sv0_vec_get(is, i);
    Instr _sv0t4;
    int _sv0t5 = sv0_box_load(_sv0t3, 0);
    _sv0t4.tag = _sv0t5;
    int _sv0t6 = sv0_box_load(_sv0t3, 1);
    _sv0t4.p0 = _sv0t6;
    int _sv0t7 = sv0_box_load(_sv0t3, 2);
    _sv0t4.p1 = _sv0t7;
    int _sv0t8 = sv0_box_load(_sv0t3, 3);
    _sv0t4.p2 = _sv0t8;
    int _sv0t9 = sv0_box_load(_sv0t3, 4);
    _sv0t4.p3 = _sv0t9;
    Instr ins;
    ins.tag = (_sv0t4).tag;
    ins.p0 = (_sv0t4).p0;
    ins.p1 = (_sv0t4).p1;
    ins.p2 = (_sv0t4).p2;
    ins.p3 = (_sv0t4).p3;
    int _sv0t10;
    if ((ins.tag == 15)) {
      int bv = ins.p0;
      Value _sv0t15;
      int _sv0t16 = sv0_box_load(bv, 0);
      _sv0t15.tag = _sv0t16;
      int _sv0t17 = sv0_box_load(bv, 1);
      _sv0t15.p0 = _sv0t17;
      int _sv0t18 = sv0_box_load(bv, 2);
      _sv0t15.p1 = _sv0t18;
      Value rv;
      rv.tag = (_sv0t15).tag;
      rv.p0 = (_sv0t15).p0;
      rv.p1 = (_sv0t15).p1;
      int _sv0t19;
      if (use_ret_slot_flag) {
        int _sv0t20 = store_value_to_slot(ret_slot, rv, out);
        int discard_sv = _sv0t20;
        int _sv0t21 = append_instrs(out, ens_instrs);
        int discard_append0 = _sv0t21;
        Instr rr;
        Value _sv0t22;
        _sv0t22.tag = 3;
        _sv0t22.p0 = ret_slot;
        int _sv0t23 = sv0_box_alloc(3);
        sv0_box_store(_sv0t23, 0, _sv0t22.tag);
        sv0_box_store(_sv0t23, 1, _sv0t22.p0);
        sv0_box_store(_sv0t23, 2, _sv0t22.p1);
        rr.tag = 15;
        rr.p0 = _sv0t23;
        int _sv0t24 = sv0_box_alloc(5);
        sv0_box_store(_sv0t24, 0, rr.tag);
        sv0_box_store(_sv0t24, 1, rr.p0);
        sv0_box_store(_sv0t24, 2, rr.p1);
        sv0_box_store(_sv0t24, 3, rr.p2);
        sv0_box_store(_sv0t24, 4, rr.p3);
        sv0_vec_push(out, _sv0t24);
        _sv0t19 = 0;
      } else {
        int _sv0t25 = append_instrs(out, ens_instrs);
        int discard_append1 = _sv0t25;
        Instr rr2;
        int _sv0t26 = sv0_box_alloc(3);
        sv0_box_store(_sv0t26, 0, rv.tag);
        sv0_box_store(_sv0t26, 1, rv.p0);
        sv0_box_store(_sv0t26, 2, rv.p1);
        rr2.tag = 15;
        rr2.p0 = _sv0t26;
        int _sv0t27 = sv0_box_alloc(5);
        sv0_box_store(_sv0t27, 0, rr2.tag);
        sv0_box_store(_sv0t27, 1, rr2.p0);
        sv0_box_store(_sv0t27, 2, rr2.p1);
        sv0_box_store(_sv0t27, 3, rr2.p2);
        sv0_box_store(_sv0t27, 4, rr2.p3);
        sv0_vec_push(out, _sv0t27);
        _sv0t19 = 0;
      }
      _sv0t10 = _sv0t19;
    } else {
      if ((ins.tag == 14)) {
        int _sv0t12 = append_instrs(out, ens_instrs);
        int discard_append2 = _sv0t12;
        Instr _sv0t13;
        _sv0t13.tag = 14;
        int _sv0t14 = sv0_box_alloc(5);
        sv0_box_store(_sv0t14, 0, _sv0t13.tag);
        sv0_box_store(_sv0t14, 1, _sv0t13.p0);
        sv0_box_store(_sv0t14, 2, _sv0t13.p1);
        sv0_box_store(_sv0t14, 3, _sv0t13.p2);
        sv0_box_store(_sv0t14, 4, _sv0t13.p3);
        sv0_vec_push(out, _sv0t14);
        _sv0t10 = 0;
      } else {
        if (1) {
          int _sv0t11 = sv0_box_alloc(5);
          sv0_box_store(_sv0t11, 0, ins.tag);
          sv0_box_store(_sv0t11, 1, ins.p0);
          sv0_box_store(_sv0t11, 2, ins.p1);
          sv0_box_store(_sv0t11, 3, ins.p2);
          sv0_box_store(_sv0t11, 4, ins.p3);
          sv0_vec_push(out, _sv0t11);
          _sv0t10 = 0;
        } else {
        }
      }
    }
    i = (i + 1);
  }
  return out;
}

static int lower_lit_supported(int lit_tag) {
  int _sv0t0 = classify_literal(lit_tag);
  int _sv0t1 = (_sv0t0 >= 0);
  return _sv0t1;
}

static int store_value_class(int is_var, int var_name_eq_slot) {
  if (is_var) {
    if (var_name_eq_slot) {
      return 0;
    } else {
    }
    return 2;
  } else {
  }
  return 1;
}

static int low_classify_assign_lhs(int expr_tag) {
  if ((expr_tag == 2)) {
    return 0;
  } else {
  }
  if ((expr_tag == 11)) {
    return 1;
  } else {
  }
  int _sv0t0 = (0 - 1);
  return _sv0t0;
}

static int enum_tag_lookup_str(int enum_names, int enum_tags_flat, int enum_tag_offsets, int enum_tag_counts, int enum_name, int variant_name, const char* source, int starts, int ends) {
  const char* _sv0t0 = handle_to_str(enum_name, source, starts, ends);
  const char* want_e;
  want_e = _sv0t0;
  const char* _sv0t1 = handle_to_str(variant_name, source, starts, ends);
  const char* want_v;
  want_v = _sv0t1;
  int _sv0t2 = sv0_vec_len(enum_names);
  int n = _sv0t2;
  int ei = 0;
  while ((ei < n)) {
    int _sv0t3 = sv0_vec_get(enum_names, ei);
    const char* _sv0t4 = handle_to_str(_sv0t3, source, starts, ends);
    const char* e_nm;
    e_nm = _sv0t4;
    int _sv0t5 = sv0_string_eq(e_nm, want_e);
    if (_sv0t5) {
      int _sv0t6 = sv0_vec_get(enum_tag_offsets, ei);
      int offset = _sv0t6;
      int _sv0t7 = sv0_vec_get(enum_tag_counts, ei);
      int count = _sv0t7;
      int vi = 0;
      while ((vi < count)) {
        int _sv0t8 = (vi * 2);
        int flat_idx = (offset + _sv0t8);
        int _sv0t9 = sv0_vec_get(enum_tags_flat, flat_idx);
        const char* _sv0t10 = handle_to_str(_sv0t9, source, starts, ends);
        const char* v_nm;
        v_nm = _sv0t10;
        int _sv0t11 = sv0_string_eq(v_nm, want_v);
        if (_sv0t11) {
          int _sv0t12 = (flat_idx + 1);
          int _sv0t13 = sv0_vec_get(enum_tags_flat, _sv0t12);
          return _sv0t13;
        } else {
        }
        vi = (vi + 1);
      }
      int _sv0t14 = (0 - 1);
      return _sv0t14;
    } else {
    }
    ei = (ei + 1);
  }
  int _sv0t15 = (0 - 2);
  return _sv0t15;
}

static int enum_tag_lookup(int enum_names, int enum_tags_flat, int enum_tag_offsets, int enum_tag_counts, int enum_name, int variant_name) {
  int _sv0t0 = sv0_vec_len(enum_names);
  int n = _sv0t0;
  int ei = 0;
  while ((ei < n)) {
    int _sv0t1 = sv0_vec_get(enum_names, ei);
    if ((_sv0t1 == enum_name)) {
      int _sv0t2 = sv0_vec_get(enum_tag_offsets, ei);
      int offset = _sv0t2;
      int _sv0t3 = sv0_vec_get(enum_tag_counts, ei);
      int count = _sv0t3;
      int vi = 0;
      while ((vi < count)) {
        int _sv0t4 = (vi * 2);
        int flat_idx = (offset + _sv0t4);
        int _sv0t5 = sv0_vec_get(enum_tags_flat, flat_idx);
        if ((_sv0t5 == variant_name)) {
          int _sv0t6 = (flat_idx + 1);
          int _sv0t7 = sv0_vec_get(enum_tags_flat, _sv0t6);
          return _sv0t7;
        } else {
        }
        vi = (vi + 1);
      }
      int _sv0t8 = (0 - 1);
      return _sv0t8;
    } else {
    }
    ei = (ei + 1);
  }
  int _sv0t9 = (0 - 2);
  return _sv0t9;
}

static int lower_scan_struct_field_names(int tok_tags, int name_tok_pos, int field_count, int out_field_names) {
  int p = (name_tok_pos + 1);
  while (1) {
    int _sv0t0 = sv0_vec_get(tok_tags, p);
    int _sv0t3 = (_sv0t0 != 8);
    if ((!_sv0t3)) {
      break;
    } else {
    }
    p = (p + 1);
    int _sv0t1 = sv0_vec_len(tok_tags);
    if ((p >= _sv0t1)) {
      int _sv0t2 = (0 - 1);
      return _sv0t2;
    } else {
    }
  }
  p = (p + 1);
  int found = 0;
  while ((found < field_count)) {
    int _sv0t4 = sv0_vec_get(tok_tags, p);
    if ((_sv0t4 == 87)) {
      p = (p + 1);
    } else {
    }
    int _sv0t5 = sv0_vec_get(tok_tags, p);
    if ((_sv0t5 != 5)) {
      int _sv0t6 = (0 - 1);
      return _sv0t6;
    } else {
    }
    sv0_vec_push(out_field_names, p);
    found = (found + 1);
    p = (p + 1);
    int _sv0t7 = sv0_vec_get(tok_tags, p);
    if ((_sv0t7 != 14)) {
      int _sv0t8 = (0 - 1);
      return _sv0t8;
    } else {
    }
    p = (p + 1);
    int depth = 0;
    int scanning_type = 1;
    while (scanning_type) {
      int _sv0t9 = sv0_vec_get(tok_tags, p);
      int t = _sv0t9;
      if ((t == 8)) {
        depth = (depth + 1);
        p = (p + 1);
      } else {
        if ((t == 10)) {
          depth = (depth + 1);
          p = (p + 1);
        } else {
          if ((t == 9)) {
            if ((depth > 0)) {
              depth = (depth - 1);
              p = (p + 1);
            } else {
              scanning_type = 0;
            }
          } else {
            if ((t == 11)) {
              if ((depth > 0)) {
                depth = (depth - 1);
                p = (p + 1);
              } else {
                scanning_type = 0;
              }
            } else {
              if ((t == 12)) {
                if ((depth == 0)) {
                  scanning_type = 0;
                  p = (p + 1);
                } else {
                  p = (p + 1);
                }
              } else {
                p = (p + 1);
              }
            }
          }
        }
      }
    }
  }
  return 0;
}

static int lower_pat_struct_field_payload_index(int tok_tags, const char* source, int starts, int ends, int vn_name_tok, int field_count, int field_name_tok) {
  int _sv0t0 = sv0_vec_new();
  int fnames = _sv0t0;
  int _sv0t1 = lower_scan_struct_field_names(tok_tags, vn_name_tok, field_count, fnames);
  int sr = _sv0t1;
  if ((sr != 0)) {
    int _sv0t2 = (0 - 1);
    return _sv0t2;
  } else {
  }
  int _sv0t3 = sv0_vec_get(starts, field_name_tok);
  int ws = _sv0t3;
  int _sv0t4 = sv0_vec_get(ends, field_name_tok);
  int we = _sv0t4;
  int _sv0t5 = (we - ws);
  const char* _sv0t6 = sv0_string_substr(source, ws, _sv0t5);
  const char* want;
  want = _sv0t6;
  int fi = 0;
  while ((fi < field_count)) {
    int _sv0t7 = sv0_vec_get(fnames, fi);
    int ftp = _sv0t7;
    int _sv0t8 = sv0_vec_get(starts, ftp);
    int fs = _sv0t8;
    int _sv0t9 = sv0_vec_get(ends, ftp);
    int fe = _sv0t9;
    int _sv0t10 = (fe - fs);
    const char* _sv0t11 = sv0_string_substr(source, fs, _sv0t10);
    const char* s2;
    s2 = _sv0t11;
    int _sv0t12 = sv0_string_eq(s2, want);
    if (_sv0t12) {
      return fi;
    } else {
    }
    fi = (fi + 1);
  }
  int _sv0t13 = (0 - 1);
  return _sv0t13;
}

static int struct_fields_lookup(int struct_names, int struct_field_offsets, int struct_field_counts, int struct_fields_flat, int name) {
  int _sv0t0 = sv0_vec_len(struct_names);
  int n = _sv0t0;
  int i = 0;
  while ((i < n)) {
    int _sv0t1 = sv0_vec_get(struct_names, i);
    if ((_sv0t1 == name)) {
      int _sv0t2 = sv0_vec_get(struct_field_offsets, i);
      return _sv0t2;
    } else {
    }
    i = (i + 1);
  }
  int _sv0t3 = (0 - 1);
  return _sv0t3;
}

static int struct_field_count(int struct_names, int struct_field_counts, int name) {
  int _sv0t0 = sv0_vec_len(struct_names);
  int n = _sv0t0;
  int i = 0;
  while ((i < n)) {
    int _sv0t1 = sv0_vec_get(struct_names, i);
    if ((_sv0t1 == name)) {
      int _sv0t2 = sv0_vec_get(struct_field_counts, i);
      return _sv0t2;
    } else {
    }
    i = (i + 1);
  }
  return 0;
}

static int is_enum_cty(int enum_names, int name) {
  int _sv0t0 = sv0_vec_len(enum_names);
  int n = _sv0t0;
  int i = 0;
  while ((i < n)) {
    int _sv0t1 = sv0_vec_get(enum_names, i);
    if ((_sv0t1 == name)) {
      return 1;
    } else {
    }
    i = (i + 1);
  }
  return 0;
}

static int is_struct_cty(int struct_names, int name) {
  int _sv0t0 = sv0_vec_len(struct_names);
  int n = _sv0t0;
  int i = 0;
  while ((i < n)) {
    int _sv0t1 = sv0_vec_get(struct_names, i);
    if ((_sv0t1 == name)) {
      return 1;
    } else {
    }
    i = (i + 1);
  }
  return 0;
}

static int variant_slot_count(int shape_tag, int field_count) {
  if ((shape_tag == 0)) {
    return 0;
  } else {
  }
  return field_count;
}

static int enum_max_payload_lookup(int enum_names, int enum_max_payloads, int name) {
  int _sv0t0 = sv0_vec_len(enum_names);
  int n = _sv0t0;
  int i = 0;
  while ((i < n)) {
    int _sv0t1 = sv0_vec_get(enum_names, i);
    if ((_sv0t1 == name)) {
      int _sv0t2 = sv0_vec_get(enum_max_payloads, i);
      return _sv0t2;
    } else {
    }
    i = (i + 1);
  }
  int _sv0t3 = (0 - 1);
  return _sv0t3;
}

static const char* try_variant_success(int has_ok, int has_some) {
  if (has_ok) {
    return "Ok";
  } else {
  }
  if (has_some) {
    return "Some";
  } else {
  }
  return "";
}

static const char* try_variant_failure(int has_err, int has_none) {
  if (has_err) {
    return "Err";
  } else {
  }
  if (has_none) {
    return "None";
  } else {
  }
  return "";
}

static int is_try_enum(int has_ok, int has_err, int has_some, int has_none) {
  if (has_ok) {
    if (has_err) {
      return 1;
    } else {
    }
  } else {
  }
  if (has_some) {
    if (has_none) {
      return 1;
    } else {
    }
  } else {
  }
  return 0;
}

static int try_variant_names_lookup(int enum_names, int enum_tag_offsets, int enum_tag_counts, int enum_tags_flat, const char* source, int starts, int ends, int enum_name, int out_handles) {
  int _sv0t0 = sv0_vec_len(enum_names);
  int n = _sv0t0;
  int ei = 0;
  while ((ei < n)) {
    int _sv0t1 = sv0_vec_get(enum_names, ei);
    if ((_sv0t1 == enum_name)) {
      int _sv0t2 = sv0_vec_get(enum_tag_offsets, ei);
      int offset = _sv0t2;
      int _sv0t3 = sv0_vec_get(enum_tag_counts, ei);
      int count = _sv0t3;
      int has_ok = 0;
      int has_err = 0;
      int has_some = 0;
      int has_none = 0;
      int ok_h = 0;
      int err_h = 0;
      int some_h = 0;
      int none_h = 0;
      int vi = 0;
      while ((vi < count)) {
        int _sv0t4 = (vi * 2);
        int _sv0t5 = (offset + _sv0t4);
        int _sv0t6 = sv0_vec_get(enum_tags_flat, _sv0t5);
        int vn_h = _sv0t6;
        const char* _sv0t7 = handle_to_str(vn_h, source, starts, ends);
        const char* vn_s;
        vn_s = _sv0t7;
        int _sv0t8 = sv0_string_eq(vn_s, "Ok");
        if (_sv0t8) {
          has_ok = 1;
          ok_h = vn_h;
        } else {
        }
        int _sv0t9 = sv0_string_eq(vn_s, "Err");
        if (_sv0t9) {
          has_err = 1;
          err_h = vn_h;
        } else {
        }
        int _sv0t10 = sv0_string_eq(vn_s, "Some");
        if (_sv0t10) {
          has_some = 1;
          some_h = vn_h;
        } else {
        }
        int _sv0t11 = sv0_string_eq(vn_s, "None");
        if (_sv0t11) {
          has_none = 1;
          none_h = vn_h;
        } else {
        }
        vi = (vi + 1);
      }
      if (has_ok) {
        if (has_err) {
          sv0_vec_push(out_handles, ok_h);
          sv0_vec_push(out_handles, err_h);
          return 1;
        } else {
        }
      } else {
      }
      if (has_some) {
        if (has_none) {
          sv0_vec_push(out_handles, some_h);
          sv0_vec_push(out_handles, none_h);
          return 1;
        } else {
        }
      } else {
      }
      return 0;
    } else {
    }
    ei = (ei + 1);
  }
  return 0;
}

static int find_variant_in_items(int item_tags, int item_names, int item_field_counts, int variant_names_flat, int enum_name, int variant_name) {
  int _sv0t0 = sv0_vec_len(item_tags);
  int n = _sv0t0;
  int i = 0;
  int vn_offset = 0;
  while ((i < n)) {
    int _sv0t1 = sv0_vec_get(item_tags, i);
    int tag = _sv0t1;
    int _sv0t2 = sv0_vec_get(item_field_counts, i);
    int vc = _sv0t2;
    if ((tag == 2)) {
      int _sv0t3 = sv0_vec_get(item_names, i);
      if ((_sv0t3 == enum_name)) {
        int vi = 0;
        while ((vi < vc)) {
          int _sv0t4 = (vn_offset + vi);
          int _sv0t5 = sv0_vec_get(variant_names_flat, _sv0t4);
          if ((_sv0t5 == variant_name)) {
            return vi;
          } else {
          }
          vi = (vi + 1);
        }
        int _sv0t6 = (0 - 2);
        return _sv0t6;
      } else {
      }
      vn_offset = (vn_offset + vc);
    } else {
    }
    i = (i + 1);
  }
  int _sv0t7 = (0 - 1);
  return _sv0t7;
}

static int classify_literal(int lit_tag) {
  if ((lit_tag == 0)) {
    return 0;
  } else {
  }
  if ((lit_tag == 1)) {
    return 1;
  } else {
  }
  if ((lit_tag == 2)) {
    return 2;
  } else {
  }
  if ((lit_tag == 3)) {
    return 3;
  } else {
  }
  int _sv0t0 = (0 - 1);
  return _sv0t0;
}

static int lower_lit_to_ir_tag(int lit_tag) {
  if ((lit_tag == 0)) {
    return 0;
  } else {
  }
  if ((lit_tag == 1)) {
    return 1;
  } else {
  }
  if ((lit_tag == 2)) {
    return 4;
  } else {
  }
  if ((lit_tag == 3)) {
    return 5;
  } else {
  }
  int _sv0t0 = (0 - 1);
  return _sv0t0;
}

static const char* fn_ret_cty(int has_ret, const char* ret_cty) {
  if (has_ret) {
    return ret_cty;
  } else {
  }
  return "int";
}

static int value_to_expr_kind(int val_tag) {
  if ((val_tag == 0)) {
    return 0;
  } else {
  }
  return 1;
}

static int payload_field_name(int index) {
  return index;
}

static int enum_store_payload_count(int val_count) {
  return val_count;
}

static int enum_alloc_width(int max_payload) {
  int _sv0t0 = (1 + max_payload);
  return _sv0t0;
}

static const char* ret_value_slot(void) {
  return "_sv0ret";
}

static int find_fn_index(int fn_names, int name_h) {
  int _sv0t0 = sv0_vec_len(fn_names);
  int n = _sv0t0;
  int i = 0;
  while ((i < n)) {
    int _sv0t1 = sv0_vec_get(fn_names, i);
    if ((_sv0t1 == name_h)) {
      return i;
    } else {
    }
    i = (i + 1);
  }
  int _sv0t2 = (0 - 1);
  return _sv0t2;
}

static int callee_fn_index(int fn_names, int aliases, int name_h) {
  int _sv0t0 = resolve_fn_callee(aliases, name_h);
  int resolved = _sv0t0;
  int _sv0t1 = find_fn_index(fn_names, resolved);
  return _sv0t1;
}

static int callee_fn_index_match_env(int env, int aliases, int name_h) {
  int _sv0t0 = lower_match_env_is_packed_v0(env);
  if ((_sv0t0 != 1)) {
    int _sv0t1 = (0 - 1);
    return _sv0t1;
  } else {
  }
  int _sv0t2 = sv0_vec_get(env, 4);
  int fc = _sv0t2;
  if ((fc <= 0)) {
    int _sv0t3 = (0 - 1);
    return _sv0t3;
  } else {
  }
  int _sv0t4 = sv0_vec_get(env, 2);
  int bfn = _sv0t4;
  if ((bfn < 0)) {
    int _sv0t5 = (0 - 1);
    return _sv0t5;
  } else {
  }
  int _sv0t6 = resolve_fn_callee(aliases, name_h);
  int resolved = _sv0t6;
  int j = 0;
  while ((j < fc)) {
    int _sv0t7 = (bfn + j);
    int _sv0t8 = sv0_vec_get(env, _sv0t7);
    if ((_sv0t8 == resolved)) {
      return j;
    } else {
    }
    j = (j + 1);
  }
  int _sv0t9 = (0 - 1);
  return _sv0t9;
}

static int scrut_local_lookup(int scrut_names, int name_h) {
  int _sv0t0 = sv0_vec_len(scrut_names);
  int n = _sv0t0;
  int i = 0;
  while ((i < n)) {
    int _sv0t1 = sv0_vec_get(scrut_names, i);
    if ((_sv0t1 == name_h)) {
      return i;
    } else {
    }
    i = (i + 1);
  }
  int _sv0t2 = (0 - 1);
  return _sv0t2;
}

static int param_ty_lookup(int param_names, int name_h) {
  int _sv0t0 = sv0_vec_len(param_names);
  int n = _sv0t0;
  int i = 0;
  while ((i < n)) {
    int _sv0t1 = sv0_vec_get(param_names, i);
    if ((_sv0t1 == name_h)) {
      return i;
    } else {
    }
    i = (i + 1);
  }
  int _sv0t2 = (0 - 1);
  return _sv0t2;
}

static const char* handle_to_str(int h, const char* source, int starts, int ends) {
  int _sv0t0 = sv0_vec_get(starts, h);
  int s = _sv0t0;
  int _sv0t1 = sv0_vec_get(ends, h);
  int e = _sv0t1;
  int _sv0t2 = (e - s);
  const char* _sv0t3 = sv0_string_substr(source, s, _sv0t2);
  return _sv0t3;
}

static int lower_name_in_handle_list(const char* name, int list, const char* source, int starts, int ends) {
  int _sv0t0 = sv0_vec_len(list);
  int n = _sv0t0;
  int i = 0;
  while ((i < n)) {
    int _sv0t1 = sv0_vec_get(list, i);
    int h = _sv0t1;
    int _sv0t2 = lower_tok_in_bounds(h, starts, ends);
    if (_sv0t2) {
      const char* _sv0t3 = handle_to_str(h, source, starts, ends);
      int _sv0t4 = sv0_string_eq(_sv0t3, name);
      if (_sv0t4) {
        return 1;
      } else {
      }
    } else {
    }
    i = (i + 1);
  }
  return 0;
}

static const char* emit_struct_td(int name_h, int fnames, int ftypes, int foff, int fcount, const char* source, int starts, int ends, int sn, int en) {
  const char* _sv0t0 = handle_to_str(name_h, source, starts, ends);
  const char* name;
  name = _sv0t0;
  const char* r;
  r = "typedef struct {\n";
  int i = 0;
  while ((i < fcount)) {
    int _sv0t1 = (foff + i);
    int _sv0t2 = sv0_vec_get(ftypes, _sv0t1);
    int th = _sv0t2;
    const char* _sv0t3 = handle_to_str(th, source, starts, ends);
    const char* tname;
    tname = _sv0t3;
    const char* _sv0t4 = ast_ty_to_c_string_with_user(tname, th, sn, en);
    const char* cty;
    cty = _sv0t4;
    int _sv0t5 = sv0_string_eq(cty, "int");
    if (_sv0t5) {
      int _sv0t6 = lower_name_in_handle_list(tname, sn, source, starts, ends);
      if (_sv0t6) {
        cty = tname;
      } else {
        int _sv0t7 = lower_name_in_handle_list(tname, en, source, starts, ends);
        if (_sv0t7) {
          cty = tname;
        } else {
        }
      }
    } else {
    }
    int _sv0t8 = (foff + i);
    int _sv0t9 = sv0_vec_get(fnames, _sv0t8);
    const char* _sv0t10 = handle_to_str(_sv0t9, source, starts, ends);
    const char* fname;
    fname = _sv0t10;
    const char* _sv0t11 = sv0_string_concat(r, "  ");
    r = _sv0t11;
    const char* _sv0t12 = sv0_string_concat(r, cty);
    r = _sv0t12;
    const char* _sv0t13 = sv0_string_concat(r, " ");
    r = _sv0t13;
    const char* _sv0t14 = sv0_string_concat(r, fname);
    r = _sv0t14;
    const char* _sv0t15 = sv0_string_concat(r, ";\n");
    r = _sv0t15;
    i = (i + 1);
  }
  const char* _sv0t16 = sv0_string_concat(r, "} ");
  r = _sv0t16;
  const char* _sv0t17 = sv0_string_concat(r, name);
  r = _sv0t17;
  const char* _sv0t18 = sv0_string_concat(r, ";\n");
  r = _sv0t18;
  return r;
}

static const char* emit_enum_td(int name_h, int max_payload, const char* source, int starts, int ends) {
  const char* _sv0t0 = handle_to_str(name_h, source, starts, ends);
  const char* name;
  name = _sv0t0;
  const char* r;
  r = "typedef struct {\n  int tag;\n";
  int i = 0;
  while ((i < max_payload)) {
    const char* _sv0t1 = sv0_string_concat(r, "  int p");
    r = _sv0t1;
    const char* _sv0t2 = lower_int_to_str(i);
    const char* _sv0t3 = sv0_string_concat(r, _sv0t2);
    r = _sv0t3;
    const char* _sv0t4 = sv0_string_concat(r, ";\n");
    r = _sv0t4;
    i = (i + 1);
  }
  const char* _sv0t5 = sv0_string_concat(r, "} ");
  r = _sv0t5;
  const char* _sv0t6 = sv0_string_concat(r, name);
  r = _sv0t6;
  const char* _sv0t7 = sv0_string_concat(r, ";\n");
  r = _sv0t7;
  return r;
}

static int build_struct_order(int item_tags, int item_names, int item_field_counts, int struct_fnames_flat, int out_names, int out_offsets, int out_counts, int out_flat) {
  int _sv0t0 = sv0_vec_len(item_tags);
  int n = _sv0t0;
  int i = 0;
  int sf = 0;
  while ((i < n)) {
    int _sv0t1 = sv0_vec_get(item_tags, i);
    int tag = _sv0t1;
    int _sv0t2 = sv0_vec_get(item_field_counts, i);
    int fc = _sv0t2;
    if ((tag == 1)) {
      int _sv0t3 = sv0_vec_get(item_names, i);
      sv0_vec_push(out_names, _sv0t3);
      int _sv0t4 = sv0_vec_len(out_flat);
      sv0_vec_push(out_offsets, _sv0t4);
      sv0_vec_push(out_counts, fc);
      int j = 0;
      while ((j < fc)) {
        int _sv0t5 = (sf + j);
        int _sv0t6 = sv0_vec_get(struct_fnames_flat, _sv0t5);
        sv0_vec_push(out_flat, _sv0t6);
        j = (j + 1);
      }
      sf = (sf + fc);
    } else {
    }
    i = (i + 1);
  }
  int _sv0t7 = sv0_vec_len(out_names);
  return _sv0t7;
}

static int build_enum_variants_data(int item_tags, int item_names, int item_field_counts, int item_vmax, int enum_vnames_flat, int out_names, int out_tag_offsets, int out_tag_counts, int out_tags_flat, int out_max_payloads) {
  int _sv0t0 = sv0_vec_len(item_tags);
  int n = _sv0t0;
  int i = 0;
  int ev = 0;
  while ((i < n)) {
    int _sv0t1 = sv0_vec_get(item_tags, i);
    int tag = _sv0t1;
    int _sv0t2 = sv0_vec_get(item_field_counts, i);
    int vc = _sv0t2;
    if ((tag == 2)) {
      int _sv0t3 = sv0_vec_get(item_names, i);
      sv0_vec_push(out_names, _sv0t3);
      int _sv0t4 = sv0_vec_len(out_tags_flat);
      sv0_vec_push(out_tag_offsets, _sv0t4);
      sv0_vec_push(out_tag_counts, vc);
      int _sv0t5 = sv0_vec_get(item_vmax, i);
      sv0_vec_push(out_max_payloads, _sv0t5);
      int j = 0;
      while ((j < vc)) {
        int _sv0t6 = (ev + j);
        int _sv0t7 = sv0_vec_get(enum_vnames_flat, _sv0t6);
        sv0_vec_push(out_tags_flat, _sv0t7);
        sv0_vec_push(out_tags_flat, j);
        j = (j + 1);
      }
      ev = (ev + vc);
    } else {
    }
    i = (i + 1);
  }
  int _sv0t8 = sv0_vec_len(out_names);
  return _sv0t8;
}

static const char* collect_typedefs_str(int item_tags, int item_names, int item_field_counts, int item_vmax, int sf_names, int sf_types, const char* source, int starts, int ends, int sn, int en) {
  int _sv0t0 = sv0_vec_len(item_tags);
  int n = _sv0t0;
  const char* r;
  r = "";
  int i = 0;
  int sf = 0;
  while ((i < n)) {
    int _sv0t1 = sv0_vec_get(item_tags, i);
    int tag = _sv0t1;
    int _sv0t2 = sv0_vec_get(item_field_counts, i);
    int fc = _sv0t2;
    if ((tag == 1)) {
      int _sv0t3 = sv0_vec_get(item_names, i);
      const char* _sv0t4 = emit_struct_td(_sv0t3, sf_names, sf_types, sf, fc, source, starts, ends, sn, en);
      const char* td;
      td = _sv0t4;
      const char* _sv0t5 = sv0_string_concat(r, td);
      r = _sv0t5;
      sf = (sf + fc);
    } else {
    }
    i = (i + 1);
  }
  i = 0;
  while ((i < n)) {
    int _sv0t6 = sv0_vec_get(item_tags, i);
    if ((_sv0t6 == 2)) {
      int _sv0t7 = sv0_vec_get(item_names, i);
      int _sv0t8 = sv0_vec_get(item_vmax, i);
      const char* _sv0t9 = emit_enum_td(_sv0t7, _sv0t8, source, starts, ends);
      const char* td2;
      td2 = _sv0t9;
      const char* _sv0t10 = sv0_string_concat(r, td2);
      r = _sv0t10;
    } else {
    }
    i = (i + 1);
  }
  return r;
}

static int mentions_result(int et, int ed1, int ed2, int ed3, int ed4, int idx, const char* source, int starts, int ends, int pp) {
  if ((idx < 0)) {
    return 0;
  } else {
  }
  int _sv0t0 = sv0_vec_get(et, idx);
  int tag = _sv0t0;
  if ((tag == 0)) {
    return 0;
  } else {
  }
  if ((tag == 1)) {
    int _sv0t1 = sv0_vec_get(ed2, idx);
    int ppc = _sv0t1;
    if ((ppc == 1)) {
      int _sv0t2 = sv0_vec_get(ed1, idx);
      int _sv0t3 = sv0_vec_get(pp, _sv0t2);
      int tok = _sv0t3;
      const char* _sv0t4 = handle_to_str(tok, source, starts, ends);
      int _sv0t5 = sv0_string_eq(_sv0t4, "result");
      return _sv0t5;
    } else {
    }
    return 0;
  } else {
  }
  if ((tag == 2)) {
    int _sv0t6 = sv0_vec_get(ed2, idx);
    int _sv0t7 = mentions_result(et, ed1, ed2, ed3, ed4, _sv0t6, source, starts, ends, pp);
    return _sv0t7;
  } else {
  }
  if ((tag == 3)) {
    int _sv0t8 = sv0_vec_get(ed2, idx);
    int _sv0t9 = mentions_result(et, ed1, ed2, ed3, ed4, _sv0t8, source, starts, ends, pp);
    if (_sv0t9) {
      return 1;
    } else {
    }
    int _sv0t10 = sv0_vec_get(ed3, idx);
    int _sv0t11 = mentions_result(et, ed1, ed2, ed3, ed4, _sv0t10, source, starts, ends, pp);
    return _sv0t11;
  } else {
  }
  if ((tag == 4)) {
    int _sv0t12 = sv0_vec_get(ed1, idx);
    int _sv0t13 = mentions_result(et, ed1, ed2, ed3, ed4, _sv0t12, source, starts, ends, pp);
    if (_sv0t13) {
      return 1;
    } else {
    }
    int _sv0t14 = sv0_vec_get(ed2, idx);
    int af = _sv0t14;
    int _sv0t15 = sv0_vec_get(ed3, idx);
    int ac = _sv0t15;
    int ai = 0;
    while ((ai < ac)) {
      int _sv0t16 = (af + ai);
      int _sv0t17 = mentions_result(et, ed1, ed2, ed3, ed4, _sv0t16, source, starts, ends, pp);
      if (_sv0t17) {
        return 1;
      } else {
      }
      ai = (ai + 1);
    }
    return 0;
  } else {
  }
  if ((tag == 6)) {
    int _sv0t18 = sv0_vec_get(ed1, idx);
    int _sv0t19 = mentions_result(et, ed1, ed2, ed3, ed4, _sv0t18, source, starts, ends, pp);
    return _sv0t19;
  } else {
  }
  if ((tag == 10)) {
    int _sv0t20 = sv0_vec_get(ed1, idx);
    int _sv0t21 = mentions_result(et, ed1, ed2, ed3, ed4, _sv0t20, source, starts, ends, pp);
    if (_sv0t21) {
      return 1;
    } else {
    }
    int _sv0t22 = sv0_vec_get(ed2, idx);
    int _sv0t23 = mentions_result(et, ed1, ed2, ed3, ed4, _sv0t22, source, starts, ends, pp);
    if (_sv0t23) {
      return 1;
    } else {
    }
    int _sv0t24 = sv0_vec_get(ed3, idx);
    int _sv0t25 = mentions_result(et, ed1, ed2, ed3, ed4, _sv0t24, source, starts, ends, pp);
    return _sv0t25;
  } else {
  }
  if ((tag == 11)) {
    int _sv0t26 = sv0_vec_get(ed1, idx);
    int _sv0t27 = mentions_result(et, ed1, ed2, ed3, ed4, _sv0t26, source, starts, ends, pp);
    if (_sv0t27) {
      return 1;
    } else {
    }
    int _sv0t28 = sv0_vec_get(ed2, idx);
    int amf = _sv0t28;
    int _sv0t29 = sv0_vec_get(ed3, idx);
    int amc = _sv0t29;
    int ami = 0;
    while ((ami < amc)) {
      int aidx = (amf + ami);
      int _sv0t30 = sv0_vec_get(ed3, aidx);
      int grd = _sv0t30;
      if ((grd >= 0)) {
        int _sv0t31 = mentions_result(et, ed1, ed2, ed3, ed4, grd, source, starts, ends, pp);
        if (_sv0t31) {
          return 1;
        } else {
        }
      } else {
      }
      int _sv0t32 = sv0_vec_get(ed4, aidx);
      int _sv0t33 = mentions_result(et, ed1, ed2, ed3, ed4, _sv0t32, source, starts, ends, pp);
      if (_sv0t33) {
        return 1;
      } else {
      }
      ami = (ami + 1);
    }
    return 0;
  } else {
  }
  if ((tag == 12)) {
    int _sv0t34 = sv0_vec_get(ed1, idx);
    int _sv0t35 = mentions_result(et, ed1, ed2, ed3, ed4, _sv0t34, source, starts, ends, pp);
    if (_sv0t35) {
      return 1;
    } else {
    }
    int _sv0t36 = sv0_vec_get(ed3, idx);
    int invf = _sv0t36;
    int _sv0t37 = sv0_vec_get(ed4, idx);
    int invc = _sv0t37;
    int ii = 0;
    while ((ii < invc)) {
      int _sv0t38 = (invf + ii);
      int _sv0t39 = mentions_result(et, ed1, ed2, ed3, ed4, _sv0t38, source, starts, ends, pp);
      if (_sv0t39) {
        return 1;
      } else {
      }
      ii = (ii + 1);
    }
    int _sv0t40 = sv0_vec_get(ed2, idx);
    int _sv0t41 = mentions_result(et, ed1, ed2, ed3, ed4, _sv0t40, source, starts, ends, pp);
    return _sv0t41;
  } else {
  }
  if ((tag == 13)) {
    int _sv0t42 = sv0_vec_get(ed2, idx);
    int _sv0t43 = mentions_result(et, ed1, ed2, ed3, ed4, _sv0t42, source, starts, ends, pp);
    if (_sv0t43) {
      return 1;
    } else {
    }
    int _sv0t44 = sv0_vec_get(ed3, idx);
    int _sv0t45 = mentions_result(et, ed1, ed2, ed3, ed4, _sv0t44, source, starts, ends, pp);
    return _sv0t45;
  } else {
  }
  if ((tag == 14)) {
    int _sv0t46 = sv0_vec_get(ed1, idx);
    int _sv0t47 = mentions_result(et, ed1, ed2, ed3, ed4, _sv0t46, source, starts, ends, pp);
    return _sv0t47;
  } else {
  }
  if ((tag == 16)) {
    int _sv0t48 = sv0_vec_get(ed1, idx);
    int _sv0t49 = mentions_result(et, ed1, ed2, ed3, ed4, _sv0t48, source, starts, ends, pp);
    return _sv0t49;
  } else {
  }
  if ((tag == 20)) {
    int _sv0t50 = sv0_vec_get(ed1, idx);
    int _sv0t51 = mentions_result(et, ed1, ed2, ed3, ed4, _sv0t50, source, starts, ends, pp);
    return _sv0t51;
  } else {
  }
  if ((tag == 22)) {
    int _sv0t52 = sv0_vec_get(ed1, idx);
    int _sv0t53 = mentions_result(et, ed1, ed2, ed3, ed4, _sv0t52, source, starts, ends, pp);
    return _sv0t53;
  } else {
  }
  if ((tag == 24)) {
    int _sv0t54 = sv0_vec_get(ed3, idx);
    int sfc = _sv0t54;
    int sfi = 0;
    while ((sfi < sfc)) {
      int _sv0t55 = (idx - sfc);
      int _sv0t56 = (_sv0t55 + sfi);
      int _sv0t57 = mentions_result(et, ed1, ed2, ed3, ed4, _sv0t56, source, starts, ends, pp);
      if (_sv0t57) {
        return 1;
      } else {
      }
      sfi = (sfi + 1);
    }
    return 0;
  } else {
  }
  if ((tag == 25)) {
    int _sv0t58 = sv0_vec_get(ed1, idx);
    int tf = _sv0t58;
    int _sv0t59 = sv0_vec_get(ed2, idx);
    int tc = _sv0t59;
    int ti = 0;
    while ((ti < tc)) {
      int _sv0t60 = (tf + ti);
      int _sv0t61 = mentions_result(et, ed1, ed2, ed3, ed4, _sv0t60, source, starts, ends, pp);
      if (_sv0t61) {
        return 1;
      } else {
      }
      ti = (ti + 1);
    }
    return 0;
  } else {
  }
  if ((tag == 30)) {
    return 0;
  } else {
  }
  return 0;
}

static int first_result_path_handle(int et, int ed1, int ed2, int ed3, int ed4, int idx, const char* source, int starts, int ends, int pp) {
  if ((idx < 0)) {
    int _sv0t0 = (0 - 1);
    return _sv0t0;
  } else {
  }
  int _sv0t1 = sv0_vec_get(et, idx);
  int tag = _sv0t1;
  if ((tag == 1)) {
    int _sv0t2 = sv0_vec_get(ed2, idx);
    int ppc = _sv0t2;
    if ((ppc == 1)) {
      int _sv0t3 = sv0_vec_get(ed1, idx);
      int _sv0t4 = sv0_vec_get(pp, _sv0t3);
      int tok = _sv0t4;
      const char* _sv0t5 = handle_to_str(tok, source, starts, ends);
      int _sv0t6 = sv0_string_eq(_sv0t5, "result");
      if (_sv0t6) {
        return tok;
      } else {
      }
    } else {
    }
    int _sv0t7 = (0 - 1);
    return _sv0t7;
  } else {
  }
  if ((tag == 0)) {
    int _sv0t8 = (0 - 1);
    return _sv0t8;
  } else {
  }
  if ((tag == 2)) {
    int _sv0t9 = sv0_vec_get(ed2, idx);
    int _sv0t10 = first_result_path_handle(et, ed1, ed2, ed3, ed4, _sv0t9, source, starts, ends, pp);
    return _sv0t10;
  } else {
  }
  if ((tag == 3)) {
    int _sv0t11 = sv0_vec_get(ed2, idx);
    int _sv0t12 = first_result_path_handle(et, ed1, ed2, ed3, ed4, _sv0t11, source, starts, ends, pp);
    int r1 = _sv0t12;
    if ((r1 >= 0)) {
      return r1;
    } else {
    }
    int _sv0t13 = sv0_vec_get(ed3, idx);
    int _sv0t14 = first_result_path_handle(et, ed1, ed2, ed3, ed4, _sv0t13, source, starts, ends, pp);
    return _sv0t14;
  } else {
  }
  if ((tag == 4)) {
    int _sv0t15 = sv0_vec_get(ed1, idx);
    int _sv0t16 = first_result_path_handle(et, ed1, ed2, ed3, ed4, _sv0t15, source, starts, ends, pp);
    int c = _sv0t16;
    if ((c >= 0)) {
      return c;
    } else {
    }
    int _sv0t17 = sv0_vec_get(ed2, idx);
    int af = _sv0t17;
    int _sv0t18 = sv0_vec_get(ed3, idx);
    int ac = _sv0t18;
    int ai = 0;
    while ((ai < ac)) {
      int _sv0t19 = (af + ai);
      int _sv0t20 = first_result_path_handle(et, ed1, ed2, ed3, ed4, _sv0t19, source, starts, ends, pp);
      int ra = _sv0t20;
      if ((ra >= 0)) {
        return ra;
      } else {
      }
      ai = (ai + 1);
    }
    int _sv0t21 = (0 - 1);
    return _sv0t21;
  } else {
  }
  int _sv0t22 = (tag == 6);
  int _sv0t23 = (tag == 14);
  int _sv0t24 = (_sv0t22 || _sv0t23);
  int _sv0t25 = (tag == 16);
  int _sv0t26 = (_sv0t24 || _sv0t25);
  int _sv0t27 = (tag == 20);
  int _sv0t28 = (_sv0t26 || _sv0t27);
  int _sv0t29 = (tag == 22);
  if ((_sv0t28 || _sv0t29)) {
    int _sv0t30 = sv0_vec_get(ed1, idx);
    int _sv0t31 = first_result_path_handle(et, ed1, ed2, ed3, ed4, _sv0t30, source, starts, ends, pp);
    return _sv0t31;
  } else {
  }
  if ((tag == 10)) {
    int _sv0t32 = sv0_vec_get(ed1, idx);
    int _sv0t33 = first_result_path_handle(et, ed1, ed2, ed3, ed4, _sv0t32, source, starts, ends, pp);
    int rc = _sv0t33;
    if ((rc >= 0)) {
      return rc;
    } else {
    }
    int _sv0t34 = sv0_vec_get(ed2, idx);
    int _sv0t35 = first_result_path_handle(et, ed1, ed2, ed3, ed4, _sv0t34, source, starts, ends, pp);
    int rt = _sv0t35;
    if ((rt >= 0)) {
      return rt;
    } else {
    }
    int _sv0t36 = sv0_vec_get(ed3, idx);
    int _sv0t37 = first_result_path_handle(et, ed1, ed2, ed3, ed4, _sv0t36, source, starts, ends, pp);
    return _sv0t37;
  } else {
  }
  if ((tag == 11)) {
    int _sv0t38 = sv0_vec_get(ed1, idx);
    int _sv0t39 = first_result_path_handle(et, ed1, ed2, ed3, ed4, _sv0t38, source, starts, ends, pp);
    int rs = _sv0t39;
    if ((rs >= 0)) {
      return rs;
    } else {
    }
    int _sv0t40 = sv0_vec_get(ed2, idx);
    int af2 = _sv0t40;
    int _sv0t41 = sv0_vec_get(ed3, idx);
    int ac2 = _sv0t41;
    int ai2 = 0;
    while ((ai2 < ac2)) {
      int aidx = (af2 + ai2);
      int _sv0t42 = sv0_vec_get(ed3, aidx);
      int grd = _sv0t42;
      if ((grd >= 0)) {
        int _sv0t43 = first_result_path_handle(et, ed1, ed2, ed3, ed4, grd, source, starts, ends, pp);
        int rg = _sv0t43;
        if ((rg >= 0)) {
          return rg;
        } else {
        }
      } else {
      }
      int _sv0t44 = sv0_vec_get(ed4, aidx);
      int _sv0t45 = first_result_path_handle(et, ed1, ed2, ed3, ed4, _sv0t44, source, starts, ends, pp);
      int rb = _sv0t45;
      if ((rb >= 0)) {
        return rb;
      } else {
      }
      ai2 = (ai2 + 1);
    }
    int _sv0t46 = (0 - 1);
    return _sv0t46;
  } else {
  }
  if ((tag == 12)) {
    int _sv0t47 = sv0_vec_get(ed1, idx);
    int _sv0t48 = first_result_path_handle(et, ed1, ed2, ed3, ed4, _sv0t47, source, starts, ends, pp);
    int rc2 = _sv0t48;
    if ((rc2 >= 0)) {
      return rc2;
    } else {
    }
    int _sv0t49 = sv0_vec_get(ed3, idx);
    int invf = _sv0t49;
    int _sv0t50 = sv0_vec_get(ed4, idx);
    int invc = _sv0t50;
    int ii = 0;
    while ((ii < invc)) {
      int _sv0t51 = (invf + ii);
      int _sv0t52 = first_result_path_handle(et, ed1, ed2, ed3, ed4, _sv0t51, source, starts, ends, pp);
      int ri = _sv0t52;
      if ((ri >= 0)) {
        return ri;
      } else {
      }
      ii = (ii + 1);
    }
    int _sv0t53 = sv0_vec_get(ed2, idx);
    int _sv0t54 = first_result_path_handle(et, ed1, ed2, ed3, ed4, _sv0t53, source, starts, ends, pp);
    return _sv0t54;
  } else {
  }
  if ((tag == 13)) {
    int _sv0t55 = sv0_vec_get(ed2, idx);
    int _sv0t56 = first_result_path_handle(et, ed1, ed2, ed3, ed4, _sv0t55, source, starts, ends, pp);
    int ri1 = _sv0t56;
    if ((ri1 >= 0)) {
      return ri1;
    } else {
    }
    int _sv0t57 = sv0_vec_get(ed3, idx);
    int _sv0t58 = first_result_path_handle(et, ed1, ed2, ed3, ed4, _sv0t57, source, starts, ends, pp);
    return _sv0t58;
  } else {
  }
  if ((tag == 24)) {
    int _sv0t59 = sv0_vec_get(ed3, idx);
    int sfc = _sv0t59;
    int sfi = 0;
    while ((sfi < sfc)) {
      int _sv0t60 = (idx - sfc);
      int _sv0t61 = (_sv0t60 + sfi);
      int _sv0t62 = first_result_path_handle(et, ed1, ed2, ed3, ed4, _sv0t61, source, starts, ends, pp);
      int rr = _sv0t62;
      if ((rr >= 0)) {
        return rr;
      } else {
      }
      sfi = (sfi + 1);
    }
    int _sv0t63 = (0 - 1);
    return _sv0t63;
  } else {
  }
  if ((tag == 25)) {
    int _sv0t64 = sv0_vec_get(ed1, idx);
    int tf = _sv0t64;
    int _sv0t65 = sv0_vec_get(ed2, idx);
    int tc = _sv0t65;
    int ti = 0;
    while ((ti < tc)) {
      int _sv0t66 = (tf + ti);
      int _sv0t67 = first_result_path_handle(et, ed1, ed2, ed3, ed4, _sv0t66, source, starts, ends, pp);
      int rr2 = _sv0t67;
      if ((rr2 >= 0)) {
        return rr2;
      } else {
      }
      ti = (ti + 1);
    }
    int _sv0t68 = (0 - 1);
    return _sv0t68;
  } else {
  }
  int _sv0t69 = (0 - 1);
  return _sv0t69;
}

static int all_old_names_expr(int et, int ed1, int ed2, int ed3, int ed4, int idx, const char* source, int starts, int ends, int pp, int out) {
  if ((idx < 0)) {
    return 0;
  } else {
  }
  int _sv0t0 = sv0_vec_get(et, idx);
  int tag = _sv0t0;
  if ((tag == 0)) {
    return 0;
  } else {
  }
  if ((tag == 1)) {
    return 0;
  } else {
  }
  if ((tag == 4)) {
    int _sv0t1 = sv0_vec_get(ed1, idx);
    int callee = _sv0t1;
    int _sv0t2 = sv0_vec_get(ed3, idx);
    int ac = _sv0t2;
    if ((ac == 1)) {
      int _sv0t3 = sv0_vec_get(et, callee);
      int ct = _sv0t3;
      if ((ct == 1)) {
        int _sv0t4 = sv0_vec_get(ed2, callee);
        int cppc = _sv0t4;
        if ((cppc == 1)) {
          int _sv0t5 = sv0_vec_get(ed1, callee);
          int _sv0t6 = sv0_vec_get(pp, _sv0t5);
          int ctok = _sv0t6;
          const char* _sv0t7 = handle_to_str(ctok, source, starts, ends);
          int _sv0t8 = sv0_string_eq(_sv0t7, "old");
          if (_sv0t8) {
            int _sv0t9 = sv0_vec_get(ed2, idx);
            int arg0 = _sv0t9;
            int _sv0t10 = sv0_vec_get(et, arg0);
            int at = _sv0t10;
            if ((at == 1)) {
              int _sv0t11 = sv0_vec_get(ed2, arg0);
              int appc = _sv0t11;
              if ((appc == 1)) {
                int _sv0t12 = sv0_vec_get(ed1, arg0);
                int _sv0t13 = sv0_vec_get(pp, _sv0t12);
                int ntok = _sv0t13;
                sv0_vec_push(out, ntok);
                return 1;
              } else {
              }
            } else {
            }
          } else {
          }
        } else {
        }
      } else {
      }
    } else {
    }
    int _sv0t14 = all_old_names_expr(et, ed1, ed2, ed3, ed4, callee, source, starts, ends, pp, out);
    int _sv0t15 = sv0_vec_get(ed2, idx);
    int af = _sv0t15;
    int ai = 0;
    while ((ai < ac)) {
      int _sv0t16 = (af + ai);
      int _sv0t17 = all_old_names_expr(et, ed1, ed2, ed3, ed4, _sv0t16, source, starts, ends, pp, out);
      ai = (ai + 1);
    }
    return 0;
  } else {
  }
  if ((tag == 2)) {
    int _sv0t18 = sv0_vec_get(ed2, idx);
    int _sv0t19 = all_old_names_expr(et, ed1, ed2, ed3, ed4, _sv0t18, source, starts, ends, pp, out);
    return 0;
  } else {
  }
  if ((tag == 3)) {
    int _sv0t20 = sv0_vec_get(ed2, idx);
    int _sv0t21 = all_old_names_expr(et, ed1, ed2, ed3, ed4, _sv0t20, source, starts, ends, pp, out);
    int _sv0t22 = sv0_vec_get(ed3, idx);
    int _sv0t23 = all_old_names_expr(et, ed1, ed2, ed3, ed4, _sv0t22, source, starts, ends, pp, out);
    return 0;
  } else {
  }
  if ((tag == 6)) {
    int _sv0t24 = sv0_vec_get(ed1, idx);
    int _sv0t25 = all_old_names_expr(et, ed1, ed2, ed3, ed4, _sv0t24, source, starts, ends, pp, out);
    return 0;
  } else {
  }
  if ((tag == 10)) {
    int _sv0t26 = sv0_vec_get(ed1, idx);
    int _sv0t27 = all_old_names_expr(et, ed1, ed2, ed3, ed4, _sv0t26, source, starts, ends, pp, out);
    int _sv0t28 = sv0_vec_get(ed2, idx);
    int _sv0t29 = all_old_names_expr(et, ed1, ed2, ed3, ed4, _sv0t28, source, starts, ends, pp, out);
    int _sv0t30 = sv0_vec_get(ed3, idx);
    int _sv0t31 = all_old_names_expr(et, ed1, ed2, ed3, ed4, _sv0t30, source, starts, ends, pp, out);
    return 0;
  } else {
  }
  if ((tag == 11)) {
    int _sv0t32 = sv0_vec_get(ed1, idx);
    int _sv0t33 = all_old_names_expr(et, ed1, ed2, ed3, ed4, _sv0t32, source, starts, ends, pp, out);
    int _sv0t34 = sv0_vec_get(ed2, idx);
    int amf = _sv0t34;
    int _sv0t35 = sv0_vec_get(ed3, idx);
    int amc = _sv0t35;
    int ami = 0;
    while ((ami < amc)) {
      int aidx = (amf + ami);
      int _sv0t36 = sv0_vec_get(ed3, aidx);
      int grd = _sv0t36;
      if ((grd >= 0)) {
        int _sv0t37 = all_old_names_expr(et, ed1, ed2, ed3, ed4, grd, source, starts, ends, pp, out);
      } else {
      }
      int _sv0t38 = sv0_vec_get(ed4, aidx);
      int _sv0t39 = all_old_names_expr(et, ed1, ed2, ed3, ed4, _sv0t38, source, starts, ends, pp, out);
      ami = (ami + 1);
    }
    return 0;
  } else {
  }
  if ((tag == 12)) {
    int _sv0t40 = sv0_vec_get(ed1, idx);
    int _sv0t41 = all_old_names_expr(et, ed1, ed2, ed3, ed4, _sv0t40, source, starts, ends, pp, out);
    int _sv0t42 = sv0_vec_get(ed3, idx);
    int invf = _sv0t42;
    int _sv0t43 = sv0_vec_get(ed4, idx);
    int invc = _sv0t43;
    int ii = 0;
    while ((ii < invc)) {
      int _sv0t44 = (invf + ii);
      int _sv0t45 = all_old_names_expr(et, ed1, ed2, ed3, ed4, _sv0t44, source, starts, ends, pp, out);
      ii = (ii + 1);
    }
    int _sv0t46 = sv0_vec_get(ed2, idx);
    int _sv0t47 = all_old_names_expr(et, ed1, ed2, ed3, ed4, _sv0t46, source, starts, ends, pp, out);
    return 0;
  } else {
  }
  if ((tag == 13)) {
    int _sv0t48 = sv0_vec_get(ed2, idx);
    int _sv0t49 = all_old_names_expr(et, ed1, ed2, ed3, ed4, _sv0t48, source, starts, ends, pp, out);
    int _sv0t50 = sv0_vec_get(ed3, idx);
    int _sv0t51 = all_old_names_expr(et, ed1, ed2, ed3, ed4, _sv0t50, source, starts, ends, pp, out);
    return 0;
  } else {
  }
  if ((tag == 14)) {
    int _sv0t52 = sv0_vec_get(ed1, idx);
    int _sv0t53 = all_old_names_expr(et, ed1, ed2, ed3, ed4, _sv0t52, source, starts, ends, pp, out);
    return 0;
  } else {
  }
  if ((tag == 20)) {
    int _sv0t54 = sv0_vec_get(ed1, idx);
    int _sv0t55 = all_old_names_expr(et, ed1, ed2, ed3, ed4, _sv0t54, source, starts, ends, pp, out);
    return 0;
  } else {
  }
  if ((tag == 21)) {
    int _sv0t56 = sv0_vec_get(ed1, idx);
    int _sv0t57 = all_old_names_expr(et, ed1, ed2, ed3, ed4, _sv0t56, source, starts, ends, pp, out);
    int _sv0t58 = sv0_vec_get(ed2, idx);
    int _sv0t59 = all_old_names_expr(et, ed1, ed2, ed3, ed4, _sv0t58, source, starts, ends, pp, out);
    return 0;
  } else {
  }
  if ((tag == 22)) {
    int _sv0t60 = sv0_vec_get(ed1, idx);
    int _sv0t61 = all_old_names_expr(et, ed1, ed2, ed3, ed4, _sv0t60, source, starts, ends, pp, out);
    return 0;
  } else {
  }
  if ((tag == 24)) {
    int _sv0t62 = sv0_vec_get(ed3, idx);
    int sfc = _sv0t62;
    int sfi = 0;
    while ((sfi < sfc)) {
      int _sv0t63 = (idx - sfc);
      int _sv0t64 = (_sv0t63 + sfi);
      int _sv0t65 = all_old_names_expr(et, ed1, ed2, ed3, ed4, _sv0t64, source, starts, ends, pp, out);
      sfi = (sfi + 1);
    }
    return 0;
  } else {
  }
  if ((tag == 25)) {
    int _sv0t66 = sv0_vec_get(ed1, idx);
    int tf = _sv0t66;
    int _sv0t67 = sv0_vec_get(ed2, idx);
    int tc = _sv0t67;
    int ti = 0;
    while ((ti < tc)) {
      int _sv0t68 = (tf + ti);
      int _sv0t69 = all_old_names_expr(et, ed1, ed2, ed3, ed4, _sv0t68, source, starts, ends, pp, out);
      ti = (ti + 1);
    }
    return 0;
  } else {
  }
  if ((tag == 9)) {
    int _sv0t70 = sv0_vec_get(ed1, idx);
    int sf = _sv0t70;
    int _sv0t71 = sv0_vec_get(ed2, idx);
    int sc = _sv0t71;
    int si = 0;
    while ((si < sc)) {
      int sidx = (sf + si);
      int _sv0t72 = sv0_vec_get(et, sidx);
      int stag = _sv0t72;
      if ((stag == 27)) {
        int _sv0t73 = sv0_vec_get(ed3, sidx);
        int ini = _sv0t73;
        if ((ini >= 0)) {
          int _sv0t74 = all_old_names_expr(et, ed1, ed2, ed3, ed4, ini, source, starts, ends, pp, out);
        } else {
        }
      } else {
      }
      if ((stag == 28)) {
        int _sv0t75 = sv0_vec_get(ed1, sidx);
        int _sv0t76 = all_old_names_expr(et, ed1, ed2, ed3, ed4, _sv0t75, source, starts, ends, pp, out);
      } else {
      }
      si = (si + 1);
    }
    int _sv0t77 = sv0_vec_get(ed3, idx);
    int tail = _sv0t77;
    if ((tail >= 0)) {
      int _sv0t78 = all_old_names_expr(et, ed1, ed2, ed3, ed4, tail, source, starts, ends, pp, out);
    } else {
    }
    return 0;
  } else {
  }
  if ((tag == 30)) {
    return 0;
  } else {
  }
  return 0;
}

static int expr_init_cty(int et, int ed1, int ed2, int ed3, int ed4, int idx, int pp, int fn_names, int fn_ret_ctys, int aliases) {
  if ((idx < 0)) {
    int _sv0t0 = (0 - 1);
    return _sv0t0;
  } else {
  }
  int _sv0t1 = sv0_vec_get(et, idx);
  int tag = _sv0t1;
  if ((tag == 24)) {
    int _sv0t2 = sv0_vec_get(ed1, idx);
    int pps = _sv0t2;
    int _sv0t3 = sv0_vec_get(pp, pps);
    return _sv0t3;
  } else {
  }
  if ((tag == 1)) {
    int _sv0t4 = sv0_vec_get(ed1, idx);
    int pps2 = _sv0t4;
    int _sv0t5 = sv0_vec_get(ed2, idx);
    int ppc2 = _sv0t5;
    if ((ppc2 == 2)) {
      int _sv0t6 = sv0_vec_get(pp, pps2);
      return _sv0t6;
    } else {
    }
    int _sv0t7 = (0 - 1);
    return _sv0t7;
  } else {
  }
  if ((tag == 4)) {
    int _sv0t8 = sv0_vec_get(ed1, idx);
    int callee = _sv0t8;
    if ((callee >= 0)) {
      int _sv0t9 = sv0_vec_get(et, callee);
      int ct = _sv0t9;
      if ((ct == 1)) {
        int _sv0t10 = sv0_vec_get(ed1, callee);
        int cpps = _sv0t10;
        int _sv0t11 = sv0_vec_get(ed2, callee);
        int cppc = _sv0t11;
        if ((cppc == 1)) {
          int _sv0t12 = sv0_vec_get(pp, cpps);
          int ftok = _sv0t12;
          int _sv0t13 = callee_fn_index(fn_names, aliases, ftok);
          int fi = _sv0t13;
          if ((fi >= 0)) {
            int _sv0t14 = sv0_vec_get(fn_ret_ctys, fi);
            return _sv0t14;
          } else {
          }
          int _sv0t15 = (0 - 1);
          return _sv0t15;
        } else {
        }
        if ((cppc == 2)) {
          int _sv0t16 = sv0_vec_get(pp, cpps);
          return _sv0t16;
        } else {
        }
      } else {
      }
    } else {
    }
    int _sv0t17 = (0 - 1);
    return _sv0t17;
  } else {
  }
  if ((tag == 25)) {
    int _sv0t18 = sv0_vec_get(ed2, idx);
    int tc = _sv0t18;
    if ((tc == 1)) {
      int _sv0t19 = sv0_vec_get(ed1, idx);
      int first = _sv0t19;
      int _sv0t20 = expr_init_cty(et, ed1, ed2, ed3, ed4, first, pp, fn_names, fn_ret_ctys, aliases);
      return _sv0t20;
    } else {
    }
  } else {
  }
  int _sv0t21 = (0 - 1);
  return _sv0t21;
}

static int scan_lets(int et, int ed1, int ed2, int ed3, int ed4, int block_idx, int pp, int fn_names, int fn_ret_ctys, int aliases, int out_names, int out_ctys) {
  if ((block_idx < 0)) {
    return 0;
  } else {
  }
  int _sv0t0 = sv0_vec_get(et, block_idx);
  int btag = _sv0t0;
  if ((btag != 9)) {
    return 0;
  } else {
  }
  int _sv0t1 = sv0_vec_get(ed1, block_idx);
  int sf = _sv0t1;
  int _sv0t2 = sv0_vec_get(ed2, block_idx);
  int sc = _sv0t2;
  int count = 0;
  int i = 0;
  while ((i < sc)) {
    int sidx = (sf + i);
    int _sv0t3 = sv0_vec_get(et, sidx);
    int stag = _sv0t3;
    if ((stag == 27)) {
      int _sv0t4 = sv0_vec_get(ed3, sidx);
      int init = _sv0t4;
      if ((init >= 0)) {
        int _sv0t5 = sv0_vec_get(ed1, sidx);
        int nh = _sv0t5;
        int _sv0t6 = expr_init_cty(et, ed1, ed2, ed3, ed4, init, pp, fn_names, fn_ret_ctys, aliases);
        int th = _sv0t6;
        sv0_vec_push(out_names, nh);
        sv0_vec_push(out_ctys, th);
        count = (count + 1);
      } else {
      }
    } else {
    }
    i = (i + 1);
  }
  return count;
}

static int scrut_locals_push(int scrut_names, int scrut_types, int extra_names, int extra_types) {
  int _sv0t0 = sv0_vec_len(scrut_names);
  int saved = _sv0t0;
  int _sv0t1 = sv0_vec_len(extra_names);
  int en = _sv0t1;
  int i = 0;
  while ((i < en)) {
    int _sv0t2 = sv0_vec_get(extra_names, i);
    sv0_vec_push(scrut_names, _sv0t2);
    int _sv0t3 = sv0_vec_get(extra_types, i);
    sv0_vec_push(scrut_types, _sv0t3);
    i = (i + 1);
  }
  return saved;
}

static int scrut_local_lookup_bounded(int scrut_names, int limit, int name_h) {
  int i = 0;
  while ((i < limit)) {
    int _sv0t0 = sv0_vec_get(scrut_names, i);
    if ((_sv0t0 == name_h)) {
      return i;
    } else {
    }
    i = (i + 1);
  }
  int _sv0t1 = (0 - 1);
  return _sv0t1;
}

static int lower_alias_replace(int target, int from_handles, int to_handles) {
  int _sv0t0 = sv0_vec_len(from_handles);
  int n = _sv0t0;
  int i = 0;
  while ((i < n)) {
    int _sv0t1 = sv0_vec_get(from_handles, i);
    sv0_vec_push(target, _sv0t1);
    int _sv0t2 = sv0_vec_get(to_handles, i);
    sv0_vec_push(target, _sv0t2);
    i = (i + 1);
  }
  return n;
}

static int match_scrut_cty(int et, int ed1, int ed2, int ed3, int ed4, int idx, int pp, int match_env, int aliases, int scrut_names, int scrut_ctys, int scrut_limit) {
  if ((idx < 0)) {
    int _sv0t0 = (0 - 1);
    return _sv0t0;
  } else {
  }
  int _sv0t1 = sv0_vec_get(et, idx);
  int tag = _sv0t1;
  if ((tag == 0)) {
    int _sv0t2 = (0 - 1);
    return _sv0t2;
  } else {
  }
  if ((tag == 24)) {
    int _sv0t3 = sv0_vec_get(ed1, idx);
    int pps24 = _sv0t3;
    int _sv0t4 = sv0_vec_get(pp, pps24);
    return _sv0t4;
  } else {
  }
  if ((tag == 1)) {
    int _sv0t5 = sv0_vec_get(ed1, idx);
    int pps = _sv0t5;
    int _sv0t6 = sv0_vec_get(ed2, idx);
    int ppc = _sv0t6;
    if ((ppc == 2)) {
      int _sv0t7 = sv0_vec_get(pp, pps);
      return _sv0t7;
    } else {
    }
    if ((ppc == 1)) {
      int _sv0t8 = sv0_vec_get(pp, pps);
      int name_h = _sv0t8;
      int _sv0t9 = scrut_local_lookup_bounded(scrut_names, scrut_limit, name_h);
      int si = _sv0t9;
      if ((si >= 0)) {
        int _sv0t10 = sv0_vec_get(scrut_ctys, si);
        return _sv0t10;
      } else {
      }
      int lim_e = 0;
      int _sv0t11 = lower_match_env_is_packed_v0(match_env);
      if ((_sv0t11 == 1)) {
        int _sv0t12 = sv0_vec_get(match_env, 10);
        lim_e = _sv0t12;
      } else {
      }
      if ((scrut_limit > 0)) {
        if ((lim_e > scrut_limit)) {
          lim_e = scrut_limit;
        } else {
        }
      } else {
      }
      if ((lim_e > 0)) {
        int _sv0t13 = lower_match_env_scrut_ty_lookup(match_env, lim_e, name_h);
        int te = _sv0t13;
        int _sv0t14 = (0 - 2);
        if ((te != _sv0t14)) {
          return te;
        } else {
        }
      } else {
      }
      int _sv0t15 = sv0_vec_get(match_env, 7);
      int pc = _sv0t15;
      if ((pc > 0)) {
        int _sv0t16 = sv0_vec_get(match_env, 5);
        int bn = _sv0t16;
        int _sv0t17 = sv0_vec_get(match_env, 6);
        int bt = _sv0t17;
        if ((bn >= 0)) {
          if ((bt >= 0)) {
            int pj = 0;
            while ((pj < pc)) {
              int _sv0t18 = (bn + pj);
              int _sv0t19 = sv0_vec_get(match_env, _sv0t18);
              if ((_sv0t19 == name_h)) {
                int _sv0t20 = (bt + pj);
                int _sv0t21 = sv0_vec_get(match_env, _sv0t20);
                return _sv0t21;
              } else {
              }
              pj = (pj + 1);
            }
          } else {
          }
        } else {
        }
      } else {
      }
      int _sv0t22 = (0 - 1);
      return _sv0t22;
    } else {
    }
    int _sv0t23 = (0 - 1);
    return _sv0t23;
  } else {
  }
  if ((tag == 4)) {
    int _sv0t24 = sv0_vec_get(ed1, idx);
    int callee = _sv0t24;
    if ((callee >= 0)) {
      int _sv0t25 = sv0_vec_get(et, callee);
      int ct = _sv0t25;
      if ((ct == 1)) {
        int _sv0t26 = sv0_vec_get(ed1, callee);
        int cpps = _sv0t26;
        int _sv0t27 = sv0_vec_get(ed2, callee);
        int cppc = _sv0t27;
        if ((cppc == 2)) {
          int _sv0t28 = sv0_vec_get(pp, cpps);
          return _sv0t28;
        } else {
        }
        if ((cppc == 1)) {
          int _sv0t29 = sv0_vec_get(pp, cpps);
          int ftok = _sv0t29;
          int _sv0t30 = callee_fn_index_match_env(match_env, aliases, ftok);
          int fi = _sv0t30;
          if ((fi >= 0)) {
            int _sv0t31 = sv0_vec_get(match_env, 3);
            int brt = _sv0t31;
            int _sv0t32 = (brt + fi);
            int _sv0t33 = sv0_vec_get(match_env, _sv0t32);
            return _sv0t33;
          } else {
          }
          int _sv0t34 = (0 - 1);
          return _sv0t34;
        } else {
        }
      } else {
      }
    } else {
    }
    int _sv0t35 = (0 - 1);
    return _sv0t35;
  } else {
  }
  if ((tag == 25)) {
    int _sv0t36 = sv0_vec_get(ed2, idx);
    int tc = _sv0t36;
    if ((tc == 1)) {
      int _sv0t37 = sv0_vec_get(ed1, idx);
      int first = _sv0t37;
      int _sv0t38 = match_scrut_cty(et, ed1, ed2, ed3, ed4, first, pp, match_env, aliases, scrut_names, scrut_ctys, scrut_limit);
      return _sv0t38;
    } else {
    }
  } else {
  }
  int _sv0t39 = (0 - 1);
  return _sv0t39;
}

static int variant_shape_in_items(int item_tags, int item_names, int item_field_counts, int variant_shapes_flat, int variant_fcs_flat, int enum_name, int variant_idx, int out_count) {
  int _sv0t0 = sv0_vec_len(item_tags);
  int n = _sv0t0;
  int i = 0;
  int vn_offset = 0;
  while ((i < n)) {
    int _sv0t1 = sv0_vec_get(item_tags, i);
    int tag = _sv0t1;
    int _sv0t2 = sv0_vec_get(item_field_counts, i);
    int vc = _sv0t2;
    if ((tag == 2)) {
      int _sv0t3 = sv0_vec_get(item_names, i);
      if ((_sv0t3 == enum_name)) {
        if ((variant_idx >= 0)) {
          if ((variant_idx < vc)) {
            int _sv0t4 = (vn_offset + variant_idx);
            int _sv0t5 = sv0_vec_get(variant_shapes_flat, _sv0t4);
            int shape = _sv0t5;
            int _sv0t6 = (vn_offset + variant_idx);
            int _sv0t7 = sv0_vec_get(variant_fcs_flat, _sv0t6);
            int fc = _sv0t7;
            sv0_vec_push(out_count, fc);
            return shape;
          } else {
          }
        } else {
        }
        int _sv0t8 = (0 - 1);
        return _sv0t8;
      } else {
      }
      vn_offset = (vn_offset + vc);
    } else {
    }
    i = (i + 1);
  }
  int _sv0t9 = (0 - 1);
  return _sv0t9;
}

static int ctr_fresh(int ctr) {
  int _sv0t0 = sv0_vec_get(ctr, 0);
  int n = _sv0t0;
  int _sv0t1 = (n + 1);
  sv0_vec_set(ctr, 0, _sv0t1);
  return n;
}

static int classify_assign_lhs_arena(int et, int ed1, int ed2, int pp, int idx, int out) {
  int _sv0t0 = sv0_vec_get(et, idx);
  int tag = _sv0t0;
  if ((tag == 1)) {
    int _sv0t1 = sv0_vec_get(ed1, idx);
    int pps = _sv0t1;
    int _sv0t2 = sv0_vec_get(ed2, idx);
    int ppc = _sv0t2;
    if ((ppc == 1)) {
      int _sv0t3 = sv0_vec_get(pp, pps);
      sv0_vec_push(out, _sv0t3);
      return 0;
    } else {
    }
    int _sv0t4 = (0 - 1);
    return _sv0t4;
  } else {
  }
  if ((tag == 6)) {
    int _sv0t5 = sv0_vec_get(ed1, idx);
    int obj_idx = _sv0t5;
    int _sv0t6 = sv0_vec_get(ed2, idx);
    int field_tok = _sv0t6;
    int _sv0t7 = sv0_vec_get(et, obj_idx);
    int obj_tag = _sv0t7;
    if ((obj_tag == 1)) {
      int _sv0t8 = sv0_vec_get(ed1, obj_idx);
      int opps = _sv0t8;
      int _sv0t9 = sv0_vec_get(ed2, obj_idx);
      int oppc = _sv0t9;
      if ((oppc == 1)) {
        int _sv0t10 = sv0_vec_get(pp, opps);
        sv0_vec_push(out, _sv0t10);
        sv0_vec_push(out, field_tok);
        return 1;
      } else {
      }
    } else {
    }
    int _sv0t11 = (0 - 1);
    return _sv0t11;
  } else {
  }
  int _sv0t12 = (0 - 1);
  return _sv0t12;
}

static int test_binop_to_c(void) {
  const char* _sv0t0 = ast_binop_to_c(0);
  int _sv0t1 = sv0_string_eq(_sv0t0, "+");
  if ((_sv0t1 != 1)) {
    return 1;
  } else {
  }
  const char* _sv0t2 = ast_binop_to_c(1);
  int _sv0t3 = sv0_string_eq(_sv0t2, "-");
  if ((_sv0t3 != 1)) {
    return 2;
  } else {
  }
  const char* _sv0t4 = ast_binop_to_c(2);
  int _sv0t5 = sv0_string_eq(_sv0t4, "*");
  if ((_sv0t5 != 1)) {
    return 3;
  } else {
  }
  const char* _sv0t6 = ast_binop_to_c(3);
  int _sv0t7 = sv0_string_eq(_sv0t6, "/");
  if ((_sv0t7 != 1)) {
    return 4;
  } else {
  }
  const char* _sv0t8 = ast_binop_to_c(4);
  int _sv0t9 = sv0_string_eq(_sv0t8, "%");
  if ((_sv0t9 != 1)) {
    return 5;
  } else {
  }
  const char* _sv0t10 = ast_binop_to_c(5);
  int _sv0t11 = sv0_string_eq(_sv0t10, "==");
  if ((_sv0t11 != 1)) {
    return 6;
  } else {
  }
  const char* _sv0t12 = ast_binop_to_c(11);
  int _sv0t13 = sv0_string_eq(_sv0t12, "&&");
  if ((_sv0t13 != 1)) {
    return 7;
  } else {
  }
  const char* _sv0t14 = ast_binop_to_c(12);
  int _sv0t15 = sv0_string_eq(_sv0t14, "||");
  if ((_sv0t15 != 1)) {
    return 8;
  } else {
  }
  const char* _sv0t16 = ast_binop_to_c(13);
  int _sv0t17 = sv0_string_eq(_sv0t16, "&");
  if ((_sv0t17 != 1)) {
    return 9;
  } else {
  }
  const char* _sv0t18 = ast_binop_to_c(16);
  int _sv0t19 = sv0_string_eq(_sv0t18, "<<");
  if ((_sv0t19 != 1)) {
    return 10;
  } else {
  }
  const char* _sv0t20 = ast_binop_to_c(17);
  int _sv0t21 = sv0_string_eq(_sv0t20, ">>");
  if ((_sv0t21 != 1)) {
    return 11;
  } else {
  }
  return 0;
}

static int test_unop_to_c(void) {
  const char* _sv0t0 = ast_unop_to_c(0);
  int _sv0t1 = sv0_string_eq(_sv0t0, "-");
  if ((_sv0t1 != 1)) {
    return 1;
  } else {
  }
  const char* _sv0t2 = ast_unop_to_c(1);
  int _sv0t3 = sv0_string_eq(_sv0t2, "!");
  if ((_sv0t3 != 1)) {
    return 2;
  } else {
  }
  const char* _sv0t4 = ast_unop_to_c(2);
  int _sv0t5 = sv0_string_eq(_sv0t4, "~");
  if ((_sv0t5 != 1)) {
    return 3;
  } else {
  }
  const char* _sv0t6 = ast_unop_to_c(3);
  int _sv0t7 = sv0_string_eq(_sv0t6, "*");
  if ((_sv0t7 != 1)) {
    return 4;
  } else {
  }
  const char* _sv0t8 = ast_unop_to_c(4);
  int _sv0t9 = sv0_string_eq(_sv0t8, "&");
  if ((_sv0t9 != 1)) {
    return 5;
  } else {
  }
  const char* _sv0t10 = ast_unop_to_c(5);
  int _sv0t11 = sv0_string_eq(_sv0t10, "&");
  if ((_sv0t11 != 1)) {
    return 6;
  } else {
  }
  int _sv0t12 = is_supported_unop(0);
  if ((_sv0t12 != 1)) {
    return 7;
  } else {
  }
  int _sv0t13 = is_supported_unop(1);
  if ((_sv0t13 != 1)) {
    return 8;
  } else {
  }
  int _sv0t14 = is_supported_unop(2);
  if ((_sv0t14 != 1)) {
    return 9;
  } else {
  }
  int _sv0t15 = is_supported_unop(4);
  if ((_sv0t15 != 1)) {
    return 10;
  } else {
  }
  int _sv0t16 = is_supported_unop(5);
  if ((_sv0t16 != 1)) {
    return 11;
  } else {
  }
  int _sv0t17 = is_supported_unop(3);
  if ((_sv0t17 != 0)) {
    return 12;
  } else {
  }
  return 0;
}

static int test_ty_to_c(void) {
  const char* _sv0t0 = ast_ty_to_c_string("i32");
  int _sv0t1 = sv0_string_eq(_sv0t0, "int");
  if ((_sv0t1 != 1)) {
    return 1;
  } else {
  }
  const char* _sv0t2 = ast_ty_to_c_string("bool");
  int _sv0t3 = sv0_string_eq(_sv0t2, "int");
  if ((_sv0t3 != 1)) {
    return 2;
  } else {
  }
  const char* _sv0t4 = ast_ty_to_c_string("unit");
  int _sv0t5 = sv0_string_eq(_sv0t4, "void");
  if ((_sv0t5 != 1)) {
    return 3;
  } else {
  }
  const char* _sv0t6 = ast_ty_to_c_string("i64");
  int _sv0t7 = sv0_string_eq(_sv0t6, "int64_t");
  if ((_sv0t7 != 1)) {
    return 4;
  } else {
  }
  const char* _sv0t8 = ast_ty_to_c_string("string");
  int _sv0t9 = sv0_string_eq(_sv0t8, "const char*");
  if ((_sv0t9 != 1)) {
    return 5;
  } else {
  }
  const char* _sv0t10 = ast_ty_to_c_string("String");
  int _sv0t11 = sv0_string_eq(_sv0t10, "const char*");
  if ((_sv0t11 != 1)) {
    return 6;
  } else {
  }
  const char* _sv0t12 = ast_ty_to_c_string("Vec");
  int _sv0t13 = sv0_string_eq(_sv0t12, "int");
  if ((_sv0t13 != 1)) {
    return 7;
  } else {
  }
  const char* _sv0t14 = ast_ty_to_c_string("Box");
  int _sv0t15 = sv0_string_eq(_sv0t14, "int");
  if ((_sv0t15 != 1)) {
    return 8;
  } else {
  }
  const char* _sv0t16 = ast_ty_to_c_string("u8");
  int _sv0t17 = sv0_string_eq(_sv0t16, "uint8_t");
  if ((_sv0t17 != 1)) {
    return 9;
  } else {
  }
  const char* _sv0t18 = ast_ty_to_c_string("isize");
  int _sv0t19 = sv0_string_eq(_sv0t18, "intptr_t");
  if ((_sv0t19 != 1)) {
    return 10;
  } else {
  }
  int _sv0t20 = is_void_cty("void");
  if ((_sv0t20 != 1)) {
    return 11;
  } else {
  }
  int _sv0t21 = is_void_cty("unit");
  if ((_sv0t21 != 1)) {
    return 12;
  } else {
  }
  int _sv0t22 = is_void_cty("int");
  if ((_sv0t22 != 0)) {
    return 13;
  } else {
  }
  return 0;
}

static int test_old_slot(void) {
  const char* _sv0t0 = old_slot_prefix();
  int _sv0t1 = sv0_string_eq(_sv0t0, "_sv0old_");
  if ((_sv0t1 != 1)) {
    return 1;
  } else {
  }
  int _sv0t2 = old_slot_name_len(3);
  if ((_sv0t2 != 11)) {
    return 2;
  } else {
  }
  const char* _sv0t3 = old_slot_name("x");
  int _sv0t4 = sv0_string_eq(_sv0t3, "_sv0old_x");
  if ((_sv0t4 != 1)) {
    return 3;
  } else {
  }
  const char* _sv0t5 = old_slot_name("count");
  int _sv0t6 = sv0_string_eq(_sv0t5, "_sv0old_count");
  if ((_sv0t6 != 1)) {
    return 4;
  } else {
  }
  return 0;
}

static int test_ret_syntax(void) {
  int _sv0t0 = ret_syntax_is_unit(0, "");
  if ((_sv0t0 != 1)) {
    return 1;
  } else {
  }
  int _sv0t1 = ret_syntax_is_unit(1, "unit");
  if ((_sv0t1 != 1)) {
    return 2;
  } else {
  }
  int _sv0t2 = ret_syntax_is_unit(1, "i32");
  if ((_sv0t2 != 0)) {
    return 3;
  } else {
  }
  return 0;
}

static int test_store_value_class(void) {
  int _sv0t0 = store_value_class(1, 1);
  if ((_sv0t0 != 0)) {
    return 1;
  } else {
  }
  int _sv0t1 = store_value_class(1, 0);
  if ((_sv0t1 != 2)) {
    return 2;
  } else {
  }
  int _sv0t2 = store_value_class(0, 0);
  if ((_sv0t2 != 1)) {
    return 3;
  } else {
  }
  return 0;
}

static int test_store_value_to_slot(void) {
  int _sv0t0 = sv0_vec_new();
  int out0 = _sv0t0;
  Value _sv0t1;
  _sv0t1.tag = 0;
  _sv0t1.p0 = 9;
  int _sv0t2 = store_value_to_slot(7, _sv0t1, out0);
  int rc0 = _sv0t2;
  if ((rc0 != 0)) {
    return 1;
  } else {
  }
  int _sv0t3 = sv0_vec_len(out0);
  if ((_sv0t3 != 1)) {
    return 2;
  } else {
  }
  int _sv0t4 = sv0_vec_new();
  int out1 = _sv0t4;
  Value _sv0t5;
  _sv0t5.tag = 3;
  _sv0t5.p0 = 12;
  int _sv0t6 = store_value_to_slot(12, _sv0t5, out1);
  int rc1 = _sv0t6;
  if ((rc1 != 0)) {
    return 3;
  } else {
  }
  int _sv0t7 = sv0_vec_len(out1);
  if ((_sv0t7 != 0)) {
    return 4;
  } else {
  }
  int _sv0t8 = sv0_vec_new();
  int out2 = _sv0t8;
  Value _sv0t9;
  _sv0t9.tag = 3;
  _sv0t9.p0 = 13;
  int _sv0t10 = store_value_to_slot(12, _sv0t9, out2);
  int rc2 = _sv0t10;
  if ((rc2 != 0)) {
    return 5;
  } else {
  }
  int _sv0t11 = sv0_vec_len(out2);
  if ((_sv0t11 != 1)) {
    return 6;
  } else {
  }
  return 0;
}

static int test_inject_ensures_and_ret_slot(void) {
  int _sv0t0 = sv0_vec_new();
  int body0 = _sv0t0;
  Instr _sv0t1;
  Value _sv0t2;
  _sv0t2.tag = 0;
  _sv0t2.p0 = 5;
  int _sv0t3 = sv0_box_alloc(3);
  sv0_box_store(_sv0t3, 0, _sv0t2.tag);
  sv0_box_store(_sv0t3, 1, _sv0t2.p0);
  sv0_box_store(_sv0t3, 2, _sv0t2.p1);
  _sv0t1.tag = 15;
  _sv0t1.p0 = _sv0t3;
  int _sv0t4 = sv0_box_alloc(5);
  sv0_box_store(_sv0t4, 0, _sv0t1.tag);
  sv0_box_store(_sv0t4, 1, _sv0t1.p0);
  sv0_box_store(_sv0t4, 2, _sv0t1.p1);
  sv0_box_store(_sv0t4, 3, _sv0t1.p2);
  sv0_box_store(_sv0t4, 4, _sv0t1.p3);
  sv0_vec_push(body0, _sv0t4);
  int _sv0t5 = sv0_vec_new();
  int ens0 = _sv0t5;
  Instr _sv0t6;
  Expr _sv0t7;
  Value _sv0t8;
  _sv0t8.tag = 1;
  int _sv0t9 = sv0_box_alloc(3);
  sv0_box_store(_sv0t9, 0, _sv0t8.tag);
  sv0_box_store(_sv0t9, 1, _sv0t8.p0);
  sv0_box_store(_sv0t9, 2, _sv0t8.p1);
  _sv0t7.tag = 0;
  _sv0t7.p0 = _sv0t9;
  int _sv0t10 = sv0_box_alloc(4);
  sv0_box_store(_sv0t10, 0, _sv0t7.tag);
  sv0_box_store(_sv0t10, 1, _sv0t7.p0);
  sv0_box_store(_sv0t10, 2, _sv0t7.p1);
  sv0_box_store(_sv0t10, 3, _sv0t7.p2);
  _sv0t6.tag = 13;
  _sv0t6.p0 = _sv0t10;
  _sv0t6.p1 = 99;
  int _sv0t11 = sv0_box_alloc(5);
  sv0_box_store(_sv0t11, 0, _sv0t6.tag);
  sv0_box_store(_sv0t11, 1, _sv0t6.p0);
  sv0_box_store(_sv0t11, 2, _sv0t6.p1);
  sv0_box_store(_sv0t11, 3, _sv0t6.p2);
  sv0_box_store(_sv0t11, 4, _sv0t6.p3);
  sv0_vec_push(ens0, _sv0t11);
  int _sv0t12 = (0 - 1);
  int _sv0t13 = inject_ensures_and_ret_slot(body0, ens0, 0, _sv0t12);
  int out0 = _sv0t13;
  int _sv0t14 = sv0_vec_len(out0);
  if ((_sv0t14 != 2)) {
    return 1;
  } else {
  }
  int _sv0t15 = sv0_vec_get(out0, 0);
  Instr _sv0t16;
  int _sv0t17 = sv0_box_load(_sv0t15, 0);
  _sv0t16.tag = _sv0t17;
  int _sv0t18 = sv0_box_load(_sv0t15, 1);
  _sv0t16.p0 = _sv0t18;
  int _sv0t19 = sv0_box_load(_sv0t15, 2);
  _sv0t16.p1 = _sv0t19;
  int _sv0t20 = sv0_box_load(_sv0t15, 3);
  _sv0t16.p2 = _sv0t20;
  int _sv0t21 = sv0_box_load(_sv0t15, 4);
  _sv0t16.p3 = _sv0t21;
  Instr i0;
  i0.tag = (_sv0t16).tag;
  i0.p0 = (_sv0t16).p0;
  i0.p1 = (_sv0t16).p1;
  i0.p2 = (_sv0t16).p2;
  i0.p3 = (_sv0t16).p3;
  int _sv0t22 = ir_instr_tag(i0);
  if ((_sv0t22 != 13)) {
    return 2;
  } else {
  }
  int _sv0t23 = sv0_vec_get(out0, 1);
  Instr _sv0t24;
  int _sv0t25 = sv0_box_load(_sv0t23, 0);
  _sv0t24.tag = _sv0t25;
  int _sv0t26 = sv0_box_load(_sv0t23, 1);
  _sv0t24.p0 = _sv0t26;
  int _sv0t27 = sv0_box_load(_sv0t23, 2);
  _sv0t24.p1 = _sv0t27;
  int _sv0t28 = sv0_box_load(_sv0t23, 3);
  _sv0t24.p2 = _sv0t28;
  int _sv0t29 = sv0_box_load(_sv0t23, 4);
  _sv0t24.p3 = _sv0t29;
  Instr i1;
  i1.tag = (_sv0t24).tag;
  i1.p0 = (_sv0t24).p0;
  i1.p1 = (_sv0t24).p1;
  i1.p2 = (_sv0t24).p2;
  i1.p3 = (_sv0t24).p3;
  int _sv0t30 = ir_instr_tag(i1);
  if ((_sv0t30 != 15)) {
    return 3;
  } else {
  }
  int _sv0t31 = sv0_vec_new();
  int body1 = _sv0t31;
  Instr _sv0t32;
  Value _sv0t33;
  _sv0t33.tag = 0;
  _sv0t33.p0 = 7;
  int _sv0t34 = sv0_box_alloc(3);
  sv0_box_store(_sv0t34, 0, _sv0t33.tag);
  sv0_box_store(_sv0t34, 1, _sv0t33.p0);
  sv0_box_store(_sv0t34, 2, _sv0t33.p1);
  _sv0t32.tag = 15;
  _sv0t32.p0 = _sv0t34;
  int _sv0t35 = sv0_box_alloc(5);
  sv0_box_store(_sv0t35, 0, _sv0t32.tag);
  sv0_box_store(_sv0t35, 1, _sv0t32.p0);
  sv0_box_store(_sv0t35, 2, _sv0t32.p1);
  sv0_box_store(_sv0t35, 3, _sv0t32.p2);
  sv0_box_store(_sv0t35, 4, _sv0t32.p3);
  sv0_vec_push(body1, _sv0t35);
  int _sv0t36 = sv0_vec_new();
  int ens1 = _sv0t36;
  Instr _sv0t37;
  Expr _sv0t38;
  Value _sv0t39;
  _sv0t39.tag = 1;
  int _sv0t40 = sv0_box_alloc(3);
  sv0_box_store(_sv0t40, 0, _sv0t39.tag);
  sv0_box_store(_sv0t40, 1, _sv0t39.p0);
  sv0_box_store(_sv0t40, 2, _sv0t39.p1);
  _sv0t38.tag = 0;
  _sv0t38.p0 = _sv0t40;
  int _sv0t41 = sv0_box_alloc(4);
  sv0_box_store(_sv0t41, 0, _sv0t38.tag);
  sv0_box_store(_sv0t41, 1, _sv0t38.p0);
  sv0_box_store(_sv0t41, 2, _sv0t38.p1);
  sv0_box_store(_sv0t41, 3, _sv0t38.p2);
  _sv0t37.tag = 13;
  _sv0t37.p0 = _sv0t41;
  _sv0t37.p1 = 77;
  int _sv0t42 = sv0_box_alloc(5);
  sv0_box_store(_sv0t42, 0, _sv0t37.tag);
  sv0_box_store(_sv0t42, 1, _sv0t37.p0);
  sv0_box_store(_sv0t42, 2, _sv0t37.p1);
  sv0_box_store(_sv0t42, 3, _sv0t37.p2);
  sv0_box_store(_sv0t42, 4, _sv0t37.p3);
  sv0_vec_push(ens1, _sv0t42);
  int _sv0t43 = inject_ensures_and_ret_slot(body1, ens1, 1, 42);
  int out1 = _sv0t43;
  int _sv0t44 = sv0_vec_len(out1);
  if ((_sv0t44 != 3)) {
    return 4;
  } else {
  }
  int _sv0t45 = sv0_vec_get(out1, 1);
  Instr _sv0t46;
  int _sv0t47 = sv0_box_load(_sv0t45, 0);
  _sv0t46.tag = _sv0t47;
  int _sv0t48 = sv0_box_load(_sv0t45, 1);
  _sv0t46.p0 = _sv0t48;
  int _sv0t49 = sv0_box_load(_sv0t45, 2);
  _sv0t46.p1 = _sv0t49;
  int _sv0t50 = sv0_box_load(_sv0t45, 3);
  _sv0t46.p2 = _sv0t50;
  int _sv0t51 = sv0_box_load(_sv0t45, 4);
  _sv0t46.p3 = _sv0t51;
  Instr j1;
  j1.tag = (_sv0t46).tag;
  j1.p0 = (_sv0t46).p0;
  j1.p1 = (_sv0t46).p1;
  j1.p2 = (_sv0t46).p2;
  j1.p3 = (_sv0t46).p3;
  int _sv0t52 = ir_instr_tag(j1);
  if ((_sv0t52 != 13)) {
    return 5;
  } else {
  }
  int _sv0t53 = sv0_vec_get(out1, 2);
  Instr _sv0t54;
  int _sv0t55 = sv0_box_load(_sv0t53, 0);
  _sv0t54.tag = _sv0t55;
  int _sv0t56 = sv0_box_load(_sv0t53, 1);
  _sv0t54.p0 = _sv0t56;
  int _sv0t57 = sv0_box_load(_sv0t53, 2);
  _sv0t54.p1 = _sv0t57;
  int _sv0t58 = sv0_box_load(_sv0t53, 3);
  _sv0t54.p2 = _sv0t58;
  int _sv0t59 = sv0_box_load(_sv0t53, 4);
  _sv0t54.p3 = _sv0t59;
  Instr j2;
  j2.tag = (_sv0t54).tag;
  j2.p0 = (_sv0t54).p0;
  j2.p1 = (_sv0t54).p1;
  j2.p2 = (_sv0t54).p2;
  j2.p3 = (_sv0t54).p3;
  int _sv0t60 = ir_instr_tag(j2);
  if ((_sv0t60 != 15)) {
    return 6;
  } else {
  }
  int _sv0t61;
  if ((j2.tag == 15)) {
    int bv2 = j2.p0;
    Value _sv0t62;
    int _sv0t63 = sv0_box_load(bv2, 0);
    _sv0t62.tag = _sv0t63;
    int _sv0t64 = sv0_box_load(bv2, 1);
    _sv0t62.p0 = _sv0t64;
    int _sv0t65 = sv0_box_load(bv2, 2);
    _sv0t62.p1 = _sv0t65;
    Value rv2;
    rv2.tag = (_sv0t62).tag;
    rv2.p0 = (_sv0t62).p0;
    rv2.p1 = (_sv0t62).p1;
    int _sv0t66 = ir_value_tag(rv2);
    if ((_sv0t66 != 3)) {
      return 7;
    } else {
    }
    int _sv0t67;
    if ((rv2.tag == 3)) {
      int h2 = rv2.p0;
      if ((h2 != 42)) {
        return 8;
        _sv0t67 = 0;
      } else {
        _sv0t67 = 0;
      }
    } else {
      if (1) {
        return 9;
        _sv0t67 = 0;
      } else {
      }
    }
    _sv0t61 = _sv0t67;
  } else {
    if (1) {
      return 10;
      _sv0t61 = 0;
    } else {
    }
  }
  return 0;
}

static int test_assign_lhs(void) {
  int _sv0t0 = low_classify_assign_lhs(2);
  if ((_sv0t0 != 0)) {
    return 1;
  } else {
  }
  int _sv0t1 = low_classify_assign_lhs(11);
  if ((_sv0t1 != 1)) {
    return 2;
  } else {
  }
  int _sv0t2 = low_classify_assign_lhs(99);
  int _sv0t3 = (0 - 1);
  if ((_sv0t2 != _sv0t3)) {
    return 3;
  } else {
  }
  return 0;
}

static int test_enum_tag(void) {
  int _sv0t0 = sv0_vec_new();
  int names = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int tags_flat = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int offsets = _sv0t2;
  int _sv0t3 = sv0_vec_new();
  int counts = _sv0t3;
  sv0_vec_push(names, 100);
  sv0_vec_push(offsets, 0);
  sv0_vec_push(counts, 3);
  sv0_vec_push(tags_flat, 10);
  sv0_vec_push(tags_flat, 0);
  sv0_vec_push(tags_flat, 20);
  sv0_vec_push(tags_flat, 1);
  sv0_vec_push(tags_flat, 30);
  sv0_vec_push(tags_flat, 2);
  int _sv0t4 = enum_tag_lookup(names, tags_flat, offsets, counts, 100, 10);
  if ((_sv0t4 != 0)) {
    return 1;
  } else {
  }
  int _sv0t5 = enum_tag_lookup(names, tags_flat, offsets, counts, 100, 20);
  if ((_sv0t5 != 1)) {
    return 2;
  } else {
  }
  int _sv0t6 = enum_tag_lookup(names, tags_flat, offsets, counts, 100, 30);
  if ((_sv0t6 != 2)) {
    return 3;
  } else {
  }
  int _sv0t7 = enum_tag_lookup(names, tags_flat, offsets, counts, 100, 99);
  int _sv0t8 = (0 - 1);
  if ((_sv0t7 != _sv0t8)) {
    return 4;
  } else {
  }
  int _sv0t9 = enum_tag_lookup(names, tags_flat, offsets, counts, 999, 10);
  int _sv0t10 = (0 - 2);
  if ((_sv0t9 != _sv0t10)) {
    return 5;
  } else {
  }
  return 0;
}

static int test_struct_fields(void) {
  int _sv0t0 = sv0_vec_new();
  int names = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int offsets = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int counts = _sv0t2;
  int _sv0t3 = sv0_vec_new();
  int flat = _sv0t3;
  sv0_vec_push(names, 50);
  sv0_vec_push(offsets, 0);
  sv0_vec_push(counts, 2);
  sv0_vec_push(flat, 10);
  sv0_vec_push(flat, 20);
  int _sv0t4 = struct_fields_lookup(names, offsets, counts, flat, 50);
  if ((_sv0t4 != 0)) {
    return 1;
  } else {
  }
  int _sv0t5 = struct_field_count(names, counts, 50);
  if ((_sv0t5 != 2)) {
    return 2;
  } else {
  }
  int _sv0t6 = struct_fields_lookup(names, offsets, counts, flat, 99);
  int _sv0t7 = (0 - 1);
  if ((_sv0t6 != _sv0t7)) {
    return 3;
  } else {
  }
  return 0;
}

static int test_type_detection(void) {
  int _sv0t0 = sv0_vec_new();
  int en = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int sn = _sv0t1;
  sv0_vec_push(en, 10);
  sv0_vec_push(sn, 20);
  int _sv0t2 = is_enum_cty(en, 10);
  if ((_sv0t2 != 1)) {
    return 1;
  } else {
  }
  int _sv0t3 = is_enum_cty(en, 99);
  if ((_sv0t3 != 0)) {
    return 2;
  } else {
  }
  int _sv0t4 = is_struct_cty(sn, 20);
  if ((_sv0t4 != 1)) {
    return 3;
  } else {
  }
  int _sv0t5 = is_struct_cty(sn, 99);
  if ((_sv0t5 != 0)) {
    return 4;
  } else {
  }
  return 0;
}

static int test_variant_slots(void) {
  int _sv0t0 = variant_slot_count(0, 0);
  if ((_sv0t0 != 0)) {
    return 1;
  } else {
  }
  int _sv0t1 = variant_slot_count(1, 3);
  if ((_sv0t1 != 3)) {
    return 2;
  } else {
  }
  int _sv0t2 = variant_slot_count(2, 2);
  if ((_sv0t2 != 2)) {
    return 3;
  } else {
  }
  return 0;
}

static int test_try_variants(void) {
  const char* _sv0t0 = try_variant_success(1, 0);
  int _sv0t1 = sv0_string_eq(_sv0t0, "Ok");
  if ((_sv0t1 != 1)) {
    return 1;
  } else {
  }
  const char* _sv0t2 = try_variant_success(0, 1);
  int _sv0t3 = sv0_string_eq(_sv0t2, "Some");
  if ((_sv0t3 != 1)) {
    return 2;
  } else {
  }
  const char* _sv0t4 = try_variant_failure(1, 0);
  int _sv0t5 = sv0_string_eq(_sv0t4, "Err");
  if ((_sv0t5 != 1)) {
    return 3;
  } else {
  }
  const char* _sv0t6 = try_variant_failure(0, 1);
  int _sv0t7 = sv0_string_eq(_sv0t6, "None");
  if ((_sv0t7 != 1)) {
    return 4;
  } else {
  }
  int _sv0t8 = is_try_enum(1, 1, 0, 0);
  if ((_sv0t8 != 1)) {
    return 5;
  } else {
  }
  int _sv0t9 = is_try_enum(0, 0, 1, 1);
  if ((_sv0t9 != 1)) {
    return 6;
  } else {
  }
  int _sv0t10 = is_try_enum(1, 0, 0, 0);
  if ((_sv0t10 != 0)) {
    return 7;
  } else {
  }
  return 0;
}

static int test_literals(void) {
  int _sv0t0 = classify_literal(0);
  if ((_sv0t0 != 0)) {
    return 1;
  } else {
  }
  int _sv0t1 = classify_literal(1);
  if ((_sv0t1 != 1)) {
    return 2;
  } else {
  }
  int _sv0t2 = classify_literal(2);
  if ((_sv0t2 != 2)) {
    return 3;
  } else {
  }
  int _sv0t3 = classify_literal(3);
  if ((_sv0t3 != 3)) {
    return 4;
  } else {
  }
  int _sv0t4 = classify_literal(99);
  int _sv0t5 = (0 - 1);
  if ((_sv0t4 != _sv0t5)) {
    return 5;
  } else {
  }
  return 0;
}

static int test_value_to_expr(void) {
  int _sv0t0 = value_to_expr_kind(0);
  if ((_sv0t0 != 0)) {
    return 1;
  } else {
  }
  int _sv0t1 = value_to_expr_kind(1);
  if ((_sv0t1 != 1)) {
    return 2;
  } else {
  }
  return 0;
}

static int test_enum_alloc(void) {
  int _sv0t0 = enum_alloc_width(0);
  if ((_sv0t0 != 1)) {
    return 1;
  } else {
  }
  int _sv0t1 = enum_alloc_width(3);
  if ((_sv0t1 != 4)) {
    return 2;
  } else {
  }
  return 0;
}

static int test_ret_value_slot(void) {
  const char* _sv0t0 = ret_value_slot();
  int _sv0t1 = sv0_string_eq(_sv0t0, "_sv0ret");
  if ((_sv0t1 != 1)) {
    return 1;
  } else {
  }
  return 0;
}

static int test_use_ret_slot(void) {
  int _sv0t0 = use_ret_slot(1, 1);
  if ((_sv0t0 != 0)) {
    return 1;
  } else {
  }
  int _sv0t1 = use_ret_slot(1, 0);
  if ((_sv0t1 != 0)) {
    return 2;
  } else {
  }
  int _sv0t2 = use_ret_slot(0, 0);
  if ((_sv0t2 != 0)) {
    return 3;
  } else {
  }
  int _sv0t3 = use_ret_slot(0, 1);
  if ((_sv0t3 != 1)) {
    return 4;
  } else {
  }
  return 0;
}

static int test_lit_supported(void) {
  int _sv0t0 = lower_lit_supported(0);
  if ((_sv0t0 != 1)) {
    return 1;
  } else {
  }
  int _sv0t1 = lower_lit_supported(1);
  if ((_sv0t1 != 1)) {
    return 2;
  } else {
  }
  int _sv0t2 = lower_lit_supported(2);
  if ((_sv0t2 != 1)) {
    return 3;
  } else {
  }
  int _sv0t3 = lower_lit_supported(3);
  if ((_sv0t3 != 1)) {
    return 4;
  } else {
  }
  int _sv0t4 = lower_lit_supported(99);
  if ((_sv0t4 != 0)) {
    return 5;
  } else {
  }
  return 0;
}

static int test_param_name_tag(void) {
  int _sv0t0 = param_name_tag(1);
  if ((_sv0t0 != 1)) {
    return 1;
  } else {
  }
  int _sv0t1 = param_name_tag(0);
  if ((_sv0t1 != 0)) {
    return 2;
  } else {
  }
  int _sv0t2 = param_name_tag(2);
  if ((_sv0t2 != 0)) {
    return 3;
  } else {
  }
  return 0;
}

static int test_uniq_old_names(void) {
  int _sv0t0 = sv0_vec_new();
  int names = _sv0t0;
  sv0_vec_push(names, 10);
  sv0_vec_push(names, 20);
  sv0_vec_push(names, 10);
  sv0_vec_push(names, 30);
  sv0_vec_push(names, 20);
  int _sv0t1 = uniq_old_names(names);
  int uniq = _sv0t1;
  int _sv0t2 = sv0_vec_len(uniq);
  if ((_sv0t2 != 3)) {
    return 1;
  } else {
  }
  int _sv0t3 = sv0_vec_get(uniq, 0);
  if ((_sv0t3 != 10)) {
    return 2;
  } else {
  }
  int _sv0t4 = sv0_vec_get(uniq, 1);
  if ((_sv0t4 != 20)) {
    return 3;
  } else {
  }
  int _sv0t5 = sv0_vec_get(uniq, 2);
  if ((_sv0t5 != 30)) {
    return 4;
  } else {
  }
  int _sv0t6 = sv0_vec_new();
  int empty = _sv0t6;
  int _sv0t7 = uniq_old_names(empty);
  int uniq2 = _sv0t7;
  int _sv0t8 = sv0_vec_len(uniq2);
  if ((_sv0t8 != 0)) {
    return 5;
  } else {
  }
  int _sv0t9 = old_name_seen(uniq, 10);
  if ((_sv0t9 != 1)) {
    return 6;
  } else {
  }
  int _sv0t10 = old_name_seen(uniq, 99);
  if ((_sv0t10 != 0)) {
    return 7;
  } else {
  }
  return 0;
}

static int test_fresh_tmp_str(void) {
  const char* _sv0t0 = fresh_tmp_str(0);
  int _sv0t1 = sv0_string_eq(_sv0t0, "_sv0t0");
  if ((_sv0t1 != 1)) {
    return 1;
  } else {
  }
  const char* _sv0t2 = fresh_tmp_str(1);
  int _sv0t3 = sv0_string_eq(_sv0t2, "_sv0t1");
  if ((_sv0t3 != 1)) {
    return 2;
  } else {
  }
  const char* _sv0t4 = fresh_tmp_str(42);
  int _sv0t5 = sv0_string_eq(_sv0t4, "_sv0t42");
  if ((_sv0t5 != 1)) {
    return 3;
  } else {
  }
  const char* _sv0t6 = fresh_tmp_str(100);
  int _sv0t7 = sv0_string_eq(_sv0t6, "_sv0t100");
  if ((_sv0t7 != 1)) {
    return 4;
  } else {
  }
  return 0;
}

static int test_lower_lit_ir(void) {
  int _sv0t0 = lower_lit_to_ir_tag(0);
  if ((_sv0t0 != 0)) {
    return 1;
  } else {
  }
  int _sv0t1 = lower_lit_to_ir_tag(1);
  if ((_sv0t1 != 1)) {
    return 2;
  } else {
  }
  int _sv0t2 = lower_lit_to_ir_tag(2);
  if ((_sv0t2 != 4)) {
    return 3;
  } else {
  }
  int _sv0t3 = lower_lit_to_ir_tag(3);
  if ((_sv0t3 != 5)) {
    return 4;
  } else {
  }
  int _sv0t4 = lower_lit_to_ir_tag(99);
  int _sv0t5 = (0 - 1);
  if ((_sv0t4 != _sv0t5)) {
    return 5;
  } else {
  }
  return 0;
}

static int test_fn_ret_cty(void) {
  const char* _sv0t0 = fn_ret_cty(0, "void");
  int _sv0t1 = sv0_string_eq(_sv0t0, "int");
  if ((_sv0t1 != 1)) {
    return 1;
  } else {
  }
  const char* _sv0t2 = fn_ret_cty(1, "int32_t");
  int _sv0t3 = sv0_string_eq(_sv0t2, "int32_t");
  if ((_sv0t3 != 1)) {
    return 2;
  } else {
  }
  const char* _sv0t4 = fn_ret_cty(1, "void");
  int _sv0t5 = sv0_string_eq(_sv0t4, "void");
  if ((_sv0t5 != 1)) {
    return 3;
  } else {
  }
  return 0;
}

static int test_lower_alias(void) {
  int _sv0t0 = lower_alias_new();
  int a = _sv0t0;
  int _sv0t1 = lower_alias_add(a, 10, 20);
  int _sv0t2 = lower_alias_add(a, 30, 40);
  int _sv0t3 = lower_alias_lookup(a, 10);
  if ((_sv0t3 != 20)) {
    return 1;
  } else {
  }
  int _sv0t4 = lower_alias_lookup(a, 30);
  if ((_sv0t4 != 40)) {
    return 2;
  } else {
  }
  int _sv0t5 = lower_alias_lookup(a, 99);
  int _sv0t6 = (0 - 1);
  if ((_sv0t5 != _sv0t6)) {
    return 3;
  } else {
  }
  return 0;
}

static int test_resolve_fn_callee(void) {
  int _sv0t0 = lower_alias_new();
  int a = _sv0t0;
  int _sv0t1 = lower_alias_add(a, 10, 20);
  int _sv0t2 = resolve_fn_callee(a, 10);
  if ((_sv0t2 != 20)) {
    return 1;
  } else {
  }
  int _sv0t3 = resolve_fn_callee(a, 99);
  if ((_sv0t3 != 99)) {
    return 2;
  } else {
  }
  return 0;
}

static int test_resolve_ctor_path(void) {
  int _sv0t0 = sv0_vec_new();
  int fe = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int fv = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int te = _sv0t2;
  int _sv0t3 = sv0_vec_new();
  int tv = _sv0t3;
  sv0_vec_push(fe, 10);
  sv0_vec_push(fv, 20);
  sv0_vec_push(te, 30);
  sv0_vec_push(tv, 40);
  int _sv0t4 = resolve_enum_ctor_path(fe, fv, te, tv, 10, 20);
  int idx = _sv0t4;
  if ((idx != 0)) {
    return 1;
  } else {
  }
  int _sv0t5 = resolved_ctor_enum(te, idx, 10);
  if ((_sv0t5 != 30)) {
    return 2;
  } else {
  }
  int _sv0t6 = resolved_ctor_variant(tv, idx, 20);
  if ((_sv0t6 != 40)) {
    return 3;
  } else {
  }
  int _sv0t7 = resolve_enum_ctor_path(fe, fv, te, tv, 99, 88);
  int miss = _sv0t7;
  int _sv0t8 = (0 - 1);
  if ((miss != _sv0t8)) {
    return 4;
  } else {
  }
  int _sv0t9 = resolved_ctor_enum(te, miss, 99);
  if ((_sv0t9 != 99)) {
    return 5;
  } else {
  }
  int _sv0t10 = resolved_ctor_variant(tv, miss, 88);
  if ((_sv0t10 != 88)) {
    return 6;
  } else {
  }
  return 0;
}

static int test_param_name(void) {
  int _sv0t0 = param_name(1, 42);
  if ((_sv0t0 != 42)) {
    return 1;
  } else {
  }
  int _sv0t1 = param_name(0, 42);
  int _sv0t2 = (0 - 1);
  if ((_sv0t1 != _sv0t2)) {
    return 2;
  } else {
  }
  int _sv0t3 = param_name(3, 99);
  int _sv0t4 = (0 - 1);
  if ((_sv0t3 != _sv0t4)) {
    return 3;
  } else {
  }
  return 0;
}

static int test_lower_field_index(void) {
  int _sv0t0 = sv0_vec_new();
  int fields = _sv0t0;
  sv0_vec_push(fields, 10);
  sv0_vec_push(fields, 20);
  sv0_vec_push(fields, 30);
  int _sv0t1 = lower_field_index(fields, 10);
  if ((_sv0t1 != 0)) {
    return 1;
  } else {
  }
  int _sv0t2 = lower_field_index(fields, 20);
  if ((_sv0t2 != 1)) {
    return 2;
  } else {
  }
  int _sv0t3 = lower_field_index(fields, 30);
  if ((_sv0t3 != 2)) {
    return 3;
  } else {
  }
  int _sv0t4 = lower_field_index(fields, 99);
  int _sv0t5 = (0 - 1);
  if ((_sv0t4 != _sv0t5)) {
    return 4;
  } else {
  }
  int _sv0t6 = sv0_vec_new();
  int empty = _sv0t6;
  int _sv0t7 = lower_field_index(empty, 10);
  int _sv0t8 = (0 - 1);
  if ((_sv0t7 != _sv0t8)) {
    return 5;
  } else {
  }
  return 0;
}

static int test_ast_ty_c_string_user(void) {
  int _sv0t0 = sv0_vec_new();
  int sn = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int en = _sv0t1;
  sv0_vec_push(sn, 100);
  sv0_vec_push(en, 200);
  const char* _sv0t2 = ast_ty_to_c_string_with_user("i32", 50, sn, en);
  int _sv0t3 = sv0_string_eq(_sv0t2, "int");
  if ((_sv0t3 != 1)) {
    return 1;
  } else {
  }
  const char* _sv0t4 = ast_ty_to_c_string_with_user("MyStruct", 100, sn, en);
  int _sv0t5 = sv0_string_eq(_sv0t4, "MyStruct");
  if ((_sv0t5 != 1)) {
    return 2;
  } else {
  }
  const char* _sv0t6 = ast_ty_to_c_string_with_user("MyEnum", 200, sn, en);
  int _sv0t7 = sv0_string_eq(_sv0t6, "MyEnum");
  if ((_sv0t7 != 1)) {
    return 3;
  } else {
  }
  const char* _sv0t8 = ast_ty_to_c_string_with_user("Unknown", 999, sn, en);
  int _sv0t9 = sv0_string_eq(_sv0t8, "int");
  if ((_sv0t9 != 1)) {
    return 4;
  } else {
  }
  const char* _sv0t10 = ast_ty_to_c_string_with_user("bool", 999, sn, en);
  int _sv0t11 = sv0_string_eq(_sv0t10, "int");
  if ((_sv0t11 != 1)) {
    return 5;
  } else {
  }
  return 0;
}

static int test_find_fn_index(void) {
  int _sv0t0 = sv0_vec_new();
  int names = _sv0t0;
  sv0_vec_push(names, 10);
  sv0_vec_push(names, 20);
  sv0_vec_push(names, 30);
  int _sv0t1 = find_fn_index(names, 10);
  if ((_sv0t1 != 0)) {
    return 1;
  } else {
  }
  int _sv0t2 = find_fn_index(names, 20);
  if ((_sv0t2 != 1)) {
    return 2;
  } else {
  }
  int _sv0t3 = find_fn_index(names, 30);
  if ((_sv0t3 != 2)) {
    return 3;
  } else {
  }
  int _sv0t4 = find_fn_index(names, 99);
  int _sv0t5 = (0 - 1);
  if ((_sv0t4 != _sv0t5)) {
    return 4;
  } else {
  }
  return 0;
}

static int test_callee_fn_index(void) {
  int _sv0t0 = sv0_vec_new();
  int names = _sv0t0;
  sv0_vec_push(names, 10);
  sv0_vec_push(names, 20);
  int _sv0t1 = sv0_vec_new();
  int aliases = _sv0t1;
  sv0_vec_push(aliases, 30);
  sv0_vec_push(aliases, 10);
  int _sv0t2 = callee_fn_index(names, aliases, 10);
  if ((_sv0t2 != 0)) {
    return 1;
  } else {
  }
  int _sv0t3 = callee_fn_index(names, aliases, 30);
  if ((_sv0t3 != 0)) {
    return 2;
  } else {
  }
  int _sv0t4 = callee_fn_index(names, aliases, 99);
  int _sv0t5 = (0 - 1);
  if ((_sv0t4 != _sv0t5)) {
    return 3;
  } else {
  }
  return 0;
}

static int test_scrut_local(void) {
  int _sv0t0 = sv0_vec_new();
  int names = _sv0t0;
  sv0_vec_push(names, 50);
  sv0_vec_push(names, 60);
  int _sv0t1 = scrut_local_lookup(names, 50);
  if ((_sv0t1 != 0)) {
    return 1;
  } else {
  }
  int _sv0t2 = scrut_local_lookup(names, 60);
  if ((_sv0t2 != 1)) {
    return 2;
  } else {
  }
  int _sv0t3 = scrut_local_lookup(names, 99);
  int _sv0t4 = (0 - 1);
  if ((_sv0t3 != _sv0t4)) {
    return 3;
  } else {
  }
  return 0;
}

static int test_param_ty(void) {
  int _sv0t0 = sv0_vec_new();
  int names = _sv0t0;
  sv0_vec_push(names, 70);
  sv0_vec_push(names, 80);
  int _sv0t1 = param_ty_lookup(names, 70);
  if ((_sv0t1 != 0)) {
    return 1;
  } else {
  }
  int _sv0t2 = param_ty_lookup(names, 80);
  if ((_sv0t2 != 1)) {
    return 2;
  } else {
  }
  int _sv0t3 = param_ty_lookup(names, 99);
  int _sv0t4 = (0 - 1);
  if ((_sv0t3 != _sv0t4)) {
    return 3;
  } else {
  }
  return 0;
}

static int test_mentions_result(void) {
  int _sv0t0 = sv0_vec_new();
  int _sv0t1 = sv0_vec_new();
  int _sv0t2 = sv0_vec_new();
  int _sv0t3 = sv0_vec_new();
  int _sv0t4 = sv0_vec_new();
  int _sv0t5 = (0 - 1);
  int _sv0t6 = sv0_vec_new();
  int _sv0t7 = sv0_vec_new();
  int _sv0t8 = sv0_vec_new();
  int _sv0t9 = mentions_result(_sv0t0, _sv0t1, _sv0t2, _sv0t3, _sv0t4, _sv0t5, "", _sv0t6, _sv0t7, _sv0t8);
  if ((_sv0t9 != 0)) {
    return 1;
  } else {
  }
  const char* src;
  src = "result x old";
  int _sv0t10 = sv0_vec_new();
  int st = _sv0t10;
  int _sv0t11 = sv0_vec_new();
  int nd = _sv0t11;
  sv0_vec_push(st, 0);
  sv0_vec_push(nd, 6);
  sv0_vec_push(st, 7);
  sv0_vec_push(nd, 8);
  sv0_vec_push(st, 9);
  sv0_vec_push(nd, 12);
  int _sv0t12 = sv0_vec_new();
  int pp = _sv0t12;
  int _sv0t13 = sv0_vec_new();
  int et = _sv0t13;
  int _sv0t14 = sv0_vec_new();
  int d1 = _sv0t14;
  int _sv0t15 = sv0_vec_new();
  int d2 = _sv0t15;
  int _sv0t16 = sv0_vec_new();
  int d3 = _sv0t16;
  int _sv0t17 = sv0_vec_new();
  int d4 = _sv0t17;
  sv0_vec_push(pp, 0);
  sv0_vec_push(pp, 1);
  sv0_vec_push(et, 1);
  sv0_vec_push(d1, 0);
  sv0_vec_push(d2, 1);
  sv0_vec_push(d3, 0);
  sv0_vec_push(d4, 0);
  int _sv0t18 = mentions_result(et, d1, d2, d3, d4, 0, src, st, nd, pp);
  if ((_sv0t18 != 1)) {
    return 2;
  } else {
  }
  int _sv0t19 = sv0_vec_new();
  int et2 = _sv0t19;
  int _sv0t20 = sv0_vec_new();
  int d1b = _sv0t20;
  int _sv0t21 = sv0_vec_new();
  int d2b = _sv0t21;
  int _sv0t22 = sv0_vec_new();
  int d3b = _sv0t22;
  int _sv0t23 = sv0_vec_new();
  int d4b = _sv0t23;
  int _sv0t24 = sv0_vec_new();
  int pp2 = _sv0t24;
  sv0_vec_push(pp2, 1);
  sv0_vec_push(et2, 1);
  sv0_vec_push(d1b, 0);
  sv0_vec_push(d2b, 1);
  sv0_vec_push(d3b, 0);
  sv0_vec_push(d4b, 0);
  int _sv0t25 = mentions_result(et2, d1b, d2b, d3b, d4b, 0, src, st, nd, pp2);
  if ((_sv0t25 != 0)) {
    return 3;
  } else {
  }
  int _sv0t26 = sv0_vec_new();
  int et3 = _sv0t26;
  int _sv0t27 = sv0_vec_new();
  int d1c = _sv0t27;
  int _sv0t28 = sv0_vec_new();
  int d2c = _sv0t28;
  int _sv0t29 = sv0_vec_new();
  int d3c = _sv0t29;
  int _sv0t30 = sv0_vec_new();
  int d4c = _sv0t30;
  int _sv0t31 = sv0_vec_new();
  int pp3 = _sv0t31;
  sv0_vec_push(pp3, 0);
  sv0_vec_push(pp3, 1);
  sv0_vec_push(et3, 1);
  sv0_vec_push(d1c, 1);
  sv0_vec_push(d2c, 1);
  sv0_vec_push(d3c, 0);
  sv0_vec_push(d4c, 0);
  sv0_vec_push(et3, 1);
  sv0_vec_push(d1c, 0);
  sv0_vec_push(d2c, 1);
  sv0_vec_push(d3c, 0);
  sv0_vec_push(d4c, 0);
  sv0_vec_push(et3, 3);
  sv0_vec_push(d1c, 0);
  sv0_vec_push(d2c, 0);
  sv0_vec_push(d3c, 1);
  sv0_vec_push(d4c, 0);
  int _sv0t32 = mentions_result(et3, d1c, d2c, d3c, d4c, 2, src, st, nd, pp3);
  if ((_sv0t32 != 1)) {
    return 4;
  } else {
  }
  return 0;
}

static int test_first_result_path_handle(void) {
  int _sv0t0 = sv0_vec_new();
  int et = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int d1 = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int d2 = _sv0t2;
  int _sv0t3 = sv0_vec_new();
  int d3 = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int d4 = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int pp = _sv0t5;
  const char* src;
  src = "result x";
  int _sv0t6 = sv0_vec_new();
  int st = _sv0t6;
  int _sv0t7 = sv0_vec_new();
  int nd = _sv0t7;
  sv0_vec_push(st, 0);
  sv0_vec_push(nd, 6);
  sv0_vec_push(st, 7);
  sv0_vec_push(nd, 8);
  sv0_vec_push(pp, 0);
  sv0_vec_push(pp, 1);
  sv0_vec_push(et, 1);
  sv0_vec_push(d1, 0);
  sv0_vec_push(d2, 1);
  sv0_vec_push(d3, 0);
  sv0_vec_push(d4, 0);
  sv0_vec_push(et, 1);
  sv0_vec_push(d1, 1);
  sv0_vec_push(d2, 1);
  sv0_vec_push(d3, 0);
  sv0_vec_push(d4, 0);
  sv0_vec_push(et, 3);
  sv0_vec_push(d1, 0);
  sv0_vec_push(d2, 0);
  sv0_vec_push(d3, 1);
  sv0_vec_push(d4, 0);
  int _sv0t8 = first_result_path_handle(et, d1, d2, d3, d4, 2, src, st, nd, pp);
  int h = _sv0t8;
  if ((h != 0)) {
    return 1;
  } else {
  }
  int _sv0t9 = first_result_path_handle(et, d1, d2, d3, d4, 1, src, st, nd, pp);
  int h2 = _sv0t9;
  int _sv0t10 = (0 - 1);
  if ((h2 != _sv0t10)) {
    return 2;
  } else {
  }
  return 0;
}

static int test_all_old_names(void) {
  int _sv0t0 = sv0_vec_new();
  int out0 = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int _sv0t2 = sv0_vec_new();
  int _sv0t3 = sv0_vec_new();
  int _sv0t4 = sv0_vec_new();
  int _sv0t5 = sv0_vec_new();
  int _sv0t6 = (0 - 1);
  int _sv0t7 = sv0_vec_new();
  int _sv0t8 = sv0_vec_new();
  int _sv0t9 = sv0_vec_new();
  int _sv0t10 = all_old_names_expr(_sv0t1, _sv0t2, _sv0t3, _sv0t4, _sv0t5, _sv0t6, "", _sv0t7, _sv0t8, _sv0t9, out0);
  int _sv0t11 = sv0_vec_len(out0);
  if ((_sv0t11 != 0)) {
    return 1;
  } else {
  }
  const char* src;
  src = "old x y";
  int _sv0t12 = sv0_vec_new();
  int st = _sv0t12;
  int _sv0t13 = sv0_vec_new();
  int nd = _sv0t13;
  sv0_vec_push(st, 0);
  sv0_vec_push(nd, 3);
  sv0_vec_push(st, 4);
  sv0_vec_push(nd, 5);
  sv0_vec_push(st, 6);
  sv0_vec_push(nd, 7);
  int _sv0t14 = sv0_vec_new();
  int pp = _sv0t14;
  sv0_vec_push(pp, 0);
  sv0_vec_push(pp, 1);
  sv0_vec_push(pp, 2);
  int _sv0t15 = sv0_vec_new();
  int et = _sv0t15;
  int _sv0t16 = sv0_vec_new();
  int d1 = _sv0t16;
  int _sv0t17 = sv0_vec_new();
  int d2 = _sv0t17;
  int _sv0t18 = sv0_vec_new();
  int d3 = _sv0t18;
  int _sv0t19 = sv0_vec_new();
  int d4 = _sv0t19;
  sv0_vec_push(et, 1);
  sv0_vec_push(d1, 0);
  sv0_vec_push(d2, 1);
  sv0_vec_push(d3, 0);
  sv0_vec_push(d4, 0);
  sv0_vec_push(et, 1);
  sv0_vec_push(d1, 1);
  sv0_vec_push(d2, 1);
  sv0_vec_push(d3, 0);
  sv0_vec_push(d4, 0);
  sv0_vec_push(et, 4);
  sv0_vec_push(d1, 0);
  sv0_vec_push(d2, 1);
  sv0_vec_push(d3, 1);
  sv0_vec_push(d4, 0);
  int _sv0t20 = sv0_vec_new();
  int out = _sv0t20;
  int _sv0t21 = all_old_names_expr(et, d1, d2, d3, d4, 2, src, st, nd, pp, out);
  int _sv0t22 = sv0_vec_len(out);
  if ((_sv0t22 != 1)) {
    return 2;
  } else {
  }
  int _sv0t23 = sv0_vec_get(out, 0);
  if ((_sv0t23 != 1)) {
    return 3;
  } else {
  }
  int _sv0t24 = sv0_vec_new();
  int et2 = _sv0t24;
  int _sv0t25 = sv0_vec_new();
  int e1 = _sv0t25;
  int _sv0t26 = sv0_vec_new();
  int e2 = _sv0t26;
  int _sv0t27 = sv0_vec_new();
  int e3 = _sv0t27;
  int _sv0t28 = sv0_vec_new();
  int e4 = _sv0t28;
  sv0_vec_push(et2, 1);
  sv0_vec_push(e1, 0);
  sv0_vec_push(e2, 1);
  sv0_vec_push(e3, 0);
  sv0_vec_push(e4, 0);
  sv0_vec_push(et2, 1);
  sv0_vec_push(e1, 1);
  sv0_vec_push(e2, 1);
  sv0_vec_push(e3, 0);
  sv0_vec_push(e4, 0);
  sv0_vec_push(et2, 4);
  sv0_vec_push(e1, 0);
  sv0_vec_push(e2, 1);
  sv0_vec_push(e3, 1);
  sv0_vec_push(e4, 0);
  sv0_vec_push(et2, 1);
  sv0_vec_push(e1, 2);
  sv0_vec_push(e2, 1);
  sv0_vec_push(e3, 0);
  sv0_vec_push(e4, 0);
  sv0_vec_push(et2, 4);
  sv0_vec_push(e1, 0);
  sv0_vec_push(e2, 3);
  sv0_vec_push(e3, 1);
  sv0_vec_push(e4, 0);
  sv0_vec_push(et2, 3);
  sv0_vec_push(e1, 0);
  sv0_vec_push(e2, 2);
  sv0_vec_push(e3, 4);
  sv0_vec_push(e4, 0);
  int _sv0t29 = sv0_vec_new();
  int out3 = _sv0t29;
  int _sv0t30 = all_old_names_expr(et2, e1, e2, e3, e4, 5, src, st, nd, pp, out3);
  int _sv0t31 = sv0_vec_len(out3);
  if ((_sv0t31 != 2)) {
    return 4;
  } else {
  }
  int _sv0t32 = sv0_vec_get(out3, 0);
  if ((_sv0t32 != 1)) {
    return 5;
  } else {
  }
  int _sv0t33 = sv0_vec_get(out3, 1);
  if ((_sv0t33 != 2)) {
    return 6;
  } else {
  }
  return 0;
}

static int test_typedef_builders(void) {
  const char* src;
  src = "P x i32 y bool E";
  int _sv0t0 = sv0_vec_new();
  int st = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int nd = _sv0t1;
  sv0_vec_push(st, 0);
  sv0_vec_push(nd, 1);
  sv0_vec_push(st, 2);
  sv0_vec_push(nd, 3);
  sv0_vec_push(st, 4);
  sv0_vec_push(nd, 7);
  sv0_vec_push(st, 8);
  sv0_vec_push(nd, 9);
  sv0_vec_push(st, 10);
  sv0_vec_push(nd, 14);
  sv0_vec_push(st, 15);
  sv0_vec_push(nd, 16);
  const char* _sv0t2 = handle_to_str(0, src, st, nd);
  int _sv0t3 = sv0_string_eq(_sv0t2, "P");
  if ((_sv0t3 != 1)) {
    return 1;
  } else {
  }
  int _sv0t4 = sv0_vec_new();
  int sn = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int en = _sv0t5;
  int _sv0t6 = sv0_vec_new();
  int fv = _sv0t6;
  int _sv0t7 = sv0_vec_new();
  int tv = _sv0t7;
  sv0_vec_push(fv, 1);
  sv0_vec_push(fv, 3);
  sv0_vec_push(tv, 2);
  sv0_vec_push(tv, 4);
  const char* _sv0t8 = emit_struct_td(0, fv, tv, 0, 2, src, st, nd, sn, en);
  const char* td;
  td = _sv0t8;
  int _sv0t9 = sv0_string_eq(td, "typedef struct {\n  int x;\n  int y;\n} P;\n");
  if ((_sv0t9 != 1)) {
    return 2;
  } else {
  }
  const char* _sv0t10 = emit_enum_td(5, 1, src, st, nd);
  const char* td2;
  td2 = _sv0t10;
  int _sv0t11 = sv0_string_eq(td2, "typedef struct {\n  int tag;\n  int p0;\n} E;\n");
  if ((_sv0t11 != 1)) {
    return 3;
  } else {
  }
  int _sv0t12 = sv0_vec_new();
  int tags = _sv0t12;
  int _sv0t13 = sv0_vec_new();
  int nms = _sv0t13;
  int _sv0t14 = sv0_vec_new();
  int fcs = _sv0t14;
  int _sv0t15 = sv0_vec_new();
  int vmx = _sv0t15;
  sv0_vec_push(tags, 1);
  sv0_vec_push(nms, 0);
  sv0_vec_push(fcs, 2);
  sv0_vec_push(vmx, 0);
  sv0_vec_push(tags, 2);
  sv0_vec_push(nms, 5);
  sv0_vec_push(fcs, 2);
  sv0_vec_push(vmx, 1);
  int _sv0t16 = sv0_vec_new();
  int sff = _sv0t16;
  sv0_vec_push(sff, 1);
  sv0_vec_push(sff, 3);
  int _sv0t17 = sv0_vec_new();
  int on = _sv0t17;
  int _sv0t18 = sv0_vec_new();
  int oo = _sv0t18;
  int _sv0t19 = sv0_vec_new();
  int oc = _sv0t19;
  int _sv0t20 = sv0_vec_new();
  int of1 = _sv0t20;
  int _sv0t21 = build_struct_order(tags, nms, fcs, sff, on, oo, oc, of1);
  if ((_sv0t21 != 1)) {
    return 4;
  } else {
  }
  int _sv0t22 = sv0_vec_get(oc, 0);
  if ((_sv0t22 != 2)) {
    return 5;
  } else {
  }
  int _sv0t23 = sv0_vec_new();
  int evn = _sv0t23;
  sv0_vec_push(evn, 10);
  sv0_vec_push(evn, 20);
  int _sv0t24 = sv0_vec_new();
  int en2 = _sv0t24;
  int _sv0t25 = sv0_vec_new();
  int eto = _sv0t25;
  int _sv0t26 = sv0_vec_new();
  int etc = _sv0t26;
  int _sv0t27 = sv0_vec_new();
  int etf = _sv0t27;
  int _sv0t28 = sv0_vec_new();
  int emp = _sv0t28;
  int _sv0t29 = build_enum_variants_data(tags, nms, fcs, vmx, evn, en2, eto, etc, etf, emp);
  if ((_sv0t29 != 1)) {
    return 6;
  } else {
  }
  int _sv0t30 = sv0_vec_get(emp, 0);
  if ((_sv0t30 != 1)) {
    return 7;
  } else {
  }
  return 0;
}

static int test_expr_init_cty(void) {
  int _sv0t0 = sv0_vec_new();
  int et = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int ed1 = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int ed2 = _sv0t2;
  int _sv0t3 = sv0_vec_new();
  int ed3 = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int ed4 = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int pp = _sv0t5;
  int _sv0t6 = sv0_vec_new();
  int fnn = _sv0t6;
  int _sv0t7 = sv0_vec_new();
  int frc = _sv0t7;
  int _sv0t8 = sv0_vec_new();
  int al = _sv0t8;
  sv0_vec_push(pp, 100);
  sv0_vec_push(et, 24);
  sv0_vec_push(ed1, 0);
  sv0_vec_push(ed2, 1);
  sv0_vec_push(ed3, 0);
  sv0_vec_push(ed4, 0);
  int _sv0t9 = expr_init_cty(et, ed1, ed2, ed3, ed4, 0, pp, fnn, frc, al);
  int r1 = _sv0t9;
  if ((r1 != 100)) {
    return 1;
  } else {
  }
  sv0_vec_push(pp, 200);
  sv0_vec_push(et, 1);
  sv0_vec_push(ed1, 0);
  sv0_vec_push(ed2, 2);
  sv0_vec_push(ed3, 0);
  sv0_vec_push(ed4, 0);
  int _sv0t10 = expr_init_cty(et, ed1, ed2, ed3, ed4, 1, pp, fnn, frc, al);
  int r2 = _sv0t10;
  if ((r2 != 100)) {
    return 2;
  } else {
  }
  sv0_vec_push(et, 0);
  sv0_vec_push(ed1, 0);
  sv0_vec_push(ed2, 0);
  sv0_vec_push(ed3, 0);
  sv0_vec_push(ed4, 0);
  int _sv0t11 = expr_init_cty(et, ed1, ed2, ed3, ed4, 2, pp, fnn, frc, al);
  int r3 = _sv0t11;
  int _sv0t12 = (0 - 1);
  if ((r3 != _sv0t12)) {
    return 3;
  } else {
  }
  int _sv0t13 = (0 - 1);
  int _sv0t14 = expr_init_cty(et, ed1, ed2, ed3, ed4, _sv0t13, pp, fnn, frc, al);
  int r4 = _sv0t14;
  int _sv0t15 = (0 - 1);
  if ((r4 != _sv0t15)) {
    return 4;
  } else {
  }
  return 0;
}

static int test_try_variant_lookup(void) {
  const char* src;
  src = "Ok Err Some None Other";
  int _sv0t0 = sv0_vec_new();
  int st = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int nd = _sv0t1;
  sv0_vec_push(st, 0);
  sv0_vec_push(nd, 2);
  sv0_vec_push(st, 3);
  sv0_vec_push(nd, 6);
  sv0_vec_push(st, 7);
  sv0_vec_push(nd, 11);
  sv0_vec_push(st, 12);
  sv0_vec_push(nd, 16);
  sv0_vec_push(st, 17);
  sv0_vec_push(nd, 22);
  int _sv0t2 = sv0_vec_new();
  int en = _sv0t2;
  int _sv0t3 = sv0_vec_new();
  int eto = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int etc = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int etf = _sv0t5;
  int _sv0t6 = sv0_vec_new();
  int emp = _sv0t6;
  sv0_vec_push(en, 100);
  sv0_vec_push(eto, 0);
  sv0_vec_push(etc, 2);
  sv0_vec_push(emp, 1);
  sv0_vec_push(etf, 0);
  sv0_vec_push(etf, 0);
  sv0_vec_push(etf, 1);
  sv0_vec_push(etf, 1);
  int _sv0t7 = sv0_vec_new();
  int out1 = _sv0t7;
  int _sv0t8 = try_variant_names_lookup(en, eto, etc, etf, src, st, nd, 100, out1);
  if ((_sv0t8 != 1)) {
    return 1;
  } else {
  }
  int _sv0t9 = sv0_vec_len(out1);
  if ((_sv0t9 != 2)) {
    return 2;
  } else {
  }
  int _sv0t10 = sv0_vec_get(out1, 0);
  if ((_sv0t10 != 0)) {
    return 3;
  } else {
  }
  int _sv0t11 = sv0_vec_get(out1, 1);
  if ((_sv0t11 != 1)) {
    return 4;
  } else {
  }
  int _sv0t12 = sv0_vec_new();
  int en2 = _sv0t12;
  int _sv0t13 = sv0_vec_new();
  int eto2 = _sv0t13;
  int _sv0t14 = sv0_vec_new();
  int etc2 = _sv0t14;
  int _sv0t15 = sv0_vec_new();
  int etf2 = _sv0t15;
  sv0_vec_push(en2, 200);
  sv0_vec_push(eto2, 0);
  sv0_vec_push(etc2, 2);
  sv0_vec_push(etf2, 2);
  sv0_vec_push(etf2, 0);
  sv0_vec_push(etf2, 3);
  sv0_vec_push(etf2, 1);
  int _sv0t16 = sv0_vec_new();
  int out2 = _sv0t16;
  int _sv0t17 = try_variant_names_lookup(en2, eto2, etc2, etf2, src, st, nd, 200, out2);
  if ((_sv0t17 != 1)) {
    return 5;
  } else {
  }
  int _sv0t18 = sv0_vec_get(out2, 0);
  if ((_sv0t18 != 2)) {
    return 6;
  } else {
  }
  int _sv0t19 = sv0_vec_get(out2, 1);
  if ((_sv0t19 != 3)) {
    return 7;
  } else {
  }
  int _sv0t20 = sv0_vec_new();
  int en3 = _sv0t20;
  int _sv0t21 = sv0_vec_new();
  int eto3 = _sv0t21;
  int _sv0t22 = sv0_vec_new();
  int etc3 = _sv0t22;
  int _sv0t23 = sv0_vec_new();
  int etf3 = _sv0t23;
  sv0_vec_push(en3, 300);
  sv0_vec_push(eto3, 0);
  sv0_vec_push(etc3, 2);
  sv0_vec_push(etf3, 0);
  sv0_vec_push(etf3, 0);
  sv0_vec_push(etf3, 4);
  sv0_vec_push(etf3, 1);
  int _sv0t24 = sv0_vec_new();
  int out3 = _sv0t24;
  int _sv0t25 = try_variant_names_lookup(en3, eto3, etc3, etf3, src, st, nd, 300, out3);
  if ((_sv0t25 != 0)) {
    return 8;
  } else {
  }
  return 0;
}

static int test_find_variant_in_items(void) {
  int _sv0t0 = sv0_vec_new();
  int tags = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int nms = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int fcs = _sv0t2;
  int _sv0t3 = sv0_vec_new();
  int vnf = _sv0t3;
  sv0_vec_push(tags, 1);
  sv0_vec_push(nms, 10);
  sv0_vec_push(fcs, 2);
  sv0_vec_push(tags, 2);
  sv0_vec_push(nms, 20);
  sv0_vec_push(fcs, 3);
  sv0_vec_push(vnf, 30);
  sv0_vec_push(vnf, 31);
  sv0_vec_push(vnf, 32);
  int _sv0t4 = find_variant_in_items(tags, nms, fcs, vnf, 20, 30);
  if ((_sv0t4 != 0)) {
    return 1;
  } else {
  }
  int _sv0t5 = find_variant_in_items(tags, nms, fcs, vnf, 20, 32);
  if ((_sv0t5 != 2)) {
    return 2;
  } else {
  }
  int _sv0t6 = find_variant_in_items(tags, nms, fcs, vnf, 20, 99);
  int _sv0t7 = (0 - 2);
  if ((_sv0t6 != _sv0t7)) {
    return 3;
  } else {
  }
  int _sv0t8 = find_variant_in_items(tags, nms, fcs, vnf, 99, 30);
  int _sv0t9 = (0 - 1);
  if ((_sv0t8 != _sv0t9)) {
    return 4;
  } else {
  }
  return 0;
}

static int test_scan_lets(void) {
  int _sv0t0 = sv0_vec_new();
  int et = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int ed1 = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int ed2 = _sv0t2;
  int _sv0t3 = sv0_vec_new();
  int ed3 = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int ed4 = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int pp = _sv0t5;
  int _sv0t6 = sv0_vec_new();
  int fnn = _sv0t6;
  int _sv0t7 = sv0_vec_new();
  int frc = _sv0t7;
  int _sv0t8 = sv0_vec_new();
  int al = _sv0t8;
  sv0_vec_push(et, 0);
  sv0_vec_push(ed1, 5);
  sv0_vec_push(ed2, 0);
  sv0_vec_push(ed3, 0);
  sv0_vec_push(ed4, 0);
  sv0_vec_push(et, 27);
  sv0_vec_push(ed1, 42);
  int _sv0t9 = (0 - 1);
  sv0_vec_push(ed2, _sv0t9);
  sv0_vec_push(ed3, 0);
  sv0_vec_push(ed4, 0);
  sv0_vec_push(et, 9);
  sv0_vec_push(ed1, 1);
  sv0_vec_push(ed2, 1);
  int _sv0t10 = (0 - 1);
  sv0_vec_push(ed3, _sv0t10);
  sv0_vec_push(ed4, 0);
  int _sv0t11 = sv0_vec_new();
  int on = _sv0t11;
  int _sv0t12 = sv0_vec_new();
  int oc = _sv0t12;
  int _sv0t13 = scan_lets(et, ed1, ed2, ed3, ed4, 2, pp, fnn, frc, al, on, oc);
  int c = _sv0t13;
  if ((c != 1)) {
    return 1;
  } else {
  }
  int _sv0t14 = sv0_vec_get(on, 0);
  if ((_sv0t14 != 42)) {
    return 2;
  } else {
  }
  int _sv0t15 = sv0_vec_get(oc, 0);
  int _sv0t16 = (0 - 1);
  if ((_sv0t15 != _sv0t16)) {
    return 3;
  } else {
  }
  int _sv0t17 = sv0_vec_new();
  int on2 = _sv0t17;
  int _sv0t18 = sv0_vec_new();
  int oc2 = _sv0t18;
  int _sv0t19 = (0 - 1);
  int _sv0t20 = scan_lets(et, ed1, ed2, ed3, ed4, _sv0t19, pp, fnn, frc, al, on2, oc2);
  int c2 = _sv0t20;
  if ((c2 != 0)) {
    return 4;
  } else {
  }
  int _sv0t21 = scan_lets(et, ed1, ed2, ed3, ed4, 0, pp, fnn, frc, al, on2, oc2);
  int c3 = _sv0t21;
  if ((c3 != 0)) {
    return 5;
  } else {
  }
  return 0;
}

static int test_scrut_locals_push(void) {
  int _sv0t0 = sv0_vec_new();
  int sn = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int st = _sv0t1;
  sv0_vec_push(sn, 100);
  sv0_vec_push(st, 200);
  int _sv0t2 = sv0_vec_new();
  int en = _sv0t2;
  int _sv0t3 = sv0_vec_new();
  int et = _sv0t3;
  sv0_vec_push(en, 300);
  sv0_vec_push(et, 400);
  int _sv0t4 = scrut_locals_push(sn, st, en, et);
  int saved = _sv0t4;
  if ((saved != 1)) {
    return 1;
  } else {
  }
  int _sv0t5 = sv0_vec_len(sn);
  if ((_sv0t5 != 2)) {
    return 2;
  } else {
  }
  int _sv0t6 = sv0_vec_get(sn, 1);
  if ((_sv0t6 != 300)) {
    return 3;
  } else {
  }
  int _sv0t7 = scrut_local_lookup_bounded(sn, saved, 300);
  int r1 = _sv0t7;
  int _sv0t8 = (0 - 1);
  if ((r1 != _sv0t8)) {
    return 4;
  } else {
  }
  int _sv0t9 = scrut_local_lookup_bounded(sn, 2, 300);
  int r2 = _sv0t9;
  if ((r2 != 1)) {
    return 5;
  } else {
  }
  int _sv0t10 = scrut_local_lookup_bounded(sn, 2, 100);
  int r3 = _sv0t10;
  if ((r3 != 0)) {
    return 6;
  } else {
  }
  return 0;
}

static int test_alias_replace(void) {
  int _sv0t0 = sv0_vec_new();
  int from = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int to = _sv0t1;
  sv0_vec_push(from, 10);
  sv0_vec_push(from, 20);
  sv0_vec_push(to, 11);
  sv0_vec_push(to, 21);
  int _sv0t2 = lower_alias_new();
  int tgt = _sv0t2;
  int _sv0t3 = lower_alias_replace(tgt, from, to);
  int c = _sv0t3;
  if ((c != 2)) {
    return 1;
  } else {
  }
  int _sv0t4 = sv0_vec_len(tgt);
  if ((_sv0t4 != 4)) {
    return 2;
  } else {
  }
  int _sv0t5 = lower_alias_lookup(tgt, 10);
  int r1 = _sv0t5;
  if ((r1 != 11)) {
    return 3;
  } else {
  }
  int _sv0t6 = lower_alias_lookup(tgt, 20);
  int r2 = _sv0t6;
  if ((r2 != 21)) {
    return 4;
  } else {
  }
  int _sv0t7 = lower_alias_lookup(tgt, 99);
  int r3 = _sv0t7;
  int _sv0t8 = (0 - 1);
  if ((r3 != _sv0t8)) {
    return 5;
  } else {
  }
  return 0;
}

static int test_fill_import_aliases_from_use_items(void) {
  int _sv0t0 = sv0_vec_new();
  int item_tags = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int item_names = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int item_d2 = _sv0t2;
  int _sv0t3 = sv0_vec_new();
  int item_d3 = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int pp = _sv0t4;
  int _sv0t5 = lower_alias_new();
  int aliases = _sv0t5;
  const char* source;
  source = "mod foo mod__foo";
  int _sv0t6 = sv0_vec_new();
  int starts = _sv0t6;
  int _sv0t7 = sv0_vec_new();
  int ends = _sv0t7;
  sv0_vec_push(starts, 0);
  sv0_vec_push(ends, 3);
  sv0_vec_push(starts, 4);
  sv0_vec_push(ends, 7);
  sv0_vec_push(starts, 8);
  sv0_vec_push(ends, 16);
  sv0_vec_push(item_tags, 0);
  sv0_vec_push(item_names, 2);
  sv0_vec_push(item_d2, 0);
  sv0_vec_push(item_d3, 0);
  sv0_vec_push(pp, 0);
  sv0_vec_push(pp, 1);
  sv0_vec_push(item_tags, 5);
  sv0_vec_push(item_names, 0);
  sv0_vec_push(item_d2, 0);
  sv0_vec_push(item_d3, 2);
  int _sv0t8 = lower_fill_import_aliases_from_use_items(aliases, item_tags, item_names, item_d2, item_d3, pp, source, starts, ends);
  int c = _sv0t8;
  if ((c != 1)) {
    return 1;
  } else {
  }
  int _sv0t9 = lower_alias_lookup(aliases, 1);
  if ((_sv0t9 != 2)) {
    return 2;
  } else {
  }
  int _sv0t10 = lower_fill_import_aliases_from_use_items(aliases, item_tags, item_names, item_d2, item_d3, pp, source, starts, ends);
  int c2 = _sv0t10;
  if ((c2 != 1)) {
    return 3;
  } else {
  }
  int _sv0t11 = sv0_vec_len(aliases);
  if ((_sv0t11 != 2)) {
    return 4;
  } else {
  }
  return 0;
}

static int test_fill_import_aliases_merges_with_preload(void) {
  int _sv0t0 = sv0_vec_new();
  int item_tags = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int item_names = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int item_d2 = _sv0t2;
  int _sv0t3 = sv0_vec_new();
  int item_d3 = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int pp = _sv0t4;
  int _sv0t5 = lower_alias_new();
  int aliases = _sv0t5;
  const char* source;
  source = "mod foo mod__foo other__fn";
  int _sv0t6 = sv0_vec_new();
  int starts = _sv0t6;
  int _sv0t7 = sv0_vec_new();
  int ends = _sv0t7;
  sv0_vec_push(starts, 0);
  sv0_vec_push(ends, 3);
  sv0_vec_push(starts, 4);
  sv0_vec_push(ends, 7);
  sv0_vec_push(starts, 8);
  sv0_vec_push(ends, 16);
  sv0_vec_push(starts, 17);
  sv0_vec_push(ends, 25);
  sv0_vec_push(item_tags, 0);
  sv0_vec_push(item_names, 2);
  sv0_vec_push(item_d2, 0);
  sv0_vec_push(item_d3, 0);
  sv0_vec_push(item_tags, 0);
  sv0_vec_push(item_names, 3);
  sv0_vec_push(item_d2, 0);
  sv0_vec_push(item_d3, 0);
  sv0_vec_push(pp, 0);
  sv0_vec_push(pp, 1);
  sv0_vec_push(item_tags, 5);
  sv0_vec_push(item_names, 0);
  sv0_vec_push(item_d2, 0);
  sv0_vec_push(item_d3, 2);
  int _sv0t8 = lower_alias_add(aliases, 9, 10);
  int discard_pre = _sv0t8;
  int _sv0t9 = lower_fill_import_aliases_from_use_items(aliases, item_tags, item_names, item_d2, item_d3, pp, source, starts, ends);
  int c = _sv0t9;
  if ((c != 2)) {
    return 1;
  } else {
  }
  int _sv0t10 = lower_alias_lookup(aliases, 9);
  if ((_sv0t10 != 10)) {
    return 2;
  } else {
  }
  int _sv0t11 = lower_alias_lookup(aliases, 1);
  if ((_sv0t11 != 2)) {
    return 3;
  } else {
  }
  int _sv0t12 = lower_alias_lookup(aliases, 1);
  if ((_sv0t12 == 10)) {
    return 4;
  } else {
  }
  return 0;
}

static int test_match_scrut_cty(void) {
  int _sv0t0 = lower_match_env_alloc_v0_empty_header();
  int me = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int et = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int ed1 = _sv0t2;
  int _sv0t3 = sv0_vec_new();
  int ed2 = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int ed3 = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int ed4 = _sv0t5;
  int _sv0t6 = sv0_vec_new();
  int pp = _sv0t6;
  int _sv0t7 = sv0_vec_new();
  int al = _sv0t7;
  int _sv0t8 = sv0_vec_new();
  int sn = _sv0t8;
  int _sv0t9 = sv0_vec_new();
  int sc = _sv0t9;
  sv0_vec_push(pp, 42);
  sv0_vec_push(pp, 99);
  sv0_vec_push(et, 1);
  sv0_vec_push(ed1, 0);
  sv0_vec_push(ed2, 2);
  sv0_vec_push(ed3, 0);
  sv0_vec_push(ed4, 0);
  int _sv0t10 = match_scrut_cty(et, ed1, ed2, ed3, ed4, 0, pp, me, al, sn, sc, 0);
  int r1 = _sv0t10;
  if ((r1 != 42)) {
    return 1;
  } else {
  }
  sv0_vec_push(et, 1);
  sv0_vec_push(ed1, 0);
  sv0_vec_push(ed2, 1);
  sv0_vec_push(ed3, 0);
  sv0_vec_push(ed4, 0);
  sv0_vec_push(sn, 42);
  sv0_vec_push(sc, 77);
  int _sv0t11 = match_scrut_cty(et, ed1, ed2, ed3, ed4, 1, pp, me, al, sn, sc, 1);
  int r2 = _sv0t11;
  if ((r2 != 77)) {
    return 2;
  } else {
  }
  int _sv0t12 = (0 - 1);
  int _sv0t13 = match_scrut_cty(et, ed1, ed2, ed3, ed4, _sv0t12, pp, me, al, sn, sc, 0);
  int r3 = _sv0t13;
  int _sv0t14 = (0 - 1);
  if ((r3 != _sv0t14)) {
    return 3;
  } else {
  }
  sv0_vec_push(et, 0);
  sv0_vec_push(ed1, 0);
  sv0_vec_push(ed2, 0);
  sv0_vec_push(ed3, 0);
  sv0_vec_push(ed4, 0);
  int _sv0t15 = match_scrut_cty(et, ed1, ed2, ed3, ed4, 2, pp, me, al, sn, sc, 0);
  int r4 = _sv0t15;
  int _sv0t16 = (0 - 1);
  if ((r4 != _sv0t16)) {
    return 4;
  } else {
  }
  sv0_vec_push(et, 4);
  sv0_vec_push(ed1, 1);
  sv0_vec_push(ed2, 0);
  sv0_vec_push(ed3, 0);
  sv0_vec_push(ed4, 0);
  int _sv0t17 = sv0_vec_len(me);
  int bfn = _sv0t17;
  sv0_vec_push(me, 42);
  int _sv0t18 = sv0_vec_len(me);
  int brt = _sv0t18;
  sv0_vec_push(me, 200);
  sv0_vec_set(me, 2, bfn);
  sv0_vec_set(me, 3, brt);
  sv0_vec_set(me, 4, 1);
  int _sv0t19 = match_scrut_cty(et, ed1, ed2, ed3, ed4, 3, pp, me, al, sn, sc, 0);
  int r5 = _sv0t19;
  if ((r5 != 200)) {
    return 5;
  } else {
  }
  int _sv0t20 = lower_match_env_alloc_v0_empty_header();
  int me2 = _sv0t20;
  int _sv0t21 = lower_match_env_append_scrut_pair(me2, 42, 200);
  int ap2 = _sv0t21;
  if ((ap2 != 0)) {
    return 6;
  } else {
  }
  int _sv0t22 = match_scrut_cty(et, ed1, ed2, ed3, ed4, 1, pp, me2, al, sn, sc, 0);
  int r6 = _sv0t22;
  if ((r6 != 200)) {
    return 7;
  } else {
  }
  return 0;
}

static int test_variant_shape_in_items(void) {
  int _sv0t0 = sv0_vec_new();
  int tags = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int nms = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int fcs = _sv0t2;
  int _sv0t3 = sv0_vec_new();
  int vsf = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int vff = _sv0t4;
  sv0_vec_push(tags, 2);
  sv0_vec_push(nms, 10);
  sv0_vec_push(fcs, 2);
  sv0_vec_push(vsf, 0);
  sv0_vec_push(vff, 0);
  sv0_vec_push(vsf, 1);
  sv0_vec_push(vff, 3);
  int _sv0t5 = sv0_vec_new();
  int oc = _sv0t5;
  int _sv0t6 = variant_shape_in_items(tags, nms, fcs, vsf, vff, 10, 0, oc);
  int s1 = _sv0t6;
  if ((s1 != 0)) {
    return 1;
  } else {
  }
  int _sv0t7 = sv0_vec_len(oc);
  if ((_sv0t7 != 1)) {
    return 2;
  } else {
  }
  int _sv0t8 = sv0_vec_get(oc, 0);
  if ((_sv0t8 != 0)) {
    return 3;
  } else {
  }
  int _sv0t9 = sv0_vec_new();
  int oc2 = _sv0t9;
  int _sv0t10 = variant_shape_in_items(tags, nms, fcs, vsf, vff, 10, 1, oc2);
  int s2 = _sv0t10;
  if ((s2 != 1)) {
    return 4;
  } else {
  }
  int _sv0t11 = sv0_vec_get(oc2, 0);
  if ((_sv0t11 != 3)) {
    return 5;
  } else {
  }
  int _sv0t12 = sv0_vec_new();
  int oc3 = _sv0t12;
  int _sv0t13 = variant_shape_in_items(tags, nms, fcs, vsf, vff, 99, 0, oc3);
  int s3 = _sv0t13;
  int _sv0t14 = (0 - 1);
  if ((s3 != _sv0t14)) {
    return 6;
  } else {
  }
  return 0;
}

static int test_ir_types(void) {
  Value vi;
  vi.tag = 0;
  vi.p0 = 42;
  int _sv0t0 = ir_value_tag(vi);
  if ((_sv0t0 != 0)) {
    return 1;
  } else {
  }
  Value vb;
  vb.tag = 1;
  int _sv0t1 = ir_value_tag(vb);
  if ((_sv0t1 != 1)) {
    return 2;
  } else {
  }
  Value vf;
  vf.tag = 2;
  int _sv0t2 = ir_value_tag(vf);
  if ((_sv0t2 != 2)) {
    return 3;
  } else {
  }
  Value vv;
  vv.tag = 3;
  vv.p0 = 10;
  int _sv0t3 = ir_value_tag(vv);
  if ((_sv0t3 != 3)) {
    return 4;
  } else {
  }
  Value vu;
  vu.tag = 4;
  int _sv0t4 = ir_value_tag(vu);
  if ((_sv0t4 != 4)) {
    return 5;
  } else {
  }
  Value vs;
  vs.tag = 5;
  vs.p0 = 7;
  int _sv0t5 = ir_value_tag(vs);
  if ((_sv0t5 != 5)) {
    return 6;
  } else {
  }
  Value base;
  base.tag = 3;
  base.p0 = 3;
  Value vm;
  int _sv0t6 = sv0_box_alloc(3);
  sv0_box_store(_sv0t6, 0, base.tag);
  sv0_box_store(_sv0t6, 1, base.p0);
  sv0_box_store(_sv0t6, 2, base.p1);
  vm.tag = 6;
  vm.p0 = _sv0t6;
  vm.p1 = 5;
  int _sv0t7 = ir_value_tag(vm);
  if ((_sv0t7 != 6)) {
    return 7;
  } else {
  }
  Value va;
  va.tag = 7;
  va.p0 = 9;
  int _sv0t8 = ir_value_tag(va);
  if ((_sv0t8 != 7)) {
    return 8;
  } else {
  }
  Expr el;
  Value _sv0t9;
  _sv0t9.tag = 0;
  _sv0t9.p0 = 5;
  int _sv0t10 = sv0_box_alloc(3);
  sv0_box_store(_sv0t10, 0, _sv0t9.tag);
  sv0_box_store(_sv0t10, 1, _sv0t9.p0);
  sv0_box_store(_sv0t10, 2, _sv0t9.p1);
  el.tag = 0;
  el.p0 = _sv0t10;
  int _sv0t11 = ir_expr_tag(el);
  if ((_sv0t11 != 0)) {
    return 9;
  } else {
  }
  Expr eld;
  eld.tag = 1;
  eld.p0 = 50;
  int _sv0t12 = ir_expr_tag(eld);
  if ((_sv0t12 != 1)) {
    return 10;
  } else {
  }
  Expr eb;
  Value _sv0t13;
  _sv0t13.tag = 0;
  _sv0t13.p0 = 3;
  int _sv0t14 = sv0_box_alloc(3);
  sv0_box_store(_sv0t14, 0, _sv0t13.tag);
  sv0_box_store(_sv0t14, 1, _sv0t13.p0);
  sv0_box_store(_sv0t14, 2, _sv0t13.p1);
  Value _sv0t15;
  _sv0t15.tag = 0;
  _sv0t15.p0 = 4;
  int _sv0t16 = sv0_box_alloc(3);
  sv0_box_store(_sv0t16, 0, _sv0t15.tag);
  sv0_box_store(_sv0t16, 1, _sv0t15.p0);
  sv0_box_store(_sv0t16, 2, _sv0t15.p1);
  eb.tag = 2;
  eb.p0 = 20;
  eb.p1 = _sv0t14;
  eb.p2 = _sv0t16;
  int _sv0t17 = ir_expr_tag(eb);
  if ((_sv0t17 != 2)) {
    return 11;
  } else {
  }
  Expr eu;
  Value _sv0t18;
  _sv0t18.tag = 1;
  int _sv0t19 = sv0_box_alloc(3);
  sv0_box_store(_sv0t19, 0, _sv0t18.tag);
  sv0_box_store(_sv0t19, 1, _sv0t18.p0);
  sv0_box_store(_sv0t19, 2, _sv0t18.p1);
  eu.tag = 3;
  eu.p0 = 1;
  eu.p1 = _sv0t19;
  int _sv0t20 = ir_expr_tag(eu);
  if ((_sv0t20 != 3)) {
    return 12;
  } else {
  }
  Expr ef;
  Value _sv0t21;
  _sv0t21.tag = 3;
  _sv0t21.p0 = 60;
  int _sv0t22 = sv0_box_alloc(3);
  sv0_box_store(_sv0t22, 0, _sv0t21.tag);
  sv0_box_store(_sv0t22, 1, _sv0t21.p0);
  sv0_box_store(_sv0t22, 2, _sv0t21.p1);
  ef.tag = 4;
  ef.p0 = _sv0t22;
  ef.p1 = 61;
  int _sv0t23 = ir_expr_tag(ef);
  if ((_sv0t23 != 4)) {
    return 13;
  } else {
  }
  Instr i_nop;
  i_nop.tag = 0;
  int _sv0t24 = ir_instr_tag(i_nop);
  if ((_sv0t24 != 0)) {
    return 14;
  } else {
  }
  Instr i_dv;
  i_dv.tag = 1;
  i_dv.p0 = 50;
  int _sv0t25 = ir_instr_tag(i_dv);
  if ((_sv0t25 != 1)) {
    return 15;
  } else {
  }
  Instr i_brk;
  i_brk.tag = 9;
  int _sv0t26 = ir_instr_tag(i_brk);
  if ((_sv0t26 != 9)) {
    return 16;
  } else {
  }
  Instr i_cont;
  i_cont.tag = 10;
  int _sv0t27 = ir_instr_tag(i_cont);
  if ((_sv0t27 != 10)) {
    return 17;
  } else {
  }
  Instr i_rv;
  i_rv.tag = 14;
  int _sv0t28 = ir_instr_tag(i_rv);
  if ((_sv0t28 != 14)) {
    return 18;
  } else {
  }
  Instr i_ret;
  Value _sv0t29;
  _sv0t29.tag = 0;
  _sv0t29.p0 = 0;
  int _sv0t30 = sv0_box_alloc(3);
  sv0_box_store(_sv0t30, 0, _sv0t29.tag);
  sv0_box_store(_sv0t30, 1, _sv0t29.p0);
  sv0_box_store(_sv0t30, 2, _sv0t29.p1);
  i_ret.tag = 15;
  i_ret.p0 = _sv0t30;
  int _sv0t31 = ir_instr_tag(i_ret);
  if ((_sv0t31 != 15)) {
    return 19;
  } else {
  }
  Expr asgn_e;
  asgn_e.tag = 1;
  asgn_e.p0 = 70;
  Instr i_asgn;
  int _sv0t32 = sv0_box_alloc(4);
  sv0_box_store(_sv0t32, 0, asgn_e.tag);
  sv0_box_store(_sv0t32, 1, asgn_e.p0);
  sv0_box_store(_sv0t32, 2, asgn_e.p1);
  sv0_box_store(_sv0t32, 3, asgn_e.p2);
  i_asgn.tag = 3;
  i_asgn.p0 = 71;
  i_asgn.p1 = _sv0t32;
  int _sv0t33 = ir_instr_tag(i_asgn);
  if ((_sv0t33 != 3)) {
    return 20;
  } else {
  }
  return 0;
}

static int test_ctr_fresh(void) {
  int _sv0t0 = sv0_vec_new();
  int ctr = _sv0t0;
  sv0_vec_push(ctr, 0);
  int _sv0t1 = ctr_fresh(ctr);
  int t0 = _sv0t1;
  if ((t0 != 0)) {
    return 1;
  } else {
  }
  int _sv0t2 = ctr_fresh(ctr);
  int t1 = _sv0t2;
  if ((t1 != 1)) {
    return 2;
  } else {
  }
  int _sv0t3 = ctr_fresh(ctr);
  int t2 = _sv0t3;
  if ((t2 != 2)) {
    return 3;
  } else {
  }
  int _sv0t4 = sv0_vec_get(ctr, 0);
  if ((_sv0t4 != 3)) {
    return 4;
  } else {
  }
  return 0;
}

static int test_classify_arena(void) {
  int _sv0t0 = sv0_vec_new();
  int et = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int ed1 = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int ed2 = _sv0t2;
  int _sv0t3 = sv0_vec_new();
  int pp = _sv0t3;
  sv0_vec_push(pp, 42);
  sv0_vec_push(et, 1);
  sv0_vec_push(ed1, 0);
  sv0_vec_push(ed2, 1);
  int _sv0t4 = sv0_vec_new();
  int out1 = _sv0t4;
  int _sv0t5 = classify_assign_lhs_arena(et, ed1, ed2, pp, 0, out1);
  int r1 = _sv0t5;
  if ((r1 != 0)) {
    return 1;
  } else {
  }
  int _sv0t6 = sv0_vec_len(out1);
  if ((_sv0t6 != 1)) {
    return 2;
  } else {
  }
  int _sv0t7 = sv0_vec_get(out1, 0);
  if ((_sv0t7 != 42)) {
    return 3;
  } else {
  }
  sv0_vec_push(et, 6);
  sv0_vec_push(ed1, 0);
  sv0_vec_push(ed2, 99);
  int _sv0t8 = sv0_vec_new();
  int out2 = _sv0t8;
  int _sv0t9 = classify_assign_lhs_arena(et, ed1, ed2, pp, 1, out2);
  int r2 = _sv0t9;
  if ((r2 != 1)) {
    return 4;
  } else {
  }
  int _sv0t10 = sv0_vec_len(out2);
  if ((_sv0t10 != 2)) {
    return 5;
  } else {
  }
  int _sv0t11 = sv0_vec_get(out2, 0);
  if ((_sv0t11 != 42)) {
    return 6;
  } else {
  }
  int _sv0t12 = sv0_vec_get(out2, 1);
  if ((_sv0t12 != 99)) {
    return 7;
  } else {
  }
  sv0_vec_push(et, 3);
  sv0_vec_push(ed1, 0);
  sv0_vec_push(ed2, 0);
  int _sv0t13 = sv0_vec_new();
  int out3 = _sv0t13;
  int _sv0t14 = classify_assign_lhs_arena(et, ed1, ed2, pp, 2, out3);
  int r3 = _sv0t14;
  int _sv0t15 = (0 - 1);
  if ((r3 != _sv0t15)) {
    return 8;
  } else {
  }
  return 0;
}

static int test_parse_decimal_i32_from_tok(void) {
  const char* src;
  src = "let x = 12045;";
  int _sv0t0 = sv0_vec_new();
  int st = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int en = _sv0t1;
  sv0_vec_push(st, 8);
  sv0_vec_push(en, 13);
  int _sv0t2 = parse_decimal_i32_from_tok(src, st, en, 0);
  int v = _sv0t2;
  if ((v != 12045)) {
    return 1;
  } else {
  }
  int _sv0t3 = parse_decimal_i32_from_tok(src, st, en, 1);
  int v2 = _sv0t3;
  if ((v2 != 0)) {
    return 2;
  } else {
  }
  int _sv0t4 = (0 - 1);
  int _sv0t5 = parse_decimal_i32_from_tok(src, st, en, _sv0t4);
  int v3 = _sv0t5;
  if ((v3 != 0)) {
    return 3;
  } else {
  }
  return 0;
}

static int test_lower_lit_val(void) {
  int _sv0t0 = sv0_vec_new();
  int tok_tags = _sv0t0;
  sv0_vec_push(tok_tags, 91);
  sv0_vec_push(tok_tags, 64);
  sv0_vec_push(tok_tags, 0);
  const char* lit_src;
  lit_src = "";
  int _sv0t1 = sv0_vec_new();
  int lit_st = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int lit_en = _sv0t2;
  Value _sv0t3 = lower_lit_to_ir_value(0, 2, tok_tags, lit_src, lit_st, lit_en);
  Value v1;
  v1 = _sv0t3;
  int _sv0t4 = ir_value_tag(v1);
  if ((_sv0t4 != 0)) {
    return 1;
  } else {
  }
  Value _sv0t5 = lower_lit_to_ir_value(5, 0, tok_tags, lit_src, lit_st, lit_en);
  Value v2;
  v2 = _sv0t5;
  int _sv0t6 = ir_value_tag(v2);
  if ((_sv0t6 != 1)) {
    return 2;
  } else {
  }
  Value _sv0t7 = lower_lit_to_ir_value(5, 1, tok_tags, lit_src, lit_st, lit_en);
  Value v3;
  v3 = _sv0t7;
  int _sv0t8 = ir_value_tag(v3);
  if ((_sv0t8 != 2)) {
    return 3;
  } else {
  }
  Value _sv0t9 = lower_lit_to_ir_value(3, 2, tok_tags, lit_src, lit_st, lit_en);
  Value v4;
  v4 = _sv0t9;
  int _sv0t10 = ir_value_tag(v4);
  if ((_sv0t10 != 5)) {
    return 4;
  } else {
  }
  Value _sv0t11 = lower_lit_to_ir_value(99, 0, tok_tags, lit_src, lit_st, lit_en);
  Value v5;
  v5 = _sv0t11;
  int _sv0t12 = ir_value_tag(v5);
  if ((_sv0t12 != 4)) {
    return 5;
  } else {
  }
  return 0;
}

static int test_lower_expr_val(void) {
  int _sv0t0 = sv0_vec_new();
  int et = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int ed1 = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int ed2 = _sv0t2;
  int _sv0t3 = sv0_vec_new();
  int ed3 = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int ed4 = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int pp = _sv0t5;
  int _sv0t6 = sv0_vec_new();
  int tok_tags = _sv0t6;
  sv0_vec_push(tok_tags, 91);
  sv0_vec_push(pp, 77);
  int _sv0t7 = sv0_vec_new();
  int ctr = _sv0t7;
  sv0_vec_push(ctr, 0);
  int _sv0t8 = sv0_vec_new();
  int out = _sv0t8;
  int _sv0t9 = sv0_vec_new();
  int en_n = _sv0t9;
  int _sv0t10 = sv0_vec_new();
  int en_to = _sv0t10;
  int _sv0t11 = sv0_vec_new();
  int en_tc = _sv0t11;
  int _sv0t12 = sv0_vec_new();
  int en_tf = _sv0t12;
  int _sv0t13 = sv0_vec_new();
  int fn_ctx = _sv0t13;
  int _sv0t14 = sv0_vec_new();
  int builtin_map = _sv0t14;
  int _sv0t15 = sv0_vec_new();
  int lit_sf_names = _sv0t15;
  const char* source;
  source = "";
  int _sv0t16 = sv0_vec_new();
  int starts = _sv0t16;
  int _sv0t17 = sv0_vec_new();
  int ends = _sv0t17;
  int _sv0t18 = sv0_vec_new();
  int item_tags = _sv0t18;
  int _sv0t19 = sv0_vec_new();
  int item_names = _sv0t19;
  int _sv0t20 = sv0_vec_new();
  int item_d2 = _sv0t20;
  int _sv0t21 = sv0_vec_new();
  int item_d3 = _sv0t21;
  int _sv0t22 = sv0_vec_new();
  int item_d4 = _sv0t22;
  int item_fn_row = (0 - 1);
  int _sv0t23 = sv0_vec_new();
  int item_field_counts = _sv0t23;
  int _sv0t24 = sv0_vec_new();
  int enum_vnames_flat = _sv0t24;
  int _sv0t25 = sv0_vec_new();
  int enum_max_payloads = _sv0t25;
  sv0_vec_push(et, 0);
  sv0_vec_push(ed1, 0);
  sv0_vec_push(ed2, 0);
  sv0_vec_push(ed3, 0);
  sv0_vec_push(ed4, 0);
  int _sv0t26 = sv0_vec_new();
  int _sv0t27 = sv0_vec_new();
  Value _sv0t28 = lower_expr_to_value(et, ed1, ed2, ed3, ed4, pp, tok_tags, 0, ctr, out, en_n, en_to, en_tc, en_tf, fn_ctx, builtin_map, lit_sf_names, source, starts, ends, item_tags, item_names, item_d2, item_d3, item_d4, item_field_counts, enum_vnames_flat, enum_max_payloads, item_fn_row, _sv0t26, _sv0t27);
  Value v1;
  v1 = _sv0t28;
  int _sv0t29 = ir_value_tag(v1);
  if ((_sv0t29 != 0)) {
    return 1;
  } else {
  }
  sv0_vec_push(et, 1);
  sv0_vec_push(ed1, 0);
  sv0_vec_push(ed2, 1);
  sv0_vec_push(ed3, 0);
  sv0_vec_push(ed4, 0);
  int _sv0t30 = sv0_vec_new();
  int _sv0t31 = sv0_vec_new();
  Value _sv0t32 = lower_expr_to_value(et, ed1, ed2, ed3, ed4, pp, tok_tags, 1, ctr, out, en_n, en_to, en_tc, en_tf, fn_ctx, builtin_map, lit_sf_names, source, starts, ends, item_tags, item_names, item_d2, item_d3, item_d4, item_field_counts, enum_vnames_flat, enum_max_payloads, item_fn_row, _sv0t30, _sv0t31);
  Value v2;
  v2 = _sv0t32;
  int _sv0t33 = ir_value_tag(v2);
  if ((_sv0t33 != 3)) {
    return 2;
  } else {
  }
  sv0_vec_push(et, 3);
  sv0_vec_push(ed1, 7);
  sv0_vec_push(ed2, 0);
  sv0_vec_push(ed3, 1);
  sv0_vec_push(ed4, 0);
  int _sv0t34 = sv0_vec_new();
  int _sv0t35 = sv0_vec_new();
  Value _sv0t36 = lower_expr_to_value(et, ed1, ed2, ed3, ed4, pp, tok_tags, 2, ctr, out, en_n, en_to, en_tc, en_tf, fn_ctx, builtin_map, lit_sf_names, source, starts, ends, item_tags, item_names, item_d2, item_d3, item_d4, item_field_counts, enum_vnames_flat, enum_max_payloads, item_fn_row, _sv0t34, _sv0t35);
  Value v3;
  v3 = _sv0t36;
  int _sv0t37 = ir_value_tag(v3);
  if ((_sv0t37 != 3)) {
    return 3;
  } else {
  }
  int _sv0t38 = sv0_vec_len(out);
  if ((_sv0t38 != 1)) {
    return 4;
  } else {
  }
  int _sv0t39 = sv0_vec_get(ctr, 0);
  if ((_sv0t39 != 1)) {
    return 5;
  } else {
  }
  return 0;
}

static int test_lower_return(void) {
  int _sv0t0 = sv0_vec_new();
  int et = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int ed1 = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int ed2 = _sv0t2;
  int _sv0t3 = sv0_vec_new();
  int ed3 = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int ed4 = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int pp = _sv0t5;
  int _sv0t6 = sv0_vec_new();
  int tok_tags = _sv0t6;
  int _sv0t7 = sv0_vec_new();
  int ctr = _sv0t7;
  sv0_vec_push(ctr, 0);
  int _sv0t8 = sv0_vec_new();
  int out = _sv0t8;
  sv0_vec_push(et, 15);
  int _sv0t9 = (0 - 1);
  sv0_vec_push(ed1, _sv0t9);
  sv0_vec_push(ed2, 0);
  sv0_vec_push(ed3, 0);
  sv0_vec_push(ed4, 0);
  int _sv0t10 = sv0_vec_new();
  int en_n = _sv0t10;
  int _sv0t11 = sv0_vec_new();
  int en_to = _sv0t11;
  int _sv0t12 = sv0_vec_new();
  int en_tc = _sv0t12;
  int _sv0t13 = sv0_vec_new();
  int en_tf = _sv0t13;
  int _sv0t14 = sv0_vec_new();
  int fn_ctx = _sv0t14;
  int _sv0t15 = sv0_vec_new();
  int builtin_map = _sv0t15;
  int _sv0t16 = sv0_vec_new();
  int lit_sf_names = _sv0t16;
  const char* source;
  source = "";
  int _sv0t17 = sv0_vec_new();
  int starts = _sv0t17;
  int _sv0t18 = sv0_vec_new();
  int ends = _sv0t18;
  int _sv0t19 = sv0_vec_new();
  int item_tags = _sv0t19;
  int _sv0t20 = sv0_vec_new();
  int item_names = _sv0t20;
  int _sv0t21 = sv0_vec_new();
  int item_d2 = _sv0t21;
  int _sv0t22 = sv0_vec_new();
  int item_d3 = _sv0t22;
  int _sv0t23 = sv0_vec_new();
  int item_d4 = _sv0t23;
  int item_fn_row = (0 - 1);
  int _sv0t24 = sv0_vec_new();
  int item_field_counts = _sv0t24;
  int _sv0t25 = sv0_vec_new();
  int enum_vnames_flat = _sv0t25;
  int _sv0t26 = sv0_vec_new();
  int enum_max_payloads = _sv0t26;
  int _sv0t27 = sv0_vec_new();
  int _sv0t28 = sv0_vec_new();
  Value _sv0t29 = lower_expr_to_value(et, ed1, ed2, ed3, ed4, pp, tok_tags, 0, ctr, out, en_n, en_to, en_tc, en_tf, fn_ctx, builtin_map, lit_sf_names, source, starts, ends, item_tags, item_names, item_d2, item_d3, item_d4, item_field_counts, enum_vnames_flat, enum_max_payloads, item_fn_row, _sv0t27, _sv0t28);
  Value v1;
  v1 = _sv0t29;
  int _sv0t30 = ir_value_tag(v1);
  if ((_sv0t30 != 4)) {
    return 1;
  } else {
  }
  int _sv0t31 = sv0_vec_len(out);
  if ((_sv0t31 != 1)) {
    return 2;
  } else {
  }
  return 0;
}

static int test_lower_if(void) {
  int _sv0t0 = sv0_vec_new();
  int et = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int ed1 = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int ed2 = _sv0t2;
  int _sv0t3 = sv0_vec_new();
  int ed3 = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int ed4 = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int pp = _sv0t5;
  int _sv0t6 = sv0_vec_new();
  int tok_tags = _sv0t6;
  sv0_vec_push(tok_tags, 91);
  int _sv0t7 = sv0_vec_new();
  int ctr = _sv0t7;
  sv0_vec_push(ctr, 0);
  int _sv0t8 = sv0_vec_new();
  int out = _sv0t8;
  sv0_vec_push(et, 0);
  sv0_vec_push(ed1, 5);
  sv0_vec_push(ed2, 0);
  sv0_vec_push(ed3, 0);
  sv0_vec_push(ed4, 0);
  sv0_vec_push(et, 0);
  sv0_vec_push(ed1, 0);
  sv0_vec_push(ed2, 0);
  sv0_vec_push(ed3, 0);
  sv0_vec_push(ed4, 0);
  sv0_vec_push(et, 10);
  sv0_vec_push(ed1, 0);
  sv0_vec_push(ed2, 1);
  int _sv0t9 = (0 - 1);
  sv0_vec_push(ed3, _sv0t9);
  sv0_vec_push(ed4, 0);
  int _sv0t10 = sv0_vec_new();
  int en_n = _sv0t10;
  int _sv0t11 = sv0_vec_new();
  int en_to = _sv0t11;
  int _sv0t12 = sv0_vec_new();
  int en_tc = _sv0t12;
  int _sv0t13 = sv0_vec_new();
  int en_tf = _sv0t13;
  int _sv0t14 = sv0_vec_new();
  int fn_ctx = _sv0t14;
  int _sv0t15 = sv0_vec_new();
  int builtin_map = _sv0t15;
  int _sv0t16 = sv0_vec_new();
  int lit_sf_names = _sv0t16;
  const char* source;
  source = "";
  int _sv0t17 = sv0_vec_new();
  int starts = _sv0t17;
  int _sv0t18 = sv0_vec_new();
  int ends = _sv0t18;
  int _sv0t19 = sv0_vec_new();
  int item_tags = _sv0t19;
  int _sv0t20 = sv0_vec_new();
  int item_names = _sv0t20;
  int _sv0t21 = sv0_vec_new();
  int item_d2 = _sv0t21;
  int _sv0t22 = sv0_vec_new();
  int item_d3 = _sv0t22;
  int _sv0t23 = sv0_vec_new();
  int item_d4 = _sv0t23;
  int item_fn_row = (0 - 1);
  int _sv0t24 = sv0_vec_new();
  int item_field_counts = _sv0t24;
  int _sv0t25 = sv0_vec_new();
  int enum_vnames_flat = _sv0t25;
  int _sv0t26 = sv0_vec_new();
  int enum_max_payloads = _sv0t26;
  int _sv0t27 = sv0_vec_new();
  int _sv0t28 = sv0_vec_new();
  Value _sv0t29 = lower_expr_to_value(et, ed1, ed2, ed3, ed4, pp, tok_tags, 2, ctr, out, en_n, en_to, en_tc, en_tf, fn_ctx, builtin_map, lit_sf_names, source, starts, ends, item_tags, item_names, item_d2, item_d3, item_d4, item_field_counts, enum_vnames_flat, enum_max_payloads, item_fn_row, _sv0t27, _sv0t28);
  Value v1;
  v1 = _sv0t29;
  int _sv0t30 = ir_value_tag(v1);
  if ((_sv0t30 != 4)) {
    return 1;
  } else {
  }
  int _sv0t31 = sv0_vec_len(out);
  if ((_sv0t31 != 1)) {
    return 2;
  } else {
  }
  return 0;
}

static int test_lower_block(void) {
  int _sv0t0 = sv0_vec_new();
  int et = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int ed1 = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int ed2 = _sv0t2;
  int _sv0t3 = sv0_vec_new();
  int ed3 = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int ed4 = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int pp = _sv0t5;
  int _sv0t6 = sv0_vec_new();
  int tok_tags = _sv0t6;
  sv0_vec_push(tok_tags, 91);
  sv0_vec_push(pp, 42);
  int _sv0t7 = sv0_vec_new();
  int ctr = _sv0t7;
  sv0_vec_push(ctr, 0);
  int _sv0t8 = sv0_vec_new();
  int out = _sv0t8;
  sv0_vec_push(et, 0);
  sv0_vec_push(ed1, 0);
  sv0_vec_push(ed2, 0);
  sv0_vec_push(ed3, 0);
  sv0_vec_push(ed4, 0);
  sv0_vec_push(et, 9);
  sv0_vec_push(ed1, 0);
  sv0_vec_push(ed2, 0);
  sv0_vec_push(ed3, 0);
  sv0_vec_push(ed4, 0);
  int _sv0t9 = sv0_vec_new();
  int en_n = _sv0t9;
  int _sv0t10 = sv0_vec_new();
  int en_to = _sv0t10;
  int _sv0t11 = sv0_vec_new();
  int en_tc = _sv0t11;
  int _sv0t12 = sv0_vec_new();
  int en_tf = _sv0t12;
  int _sv0t13 = sv0_vec_new();
  int fn_ctx = _sv0t13;
  int _sv0t14 = sv0_vec_new();
  int builtin_map = _sv0t14;
  int _sv0t15 = sv0_vec_new();
  int lit_sf_names = _sv0t15;
  const char* source;
  source = "";
  int _sv0t16 = sv0_vec_new();
  int starts = _sv0t16;
  int _sv0t17 = sv0_vec_new();
  int ends = _sv0t17;
  int _sv0t18 = sv0_vec_new();
  int item_tags = _sv0t18;
  int _sv0t19 = sv0_vec_new();
  int item_names = _sv0t19;
  int _sv0t20 = sv0_vec_new();
  int item_d2 = _sv0t20;
  int _sv0t21 = sv0_vec_new();
  int item_d3 = _sv0t21;
  int _sv0t22 = sv0_vec_new();
  int item_d4 = _sv0t22;
  int item_fn_row = (0 - 1);
  int _sv0t23 = sv0_vec_new();
  int item_field_counts = _sv0t23;
  int _sv0t24 = sv0_vec_new();
  int enum_vnames_flat = _sv0t24;
  int _sv0t25 = sv0_vec_new();
  int enum_max_payloads = _sv0t25;
  int _sv0t26 = sv0_vec_new();
  int _sv0t27 = sv0_vec_new();
  Value _sv0t28 = lower_expr_to_value(et, ed1, ed2, ed3, ed4, pp, tok_tags, 1, ctr, out, en_n, en_to, en_tc, en_tf, fn_ctx, builtin_map, lit_sf_names, source, starts, ends, item_tags, item_names, item_d2, item_d3, item_d4, item_field_counts, enum_vnames_flat, enum_max_payloads, item_fn_row, _sv0t26, _sv0t27);
  Value v1;
  v1 = _sv0t28;
  int _sv0t29 = ir_value_tag(v1);
  if ((_sv0t29 != 0)) {
    return 1;
  } else {
  }
  sv0_vec_push(et, 9);
  sv0_vec_push(ed1, 0);
  sv0_vec_push(ed2, 0);
  int _sv0t30 = (0 - 1);
  sv0_vec_push(ed3, _sv0t30);
  sv0_vec_push(ed4, 0);
  int _sv0t31 = sv0_vec_new();
  int _sv0t32 = sv0_vec_new();
  Value _sv0t33 = lower_expr_to_value(et, ed1, ed2, ed3, ed4, pp, tok_tags, 2, ctr, out, en_n, en_to, en_tc, en_tf, fn_ctx, builtin_map, lit_sf_names, source, starts, ends, item_tags, item_names, item_d2, item_d3, item_d4, item_field_counts, enum_vnames_flat, enum_max_payloads, item_fn_row, _sv0t31, _sv0t32);
  Value v2;
  v2 = _sv0t33;
  int _sv0t34 = ir_value_tag(v2);
  if ((_sv0t34 != 4)) {
    return 2;
  } else {
  }
  return 0;
}

static int test_lower_assign(void) {
  int _sv0t0 = sv0_vec_new();
  int et = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int ed1 = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int ed2 = _sv0t2;
  int _sv0t3 = sv0_vec_new();
  int ed3 = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int ed4 = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int pp = _sv0t5;
  int _sv0t6 = sv0_vec_new();
  int tok_tags = _sv0t6;
  sv0_vec_push(tok_tags, 91);
  sv0_vec_push(pp, 55);
  int _sv0t7 = sv0_vec_new();
  int ctr = _sv0t7;
  sv0_vec_push(ctr, 0);
  int _sv0t8 = sv0_vec_new();
  int out = _sv0t8;
  sv0_vec_push(et, 1);
  sv0_vec_push(ed1, 0);
  sv0_vec_push(ed2, 1);
  sv0_vec_push(ed3, 0);
  sv0_vec_push(ed4, 0);
  sv0_vec_push(et, 0);
  sv0_vec_push(ed1, 0);
  sv0_vec_push(ed2, 0);
  sv0_vec_push(ed3, 0);
  sv0_vec_push(ed4, 0);
  sv0_vec_push(et, 18);
  sv0_vec_push(ed1, 0);
  sv0_vec_push(ed2, 1);
  sv0_vec_push(ed3, 0);
  sv0_vec_push(ed4, 0);
  int _sv0t9 = sv0_vec_new();
  int en_n = _sv0t9;
  int _sv0t10 = sv0_vec_new();
  int en_to = _sv0t10;
  int _sv0t11 = sv0_vec_new();
  int en_tc = _sv0t11;
  int _sv0t12 = sv0_vec_new();
  int en_tf = _sv0t12;
  int _sv0t13 = sv0_vec_new();
  int fn_ctx = _sv0t13;
  int _sv0t14 = sv0_vec_new();
  int builtin_map = _sv0t14;
  int _sv0t15 = sv0_vec_new();
  int lit_sf_names = _sv0t15;
  const char* source;
  source = "";
  int _sv0t16 = sv0_vec_new();
  int starts = _sv0t16;
  int _sv0t17 = sv0_vec_new();
  int ends = _sv0t17;
  int _sv0t18 = sv0_vec_new();
  int item_tags = _sv0t18;
  int _sv0t19 = sv0_vec_new();
  int item_names = _sv0t19;
  int _sv0t20 = sv0_vec_new();
  int item_d2 = _sv0t20;
  int _sv0t21 = sv0_vec_new();
  int item_d3 = _sv0t21;
  int _sv0t22 = sv0_vec_new();
  int item_d4 = _sv0t22;
  int item_fn_row = (0 - 1);
  int _sv0t23 = sv0_vec_new();
  int item_field_counts = _sv0t23;
  int _sv0t24 = sv0_vec_new();
  int enum_vnames_flat = _sv0t24;
  int _sv0t25 = sv0_vec_new();
  int enum_max_payloads = _sv0t25;
  int _sv0t26 = sv0_vec_new();
  int _sv0t27 = sv0_vec_new();
  Value _sv0t28 = lower_expr_to_value(et, ed1, ed2, ed3, ed4, pp, tok_tags, 2, ctr, out, en_n, en_to, en_tc, en_tf, fn_ctx, builtin_map, lit_sf_names, source, starts, ends, item_tags, item_names, item_d2, item_d3, item_d4, item_field_counts, enum_vnames_flat, enum_max_payloads, item_fn_row, _sv0t26, _sv0t27);
  Value v1;
  v1 = _sv0t28;
  int _sv0t29 = ir_value_tag(v1);
  if ((_sv0t29 != 4)) {
    return 1;
  } else {
  }
  int _sv0t30 = sv0_vec_len(out);
  if ((_sv0t30 != 1)) {
    return 2;
  } else {
  }
  return 0;
}

static int test_lower_while(void) {
  int _sv0t0 = sv0_vec_new();
  int et = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int ed1 = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int ed2 = _sv0t2;
  int _sv0t3 = sv0_vec_new();
  int ed3 = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int ed4 = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int pp = _sv0t5;
  int _sv0t6 = sv0_vec_new();
  int tok_tags = _sv0t6;
  sv0_vec_push(tok_tags, 91);
  sv0_vec_push(pp, 55);
  int _sv0t7 = sv0_vec_new();
  int ctr = _sv0t7;
  sv0_vec_push(ctr, 0);
  int _sv0t8 = sv0_vec_new();
  int out = _sv0t8;
  sv0_vec_push(et, 0);
  sv0_vec_push(ed1, 5);
  sv0_vec_push(ed2, 0);
  sv0_vec_push(ed3, 0);
  sv0_vec_push(ed4, 0);
  sv0_vec_push(et, 4);
  sv0_vec_push(ed1, 0);
  sv0_vec_push(ed2, 0);
  sv0_vec_push(ed3, 0);
  sv0_vec_push(ed4, 0);
  sv0_vec_push(et, 12);
  sv0_vec_push(ed1, 0);
  sv0_vec_push(ed2, 1);
  sv0_vec_push(ed3, 0);
  sv0_vec_push(ed4, 0);
  int _sv0t9 = sv0_vec_new();
  int en_n = _sv0t9;
  int _sv0t10 = sv0_vec_new();
  int en_to = _sv0t10;
  int _sv0t11 = sv0_vec_new();
  int en_tc = _sv0t11;
  int _sv0t12 = sv0_vec_new();
  int en_tf = _sv0t12;
  int _sv0t13 = sv0_vec_new();
  int fn_ctx = _sv0t13;
  int _sv0t14 = sv0_vec_new();
  int builtin_map = _sv0t14;
  int _sv0t15 = sv0_vec_new();
  int lit_sf_names = _sv0t15;
  const char* source;
  source = "";
  int _sv0t16 = sv0_vec_new();
  int starts = _sv0t16;
  int _sv0t17 = sv0_vec_new();
  int ends = _sv0t17;
  int _sv0t18 = sv0_vec_new();
  int item_tags = _sv0t18;
  int _sv0t19 = sv0_vec_new();
  int item_names = _sv0t19;
  int _sv0t20 = sv0_vec_new();
  int item_d2 = _sv0t20;
  int _sv0t21 = sv0_vec_new();
  int item_d3 = _sv0t21;
  int _sv0t22 = sv0_vec_new();
  int item_d4 = _sv0t22;
  int item_fn_row = (0 - 1);
  int _sv0t23 = sv0_vec_new();
  int item_field_counts = _sv0t23;
  int _sv0t24 = sv0_vec_new();
  int enum_vnames_flat = _sv0t24;
  int _sv0t25 = sv0_vec_new();
  int enum_max_payloads = _sv0t25;
  int _sv0t26 = sv0_vec_new();
  int _sv0t27 = sv0_vec_new();
  Value _sv0t28 = lower_expr_to_value(et, ed1, ed2, ed3, ed4, pp, tok_tags, 2, ctr, out, en_n, en_to, en_tc, en_tf, fn_ctx, builtin_map, lit_sf_names, source, starts, ends, item_tags, item_names, item_d2, item_d3, item_d4, item_field_counts, enum_vnames_flat, enum_max_payloads, item_fn_row, _sv0t26, _sv0t27);
  Value v1;
  v1 = _sv0t28;
  int _sv0t29 = ir_value_tag(v1);
  if ((_sv0t29 != 4)) {
    return 1;
  } else {
  }
  int _sv0t30 = sv0_vec_len(out);
  if ((_sv0t30 != 1)) {
    return 2;
  } else {
  }
  return 0;
}

static int test_lower_loop(void) {
  int _sv0t0 = sv0_vec_new();
  int et = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int ed1 = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int ed2 = _sv0t2;
  int _sv0t3 = sv0_vec_new();
  int ed3 = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int ed4 = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int pp = _sv0t5;
  int _sv0t6 = sv0_vec_new();
  int tok_tags = _sv0t6;
  sv0_vec_push(tok_tags, 91);
  sv0_vec_push(pp, 55);
  int _sv0t7 = sv0_vec_new();
  int ctr = _sv0t7;
  sv0_vec_push(ctr, 0);
  int _sv0t8 = sv0_vec_new();
  int out = _sv0t8;
  sv0_vec_push(et, 16);
  sv0_vec_push(ed1, 0);
  sv0_vec_push(ed2, 0);
  sv0_vec_push(ed3, 0);
  sv0_vec_push(ed4, 0);
  sv0_vec_push(et, 14);
  sv0_vec_push(ed1, 0);
  sv0_vec_push(ed2, 0);
  sv0_vec_push(ed3, 0);
  sv0_vec_push(ed4, 0);
  int _sv0t9 = sv0_vec_new();
  int en_n = _sv0t9;
  int _sv0t10 = sv0_vec_new();
  int en_to = _sv0t10;
  int _sv0t11 = sv0_vec_new();
  int en_tc = _sv0t11;
  int _sv0t12 = sv0_vec_new();
  int en_tf = _sv0t12;
  int _sv0t13 = sv0_vec_new();
  int fn_ctx = _sv0t13;
  int _sv0t14 = sv0_vec_new();
  int builtin_map = _sv0t14;
  int _sv0t15 = sv0_vec_new();
  int lit_sf_names = _sv0t15;
  const char* source;
  source = "";
  int _sv0t16 = sv0_vec_new();
  int starts = _sv0t16;
  int _sv0t17 = sv0_vec_new();
  int ends = _sv0t17;
  int _sv0t18 = sv0_vec_new();
  int item_tags = _sv0t18;
  int _sv0t19 = sv0_vec_new();
  int item_names = _sv0t19;
  int _sv0t20 = sv0_vec_new();
  int item_d2 = _sv0t20;
  int _sv0t21 = sv0_vec_new();
  int item_d3 = _sv0t21;
  int _sv0t22 = sv0_vec_new();
  int item_d4 = _sv0t22;
  int item_fn_row = (0 - 1);
  int _sv0t23 = sv0_vec_new();
  int item_field_counts = _sv0t23;
  int _sv0t24 = sv0_vec_new();
  int enum_vnames_flat = _sv0t24;
  int _sv0t25 = sv0_vec_new();
  int enum_max_payloads = _sv0t25;
  int _sv0t26 = sv0_vec_new();
  int _sv0t27 = sv0_vec_new();
  Value _sv0t28 = lower_expr_to_value(et, ed1, ed2, ed3, ed4, pp, tok_tags, 1, ctr, out, en_n, en_to, en_tc, en_tf, fn_ctx, builtin_map, lit_sf_names, source, starts, ends, item_tags, item_names, item_d2, item_d3, item_d4, item_field_counts, enum_vnames_flat, enum_max_payloads, item_fn_row, _sv0t26, _sv0t27);
  Value v1;
  v1 = _sv0t28;
  int _sv0t29 = ir_value_tag(v1);
  if ((_sv0t29 != 4)) {
    return 1;
  } else {
  }
  int _sv0t30 = sv0_vec_len(out);
  if ((_sv0t30 != 1)) {
    return 2;
  } else {
  }
  return 0;
}

static int test_lower_cast(void) {
  int _sv0t0 = sv0_vec_new();
  int et = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int ed1 = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int ed2 = _sv0t2;
  int _sv0t3 = sv0_vec_new();
  int ed3 = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int ed4 = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int pp = _sv0t5;
  int _sv0t6 = sv0_vec_new();
  int tok_tags = _sv0t6;
  sv0_vec_push(tok_tags, 91);
  sv0_vec_push(pp, 55);
  int _sv0t7 = sv0_vec_new();
  int ctr = _sv0t7;
  sv0_vec_push(ctr, 0);
  int _sv0t8 = sv0_vec_new();
  int out = _sv0t8;
  sv0_vec_push(et, 0);
  sv0_vec_push(ed1, 0);
  sv0_vec_push(ed2, 0);
  sv0_vec_push(ed3, 0);
  sv0_vec_push(ed4, 0);
  sv0_vec_push(et, 20);
  sv0_vec_push(ed1, 0);
  sv0_vec_push(ed2, 0);
  sv0_vec_push(ed3, 0);
  sv0_vec_push(ed4, 0);
  int _sv0t9 = sv0_vec_new();
  int en_n = _sv0t9;
  int _sv0t10 = sv0_vec_new();
  int en_to = _sv0t10;
  int _sv0t11 = sv0_vec_new();
  int en_tc = _sv0t11;
  int _sv0t12 = sv0_vec_new();
  int en_tf = _sv0t12;
  int _sv0t13 = sv0_vec_new();
  int fn_ctx = _sv0t13;
  int _sv0t14 = sv0_vec_new();
  int builtin_map = _sv0t14;
  int _sv0t15 = sv0_vec_new();
  int lit_sf_names = _sv0t15;
  const char* source;
  source = "";
  int _sv0t16 = sv0_vec_new();
  int starts = _sv0t16;
  int _sv0t17 = sv0_vec_new();
  int ends = _sv0t17;
  int _sv0t18 = sv0_vec_new();
  int item_tags = _sv0t18;
  int _sv0t19 = sv0_vec_new();
  int item_names = _sv0t19;
  int _sv0t20 = sv0_vec_new();
  int item_d2 = _sv0t20;
  int _sv0t21 = sv0_vec_new();
  int item_d3 = _sv0t21;
  int _sv0t22 = sv0_vec_new();
  int item_d4 = _sv0t22;
  int item_fn_row = (0 - 1);
  int _sv0t23 = sv0_vec_new();
  int item_field_counts = _sv0t23;
  int _sv0t24 = sv0_vec_new();
  int enum_vnames_flat = _sv0t24;
  int _sv0t25 = sv0_vec_new();
  int enum_max_payloads = _sv0t25;
  int _sv0t26 = sv0_vec_new();
  int _sv0t27 = sv0_vec_new();
  Value _sv0t28 = lower_expr_to_value(et, ed1, ed2, ed3, ed4, pp, tok_tags, 1, ctr, out, en_n, en_to, en_tc, en_tf, fn_ctx, builtin_map, lit_sf_names, source, starts, ends, item_tags, item_names, item_d2, item_d3, item_d4, item_field_counts, enum_vnames_flat, enum_max_payloads, item_fn_row, _sv0t26, _sv0t27);
  Value v1;
  v1 = _sv0t28;
  int _sv0t29 = ir_value_tag(v1);
  if ((_sv0t29 != 3)) {
    return 1;
  } else {
  }
  int _sv0t30 = sv0_vec_len(out);
  if ((_sv0t30 != 1)) {
    return 2;
  } else {
  }
  return 0;
}

static int test_lower_body_fn(void) {
  int _sv0t0 = sv0_vec_new();
  int et = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int ed1 = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int ed2 = _sv0t2;
  int _sv0t3 = sv0_vec_new();
  int ed3 = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int ed4 = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int pp = _sv0t5;
  int _sv0t6 = sv0_vec_new();
  int tok_tags = _sv0t6;
  sv0_vec_push(tok_tags, 91);
  sv0_vec_push(pp, 55);
  int _sv0t7 = sv0_vec_new();
  int ctr = _sv0t7;
  sv0_vec_push(ctr, 0);
  int _sv0t8 = sv0_vec_new();
  int out = _sv0t8;
  sv0_vec_push(et, 0);
  sv0_vec_push(ed1, 0);
  sv0_vec_push(ed2, 0);
  sv0_vec_push(ed3, 0);
  sv0_vec_push(ed4, 0);
  int _sv0t9 = sv0_vec_new();
  int en_n = _sv0t9;
  int _sv0t10 = sv0_vec_new();
  int en_to = _sv0t10;
  int _sv0t11 = sv0_vec_new();
  int en_tc = _sv0t11;
  int _sv0t12 = sv0_vec_new();
  int en_tf = _sv0t12;
  int _sv0t13 = sv0_vec_new();
  int fn_ctx = _sv0t13;
  int _sv0t14 = sv0_vec_new();
  int builtin_map = _sv0t14;
  int _sv0t15 = sv0_vec_new();
  int lit_sf_names = _sv0t15;
  const char* source;
  source = "";
  int _sv0t16 = sv0_vec_new();
  int starts = _sv0t16;
  int _sv0t17 = sv0_vec_new();
  int ends = _sv0t17;
  int _sv0t18 = sv0_vec_new();
  int item_tags = _sv0t18;
  int _sv0t19 = sv0_vec_new();
  int item_names = _sv0t19;
  int _sv0t20 = sv0_vec_new();
  int item_d2 = _sv0t20;
  int _sv0t21 = sv0_vec_new();
  int item_d3 = _sv0t21;
  int _sv0t22 = sv0_vec_new();
  int item_d4 = _sv0t22;
  int item_fn_row = (0 - 1);
  int _sv0t23 = sv0_vec_new();
  int item_field_counts = _sv0t23;
  int _sv0t24 = sv0_vec_new();
  int enum_vnames_flat = _sv0t24;
  int _sv0t25 = sv0_vec_new();
  int enum_max_payloads = _sv0t25;
  int _sv0t26 = sv0_vec_new();
  int _sv0t27 = sv0_vec_new();
  int _sv0t28 = lower_body(et, ed1, ed2, ed3, ed4, pp, tok_tags, 0, ctr, out, en_n, en_to, en_tc, en_tf, fn_ctx, builtin_map, lit_sf_names, source, starts, ends, item_tags, item_names, item_d2, item_d3, item_d4, item_field_counts, enum_vnames_flat, enum_max_payloads, item_fn_row, _sv0t26, _sv0t27);
  int rc = _sv0t28;
  if ((rc != 0)) {
    return 1;
  } else {
  }
  int _sv0t29 = sv0_vec_len(out);
  if ((_sv0t29 != 1)) {
    return 2;
  } else {
  }
  return 0;
}

static int test_lower_fn_fn(void) {
  int _sv0t0 = sv0_vec_new();
  int et = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int ed1 = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int ed2 = _sv0t2;
  int _sv0t3 = sv0_vec_new();
  int ed3 = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int ed4 = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int pp = _sv0t5;
  int _sv0t6 = sv0_vec_new();
  int tok_tags = _sv0t6;
  sv0_vec_push(tok_tags, 91);
  sv0_vec_push(pp, 55);
  int _sv0t7 = sv0_vec_new();
  int out = _sv0t7;
  sv0_vec_push(et, 0);
  sv0_vec_push(ed1, 0);
  sv0_vec_push(ed2, 0);
  sv0_vec_push(ed3, 0);
  sv0_vec_push(ed4, 0);
  sv0_vec_push(et, 27);
  sv0_vec_push(ed1, 99);
  sv0_vec_push(ed2, 0);
  sv0_vec_push(ed3, 0);
  sv0_vec_push(ed4, 0);
  sv0_vec_push(et, 9);
  sv0_vec_push(ed1, 1);
  sv0_vec_push(ed2, 1);
  sv0_vec_push(ed3, 0);
  sv0_vec_push(ed4, 0);
  int _sv0t8 = sv0_vec_new();
  int en_n = _sv0t8;
  int _sv0t9 = sv0_vec_new();
  int en_to = _sv0t9;
  int _sv0t10 = sv0_vec_new();
  int en_tc = _sv0t10;
  int _sv0t11 = sv0_vec_new();
  int en_tf = _sv0t11;
  int _sv0t12 = sv0_vec_new();
  int fn_ctx = _sv0t12;
  int _sv0t13 = sv0_vec_new();
  int builtin_map = _sv0t13;
  int _sv0t14 = sv0_vec_new();
  int lit_sf_names = _sv0t14;
  const char* source;
  source = "";
  int _sv0t15 = sv0_vec_new();
  int starts = _sv0t15;
  int _sv0t16 = sv0_vec_new();
  int ends = _sv0t16;
  int _sv0t17 = sv0_vec_new();
  int item_tags = _sv0t17;
  int _sv0t18 = sv0_vec_new();
  int item_names = _sv0t18;
  int _sv0t19 = sv0_vec_new();
  int item_d2 = _sv0t19;
  int _sv0t20 = sv0_vec_new();
  int item_d3 = _sv0t20;
  int _sv0t21 = sv0_vec_new();
  int item_d4 = _sv0t21;
  int item_fn_row = (0 - 1);
  int _sv0t22 = sv0_vec_new();
  int item_field_counts = _sv0t22;
  int _sv0t23 = sv0_vec_new();
  int enum_vnames_flat = _sv0t23;
  int _sv0t24 = sv0_vec_new();
  int enum_max_payloads = _sv0t24;
  int _sv0t25 = sv0_vec_new();
  int _sv0t26 = sv0_vec_new();
  int _sv0t27 = sv0_vec_new();
  int _sv0t28 = sv0_vec_new();
  int _sv0t29 = lower_fn(et, ed1, ed2, ed3, ed4, pp, tok_tags, 2, 99, _sv0t25, _sv0t26, out, en_n, en_to, en_tc, en_tf, fn_ctx, builtin_map, lit_sf_names, source, starts, ends, item_tags, item_names, item_d2, item_d3, item_d4, item_field_counts, enum_vnames_flat, enum_max_payloads, item_fn_row, _sv0t27, _sv0t28);
  int rc = _sv0t29;
  if ((rc != 0)) {
    return 1;
  } else {
  }
  int _sv0t30 = sv0_vec_len(out);
  if ((_sv0t30 < 3)) {
    return 2;
  } else {
  }
  return 0;
}

static int test_lower_fn_contract_glue(void) {
  int _sv0t0 = sv0_vec_new();
  int et = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int ed1 = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int ed2 = _sv0t2;
  int _sv0t3 = sv0_vec_new();
  int ed3 = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int ed4 = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int pp = _sv0t5;
  int _sv0t6 = sv0_vec_new();
  int tok_tags = _sv0t6;
  sv0_vec_push(et, 0);
  sv0_vec_push(ed1, 0);
  sv0_vec_push(ed2, 0);
  sv0_vec_push(ed3, 0);
  sv0_vec_push(ed4, 0);
  sv0_vec_push(et, 0);
  sv0_vec_push(ed1, 0);
  sv0_vec_push(ed2, 0);
  sv0_vec_push(ed3, 0);
  sv0_vec_push(ed4, 0);
  sv0_vec_push(et, 1);
  sv0_vec_push(ed1, 0);
  sv0_vec_push(ed2, 1);
  sv0_vec_push(ed3, 0);
  sv0_vec_push(ed4, 0);
  const char* source;
  source = "result";
  int _sv0t7 = sv0_vec_new();
  int starts = _sv0t7;
  int _sv0t8 = sv0_vec_new();
  int ends = _sv0t8;
  sv0_vec_push(starts, 0);
  sv0_vec_push(ends, 6);
  sv0_vec_push(pp, 0);
  int _sv0t9 = sv0_vec_new();
  int req_roots = _sv0t9;
  sv0_vec_push(req_roots, 1);
  int _sv0t10 = sv0_vec_new();
  int ens_roots = _sv0t10;
  sv0_vec_push(ens_roots, 2);
  int _sv0t11 = sv0_vec_new();
  int out = _sv0t11;
  int _sv0t12 = sv0_vec_new();
  int _sv0t13 = sv0_vec_new();
  int _sv0t14 = sv0_vec_new();
  int _sv0t15 = sv0_vec_new();
  int _sv0t16 = sv0_vec_new();
  int _sv0t17 = sv0_vec_new();
  int _sv0t18 = sv0_vec_new();
  int _sv0t19 = sv0_vec_new();
  int _sv0t20 = sv0_vec_new();
  int _sv0t21 = sv0_vec_new();
  int _sv0t22 = sv0_vec_new();
  int _sv0t23 = sv0_vec_new();
  int _sv0t24 = sv0_vec_new();
  int _sv0t25 = sv0_vec_new();
  int _sv0t26 = sv0_vec_new();
  int _sv0t27 = (0 - 1);
  int _sv0t28 = sv0_vec_new();
  int _sv0t29 = sv0_vec_new();
  int _sv0t30 = lower_fn(et, ed1, ed2, ed3, ed4, pp, tok_tags, 0, 55, req_roots, ens_roots, out, _sv0t12, _sv0t13, _sv0t14, _sv0t15, _sv0t16, _sv0t17, _sv0t18, source, starts, ends, _sv0t19, _sv0t20, _sv0t21, _sv0t22, _sv0t23, _sv0t24, _sv0t25, _sv0t26, _sv0t27, _sv0t28, _sv0t29);
  int rc = _sv0t30;
  if ((rc != 0)) {
    return 1;
  } else {
  }
  int _sv0t31 = sv0_vec_len(out);
  if ((_sv0t31 < 5)) {
    return 2;
  } else {
  }
  int _sv0t32 = sv0_vec_get(out, 0);
  Instr _sv0t33;
  int _sv0t34 = sv0_box_load(_sv0t32, 0);
  _sv0t33.tag = _sv0t34;
  int _sv0t35 = sv0_box_load(_sv0t32, 1);
  _sv0t33.p0 = _sv0t35;
  int _sv0t36 = sv0_box_load(_sv0t32, 2);
  _sv0t33.p1 = _sv0t36;
  int _sv0t37 = sv0_box_load(_sv0t32, 3);
  _sv0t33.p2 = _sv0t37;
  int _sv0t38 = sv0_box_load(_sv0t32, 4);
  _sv0t33.p3 = _sv0t38;
  Instr i0;
  i0.tag = (_sv0t33).tag;
  i0.p0 = (_sv0t33).p0;
  i0.p1 = (_sv0t33).p1;
  i0.p2 = (_sv0t33).p2;
  i0.p3 = (_sv0t33).p3;
  int _sv0t39 = ir_instr_tag(i0);
  if ((_sv0t39 != 1)) {
    return 3;
  } else {
  }
  int _sv0t40 = sv0_vec_get(out, 1);
  Instr _sv0t41;
  int _sv0t42 = sv0_box_load(_sv0t40, 0);
  _sv0t41.tag = _sv0t42;
  int _sv0t43 = sv0_box_load(_sv0t40, 1);
  _sv0t41.p0 = _sv0t43;
  int _sv0t44 = sv0_box_load(_sv0t40, 2);
  _sv0t41.p1 = _sv0t44;
  int _sv0t45 = sv0_box_load(_sv0t40, 3);
  _sv0t41.p2 = _sv0t45;
  int _sv0t46 = sv0_box_load(_sv0t40, 4);
  _sv0t41.p3 = _sv0t46;
  Instr i1;
  i1.tag = (_sv0t41).tag;
  i1.p0 = (_sv0t41).p0;
  i1.p1 = (_sv0t41).p1;
  i1.p2 = (_sv0t41).p2;
  i1.p3 = (_sv0t41).p3;
  int _sv0t47 = ir_instr_tag(i1);
  if ((_sv0t47 != 12)) {
    return 4;
  } else {
  }
  int has_ens = 0;
  int i = 2;
  while (1) {
    int _sv0t48 = sv0_vec_len(out);
    int _sv0t57 = (i < _sv0t48);
    if ((!_sv0t57)) {
      break;
    } else {
    }
    int _sv0t49 = sv0_vec_get(out, i);
    Instr _sv0t50;
    int _sv0t51 = sv0_box_load(_sv0t49, 0);
    _sv0t50.tag = _sv0t51;
    int _sv0t52 = sv0_box_load(_sv0t49, 1);
    _sv0t50.p0 = _sv0t52;
    int _sv0t53 = sv0_box_load(_sv0t49, 2);
    _sv0t50.p1 = _sv0t53;
    int _sv0t54 = sv0_box_load(_sv0t49, 3);
    _sv0t50.p2 = _sv0t54;
    int _sv0t55 = sv0_box_load(_sv0t49, 4);
    _sv0t50.p3 = _sv0t55;
    Instr ii;
    ii.tag = (_sv0t50).tag;
    ii.p0 = (_sv0t50).p0;
    ii.p1 = (_sv0t50).p1;
    ii.p2 = (_sv0t50).p2;
    ii.p3 = (_sv0t50).p3;
    int _sv0t56 = ir_instr_tag(ii);
    if ((_sv0t56 == 13)) {
      has_ens = 1;
    } else {
    }
    i = (i + 1);
  }
  if ((has_ens != 1)) {
    return 5;
  } else {
  }
  int _sv0t58 = sv0_vec_len(out);
  int _sv0t59 = (_sv0t58 - 1);
  int _sv0t60 = sv0_vec_get(out, _sv0t59);
  Instr _sv0t61;
  int _sv0t62 = sv0_box_load(_sv0t60, 0);
  _sv0t61.tag = _sv0t62;
  int _sv0t63 = sv0_box_load(_sv0t60, 1);
  _sv0t61.p0 = _sv0t63;
  int _sv0t64 = sv0_box_load(_sv0t60, 2);
  _sv0t61.p1 = _sv0t64;
  int _sv0t65 = sv0_box_load(_sv0t60, 3);
  _sv0t61.p2 = _sv0t65;
  int _sv0t66 = sv0_box_load(_sv0t60, 4);
  _sv0t61.p3 = _sv0t66;
  Instr ilast;
  ilast.tag = (_sv0t61).tag;
  ilast.p0 = (_sv0t61).p0;
  ilast.p1 = (_sv0t61).p1;
  ilast.p2 = (_sv0t61).p2;
  ilast.p3 = (_sv0t61).p3;
  int _sv0t67 = ir_instr_tag(ilast);
  if ((_sv0t67 != 15)) {
    return 6;
  } else {
  }
  return 0;
}

static int test_lower_for_loop(void) {
  int _sv0t0 = sv0_vec_new();
  int et = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int ed1v = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int ed2v = _sv0t2;
  int _sv0t3 = sv0_vec_new();
  int ed3v = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int ed4v = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int ppv = _sv0t5;
  int _sv0t6 = sv0_vec_new();
  int ttv = _sv0t6;
  sv0_vec_push(ttv, 91);
  sv0_vec_push(ppv, 55);
  int _sv0t7 = sv0_vec_new();
  int out = _sv0t7;
  int _sv0t8 = sv0_vec_new();
  int ctr = _sv0t8;
  sv0_vec_push(ctr, 0);
  sv0_vec_push(et, 0);
  sv0_vec_push(ed1v, 0);
  sv0_vec_push(ed2v, 0);
  sv0_vec_push(ed3v, 0);
  sv0_vec_push(ed4v, 0);
  sv0_vec_push(et, 0);
  sv0_vec_push(ed1v, 0);
  sv0_vec_push(ed2v, 0);
  sv0_vec_push(ed3v, 0);
  sv0_vec_push(ed4v, 0);
  sv0_vec_push(et, 21);
  sv0_vec_push(ed1v, 0);
  sv0_vec_push(ed2v, 1);
  sv0_vec_push(ed3v, 0);
  sv0_vec_push(ed4v, 0);
  sv0_vec_push(et, 0);
  sv0_vec_push(ed1v, 0);
  sv0_vec_push(ed2v, 0);
  sv0_vec_push(ed3v, 0);
  sv0_vec_push(ed4v, 0);
  sv0_vec_push(et, 13);
  sv0_vec_push(ed1v, 99);
  sv0_vec_push(ed2v, 2);
  sv0_vec_push(ed3v, 3);
  sv0_vec_push(ed4v, 0);
  int _sv0t9 = sv0_vec_new();
  int en_n = _sv0t9;
  int _sv0t10 = sv0_vec_new();
  int en_to = _sv0t10;
  int _sv0t11 = sv0_vec_new();
  int en_tc = _sv0t11;
  int _sv0t12 = sv0_vec_new();
  int en_tf = _sv0t12;
  int _sv0t13 = sv0_vec_new();
  int fn_ctx = _sv0t13;
  int _sv0t14 = sv0_vec_new();
  int builtin_map = _sv0t14;
  int _sv0t15 = sv0_vec_new();
  int lit_sf_names = _sv0t15;
  const char* source;
  source = "";
  int _sv0t16 = sv0_vec_new();
  int starts = _sv0t16;
  int _sv0t17 = sv0_vec_new();
  int ends = _sv0t17;
  int _sv0t18 = sv0_vec_new();
  int item_tags = _sv0t18;
  int _sv0t19 = sv0_vec_new();
  int item_names = _sv0t19;
  int _sv0t20 = sv0_vec_new();
  int item_d2 = _sv0t20;
  int _sv0t21 = sv0_vec_new();
  int item_d3 = _sv0t21;
  int _sv0t22 = sv0_vec_new();
  int item_d4 = _sv0t22;
  int item_fn_row = (0 - 1);
  int _sv0t23 = sv0_vec_new();
  int item_field_counts = _sv0t23;
  int _sv0t24 = sv0_vec_new();
  int enum_vnames_flat = _sv0t24;
  int _sv0t25 = sv0_vec_new();
  int enum_max_payloads = _sv0t25;
  int _sv0t26 = sv0_vec_new();
  int _sv0t27 = sv0_vec_new();
  Value _sv0t28 = lower_expr_to_value(et, ed1v, ed2v, ed3v, ed4v, ppv, ttv, 4, ctr, out, en_n, en_to, en_tc, en_tf, fn_ctx, builtin_map, lit_sf_names, source, starts, ends, item_tags, item_names, item_d2, item_d3, item_d4, item_field_counts, enum_vnames_flat, enum_max_payloads, item_fn_row, _sv0t26, _sv0t27);
  Value rv;
  rv = _sv0t28;
  int _sv0t29 = sv0_vec_len(out);
  if ((_sv0t29 < 3)) {
    return 1;
  } else {
  }
  return 0;
}

static int test_lower_expr_assert(void) {
  int _sv0t0 = sv0_vec_new();
  int et = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int ed1 = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int ed2 = _sv0t2;
  int _sv0t3 = sv0_vec_new();
  int ed3 = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int ed4 = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int pp = _sv0t5;
  int _sv0t6 = sv0_vec_new();
  int tok_tags = _sv0t6;
  sv0_vec_push(tok_tags, 91);
  int _sv0t7 = sv0_vec_new();
  int ctr = _sv0t7;
  sv0_vec_push(ctr, 0);
  int _sv0t8 = sv0_vec_new();
  int out = _sv0t8;
  sv0_vec_push(et, 0);
  sv0_vec_push(ed1, 5);
  sv0_vec_push(ed2, 0);
  sv0_vec_push(ed3, 0);
  sv0_vec_push(ed4, 0);
  sv0_vec_push(et, 23);
  sv0_vec_push(ed1, 0);
  sv0_vec_push(ed2, 0);
  sv0_vec_push(ed3, 0);
  sv0_vec_push(ed4, 0);
  int _sv0t9 = sv0_vec_new();
  int en_n = _sv0t9;
  int _sv0t10 = sv0_vec_new();
  int en_to = _sv0t10;
  int _sv0t11 = sv0_vec_new();
  int en_tc = _sv0t11;
  int _sv0t12 = sv0_vec_new();
  int en_tf = _sv0t12;
  int _sv0t13 = sv0_vec_new();
  int fn_ctx = _sv0t13;
  int _sv0t14 = sv0_vec_new();
  int builtin_map = _sv0t14;
  int _sv0t15 = sv0_vec_new();
  int lit_sf_names = _sv0t15;
  const char* source;
  source = "";
  int _sv0t16 = sv0_vec_new();
  int starts = _sv0t16;
  int _sv0t17 = sv0_vec_new();
  int ends = _sv0t17;
  int _sv0t18 = sv0_vec_new();
  int item_tags = _sv0t18;
  int _sv0t19 = sv0_vec_new();
  int item_names = _sv0t19;
  int _sv0t20 = sv0_vec_new();
  int item_d2 = _sv0t20;
  int _sv0t21 = sv0_vec_new();
  int item_d3 = _sv0t21;
  int _sv0t22 = sv0_vec_new();
  int item_d4 = _sv0t22;
  int item_fn_row = (0 - 1);
  int _sv0t23 = sv0_vec_new();
  int item_field_counts = _sv0t23;
  int _sv0t24 = sv0_vec_new();
  int enum_vnames_flat = _sv0t24;
  int _sv0t25 = sv0_vec_new();
  int enum_max_payloads = _sv0t25;
  int _sv0t26 = sv0_vec_new();
  int _sv0t27 = sv0_vec_new();
  Value _sv0t28 = lower_expr_to_value(et, ed1, ed2, ed3, ed4, pp, tok_tags, 1, ctr, out, en_n, en_to, en_tc, en_tf, fn_ctx, builtin_map, lit_sf_names, source, starts, ends, item_tags, item_names, item_d2, item_d3, item_d4, item_field_counts, enum_vnames_flat, enum_max_payloads, item_fn_row, _sv0t26, _sv0t27);
  Value v;
  v = _sv0t28;
  int _sv0t29 = ir_value_tag(v);
  if ((_sv0t29 != 4)) {
    return 1;
  } else {
  }
  return 0;
}

static int test_lower_expr_range(void) {
  int _sv0t0 = sv0_vec_new();
  int et = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int ed1 = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int ed2 = _sv0t2;
  int _sv0t3 = sv0_vec_new();
  int ed3 = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int ed4 = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int pp = _sv0t5;
  int _sv0t6 = sv0_vec_new();
  int tok_tags = _sv0t6;
  sv0_vec_push(tok_tags, 91);
  int _sv0t7 = sv0_vec_new();
  int ctr = _sv0t7;
  sv0_vec_push(ctr, 0);
  int _sv0t8 = sv0_vec_new();
  int out = _sv0t8;
  sv0_vec_push(et, 0);
  sv0_vec_push(ed1, 5);
  sv0_vec_push(ed2, 0);
  sv0_vec_push(ed3, 0);
  sv0_vec_push(ed4, 0);
  sv0_vec_push(et, 0);
  sv0_vec_push(ed1, 0);
  sv0_vec_push(ed2, 0);
  sv0_vec_push(ed3, 0);
  sv0_vec_push(ed4, 0);
  sv0_vec_push(et, 21);
  sv0_vec_push(ed1, 0);
  sv0_vec_push(ed2, 1);
  sv0_vec_push(ed3, 0);
  sv0_vec_push(ed4, 0);
  int _sv0t9 = sv0_vec_new();
  int en_n = _sv0t9;
  int _sv0t10 = sv0_vec_new();
  int en_to = _sv0t10;
  int _sv0t11 = sv0_vec_new();
  int en_tc = _sv0t11;
  int _sv0t12 = sv0_vec_new();
  int en_tf = _sv0t12;
  int _sv0t13 = sv0_vec_new();
  int fn_ctx = _sv0t13;
  int _sv0t14 = sv0_vec_new();
  int builtin_map = _sv0t14;
  int _sv0t15 = sv0_vec_new();
  int lit_sf_names = _sv0t15;
  const char* source;
  source = "";
  int _sv0t16 = sv0_vec_new();
  int starts = _sv0t16;
  int _sv0t17 = sv0_vec_new();
  int ends = _sv0t17;
  int _sv0t18 = sv0_vec_new();
  int item_tags = _sv0t18;
  int _sv0t19 = sv0_vec_new();
  int item_names = _sv0t19;
  int _sv0t20 = sv0_vec_new();
  int item_d2 = _sv0t20;
  int _sv0t21 = sv0_vec_new();
  int item_d3 = _sv0t21;
  int _sv0t22 = sv0_vec_new();
  int item_d4 = _sv0t22;
  int item_fn_row = (0 - 1);
  int _sv0t23 = sv0_vec_new();
  int item_field_counts = _sv0t23;
  int _sv0t24 = sv0_vec_new();
  int enum_vnames_flat = _sv0t24;
  int _sv0t25 = sv0_vec_new();
  int enum_max_payloads = _sv0t25;
  int _sv0t26 = sv0_vec_new();
  int _sv0t27 = sv0_vec_new();
  Value _sv0t28 = lower_expr_to_value(et, ed1, ed2, ed3, ed4, pp, tok_tags, 2, ctr, out, en_n, en_to, en_tc, en_tf, fn_ctx, builtin_map, lit_sf_names, source, starts, ends, item_tags, item_names, item_d2, item_d3, item_d4, item_field_counts, enum_vnames_flat, enum_max_payloads, item_fn_row, _sv0t26, _sv0t27);
  Value v2;
  v2 = _sv0t28;
  int _sv0t29 = ir_value_tag(v2);
  if ((_sv0t29 != 4)) {
    return 1;
  } else {
  }
  int _sv0t30 = sv0_vec_new();
  int et2 = _sv0t30;
  int _sv0t31 = sv0_vec_new();
  int ed1b = _sv0t31;
  int _sv0t32 = sv0_vec_new();
  int ed2b = _sv0t32;
  int _sv0t33 = sv0_vec_new();
  int ed3b = _sv0t33;
  int _sv0t34 = sv0_vec_new();
  int ed4b = _sv0t34;
  int _sv0t35 = sv0_vec_new();
  int pp2 = _sv0t35;
  int _sv0t36 = sv0_vec_new();
  int tok2 = _sv0t36;
  sv0_vec_push(tok2, 91);
  int _sv0t37 = sv0_vec_new();
  int ctr2 = _sv0t37;
  sv0_vec_push(ctr2, 0);
  int _sv0t38 = sv0_vec_new();
  int out2 = _sv0t38;
  sv0_vec_push(et2, 21);
  int _sv0t39 = (0 - 1);
  sv0_vec_push(ed1b, _sv0t39);
  int _sv0t40 = (0 - 1);
  sv0_vec_push(ed2b, _sv0t40);
  sv0_vec_push(ed3b, 0);
  sv0_vec_push(ed4b, 0);
  int _sv0t41 = sv0_vec_new();
  int _sv0t42 = sv0_vec_new();
  Value _sv0t43 = lower_expr_to_value(et2, ed1b, ed2b, ed3b, ed4b, pp2, tok2, 0, ctr2, out2, en_n, en_to, en_tc, en_tf, fn_ctx, builtin_map, lit_sf_names, source, starts, ends, item_tags, item_names, item_d2, item_d3, item_d4, item_field_counts, enum_vnames_flat, enum_max_payloads, item_fn_row, _sv0t41, _sv0t42);
  Value v3;
  v3 = _sv0t43;
  int _sv0t44 = ir_value_tag(v3);
  if ((_sv0t44 != 4)) {
    return 2;
  } else {
  }
  return 0;
}

static int test_lower_expr_tuple_field(void) {
  int _sv0t0 = sv0_vec_new();
  int et = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int ed1 = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int ed2 = _sv0t2;
  int _sv0t3 = sv0_vec_new();
  int ed3 = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int ed4 = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int pp = _sv0t5;
  sv0_vec_push(pp, 200);
  int _sv0t6 = sv0_vec_new();
  int tok_tags = _sv0t6;
  sv0_vec_push(tok_tags, 91);
  int _sv0t7 = sv0_vec_new();
  int ctr = _sv0t7;
  sv0_vec_push(ctr, 0);
  int _sv0t8 = sv0_vec_new();
  int out = _sv0t8;
  sv0_vec_push(et, 1);
  sv0_vec_push(ed1, 0);
  sv0_vec_push(ed2, 1);
  sv0_vec_push(ed3, 0);
  sv0_vec_push(ed4, 0);
  sv0_vec_push(et, 7);
  sv0_vec_push(ed1, 0);
  sv0_vec_push(ed2, 2);
  sv0_vec_push(ed3, 0);
  sv0_vec_push(ed4, 0);
  int _sv0t9 = sv0_vec_new();
  int en_n = _sv0t9;
  int _sv0t10 = sv0_vec_new();
  int en_to = _sv0t10;
  int _sv0t11 = sv0_vec_new();
  int en_tc = _sv0t11;
  int _sv0t12 = sv0_vec_new();
  int en_tf = _sv0t12;
  int _sv0t13 = sv0_vec_new();
  int fn_ctx = _sv0t13;
  int _sv0t14 = sv0_vec_new();
  int builtin_map = _sv0t14;
  int _sv0t15 = sv0_vec_new();
  int lit_sf_names = _sv0t15;
  const char* source;
  source = "";
  int _sv0t16 = sv0_vec_new();
  int starts = _sv0t16;
  int _sv0t17 = sv0_vec_new();
  int ends = _sv0t17;
  int _sv0t18 = sv0_vec_new();
  int item_tags = _sv0t18;
  int _sv0t19 = sv0_vec_new();
  int item_names = _sv0t19;
  int _sv0t20 = sv0_vec_new();
  int item_d2 = _sv0t20;
  int _sv0t21 = sv0_vec_new();
  int item_d3 = _sv0t21;
  int _sv0t22 = sv0_vec_new();
  int item_d4 = _sv0t22;
  int item_fn_row = (0 - 1);
  int _sv0t23 = sv0_vec_new();
  int item_field_counts = _sv0t23;
  int _sv0t24 = sv0_vec_new();
  int enum_vnames_flat = _sv0t24;
  int _sv0t25 = sv0_vec_new();
  int enum_max_payloads = _sv0t25;
  int _sv0t26 = sv0_vec_new();
  int _sv0t27 = sv0_vec_new();
  Value _sv0t28 = lower_expr_to_value(et, ed1, ed2, ed3, ed4, pp, tok_tags, 1, ctr, out, en_n, en_to, en_tc, en_tf, fn_ctx, builtin_map, lit_sf_names, source, starts, ends, item_tags, item_names, item_d2, item_d3, item_d4, item_field_counts, enum_vnames_flat, enum_max_payloads, item_fn_row, _sv0t26, _sv0t27);
  Value v;
  v = _sv0t28;
  int _sv0t29 = ir_value_tag(v);
  if ((_sv0t29 != 6)) {
    return 1;
  } else {
  }
  return 0;
}

static int test_lower_expr_index_effect(void) {
  int _sv0t0 = sv0_vec_new();
  int tok_tags = _sv0t0;
  sv0_vec_push(tok_tags, 91);
  int _sv0t1 = sv0_vec_new();
  int en_n = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int en_to = _sv0t2;
  int _sv0t3 = sv0_vec_new();
  int en_tc = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int en_tf = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int fn_ctx = _sv0t5;
  int _sv0t6 = sv0_vec_new();
  int builtin_map = _sv0t6;
  int _sv0t7 = sv0_vec_new();
  int lit_sf_names = _sv0t7;
  const char* source;
  source = "";
  int _sv0t8 = sv0_vec_new();
  int starts = _sv0t8;
  int _sv0t9 = sv0_vec_new();
  int ends = _sv0t9;
  int _sv0t10 = sv0_vec_new();
  int item_tags = _sv0t10;
  int _sv0t11 = sv0_vec_new();
  int item_names = _sv0t11;
  int _sv0t12 = sv0_vec_new();
  int item_d2 = _sv0t12;
  int _sv0t13 = sv0_vec_new();
  int item_d3 = _sv0t13;
  int _sv0t14 = sv0_vec_new();
  int item_d4 = _sv0t14;
  int item_fn_row = (0 - 1);
  int _sv0t15 = sv0_vec_new();
  int item_field_counts = _sv0t15;
  int _sv0t16 = sv0_vec_new();
  int enum_vnames_flat = _sv0t16;
  int _sv0t17 = sv0_vec_new();
  int enum_max_payloads = _sv0t17;
  int _sv0t18 = sv0_vec_new();
  int et = _sv0t18;
  int _sv0t19 = sv0_vec_new();
  int ed1 = _sv0t19;
  int _sv0t20 = sv0_vec_new();
  int ed2 = _sv0t20;
  int _sv0t21 = sv0_vec_new();
  int ed3 = _sv0t21;
  int _sv0t22 = sv0_vec_new();
  int ed4 = _sv0t22;
  int _sv0t23 = sv0_vec_new();
  int pp = _sv0t23;
  sv0_vec_push(et, 0);
  sv0_vec_push(ed1, 0);
  sv0_vec_push(ed2, 0);
  sv0_vec_push(ed3, 0);
  sv0_vec_push(ed4, 0);
  sv0_vec_push(et, 0);
  sv0_vec_push(ed1, 0);
  sv0_vec_push(ed2, 0);
  sv0_vec_push(ed3, 0);
  sv0_vec_push(ed4, 0);
  sv0_vec_push(et, 8);
  sv0_vec_push(ed1, 0);
  sv0_vec_push(ed2, 1);
  sv0_vec_push(ed3, 0);
  sv0_vec_push(ed4, 0);
  int _sv0t24 = sv0_vec_new();
  int ctr = _sv0t24;
  sv0_vec_push(ctr, 0);
  int _sv0t25 = sv0_vec_new();
  int out = _sv0t25;
  int _sv0t26 = sv0_vec_new();
  int _sv0t27 = sv0_vec_new();
  Value _sv0t28 = lower_expr_to_value(et, ed1, ed2, ed3, ed4, pp, tok_tags, 2, ctr, out, en_n, en_to, en_tc, en_tf, fn_ctx, builtin_map, lit_sf_names, source, starts, ends, item_tags, item_names, item_d2, item_d3, item_d4, item_field_counts, enum_vnames_flat, enum_max_payloads, item_fn_row, _sv0t26, _sv0t27);
  Value v;
  v = _sv0t28;
  int _sv0t29 = ir_value_tag(v);
  if ((_sv0t29 != 3)) {
    return 1;
  } else {
  }
  return 0;
}

static int test_lower_expr_method_call_effect(void) {
  int _sv0t0 = sv0_vec_new();
  int tok_tags = _sv0t0;
  sv0_vec_push(tok_tags, 91);
  int _sv0t1 = sv0_vec_new();
  int en_n = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int en_to = _sv0t2;
  int _sv0t3 = sv0_vec_new();
  int en_tc = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int en_tf = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int fn_ctx = _sv0t5;
  int _sv0t6 = sv0_vec_new();
  int builtin_map = _sv0t6;
  int _sv0t7 = sv0_vec_new();
  int lit_sf_names = _sv0t7;
  const char* source;
  source = "";
  int _sv0t8 = sv0_vec_new();
  int starts = _sv0t8;
  int _sv0t9 = sv0_vec_new();
  int ends = _sv0t9;
  int _sv0t10 = sv0_vec_new();
  int item_tags = _sv0t10;
  int _sv0t11 = sv0_vec_new();
  int item_names = _sv0t11;
  int _sv0t12 = sv0_vec_new();
  int item_d2 = _sv0t12;
  int _sv0t13 = sv0_vec_new();
  int item_d3 = _sv0t13;
  int _sv0t14 = sv0_vec_new();
  int item_d4 = _sv0t14;
  int item_fn_row = (0 - 1);
  int _sv0t15 = sv0_vec_new();
  int item_field_counts = _sv0t15;
  int _sv0t16 = sv0_vec_new();
  int enum_vnames_flat = _sv0t16;
  int _sv0t17 = sv0_vec_new();
  int enum_max_payloads = _sv0t17;
  int _sv0t18 = sv0_vec_new();
  int et = _sv0t18;
  int _sv0t19 = sv0_vec_new();
  int ed1 = _sv0t19;
  int _sv0t20 = sv0_vec_new();
  int ed2 = _sv0t20;
  int _sv0t21 = sv0_vec_new();
  int ed3 = _sv0t21;
  int _sv0t22 = sv0_vec_new();
  int ed4 = _sv0t22;
  int _sv0t23 = sv0_vec_new();
  int pp = _sv0t23;
  sv0_vec_push(et, 0);
  sv0_vec_push(ed1, 0);
  sv0_vec_push(ed2, 0);
  sv0_vec_push(ed3, 0);
  sv0_vec_push(ed4, 0);
  sv0_vec_push(et, 0);
  sv0_vec_push(ed1, 0);
  sv0_vec_push(ed2, 0);
  sv0_vec_push(ed3, 0);
  sv0_vec_push(ed4, 0);
  sv0_vec_push(et, 5);
  sv0_vec_push(ed1, 0);
  sv0_vec_push(ed2, 99);
  sv0_vec_push(ed3, 1);
  sv0_vec_push(ed4, 1);
  int _sv0t24 = sv0_vec_new();
  int ctr = _sv0t24;
  sv0_vec_push(ctr, 0);
  int _sv0t25 = sv0_vec_new();
  int out = _sv0t25;
  int _sv0t26 = sv0_vec_new();
  int _sv0t27 = sv0_vec_new();
  Value _sv0t28 = lower_expr_to_value(et, ed1, ed2, ed3, ed4, pp, tok_tags, 2, ctr, out, en_n, en_to, en_tc, en_tf, fn_ctx, builtin_map, lit_sf_names, source, starts, ends, item_tags, item_names, item_d2, item_d3, item_d4, item_field_counts, enum_vnames_flat, enum_max_payloads, item_fn_row, _sv0t26, _sv0t27);
  Value v;
  v = _sv0t28;
  int _sv0t29 = ir_value_tag(v);
  if ((_sv0t29 != 3)) {
    return 1;
  } else {
  }
  return 0;
}

static int test_lower_expr_array_effect(void) {
  int _sv0t0 = sv0_vec_new();
  int tok_tags = _sv0t0;
  sv0_vec_push(tok_tags, 91);
  int _sv0t1 = sv0_vec_new();
  int en_n = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int en_to = _sv0t2;
  int _sv0t3 = sv0_vec_new();
  int en_tc = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int en_tf = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int fn_ctx = _sv0t5;
  int _sv0t6 = sv0_vec_new();
  int builtin_map = _sv0t6;
  int _sv0t7 = sv0_vec_new();
  int lit_sf_names = _sv0t7;
  const char* source;
  source = "";
  int _sv0t8 = sv0_vec_new();
  int starts = _sv0t8;
  int _sv0t9 = sv0_vec_new();
  int ends = _sv0t9;
  int _sv0t10 = sv0_vec_new();
  int item_tags = _sv0t10;
  int _sv0t11 = sv0_vec_new();
  int item_names = _sv0t11;
  int _sv0t12 = sv0_vec_new();
  int item_d2 = _sv0t12;
  int _sv0t13 = sv0_vec_new();
  int item_d3 = _sv0t13;
  int _sv0t14 = sv0_vec_new();
  int item_d4 = _sv0t14;
  int item_fn_row = (0 - 1);
  int _sv0t15 = sv0_vec_new();
  int item_field_counts = _sv0t15;
  int _sv0t16 = sv0_vec_new();
  int enum_vnames_flat = _sv0t16;
  int _sv0t17 = sv0_vec_new();
  int enum_max_payloads = _sv0t17;
  int _sv0t18 = sv0_vec_new();
  int et = _sv0t18;
  int _sv0t19 = sv0_vec_new();
  int ed1 = _sv0t19;
  int _sv0t20 = sv0_vec_new();
  int ed2 = _sv0t20;
  int _sv0t21 = sv0_vec_new();
  int ed3 = _sv0t21;
  int _sv0t22 = sv0_vec_new();
  int ed4 = _sv0t22;
  int _sv0t23 = sv0_vec_new();
  int pp = _sv0t23;
  sv0_vec_push(et, 0);
  sv0_vec_push(ed1, 0);
  sv0_vec_push(ed2, 0);
  sv0_vec_push(ed3, 0);
  sv0_vec_push(ed4, 0);
  sv0_vec_push(et, 0);
  sv0_vec_push(ed1, 0);
  sv0_vec_push(ed2, 0);
  sv0_vec_push(ed3, 0);
  sv0_vec_push(ed4, 0);
  sv0_vec_push(et, 26);
  sv0_vec_push(ed1, 0);
  sv0_vec_push(ed2, 2);
  sv0_vec_push(ed3, 0);
  sv0_vec_push(ed4, 0);
  int _sv0t24 = sv0_vec_new();
  int ctr = _sv0t24;
  sv0_vec_push(ctr, 0);
  int _sv0t25 = sv0_vec_new();
  int out = _sv0t25;
  int _sv0t26 = sv0_vec_new();
  int _sv0t27 = sv0_vec_new();
  Value _sv0t28 = lower_expr_to_value(et, ed1, ed2, ed3, ed4, pp, tok_tags, 2, ctr, out, en_n, en_to, en_tc, en_tf, fn_ctx, builtin_map, lit_sf_names, source, starts, ends, item_tags, item_names, item_d2, item_d3, item_d4, item_field_counts, enum_vnames_flat, enum_max_payloads, item_fn_row, _sv0t26, _sv0t27);
  Value v;
  v = _sv0t28;
  int _sv0t29 = ir_value_tag(v);
  if ((_sv0t29 != 3)) {
    return 1;
  } else {
  }
  return 0;
}

static int test_lower_expr_struct(void) {
  int _sv0t0 = sv0_vec_new();
  int et = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int ed1 = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int ed2 = _sv0t2;
  int _sv0t3 = sv0_vec_new();
  int ed3 = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int ed4 = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int pp = _sv0t5;
  int _sv0t6 = sv0_vec_new();
  int tok_tags = _sv0t6;
  sv0_vec_push(tok_tags, 91);
  sv0_vec_push(pp, 100);
  int _sv0t7 = sv0_vec_new();
  int ctr = _sv0t7;
  sv0_vec_push(ctr, 0);
  int _sv0t8 = sv0_vec_new();
  int out = _sv0t8;
  sv0_vec_push(et, 0);
  sv0_vec_push(ed1, 0);
  sv0_vec_push(ed2, 0);
  sv0_vec_push(ed3, 0);
  sv0_vec_push(ed4, 0);
  sv0_vec_push(et, 0);
  sv0_vec_push(ed1, 0);
  sv0_vec_push(ed2, 0);
  sv0_vec_push(ed3, 0);
  sv0_vec_push(ed4, 0);
  sv0_vec_push(et, 24);
  sv0_vec_push(ed1, 0);
  sv0_vec_push(ed2, 1);
  sv0_vec_push(ed3, 2);
  sv0_vec_push(ed4, 0);
  int _sv0t9 = sv0_vec_new();
  int en_n = _sv0t9;
  int _sv0t10 = sv0_vec_new();
  int en_to = _sv0t10;
  int _sv0t11 = sv0_vec_new();
  int en_tc = _sv0t11;
  int _sv0t12 = sv0_vec_new();
  int en_tf = _sv0t12;
  int _sv0t13 = sv0_vec_new();
  int fn_ctx = _sv0t13;
  int _sv0t14 = sv0_vec_new();
  int builtin_map = _sv0t14;
  int _sv0t15 = sv0_vec_new();
  int lit_sf_names = _sv0t15;
  const char* source;
  source = "";
  int _sv0t16 = sv0_vec_new();
  int starts = _sv0t16;
  int _sv0t17 = sv0_vec_new();
  int ends = _sv0t17;
  int _sv0t18 = sv0_vec_new();
  int item_tags = _sv0t18;
  int _sv0t19 = sv0_vec_new();
  int item_names = _sv0t19;
  int _sv0t20 = sv0_vec_new();
  int item_d2 = _sv0t20;
  int _sv0t21 = sv0_vec_new();
  int item_d3 = _sv0t21;
  int _sv0t22 = sv0_vec_new();
  int item_d4 = _sv0t22;
  int item_fn_row = (0 - 1);
  int _sv0t23 = sv0_vec_new();
  int item_field_counts = _sv0t23;
  int _sv0t24 = sv0_vec_new();
  int enum_vnames_flat = _sv0t24;
  int _sv0t25 = sv0_vec_new();
  int enum_max_payloads = _sv0t25;
  sv0_vec_push(lit_sf_names, 201);
  sv0_vec_push(lit_sf_names, 0);
  sv0_vec_push(lit_sf_names, 202);
  sv0_vec_push(lit_sf_names, 1);
  int _sv0t26 = sv0_vec_new();
  int _sv0t27 = sv0_vec_new();
  Value _sv0t28 = lower_expr_to_value(et, ed1, ed2, ed3, ed4, pp, tok_tags, 2, ctr, out, en_n, en_to, en_tc, en_tf, fn_ctx, builtin_map, lit_sf_names, source, starts, ends, item_tags, item_names, item_d2, item_d3, item_d4, item_field_counts, enum_vnames_flat, enum_max_payloads, item_fn_row, _sv0t26, _sv0t27);
  Value v;
  v = _sv0t28;
  int _sv0t29 = ir_value_tag(v);
  if ((_sv0t29 != 3)) {
    return 1;
  } else {
  }
  int _sv0t30 = sv0_vec_len(out);
  if ((_sv0t30 < 3)) {
    return 2;
  } else {
  }
  return 0;
}

static int test_lower_expr_struct_qualified_path(void) {
  int _sv0t0 = sv0_vec_new();
  int et = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int ed1 = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int ed2 = _sv0t2;
  int _sv0t3 = sv0_vec_new();
  int ed3 = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int ed4 = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int pp = _sv0t5;
  int _sv0t6 = sv0_vec_new();
  int tok_tags = _sv0t6;
  sv0_vec_push(tok_tags, 91);
  sv0_vec_push(pp, 50);
  sv0_vec_push(pp, 100);
  int _sv0t7 = sv0_vec_new();
  int ctr = _sv0t7;
  sv0_vec_push(ctr, 0);
  int _sv0t8 = sv0_vec_new();
  int out = _sv0t8;
  sv0_vec_push(et, 0);
  sv0_vec_push(ed1, 0);
  sv0_vec_push(ed2, 0);
  sv0_vec_push(ed3, 0);
  sv0_vec_push(ed4, 0);
  sv0_vec_push(et, 0);
  sv0_vec_push(ed1, 0);
  sv0_vec_push(ed2, 0);
  sv0_vec_push(ed3, 0);
  sv0_vec_push(ed4, 0);
  sv0_vec_push(et, 24);
  sv0_vec_push(ed1, 0);
  sv0_vec_push(ed2, 2);
  sv0_vec_push(ed3, 2);
  sv0_vec_push(ed4, 0);
  int _sv0t9 = sv0_vec_new();
  int en_n = _sv0t9;
  int _sv0t10 = sv0_vec_new();
  int en_to = _sv0t10;
  int _sv0t11 = sv0_vec_new();
  int en_tc = _sv0t11;
  int _sv0t12 = sv0_vec_new();
  int en_tf = _sv0t12;
  int _sv0t13 = sv0_vec_new();
  int fn_ctx = _sv0t13;
  int _sv0t14 = sv0_vec_new();
  int builtin_map = _sv0t14;
  int _sv0t15 = sv0_vec_new();
  int lit_sf_names = _sv0t15;
  const char* source;
  source = "";
  int _sv0t16 = sv0_vec_new();
  int starts = _sv0t16;
  int _sv0t17 = sv0_vec_new();
  int ends = _sv0t17;
  int _sv0t18 = sv0_vec_new();
  int item_tags = _sv0t18;
  int _sv0t19 = sv0_vec_new();
  int item_names = _sv0t19;
  int _sv0t20 = sv0_vec_new();
  int item_d2 = _sv0t20;
  int _sv0t21 = sv0_vec_new();
  int item_d3 = _sv0t21;
  int _sv0t22 = sv0_vec_new();
  int item_d4 = _sv0t22;
  int item_fn_row = (0 - 1);
  int _sv0t23 = sv0_vec_new();
  int item_field_counts = _sv0t23;
  int _sv0t24 = sv0_vec_new();
  int enum_vnames_flat = _sv0t24;
  int _sv0t25 = sv0_vec_new();
  int enum_max_payloads = _sv0t25;
  sv0_vec_push(lit_sf_names, 201);
  sv0_vec_push(lit_sf_names, 0);
  sv0_vec_push(lit_sf_names, 202);
  sv0_vec_push(lit_sf_names, 1);
  int _sv0t26 = sv0_vec_new();
  int _sv0t27 = sv0_vec_new();
  Value _sv0t28 = lower_expr_to_value(et, ed1, ed2, ed3, ed4, pp, tok_tags, 2, ctr, out, en_n, en_to, en_tc, en_tf, fn_ctx, builtin_map, lit_sf_names, source, starts, ends, item_tags, item_names, item_d2, item_d3, item_d4, item_field_counts, enum_vnames_flat, enum_max_payloads, item_fn_row, _sv0t26, _sv0t27);
  Value v;
  v = _sv0t28;
  int _sv0t29 = ir_value_tag(v);
  if ((_sv0t29 != 3)) {
    return 1;
  } else {
  }
  int _sv0t30 = sv0_vec_len(out);
  if ((_sv0t30 < 3)) {
    return 2;
  } else {
  }
  return 0;
}

static int test_lower_program_fn(void) {
  const char* src;
  src = "fn foo() { } struct Bar { x: i32, } enum Qux { A, B, }";
  int _sv0t0 = sv0_vec_new();
  int strt = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int endi = _sv0t1;
  int p = 0;
  while ((p < 54)) {
    sv0_vec_push(strt, p);
    int _sv0t2 = (p + 1);
    sv0_vec_push(endi, _sv0t2);
    p = (p + 1);
  }
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
  int ifc = _sv0t8;
  int _sv0t9 = sv0_vec_new();
  int ivm = _sv0t9;
  int _sv0t10 = sv0_vec_new();
  int sfn = _sv0t10;
  int _sv0t11 = sv0_vec_new();
  int evn = _sv0t11;
  int _sv0t12 = sv0_vec_new();
  int sfnames = _sv0t12;
  int _sv0t13 = sv0_vec_new();
  int sftypes = _sv0t13;
  sv0_vec_push(it, 0);
  sv0_vec_push(id1, 3);
  sv0_vec_push(id2, 0);
  sv0_vec_push(id3, 0);
  sv0_vec_push(id4, 0);
  sv0_vec_push(ifc, 0);
  sv0_vec_push(ivm, 0);
  sv0_vec_push(it, 1);
  sv0_vec_push(id1, 14);
  sv0_vec_push(id2, 1);
  sv0_vec_push(id3, 0);
  sv0_vec_push(id4, 0);
  sv0_vec_push(ifc, 1);
  sv0_vec_push(ivm, 0);
  sv0_vec_push(sfn, 20);
  sv0_vec_push(sfnames, 20);
  sv0_vec_push(sftypes, 22);
  sv0_vec_push(it, 2);
  sv0_vec_push(id1, 30);
  sv0_vec_push(id2, 2);
  sv0_vec_push(id3, 0);
  sv0_vec_push(id4, 0);
  sv0_vec_push(ifc, 0);
  sv0_vec_push(ivm, 0);
  sv0_vec_push(evn, 40);
  sv0_vec_push(evn, 45);
  int _sv0t14 = sv0_vec_new();
  int ob = _sv0t14;
  int _sv0t15 = sv0_vec_new();
  int osn = _sv0t15;
  int _sv0t16 = sv0_vec_new();
  int oso = _sv0t16;
  int _sv0t17 = sv0_vec_new();
  int osc = _sv0t17;
  int _sv0t18 = sv0_vec_new();
  int osf = _sv0t18;
  int _sv0t19 = sv0_vec_new();
  int oen = _sv0t19;
  int _sv0t20 = sv0_vec_new();
  int oeto = _sv0t20;
  int _sv0t21 = sv0_vec_new();
  int oetc = _sv0t21;
  int _sv0t22 = sv0_vec_new();
  int oetf = _sv0t22;
  int _sv0t23 = sv0_vec_new();
  int oem = _sv0t23;
  int _sv0t24 = sv0_vec_new();
  int bet = _sv0t24;
  int _sv0t25 = sv0_vec_new();
  int be1 = _sv0t25;
  int _sv0t26 = sv0_vec_new();
  int be2 = _sv0t26;
  int _sv0t27 = sv0_vec_new();
  int be3 = _sv0t27;
  int _sv0t28 = sv0_vec_new();
  int be4 = _sv0t28;
  int _sv0t29 = sv0_vec_new();
  int bsf = _sv0t29;
  int _sv0t30 = sv0_vec_new();
  int bpp = _sv0t30;
  int _sv0t31 = sv0_vec_new();
  int ttags = _sv0t31;
  int _sv0t32 = sv0_vec_new();
  int oblks = _sv0t32;
  int _sv0t33 = sv0_vec_new();
  const char* _sv0t34 = lower(it, id1, id2, id3, id4, ifc, ivm, sfn, evn, sfnames, sftypes, src, strt, endi, ob, osn, oso, osc, osf, oen, oeto, oetc, oetf, oem, bet, be1, be2, be3, be4, bsf, bpp, ttags, oblks, _sv0t33);
  const char* td;
  td = _sv0t34;
  int _sv0t35 = sv0_vec_len(ob);
  if ((_sv0t35 != 1)) {
    return 1;
  } else {
  }
  int _sv0t36 = sv0_vec_get(ob, 0);
  if ((_sv0t36 != 3)) {
    return 2;
  } else {
  }
  int _sv0t37 = sv0_vec_len(osn);
  if ((_sv0t37 != 1)) {
    return 3;
  } else {
  }
  int _sv0t38 = sv0_vec_len(oen);
  if ((_sv0t38 != 1)) {
    return 4;
  } else {
  }
  int _sv0t39 = sv0_vec_len(oblks);
  if ((_sv0t39 != 4)) {
    return 5;
  } else {
  }
  int _sv0t40 = sv0_vec_get(oblks, 0);
  if ((_sv0t40 != 3)) {
    return 6;
  } else {
  }
  return 0;
}

static int test_lower_return_fn(void) {
  int _sv0t0 = sv0_vec_new();
  int et = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int ed1 = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int ed2 = _sv0t2;
  int _sv0t3 = sv0_vec_new();
  int ed3 = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int ed4 = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int pp = _sv0t5;
  int _sv0t6 = sv0_vec_new();
  int tok_tags = _sv0t6;
  sv0_vec_push(tok_tags, 91);
  sv0_vec_push(pp, 55);
  int _sv0t7 = sv0_vec_new();
  int ctr = _sv0t7;
  sv0_vec_push(ctr, 0);
  int _sv0t8 = sv0_vec_new();
  int out = _sv0t8;
  sv0_vec_push(et, 0);
  sv0_vec_push(ed1, 0);
  sv0_vec_push(ed2, 0);
  sv0_vec_push(ed3, 0);
  sv0_vec_push(ed4, 0);
  int _sv0t9 = sv0_vec_new();
  int en_n = _sv0t9;
  int _sv0t10 = sv0_vec_new();
  int en_to = _sv0t10;
  int _sv0t11 = sv0_vec_new();
  int en_tc = _sv0t11;
  int _sv0t12 = sv0_vec_new();
  int en_tf = _sv0t12;
  int _sv0t13 = sv0_vec_new();
  int fn_ctx = _sv0t13;
  int _sv0t14 = sv0_vec_new();
  int builtin_map = _sv0t14;
  int _sv0t15 = sv0_vec_new();
  int lit_sf_names = _sv0t15;
  const char* source;
  source = "";
  int _sv0t16 = sv0_vec_new();
  int starts = _sv0t16;
  int _sv0t17 = sv0_vec_new();
  int ends = _sv0t17;
  int _sv0t18 = sv0_vec_new();
  int item_tags = _sv0t18;
  int _sv0t19 = sv0_vec_new();
  int item_names = _sv0t19;
  int _sv0t20 = sv0_vec_new();
  int item_d2 = _sv0t20;
  int _sv0t21 = sv0_vec_new();
  int item_d3 = _sv0t21;
  int _sv0t22 = sv0_vec_new();
  int item_d4 = _sv0t22;
  int item_fn_row = (0 - 1);
  int _sv0t23 = sv0_vec_new();
  int item_field_counts = _sv0t23;
  int _sv0t24 = sv0_vec_new();
  int enum_vnames_flat = _sv0t24;
  int _sv0t25 = sv0_vec_new();
  int enum_max_payloads = _sv0t25;
  int _sv0t26 = sv0_vec_new();
  int _sv0t27 = sv0_vec_new();
  int _sv0t28 = lower_return(et, ed1, ed2, ed3, ed4, pp, tok_tags, 0, ctr, out, en_n, en_to, en_tc, en_tf, fn_ctx, builtin_map, lit_sf_names, source, starts, ends, item_tags, item_names, item_d2, item_d3, item_d4, item_field_counts, enum_vnames_flat, enum_max_payloads, item_fn_row, _sv0t26, _sv0t27);
  int rc = _sv0t28;
  if ((rc != 0)) {
    return 1;
  } else {
  }
  int _sv0t29 = sv0_vec_len(out);
  if ((_sv0t29 != 1)) {
    return 2;
  } else {
  }
  return 0;
}

static int test_lower_stmt_fn(void) {
  int _sv0t0 = sv0_vec_new();
  int et = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int ed1 = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int ed2 = _sv0t2;
  int _sv0t3 = sv0_vec_new();
  int ed3 = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int ed4 = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int pp = _sv0t5;
  int _sv0t6 = sv0_vec_new();
  int tok_tags = _sv0t6;
  sv0_vec_push(tok_tags, 91);
  sv0_vec_push(pp, 55);
  int _sv0t7 = sv0_vec_new();
  int ctr = _sv0t7;
  sv0_vec_push(ctr, 0);
  int _sv0t8 = sv0_vec_new();
  int out = _sv0t8;
  sv0_vec_push(et, 0);
  sv0_vec_push(ed1, 0);
  sv0_vec_push(ed2, 0);
  sv0_vec_push(ed3, 0);
  sv0_vec_push(ed4, 0);
  sv0_vec_push(et, 27);
  sv0_vec_push(ed1, 99);
  sv0_vec_push(ed2, 0);
  sv0_vec_push(ed3, 0);
  sv0_vec_push(ed4, 0);
  int _sv0t9 = sv0_vec_new();
  int en_n = _sv0t9;
  int _sv0t10 = sv0_vec_new();
  int en_to = _sv0t10;
  int _sv0t11 = sv0_vec_new();
  int en_tc = _sv0t11;
  int _sv0t12 = sv0_vec_new();
  int en_tf = _sv0t12;
  int _sv0t13 = sv0_vec_new();
  int fn_ctx = _sv0t13;
  int _sv0t14 = sv0_vec_new();
  int builtin_map = _sv0t14;
  int _sv0t15 = sv0_vec_new();
  int lit_sf_names = _sv0t15;
  const char* source;
  source = "";
  int _sv0t16 = sv0_vec_new();
  int starts = _sv0t16;
  int _sv0t17 = sv0_vec_new();
  int ends = _sv0t17;
  int _sv0t18 = sv0_vec_new();
  int item_tags = _sv0t18;
  int _sv0t19 = sv0_vec_new();
  int item_names = _sv0t19;
  int _sv0t20 = sv0_vec_new();
  int item_d2 = _sv0t20;
  int _sv0t21 = sv0_vec_new();
  int item_d3 = _sv0t21;
  int _sv0t22 = sv0_vec_new();
  int item_d4 = _sv0t22;
  int item_fn_row = (0 - 1);
  int _sv0t23 = sv0_vec_new();
  int item_field_counts = _sv0t23;
  int _sv0t24 = sv0_vec_new();
  int enum_vnames_flat = _sv0t24;
  int _sv0t25 = sv0_vec_new();
  int enum_max_payloads = _sv0t25;
  int _sv0t26 = sv0_vec_new();
  int _sv0t27 = sv0_vec_new();
  int _sv0t28 = sv0_vec_new();
  int _sv0t29 = lower_stmt(et, ed1, ed2, ed3, ed4, pp, tok_tags, 1, ctr, out, en_n, en_to, en_tc, en_tf, fn_ctx, builtin_map, lit_sf_names, source, starts, ends, item_tags, item_names, item_d2, item_d3, item_d4, item_field_counts, enum_vnames_flat, enum_max_payloads, item_fn_row, _sv0t26, _sv0t27, _sv0t28);
  int rc = _sv0t29;
  if ((rc != 0)) {
    return 1;
  } else {
  }
  int _sv0t30 = sv0_vec_len(out);
  if ((_sv0t30 != 2)) {
    return 2;
  } else {
  }
  int _sv0t31 = sv0_vec_new();
  int out2 = _sv0t31;
  sv0_vec_push(et, 28);
  sv0_vec_push(ed1, 0);
  sv0_vec_push(ed2, 0);
  sv0_vec_push(ed3, 0);
  sv0_vec_push(ed4, 0);
  int _sv0t32 = sv0_vec_new();
  int _sv0t33 = sv0_vec_new();
  int _sv0t34 = sv0_vec_new();
  int _sv0t35 = lower_stmt(et, ed1, ed2, ed3, ed4, pp, tok_tags, 2, ctr, out2, en_n, en_to, en_tc, en_tf, fn_ctx, builtin_map, lit_sf_names, source, starts, ends, item_tags, item_names, item_d2, item_d3, item_d4, item_field_counts, enum_vnames_flat, enum_max_payloads, item_fn_row, _sv0t32, _sv0t33, _sv0t34);
  int rc2 = _sv0t35;
  if ((rc2 != 0)) {
    return 3;
  } else {
  }
  return 0;
}

static int test_lower_block_fn(void) {
  int _sv0t0 = sv0_vec_new();
  int et = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int ed1 = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int ed2 = _sv0t2;
  int _sv0t3 = sv0_vec_new();
  int ed3 = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int ed4 = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int pp = _sv0t5;
  int _sv0t6 = sv0_vec_new();
  int tok_tags = _sv0t6;
  sv0_vec_push(tok_tags, 91);
  sv0_vec_push(pp, 55);
  int _sv0t7 = sv0_vec_new();
  int ctr = _sv0t7;
  sv0_vec_push(ctr, 0);
  int _sv0t8 = sv0_vec_new();
  int out = _sv0t8;
  sv0_vec_push(et, 0);
  sv0_vec_push(ed1, 0);
  sv0_vec_push(ed2, 0);
  sv0_vec_push(ed3, 0);
  sv0_vec_push(ed4, 0);
  sv0_vec_push(et, 27);
  sv0_vec_push(ed1, 99);
  sv0_vec_push(ed2, 0);
  sv0_vec_push(ed3, 0);
  sv0_vec_push(ed4, 0);
  int _sv0t9 = sv0_vec_new();
  int en_n = _sv0t9;
  int _sv0t10 = sv0_vec_new();
  int en_to = _sv0t10;
  int _sv0t11 = sv0_vec_new();
  int en_tc = _sv0t11;
  int _sv0t12 = sv0_vec_new();
  int en_tf = _sv0t12;
  int _sv0t13 = sv0_vec_new();
  int fn_ctx = _sv0t13;
  int _sv0t14 = sv0_vec_new();
  int builtin_map = _sv0t14;
  int _sv0t15 = sv0_vec_new();
  int lit_sf_names = _sv0t15;
  const char* source;
  source = "";
  int _sv0t16 = sv0_vec_new();
  int starts = _sv0t16;
  int _sv0t17 = sv0_vec_new();
  int ends = _sv0t17;
  int _sv0t18 = sv0_vec_new();
  int item_tags = _sv0t18;
  int _sv0t19 = sv0_vec_new();
  int item_names = _sv0t19;
  int _sv0t20 = sv0_vec_new();
  int item_d2 = _sv0t20;
  int _sv0t21 = sv0_vec_new();
  int item_d3 = _sv0t21;
  int _sv0t22 = sv0_vec_new();
  int item_d4 = _sv0t22;
  int item_fn_row = (0 - 1);
  int _sv0t23 = sv0_vec_new();
  int item_field_counts = _sv0t23;
  int _sv0t24 = sv0_vec_new();
  int enum_vnames_flat = _sv0t24;
  int _sv0t25 = sv0_vec_new();
  int enum_max_payloads = _sv0t25;
  int _sv0t26 = sv0_vec_new();
  int _sv0t27 = sv0_vec_new();
  int _sv0t28 = lower_block(et, ed1, ed2, ed3, ed4, pp, tok_tags, 1, 1, 0, 0, ctr, out, en_n, en_to, en_tc, en_tf, fn_ctx, builtin_map, lit_sf_names, source, starts, ends, item_tags, item_names, item_d2, item_d3, item_d4, item_field_counts, enum_vnames_flat, enum_max_payloads, item_fn_row, _sv0t26, _sv0t27);
  int rc = _sv0t28;
  if ((rc != 0)) {
    return 1;
  } else {
  }
  int _sv0t29 = sv0_vec_len(out);
  if ((_sv0t29 < 3)) {
    return 2;
  } else {
  }
  int _sv0t30 = sv0_vec_new();
  int out2 = _sv0t30;
  int _sv0t31 = (0 - 1);
  int _sv0t32 = sv0_vec_new();
  int _sv0t33 = sv0_vec_new();
  int _sv0t34 = lower_block(et, ed1, ed2, ed3, ed4, pp, tok_tags, 1, 1, _sv0t31, 0, ctr, out2, en_n, en_to, en_tc, en_tf, fn_ctx, builtin_map, lit_sf_names, source, starts, ends, item_tags, item_names, item_d2, item_d3, item_d4, item_field_counts, enum_vnames_flat, enum_max_payloads, item_fn_row, _sv0t32, _sv0t33);
  int rc2 = _sv0t34;
  if ((rc2 != 0)) {
    return 3;
  } else {
  }
  int _sv0t35 = sv0_vec_len(out2);
  if ((_sv0t35 != 2)) {
    return 4;
  } else {
  }
  return 0;
}

static int test_value_to_expr_fn(void) {
  Value v1;
  v1.tag = 0;
  v1.p0 = 42;
  Expr _sv0t0 = value_to_expr(v1);
  Expr e1;
  e1 = _sv0t0;
  int _sv0t1 = ir_expr_tag(e1);
  if ((_sv0t1 != 0)) {
    return 1;
  } else {
  }
  Value v2;
  v2.tag = 3;
  v2.p0 = 7;
  Expr _sv0t2 = value_to_expr(v2);
  Expr e2;
  e2 = _sv0t2;
  int _sv0t3 = ir_expr_tag(e2);
  if ((_sv0t3 != 1)) {
    return 2;
  } else {
  }
  Value v3;
  v3.tag = 4;
  Expr _sv0t4 = value_to_expr(v3);
  Expr e3;
  e3 = _sv0t4;
  int _sv0t5 = ir_expr_tag(e3);
  if ((_sv0t5 != 0)) {
    return 3;
  } else {
  }
  Value v4;
  v4.tag = 1;
  Expr _sv0t6 = value_to_expr(v4);
  Expr e4;
  e4 = _sv0t6;
  int _sv0t7 = ir_expr_tag(e4);
  if ((_sv0t7 != 0)) {
    return 4;
  } else {
  }
  return 0;
}

static int test_lower_with_instrs(void) {
  int _sv0t0 = sv0_vec_new();
  int et = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int ed1 = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int ed2 = _sv0t2;
  int _sv0t3 = sv0_vec_new();
  int ed3 = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int ed4 = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int pp = _sv0t5;
  int _sv0t6 = sv0_vec_new();
  int tok_tags = _sv0t6;
  sv0_vec_push(tok_tags, 91);
  sv0_vec_push(pp, 55);
  int _sv0t7 = sv0_vec_new();
  int ctr = _sv0t7;
  sv0_vec_push(ctr, 0);
  int _sv0t8 = sv0_vec_new();
  int out = _sv0t8;
  sv0_vec_push(et, 0);
  sv0_vec_push(ed1, 0);
  sv0_vec_push(ed2, 0);
  sv0_vec_push(ed3, 0);
  sv0_vec_push(ed4, 0);
  int _sv0t9 = sv0_vec_new();
  int en_n = _sv0t9;
  int _sv0t10 = sv0_vec_new();
  int en_to = _sv0t10;
  int _sv0t11 = sv0_vec_new();
  int en_tc = _sv0t11;
  int _sv0t12 = sv0_vec_new();
  int en_tf = _sv0t12;
  int _sv0t13 = sv0_vec_new();
  int fn_ctx = _sv0t13;
  int _sv0t14 = sv0_vec_new();
  int builtin_map = _sv0t14;
  int _sv0t15 = sv0_vec_new();
  int lit_sf_names = _sv0t15;
  const char* source;
  source = "";
  int _sv0t16 = sv0_vec_new();
  int starts = _sv0t16;
  int _sv0t17 = sv0_vec_new();
  int ends = _sv0t17;
  int _sv0t18 = sv0_vec_new();
  int item_tags = _sv0t18;
  int _sv0t19 = sv0_vec_new();
  int item_names = _sv0t19;
  int _sv0t20 = sv0_vec_new();
  int item_d2 = _sv0t20;
  int _sv0t21 = sv0_vec_new();
  int item_d3 = _sv0t21;
  int _sv0t22 = sv0_vec_new();
  int item_d4 = _sv0t22;
  int item_fn_row = (0 - 1);
  int _sv0t23 = sv0_vec_new();
  int item_field_counts = _sv0t23;
  int _sv0t24 = sv0_vec_new();
  int enum_vnames_flat = _sv0t24;
  int _sv0t25 = sv0_vec_new();
  int enum_max_payloads = _sv0t25;
  int _sv0t26 = sv0_vec_new();
  int _sv0t27 = sv0_vec_new();
  Expr _sv0t28 = lower_expr_with_instrs(et, ed1, ed2, ed3, ed4, pp, tok_tags, 0, ctr, out, en_n, en_to, en_tc, en_tf, fn_ctx, builtin_map, lit_sf_names, source, starts, ends, item_tags, item_names, item_d2, item_d3, item_d4, item_field_counts, enum_vnames_flat, enum_max_payloads, item_fn_row, _sv0t26, _sv0t27);
  Expr e1;
  e1 = _sv0t28;
  int _sv0t29 = ir_expr_tag(e1);
  if ((_sv0t29 != 0)) {
    return 1;
  } else {
  }
  sv0_vec_push(et, 1);
  sv0_vec_push(ed1, 0);
  sv0_vec_push(ed2, 1);
  sv0_vec_push(ed3, 0);
  sv0_vec_push(ed4, 0);
  int _sv0t30 = sv0_vec_new();
  int _sv0t31 = sv0_vec_new();
  Expr _sv0t32 = lower_expr_with_instrs(et, ed1, ed2, ed3, ed4, pp, tok_tags, 1, ctr, out, en_n, en_to, en_tc, en_tf, fn_ctx, builtin_map, lit_sf_names, source, starts, ends, item_tags, item_names, item_d2, item_d3, item_d4, item_field_counts, enum_vnames_flat, enum_max_payloads, item_fn_row, _sv0t30, _sv0t31);
  Expr e2;
  e2 = _sv0t32;
  int _sv0t33 = ir_expr_tag(e2);
  if ((_sv0t33 != 1)) {
    return 2;
  } else {
  }
  return 0;
}

static int test_lower_for_effect_fn(void) {
  int _sv0t0 = sv0_vec_new();
  int et = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int ed1 = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int ed2 = _sv0t2;
  int _sv0t3 = sv0_vec_new();
  int ed3 = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int ed4 = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int pp = _sv0t5;
  int _sv0t6 = sv0_vec_new();
  int tok_tags = _sv0t6;
  sv0_vec_push(tok_tags, 91);
  sv0_vec_push(pp, 55);
  int _sv0t7 = sv0_vec_new();
  int ctr = _sv0t7;
  sv0_vec_push(ctr, 0);
  int _sv0t8 = sv0_vec_new();
  int out = _sv0t8;
  sv0_vec_push(et, 0);
  sv0_vec_push(ed1, 0);
  sv0_vec_push(ed2, 0);
  sv0_vec_push(ed3, 0);
  sv0_vec_push(ed4, 0);
  int _sv0t9 = sv0_vec_new();
  int en_n = _sv0t9;
  int _sv0t10 = sv0_vec_new();
  int en_to = _sv0t10;
  int _sv0t11 = sv0_vec_new();
  int en_tc = _sv0t11;
  int _sv0t12 = sv0_vec_new();
  int en_tf = _sv0t12;
  int _sv0t13 = sv0_vec_new();
  int fn_ctx = _sv0t13;
  int _sv0t14 = sv0_vec_new();
  int builtin_map = _sv0t14;
  int _sv0t15 = sv0_vec_new();
  int lit_sf_names = _sv0t15;
  const char* source;
  source = "";
  int _sv0t16 = sv0_vec_new();
  int starts = _sv0t16;
  int _sv0t17 = sv0_vec_new();
  int ends = _sv0t17;
  int _sv0t18 = sv0_vec_new();
  int item_tags = _sv0t18;
  int _sv0t19 = sv0_vec_new();
  int item_names = _sv0t19;
  int _sv0t20 = sv0_vec_new();
  int item_d2 = _sv0t20;
  int _sv0t21 = sv0_vec_new();
  int item_d3 = _sv0t21;
  int _sv0t22 = sv0_vec_new();
  int item_d4 = _sv0t22;
  int item_fn_row = (0 - 1);
  int _sv0t23 = sv0_vec_new();
  int item_field_counts = _sv0t23;
  int _sv0t24 = sv0_vec_new();
  int enum_vnames_flat = _sv0t24;
  int _sv0t25 = sv0_vec_new();
  int enum_max_payloads = _sv0t25;
  int _sv0t26 = sv0_vec_new();
  int _sv0t27 = sv0_vec_new();
  int _sv0t28 = lower_for_effect(et, ed1, ed2, ed3, ed4, pp, tok_tags, 0, ctr, out, en_n, en_to, en_tc, en_tf, fn_ctx, builtin_map, lit_sf_names, source, starts, ends, item_tags, item_names, item_d2, item_d3, item_d4, item_field_counts, enum_vnames_flat, enum_max_payloads, item_fn_row, _sv0t26, _sv0t27);
  int rc = _sv0t28;
  if ((rc != 0)) {
    return 1;
  } else {
  }
  return 0;
}

static int test_lower_enum_ctor(void) {
  int _sv0t0 = sv0_vec_new();
  int et = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int ed1 = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int ed2 = _sv0t2;
  int _sv0t3 = sv0_vec_new();
  int ed3 = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int ed4 = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int pp = _sv0t5;
  int _sv0t6 = sv0_vec_new();
  int tok_tags = _sv0t6;
  sv0_vec_push(tok_tags, 91);
  int _sv0t7 = sv0_vec_new();
  int ctr = _sv0t7;
  sv0_vec_push(ctr, 0);
  int _sv0t8 = sv0_vec_new();
  int out = _sv0t8;
  int _sv0t9 = sv0_vec_new();
  int en_n = _sv0t9;
  int _sv0t10 = sv0_vec_new();
  int en_to = _sv0t10;
  int _sv0t11 = sv0_vec_new();
  int en_tc = _sv0t11;
  int _sv0t12 = sv0_vec_new();
  int en_tf = _sv0t12;
  int _sv0t13 = sv0_vec_new();
  int fn_ctx = _sv0t13;
  int _sv0t14 = sv0_vec_new();
  int builtin_map = _sv0t14;
  int _sv0t15 = sv0_vec_new();
  int lit_sf_names = _sv0t15;
  const char* source;
  source = "E A B";
  int _sv0t16 = sv0_vec_new();
  int starts = _sv0t16;
  int _sv0t17 = sv0_vec_new();
  int ends = _sv0t17;
  sv0_vec_push(starts, 0);
  sv0_vec_push(ends, 1);
  sv0_vec_push(starts, 2);
  sv0_vec_push(ends, 3);
  sv0_vec_push(starts, 4);
  sv0_vec_push(ends, 5);
  int _sv0t18 = sv0_vec_new();
  int item_tags = _sv0t18;
  int _sv0t19 = sv0_vec_new();
  int item_names = _sv0t19;
  int _sv0t20 = sv0_vec_new();
  int item_d2 = _sv0t20;
  int _sv0t21 = sv0_vec_new();
  int item_d3 = _sv0t21;
  int _sv0t22 = sv0_vec_new();
  int item_d4 = _sv0t22;
  int item_fn_row = (0 - 1);
  int _sv0t23 = sv0_vec_new();
  int item_field_counts = _sv0t23;
  int _sv0t24 = sv0_vec_new();
  int enum_vnames_flat = _sv0t24;
  int _sv0t25 = sv0_vec_new();
  int enum_max_payloads = _sv0t25;
  sv0_vec_push(en_n, 0);
  sv0_vec_push(en_to, 0);
  sv0_vec_push(en_tc, 2);
  sv0_vec_push(en_tf, 1);
  sv0_vec_push(en_tf, 0);
  sv0_vec_push(en_tf, 2);
  sv0_vec_push(en_tf, 1);
  sv0_vec_push(pp, 0);
  sv0_vec_push(pp, 1);
  sv0_vec_push(et, 1);
  sv0_vec_push(ed1, 0);
  sv0_vec_push(ed2, 2);
  sv0_vec_push(ed3, 0);
  sv0_vec_push(ed4, 0);
  int _sv0t26 = sv0_vec_new();
  int _sv0t27 = sv0_vec_new();
  Value _sv0t28 = lower_expr_to_value(et, ed1, ed2, ed3, ed4, pp, tok_tags, 0, ctr, out, en_n, en_to, en_tc, en_tf, fn_ctx, builtin_map, lit_sf_names, source, starts, ends, item_tags, item_names, item_d2, item_d3, item_d4, item_field_counts, enum_vnames_flat, enum_max_payloads, item_fn_row, _sv0t26, _sv0t27);
  Value v1;
  v1 = _sv0t28;
  int _sv0t29 = ir_value_tag(v1);
  if ((_sv0t29 != 3)) {
    return 1;
  } else {
  }
  int _sv0t30 = sv0_vec_len(out);
  if ((_sv0t30 != 2)) {
    return 2;
  } else {
  }
  return 0;
}

static int test_lower_try(void) {
  int _sv0t0 = sv0_vec_new();
  int et = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int ed1 = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int ed2 = _sv0t2;
  int _sv0t3 = sv0_vec_new();
  int ed3 = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int ed4 = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int pp = _sv0t5;
  int _sv0t6 = sv0_vec_new();
  int tok_tags = _sv0t6;
  sv0_vec_push(tok_tags, 91);
  int _sv0t7 = sv0_vec_new();
  int ctr = _sv0t7;
  sv0_vec_push(ctr, 0);
  int _sv0t8 = sv0_vec_new();
  int out = _sv0t8;
  int _sv0t9 = sv0_vec_new();
  int en_n = _sv0t9;
  int _sv0t10 = sv0_vec_new();
  int en_to = _sv0t10;
  int _sv0t11 = sv0_vec_new();
  int en_tc = _sv0t11;
  int _sv0t12 = sv0_vec_new();
  int en_tf = _sv0t12;
  sv0_vec_push(en_n, 500);
  sv0_vec_push(en_to, 0);
  sv0_vec_push(en_tc, 2);
  sv0_vec_push(en_tf, 600);
  sv0_vec_push(en_tf, 0);
  sv0_vec_push(en_tf, 601);
  sv0_vec_push(en_tf, 1);
  sv0_vec_push(pp, 300);
  sv0_vec_push(pp, 300);
  sv0_vec_push(et, 1);
  sv0_vec_push(ed1, 0);
  sv0_vec_push(ed2, 1);
  sv0_vec_push(ed3, 0);
  sv0_vec_push(ed4, 0);
  sv0_vec_push(et, 4);
  sv0_vec_push(ed1, 0);
  sv0_vec_push(ed2, 0);
  sv0_vec_push(ed3, 0);
  sv0_vec_push(ed4, 0);
  sv0_vec_push(et, 22);
  sv0_vec_push(ed1, 1);
  sv0_vec_push(ed2, 0);
  sv0_vec_push(ed3, 0);
  sv0_vec_push(ed4, 0);
  int _sv0t13 = sv0_vec_new();
  int fn_ctx = _sv0t13;
  int _sv0t14 = sv0_vec_new();
  int builtin_map = _sv0t14;
  int _sv0t15 = sv0_vec_new();
  int lit_sf_names = _sv0t15;
  const char* source;
  source = "";
  int _sv0t16 = sv0_vec_new();
  int starts = _sv0t16;
  int _sv0t17 = sv0_vec_new();
  int ends = _sv0t17;
  int _sv0t18 = sv0_vec_new();
  int item_tags = _sv0t18;
  int _sv0t19 = sv0_vec_new();
  int item_names = _sv0t19;
  int _sv0t20 = sv0_vec_new();
  int item_d2 = _sv0t20;
  int _sv0t21 = sv0_vec_new();
  int item_d3 = _sv0t21;
  int _sv0t22 = sv0_vec_new();
  int item_d4 = _sv0t22;
  int item_fn_row = (0 - 1);
  int _sv0t23 = sv0_vec_new();
  int item_field_counts = _sv0t23;
  int _sv0t24 = sv0_vec_new();
  int enum_vnames_flat = _sv0t24;
  int _sv0t25 = sv0_vec_new();
  int enum_max_payloads = _sv0t25;
  sv0_vec_push(fn_ctx, 1);
  sv0_vec_push(fn_ctx, 300);
  sv0_vec_push(fn_ctx, 500);
  int _sv0t26 = sv0_vec_new();
  int _sv0t27 = sv0_vec_new();
  Value _sv0t28 = lower_expr_to_value(et, ed1, ed2, ed3, ed4, pp, tok_tags, 2, ctr, out, en_n, en_to, en_tc, en_tf, fn_ctx, builtin_map, lit_sf_names, source, starts, ends, item_tags, item_names, item_d2, item_d3, item_d4, item_field_counts, enum_vnames_flat, enum_max_payloads, item_fn_row, _sv0t26, _sv0t27);
  Value v1;
  v1 = _sv0t28;
  int _sv0t29 = ir_value_tag(v1);
  if ((_sv0t29 != 3)) {
    return 1;
  } else {
  }
  int _sv0t30 = sv0_vec_len(out);
  if ((_sv0t30 < 4)) {
    return 2;
  } else {
  }
  int _sv0t31 = sv0_vec_get(out, 0);
  Instr _sv0t32;
  int _sv0t33 = sv0_box_load(_sv0t31, 0);
  _sv0t32.tag = _sv0t33;
  int _sv0t34 = sv0_box_load(_sv0t31, 1);
  _sv0t32.p0 = _sv0t34;
  int _sv0t35 = sv0_box_load(_sv0t31, 2);
  _sv0t32.p1 = _sv0t35;
  int _sv0t36 = sv0_box_load(_sv0t31, 3);
  _sv0t32.p2 = _sv0t36;
  int _sv0t37 = sv0_box_load(_sv0t31, 4);
  _sv0t32.p3 = _sv0t37;
  Instr i0;
  i0.tag = (_sv0t32).tag;
  i0.p0 = (_sv0t32).p0;
  i0.p1 = (_sv0t32).p1;
  i0.p2 = (_sv0t32).p2;
  i0.p3 = (_sv0t32).p3;
  int _sv0t38 = ir_instr_tag(i0);
  if ((_sv0t38 != 11)) {
    return 3;
  } else {
  }
  int _sv0t39 = sv0_vec_get(out, 1);
  Instr _sv0t40;
  int _sv0t41 = sv0_box_load(_sv0t39, 0);
  _sv0t40.tag = _sv0t41;
  int _sv0t42 = sv0_box_load(_sv0t39, 1);
  _sv0t40.p0 = _sv0t42;
  int _sv0t43 = sv0_box_load(_sv0t39, 2);
  _sv0t40.p1 = _sv0t43;
  int _sv0t44 = sv0_box_load(_sv0t39, 3);
  _sv0t40.p2 = _sv0t44;
  int _sv0t45 = sv0_box_load(_sv0t39, 4);
  _sv0t40.p3 = _sv0t45;
  Instr i1;
  i1.tag = (_sv0t40).tag;
  i1.p0 = (_sv0t40).p0;
  i1.p1 = (_sv0t40).p1;
  i1.p2 = (_sv0t40).p2;
  i1.p3 = (_sv0t40).p3;
  int _sv0t46 = ir_instr_tag(i1);
  if ((_sv0t46 != 2)) {
    return 4;
  } else {
  }
  int _sv0t47 = sv0_vec_get(out, 2);
  Instr _sv0t48;
  int _sv0t49 = sv0_box_load(_sv0t47, 0);
  _sv0t48.tag = _sv0t49;
  int _sv0t50 = sv0_box_load(_sv0t47, 1);
  _sv0t48.p0 = _sv0t50;
  int _sv0t51 = sv0_box_load(_sv0t47, 2);
  _sv0t48.p1 = _sv0t51;
  int _sv0t52 = sv0_box_load(_sv0t47, 3);
  _sv0t48.p2 = _sv0t52;
  int _sv0t53 = sv0_box_load(_sv0t47, 4);
  _sv0t48.p3 = _sv0t53;
  Instr i2;
  i2.tag = (_sv0t48).tag;
  i2.p0 = (_sv0t48).p0;
  i2.p1 = (_sv0t48).p1;
  i2.p2 = (_sv0t48).p2;
  i2.p3 = (_sv0t48).p3;
  int _sv0t54 = ir_instr_tag(i2);
  if ((_sv0t54 != 4)) {
    return 5;
  } else {
  }
  int _sv0t55 = sv0_vec_get(out, 3);
  Instr _sv0t56;
  int _sv0t57 = sv0_box_load(_sv0t55, 0);
  _sv0t56.tag = _sv0t57;
  int _sv0t58 = sv0_box_load(_sv0t55, 1);
  _sv0t56.p0 = _sv0t58;
  int _sv0t59 = sv0_box_load(_sv0t55, 2);
  _sv0t56.p1 = _sv0t59;
  int _sv0t60 = sv0_box_load(_sv0t55, 3);
  _sv0t56.p2 = _sv0t60;
  int _sv0t61 = sv0_box_load(_sv0t55, 4);
  _sv0t56.p3 = _sv0t61;
  Instr i3;
  i3.tag = (_sv0t56).tag;
  i3.p0 = (_sv0t56).p0;
  i3.p1 = (_sv0t56).p1;
  i3.p2 = (_sv0t56).p2;
  i3.p3 = (_sv0t56).p3;
  int _sv0t62 = ir_instr_tag(i3);
  if ((_sv0t62 != 1)) {
    return 6;
  } else {
  }
  int _sv0t63 = sv0_vec_get(out, 4);
  Instr _sv0t64;
  int _sv0t65 = sv0_box_load(_sv0t63, 0);
  _sv0t64.tag = _sv0t65;
  int _sv0t66 = sv0_box_load(_sv0t63, 1);
  _sv0t64.p0 = _sv0t66;
  int _sv0t67 = sv0_box_load(_sv0t63, 2);
  _sv0t64.p1 = _sv0t67;
  int _sv0t68 = sv0_box_load(_sv0t63, 3);
  _sv0t64.p2 = _sv0t68;
  int _sv0t69 = sv0_box_load(_sv0t63, 4);
  _sv0t64.p3 = _sv0t69;
  Instr i4;
  i4.tag = (_sv0t64).tag;
  i4.p0 = (_sv0t64).p0;
  i4.p1 = (_sv0t64).p1;
  i4.p2 = (_sv0t64).p2;
  i4.p3 = (_sv0t64).p3;
  int _sv0t70 = ir_instr_tag(i4);
  if ((_sv0t70 != 6)) {
    return 7;
  } else {
  }
  return 0;
}

static int test_build_builtin_map(void) {
  int _sv0t0 = sv0_vec_new();
  int tt = _sv0t0;
  const char* src;
  src = "println vec_push foo vec_get no_alias bar";
  int _sv0t1 = sv0_vec_new();
  int st = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int en = _sv0t2;
  sv0_vec_push(tt, 5);
  sv0_vec_push(st, 0);
  sv0_vec_push(en, 7);
  sv0_vec_push(tt, 5);
  sv0_vec_push(st, 8);
  sv0_vec_push(en, 16);
  sv0_vec_push(tt, 5);
  sv0_vec_push(st, 17);
  sv0_vec_push(en, 20);
  sv0_vec_push(tt, 5);
  sv0_vec_push(st, 21);
  sv0_vec_push(en, 28);
  sv0_vec_push(tt, 5);
  sv0_vec_push(st, 29);
  sv0_vec_push(en, 37);
  sv0_vec_push(tt, 5);
  sv0_vec_push(st, 38);
  sv0_vec_push(en, 41);
  int _sv0t3 = build_builtin_map(tt, src, st, en);
  int bm = _sv0t3;
  int _sv0t4 = sv0_vec_len(bm);
  if ((_sv0t4 != 6)) {
    return 1;
  } else {
  }
  int _sv0t5 = sv0_vec_get(bm, 0);
  if ((_sv0t5 != 1)) {
    return 2;
  } else {
  }
  int _sv0t6 = sv0_vec_get(bm, 1);
  if ((_sv0t6 != 8)) {
    return 3;
  } else {
  }
  int _sv0t7 = sv0_vec_get(bm, 2);
  if ((_sv0t7 != 0)) {
    return 4;
  } else {
  }
  int _sv0t8 = sv0_vec_get(bm, 3);
  if ((_sv0t8 != 10)) {
    return 5;
  } else {
  }
  int _sv0t9 = sv0_vec_get(bm, 4);
  if ((_sv0t9 != 14)) {
    return 6;
  } else {
  }
  int _sv0t10 = sv0_vec_get(bm, 5);
  if ((_sv0t10 != 0)) {
    return 7;
  } else {
  }
  return 0;
}

static int test_lower_match_env_helpers(void) {
  int _sv0t0 = sv0_vec_new();
  int empty = _sv0t0;
  int _sv0t1 = lower_match_env_is_legacy_path(empty);
  if ((_sv0t1 != 1)) {
    return 1;
  } else {
  }
  int _sv0t2 = lower_match_env_is_packed_v0(empty);
  if ((_sv0t2 != 0)) {
    return 2;
  } else {
  }
  int _sv0t3 = lower_match_env_alloc_v0_empty_header();
  int hdr = _sv0t3;
  int _sv0t4 = sv0_vec_len(hdr);
  int _sv0t5 = lower_match_env_hdr_len();
  if ((_sv0t4 != _sv0t5)) {
    return 3;
  } else {
  }
  int _sv0t6 = lower_match_env_is_packed_v0(hdr);
  if ((_sv0t6 != 1)) {
    return 4;
  } else {
  }
  int _sv0t7 = lower_match_env_is_legacy_path(hdr);
  if ((_sv0t7 != 0)) {
    return 5;
  } else {
  }
  int _sv0t8 = sv0_vec_get(hdr, 0);
  int _sv0t9 = lower_match_env_magic_v0();
  if ((_sv0t8 != _sv0t9)) {
    return 6;
  } else {
  }
  int _sv0t10 = sv0_vec_get(hdr, 4);
  if ((_sv0t10 != 0)) {
    return 7;
  } else {
  }
  int _sv0t11 = sv0_vec_new();
  int bad = _sv0t11;
  int _sv0t12 = lower_match_env_magic_v0();
  sv0_vec_push(bad, _sv0t12);
  sv0_vec_push(bad, 2);
  int k = 2;
  while (1) {
    int _sv0t13 = lower_match_env_hdr_len();
    int _sv0t14 = (k < _sv0t13);
    if ((!_sv0t14)) {
      break;
    } else {
    }
    sv0_vec_push(bad, 0);
    k = (k + 1);
  }
  int _sv0t15 = lower_match_env_is_packed_v0(bad);
  if ((_sv0t15 != 0)) {
    return 8;
  } else {
  }
  return 0;
}

static int test_lower_match_env_fn_pack_roundtrip(void) {
  int _sv0t0 = lower_match_env_alloc_v0_empty_header();
  int env = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int tags = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int names = _sv0t2;
  int _sv0t3 = sv0_vec_new();
  int d2 = _sv0t3;
  sv0_vec_push(tags, 0);
  sv0_vec_push(names, 100);
  sv0_vec_push(d2, 0);
  const char* src;
  src = "";
  int _sv0t4 = sv0_vec_new();
  int sts = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int ens = _sv0t5;
  int _sv0t6 = sv0_vec_new();
  int tt = _sv0t6;
  int _sv0t7 = lower_match_env_append_top_fn_match_tables(env, tags, names, d2, src, sts, ens, tt);
  int ap = _sv0t7;
  if ((ap != 0)) {
    return 1;
  } else {
  }
  int _sv0t8 = sv0_vec_new();
  int from_env_n = _sv0t8;
  int _sv0t9 = sv0_vec_new();
  int from_env_r = _sv0t9;
  int _sv0t10 = lower_match_env_unpack_fn_tables_for_match(env, from_env_n, from_env_r);
  int un = _sv0t10;
  if ((un != 0)) {
    return 2;
  } else {
  }
  int _sv0t11 = sv0_vec_new();
  int direct_n = _sv0t11;
  int _sv0t12 = sv0_vec_new();
  int direct_r = _sv0t12;
  int _sv0t13 = lower_top_fn_tables_for_match(tags, names, d2, src, sts, ens, tt, direct_n, direct_r);
  int du = _sv0t13;
  if ((du != 0)) {
    return 3;
  } else {
  }
  int _sv0t14 = sv0_vec_len(from_env_n);
  int _sv0t15 = sv0_vec_len(direct_n);
  if ((_sv0t14 != _sv0t15)) {
    return 4;
  } else {
  }
  int _sv0t16 = sv0_vec_len(from_env_r);
  int _sv0t17 = sv0_vec_len(direct_r);
  if ((_sv0t16 != _sv0t17)) {
    return 5;
  } else {
  }
  int j = 0;
  int _sv0t18 = sv0_vec_len(direct_n);
  int nlen = _sv0t18;
  while ((j < nlen)) {
    int _sv0t19 = sv0_vec_get(from_env_n, j);
    int _sv0t20 = sv0_vec_get(direct_n, j);
    if ((_sv0t19 != _sv0t20)) {
      return 6;
    } else {
    }
    int _sv0t21 = sv0_vec_get(from_env_r, j);
    int _sv0t22 = sv0_vec_get(direct_r, j);
    if ((_sv0t21 != _sv0t22)) {
      return 7;
    } else {
    }
    j = (j + 1);
  }
  int _sv0t23 = sv0_vec_get(env, 4);
  if ((_sv0t23 != 1)) {
    return 8;
  } else {
  }
  return 0;
}

static int test_lower_match_env_scrut_merge_roundtrip(void) {
  int _sv0t0 = lower_match_env_alloc_v0_empty_header();
  int parent = _sv0t0;
  int _sv0t1 = lower_match_env_append_scrut_pair(parent, 501, 701);
  int ap1 = _sv0t1;
  if ((ap1 != 0)) {
    return 1;
  } else {
  }
  int _sv0t2 = lower_match_env_append_scrut_pair(parent, 502, 702);
  int ap2 = _sv0t2;
  if ((ap2 != 0)) {
    return 2;
  } else {
  }
  int _sv0t3 = sv0_vec_get(parent, 10);
  if ((_sv0t3 != 2)) {
    return 3;
  } else {
  }
  int _sv0t4 = lower_match_env_alloc_v0_empty_header();
  int dst = _sv0t4;
  int _sv0t5 = lower_match_env_merge_parent_scrut(dst, parent);
  int mg = _sv0t5;
  if ((mg != 0)) {
    return 4;
  } else {
  }
  int _sv0t6 = sv0_vec_get(dst, 10);
  if ((_sv0t6 != 2)) {
    return 5;
  } else {
  }
  int _sv0t7 = lower_match_env_scrut_ty_lookup(dst, 99, 501);
  int ty_a = _sv0t7;
  if ((ty_a != 701)) {
    return 6;
  } else {
  }
  int _sv0t8 = lower_match_env_scrut_ty_lookup(dst, 99, 502);
  int ty_b = _sv0t8;
  if ((ty_b != 702)) {
    return 7;
  } else {
  }
  int _sv0t9 = lower_match_env_scrut_ty_lookup(dst, 1, 502);
  int ty_lim = _sv0t9;
  int _sv0t10 = (0 - 2);
  if ((ty_lim != _sv0t10)) {
    return 8;
  } else {
  }
  int _sv0t11 = lower_match_env_scrut_ty_lookup(dst, 99, 999);
  int ty_miss = _sv0t11;
  int _sv0t12 = (0 - 2);
  if ((ty_miss != _sv0t12)) {
    return 9;
  } else {
  }
  int _sv0t13 = lower_match_env_append_scrut_pair(dst, 501, 1701);
  int ap3 = _sv0t13;
  if ((ap3 != 0)) {
    return 10;
  } else {
  }
  int _sv0t14 = lower_match_env_scrut_ty_lookup(dst, 99, 501);
  int ty_shadow = _sv0t14;
  if ((ty_shadow != 1701)) {
    return 11;
  } else {
  }
  int _sv0t15 = sv0_vec_new();
  int leg = _sv0t15;
  sv0_vec_push(leg, 0);
  int _sv0t16 = lower_match_env_merge_parent_scrut(dst, leg);
  int mg2 = _sv0t16;
  if ((mg2 != 0)) {
    return 12;
  } else {
  }
  int _sv0t17 = sv0_vec_get(dst, 10);
  if ((_sv0t17 != 3)) {
    return 13;
  } else {
  }
  int _sv0t18 = sv0_vec_new();
  int bad_dst = _sv0t18;
  sv0_vec_push(bad_dst, 0);
  int _sv0t19 = lower_match_env_merge_parent_scrut(bad_dst, parent);
  int mg3 = _sv0t19;
  int _sv0t20 = (0 - 1);
  if ((mg3 != _sv0t20)) {
    return 14;
  } else {
  }
  int _sv0t21 = lower_match_env_alloc_v0_empty_header();
  int par_alt = _sv0t21;
  int _sv0t22 = sv0_vec_len(par_alt);
  int nm0 = _sv0t22;
  sv0_vec_push(par_alt, 601);
  sv0_vec_push(par_alt, 602);
  int _sv0t23 = sv0_vec_len(par_alt);
  int ty0 = _sv0t23;
  sv0_vec_push(par_alt, 801);
  sv0_vec_push(par_alt, 802);
  sv0_vec_set(par_alt, 8, nm0);
  sv0_vec_set(par_alt, 9, ty0);
  sv0_vec_set(par_alt, 10, 2);
  int _sv0t24 = lower_match_env_alloc_v0_empty_header();
  int dst2 = _sv0t24;
  int _sv0t25 = lower_match_env_merge_parent_scrut(dst2, par_alt);
  int mg4 = _sv0t25;
  if ((mg4 != 0)) {
    return 15;
  } else {
  }
  int _sv0t26 = lower_match_env_scrut_ty_lookup(dst2, 99, 601);
  int ty_c = _sv0t26;
  if ((ty_c != 801)) {
    return 16;
  } else {
  }
  int _sv0t27 = lower_match_env_scrut_ty_lookup(dst2, 99, 602);
  int ty_d = _sv0t27;
  if ((ty_d != 802)) {
    return 17;
  } else {
  }
  return 0;
}

static int test_lower_match_env_clone_scrut_roundtrip(void) {
  int _sv0t0 = lower_match_env_alloc_v0_empty_header();
  int parent = _sv0t0;
  int _sv0t1 = lower_match_env_append_scrut_pair(parent, 1001, 2001);
  int ap0 = _sv0t1;
  if ((ap0 != 0)) {
    return 1;
  } else {
  }
  int _sv0t2 = lower_match_env_clone(parent);
  int arm = _sv0t2;
  int _sv0t3 = sv0_vec_get(arm, 10);
  if ((_sv0t3 != 1)) {
    return 2;
  } else {
  }
  int _sv0t4 = lower_match_env_scrut_ty_lookup(arm, 99, 1001);
  int ty1 = _sv0t4;
  if ((ty1 != 2001)) {
    return 3;
  } else {
  }
  int _sv0t5 = lower_match_env_append_scrut_pair(arm, 1002, 2002);
  int ap1 = _sv0t5;
  if ((ap1 != 0)) {
    return 4;
  } else {
  }
  int _sv0t6 = sv0_vec_get(arm, 10);
  if ((_sv0t6 != 2)) {
    return 5;
  } else {
  }
  int _sv0t7 = sv0_vec_get(parent, 10);
  if ((_sv0t7 != 1)) {
    return 6;
  } else {
  }
  int _sv0t8 = lower_match_env_scrut_ty_lookup(arm, 99, 1002);
  int ty2 = _sv0t8;
  if ((ty2 != 2002)) {
    return 7;
  } else {
  }
  return 0;
}

int main(void) {
  int _sv0t0 = test_binop_to_c();
  int r3 = _sv0t0;
  if ((r3 != 0)) {
    int _sv0t1 = (30 + r3);
    return _sv0t1;
  } else {
  }
  int _sv0t2 = test_unop_to_c();
  int r4 = _sv0t2;
  if ((r4 != 0)) {
    int _sv0t3 = (50 + r4);
    return _sv0t3;
  } else {
  }
  int _sv0t4 = test_ty_to_c();
  int r5 = _sv0t4;
  if ((r5 != 0)) {
    int _sv0t5 = (60 + r5);
    return _sv0t5;
  } else {
  }
  int _sv0t6 = test_old_slot();
  int r6 = _sv0t6;
  if ((r6 != 0)) {
    int _sv0t7 = (80 + r6);
    return _sv0t7;
  } else {
  }
  int _sv0t8 = test_ret_syntax();
  int r7 = _sv0t8;
  if ((r7 != 0)) {
    int _sv0t9 = (90 + r7);
    return _sv0t9;
  } else {
  }
  int _sv0t10 = test_store_value_class();
  int r8 = _sv0t10;
  if ((r8 != 0)) {
    int _sv0t11 = (100 + r8);
    return _sv0t11;
  } else {
  }
  int _sv0t12 = test_store_value_to_slot();
  int r8a = _sv0t12;
  if ((r8a != 0)) {
    int _sv0t13 = (105 + r8a);
    return _sv0t13;
  } else {
  }
  int _sv0t14 = test_inject_ensures_and_ret_slot();
  int r8b = _sv0t14;
  if ((r8b != 0)) {
    int _sv0t15 = (108 + r8b);
    return _sv0t15;
  } else {
  }
  int _sv0t16 = test_assign_lhs();
  int r9 = _sv0t16;
  if ((r9 != 0)) {
    int _sv0t17 = (110 + r9);
    return _sv0t17;
  } else {
  }
  int _sv0t18 = test_enum_tag();
  int r10 = _sv0t18;
  if ((r10 != 0)) {
    int _sv0t19 = (120 + r10);
    return _sv0t19;
  } else {
  }
  int _sv0t20 = test_struct_fields();
  int r11 = _sv0t20;
  if ((r11 != 0)) {
    int _sv0t21 = (130 + r11);
    return _sv0t21;
  } else {
  }
  int _sv0t22 = test_type_detection();
  int r12 = _sv0t22;
  if ((r12 != 0)) {
    int _sv0t23 = (140 + r12);
    return _sv0t23;
  } else {
  }
  int _sv0t24 = test_variant_slots();
  int r13 = _sv0t24;
  if ((r13 != 0)) {
    int _sv0t25 = (150 + r13);
    return _sv0t25;
  } else {
  }
  int _sv0t26 = test_try_variants();
  int r14 = _sv0t26;
  if ((r14 != 0)) {
    int _sv0t27 = (160 + r14);
    return _sv0t27;
  } else {
  }
  int _sv0t28 = test_literals();
  int r15 = _sv0t28;
  if ((r15 != 0)) {
    int _sv0t29 = (170 + r15);
    return _sv0t29;
  } else {
  }
  int _sv0t30 = test_value_to_expr();
  int r16 = _sv0t30;
  if ((r16 != 0)) {
    int _sv0t31 = (180 + r16);
    return _sv0t31;
  } else {
  }
  int _sv0t32 = test_enum_alloc();
  int r17 = _sv0t32;
  if ((r17 != 0)) {
    int _sv0t33 = (190 + r17);
    return _sv0t33;
  } else {
  }
  int _sv0t34 = test_ret_value_slot();
  int r18 = _sv0t34;
  if ((r18 != 0)) {
    int _sv0t35 = (200 + r18);
    return _sv0t35;
  } else {
  }
  int _sv0t36 = test_use_ret_slot();
  int r19 = _sv0t36;
  if ((r19 != 0)) {
    int _sv0t37 = (210 + r19);
    return _sv0t37;
  } else {
  }
  int _sv0t38 = test_lit_supported();
  int r20 = _sv0t38;
  if ((r20 != 0)) {
    int _sv0t39 = (220 + r20);
    return _sv0t39;
  } else {
  }
  int _sv0t40 = test_param_name_tag();
  int r21 = _sv0t40;
  if ((r21 != 0)) {
    int _sv0t41 = (230 + r21);
    return _sv0t41;
  } else {
  }
  int _sv0t42 = test_uniq_old_names();
  int r22 = _sv0t42;
  if ((r22 != 0)) {
    int _sv0t43 = (240 + r22);
    return _sv0t43;
  } else {
  }
  int _sv0t44 = test_fresh_tmp_str();
  int r23 = _sv0t44;
  if ((r23 != 0)) {
    int _sv0t45 = (250 + r23);
    return _sv0t45;
  } else {
  }
  int _sv0t46 = test_lower_lit_ir();
  int r24 = _sv0t46;
  if ((r24 != 0)) {
    int _sv0t47 = (260 + r24);
    return _sv0t47;
  } else {
  }
  int _sv0t48 = test_fn_ret_cty();
  int r25 = _sv0t48;
  if ((r25 != 0)) {
    int _sv0t49 = (270 + r25);
    return _sv0t49;
  } else {
  }
  int _sv0t50 = test_lower_alias();
  int r26 = _sv0t50;
  if ((r26 != 0)) {
    int _sv0t51 = (280 + r26);
    return _sv0t51;
  } else {
  }
  int _sv0t52 = test_resolve_fn_callee();
  int r27 = _sv0t52;
  if ((r27 != 0)) {
    int _sv0t53 = (290 + r27);
    return _sv0t53;
  } else {
  }
  int _sv0t54 = test_resolve_ctor_path();
  int r28 = _sv0t54;
  if ((r28 != 0)) {
    int _sv0t55 = (300 + r28);
    return _sv0t55;
  } else {
  }
  int _sv0t56 = test_param_name();
  int r29 = _sv0t56;
  if ((r29 != 0)) {
    int _sv0t57 = (310 + r29);
    return _sv0t57;
  } else {
  }
  int _sv0t58 = test_lower_field_index();
  int r30 = _sv0t58;
  if ((r30 != 0)) {
    int _sv0t59 = (320 + r30);
    return _sv0t59;
  } else {
  }
  int _sv0t60 = test_ast_ty_c_string_user();
  int r31 = _sv0t60;
  if ((r31 != 0)) {
    int _sv0t61 = (330 + r31);
    return _sv0t61;
  } else {
  }
  int _sv0t62 = test_find_fn_index();
  int r32 = _sv0t62;
  if ((r32 != 0)) {
    int _sv0t63 = (340 + r32);
    return _sv0t63;
  } else {
  }
  int _sv0t64 = test_callee_fn_index();
  int r33 = _sv0t64;
  if ((r33 != 0)) {
    int _sv0t65 = (350 + r33);
    return _sv0t65;
  } else {
  }
  int _sv0t66 = test_scrut_local();
  int r34 = _sv0t66;
  if ((r34 != 0)) {
    int _sv0t67 = (360 + r34);
    return _sv0t67;
  } else {
  }
  int _sv0t68 = test_param_ty();
  int r35 = _sv0t68;
  if ((r35 != 0)) {
    int _sv0t69 = (370 + r35);
    return _sv0t69;
  } else {
  }
  int _sv0t70 = test_typedef_builders();
  int r36 = _sv0t70;
  if ((r36 != 0)) {
    int _sv0t71 = (380 + r36);
    return _sv0t71;
  } else {
  }
  int _sv0t72 = test_mentions_result();
  int r37 = _sv0t72;
  if ((r37 != 0)) {
    int _sv0t73 = (390 + r37);
    return _sv0t73;
  } else {
  }
  int _sv0t74 = test_first_result_path_handle();
  int r37a = _sv0t74;
  if ((r37a != 0)) {
    int _sv0t75 = (395 + r37a);
    return _sv0t75;
  } else {
  }
  int _sv0t76 = test_all_old_names();
  int r38 = _sv0t76;
  if ((r38 != 0)) {
    int _sv0t77 = (400 + r38);
    return _sv0t77;
  } else {
  }
  int _sv0t78 = test_expr_init_cty();
  int r39 = _sv0t78;
  if ((r39 != 0)) {
    int _sv0t79 = (410 + r39);
    return _sv0t79;
  } else {
  }
  int _sv0t80 = test_scan_lets();
  int r40 = _sv0t80;
  if ((r40 != 0)) {
    int _sv0t81 = (420 + r40);
    return _sv0t81;
  } else {
  }
  int _sv0t82 = test_try_variant_lookup();
  int r41 = _sv0t82;
  if ((r41 != 0)) {
    int _sv0t83 = (430 + r41);
    return _sv0t83;
  } else {
  }
  int _sv0t84 = test_find_variant_in_items();
  int r42 = _sv0t84;
  if ((r42 != 0)) {
    int _sv0t85 = (440 + r42);
    return _sv0t85;
  } else {
  }
  int _sv0t86 = test_scrut_locals_push();
  int r43 = _sv0t86;
  if ((r43 != 0)) {
    int _sv0t87 = (450 + r43);
    return _sv0t87;
  } else {
  }
  int _sv0t88 = test_alias_replace();
  int r44 = _sv0t88;
  if ((r44 != 0)) {
    int _sv0t89 = (460 + r44);
    return _sv0t89;
  } else {
  }
  int _sv0t90 = test_fill_import_aliases_from_use_items();
  int r44a = _sv0t90;
  if ((r44a != 0)) {
    int _sv0t91 = (465 + r44a);
    return _sv0t91;
  } else {
  }
  int _sv0t92 = test_fill_import_aliases_merges_with_preload();
  int r44b = _sv0t92;
  if ((r44b != 0)) {
    int _sv0t93 = (467 + r44b);
    return _sv0t93;
  } else {
  }
  int _sv0t94 = test_match_scrut_cty();
  int r45 = _sv0t94;
  if ((r45 != 0)) {
    int _sv0t95 = (470 + r45);
    return _sv0t95;
  } else {
  }
  int _sv0t96 = test_variant_shape_in_items();
  int r46 = _sv0t96;
  if ((r46 != 0)) {
    int _sv0t97 = (480 + r46);
    return _sv0t97;
  } else {
  }
  int _sv0t98 = test_ir_types();
  int r47 = _sv0t98;
  if ((r47 != 0)) {
    int _sv0t99 = (490 + r47);
    return _sv0t99;
  } else {
  }
  int _sv0t100 = test_ctr_fresh();
  int r48 = _sv0t100;
  if ((r48 != 0)) {
    int _sv0t101 = (510 + r48);
    return _sv0t101;
  } else {
  }
  int _sv0t102 = test_classify_arena();
  int r49 = _sv0t102;
  if ((r49 != 0)) {
    int _sv0t103 = (520 + r49);
    return _sv0t103;
  } else {
  }
  int _sv0t104 = test_parse_decimal_i32_from_tok();
  int r49b = _sv0t104;
  if ((r49b != 0)) {
    int _sv0t105 = (525 + r49b);
    return _sv0t105;
  } else {
  }
  int _sv0t106 = test_lower_lit_val();
  int r50 = _sv0t106;
  if ((r50 != 0)) {
    int _sv0t107 = (540 + r50);
    return _sv0t107;
  } else {
  }
  int _sv0t108 = test_lower_expr_val();
  int r51 = _sv0t108;
  if ((r51 != 0)) {
    int _sv0t109 = (560 + r51);
    return _sv0t109;
  } else {
  }
  int _sv0t110 = test_lower_return();
  int r52 = _sv0t110;
  if ((r52 != 0)) {
    int _sv0t111 = (580 + r52);
    return _sv0t111;
  } else {
  }
  int _sv0t112 = test_lower_if();
  int r53 = _sv0t112;
  if ((r53 != 0)) {
    int _sv0t113 = (600 + r53);
    return _sv0t113;
  } else {
  }
  int _sv0t114 = test_lower_block();
  int r54 = _sv0t114;
  if ((r54 != 0)) {
    int _sv0t115 = (620 + r54);
    return _sv0t115;
  } else {
  }
  int _sv0t116 = test_lower_assign();
  int r55 = _sv0t116;
  if ((r55 != 0)) {
    int _sv0t117 = (640 + r55);
    return _sv0t117;
  } else {
  }
  int _sv0t118 = test_lower_while();
  int r56 = _sv0t118;
  if ((r56 != 0)) {
    int _sv0t119 = (660 + r56);
    return _sv0t119;
  } else {
  }
  int _sv0t120 = test_lower_loop();
  int r57 = _sv0t120;
  if ((r57 != 0)) {
    int _sv0t121 = (680 + r57);
    return _sv0t121;
  } else {
  }
  int _sv0t122 = test_lower_cast();
  int r58 = _sv0t122;
  if ((r58 != 0)) {
    int _sv0t123 = (700 + r58);
    return _sv0t123;
  } else {
  }
  int _sv0t124 = test_value_to_expr_fn();
  int r59 = _sv0t124;
  if ((r59 != 0)) {
    int _sv0t125 = (720 + r59);
    return _sv0t125;
  } else {
  }
  int _sv0t126 = test_lower_with_instrs();
  int r60 = _sv0t126;
  if ((r60 != 0)) {
    int _sv0t127 = (740 + r60);
    return _sv0t127;
  } else {
  }
  int _sv0t128 = test_lower_for_effect_fn();
  int r61 = _sv0t128;
  if ((r61 != 0)) {
    int _sv0t129 = (760 + r61);
    return _sv0t129;
  } else {
  }
  int _sv0t130 = test_lower_return_fn();
  int r62 = _sv0t130;
  if ((r62 != 0)) {
    int _sv0t131 = (780 + r62);
    return _sv0t131;
  } else {
  }
  int _sv0t132 = test_lower_stmt_fn();
  int r63 = _sv0t132;
  if ((r63 != 0)) {
    int _sv0t133 = (800 + r63);
    return _sv0t133;
  } else {
  }
  int _sv0t134 = test_lower_block_fn();
  int r64 = _sv0t134;
  if ((r64 != 0)) {
    int _sv0t135 = (820 + r64);
    return _sv0t135;
  } else {
  }
  int _sv0t136 = test_lower_body_fn();
  int r65 = _sv0t136;
  if ((r65 != 0)) {
    int _sv0t137 = (840 + r65);
    return _sv0t137;
  } else {
  }
  int _sv0t138 = test_lower_fn_fn();
  int r66 = _sv0t138;
  if ((r66 != 0)) {
    int _sv0t139 = (860 + r66);
    return _sv0t139;
  } else {
  }
  int _sv0t140 = test_lower_fn_contract_glue();
  int r66a = _sv0t140;
  if ((r66a != 0)) {
    int _sv0t141 = (865 + r66a);
    return _sv0t141;
  } else {
  }
  int _sv0t142 = test_lower_program_fn();
  int r67 = _sv0t142;
  if ((r67 != 0)) {
    int _sv0t143 = (880 + r67);
    return _sv0t143;
  } else {
  }
  int _sv0t144 = test_lower_for_loop();
  int r68 = _sv0t144;
  if ((r68 != 0)) {
    int _sv0t145 = (900 + r68);
    return _sv0t145;
  } else {
  }
  int _sv0t146 = test_lower_expr_assert();
  int r68a = _sv0t146;
  if ((r68a != 0)) {
    int _sv0t147 = (903 + r68a);
    return _sv0t147;
  } else {
  }
  int _sv0t148 = test_lower_expr_range();
  int r68ar = _sv0t148;
  if ((r68ar != 0)) {
    int _sv0t149 = (904 + r68ar);
    return _sv0t149;
  } else {
  }
  int _sv0t150 = test_lower_expr_tuple_field();
  int r68tf = _sv0t150;
  if ((r68tf != 0)) {
    int _sv0t151 = (906 + r68tf);
    return _sv0t151;
  } else {
  }
  int _sv0t152 = test_lower_expr_index_effect();
  int r68ix = _sv0t152;
  if ((r68ix != 0)) {
    int _sv0t153 = (907 + r68ix);
    return _sv0t153;
  } else {
  }
  int _sv0t154 = test_lower_expr_method_call_effect();
  int r68mc = _sv0t154;
  if ((r68mc != 0)) {
    int _sv0t155 = (908 + r68mc);
    return _sv0t155;
  } else {
  }
  int _sv0t156 = test_lower_expr_array_effect();
  int r68arr = _sv0t156;
  if ((r68arr != 0)) {
    int _sv0t157 = (909 + r68arr);
    return _sv0t157;
  } else {
  }
  int _sv0t158 = test_lower_expr_struct();
  int r68b = _sv0t158;
  if ((r68b != 0)) {
    int _sv0t159 = (905 + r68b);
    return _sv0t159;
  } else {
  }
  int _sv0t160 = test_lower_expr_struct_qualified_path();
  int r68bq = _sv0t160;
  if ((r68bq != 0)) {
    int _sv0t161 = (910 + r68bq);
    return _sv0t161;
  } else {
  }
  int _sv0t162 = test_lower_enum_ctor();
  int r69 = _sv0t162;
  if ((r69 != 0)) {
    int _sv0t163 = (920 + r69);
    return _sv0t163;
  } else {
  }
  int _sv0t164 = test_lower_try();
  int r70 = _sv0t164;
  if ((r70 != 0)) {
    int _sv0t165 = (940 + r70);
    return _sv0t165;
  } else {
  }
  int _sv0t166 = test_lower_match_env_helpers();
  int r70e = _sv0t166;
  if ((r70e != 0)) {
    int _sv0t167 = (955 + r70e);
    return _sv0t167;
  } else {
  }
  int _sv0t168 = test_lower_match_env_fn_pack_roundtrip();
  int r70f = _sv0t168;
  if ((r70f != 0)) {
    int _sv0t169 = (956 + r70f);
    return _sv0t169;
  } else {
  }
  int _sv0t170 = test_lower_match_env_scrut_merge_roundtrip();
  int r70g = _sv0t170;
  if ((r70g != 0)) {
    int _sv0t171 = (957 + r70g);
    return _sv0t171;
  } else {
  }
  int _sv0t172 = test_lower_match_env_clone_scrut_roundtrip();
  int r70h = _sv0t172;
  if ((r70h != 0)) {
    int _sv0t173 = (958 + r70h);
    return _sv0t173;
  } else {
  }
  int _sv0t174 = test_build_builtin_map();
  int r71 = _sv0t174;
  if ((r71 != 0)) {
    int _sv0t175 = (960 + r71);
    return _sv0t175;
  } else {
  }
  return 0;
}


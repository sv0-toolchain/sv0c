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

static int variant_slots_unit(void);
static int variant_slots_tuple(int field_count);
static int variant_slots_struct(int field_count);
static int variant_slots(int variant_kind, int field_count);
static int enum_field_count_from_width(int width);
static int enum_field_is_tag(int idx);
static int enum_payload_field_index(int field_idx);
static int layout_lookup(int names, int widths, int name);
static int is_void_cty(const char* cty);
static int is_scalar_cty(const char* cty);
static int width_of_cty(int cty_handle, int structs_names, int structs_field_counts, int enums_names, int enums_widths, const char* source, int starts, int ends);
static int cty_cat_of_handle(int cty_h, const char* source, int starts, int ends);
static int width_of_cty_scalar(const char* cty);
static int width_of_cty_simple(const char* cty);
static int index_of_field(int fields, int field_name);
static int index_of_field_range(int fields_flat, int start, int count, int name, const char* source, int starts, int ends);
static int enum_field_count(int width);
static int enum_field_names(int width);
static int binop_to_insn(const char* op);
static int unop_to_insn(const char* op);
static int insn_size(int opc);
static int enc_len(int opcode);
static int enc_lens(int opcodes);
static int loop_exit_sentinel(void);
static int loop_continue_sentinel(void);
static int loop_break_sentinel(void);
static int is_loop_exit_sentinel(int v);
static int is_loop_continue_sentinel(int v);
static int is_loop_break_sentinel(int v);
static int pool_new(void);
static const char* pool_resolve(int h, const char* source, int starts, int ends);
static int pool_key_eq(int a, int b, const char* source, int starts, int ends);
static int pool_add(int pool, int s, const char* source, int starts, int ends);
static int pool_size(int pool);
static int contract_pool_key(int fn_h, int kind);
static int builtin_id(const char* name);
static int is_builtin(const char* name);
static int builtin_has_result(const char* name);
static int builtin_arg_count(const char* name);
static int value_width_scalar(void);
static int value_is_var(int value_tag);
static int value_width(int value_tag, int var_name, int env_names, int env_bases, int env_widths);
static int store_slot_offset(int base, int width, int k);
static int slot_handle_in_range(int h, int starts);
static int slot_name_eq(int a, int b, const char* source, int starts, int ends);
static int lookup_slot(int env_names, int env_bases, int env_widths, int name, const char* source, int starts, int ends);
static int slot_base(int env_bases, int idx);
static int slot_width(int env_widths, int idx);
static int alloc_local(int env_names, int env_bases, int env_widths, int name, int width, int slot);
static int bind_param(int name, int cty_handle, int slot, int env_names, int env_bases, int env_widths, int env_fields, int structs_names, int structs_field_counts, int enums_names, int enums_widths);
static int replace_loop_exit_jump(int instrs, int back_offset);
static int insn_vec_stride(int opc);
static int patch_continue_jumps(int instrs, int from_pos, int target_abs);
static int patch_break_jumps(int instrs, int from_pos, int target_abs);
static int struct_layouts_build(int item_tags, int item_names, int item_field_counts, int out_names, int out_field_counts);
static int record_field_layout(int cty, int structs_names, int structs_field_counts, int struct_fields_flat, int struct_field_starts, const char* source, int starts, int ends, int env_fields_flat);
static int struct_fields_build(int item_tags, int item_field_counts, int item_field_bases, int struct_field_name_toks, int out_fields_flat, int out_field_starts);
static int enum_layouts_build(int item_tags, int item_names, int item_variant_counts, int item_variant_max_payload, int out_names, int out_widths);
static int binop_tag_to_insn(int tag);
static int unop_tag_to_insn(int tag);
static int binop_tag_to_insn_typed(int tag, int cat);
static int unop_tag_to_insn_typed(int tag, int cat);
static int slot_cat(int env_cats, int idx);
static int combine_cat(int a, int b);
static int operand_cat(Value v, int env_names, int env_bases, int env_widths, int env_cats, const char* source, int starts, int ends);
static int binop_handle_to_insn(int op_h, const char* source, int starts, int ends);
static int unop_handle_to_insn(int op_h, const char* source, int starts, int ends);
static const char* handle_to_str(int h, const char* source, int starts, int ends);
static int index_of_fn(int fn_h, int fn_names, const char* source, int starts, int ends);
static int member_offset_from_field(int width, int field_h);
static int emit_member_load_from_var(int x, int field_h, int env_names, int env_bases, int env_widths, int env_field_starts, int env_fields_flat, const char* source, int starts, int ends, int out);
static int push_wide_int_literal(int h, const char* source, int starts, int ends, int out);
static int bi_limbs(void);
static int bi_base_bits(void);
static int bi_base_mask(void);
static int bi_zero(void);
static int bi_set_u32(int a, int v);
static int bi_copy(int a);
static int bi_is_zero(int a);
static int bi_cmp(int a, int b);
static int bi_mul_small(int a, int m);
static int bi_mul_pow10(int a, int e);
static int bi_shl_small(int a, int r);
static int bi_shl_limbs(int a, int k);
static int bi_shl_bits(int a, int nbits);
static int bi_sub(int a, int b);
static int bit_len_15(int x0);
static int bi_bitlen(int a);
static int bi_bit(int a, int p);
static int is_dec_digit(int c);
static int parse_f64_literal(const char* s, int out_digits, int out_meta);
static int emit_value(Value v, int env_names, int env_bases, int env_widths, int env_field_starts, int env_fields_flat, int pool, const char* source, int starts, int ends, int out);
static int emit_expr(Expr e, int env_names, int env_bases, int env_widths, int env_cats, int env_field_starts, int env_fields_flat, int pool, const char* source, int starts, int ends, int out);
static int vec_append(int dst, int src);
static int flat_byte_size(int instrs);
static int emit_ret_width(Value v, int env_names, int env_bases, int env_widths, const char* source, int starts, int ends);
static int emit_box_new(int dst_h, int args, int env_names, int env_bases, int env_widths, int env_field_starts, int env_fields_flat, int pool, const char* source, int starts, int ends, int out);
static int emit_instr(Instr ins, int env_names, int env_bases, int env_widths, int env_cats, int env_field_starts, int env_fields_flat, int pool, const char* source, int starts, int ends, int fn_names, int out);
static int emit_instrs(int instrs, int env_names, int env_bases, int env_widths, int env_cats, int env_field_starts, int env_fields_flat, int pool, const char* source, int starts, int ends, int fn_names, int out);
static int prepool_loop_seq(int instrs, int i0, int env_names, int env_bases, int env_widths, int env_cats, int env_field_starts, int env_fields_flat, int pool, const char* source, int starts, int ends, int fn_names);
static int ensure_var_int(int name, int env_names, int env_bases, int env_widths, int env_cats, int env_field_starts, int next_slot, const char* source, int starts, int ends);
static int scan_instr_env(Instr ins, int env_names, int env_bases, int env_widths, int env_cats, int env_field_starts, int next_slot, int structs_names, int structs_field_counts, int enums_names, int enums_widths, int structs_fields_flat, int structs_field_starts, int env_fields_flat, const char* source, int starts, int ends);
static int scan_instrs_env(int instrs, int env_names, int env_bases, int env_widths, int env_cats, int env_field_starts, int next_slot, int structs_names, int structs_field_counts, int enums_names, int enums_widths, int structs_fields_flat, int structs_field_starts, int env_fields_flat, const char* source, int starts, int ends);
static int local_count_and_env(int param_names, int param_ctys, int instrs, int env_names, int env_bases, int env_widths, int env_cats, int env_field_starts, int structs_names, int structs_field_counts, int enums_names, int enums_widths, int structs_fields_flat, int structs_field_starts, int env_fields_flat, const char* source, int starts, int ends);
static int compute_arity(int param_ctys, int structs_names, int structs_field_counts, int enums_names, int enums_widths, const char* source, int starts, int ends);
static int emit_fn(int label, int param_names, int param_ctys, int instrs, int structs_names, int structs_field_counts, int enums_names, int enums_widths, int structs_fields_flat, int structs_field_starts, int pool, const char* source, int starts, int ends, int fn_names, int ft_entries);
static int is_main_label(int label, const char* source, int starts, int ends);
static int build_func_order(int block_labels, const char* source, int starts, int ends, int order);
static int emit_program(int item_tags, int item_names, int item_field_counts, int item_variant_max_payload, int item_field_bases, int struct_field_name_toks, int block_labels, int block_param_names, int block_param_ctys, int block_instrs, const char* source, int starts, int ends, int out_pool, int out_ft);
static int test_variant_slots(void);
static int test_cty_classify(void);
static int test_width_of_cty(void);
static int test_enum_field_helpers(void);
static int test_layout_lookup(void);
static int test_index_of_field(void);
static int test_binop_to_insn(void);
static int test_unop_to_insn(void);
static int test_enc_len(void);
static int test_sentinels(void);
static int test_pool(void);
static int test_builtin_id(void);
static int test_store_ordering(void);
static int test_lookup_slot(void);
static int test_alloc_local(void);
static int test_replace_loop_exit(void);
static int test_enum_field_names(void);
static int test_value_width(void);
static int test_bind_param(void);
static int test_patch_continue_jumps(void);
static int test_struct_layouts_build(void);
static int test_enum_layouts_build(void);
static int test_emit_value(void);
static int test_emit_expr(void);
static int test_emit_instr(void);
static int test_emit_instrs(void);
static int test_vec_append(void);
static int test_flat_byte_size(void);
static int test_emit_ifelse_block(void);
static int test_emit_while_loop(void);
static int test_emit_call(void);
static int test_emit_store_field(void);
static int test_emit_contract(void);
static int test_compute_arity(void);
static int test_emit_fn(void);
static int test_local_count_and_env(void);
static int test_is_main_label(void);
static int test_build_func_order(void);
static int emit_program_1fn(const char* source, int starts, int ends, int lab, int pn, int pc, int ins, int pool, int ft);
static int test_emit_program(void);
static int bi_from(int v);
static int test_bigint(void);
static int pfl_check(const char* s, int want_neg, int want_exp, int want_digs);
static int dv(int a, int b, int c, int d, int e, int f, int cnt);
static int test_parse_f64_literal(void);
static int test_wide_int_literal(void);
static int test_typed_opcode_select(void);

static int variant_slots_unit(void) {
  return 0;
}

static int variant_slots_tuple(int field_count) {
  return field_count;
}

static int variant_slots_struct(int field_count) {
  return field_count;
}

static int variant_slots(int variant_kind, int field_count) {
  if ((variant_kind == 0)) {
    return 0;
  } else {
  }
  return field_count;
}

static int enum_field_count_from_width(int width) {
  return width;
}

static int enum_field_is_tag(int idx) {
  int _sv0t0 = (idx == 0);
  return _sv0t0;
}

static int enum_payload_field_index(int field_idx) {
  int _sv0t0 = (field_idx - 1);
  return _sv0t0;
}

static int layout_lookup(int names, int widths, int name) {
  int _sv0t0 = sv0_vec_len(names);
  int n = _sv0t0;
  int i = 0;
  while ((i < n)) {
    int _sv0t1 = sv0_vec_get(names, i);
    if ((_sv0t1 == name)) {
      int _sv0t2 = sv0_vec_get(widths, i);
      return _sv0t2;
    } else {
    }
    i = (i + 1);
  }
  int _sv0t3 = (0 - 1);
  return _sv0t3;
}

static int is_void_cty(const char* cty) {
  int _sv0t0 = sv0_string_eq(cty, "void");
  return _sv0t0;
}

static int is_scalar_cty(const char* cty) {
  int _sv0t0 = sv0_string_eq(cty, "bool");
  if (_sv0t0) {
    return 1;
  } else {
  }
  int _sv0t1 = sv0_string_len(cty);
  if ((_sv0t1 >= 3)) {
    int _sv0t2 = sv0_string_char_at(cty, 0);
    int c0 = _sv0t2;
    int _sv0t3 = sv0_string_char_at(cty, 1);
    int c1 = _sv0t3;
    int _sv0t4 = sv0_string_char_at(cty, 2);
    int c2 = _sv0t4;
    if ((c0 == 105)) {
      if ((c1 == 110)) {
        if ((c2 == 116)) {
          return 1;
        } else {
        }
      } else {
      }
    } else {
    }
    if ((c0 == 117)) {
      if ((c1 == 105)) {
        if ((c2 == 110)) {
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

static int width_of_cty(int cty_handle, int structs_names, int structs_field_counts, int enums_names, int enums_widths, const char* source, int starts, int ends) {
  int _sv0t0 = sv0_vec_len(structs_names);
  int sn = _sv0t0;
  int si = 0;
  while ((si < sn)) {
    int _sv0t1 = sv0_vec_get(structs_names, si);
    int _sv0t2 = slot_name_eq(_sv0t1, cty_handle, source, starts, ends);
    if (_sv0t2) {
      int _sv0t3 = sv0_vec_get(structs_field_counts, si);
      return _sv0t3;
    } else {
    }
    si = (si + 1);
  }
  int _sv0t4 = sv0_vec_len(enums_names);
  int en = _sv0t4;
  int ei = 0;
  while ((ei < en)) {
    int _sv0t5 = sv0_vec_get(enums_names, ei);
    int _sv0t6 = slot_name_eq(_sv0t5, cty_handle, source, starts, ends);
    if (_sv0t6) {
      int _sv0t7 = sv0_vec_get(enums_widths, ei);
      return _sv0t7;
    } else {
    }
    ei = (ei + 1);
  }
  return 1;
}

static int cty_cat_of_handle(int cty_h, const char* source, int starts, int ends) {
  int _sv0t0 = slot_handle_in_range(cty_h, starts);
  if ((_sv0t0 != 1)) {
    return 0;
  } else {
  }
  const char* _sv0t1 = handle_to_str(cty_h, source, starts, ends);
  const char* s;
  s = _sv0t1;
  int _sv0t2 = sv0_string_eq(s, "f64");
  if (_sv0t2) {
    return 1;
  } else {
  }
  int _sv0t3 = sv0_string_eq(s, "f32");
  if (_sv0t3) {
    return 1;
  } else {
  }
  int _sv0t4 = sv0_string_eq(s, "i64");
  if (_sv0t4) {
    return 2;
  } else {
  }
  int _sv0t5 = sv0_string_eq(s, "u64");
  if (_sv0t5) {
    return 2;
  } else {
  }
  return 0;
}

static int width_of_cty_scalar(const char* cty) {
  int _sv0t0 = is_void_cty(cty);
  if (_sv0t0) {
    return 0;
  } else {
  }
  return 1;
}

static int width_of_cty_simple(const char* cty) {
  int _sv0t0 = is_void_cty(cty);
  if (_sv0t0) {
    return 0;
  } else {
  }
  return 1;
}

static int index_of_field(int fields, int field_name) {
  int _sv0t0 = sv0_vec_len(fields);
  int n = _sv0t0;
  int i = 0;
  while ((i < n)) {
    int _sv0t1 = sv0_vec_get(fields, i);
    if ((_sv0t1 == field_name)) {
      return i;
    } else {
    }
    i = (i + 1);
  }
  int _sv0t2 = (0 - 1);
  return _sv0t2;
}

static int index_of_field_range(int fields_flat, int start, int count, int name, const char* source, int starts, int ends) {
  int i = 0;
  while ((i < count)) {
    int _sv0t0 = (start + i);
    int _sv0t1 = sv0_vec_get(fields_flat, _sv0t0);
    int _sv0t2 = slot_name_eq(_sv0t1, name, source, starts, ends);
    if (_sv0t2) {
      return i;
    } else {
    }
    i = (i + 1);
  }
  int _sv0t3 = (0 - 1);
  return _sv0t3;
}

static int enum_field_count(int width) {
  if ((width <= 1)) {
    return 1;
  } else {
  }
  return width;
}

static int enum_field_names(int width) {
  int _sv0t0 = sv0_vec_new();
  int out = _sv0t0;
  int i = 0;
  while ((i < width)) {
    sv0_vec_push(out, i);
    i = (i + 1);
  }
  return out;
}

static int binop_to_insn(const char* op) {
  int _sv0t0 = sv0_string_eq(op, "+");
  if (_sv0t0) {
    return 16;
  } else {
  }
  int _sv0t1 = sv0_string_eq(op, "-");
  if (_sv0t1) {
    return 17;
  } else {
  }
  int _sv0t2 = sv0_string_eq(op, "*");
  if (_sv0t2) {
    return 18;
  } else {
  }
  int _sv0t3 = sv0_string_eq(op, "/");
  if (_sv0t3) {
    return 19;
  } else {
  }
  int _sv0t4 = sv0_string_eq(op, "%");
  if (_sv0t4) {
    return 20;
  } else {
  }
  int _sv0t5 = sv0_string_eq(op, "==");
  if (_sv0t5) {
    return 64;
  } else {
  }
  int _sv0t6 = sv0_string_eq(op, "!=");
  if (_sv0t6) {
    return 65;
  } else {
  }
  int _sv0t7 = sv0_string_eq(op, "<");
  if (_sv0t7) {
    return 66;
  } else {
  }
  int _sv0t8 = sv0_string_eq(op, ">");
  if (_sv0t8) {
    return 67;
  } else {
  }
  int _sv0t9 = sv0_string_eq(op, "<=");
  if (_sv0t9) {
    return 68;
  } else {
  }
  int _sv0t10 = sv0_string_eq(op, ">=");
  if (_sv0t10) {
    return 69;
  } else {
  }
  int _sv0t11 = sv0_string_eq(op, "&&");
  if (_sv0t11) {
    return 80;
  } else {
  }
  int _sv0t12 = sv0_string_eq(op, "||");
  if (_sv0t12) {
    return 81;
  } else {
  }
  int _sv0t13 = sv0_string_eq(op, "&");
  if (_sv0t13) {
    return 88;
  } else {
  }
  int _sv0t14 = sv0_string_eq(op, "|");
  if (_sv0t14) {
    return 89;
  } else {
  }
  int _sv0t15 = sv0_string_eq(op, "^");
  if (_sv0t15) {
    return 90;
  } else {
  }
  int _sv0t16 = sv0_string_eq(op, "<<");
  if (_sv0t16) {
    return 92;
  } else {
  }
  int _sv0t17 = sv0_string_eq(op, ">>");
  if (_sv0t17) {
    return 93;
  } else {
  }
  int _sv0t18 = (0 - 1);
  return _sv0t18;
}

static int unop_to_insn(const char* op) {
  int _sv0t0 = sv0_string_eq(op, "!");
  if (_sv0t0) {
    return 82;
  } else {
  }
  int _sv0t1 = sv0_string_eq(op, "-");
  if (_sv0t1) {
    return 21;
  } else {
  }
  int _sv0t2 = (0 - 1);
  return _sv0t2;
}

static int insn_size(int opc) {
  if ((opc == 0)) {
    return 1;
  } else {
  }
  if ((opc == 1)) {
    return 1;
  } else {
  }
  if ((opc == 2)) {
    return 1;
  } else {
  }
  if ((opc == 3)) {
    return 1;
  } else {
  }
  if ((opc == 4)) {
    return 5;
  } else {
  }
  if ((opc == 5)) {
    return 9;
  } else {
  }
  if ((opc == 6)) {
    return 9;
  } else {
  }
  if ((opc == 7)) {
    return 2;
  } else {
  }
  if ((opc == 8)) {
    return 5;
  } else {
  }
  if ((opc >= 16)) {
    if ((opc <= 21)) {
      return 1;
    } else {
    }
  } else {
  }
  if ((opc >= 32)) {
    if ((opc <= 37)) {
      return 1;
    } else {
    }
  } else {
  }
  if ((opc >= 48)) {
    if ((opc <= 52)) {
      return 1;
    } else {
    }
  } else {
  }
  if ((opc >= 64)) {
    if ((opc <= 69)) {
      return 1;
    } else {
    }
  } else {
  }
  if ((opc >= 80)) {
    if ((opc <= 82)) {
      return 1;
    } else {
    }
  } else {
  }
  if ((opc >= 88)) {
    if ((opc <= 93)) {
      return 1;
    } else {
    }
  } else {
  }
  if ((opc == 96)) {
    return 5;
  } else {
  }
  if ((opc == 97)) {
    return 5;
  } else {
  }
  if ((opc == 112)) {
    return 5;
  } else {
  }
  if ((opc == 113)) {
    return 5;
  } else {
  }
  if ((opc == 114)) {
    return 5;
  } else {
  }
  if ((opc == 115)) {
    return 9;
  } else {
  }
  if ((opc == 116)) {
    return 1;
  } else {
  }
  if ((opc == 117)) {
    return 5;
  } else {
  }
  if ((opc == 118)) {
    return 2;
  } else {
  }
  if ((opc == 128)) {
    return 5;
  } else {
  }
  if ((opc == 129)) {
    return 5;
  } else {
  }
  if ((opc == 130)) {
    return 5;
  } else {
  }
  if ((opc == 131)) {
    return 5;
  } else {
  }
  if ((opc == 132)) {
    return 1;
  } else {
  }
  if ((opc == 133)) {
    return 1;
  } else {
  }
  if ((opc == 144)) {
    return 13;
  } else {
  }
  if ((opc == 145)) {
    return 1;
  } else {
  }
  if ((opc == 146)) {
    return 5;
  } else {
  }
  if ((opc == 160)) {
    return 5;
  } else {
  }
  if ((opc == 161)) {
    return 5;
  } else {
  }
  return 0;
}

static int enc_len(int opcode) {
  int _sv0t0 = insn_size(opcode);
  return _sv0t0;
}

static int enc_lens(int opcodes) {
  int _sv0t0 = sv0_vec_len(opcodes);
  int n = _sv0t0;
  int total = 0;
  int i = 0;
  while ((i < n)) {
    int _sv0t1 = sv0_vec_get(opcodes, i);
    int _sv0t2 = insn_size(_sv0t1);
    total = (total + _sv0t2);
    i = (i + 1);
  }
  return total;
}

static int loop_exit_sentinel(void) {
  int _sv0t0 = (0 - 987654321);
  return _sv0t0;
}

static int loop_continue_sentinel(void) {
  int _sv0t0 = (0 - 876543210);
  return _sv0t0;
}

static int loop_break_sentinel(void) {
  int _sv0t0 = (0 - 765432109);
  return _sv0t0;
}

static int is_loop_exit_sentinel(int v) {
  int _sv0t0 = (0 - 987654321);
  int _sv0t1 = (v == _sv0t0);
  return _sv0t1;
}

static int is_loop_continue_sentinel(int v) {
  int _sv0t0 = (0 - 876543210);
  int _sv0t1 = (v == _sv0t0);
  return _sv0t1;
}

static int is_loop_break_sentinel(int v) {
  int _sv0t0 = (0 - 765432109);
  int _sv0t1 = (v == _sv0t0);
  return _sv0t1;
}

static int pool_new(void) {
  int _sv0t0 = sv0_vec_new();
  return _sv0t0;
}

static const char* pool_resolve(int h, const char* source, int starts, int ends) {
  const char* _sv0t0 = handle_to_str(h, source, starts, ends);
  const char* raw;
  raw = _sv0t0;
  int _sv0t1 = sv0_string_len(raw);
  int n = _sv0t1;
  if ((n >= 2)) {
    int _sv0t2 = sv0_string_char_at(raw, 0);
    if ((_sv0t2 == 34)) {
      int _sv0t3 = (n - 2);
      const char* _sv0t4 = sv0_string_substr(raw, 1, _sv0t3);
      return _sv0t4;
    } else {
    }
  } else {
  }
  return raw;
}

static int pool_key_eq(int a, int b, const char* source, int starts, int ends) {
  if ((a == b)) {
    return 1;
  } else {
  }
  int _sv0t0 = sv0_string_len(source);
  if ((_sv0t0 == 0)) {
    return 0;
  } else {
  }
  int _sv0t1 = slot_handle_in_range(a, starts);
  if ((_sv0t1 != 1)) {
    return 0;
  } else {
  }
  int _sv0t2 = slot_handle_in_range(b, starts);
  if ((_sv0t2 != 1)) {
    return 0;
  } else {
  }
  const char* _sv0t3 = pool_resolve(a, source, starts, ends);
  const char* _sv0t4 = pool_resolve(b, source, starts, ends);
  int _sv0t5 = sv0_string_eq(_sv0t3, _sv0t4);
  return _sv0t5;
}

static int pool_add(int pool, int s, const char* source, int starts, int ends) {
  int _sv0t0 = sv0_vec_len(pool);
  int n = _sv0t0;
  int i = 0;
  while ((i < n)) {
    int _sv0t1 = sv0_vec_get(pool, i);
    int _sv0t2 = pool_key_eq(_sv0t1, s, source, starts, ends);
    if (_sv0t2) {
      return i;
    } else {
    }
    i = (i + 1);
  }
  sv0_vec_push(pool, s);
  return n;
}

static int pool_size(int pool) {
  int _sv0t0 = sv0_vec_len(pool);
  return _sv0t0;
}

static int contract_pool_key(int fn_h, int kind) {
  int _sv0t0 = (kind * 1000000);
  int _sv0t1 = (_sv0t0 + fn_h);
  int _sv0t2 = (_sv0t1 + 1);
  int _sv0t3 = (0 - _sv0t2);
  return _sv0t3;
}

static int builtin_id(const char* name) {
  int _sv0t0 = sv0_string_eq(name, "sv0_println");
  if (_sv0t0) {
    return 0;
  } else {
  }
  int _sv0t1 = sv0_string_eq(name, "sv0_no_alias");
  if (_sv0t1) {
    return 1;
  } else {
  }
  int _sv0t2 = sv0_string_eq(name, "sv0_string_len");
  if (_sv0t2) {
    return 2;
  } else {
  }
  int _sv0t3 = sv0_string_eq(name, "sv0_string_eq");
  if (_sv0t3) {
    return 3;
  } else {
  }
  int _sv0t4 = sv0_string_eq(name, "sv0_string_concat");
  if (_sv0t4) {
    return 4;
  } else {
  }
  int _sv0t5 = sv0_string_eq(name, "sv0_string_char_at");
  if (_sv0t5) {
    return 5;
  } else {
  }
  int _sv0t6 = sv0_string_eq(name, "sv0_string_substr");
  if (_sv0t6) {
    return 6;
  } else {
  }
  int _sv0t7 = sv0_string_eq(name, "sv0_vec_new");
  if (_sv0t7) {
    return 7;
  } else {
  }
  int _sv0t8 = sv0_string_eq(name, "sv0_vec_push");
  if (_sv0t8) {
    return 8;
  } else {
  }
  int _sv0t9 = sv0_string_eq(name, "sv0_vec_len");
  if (_sv0t9) {
    return 9;
  } else {
  }
  int _sv0t10 = sv0_string_eq(name, "sv0_vec_get");
  if (_sv0t10) {
    return 10;
  } else {
  }
  int _sv0t11 = sv0_string_eq(name, "sv0_vec_set");
  if (_sv0t11) {
    return 11;
  } else {
  }
  int _sv0t12 = sv0_string_eq(name, "sv0_box_alloc");
  if (_sv0t12) {
    return 12;
  } else {
  }
  int _sv0t13 = sv0_string_eq(name, "sv0_box_store");
  if (_sv0t13) {
    return 13;
  } else {
  }
  int _sv0t14 = sv0_string_eq(name, "sv0_box_load");
  if (_sv0t14) {
    return 14;
  } else {
  }
  int _sv0t15 = sv0_string_eq(name, "sv0_read_file");
  if (_sv0t15) {
    return 15;
  } else {
  }
  int _sv0t16 = sv0_string_eq(name, "sv0_write_file");
  if (_sv0t16) {
    return 16;
  } else {
  }
  int _sv0t17 = sv0_string_eq(name, "sv0_read_dir");
  if (_sv0t17) {
    return 17;
  } else {
  }
  int _sv0t18 = (0 - 1);
  return _sv0t18;
}

static int is_builtin(const char* name) {
  int _sv0t0 = builtin_id(name);
  int _sv0t1 = (_sv0t0 >= 0);
  return _sv0t1;
}

static int builtin_has_result(const char* name) {
  int _sv0t0 = sv0_string_eq(name, "sv0_println");
  if (_sv0t0) {
    return 0;
  } else {
  }
  int _sv0t1 = sv0_string_eq(name, "sv0_vec_push");
  if (_sv0t1) {
    return 0;
  } else {
  }
  int _sv0t2 = sv0_string_eq(name, "sv0_vec_set");
  if (_sv0t2) {
    return 0;
  } else {
  }
  int _sv0t3 = sv0_string_eq(name, "sv0_box_store");
  if (_sv0t3) {
    return 0;
  } else {
  }
  int _sv0t4 = sv0_string_eq(name, "sv0_write_file");
  if (_sv0t4) {
    return 0;
  } else {
  }
  return 1;
}

static int builtin_arg_count(const char* name) {
  int _sv0t0 = sv0_string_eq(name, "sv0_println");
  if (_sv0t0) {
    return 1;
  } else {
  }
  int _sv0t1 = sv0_string_eq(name, "sv0_no_alias");
  if (_sv0t1) {
    return 0;
  } else {
  }
  int _sv0t2 = sv0_string_eq(name, "sv0_string_len");
  if (_sv0t2) {
    return 1;
  } else {
  }
  int _sv0t3 = sv0_string_eq(name, "sv0_string_eq");
  if (_sv0t3) {
    return 2;
  } else {
  }
  int _sv0t4 = sv0_string_eq(name, "sv0_string_concat");
  if (_sv0t4) {
    return 2;
  } else {
  }
  int _sv0t5 = sv0_string_eq(name, "sv0_string_char_at");
  if (_sv0t5) {
    return 2;
  } else {
  }
  int _sv0t6 = sv0_string_eq(name, "sv0_string_substr");
  if (_sv0t6) {
    return 3;
  } else {
  }
  int _sv0t7 = sv0_string_eq(name, "sv0_vec_new");
  if (_sv0t7) {
    return 0;
  } else {
  }
  int _sv0t8 = sv0_string_eq(name, "sv0_vec_push");
  if (_sv0t8) {
    return 2;
  } else {
  }
  int _sv0t9 = sv0_string_eq(name, "sv0_vec_len");
  if (_sv0t9) {
    return 1;
  } else {
  }
  int _sv0t10 = sv0_string_eq(name, "sv0_vec_get");
  if (_sv0t10) {
    return 2;
  } else {
  }
  int _sv0t11 = sv0_string_eq(name, "sv0_vec_set");
  if (_sv0t11) {
    return 3;
  } else {
  }
  int _sv0t12 = sv0_string_eq(name, "sv0_box_alloc");
  if (_sv0t12) {
    return 1;
  } else {
  }
  int _sv0t13 = sv0_string_eq(name, "sv0_box_store");
  if (_sv0t13) {
    return 3;
  } else {
  }
  int _sv0t14 = sv0_string_eq(name, "sv0_box_load");
  if (_sv0t14) {
    return 2;
  } else {
  }
  int _sv0t15 = sv0_string_eq(name, "sv0_read_file");
  if (_sv0t15) {
    return 1;
  } else {
  }
  int _sv0t16 = sv0_string_eq(name, "sv0_write_file");
  if (_sv0t16) {
    return 2;
  } else {
  }
  int _sv0t17 = sv0_string_eq(name, "sv0_read_dir");
  if (_sv0t17) {
    return 1;
  } else {
  }
  return 0;
}

static int value_width_scalar(void) {
  return 1;
}

static int value_is_var(int value_tag) {
  int _sv0t0 = (value_tag == 4);
  return _sv0t0;
}

static int value_width(int value_tag, int var_name, int env_names, int env_bases, int env_widths) {
  if ((value_tag == 4)) {
    int _sv0t0 = sv0_vec_new();
    int _sv0t1 = sv0_vec_new();
    int _sv0t2 = lookup_slot(env_names, env_bases, env_widths, var_name, "", _sv0t0, _sv0t1);
    int idx = _sv0t2;
    if ((idx >= 0)) {
      int _sv0t3 = slot_width(env_widths, idx);
      return _sv0t3;
    } else {
    }
  } else {
  }
  return 1;
}

static int store_slot_offset(int base, int width, int k) {
  int _sv0t0 = (base + width);
  int _sv0t1 = (_sv0t0 - 1);
  int _sv0t2 = (_sv0t1 - k);
  return _sv0t2;
}

static int slot_handle_in_range(int h, int starts) {
  if ((h < 0)) {
    return 0;
  } else {
  }
  int _sv0t0 = sv0_vec_len(starts);
  int _sv0t1 = (h < _sv0t0);
  return _sv0t1;
}

static int slot_name_eq(int a, int b, const char* source, int starts, int ends) {
  if ((a == b)) {
    return 1;
  } else {
  }
  int _sv0t0 = sv0_string_len(source);
  if ((_sv0t0 == 0)) {
    return 0;
  } else {
  }
  int _sv0t1 = slot_handle_in_range(a, starts);
  if ((_sv0t1 != 1)) {
    return 0;
  } else {
  }
  int _sv0t2 = slot_handle_in_range(b, starts);
  if ((_sv0t2 != 1)) {
    return 0;
  } else {
  }
  const char* _sv0t3 = handle_to_str(a, source, starts, ends);
  const char* _sv0t4 = handle_to_str(b, source, starts, ends);
  int _sv0t5 = sv0_string_eq(_sv0t3, _sv0t4);
  return _sv0t5;
}

static int lookup_slot(int env_names, int env_bases, int env_widths, int name, const char* source, int starts, int ends) {
  int _sv0t0 = sv0_vec_len(env_names);
  int n = _sv0t0;
  int i = (n - 1);
  while ((i >= 0)) {
    int _sv0t1 = sv0_vec_get(env_names, i);
    int _sv0t2 = slot_name_eq(_sv0t1, name, source, starts, ends);
    if (_sv0t2) {
      return i;
    } else {
    }
    i = (i - 1);
  }
  int _sv0t3 = (0 - 1);
  return _sv0t3;
}

static int slot_base(int env_bases, int idx) {
  int _sv0t0 = sv0_vec_get(env_bases, idx);
  return _sv0t0;
}

static int slot_width(int env_widths, int idx) {
  int _sv0t0 = sv0_vec_get(env_widths, idx);
  return _sv0t0;
}

static int alloc_local(int env_names, int env_bases, int env_widths, int name, int width, int slot) {
  sv0_vec_push(env_names, name);
  sv0_vec_push(env_bases, slot);
  sv0_vec_push(env_widths, width);
  int _sv0t0 = (slot + width);
  return _sv0t0;
}

static int bind_param(int name, int cty_handle, int slot, int env_names, int env_bases, int env_widths, int env_fields, int structs_names, int structs_field_counts, int enums_names, int enums_widths) {
  int _sv0t0 = sv0_vec_new();
  int _sv0t1 = sv0_vec_new();
  int _sv0t2 = width_of_cty(cty_handle, structs_names, structs_field_counts, enums_names, enums_widths, "", _sv0t0, _sv0t1);
  int w = _sv0t2;
  sv0_vec_push(env_names, name);
  sv0_vec_push(env_bases, slot);
  sv0_vec_push(env_widths, w);
  int _sv0t3 = sv0_vec_len(env_fields);
  sv0_vec_push(env_fields, _sv0t3);
  int _sv0t4 = (slot + w);
  return _sv0t4;
}

static int replace_loop_exit_jump(int instrs, int back_offset) {
  int _sv0t0 = sv0_vec_len(instrs);
  int n = _sv0t0;
  if ((n < 2)) {
    return 0;
  } else {
  }
  int _sv0t1 = (n - 2);
  int _sv0t2 = sv0_vec_get(instrs, _sv0t1);
  int last_op = _sv0t2;
  int _sv0t3 = (n - 1);
  int _sv0t4 = sv0_vec_get(instrs, _sv0t3);
  int last_arg = _sv0t4;
  if ((last_op != 112)) {
    return 0;
  } else {
  }
  int _sv0t5 = loop_exit_sentinel();
  if ((last_arg != _sv0t5)) {
    return 0;
  } else {
  }
  int _sv0t6 = (n - 1);
  sv0_vec_set(instrs, _sv0t6, back_offset);
  return 1;
}

static int insn_vec_stride(int opc) {
  int _sv0t0 = insn_size(opc);
  int bs = _sv0t0;
  if ((bs <= 1)) {
    return 1;
  } else {
  }
  int payload = (bs - 1);
  int _sv0t1 = (payload + 3);
  int slots = (_sv0t1 / 4);
  int _sv0t2 = (1 + slots);
  return _sv0t2;
}

static int patch_continue_jumps(int instrs, int from_pos, int target_abs) {
  int _sv0t0 = sv0_vec_len(instrs);
  int n = _sv0t0;
  int i = 0;
  int pos = from_pos;
  int patched = 0;
  while ((i < n)) {
    int _sv0t1 = sv0_vec_get(instrs, i);
    int op = _sv0t1;
    int _sv0t2 = insn_size(op);
    int byte_sz = _sv0t2;
    int _sv0t3 = insn_vec_stride(op);
    int stride = _sv0t3;
    if ((op == 112)) {
      int _sv0t4 = (i + 1);
      if ((_sv0t4 < n)) {
        int _sv0t5 = (i + 1);
        int _sv0t6 = sv0_vec_get(instrs, _sv0t5);
        int arg = _sv0t6;
        int _sv0t7 = is_loop_continue_sentinel(arg);
        if (_sv0t7) {
          int _sv0t8 = (i + 1);
          int _sv0t9 = (pos + byte_sz);
          int _sv0t10 = (target_abs - _sv0t9);
          sv0_vec_set(instrs, _sv0t8, _sv0t10);
          patched = (patched + 1);
        } else {
        }
      } else {
      }
    } else {
    }
    i = (i + stride);
    pos = (pos + byte_sz);
  }
  return patched;
}

static int patch_break_jumps(int instrs, int from_pos, int target_abs) {
  int _sv0t0 = sv0_vec_len(instrs);
  int n = _sv0t0;
  int i = 0;
  int pos = from_pos;
  int patched = 0;
  while ((i < n)) {
    int _sv0t1 = sv0_vec_get(instrs, i);
    int op = _sv0t1;
    int _sv0t2 = insn_size(op);
    int byte_sz = _sv0t2;
    int _sv0t3 = insn_vec_stride(op);
    int stride = _sv0t3;
    if ((op == 112)) {
      int _sv0t4 = (i + 1);
      if ((_sv0t4 < n)) {
        int _sv0t5 = (i + 1);
        int _sv0t6 = sv0_vec_get(instrs, _sv0t5);
        int arg = _sv0t6;
        int _sv0t7 = is_loop_break_sentinel(arg);
        if (_sv0t7) {
          int _sv0t8 = (i + 1);
          int _sv0t9 = (pos + byte_sz);
          int _sv0t10 = (target_abs - _sv0t9);
          sv0_vec_set(instrs, _sv0t8, _sv0t10);
          patched = (patched + 1);
        } else {
        }
      } else {
      }
    } else {
    }
    i = (i + stride);
    pos = (pos + byte_sz);
  }
  return patched;
}

static int struct_layouts_build(int item_tags, int item_names, int item_field_counts, int out_names, int out_field_counts) {
  int _sv0t0 = sv0_vec_len(item_tags);
  int n = _sv0t0;
  int count = 0;
  int i = 0;
  while ((i < n)) {
    int _sv0t1 = sv0_vec_get(item_tags, i);
    if ((_sv0t1 == 1)) {
      int _sv0t2 = sv0_vec_get(item_names, i);
      sv0_vec_push(out_names, _sv0t2);
      int _sv0t3 = sv0_vec_get(item_field_counts, i);
      sv0_vec_push(out_field_counts, _sv0t3);
      count = (count + 1);
    } else {
    }
    i = (i + 1);
  }
  return count;
}

static int record_field_layout(int cty, int structs_names, int structs_field_counts, int struct_fields_flat, int struct_field_starts, const char* source, int starts, int ends, int env_fields_flat) {
  int _sv0t0 = sv0_vec_len(structs_names);
  int ns = _sv0t0;
  int k = 0;
  while ((k < ns)) {
    int _sv0t1 = sv0_vec_get(structs_names, k);
    int _sv0t2 = slot_name_eq(_sv0t1, cty, source, starts, ends);
    if (_sv0t2) {
      int _sv0t3 = sv0_vec_len(struct_field_starts);
      if ((k >= _sv0t3)) {
        return 0;
      } else {
      }
      int _sv0t4 = sv0_vec_len(env_fields_flat);
      int fstart = _sv0t4;
      int _sv0t5 = sv0_vec_get(structs_field_counts, k);
      int fcount = _sv0t5;
      int _sv0t6 = sv0_vec_get(struct_field_starts, k);
      int sbase = _sv0t6;
      int j = 0;
      while ((j < fcount)) {
        int _sv0t7 = (sbase + j);
        int _sv0t8 = sv0_vec_get(struct_fields_flat, _sv0t7);
        sv0_vec_push(env_fields_flat, _sv0t8);
        j = (j + 1);
      }
      return fstart;
    } else {
    }
    k = (k + 1);
  }
  return 0;
}

static int struct_fields_build(int item_tags, int item_field_counts, int item_field_bases, int struct_field_name_toks, int out_fields_flat, int out_field_starts) {
  int _sv0t0 = sv0_vec_len(item_tags);
  int n = _sv0t0;
  int i = 0;
  while ((i < n)) {
    int _sv0t1 = sv0_vec_get(item_tags, i);
    if ((_sv0t1 == 1)) {
      int _sv0t2 = sv0_vec_len(out_fields_flat);
      sv0_vec_push(out_field_starts, _sv0t2);
      int _sv0t3 = sv0_vec_get(item_field_counts, i);
      int fc = _sv0t3;
      int _sv0t4 = sv0_vec_get(item_field_bases, i);
      int base = _sv0t4;
      int k = 0;
      while ((k < fc)) {
        int _sv0t5 = (base + k);
        int _sv0t6 = sv0_vec_get(struct_field_name_toks, _sv0t5);
        sv0_vec_push(out_fields_flat, _sv0t6);
        k = (k + 1);
      }
    } else {
    }
    i = (i + 1);
  }
  int _sv0t7 = sv0_vec_len(out_field_starts);
  return _sv0t7;
}

static int enum_layouts_build(int item_tags, int item_names, int item_variant_counts, int item_variant_max_payload, int out_names, int out_widths) {
  int _sv0t0 = sv0_vec_len(item_tags);
  int n = _sv0t0;
  int count = 0;
  int i = 0;
  while ((i < n)) {
    int _sv0t1 = sv0_vec_get(item_tags, i);
    if ((_sv0t1 == 2)) {
      int _sv0t2 = sv0_vec_get(item_names, i);
      sv0_vec_push(out_names, _sv0t2);
      int _sv0t3 = sv0_vec_get(item_variant_max_payload, i);
      int _sv0t4 = (1 + _sv0t3);
      sv0_vec_push(out_widths, _sv0t4);
      count = (count + 1);
    } else {
    }
    i = (i + 1);
  }
  return count;
}

static int binop_tag_to_insn(int tag) {
  if ((tag == 0)) {
    return 16;
  } else {
  }
  if ((tag == 1)) {
    return 17;
  } else {
  }
  if ((tag == 2)) {
    return 18;
  } else {
  }
  if ((tag == 3)) {
    return 19;
  } else {
  }
  if ((tag == 4)) {
    return 20;
  } else {
  }
  if ((tag == 5)) {
    return 64;
  } else {
  }
  if ((tag == 6)) {
    return 65;
  } else {
  }
  if ((tag == 7)) {
    return 66;
  } else {
  }
  if ((tag == 8)) {
    return 67;
  } else {
  }
  if ((tag == 9)) {
    return 68;
  } else {
  }
  if ((tag == 10)) {
    return 69;
  } else {
  }
  if ((tag == 11)) {
    return 80;
  } else {
  }
  if ((tag == 12)) {
    return 81;
  } else {
  }
  if ((tag == 13)) {
    return 88;
  } else {
  }
  if ((tag == 14)) {
    return 89;
  } else {
  }
  if ((tag == 15)) {
    return 90;
  } else {
  }
  if ((tag == 16)) {
    return 92;
  } else {
  }
  if ((tag == 17)) {
    return 93;
  } else {
  }
  int _sv0t0 = (0 - 1);
  return _sv0t0;
}

static int unop_tag_to_insn(int tag) {
  if ((tag == 0)) {
    return 21;
  } else {
  }
  if ((tag == 1)) {
    return 82;
  } else {
  }
  int _sv0t0 = (0 - 1);
  return _sv0t0;
}

static int binop_tag_to_insn_typed(int tag, int cat) {
  if ((cat == 1)) {
    if ((tag == 0)) {
      return 48;
    } else {
    }
    if ((tag == 1)) {
      return 49;
    } else {
    }
    if ((tag == 2)) {
      return 50;
    } else {
    }
    if ((tag == 3)) {
      return 51;
    } else {
    }
    if ((tag == 4)) {
      int _sv0t0 = (0 - 1);
      return _sv0t0;
    } else {
    }
    if ((tag >= 13)) {
      int _sv0t1 = (0 - 1);
      return _sv0t1;
    } else {
    }
    int _sv0t2 = binop_tag_to_insn(tag);
    return _sv0t2;
  } else {
  }
  if ((cat == 2)) {
    if ((tag == 0)) {
      return 32;
    } else {
    }
    if ((tag == 1)) {
      return 33;
    } else {
    }
    if ((tag == 2)) {
      return 34;
    } else {
    }
    if ((tag == 3)) {
      return 35;
    } else {
    }
    if ((tag == 4)) {
      return 36;
    } else {
    }
    int _sv0t3 = binop_tag_to_insn(tag);
    return _sv0t3;
  } else {
  }
  int _sv0t4 = binop_tag_to_insn(tag);
  return _sv0t4;
}

static int unop_tag_to_insn_typed(int tag, int cat) {
  if ((tag == 0)) {
    if ((cat == 1)) {
      return 52;
    } else {
    }
    if ((cat == 2)) {
      return 37;
    } else {
    }
    return 21;
  } else {
  }
  int _sv0t0 = unop_tag_to_insn(tag);
  return _sv0t0;
}

static int slot_cat(int env_cats, int idx) {
  if ((idx < 0)) {
    return 0;
  } else {
  }
  int _sv0t0 = sv0_vec_len(env_cats);
  if ((idx >= _sv0t0)) {
    return 0;
  } else {
  }
  int _sv0t1 = sv0_vec_get(env_cats, idx);
  return _sv0t1;
}

static int combine_cat(int a, int b) {
  if ((a == 1)) {
    return 1;
  } else {
  }
  if ((b == 1)) {
    return 1;
  } else {
  }
  if ((a == 2)) {
    return 2;
  } else {
  }
  if ((b == 2)) {
    return 2;
  } else {
  }
  return 0;
}

static int operand_cat(Value v, int env_names, int env_bases, int env_widths, int env_cats, const char* source, int starts, int ends) {
  int _sv0t0;
  if ((v.tag == 8)) {
    int h = v.p0;
    return 1;
    _sv0t0 = 0;
  } else {
    if ((v.tag == 9)) {
      int h = v.p0;
      return 2;
      _sv0t0 = 0;
    } else {
      if ((v.tag == 3)) {
        int x = v.p0;
        int _sv0t1 = lookup_slot(env_names, env_bases, env_widths, x, source, starts, ends);
        int si = _sv0t1;
        int _sv0t2 = slot_cat(env_cats, si);
        return _sv0t2;
        _sv0t0 = 0;
      } else {
        if ((v.tag == 0)) {
          int n = v.p0;
          return 0;
          _sv0t0 = 0;
        } else {
          if ((v.tag == 1)) {
            return 0;
            _sv0t0 = 0;
          } else {
            if ((v.tag == 2)) {
              return 0;
              _sv0t0 = 0;
            } else {
              if ((v.tag == 4)) {
                return 0;
                _sv0t0 = 0;
              } else {
                if ((v.tag == 5)) {
                  int s = v.p0;
                  return 0;
                  _sv0t0 = 0;
                } else {
                  if ((v.tag == 6)) {
                    int bv = v.p0;
                    int f = v.p1;
                    return 0;
                    _sv0t0 = 0;
                  } else {
                    if ((v.tag == 7)) {
                      int x = v.p0;
                      return 0;
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
  return 0;
}

static int binop_handle_to_insn(int op_h, const char* source, int starts, int ends) {
  int _sv0t0 = sv0_vec_get(starts, op_h);
  int s = _sv0t0;
  int _sv0t1 = sv0_vec_get(ends, op_h);
  int e = _sv0t1;
  int _sv0t2 = (e - s);
  const char* _sv0t3 = sv0_string_substr(source, s, _sv0t2);
  const char* op;
  op = _sv0t3;
  int _sv0t4 = binop_to_insn(op);
  return _sv0t4;
}

static int unop_handle_to_insn(int op_h, const char* source, int starts, int ends) {
  int _sv0t0 = sv0_vec_get(starts, op_h);
  int s = _sv0t0;
  int _sv0t1 = sv0_vec_get(ends, op_h);
  int e = _sv0t1;
  int _sv0t2 = (e - s);
  const char* _sv0t3 = sv0_string_substr(source, s, _sv0t2);
  const char* op;
  op = _sv0t3;
  int _sv0t4 = unop_to_insn(op);
  return _sv0t4;
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

static int index_of_fn(int fn_h, int fn_names, const char* source, int starts, int ends) {
  const char* _sv0t0 = handle_to_str(fn_h, source, starts, ends);
  const char* target;
  target = _sv0t0;
  int _sv0t1 = sv0_vec_len(fn_names);
  int n = _sv0t1;
  int i = 0;
  while ((i < n)) {
    int _sv0t2 = sv0_vec_get(fn_names, i);
    const char* _sv0t3 = handle_to_str(_sv0t2, source, starts, ends);
    const char* candidate;
    candidate = _sv0t3;
    int _sv0t4 = sv0_string_eq(target, candidate);
    if (_sv0t4) {
      return i;
    } else {
    }
    i = (i + 1);
  }
  int _sv0t5 = (0 - 1);
  return _sv0t5;
}

static int member_offset_from_field(int width, int field_h) {
  if ((width <= 0)) {
    int _sv0t0 = (0 - 1);
    return _sv0t0;
  } else {
  }
  int off = field_h;
  int _sv0t1 = (0 - 1);
  if ((field_h == _sv0t1)) {
    off = 0;
  } else {
  }
  int _sv0t2 = (0 - 10);
  if ((field_h <= _sv0t2)) {
    int _sv0t3 = (0 - 10);
    int _sv0t4 = (_sv0t3 - field_h);
    off = (_sv0t4 + 1);
  } else {
  }
  if ((off < 0)) {
    int _sv0t5 = (0 - 1);
    return _sv0t5;
  } else {
  }
  if ((off >= width)) {
    int _sv0t6 = (0 - 1);
    return _sv0t6;
  } else {
  }
  return off;
}

static int emit_member_load_from_var(int x, int field_h, int env_names, int env_bases, int env_widths, int env_field_starts, int env_fields_flat, const char* source, int starts, int ends, int out) {
  int _sv0t0 = lookup_slot(env_names, env_bases, env_widths, x, source, starts, ends);
  int si = _sv0t0;
  if ((si < 0)) {
    int _sv0t1 = (0 - 1);
    return _sv0t1;
  } else {
  }
  int _sv0t2 = slot_base(env_bases, si);
  int base = _sv0t2;
  int _sv0t3 = slot_width(env_widths, si);
  int width = _sv0t3;
  int _sv0t4 = member_offset_from_field(width, field_h);
  int off = _sv0t4;
  if ((field_h >= 0)) {
    int _sv0t5 = sv0_vec_len(env_field_starts);
    if ((si < _sv0t5)) {
      int _sv0t6 = sv0_vec_get(env_field_starts, si);
      int fs = _sv0t6;
      int _sv0t7 = index_of_field_range(env_fields_flat, fs, width, field_h, source, starts, ends);
      off = _sv0t7;
    } else {
    }
  } else {
  }
  if ((off < 0)) {
    int _sv0t8 = (0 - 1);
    return _sv0t8;
  } else {
  }
  sv0_vec_push(out, 96);
  int _sv0t9 = (base + off);
  sv0_vec_push(out, _sv0t9);
  return 2;
}

static int push_wide_int_literal(int h, const char* source, int starts, int ends, int out) {
  int _sv0t0 = slot_handle_in_range(h, starts);
  if ((_sv0t0 != 1)) {
    int _sv0t1 = (0 - 1);
    return _sv0t1;
  } else {
  }
  const char* _sv0t2 = handle_to_str(h, source, starts, ends);
  const char* s;
  s = _sv0t2;
  int _sv0t3 = sv0_string_len(s);
  int n = _sv0t3;
  if ((n <= 0)) {
    int _sv0t4 = (0 - 1);
    return _sv0t4;
  } else {
  }
  int l0 = 0;
  int l1 = 0;
  int l2 = 0;
  int l3 = 0;
  int i = 0;
  while ((i < n)) {
    int _sv0t5 = sv0_string_char_at(s, i);
    int c = _sv0t5;
    if ((c < 48)) {
      int _sv0t6 = (0 - 1);
      return _sv0t6;
    } else {
    }
    if ((c > 57)) {
      int _sv0t7 = (0 - 1);
      return _sv0t7;
    } else {
    }
    int d = (c - 48);
    int _sv0t8 = (l0 * 10);
    int t0 = (_sv0t8 + d);
    l0 = (t0 & 65535);
    int _sv0t9 = (l1 * 10);
    int _sv0t10 = (t0 >> 16);
    int t1 = (_sv0t9 + _sv0t10);
    l1 = (t1 & 65535);
    int _sv0t11 = (l2 * 10);
    int _sv0t12 = (t1 >> 16);
    int t2 = (_sv0t11 + _sv0t12);
    l2 = (t2 & 65535);
    int _sv0t13 = (l3 * 10);
    int _sv0t14 = (t2 >> 16);
    int t3 = (_sv0t13 + _sv0t14);
    l3 = (t3 & 65535);
    i = (i + 1);
  }
  int _sv0t15 = (l1 << 16);
  int lo = (l0 | _sv0t15);
  int _sv0t16 = (l3 << 16);
  int hi = (l2 | _sv0t16);
  sv0_vec_push(out, 5);
  sv0_vec_push(out, lo);
  sv0_vec_push(out, hi);
  return 3;
}

static int bi_limbs(void) {
  return 160;
}

static int bi_base_bits(void) {
  return 15;
}

static int bi_base_mask(void) {
  return 32767;
}

static int bi_zero(void) {
  int _sv0t0 = sv0_vec_new();
  int a = _sv0t0;
  int i = 0;
  while (1) {
    int _sv0t1 = bi_limbs();
    int _sv0t2 = (i < _sv0t1);
    if ((!_sv0t2)) {
      break;
    } else {
    }
    sv0_vec_push(a, 0);
    i = (i + 1);
  }
  return a;
}

static int bi_set_u32(int a, int v) {
  int i = 0;
  while (1) {
    int _sv0t0 = bi_limbs();
    int _sv0t1 = (i < _sv0t0);
    if ((!_sv0t1)) {
      break;
    } else {
    }
    sv0_vec_set(a, i, 0);
    i = (i + 1);
  }
  int _sv0t2 = (v & 32767);
  sv0_vec_set(a, 0, _sv0t2);
  int _sv0t3 = (v >> 15);
  int _sv0t4 = (_sv0t3 & 32767);
  sv0_vec_set(a, 1, _sv0t4);
  int _sv0t5 = (v >> 30);
  int _sv0t6 = (_sv0t5 & 32767);
  sv0_vec_set(a, 2, _sv0t6);
  return 0;
}

static int bi_copy(int a) {
  int _sv0t0 = sv0_vec_new();
  int b = _sv0t0;
  int i = 0;
  while (1) {
    int _sv0t1 = bi_limbs();
    int _sv0t3 = (i < _sv0t1);
    if ((!_sv0t3)) {
      break;
    } else {
    }
    int _sv0t2 = sv0_vec_get(a, i);
    sv0_vec_push(b, _sv0t2);
    i = (i + 1);
  }
  return b;
}

static int bi_is_zero(int a) {
  int i = 0;
  while (1) {
    int _sv0t0 = bi_limbs();
    int _sv0t2 = (i < _sv0t0);
    if ((!_sv0t2)) {
      break;
    } else {
    }
    int _sv0t1 = sv0_vec_get(a, i);
    if ((_sv0t1 != 0)) {
      return 0;
    } else {
    }
    i = (i + 1);
  }
  return 1;
}

static int bi_cmp(int a, int b) {
  int _sv0t0 = bi_limbs();
  int i = (_sv0t0 - 1);
  while ((i >= 0)) {
    int _sv0t1 = sv0_vec_get(a, i);
    int av = _sv0t1;
    int _sv0t2 = sv0_vec_get(b, i);
    int bv = _sv0t2;
    if ((av < bv)) {
      int _sv0t3 = (0 - 1);
      return _sv0t3;
    } else {
    }
    if ((av > bv)) {
      return 1;
    } else {
    }
    i = (i - 1);
  }
  return 0;
}

static int bi_mul_small(int a, int m) {
  int carry = 0;
  int i = 0;
  while (1) {
    int _sv0t0 = bi_limbs();
    int _sv0t4 = (i < _sv0t0);
    if ((!_sv0t4)) {
      break;
    } else {
    }
    int _sv0t1 = sv0_vec_get(a, i);
    int _sv0t2 = (_sv0t1 * m);
    int t = (_sv0t2 + carry);
    int _sv0t3 = (t & 32767);
    sv0_vec_set(a, i, _sv0t3);
    carry = (t >> 15);
    i = (i + 1);
  }
  return 0;
}

static int bi_mul_pow10(int a, int e) {
  int k = e;
  while ((k >= 4)) {
    int _sv0t0 = bi_mul_small(a, 10000);
    int _u1 = _sv0t0;
    k = (k - 4);
  }
  while ((k > 0)) {
    int _sv0t1 = bi_mul_small(a, 10);
    int _u2 = _sv0t1;
    k = (k - 1);
  }
  return 0;
}

static int bi_shl_small(int a, int r) {
  if ((r <= 0)) {
    return 0;
  } else {
  }
  int carry = 0;
  int i = 0;
  while (1) {
    int _sv0t0 = bi_limbs();
    int _sv0t4 = (i < _sv0t0);
    if ((!_sv0t4)) {
      break;
    } else {
    }
    int _sv0t1 = sv0_vec_get(a, i);
    int _sv0t2 = (_sv0t1 << r);
    int t = (_sv0t2 | carry);
    int _sv0t3 = (t & 32767);
    sv0_vec_set(a, i, _sv0t3);
    carry = (t >> 15);
    i = (i + 1);
  }
  return 0;
}

static int bi_shl_limbs(int a, int k) {
  if ((k <= 0)) {
    return 0;
  } else {
  }
  int _sv0t0 = bi_limbs();
  int i = (_sv0t0 - 1);
  while ((i >= 0)) {
    if ((i >= k)) {
      int _sv0t1 = (i - k);
      int _sv0t2 = sv0_vec_get(a, _sv0t1);
      sv0_vec_set(a, i, _sv0t2);
    } else {
      sv0_vec_set(a, i, 0);
    }
    i = (i - 1);
  }
  return 0;
}

static int bi_shl_bits(int a, int nbits) {
  if ((nbits <= 0)) {
    return 0;
  } else {
  }
  int _sv0t0 = (nbits / 15);
  int _sv0t1 = bi_shl_limbs(a, _sv0t0);
  int _u1 = _sv0t1;
  int _sv0t2 = (nbits / 15);
  int _sv0t3 = (_sv0t2 * 15);
  int _sv0t4 = (nbits - _sv0t3);
  int _sv0t5 = bi_shl_small(a, _sv0t4);
  int _u2 = _sv0t5;
  return 0;
}

static int bi_sub(int a, int b) {
  int borrow = 0;
  int i = 0;
  while (1) {
    int _sv0t0 = bi_limbs();
    int _sv0t4 = (i < _sv0t0);
    if ((!_sv0t4)) {
      break;
    } else {
    }
    int _sv0t1 = sv0_vec_get(a, i);
    int _sv0t2 = sv0_vec_get(b, i);
    int _sv0t3 = (_sv0t1 - _sv0t2);
    int t = (_sv0t3 - borrow);
    if ((t < 0)) {
      t = (t + 32768);
      borrow = 1;
    } else {
      borrow = 0;
    }
    sv0_vec_set(a, i, t);
    i = (i + 1);
  }
  return 0;
}

static int bit_len_15(int x0) {
  int x = x0;
  int n = 0;
  while ((x > 0)) {
    n = (n + 1);
    x = (x >> 1);
  }
  return n;
}

static int bi_bitlen(int a) {
  int _sv0t0 = bi_limbs();
  int i = (_sv0t0 - 1);
  while ((i >= 0)) {
    int _sv0t1 = sv0_vec_get(a, i);
    int v = _sv0t1;
    if ((v != 0)) {
      int _sv0t2 = (i * 15);
      int _sv0t3 = bit_len_15(v);
      int _sv0t4 = (_sv0t2 + _sv0t3);
      return _sv0t4;
    } else {
    }
    i = (i - 1);
  }
  return 0;
}

static int bi_bit(int a, int p) {
  if ((p < 0)) {
    return 0;
  } else {
  }
  int limb = (p / 15);
  int _sv0t0 = bi_limbs();
  if ((limb >= _sv0t0)) {
    return 0;
  } else {
  }
  int _sv0t1 = sv0_vec_get(a, limb);
  int _sv0t2 = (limb * 15);
  int _sv0t3 = (p - _sv0t2);
  int _sv0t4 = (_sv0t1 >> _sv0t3);
  int _sv0t5 = (_sv0t4 & 1);
  return _sv0t5;
}

static int is_dec_digit(int c) {
  if ((c < 48)) {
    return 0;
  } else {
  }
  if ((c > 57)) {
    return 0;
  } else {
  }
  return 1;
}

static int parse_f64_literal(const char* s, int out_digits, int out_meta) {
  int _sv0t0 = sv0_string_len(s);
  int n = _sv0t0;
  if ((n <= 0)) {
    int _sv0t1 = (0 - 1);
    return _sv0t1;
  } else {
  }
  int k = 0;
  int neg = 0;
  int _sv0t2 = sv0_string_char_at(s, 0);
  int c0 = _sv0t2;
  if ((c0 == 45)) {
    neg = 1;
    k = 1;
  } else {
    if ((c0 == 43)) {
      k = 1;
    } else {
    }
  }
  int _sv0t3 = sv0_vec_new();
  int digs = _sv0t3;
  int frac_len = 0;
  int saw_digit = 0;
  int saw_dot = 0;
  int exp10 = 0;
  int done = 0;
  while ((k < n)) {
    if ((done == 1)) {
      int _sv0t4 = (0 - 1);
      return _sv0t4;
    } else {
    }
    int _sv0t5 = sv0_string_char_at(s, k);
    int c = _sv0t5;
    int _sv0t6 = is_dec_digit(c);
    if (_sv0t6) {
      int _sv0t7 = (c - 48);
      sv0_vec_push(digs, _sv0t7);
      saw_digit = 1;
      if ((saw_dot == 1)) {
        frac_len = (frac_len + 1);
      } else {
      }
      k = (k + 1);
    } else {
      if ((c == 46)) {
        if ((saw_dot == 1)) {
          int _sv0t8 = (0 - 1);
          return _sv0t8;
        } else {
        }
        saw_dot = 1;
        k = (k + 1);
      } else {
        int _sv0t9 = (c == 101);
        int _sv0t10 = (c == 69);
        if ((_sv0t9 || _sv0t10)) {
          k = (k + 1);
          int esign = 1;
          if ((k < n)) {
            int _sv0t11 = sv0_string_char_at(s, k);
            int ec = _sv0t11;
            if ((ec == 45)) {
              esign = (0 - 1);
              k = (k + 1);
            } else {
              if ((ec == 43)) {
                k = (k + 1);
              } else {
              }
            }
          } else {
          }
          int edig = 0;
          int eany = 0;
          while ((k < n)) {
            int _sv0t12 = sv0_string_char_at(s, k);
            int d = _sv0t12;
            int _sv0t13 = is_dec_digit(d);
            if ((_sv0t13 != 1)) {
              int _sv0t14 = (0 - 1);
              return _sv0t14;
            } else {
            }
            int _sv0t15 = (edig * 10);
            int _sv0t16 = (d - 48);
            edig = (_sv0t15 + _sv0t16);
            eany = 1;
            k = (k + 1);
          }
          if ((eany == 0)) {
            int _sv0t17 = (0 - 1);
            return _sv0t17;
          } else {
          }
          exp10 = (esign * edig);
          done = 1;
        } else {
          int _sv0t18 = (0 - 1);
          return _sv0t18;
        }
      }
    }
  }
  if ((saw_digit == 0)) {
    int _sv0t19 = (0 - 1);
    return _sv0t19;
  } else {
  }
  int _sv0t20 = sv0_vec_len(digs);
  int dn = _sv0t20;
  int lead = 0;
  while (1) {
    int _sv0t21 = (dn - 1);
    int _sv0t23 = (lead < _sv0t21);
    if ((!_sv0t23)) {
      break;
    } else {
    }
    int _sv0t22 = sv0_vec_get(digs, lead);
    if ((_sv0t22 != 0)) {
      lead = (dn + 1);
    } else {
      lead = (lead + 1);
    }
  }
  if ((lead > dn)) {
    int _sv0t24 = (lead - dn);
    lead = (_sv0t24 - 1);
  } else {
  }
  int z = lead;
  while ((z < dn)) {
    int _sv0t25 = sv0_vec_get(digs, z);
    sv0_vec_push(out_digits, _sv0t25);
    z = (z + 1);
  }
  sv0_vec_push(out_meta, neg);
  int _sv0t26 = (exp10 - frac_len);
  sv0_vec_push(out_meta, _sv0t26);
  return 0;
}

static int emit_value(Value v, int env_names, int env_bases, int env_widths, int env_field_starts, int env_fields_flat, int pool, const char* source, int starts, int ends, int out) {
  int _sv0t0;
  if ((v.tag == 0)) {
    int n = v.p0;
    sv0_vec_push(out, 4);
    sv0_vec_push(out, n);
    return 2;
    _sv0t0 = 0;
  } else {
    if ((v.tag == 1)) {
      sv0_vec_push(out, 7);
      sv0_vec_push(out, 1);
      return 2;
      _sv0t0 = 0;
    } else {
      if ((v.tag == 2)) {
        sv0_vec_push(out, 7);
        sv0_vec_push(out, 0);
        return 2;
        _sv0t0 = 0;
      } else {
        if ((v.tag == 5)) {
          int s = v.p0;
          int _sv0t28 = pool_add(pool, s, source, starts, ends);
          int idx = _sv0t28;
          sv0_vec_push(out, 8);
          sv0_vec_push(out, idx);
          return 2;
          _sv0t0 = 0;
        } else {
          if ((v.tag == 4)) {
            sv0_vec_push(out, 3);
            return 1;
            _sv0t0 = 0;
          } else {
            if ((v.tag == 3)) {
              int x = v.p0;
              int _sv0t22 = lookup_slot(env_names, env_bases, env_widths, x, source, starts, ends);
              int si = _sv0t22;
              if ((si < 0)) {
                int _sv0t23 = (0 - 1);
                return _sv0t23;
              } else {
              }
              int _sv0t24 = slot_base(env_bases, si);
              int base = _sv0t24;
              int _sv0t25 = slot_width(env_widths, si);
              int width = _sv0t25;
              int k = 0;
              while ((k < width)) {
                sv0_vec_push(out, 96);
                int _sv0t26 = (base + k);
                sv0_vec_push(out, _sv0t26);
                k = (k + 1);
              }
              int _sv0t27 = (width * 2);
              return _sv0t27;
              _sv0t0 = 0;
            } else {
              if ((v.tag == 6)) {
                int bv = v.p0;
                int f = v.p1;
                Value _sv0t6;
                int _sv0t7 = sv0_box_load(bv, 0);
                _sv0t6.tag = _sv0t7;
                int _sv0t8 = sv0_box_load(bv, 1);
                _sv0t6.p0 = _sv0t8;
                int _sv0t9 = sv0_box_load(bv, 2);
                _sv0t6.p1 = _sv0t9;
                Value base_v;
                base_v.tag = (_sv0t6).tag;
                base_v.p0 = (_sv0t6).p0;
                base_v.p1 = (_sv0t6).p1;
                int _sv0t10;
                if ((base_v.tag == 3)) {
                  int x = base_v.p0;
                  int _sv0t20 = emit_member_load_from_var(x, f, env_names, env_bases, env_widths, env_field_starts, env_fields_flat, source, starts, ends, out);
                  return _sv0t20;
                  _sv0t10 = 0;
                } else {
                  if ((base_v.tag == 0)) {
                    int n = base_v.p0;
                    int _sv0t19 = (0 - 1);
                    return _sv0t19;
                    _sv0t10 = 0;
                  } else {
                    if ((base_v.tag == 1)) {
                      int _sv0t18 = (0 - 1);
                      return _sv0t18;
                      _sv0t10 = 0;
                    } else {
                      if ((base_v.tag == 2)) {
                        int _sv0t17 = (0 - 1);
                        return _sv0t17;
                        _sv0t10 = 0;
                      } else {
                        if ((base_v.tag == 5)) {
                          int s = base_v.p0;
                          int _sv0t16 = (0 - 1);
                          return _sv0t16;
                          _sv0t10 = 0;
                        } else {
                          if ((base_v.tag == 4)) {
                            int _sv0t15 = (0 - 1);
                            return _sv0t15;
                            _sv0t10 = 0;
                          } else {
                            if ((base_v.tag == 6)) {
                              int bv2 = base_v.p0;
                              int f2 = base_v.p1;
                              int _sv0t14 = (0 - 1);
                              return _sv0t14;
                              _sv0t10 = 0;
                            } else {
                              if ((base_v.tag == 7)) {
                                int h2 = base_v.p0;
                                int _sv0t13 = (0 - 1);
                                return _sv0t13;
                                _sv0t10 = 0;
                              } else {
                                if ((base_v.tag == 8)) {
                                  int h2 = base_v.p0;
                                  int _sv0t12 = (0 - 1);
                                  return _sv0t12;
                                  _sv0t10 = 0;
                                } else {
                                  if ((base_v.tag == 9)) {
                                    int h2 = base_v.p0;
                                    int _sv0t11 = (0 - 1);
                                    return _sv0t11;
                                    _sv0t10 = 0;
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
                int _sv0t21 = (0 - 1);
                return _sv0t21;
                _sv0t0 = 0;
              } else {
                if ((v.tag == 7)) {
                  int x = v.p0;
                  int _sv0t3 = lookup_slot(env_names, env_bases, env_widths, x, source, starts, ends);
                  int si = _sv0t3;
                  if ((si < 0)) {
                    int _sv0t4 = (0 - 1);
                    return _sv0t4;
                  } else {
                  }
                  int _sv0t5 = slot_base(env_bases, si);
                  int base = _sv0t5;
                  sv0_vec_push(out, 4);
                  sv0_vec_push(out, base);
                  return 2;
                  _sv0t0 = 0;
                } else {
                  if ((v.tag == 8)) {
                    int h = v.p0;
                    int _sv0t2 = (0 - 1);
                    return _sv0t2;
                    _sv0t0 = 0;
                  } else {
                    if ((v.tag == 9)) {
                      int h = v.p0;
                      int _sv0t1 = push_wide_int_literal(h, source, starts, ends, out);
                      return _sv0t1;
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
  int _sv0t29 = (0 - 1);
  return _sv0t29;
}

static int emit_expr(Expr e, int env_names, int env_bases, int env_widths, int env_cats, int env_field_starts, int env_fields_flat, int pool, const char* source, int starts, int ends, int out) {
  int _sv0t0;
  if ((e.tag == 0)) {
    int bv = e.p0;
    Value _sv0t51;
    int _sv0t52 = sv0_box_load(bv, 0);
    _sv0t51.tag = _sv0t52;
    int _sv0t53 = sv0_box_load(bv, 1);
    _sv0t51.p0 = _sv0t53;
    int _sv0t54 = sv0_box_load(bv, 2);
    _sv0t51.p1 = _sv0t54;
    Value v;
    v.tag = (_sv0t51).tag;
    v.p0 = (_sv0t51).p0;
    v.p1 = (_sv0t51).p1;
    int _sv0t55 = emit_value(v, env_names, env_bases, env_widths, env_field_starts, env_fields_flat, pool, source, starts, ends, out);
    return _sv0t55;
    _sv0t0 = 0;
  } else {
    if ((e.tag == 1)) {
      int x = e.p0;
      int _sv0t45 = lookup_slot(env_names, env_bases, env_widths, x, source, starts, ends);
      int si = _sv0t45;
      if ((si < 0)) {
        int _sv0t46 = (0 - 1);
        return _sv0t46;
      } else {
      }
      int _sv0t47 = slot_base(env_bases, si);
      int base = _sv0t47;
      int _sv0t48 = slot_width(env_widths, si);
      int width = _sv0t48;
      int k = 0;
      while ((k < width)) {
        sv0_vec_push(out, 96);
        int _sv0t49 = (base + k);
        sv0_vec_push(out, _sv0t49);
        k = (k + 1);
      }
      int _sv0t50 = (width * 2);
      return _sv0t50;
      _sv0t0 = 0;
    } else {
      if ((e.tag == 2)) {
        int op_h = e.p0;
        int bv1 = e.p1;
        int bv2 = e.p2;
        Value _sv0t26;
        int _sv0t27 = sv0_box_load(bv1, 0);
        _sv0t26.tag = _sv0t27;
        int _sv0t28 = sv0_box_load(bv1, 1);
        _sv0t26.p0 = _sv0t28;
        int _sv0t29 = sv0_box_load(bv1, 2);
        _sv0t26.p1 = _sv0t29;
        Value v1;
        v1.tag = (_sv0t26).tag;
        v1.p0 = (_sv0t26).p0;
        v1.p1 = (_sv0t26).p1;
        Value _sv0t30;
        int _sv0t31 = sv0_box_load(bv2, 0);
        _sv0t30.tag = _sv0t31;
        int _sv0t32 = sv0_box_load(bv2, 1);
        _sv0t30.p0 = _sv0t32;
        int _sv0t33 = sv0_box_load(bv2, 2);
        _sv0t30.p1 = _sv0t33;
        Value v2;
        v2.tag = (_sv0t30).tag;
        v2.p0 = (_sv0t30).p0;
        v2.p1 = (_sv0t30).p1;
        int _sv0t34 = operand_cat(v1, env_names, env_bases, env_widths, env_cats, source, starts, ends);
        int c1 = _sv0t34;
        int _sv0t35 = operand_cat(v2, env_names, env_bases, env_widths, env_cats, source, starts, ends);
        int c2 = _sv0t35;
        int _sv0t36 = emit_value(v1, env_names, env_bases, env_widths, env_field_starts, env_fields_flat, pool, source, starts, ends, out);
        int n1 = _sv0t36;
        if ((n1 < 0)) {
          int _sv0t37 = (0 - 1);
          return _sv0t37;
        } else {
        }
        int _sv0t38 = emit_value(v2, env_names, env_bases, env_widths, env_field_starts, env_fields_flat, pool, source, starts, ends, out);
        int n2 = _sv0t38;
        if ((n2 < 0)) {
          int _sv0t39 = (0 - 1);
          return _sv0t39;
        } else {
        }
        int _sv0t40 = combine_cat(c1, c2);
        int _sv0t41 = binop_tag_to_insn_typed(op_h, _sv0t40);
        int opc = _sv0t41;
        if ((opc < 0)) {
          int _sv0t42 = (0 - 1);
          return _sv0t42;
        } else {
        }
        sv0_vec_push(out, opc);
        int _sv0t43 = (n1 + n2);
        int _sv0t44 = (_sv0t43 + 1);
        return _sv0t44;
        _sv0t0 = 0;
      } else {
        if ((e.tag == 3)) {
          int op_h = e.p0;
          int bv = e.p1;
          Value _sv0t17;
          int _sv0t18 = sv0_box_load(bv, 0);
          _sv0t17.tag = _sv0t18;
          int _sv0t19 = sv0_box_load(bv, 1);
          _sv0t17.p0 = _sv0t19;
          int _sv0t20 = sv0_box_load(bv, 2);
          _sv0t17.p1 = _sv0t20;
          Value v;
          v.tag = (_sv0t17).tag;
          v.p0 = (_sv0t17).p0;
          v.p1 = (_sv0t17).p1;
          int _sv0t21 = operand_cat(v, env_names, env_bases, env_widths, env_cats, source, starts, ends);
          int c = _sv0t21;
          int _sv0t22 = emit_value(v, env_names, env_bases, env_widths, env_field_starts, env_fields_flat, pool, source, starts, ends, out);
          int n = _sv0t22;
          if ((n < 0)) {
            int _sv0t23 = (0 - 1);
            return _sv0t23;
          } else {
          }
          int _sv0t24 = unop_tag_to_insn_typed(op_h, c);
          int opc = _sv0t24;
          if ((opc >= 0)) {
            sv0_vec_push(out, opc);
            int _sv0t25 = (n + 1);
            return _sv0t25;
          } else {
          }
          return n;
          _sv0t0 = 0;
        } else {
          if ((e.tag == 4)) {
            int bv = e.p0;
            int f = e.p1;
            Value _sv0t1;
            int _sv0t2 = sv0_box_load(bv, 0);
            _sv0t1.tag = _sv0t2;
            int _sv0t3 = sv0_box_load(bv, 1);
            _sv0t1.p0 = _sv0t3;
            int _sv0t4 = sv0_box_load(bv, 2);
            _sv0t1.p1 = _sv0t4;
            Value base_v;
            base_v.tag = (_sv0t1).tag;
            base_v.p0 = (_sv0t1).p0;
            base_v.p1 = (_sv0t1).p1;
            int _sv0t5;
            if ((base_v.tag == 3)) {
              int x = base_v.p0;
              int _sv0t15 = emit_member_load_from_var(x, f, env_names, env_bases, env_widths, env_field_starts, env_fields_flat, source, starts, ends, out);
              return _sv0t15;
              _sv0t5 = 0;
            } else {
              if ((base_v.tag == 0)) {
                int n = base_v.p0;
                int _sv0t14 = (0 - 1);
                return _sv0t14;
                _sv0t5 = 0;
              } else {
                if ((base_v.tag == 1)) {
                  int _sv0t13 = (0 - 1);
                  return _sv0t13;
                  _sv0t5 = 0;
                } else {
                  if ((base_v.tag == 2)) {
                    int _sv0t12 = (0 - 1);
                    return _sv0t12;
                    _sv0t5 = 0;
                  } else {
                    if ((base_v.tag == 5)) {
                      int s = base_v.p0;
                      int _sv0t11 = (0 - 1);
                      return _sv0t11;
                      _sv0t5 = 0;
                    } else {
                      if ((base_v.tag == 4)) {
                        int _sv0t10 = (0 - 1);
                        return _sv0t10;
                        _sv0t5 = 0;
                      } else {
                        if ((base_v.tag == 6)) {
                          int bv2 = base_v.p0;
                          int f2 = base_v.p1;
                          int _sv0t9 = (0 - 1);
                          return _sv0t9;
                          _sv0t5 = 0;
                        } else {
                          if ((base_v.tag == 7)) {
                            int h2 = base_v.p0;
                            int _sv0t8 = (0 - 1);
                            return _sv0t8;
                            _sv0t5 = 0;
                          } else {
                            if ((base_v.tag == 8)) {
                              int h2 = base_v.p0;
                              int _sv0t7 = (0 - 1);
                              return _sv0t7;
                              _sv0t5 = 0;
                            } else {
                              if ((base_v.tag == 9)) {
                                int h2 = base_v.p0;
                                int _sv0t6 = (0 - 1);
                                return _sv0t6;
                                _sv0t5 = 0;
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
            int _sv0t16 = (0 - 1);
            return _sv0t16;
            _sv0t0 = 0;
          } else {
          }
        }
      }
    }
  }
  int _sv0t56 = (0 - 1);
  return _sv0t56;
}

static int vec_append(int dst, int src) {
  int _sv0t0 = sv0_vec_len(src);
  int n = _sv0t0;
  int i = 0;
  while ((i < n)) {
    int _sv0t1 = sv0_vec_get(src, i);
    sv0_vec_push(dst, _sv0t1);
    i = (i + 1);
  }
  return n;
}

static int flat_byte_size(int instrs) {
  int _sv0t0 = sv0_vec_len(instrs);
  int n = _sv0t0;
  int total = 0;
  int i = 0;
  while ((i < n)) {
    int _sv0t1 = sv0_vec_get(instrs, i);
    int opc = _sv0t1;
    int _sv0t2 = insn_size(opc);
    total = (total + _sv0t2);
    int _sv0t3 = insn_vec_stride(opc);
    i = (i + _sv0t3);
  }
  return total;
}

static int emit_ret_width(Value v, int env_names, int env_bases, int env_widths, const char* source, int starts, int ends) {
  int _sv0t0;
  if ((v.tag == 3)) {
    int x = v.p0;
    int _sv0t1 = lookup_slot(env_names, env_bases, env_widths, x, source, starts, ends);
    int si = _sv0t1;
    if ((si >= 0)) {
      int _sv0t2 = slot_width(env_widths, si);
      return _sv0t2;
    } else {
    }
    return 1;
    _sv0t0 = 0;
  } else {
    if ((v.tag == 0)) {
      int n = v.p0;
      return 1;
      _sv0t0 = 0;
    } else {
      if ((v.tag == 1)) {
        return 1;
        _sv0t0 = 0;
      } else {
        if ((v.tag == 2)) {
          return 1;
          _sv0t0 = 0;
        } else {
          if ((v.tag == 5)) {
            int s = v.p0;
            return 1;
            _sv0t0 = 0;
          } else {
            if ((v.tag == 4)) {
              return 1;
              _sv0t0 = 0;
            } else {
              if ((v.tag == 6)) {
                int bv = v.p0;
                int f = v.p1;
                return 1;
                _sv0t0 = 0;
              } else {
                if ((v.tag == 7)) {
                  int h = v.p0;
                  return 1;
                  _sv0t0 = 0;
                } else {
                  if ((v.tag == 8)) {
                    int h = v.p0;
                    return 1;
                    _sv0t0 = 0;
                  } else {
                    if ((v.tag == 9)) {
                      int h = v.p0;
                      return 1;
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
  return 1;
}

static int emit_box_new(int dst_h, int args, int env_names, int env_bases, int env_widths, int env_field_starts, int env_fields_flat, int pool, const char* source, int starts, int ends, int out) {
  int _sv0t0 = sv0_vec_len(args);
  if ((_sv0t0 < 1)) {
    int _sv0t1 = (0 - 1);
    return _sv0t1;
  } else {
  }
  int _sv0t2 = sv0_vec_get(args, 0);
  Value _sv0t3;
  int _sv0t4 = sv0_box_load(_sv0t2, 0);
  _sv0t3.tag = _sv0t4;
  int _sv0t5 = sv0_box_load(_sv0t2, 1);
  _sv0t3.p0 = _sv0t5;
  int _sv0t6 = sv0_box_load(_sv0t2, 2);
  _sv0t3.p1 = _sv0t6;
  Value inner_v;
  inner_v.tag = (_sv0t3).tag;
  inner_v.p0 = (_sv0t3).p0;
  inner_v.p1 = (_sv0t3).p1;
  int _sv0t7 = lookup_slot(env_names, env_bases, env_widths, dst_h, source, starts, ends);
  int dsi = _sv0t7;
  if ((dsi < 0)) {
    int _sv0t8 = (0 - 1);
    return _sv0t8;
  } else {
  }
  int _sv0t9 = slot_base(env_bases, dsi);
  int base_d = _sv0t9;
  int is_var = 0;
  int base_in = 0;
  int w = 1;
  int _sv0t10;
  if ((inner_v.tag == 3)) {
    int x = inner_v.p0;
    int _sv0t11 = lookup_slot(env_names, env_bases, env_widths, x, source, starts, ends);
    int si = _sv0t11;
    if ((si < 0)) {
      int _sv0t12 = (0 - 1);
      return _sv0t12;
    } else {
    }
    int _sv0t13 = slot_base(env_bases, si);
    base_in = _sv0t13;
    int _sv0t14 = slot_width(env_widths, si);
    w = _sv0t14;
    is_var = 1;
    _sv0t10 = 0;
  } else {
    if ((inner_v.tag == 0)) {
      int n = inner_v.p0;
      w = 1;
      _sv0t10 = 0;
    } else {
      if ((inner_v.tag == 1)) {
        w = 1;
        _sv0t10 = 0;
      } else {
        if ((inner_v.tag == 2)) {
          w = 1;
          _sv0t10 = 0;
        } else {
          if ((inner_v.tag == 5)) {
            int s = inner_v.p0;
            w = 1;
            _sv0t10 = 0;
          } else {
            if ((inner_v.tag == 4)) {
              w = 1;
              _sv0t10 = 0;
            } else {
              if ((inner_v.tag == 6)) {
                int bv = inner_v.p0;
                int f = inner_v.p1;
                w = 1;
                _sv0t10 = 0;
              } else {
                if ((inner_v.tag == 7)) {
                  int h2 = inner_v.p0;
                  w = 1;
                  _sv0t10 = 0;
                } else {
                  if ((inner_v.tag == 8)) {
                    int h2 = inner_v.p0;
                    w = 1;
                    _sv0t10 = 0;
                  } else {
                    if ((inner_v.tag == 9)) {
                      int h2 = inner_v.p0;
                      w = 1;
                      _sv0t10 = 0;
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
  sv0_vec_push(out, 4);
  sv0_vec_push(out, w);
  sv0_vec_push(out, 117);
  sv0_vec_push(out, 12);
  sv0_vec_push(out, 97);
  sv0_vec_push(out, base_d);
  int total = 6;
  if ((is_var == 1)) {
    int k = 0;
    while ((k < w)) {
      sv0_vec_push(out, 96);
      sv0_vec_push(out, base_d);
      sv0_vec_push(out, 4);
      sv0_vec_push(out, k);
      sv0_vec_push(out, 96);
      int _sv0t15 = (base_in + k);
      sv0_vec_push(out, _sv0t15);
      sv0_vec_push(out, 117);
      sv0_vec_push(out, 13);
      total = (total + 8);
      k = (k + 1);
    }
  } else {
    sv0_vec_push(out, 96);
    sv0_vec_push(out, base_d);
    sv0_vec_push(out, 4);
    sv0_vec_push(out, 0);
    int _sv0t16 = emit_value(inner_v, env_names, env_bases, env_widths, env_field_starts, env_fields_flat, pool, source, starts, ends, out);
    int nv = _sv0t16;
    if ((nv < 0)) {
      int _sv0t17 = (0 - 1);
      return _sv0t17;
    } else {
    }
    sv0_vec_push(out, 117);
    sv0_vec_push(out, 13);
    int _sv0t18 = (total + 4);
    int _sv0t19 = (_sv0t18 + nv);
    total = (_sv0t19 + 2);
  }
  return total;
}

static int emit_instr(Instr ins, int env_names, int env_bases, int env_widths, int env_cats, int env_field_starts, int env_fields_flat, int pool, const char* source, int starts, int ends, int fn_names, int out) {
  int _sv0t0;
  if ((ins.tag == 0)) {
    return 0;
    _sv0t0 = 0;
  } else {
    if ((ins.tag == 1)) {
      int x = ins.p0;
      return 0;
      _sv0t0 = 0;
    } else {
      if ((ins.tag == 2)) {
        int cty = ins.p0;
        int x = ins.p1;
        return 0;
        _sv0t0 = 0;
      } else {
        if ((ins.tag == 3)) {
          int x = ins.p0;
          int be = ins.p1;
          Expr _sv0t151;
          int _sv0t152 = sv0_box_load(be, 0);
          _sv0t151.tag = _sv0t152;
          int _sv0t153 = sv0_box_load(be, 1);
          _sv0t151.p0 = _sv0t153;
          int _sv0t154 = sv0_box_load(be, 2);
          _sv0t151.p1 = _sv0t154;
          int _sv0t155 = sv0_box_load(be, 3);
          _sv0t151.p2 = _sv0t155;
          Expr e;
          e.tag = (_sv0t151).tag;
          e.p0 = (_sv0t151).p0;
          e.p1 = (_sv0t151).p1;
          e.p2 = (_sv0t151).p2;
          int _sv0t156 = emit_expr(e, env_names, env_bases, env_widths, env_cats, env_field_starts, env_fields_flat, pool, source, starts, ends, out);
          int n = _sv0t156;
          if ((n < 0)) {
            int _sv0t157 = (0 - 1);
            return _sv0t157;
          } else {
          }
          int _sv0t158 = lookup_slot(env_names, env_bases, env_widths, x, source, starts, ends);
          int si = _sv0t158;
          if ((si < 0)) {
            int _sv0t159 = (0 - 1);
            return _sv0t159;
          } else {
          }
          int _sv0t160 = slot_base(env_bases, si);
          int base = _sv0t160;
          int _sv0t161 = slot_width(env_widths, si);
          int width = _sv0t161;
          int k = 0;
          while ((k < width)) {
            sv0_vec_push(out, 97);
            int _sv0t162 = (base + width);
            int _sv0t163 = (_sv0t162 - 1);
            int _sv0t164 = (_sv0t163 - k);
            sv0_vec_push(out, _sv0t164);
            k = (k + 1);
          }
          int _sv0t165 = (width * 2);
          int _sv0t166 = (n + _sv0t165);
          return _sv0t166;
          _sv0t0 = 0;
        } else {
          if ((ins.tag == 4)) {
            int x = ins.p0;
            int be = ins.p1;
            Expr _sv0t135;
            int _sv0t136 = sv0_box_load(be, 0);
            _sv0t135.tag = _sv0t136;
            int _sv0t137 = sv0_box_load(be, 1);
            _sv0t135.p0 = _sv0t137;
            int _sv0t138 = sv0_box_load(be, 2);
            _sv0t135.p1 = _sv0t138;
            int _sv0t139 = sv0_box_load(be, 3);
            _sv0t135.p2 = _sv0t139;
            Expr e;
            e.tag = (_sv0t135).tag;
            e.p0 = (_sv0t135).p0;
            e.p1 = (_sv0t135).p1;
            e.p2 = (_sv0t135).p2;
            int _sv0t140 = emit_expr(e, env_names, env_bases, env_widths, env_cats, env_field_starts, env_fields_flat, pool, source, starts, ends, out);
            int n = _sv0t140;
            if ((n < 0)) {
              int _sv0t141 = (0 - 1);
              return _sv0t141;
            } else {
            }
            int _sv0t142 = lookup_slot(env_names, env_bases, env_widths, x, source, starts, ends);
            int si = _sv0t142;
            if ((si < 0)) {
              int _sv0t143 = (0 - 1);
              return _sv0t143;
            } else {
            }
            int _sv0t144 = slot_base(env_bases, si);
            int base = _sv0t144;
            int _sv0t145 = slot_width(env_widths, si);
            int width = _sv0t145;
            int k = 0;
            while ((k < width)) {
              sv0_vec_push(out, 97);
              int _sv0t146 = (base + width);
              int _sv0t147 = (_sv0t146 - 1);
              int _sv0t148 = (_sv0t147 - k);
              sv0_vec_push(out, _sv0t148);
              k = (k + 1);
            }
            int _sv0t149 = (width * 2);
            int _sv0t150 = (n + _sv0t149);
            return _sv0t150;
            _sv0t0 = 0;
          } else {
            if ((ins.tag == 5)) {
              int bh = ins.p0;
              int fh = ins.p1;
              int be = ins.p2;
              Expr _sv0t117;
              int _sv0t118 = sv0_box_load(be, 0);
              _sv0t117.tag = _sv0t118;
              int _sv0t119 = sv0_box_load(be, 1);
              _sv0t117.p0 = _sv0t119;
              int _sv0t120 = sv0_box_load(be, 2);
              _sv0t117.p1 = _sv0t120;
              int _sv0t121 = sv0_box_load(be, 3);
              _sv0t117.p2 = _sv0t121;
              Expr e;
              e.tag = (_sv0t117).tag;
              e.p0 = (_sv0t117).p0;
              e.p1 = (_sv0t117).p1;
              e.p2 = (_sv0t117).p2;
              int _sv0t122 = emit_expr(e, env_names, env_bases, env_widths, env_cats, env_field_starts, env_fields_flat, pool, source, starts, ends, out);
              int ne = _sv0t122;
              if ((ne < 0)) {
                int _sv0t123 = (0 - 1);
                return _sv0t123;
              } else {
              }
              int _sv0t124 = lookup_slot(env_names, env_bases, env_widths, bh, source, starts, ends);
              int si = _sv0t124;
              if ((si < 0)) {
                int _sv0t125 = (0 - 1);
                return _sv0t125;
              } else {
              }
              int _sv0t126 = slot_base(env_bases, si);
              int b0 = _sv0t126;
              int _sv0t127 = slot_width(env_widths, si);
              int w = _sv0t127;
              int _sv0t128 = member_offset_from_field(w, fh);
              int off = _sv0t128;
              if ((fh >= 0)) {
                int _sv0t129 = sv0_vec_len(env_field_starts);
                if ((si < _sv0t129)) {
                  int _sv0t130 = sv0_vec_get(env_field_starts, si);
                  int fs = _sv0t130;
                  int _sv0t131 = index_of_field_range(env_fields_flat, fs, w, fh, source, starts, ends);
                  off = _sv0t131;
                } else {
                }
              } else {
              }
              if ((off < 0)) {
                int _sv0t132 = (0 - 1);
                return _sv0t132;
              } else {
              }
              sv0_vec_push(out, 97);
              int _sv0t133 = (b0 + off);
              sv0_vec_push(out, _sv0t133);
              int _sv0t134 = (ne + 2);
              return _sv0t134;
              _sv0t0 = 0;
            } else {
              if ((ins.tag == 6)) {
                int be = ins.p0;
                int th = ins.p1;
                int el = ins.p2;
                Expr _sv0t95;
                int _sv0t96 = sv0_box_load(be, 0);
                _sv0t95.tag = _sv0t96;
                int _sv0t97 = sv0_box_load(be, 1);
                _sv0t95.p0 = _sv0t97;
                int _sv0t98 = sv0_box_load(be, 2);
                _sv0t95.p1 = _sv0t98;
                int _sv0t99 = sv0_box_load(be, 3);
                _sv0t95.p2 = _sv0t99;
                Expr cond;
                cond.tag = (_sv0t95).tag;
                cond.p0 = (_sv0t95).p0;
                cond.p1 = (_sv0t95).p1;
                cond.p2 = (_sv0t95).p2;
                int _sv0t100 = emit_expr(cond, env_names, env_bases, env_widths, env_cats, env_field_starts, env_fields_flat, pool, source, starts, ends, out);
                int nc = _sv0t100;
                if ((nc < 0)) {
                  int _sv0t101 = (0 - 1);
                  return _sv0t101;
                } else {
                }
                int _sv0t102 = sv0_vec_new();
                int then_out = _sv0t102;
                int _sv0t103 = emit_instrs(th, env_names, env_bases, env_widths, env_cats, env_field_starts, env_fields_flat, pool, source, starts, ends, fn_names, then_out);
                int nt = _sv0t103;
                if ((nt < 0)) {
                  int _sv0t104 = (0 - 1);
                  return _sv0t104;
                } else {
                }
                int _sv0t105 = sv0_vec_new();
                int else_out = _sv0t105;
                int _sv0t106 = emit_instrs(el, env_names, env_bases, env_widths, env_cats, env_field_starts, env_fields_flat, pool, source, starts, ends, fn_names, else_out);
                int ne = _sv0t106;
                if ((ne < 0)) {
                  int _sv0t107 = (0 - 1);
                  return _sv0t107;
                } else {
                }
                int _sv0t108 = flat_byte_size(then_out);
                int _sv0t109 = insn_size(112);
                int off_else = (_sv0t108 + _sv0t109);
                int _sv0t110 = flat_byte_size(else_out);
                int off_end = _sv0t110;
                sv0_vec_push(out, 114);
                sv0_vec_push(out, off_else);
                int _sv0t111 = vec_append(out, then_out);
                int at = _sv0t111;
                sv0_vec_push(out, 112);
                sv0_vec_push(out, off_end);
                int _sv0t112 = vec_append(out, else_out);
                int ae = _sv0t112;
                int _sv0t113 = (nc + 2);
                int _sv0t114 = (_sv0t113 + at);
                int _sv0t115 = (_sv0t114 + 2);
                int _sv0t116 = (_sv0t115 + ae);
                return _sv0t116;
                _sv0t0 = 0;
              } else {
                if ((ins.tag == 7)) {
                  int be = ins.p0;
                  int body = ins.p1;
                  Expr _sv0t69;
                  int _sv0t70 = sv0_box_load(be, 0);
                  _sv0t69.tag = _sv0t70;
                  int _sv0t71 = sv0_box_load(be, 1);
                  _sv0t69.p0 = _sv0t71;
                  int _sv0t72 = sv0_box_load(be, 2);
                  _sv0t69.p1 = _sv0t72;
                  int _sv0t73 = sv0_box_load(be, 3);
                  _sv0t69.p2 = _sv0t73;
                  Expr cond;
                  cond.tag = (_sv0t69).tag;
                  cond.p0 = (_sv0t69).p0;
                  cond.p1 = (_sv0t69).p1;
                  cond.p2 = (_sv0t69).p2;
                  int _sv0t74 = sv0_vec_new();
                  int cond_out = _sv0t74;
                  int _sv0t75 = emit_expr(cond, env_names, env_bases, env_widths, env_cats, env_field_starts, env_fields_flat, pool, source, starts, ends, cond_out);
                  int nc = _sv0t75;
                  if ((nc < 0)) {
                    int _sv0t76 = (0 - 1);
                    return _sv0t76;
                  } else {
                  }
                  int _sv0t77 = prepool_loop_seq(body, 0, env_names, env_bases, env_widths, env_cats, env_field_starts, env_fields_flat, pool, source, starts, ends, fn_names);
                  int _sv0t78 = sv0_vec_new();
                  int body_out = _sv0t78;
                  int _sv0t79 = emit_instrs(body, env_names, env_bases, env_widths, env_cats, env_field_starts, env_fields_flat, pool, source, starts, ends, fn_names, body_out);
                  int nb = _sv0t79;
                  if ((nb < 0)) {
                    int _sv0t80 = (0 - 1);
                    return _sv0t80;
                  } else {
                  }
                  sv0_vec_push(body_out, 112);
                  int _sv0t81 = loop_exit_sentinel();
                  sv0_vec_push(body_out, _sv0t81);
                  int _sv0t82 = flat_byte_size(cond_out);
                  int cond_bytes = _sv0t82;
                  int _sv0t83 = insn_size(114);
                  int jif_bytes = _sv0t83;
                  int _sv0t84 = flat_byte_size(body_out);
                  int body_bytes = _sv0t84;
                  int forward = body_bytes;
                  int _sv0t85 = (cond_bytes + jif_bytes);
                  int _sv0t86 = (_sv0t85 + body_bytes);
                  int back = (0 - _sv0t86);
                  int _sv0t87 = replace_loop_exit_jump(body_out, back);
                  int _sv0t88 = sv0_vec_new();
                  int loop_out = _sv0t88;
                  int _sv0t89 = vec_append(loop_out, cond_out);
                  sv0_vec_push(loop_out, 114);
                  sv0_vec_push(loop_out, forward);
                  int _sv0t90 = vec_append(loop_out, body_out);
                  int _sv0t91 = flat_byte_size(loop_out);
                  int total_bytes = _sv0t91;
                  int _sv0t92 = patch_continue_jumps(loop_out, 0, 0);
                  int _sv0t93 = patch_break_jumps(loop_out, 0, total_bytes);
                  int _sv0t94 = vec_append(out, loop_out);
                  int added = _sv0t94;
                  return added;
                  _sv0t0 = 0;
                } else {
                  if ((ins.tag == 8)) {
                    int body = ins.p0;
                    int _sv0t68 = emit_instrs(body, env_names, env_bases, env_widths, env_cats, env_field_starts, env_fields_flat, pool, source, starts, ends, fn_names, out);
                    return _sv0t68;
                    _sv0t0 = 0;
                  } else {
                    if ((ins.tag == 9)) {
                      sv0_vec_push(out, 112);
                      int _sv0t67 = loop_break_sentinel();
                      sv0_vec_push(out, _sv0t67);
                      return 2;
                      _sv0t0 = 0;
                    } else {
                      if ((ins.tag == 10)) {
                        sv0_vec_push(out, 112);
                        int _sv0t66 = loop_continue_sentinel();
                        sv0_vec_push(out, _sv0t66);
                        return 2;
                        _sv0t0 = 0;
                      } else {
                        if ((ins.tag == 11)) {
                          int dst_h = ins.p0;
                          int fn_h = ins.p1;
                          int args = ins.p2;
                          int rt_h = ins.p3;
                          int bid = (0 - 1);
                          if ((fn_h < 0)) {
                            bid = (0 - fn_h);
                            if ((bid == 1)) {
                              bid = 0;
                            } else {
                            }
                            if ((bid == 13)) {
                              bid = 14;
                            } else {
                            }
                            if ((bid == 21)) {
                              bid = 14;
                            } else {
                            }
                          } else {
                            const char* _sv0t29 = handle_to_str(fn_h, source, starts, ends);
                            const char* fn_name;
                            fn_name = _sv0t29;
                            int _sv0t30 = builtin_id(fn_name);
                            bid = _sv0t30;
                          }
                          if ((bid == 12)) {
                            int _sv0t31 = emit_box_new(dst_h, args, env_names, env_bases, env_widths, env_field_starts, env_fields_flat, pool, source, starts, ends, out);
                            return _sv0t31;
                          } else {
                          }
                          int _sv0t32 = sv0_vec_len(args);
                          int nargs_vec = _sv0t32;
                          int total = 0;
                          if ((bid >= 0)) {
                            int ai = 0;
                            while ((ai < nargs_vec)) {
                              int _sv0t33 = sv0_vec_get(args, ai);
                              int bv = _sv0t33;
                              Value _sv0t34;
                              int _sv0t35 = sv0_box_load(bv, 0);
                              _sv0t34.tag = _sv0t35;
                              int _sv0t36 = sv0_box_load(bv, 1);
                              _sv0t34.p0 = _sv0t36;
                              int _sv0t37 = sv0_box_load(bv, 2);
                              _sv0t34.p1 = _sv0t37;
                              Value v;
                              v.tag = (_sv0t34).tag;
                              v.p0 = (_sv0t34).p0;
                              v.p1 = (_sv0t34).p1;
                              int _sv0t38 = emit_value(v, env_names, env_bases, env_widths, env_field_starts, env_fields_flat, pool, source, starts, ends, out);
                              int nv = _sv0t38;
                              if ((nv < 0)) {
                                int _sv0t39 = (0 - 1);
                                return _sv0t39;
                              } else {
                              }
                              total = (total + nv);
                              ai = (ai + 1);
                            }
                            sv0_vec_push(out, 117);
                            sv0_vec_push(out, bid);
                            total = (total + 2);
                            if ((dst_h != 0)) {
                              int _sv0t40 = lookup_slot(env_names, env_bases, env_widths, dst_h, source, starts, ends);
                              int si = _sv0t40;
                              if ((si < 0)) {
                                int _sv0t41 = (0 - 1);
                                return _sv0t41;
                              } else {
                              }
                              int _sv0t42 = slot_base(env_bases, si);
                              int base = _sv0t42;
                              int _sv0t43 = slot_width(env_widths, si);
                              int width = _sv0t43;
                              int k = 0;
                              while ((k < width)) {
                                sv0_vec_push(out, 97);
                                int _sv0t44 = (base + width);
                                int _sv0t45 = (_sv0t44 - 1);
                                int _sv0t46 = (_sv0t45 - k);
                                sv0_vec_push(out, _sv0t46);
                                k = (k + 1);
                              }
                              int _sv0t47 = (width * 2);
                              total = (total + _sv0t47);
                            } else {
                            }
                            return total;
                          } else {
                          }
                          int nargs_total = 0;
                          int ai = 0;
                          while ((ai < nargs_vec)) {
                            int _sv0t48 = sv0_vec_get(args, ai);
                            int bv = _sv0t48;
                            Value _sv0t49;
                            int _sv0t50 = sv0_box_load(bv, 0);
                            _sv0t49.tag = _sv0t50;
                            int _sv0t51 = sv0_box_load(bv, 1);
                            _sv0t49.p0 = _sv0t51;
                            int _sv0t52 = sv0_box_load(bv, 2);
                            _sv0t49.p1 = _sv0t52;
                            Value v;
                            v.tag = (_sv0t49).tag;
                            v.p0 = (_sv0t49).p0;
                            v.p1 = (_sv0t49).p1;
                            int _sv0t53 = emit_ret_width(v, env_names, env_bases, env_widths, source, starts, ends);
                            int w = _sv0t53;
                            nargs_total = (nargs_total + w);
                            int _sv0t54 = emit_value(v, env_names, env_bases, env_widths, env_field_starts, env_fields_flat, pool, source, starts, ends, out);
                            int nv = _sv0t54;
                            if ((nv < 0)) {
                              int _sv0t55 = (0 - 1);
                              return _sv0t55;
                            } else {
                            }
                            total = (total + nv);
                            ai = (ai + 1);
                          }
                          int _sv0t56 = index_of_fn(fn_h, fn_names, source, starts, ends);
                          int fix = _sv0t56;
                          if ((fix < 0)) {
                            int _sv0t57 = (0 - 1);
                            return _sv0t57;
                          } else {
                          }
                          sv0_vec_push(out, 115);
                          sv0_vec_push(out, fix);
                          sv0_vec_push(out, nargs_total);
                          total = (total + 3);
                          if ((dst_h != 0)) {
                            int _sv0t58 = lookup_slot(env_names, env_bases, env_widths, dst_h, source, starts, ends);
                            int si = _sv0t58;
                            if ((si < 0)) {
                              int _sv0t59 = (0 - 1);
                              return _sv0t59;
                            } else {
                            }
                            int _sv0t60 = slot_base(env_bases, si);
                            int base = _sv0t60;
                            int _sv0t61 = slot_width(env_widths, si);
                            int width = _sv0t61;
                            int k = 0;
                            while ((k < width)) {
                              sv0_vec_push(out, 97);
                              int _sv0t62 = (base + width);
                              int _sv0t63 = (_sv0t62 - 1);
                              int _sv0t64 = (_sv0t63 - k);
                              sv0_vec_push(out, _sv0t64);
                              k = (k + 1);
                            }
                            int _sv0t65 = (width * 2);
                            total = (total + _sv0t65);
                          } else {
                          }
                          return total;
                          _sv0t0 = 0;
                        } else {
                          if ((ins.tag == 12)) {
                            int be = ins.p0;
                            int fn_h = ins.p1;
                            Expr _sv0t19;
                            int _sv0t20 = sv0_box_load(be, 0);
                            _sv0t19.tag = _sv0t20;
                            int _sv0t21 = sv0_box_load(be, 1);
                            _sv0t19.p0 = _sv0t21;
                            int _sv0t22 = sv0_box_load(be, 2);
                            _sv0t19.p1 = _sv0t22;
                            int _sv0t23 = sv0_box_load(be, 3);
                            _sv0t19.p2 = _sv0t23;
                            Expr e;
                            e.tag = (_sv0t19).tag;
                            e.p0 = (_sv0t19).p0;
                            e.p1 = (_sv0t19).p1;
                            e.p2 = (_sv0t19).p2;
                            int _sv0t24 = emit_expr(e, env_names, env_bases, env_widths, env_cats, env_field_starts, env_fields_flat, pool, source, starts, ends, out);
                            int ne = _sv0t24;
                            if ((ne < 0)) {
                              int _sv0t25 = (0 - 1);
                              return _sv0t25;
                            } else {
                            }
                            int _sv0t26 = contract_pool_key(fn_h, 0);
                            int key = _sv0t26;
                            int _sv0t27 = pool_add(pool, key, source, starts, ends);
                            int pidx = _sv0t27;
                            sv0_vec_push(out, 160);
                            sv0_vec_push(out, pidx);
                            int _sv0t28 = (ne + 2);
                            return _sv0t28;
                            _sv0t0 = 0;
                          } else {
                            if ((ins.tag == 13)) {
                              int be = ins.p0;
                              int fn_h = ins.p1;
                              Expr _sv0t9;
                              int _sv0t10 = sv0_box_load(be, 0);
                              _sv0t9.tag = _sv0t10;
                              int _sv0t11 = sv0_box_load(be, 1);
                              _sv0t9.p0 = _sv0t11;
                              int _sv0t12 = sv0_box_load(be, 2);
                              _sv0t9.p1 = _sv0t12;
                              int _sv0t13 = sv0_box_load(be, 3);
                              _sv0t9.p2 = _sv0t13;
                              Expr e;
                              e.tag = (_sv0t9).tag;
                              e.p0 = (_sv0t9).p0;
                              e.p1 = (_sv0t9).p1;
                              e.p2 = (_sv0t9).p2;
                              int _sv0t14 = emit_expr(e, env_names, env_bases, env_widths, env_cats, env_field_starts, env_fields_flat, pool, source, starts, ends, out);
                              int ne = _sv0t14;
                              if ((ne < 0)) {
                                int _sv0t15 = (0 - 1);
                                return _sv0t15;
                              } else {
                              }
                              int _sv0t16 = contract_pool_key(fn_h, 1);
                              int key = _sv0t16;
                              int _sv0t17 = pool_add(pool, key, source, starts, ends);
                              int pidx = _sv0t17;
                              sv0_vec_push(out, 160);
                              sv0_vec_push(out, pidx);
                              int _sv0t18 = (ne + 2);
                              return _sv0t18;
                              _sv0t0 = 0;
                            } else {
                              if ((ins.tag == 14)) {
                                sv0_vec_push(out, 118);
                                sv0_vec_push(out, 0);
                                return 2;
                                _sv0t0 = 0;
                              } else {
                                if ((ins.tag == 15)) {
                                  int bv = ins.p0;
                                  Value _sv0t1;
                                  int _sv0t2 = sv0_box_load(bv, 0);
                                  _sv0t1.tag = _sv0t2;
                                  int _sv0t3 = sv0_box_load(bv, 1);
                                  _sv0t1.p0 = _sv0t3;
                                  int _sv0t4 = sv0_box_load(bv, 2);
                                  _sv0t1.p1 = _sv0t4;
                                  Value v;
                                  v.tag = (_sv0t1).tag;
                                  v.p0 = (_sv0t1).p0;
                                  v.p1 = (_sv0t1).p1;
                                  int _sv0t5 = emit_ret_width(v, env_names, env_bases, env_widths, source, starts, ends);
                                  int w = _sv0t5;
                                  int _sv0t6 = emit_value(v, env_names, env_bases, env_widths, env_field_starts, env_fields_flat, pool, source, starts, ends, out);
                                  int n = _sv0t6;
                                  if ((n < 0)) {
                                    int _sv0t7 = (0 - 1);
                                    return _sv0t7;
                                  } else {
                                  }
                                  sv0_vec_push(out, 118);
                                  sv0_vec_push(out, w);
                                  int _sv0t8 = (n + 2);
                                  return _sv0t8;
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
  int _sv0t167 = (0 - 1);
  return _sv0t167;
}

static int emit_instrs(int instrs, int env_names, int env_bases, int env_widths, int env_cats, int env_field_starts, int env_fields_flat, int pool, const char* source, int starts, int ends, int fn_names, int out) {
  int _sv0t0 = sv0_vec_len(instrs);
  int count = _sv0t0;
  int total = 0;
  int idx = 0;
  while ((idx < count)) {
    int _sv0t1 = sv0_vec_get(instrs, idx);
    int bh = _sv0t1;
    Instr _sv0t2;
    int _sv0t3 = sv0_box_load(bh, 0);
    _sv0t2.tag = _sv0t3;
    int _sv0t4 = sv0_box_load(bh, 1);
    _sv0t2.p0 = _sv0t4;
    int _sv0t5 = sv0_box_load(bh, 2);
    _sv0t2.p1 = _sv0t5;
    int _sv0t6 = sv0_box_load(bh, 3);
    _sv0t2.p2 = _sv0t6;
    int _sv0t7 = sv0_box_load(bh, 4);
    _sv0t2.p3 = _sv0t7;
    Instr ins;
    ins.tag = (_sv0t2).tag;
    ins.p0 = (_sv0t2).p0;
    ins.p1 = (_sv0t2).p1;
    ins.p2 = (_sv0t2).p2;
    ins.p3 = (_sv0t2).p3;
    int _sv0t8 = emit_instr(ins, env_names, env_bases, env_widths, env_cats, env_field_starts, env_fields_flat, pool, source, starts, ends, fn_names, out);
    int n = _sv0t8;
    if ((n < 0)) {
      int _sv0t9 = (0 - 1);
      return _sv0t9;
    } else {
    }
    total = (total + n);
    idx = (idx + 1);
  }
  return total;
}

static int prepool_loop_seq(int instrs, int i0, int env_names, int env_bases, int env_widths, int env_cats, int env_field_starts, int env_fields_flat, int pool, const char* source, int starts, int ends, int fn_names) {
  int _sv0t0 = sv0_vec_len(instrs);
  if ((i0 >= _sv0t0)) {
    return 0;
  } else {
  }
  int _sv0t1 = sv0_vec_get(instrs, i0);
  int bh = _sv0t1;
  Instr _sv0t2;
  int _sv0t3 = sv0_box_load(bh, 0);
  _sv0t2.tag = _sv0t3;
  int _sv0t4 = sv0_box_load(bh, 1);
  _sv0t2.p0 = _sv0t4;
  int _sv0t5 = sv0_box_load(bh, 2);
  _sv0t2.p1 = _sv0t5;
  int _sv0t6 = sv0_box_load(bh, 3);
  _sv0t2.p2 = _sv0t6;
  int _sv0t7 = sv0_box_load(bh, 4);
  _sv0t2.p3 = _sv0t7;
  Instr ins;
  ins.tag = (_sv0t2).tag;
  ins.p0 = (_sv0t2).p0;
  ins.p1 = (_sv0t2).p1;
  ins.p2 = (_sv0t2).p2;
  ins.p3 = (_sv0t2).p3;
  int _sv0t8;
  if ((ins.tag == 6)) {
    int be = ins.p0;
    int th = ins.p1;
    int el = ins.p2;
    int _sv0t26 = (i0 + 1);
    int _sv0t27 = prepool_loop_seq(instrs, _sv0t26, env_names, env_bases, env_widths, env_cats, env_field_starts, env_fields_flat, pool, source, starts, ends, fn_names);
    int _sv0t28 = prepool_loop_seq(el, 0, env_names, env_bases, env_widths, env_cats, env_field_starts, env_fields_flat, pool, source, starts, ends, fn_names);
    int _sv0t29 = prepool_loop_seq(th, 0, env_names, env_bases, env_widths, env_cats, env_field_starts, env_fields_flat, pool, source, starts, ends, fn_names);
    Expr _sv0t30;
    int _sv0t31 = sv0_box_load(be, 0);
    _sv0t30.tag = _sv0t31;
    int _sv0t32 = sv0_box_load(be, 1);
    _sv0t30.p0 = _sv0t32;
    int _sv0t33 = sv0_box_load(be, 2);
    _sv0t30.p1 = _sv0t33;
    int _sv0t34 = sv0_box_load(be, 3);
    _sv0t30.p2 = _sv0t34;
    Expr ce_if;
    ce_if.tag = (_sv0t30).tag;
    ce_if.p0 = (_sv0t30).p0;
    ce_if.p1 = (_sv0t30).p1;
    ce_if.p2 = (_sv0t30).p2;
    int _sv0t35 = sv0_vec_new();
    int cscratch = _sv0t35;
    int _sv0t36 = emit_expr(ce_if, env_names, env_bases, env_widths, env_cats, env_field_starts, env_fields_flat, pool, source, starts, ends, cscratch);
    return 0;
    _sv0t8 = 0;
  } else {
    if ((ins.tag == 7)) {
      int be = ins.p0;
      int body = ins.p1;
      Expr _sv0t16;
      int _sv0t17 = sv0_box_load(be, 0);
      _sv0t16.tag = _sv0t17;
      int _sv0t18 = sv0_box_load(be, 1);
      _sv0t16.p0 = _sv0t18;
      int _sv0t19 = sv0_box_load(be, 2);
      _sv0t16.p1 = _sv0t19;
      int _sv0t20 = sv0_box_load(be, 3);
      _sv0t16.p2 = _sv0t20;
      Expr ce_wh;
      ce_wh.tag = (_sv0t16).tag;
      ce_wh.p0 = (_sv0t16).p0;
      ce_wh.p1 = (_sv0t16).p1;
      ce_wh.p2 = (_sv0t16).p2;
      int _sv0t21 = sv0_vec_new();
      int cscratch = _sv0t21;
      int _sv0t22 = emit_expr(ce_wh, env_names, env_bases, env_widths, env_cats, env_field_starts, env_fields_flat, pool, source, starts, ends, cscratch);
      int _sv0t23 = prepool_loop_seq(body, 0, env_names, env_bases, env_widths, env_cats, env_field_starts, env_fields_flat, pool, source, starts, ends, fn_names);
      int _sv0t24 = (i0 + 1);
      int _sv0t25 = prepool_loop_seq(instrs, _sv0t24, env_names, env_bases, env_widths, env_cats, env_field_starts, env_fields_flat, pool, source, starts, ends, fn_names);
      return 0;
      _sv0t8 = 0;
    } else {
      if ((ins.tag == 8)) {
        int body = ins.p0;
        int _sv0t13 = prepool_loop_seq(body, 0, env_names, env_bases, env_widths, env_cats, env_field_starts, env_fields_flat, pool, source, starts, ends, fn_names);
        int _sv0t14 = (i0 + 1);
        int _sv0t15 = prepool_loop_seq(instrs, _sv0t14, env_names, env_bases, env_widths, env_cats, env_field_starts, env_fields_flat, pool, source, starts, ends, fn_names);
        return 0;
        _sv0t8 = 0;
      } else {
        if ((ins.tag == 10)) {
          return 0;
          _sv0t8 = 0;
        } else {
          if (1) {
            int _sv0t9 = sv0_vec_new();
            int scratch = _sv0t9;
            int _sv0t10 = emit_instr(ins, env_names, env_bases, env_widths, env_cats, env_field_starts, env_fields_flat, pool, source, starts, ends, fn_names, scratch);
            int _sv0t11 = (i0 + 1);
            int _sv0t12 = prepool_loop_seq(instrs, _sv0t11, env_names, env_bases, env_widths, env_cats, env_field_starts, env_fields_flat, pool, source, starts, ends, fn_names);
            return 0;
            _sv0t8 = 0;
          } else {
          }
        }
      }
    }
  }
  return 0;
}

static int ensure_var_int(int name, int env_names, int env_bases, int env_widths, int env_cats, int env_field_starts, int next_slot, const char* source, int starts, int ends) {
  int _sv0t0 = lookup_slot(env_names, env_bases, env_widths, name, source, starts, ends);
  int si = _sv0t0;
  if ((si >= 0)) {
    return next_slot;
  } else {
  }
  sv0_vec_push(env_names, name);
  sv0_vec_push(env_bases, next_slot);
  sv0_vec_push(env_widths, 1);
  sv0_vec_push(env_cats, 0);
  sv0_vec_push(env_field_starts, 0);
  int _sv0t1 = (next_slot + 1);
  return _sv0t1;
}

static int scan_instr_env(Instr ins, int env_names, int env_bases, int env_widths, int env_cats, int env_field_starts, int next_slot, int structs_names, int structs_field_counts, int enums_names, int enums_widths, int structs_fields_flat, int structs_field_starts, int env_fields_flat, const char* source, int starts, int ends) {
  int _sv0t0;
  if ((ins.tag == 1)) {
    int x = ins.p0;
    int _sv0t16 = ensure_var_int(x, env_names, env_bases, env_widths, env_cats, env_field_starts, next_slot, source, starts, ends);
    return _sv0t16;
    _sv0t0 = 0;
  } else {
    if ((ins.tag == 2)) {
      int cty = ins.p0;
      int x = ins.p1;
      int _sv0t12 = width_of_cty(cty, structs_names, structs_field_counts, enums_names, enums_widths, source, starts, ends);
      int w = _sv0t12;
      sv0_vec_push(env_names, x);
      sv0_vec_push(env_bases, next_slot);
      sv0_vec_push(env_widths, w);
      int _sv0t13 = cty_cat_of_handle(cty, source, starts, ends);
      sv0_vec_push(env_cats, _sv0t13);
      int _sv0t14 = record_field_layout(cty, structs_names, structs_field_counts, structs_fields_flat, structs_field_starts, source, starts, ends, env_fields_flat);
      sv0_vec_push(env_field_starts, _sv0t14);
      int _sv0t15 = (next_slot + w);
      return _sv0t15;
      _sv0t0 = 0;
    } else {
      if ((ins.tag == 3)) {
        int x = ins.p0;
        int be = ins.p1;
        int _sv0t11 = ensure_var_int(x, env_names, env_bases, env_widths, env_cats, env_field_starts, next_slot, source, starts, ends);
        return _sv0t11;
        _sv0t0 = 0;
      } else {
        if ((ins.tag == 11)) {
          int dst = ins.p0;
          int fn_h = ins.p1;
          int args = ins.p2;
          int rty = ins.p3;
          if ((dst == 0)) {
            return next_slot;
          } else {
          }
          int _sv0t5 = width_of_cty(rty, structs_names, structs_field_counts, enums_names, enums_widths, source, starts, ends);
          int w0 = _sv0t5;
          int _sv0t6;
          int _sv0t7;
          if ((w0 < 1)) {
            _sv0t7 = 1;
          } else {
            _sv0t7 = w0;
          }
          _sv0t6 = _sv0t7;
          int w = _sv0t6;
          sv0_vec_push(env_names, dst);
          sv0_vec_push(env_bases, next_slot);
          sv0_vec_push(env_widths, w);
          int _sv0t8 = cty_cat_of_handle(rty, source, starts, ends);
          sv0_vec_push(env_cats, _sv0t8);
          int _sv0t9 = record_field_layout(rty, structs_names, structs_field_counts, structs_fields_flat, structs_field_starts, source, starts, ends, env_fields_flat);
          sv0_vec_push(env_field_starts, _sv0t9);
          int _sv0t10 = (next_slot + w);
          return _sv0t10;
          _sv0t0 = 0;
        } else {
          if ((ins.tag == 6)) {
            int be = ins.p0;
            int th = ins.p1;
            int el = ins.p2;
            int _sv0t3 = scan_instrs_env(th, env_names, env_bases, env_widths, env_cats, env_field_starts, next_slot, structs_names, structs_field_counts, enums_names, enums_widths, structs_fields_flat, structs_field_starts, env_fields_flat, source, starts, ends);
            int ns1 = _sv0t3;
            int _sv0t4 = scan_instrs_env(el, env_names, env_bases, env_widths, env_cats, env_field_starts, ns1, structs_names, structs_field_counts, enums_names, enums_widths, structs_fields_flat, structs_field_starts, env_fields_flat, source, starts, ends);
            return _sv0t4;
            _sv0t0 = 0;
          } else {
            if ((ins.tag == 7)) {
              int be = ins.p0;
              int body = ins.p1;
              int _sv0t2 = scan_instrs_env(body, env_names, env_bases, env_widths, env_cats, env_field_starts, next_slot, structs_names, structs_field_counts, enums_names, enums_widths, structs_fields_flat, structs_field_starts, env_fields_flat, source, starts, ends);
              return _sv0t2;
              _sv0t0 = 0;
            } else {
              if ((ins.tag == 8)) {
                int body = ins.p0;
                int _sv0t1 = scan_instrs_env(body, env_names, env_bases, env_widths, env_cats, env_field_starts, next_slot, structs_names, structs_field_counts, enums_names, enums_widths, structs_fields_flat, structs_field_starts, env_fields_flat, source, starts, ends);
                return _sv0t1;
                _sv0t0 = 0;
              } else {
                if ((ins.tag == 0)) {
                  return next_slot;
                  _sv0t0 = 0;
                } else {
                  if ((ins.tag == 4)) {
                    int x = ins.p0;
                    int be = ins.p1;
                    return next_slot;
                    _sv0t0 = 0;
                  } else {
                    if ((ins.tag == 5)) {
                      int bh = ins.p0;
                      int fh = ins.p1;
                      int be = ins.p2;
                      return next_slot;
                      _sv0t0 = 0;
                    } else {
                      if ((ins.tag == 9)) {
                        return next_slot;
                        _sv0t0 = 0;
                      } else {
                        if ((ins.tag == 10)) {
                          return next_slot;
                          _sv0t0 = 0;
                        } else {
                          if ((ins.tag == 12)) {
                            int be = ins.p0;
                            int fn_h = ins.p1;
                            return next_slot;
                            _sv0t0 = 0;
                          } else {
                            if ((ins.tag == 13)) {
                              int be = ins.p0;
                              int fn_h = ins.p1;
                              return next_slot;
                              _sv0t0 = 0;
                            } else {
                              if ((ins.tag == 14)) {
                                return next_slot;
                                _sv0t0 = 0;
                              } else {
                                if ((ins.tag == 15)) {
                                  int bv = ins.p0;
                                  return next_slot;
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
  return next_slot;
}

static int scan_instrs_env(int instrs, int env_names, int env_bases, int env_widths, int env_cats, int env_field_starts, int next_slot, int structs_names, int structs_field_counts, int enums_names, int enums_widths, int structs_fields_flat, int structs_field_starts, int env_fields_flat, const char* source, int starts, int ends) {
  int _sv0t0 = sv0_vec_len(instrs);
  int n = _sv0t0;
  int ns = next_slot;
  int i = 0;
  while ((i < n)) {
    int _sv0t1 = sv0_vec_get(instrs, i);
    int bh = _sv0t1;
    Instr _sv0t2;
    int _sv0t3 = sv0_box_load(bh, 0);
    _sv0t2.tag = _sv0t3;
    int _sv0t4 = sv0_box_load(bh, 1);
    _sv0t2.p0 = _sv0t4;
    int _sv0t5 = sv0_box_load(bh, 2);
    _sv0t2.p1 = _sv0t5;
    int _sv0t6 = sv0_box_load(bh, 3);
    _sv0t2.p2 = _sv0t6;
    int _sv0t7 = sv0_box_load(bh, 4);
    _sv0t2.p3 = _sv0t7;
    Instr ins;
    ins.tag = (_sv0t2).tag;
    ins.p0 = (_sv0t2).p0;
    ins.p1 = (_sv0t2).p1;
    ins.p2 = (_sv0t2).p2;
    ins.p3 = (_sv0t2).p3;
    int _sv0t8 = scan_instr_env(ins, env_names, env_bases, env_widths, env_cats, env_field_starts, ns, structs_names, structs_field_counts, enums_names, enums_widths, structs_fields_flat, structs_field_starts, env_fields_flat, source, starts, ends);
    ns = _sv0t8;
    i = (i + 1);
  }
  return ns;
}

static int local_count_and_env(int param_names, int param_ctys, int instrs, int env_names, int env_bases, int env_widths, int env_cats, int env_field_starts, int structs_names, int structs_field_counts, int enums_names, int enums_widths, int structs_fields_flat, int structs_field_starts, int env_fields_flat, const char* source, int starts, int ends) {
  int _sv0t0 = sv0_vec_len(param_names);
  int np = _sv0t0;
  int ns = 0;
  int pi = 0;
  while ((pi < np)) {
    int _sv0t1 = sv0_vec_get(param_names, pi);
    int pn = _sv0t1;
    int _sv0t2 = sv0_vec_get(param_ctys, pi);
    int pc = _sv0t2;
    int _sv0t3 = width_of_cty(pc, structs_names, structs_field_counts, enums_names, enums_widths, source, starts, ends);
    int w = _sv0t3;
    sv0_vec_push(env_names, pn);
    sv0_vec_push(env_bases, ns);
    sv0_vec_push(env_widths, w);
    int _sv0t4 = cty_cat_of_handle(pc, source, starts, ends);
    sv0_vec_push(env_cats, _sv0t4);
    int _sv0t5 = record_field_layout(pc, structs_names, structs_field_counts, structs_fields_flat, structs_field_starts, source, starts, ends, env_fields_flat);
    sv0_vec_push(env_field_starts, _sv0t5);
    ns = (ns + w);
    pi = (pi + 1);
  }
  int _sv0t6 = scan_instrs_env(instrs, env_names, env_bases, env_widths, env_cats, env_field_starts, ns, structs_names, structs_field_counts, enums_names, enums_widths, structs_fields_flat, structs_field_starts, env_fields_flat, source, starts, ends);
  return _sv0t6;
}

static int compute_arity(int param_ctys, int structs_names, int structs_field_counts, int enums_names, int enums_widths, const char* source, int starts, int ends) {
  int _sv0t0 = sv0_vec_len(param_ctys);
  int np = _sv0t0;
  int total = 0;
  int i = 0;
  while ((i < np)) {
    int _sv0t1 = sv0_vec_get(param_ctys, i);
    int _sv0t2 = width_of_cty(_sv0t1, structs_names, structs_field_counts, enums_names, enums_widths, source, starts, ends);
    total = (total + _sv0t2);
    i = (i + 1);
  }
  return total;
}

static int emit_fn(int label, int param_names, int param_ctys, int instrs, int structs_names, int structs_field_counts, int enums_names, int enums_widths, int structs_fields_flat, int structs_field_starts, int pool, const char* source, int starts, int ends, int fn_names, int ft_entries) {
  int _sv0t0 = sv0_vec_new();
  int en = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int eb = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int ew = _sv0t2;
  int _sv0t3 = sv0_vec_new();
  int ec = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int efs = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int eff = _sv0t5;
  int _sv0t6 = local_count_and_env(param_names, param_ctys, instrs, en, eb, ew, ec, efs, structs_names, structs_field_counts, enums_names, enums_widths, structs_fields_flat, structs_field_starts, eff, source, starts, ends);
  int lc = _sv0t6;
  int _sv0t7 = sv0_vec_new();
  int code = _sv0t7;
  int _sv0t8 = emit_instrs(instrs, en, eb, ew, ec, efs, eff, pool, source, starts, ends, fn_names, code);
  int n = _sv0t8;
  if ((n < 0)) {
    int _sv0t9 = (0 - 1);
    return _sv0t9;
  } else {
  }
  int _sv0t10 = compute_arity(param_ctys, structs_names, structs_field_counts, enums_names, enums_widths, source, starts, ends);
  int arity = _sv0t10;
  int _sv0t11 = pool_add(pool, label, source, starts, ends);
  int name_idx = _sv0t11;
  sv0_vec_push(ft_entries, name_idx);
  sv0_vec_push(ft_entries, arity);
  sv0_vec_push(ft_entries, lc);
  sv0_vec_push(ft_entries, code);
  return 0;
}

static int is_main_label(int label, const char* source, int starts, int ends) {
  const char* _sv0t0 = handle_to_str(label, source, starts, ends);
  const char* s;
  s = _sv0t0;
  int _sv0t1 = sv0_string_eq(s, "main");
  return _sv0t1;
}

static int build_func_order(int block_labels, const char* source, int starts, int ends, int order) {
  int _sv0t0 = sv0_vec_len(block_labels);
  int n = _sv0t0;
  int main_idx = (0 - 1);
  int i = 0;
  while ((i < n)) {
    int _sv0t1 = sv0_vec_get(block_labels, i);
    int lab = _sv0t1;
    int _sv0t2 = is_main_label(lab, source, starts, ends);
    if (_sv0t2) {
      main_idx = i;
    } else {
      sv0_vec_push(order, i);
    }
    i = (i + 1);
  }
  if ((main_idx >= 0)) {
    sv0_vec_push(order, main_idx);
  } else {
  }
  int _sv0t3 = sv0_vec_len(order);
  return _sv0t3;
}

static int emit_program(int item_tags, int item_names, int item_field_counts, int item_variant_max_payload, int item_field_bases, int struct_field_name_toks, int block_labels, int block_param_names, int block_param_ctys, int block_instrs, const char* source, int starts, int ends, int out_pool, int out_ft) {
  int _sv0t0 = sv0_vec_new();
  int sn = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int sfc = _sv0t1;
  int _sv0t2 = struct_layouts_build(item_tags, item_names, item_field_counts, sn, sfc);
  int _sv0t3 = sv0_vec_new();
  int sff = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int sfs = _sv0t4;
  int _sv0t5 = struct_fields_build(item_tags, item_field_counts, item_field_bases, struct_field_name_toks, sff, sfs);
  int _sv0t6 = sv0_vec_new();
  int enn = _sv0t6;
  int _sv0t7 = sv0_vec_new();
  int enw = _sv0t7;
  int _sv0t8 = enum_layouts_build(item_tags, item_names, item_field_counts, item_variant_max_payload, enn, enw);
  int _sv0t9 = sv0_vec_new();
  int func_order = _sv0t9;
  int _sv0t10 = build_func_order(block_labels, source, starts, ends, func_order);
  int fc = _sv0t10;
  int _sv0t11 = sv0_vec_new();
  int fn_names = _sv0t11;
  int fi = 0;
  while ((fi < fc)) {
    int _sv0t12 = sv0_vec_get(func_order, fi);
    int bi = _sv0t12;
    int _sv0t13 = sv0_vec_get(block_labels, bi);
    sv0_vec_push(fn_names, _sv0t13);
    fi = (fi + 1);
  }
  fi = 0;
  while ((fi < fc)) {
    int _sv0t14 = sv0_vec_get(func_order, fi);
    int bi2 = _sv0t14;
    int _sv0t15 = sv0_vec_get(block_param_names, bi2);
    int pn = _sv0t15;
    int _sv0t16 = sv0_vec_get(block_param_ctys, bi2);
    int pc = _sv0t16;
    int _sv0t17 = sv0_vec_get(block_instrs, bi2);
    int ins = _sv0t17;
    int _sv0t18 = sv0_vec_get(block_labels, bi2);
    int lab = _sv0t18;
    int _sv0t19 = emit_fn(lab, pn, pc, ins, sn, sfc, enn, enw, sff, sfs, out_pool, source, starts, ends, fn_names, out_ft);
    int r = _sv0t19;
    if ((r < 0)) {
      int _sv0t20 = (0 - 1);
      return _sv0t20;
    } else {
    }
    fi = (fi + 1);
  }
  return fc;
}

static int test_variant_slots(void) {
  int _sv0t0 = variant_slots(0, 0);
  if ((_sv0t0 != 0)) {
    return 1;
  } else {
  }
  int _sv0t1 = variant_slots(1, 3);
  if ((_sv0t1 != 3)) {
    return 2;
  } else {
  }
  int _sv0t2 = variant_slots(2, 2);
  if ((_sv0t2 != 2)) {
    return 3;
  } else {
  }
  int _sv0t3 = variant_slots_unit();
  if ((_sv0t3 != 0)) {
    return 4;
  } else {
  }
  int _sv0t4 = variant_slots_tuple(5);
  if ((_sv0t4 != 5)) {
    return 5;
  } else {
  }
  return 0;
}

static int test_cty_classify(void) {
  int _sv0t0 = is_void_cty("void");
  if ((_sv0t0 != 1)) {
    return 1;
  } else {
  }
  int _sv0t1 = is_void_cty("int32_t");
  if ((_sv0t1 != 0)) {
    return 2;
  } else {
  }
  int _sv0t2 = is_scalar_cty("bool");
  if ((_sv0t2 != 1)) {
    return 3;
  } else {
  }
  int _sv0t3 = is_scalar_cty("int32_t");
  if ((_sv0t3 != 1)) {
    return 4;
  } else {
  }
  int _sv0t4 = is_scalar_cty("uint8_t");
  if ((_sv0t4 != 1)) {
    return 5;
  } else {
  }
  int _sv0t5 = is_scalar_cty("MyStruct");
  if ((_sv0t5 != 0)) {
    return 6;
  } else {
  }
  int _sv0t6 = width_of_cty_scalar("void");
  if ((_sv0t6 != 0)) {
    return 7;
  } else {
  }
  int _sv0t7 = width_of_cty_scalar("int32_t");
  if ((_sv0t7 != 1)) {
    return 8;
  } else {
  }
  int _sv0t8 = width_of_cty_scalar("bool");
  if ((_sv0t8 != 1)) {
    return 9;
  } else {
  }
  return 0;
}

static int test_width_of_cty(void) {
  int _sv0t0 = sv0_vec_new();
  int sn = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int sf = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int en = _sv0t2;
  int _sv0t3 = sv0_vec_new();
  int ew = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int ec = _sv0t4;
  sv0_vec_push(sn, 100);
  sv0_vec_push(sf, 3);
  sv0_vec_push(sn, 200);
  sv0_vec_push(sf, 5);
  sv0_vec_push(en, 300);
  sv0_vec_push(ew, 4);
  int _sv0t5 = sv0_vec_new();
  int _sv0t6 = sv0_vec_new();
  int _sv0t7 = width_of_cty(100, sn, sf, en, ew, "", _sv0t5, _sv0t6);
  if ((_sv0t7 != 3)) {
    return 1;
  } else {
  }
  int _sv0t8 = sv0_vec_new();
  int _sv0t9 = sv0_vec_new();
  int _sv0t10 = width_of_cty(200, sn, sf, en, ew, "", _sv0t8, _sv0t9);
  if ((_sv0t10 != 5)) {
    return 2;
  } else {
  }
  int _sv0t11 = sv0_vec_new();
  int _sv0t12 = sv0_vec_new();
  int _sv0t13 = width_of_cty(300, sn, sf, en, ew, "", _sv0t11, _sv0t12);
  if ((_sv0t13 != 4)) {
    return 3;
  } else {
  }
  int _sv0t14 = sv0_vec_new();
  int _sv0t15 = sv0_vec_new();
  int _sv0t16 = width_of_cty(999, sn, sf, en, ew, "", _sv0t14, _sv0t15);
  if ((_sv0t16 != 1)) {
    return 4;
  } else {
  }
  return 0;
}

static int test_enum_field_helpers(void) {
  int _sv0t0 = enum_field_count_from_width(4);
  if ((_sv0t0 != 4)) {
    return 1;
  } else {
  }
  int _sv0t1 = enum_field_is_tag(0);
  if ((_sv0t1 != 1)) {
    return 2;
  } else {
  }
  int _sv0t2 = enum_field_is_tag(1);
  if ((_sv0t2 != 0)) {
    return 3;
  } else {
  }
  int _sv0t3 = enum_payload_field_index(1);
  if ((_sv0t3 != 0)) {
    return 4;
  } else {
  }
  int _sv0t4 = enum_payload_field_index(3);
  if ((_sv0t4 != 2)) {
    return 5;
  } else {
  }
  return 0;
}

static int test_layout_lookup(void) {
  int _sv0t0 = sv0_vec_new();
  int names = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int widths = _sv0t1;
  sv0_vec_push(names, 10);
  sv0_vec_push(widths, 3);
  sv0_vec_push(names, 20);
  sv0_vec_push(widths, 7);
  int _sv0t2 = layout_lookup(names, widths, 10);
  if ((_sv0t2 != 3)) {
    return 1;
  } else {
  }
  int _sv0t3 = layout_lookup(names, widths, 20);
  if ((_sv0t3 != 7)) {
    return 2;
  } else {
  }
  int _sv0t4 = layout_lookup(names, widths, 99);
  int _sv0t5 = (0 - 1);
  if ((_sv0t4 != _sv0t5)) {
    return 3;
  } else {
  }
  return 0;
}

static int test_index_of_field(void) {
  int _sv0t0 = sv0_vec_new();
  int fields = _sv0t0;
  sv0_vec_push(fields, 10);
  sv0_vec_push(fields, 20);
  sv0_vec_push(fields, 30);
  int _sv0t1 = index_of_field(fields, 10);
  if ((_sv0t1 != 0)) {
    return 1;
  } else {
  }
  int _sv0t2 = index_of_field(fields, 20);
  if ((_sv0t2 != 1)) {
    return 2;
  } else {
  }
  int _sv0t3 = index_of_field(fields, 30);
  if ((_sv0t3 != 2)) {
    return 3;
  } else {
  }
  int _sv0t4 = index_of_field(fields, 99);
  int _sv0t5 = (0 - 1);
  if ((_sv0t4 != _sv0t5)) {
    return 4;
  } else {
  }
  return 0;
}

static int test_binop_to_insn(void) {
  int _sv0t0 = binop_to_insn("+");
  if ((_sv0t0 != 16)) {
    return 1;
  } else {
  }
  int _sv0t1 = binop_to_insn("-");
  if ((_sv0t1 != 17)) {
    return 2;
  } else {
  }
  int _sv0t2 = binop_to_insn("*");
  if ((_sv0t2 != 18)) {
    return 3;
  } else {
  }
  int _sv0t3 = binop_to_insn("==");
  if ((_sv0t3 != 64)) {
    return 4;
  } else {
  }
  int _sv0t4 = binop_to_insn("!=");
  if ((_sv0t4 != 65)) {
    return 5;
  } else {
  }
  int _sv0t5 = binop_to_insn("<");
  if ((_sv0t5 != 66)) {
    return 6;
  } else {
  }
  int _sv0t6 = binop_to_insn("&&");
  if ((_sv0t6 != 80)) {
    return 7;
  } else {
  }
  int _sv0t7 = binop_to_insn("&");
  if ((_sv0t7 != 88)) {
    return 8;
  } else {
  }
  int _sv0t8 = binop_to_insn("<<");
  if ((_sv0t8 != 92)) {
    return 9;
  } else {
  }
  int _sv0t9 = binop_to_insn(">>");
  if ((_sv0t9 != 93)) {
    return 10;
  } else {
  }
  int _sv0t10 = binop_to_insn("??");
  int _sv0t11 = (0 - 1);
  if ((_sv0t10 != _sv0t11)) {
    return 11;
  } else {
  }
  return 0;
}

static int test_unop_to_insn(void) {
  int _sv0t0 = unop_to_insn("!");
  if ((_sv0t0 != 82)) {
    return 1;
  } else {
  }
  int _sv0t1 = unop_to_insn("-");
  if ((_sv0t1 != 21)) {
    return 2;
  } else {
  }
  int _sv0t2 = unop_to_insn("~");
  int _sv0t3 = (0 - 1);
  if ((_sv0t2 != _sv0t3)) {
    return 3;
  } else {
  }
  return 0;
}

static int test_enc_len(void) {
  int _sv0t0 = enc_len(0);
  if ((_sv0t0 != 1)) {
    return 1;
  } else {
  }
  int _sv0t1 = enc_len(4);
  if ((_sv0t1 != 5)) {
    return 2;
  } else {
  }
  int _sv0t2 = enc_len(5);
  if ((_sv0t2 != 9)) {
    return 3;
  } else {
  }
  int _sv0t3 = enc_len(115);
  if ((_sv0t3 != 9)) {
    return 4;
  } else {
  }
  int _sv0t4 = enc_len(144);
  if ((_sv0t4 != 13)) {
    return 5;
  } else {
  }
  int _sv0t5 = sv0_vec_new();
  int opcodes = _sv0t5;
  sv0_vec_push(opcodes, 0);
  sv0_vec_push(opcodes, 4);
  sv0_vec_push(opcodes, 116);
  int _sv0t6 = enc_lens(opcodes);
  if ((_sv0t6 != 7)) {
    return 6;
  } else {
  }
  return 0;
}

static int test_sentinels(void) {
  int _sv0t0 = loop_exit_sentinel();
  int _sv0t1 = is_loop_exit_sentinel(_sv0t0);
  if ((_sv0t1 != 1)) {
    return 1;
  } else {
  }
  int _sv0t2 = loop_continue_sentinel();
  int _sv0t3 = is_loop_continue_sentinel(_sv0t2);
  if ((_sv0t3 != 1)) {
    return 2;
  } else {
  }
  int _sv0t4 = is_loop_exit_sentinel(0);
  if ((_sv0t4 != 0)) {
    return 3;
  } else {
  }
  int _sv0t5 = is_loop_continue_sentinel(0);
  if ((_sv0t5 != 0)) {
    return 4;
  } else {
  }
  int _sv0t6 = loop_exit_sentinel();
  int _sv0t7 = loop_continue_sentinel();
  if ((_sv0t6 == _sv0t7)) {
    return 5;
  } else {
  }
  return 0;
}

static int test_pool(void) {
  int _sv0t0 = pool_new();
  int pool = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int _sv0t2 = sv0_vec_new();
  int _sv0t3 = pool_add(pool, 100, "", _sv0t1, _sv0t2);
  int i0 = _sv0t3;
  if ((i0 != 0)) {
    return 1;
  } else {
  }
  int _sv0t4 = sv0_vec_new();
  int _sv0t5 = sv0_vec_new();
  int _sv0t6 = pool_add(pool, 200, "", _sv0t4, _sv0t5);
  int i1 = _sv0t6;
  if ((i1 != 1)) {
    return 2;
  } else {
  }
  int _sv0t7 = sv0_vec_new();
  int _sv0t8 = sv0_vec_new();
  int _sv0t9 = pool_add(pool, 100, "", _sv0t7, _sv0t8);
  int i0b = _sv0t9;
  if ((i0b != 0)) {
    return 3;
  } else {
  }
  int _sv0t10 = pool_size(pool);
  if ((_sv0t10 != 2)) {
    return 4;
  } else {
  }
  return 0;
}

static int test_builtin_id(void) {
  int _sv0t0 = builtin_id("sv0_println");
  if ((_sv0t0 != 0)) {
    return 1;
  } else {
  }
  int _sv0t1 = builtin_id("sv0_string_len");
  if ((_sv0t1 != 2)) {
    return 2;
  } else {
  }
  int _sv0t2 = builtin_id("sv0_vec_new");
  if ((_sv0t2 != 7)) {
    return 3;
  } else {
  }
  int _sv0t3 = builtin_id("sv0_box_load");
  if ((_sv0t3 != 14)) {
    return 4;
  } else {
  }
  int _sv0t4 = builtin_id("sv0_read_file");
  if ((_sv0t4 != 15)) {
    return 5;
  } else {
  }
  int _sv0t5 = builtin_id("sv0_write_file");
  if ((_sv0t5 != 16)) {
    return 6;
  } else {
  }
  int _sv0t6 = builtin_id("sv0_read_dir");
  if ((_sv0t6 != 17)) {
    return 7;
  } else {
  }
  int _sv0t7 = builtin_id("unknown");
  int _sv0t8 = (0 - 1);
  if ((_sv0t7 != _sv0t8)) {
    return 8;
  } else {
  }
  int _sv0t9 = is_builtin("sv0_println");
  if ((_sv0t9 != 1)) {
    return 9;
  } else {
  }
  int _sv0t10 = is_builtin("foo");
  if ((_sv0t10 != 0)) {
    return 10;
  } else {
  }
  int _sv0t11 = builtin_has_result("sv0_println");
  if ((_sv0t11 != 0)) {
    return 11;
  } else {
  }
  int _sv0t12 = builtin_has_result("sv0_string_len");
  if ((_sv0t12 != 1)) {
    return 12;
  } else {
  }
  int _sv0t13 = builtin_arg_count("sv0_println");
  if ((_sv0t13 != 1)) {
    return 13;
  } else {
  }
  int _sv0t14 = builtin_arg_count("sv0_string_substr");
  if ((_sv0t14 != 3)) {
    return 14;
  } else {
  }
  return 0;
}

static int test_store_ordering(void) {
  int _sv0t0 = store_slot_offset(10, 3, 0);
  if ((_sv0t0 != 12)) {
    return 1;
  } else {
  }
  int _sv0t1 = store_slot_offset(10, 3, 1);
  if ((_sv0t1 != 11)) {
    return 2;
  } else {
  }
  int _sv0t2 = store_slot_offset(10, 3, 2);
  if ((_sv0t2 != 10)) {
    return 3;
  } else {
  }
  int _sv0t3 = store_slot_offset(0, 1, 0);
  if ((_sv0t3 != 0)) {
    return 4;
  } else {
  }
  return 0;
}

static int test_lookup_slot(void) {
  int _sv0t0 = sv0_vec_new();
  int names = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int bases = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int widths = _sv0t2;
  sv0_vec_push(names, 10);
  sv0_vec_push(bases, 0);
  sv0_vec_push(widths, 1);
  sv0_vec_push(names, 20);
  sv0_vec_push(bases, 1);
  sv0_vec_push(widths, 3);
  int _sv0t3 = sv0_vec_new();
  int _sv0t4 = sv0_vec_new();
  int _sv0t5 = lookup_slot(names, bases, widths, 10, "", _sv0t3, _sv0t4);
  if ((_sv0t5 != 0)) {
    return 1;
  } else {
  }
  int _sv0t6 = sv0_vec_new();
  int _sv0t7 = sv0_vec_new();
  int _sv0t8 = lookup_slot(names, bases, widths, 20, "", _sv0t6, _sv0t7);
  if ((_sv0t8 != 1)) {
    return 2;
  } else {
  }
  int _sv0t9 = sv0_vec_new();
  int _sv0t10 = sv0_vec_new();
  int _sv0t11 = lookup_slot(names, bases, widths, 99, "", _sv0t9, _sv0t10);
  int _sv0t12 = (0 - 1);
  if ((_sv0t11 != _sv0t12)) {
    return 3;
  } else {
  }
  int _sv0t13 = slot_base(bases, 1);
  if ((_sv0t13 != 1)) {
    return 4;
  } else {
  }
  int _sv0t14 = slot_width(widths, 1);
  if ((_sv0t14 != 3)) {
    return 5;
  } else {
  }
  return 0;
}

static int test_alloc_local(void) {
  int _sv0t0 = sv0_vec_new();
  int names = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int bases = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int widths = _sv0t2;
  int _sv0t3 = alloc_local(names, bases, widths, 10, 1, 0);
  int s1 = _sv0t3;
  if ((s1 != 1)) {
    return 1;
  } else {
  }
  int _sv0t4 = alloc_local(names, bases, widths, 20, 3, 1);
  int s2 = _sv0t4;
  if ((s2 != 4)) {
    return 2;
  } else {
  }
  int _sv0t5 = sv0_vec_len(names);
  if ((_sv0t5 != 2)) {
    return 3;
  } else {
  }
  int _sv0t6 = slot_base(bases, 0);
  if ((_sv0t6 != 0)) {
    return 4;
  } else {
  }
  int _sv0t7 = slot_width(widths, 1);
  if ((_sv0t7 != 3)) {
    return 5;
  } else {
  }
  return 0;
}

static int test_replace_loop_exit(void) {
  int _sv0t0 = sv0_vec_new();
  int instrs = _sv0t0;
  sv0_vec_push(instrs, 112);
  int _sv0t1 = loop_exit_sentinel();
  sv0_vec_push(instrs, _sv0t1);
  int _sv0t2 = replace_loop_exit_jump(instrs, 42);
  if ((_sv0t2 != 1)) {
    return 1;
  } else {
  }
  int _sv0t3 = sv0_vec_get(instrs, 1);
  if ((_sv0t3 != 42)) {
    return 2;
  } else {
  }
  int _sv0t4 = sv0_vec_new();
  int bad = _sv0t4;
  sv0_vec_push(bad, 112);
  sv0_vec_push(bad, 999);
  int _sv0t5 = replace_loop_exit_jump(bad, 42);
  if ((_sv0t5 != 0)) {
    return 3;
  } else {
  }
  int _sv0t6 = sv0_vec_new();
  int empty = _sv0t6;
  int _sv0t7 = replace_loop_exit_jump(empty, 42);
  if ((_sv0t7 != 0)) {
    return 4;
  } else {
  }
  return 0;
}

static int test_enum_field_names(void) {
  int _sv0t0 = enum_field_names(0);
  int f0 = _sv0t0;
  int _sv0t1 = sv0_vec_len(f0);
  if ((_sv0t1 != 0)) {
    return 1;
  } else {
  }
  int _sv0t2 = enum_field_names(1);
  int f1 = _sv0t2;
  int _sv0t3 = sv0_vec_len(f1);
  if ((_sv0t3 != 1)) {
    return 2;
  } else {
  }
  int _sv0t4 = sv0_vec_get(f1, 0);
  if ((_sv0t4 != 0)) {
    return 3;
  } else {
  }
  int _sv0t5 = enum_field_names(4);
  int f4 = _sv0t5;
  int _sv0t6 = sv0_vec_len(f4);
  if ((_sv0t6 != 4)) {
    return 4;
  } else {
  }
  int _sv0t7 = sv0_vec_get(f4, 0);
  if ((_sv0t7 != 0)) {
    return 5;
  } else {
  }
  int _sv0t8 = sv0_vec_get(f4, 3);
  if ((_sv0t8 != 3)) {
    return 6;
  } else {
  }
  return 0;
}

static int test_value_width(void) {
  int _sv0t0 = sv0_vec_new();
  int names = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int bases = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int widths = _sv0t2;
  sv0_vec_push(names, 10);
  sv0_vec_push(bases, 0);
  sv0_vec_push(widths, 1);
  sv0_vec_push(names, 20);
  sv0_vec_push(bases, 1);
  sv0_vec_push(widths, 3);
  int _sv0t3 = value_width(4, 20, names, bases, widths);
  if ((_sv0t3 != 3)) {
    return 1;
  } else {
  }
  int _sv0t4 = value_width(4, 10, names, bases, widths);
  if ((_sv0t4 != 1)) {
    return 2;
  } else {
  }
  int _sv0t5 = value_width(4, 99, names, bases, widths);
  if ((_sv0t5 != 1)) {
    return 3;
  } else {
  }
  int _sv0t6 = value_width(0, 10, names, bases, widths);
  if ((_sv0t6 != 1)) {
    return 4;
  } else {
  }
  return 0;
}

static int test_bind_param(void) {
  int _sv0t0 = sv0_vec_new();
  int en = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int eb = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int ew = _sv0t2;
  int _sv0t3 = sv0_vec_new();
  int ec = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int ef = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int sn = _sv0t5;
  int _sv0t6 = sv0_vec_new();
  int sf = _sv0t6;
  int _sv0t7 = sv0_vec_new();
  int enn = _sv0t7;
  int _sv0t8 = sv0_vec_new();
  int enw = _sv0t8;
  sv0_vec_push(sn, 100);
  sv0_vec_push(sf, 3);
  sv0_vec_push(enn, 200);
  sv0_vec_push(enw, 4);
  int _sv0t9 = bind_param(10, 100, 0, en, eb, ew, ef, sn, sf, enn, enw);
  int s1 = _sv0t9;
  if ((s1 != 3)) {
    return 1;
  } else {
  }
  int _sv0t10 = bind_param(20, 200, 3, en, eb, ew, ef, sn, sf, enn, enw);
  int s2 = _sv0t10;
  if ((s2 != 7)) {
    return 2;
  } else {
  }
  int _sv0t11 = bind_param(30, 999, 7, en, eb, ew, ef, sn, sf, enn, enw);
  int s3 = _sv0t11;
  if ((s3 != 8)) {
    return 3;
  } else {
  }
  int _sv0t12 = sv0_vec_len(en);
  if ((_sv0t12 != 3)) {
    return 4;
  } else {
  }
  int _sv0t13 = sv0_vec_get(ew, 0);
  if ((_sv0t13 != 3)) {
    return 5;
  } else {
  }
  int _sv0t14 = sv0_vec_get(ew, 1);
  if ((_sv0t14 != 4)) {
    return 6;
  } else {
  }
  int _sv0t15 = sv0_vec_get(ew, 2);
  if ((_sv0t15 != 1)) {
    return 7;
  } else {
  }
  return 0;
}

static int test_patch_continue_jumps(void) {
  int _sv0t0 = sv0_vec_new();
  int instrs = _sv0t0;
  sv0_vec_push(instrs, 112);
  int _sv0t1 = loop_continue_sentinel();
  sv0_vec_push(instrs, _sv0t1);
  int _sv0t2 = patch_continue_jumps(instrs, 0, 100);
  int n = _sv0t2;
  if ((n != 1)) {
    return 1;
  } else {
  }
  int _sv0t3 = sv0_vec_get(instrs, 1);
  if ((_sv0t3 != 95)) {
    return 2;
  } else {
  }
  int _sv0t4 = sv0_vec_new();
  int i2 = _sv0t4;
  sv0_vec_push(i2, 0);
  sv0_vec_push(i2, 112);
  int _sv0t5 = loop_continue_sentinel();
  sv0_vec_push(i2, _sv0t5);
  int _sv0t6 = patch_continue_jumps(i2, 10, 50);
  int n2 = _sv0t6;
  if ((n2 != 1)) {
    return 3;
  } else {
  }
  int _sv0t7 = sv0_vec_get(i2, 2);
  if ((_sv0t7 != 34)) {
    return 4;
  } else {
  }
  int _sv0t8 = sv0_vec_new();
  int i3 = _sv0t8;
  sv0_vec_push(i3, 112);
  sv0_vec_push(i3, 42);
  int _sv0t9 = patch_continue_jumps(i3, 0, 100);
  int n3 = _sv0t9;
  if ((n3 != 0)) {
    return 5;
  } else {
  }
  int _sv0t10 = sv0_vec_get(i3, 1);
  if ((_sv0t10 != 42)) {
    return 6;
  } else {
  }
  int _sv0t11 = sv0_vec_new();
  int empty = _sv0t11;
  int _sv0t12 = patch_continue_jumps(empty, 0, 100);
  int n4 = _sv0t12;
  if ((n4 != 0)) {
    return 7;
  } else {
  }
  return 0;
}

static int test_struct_layouts_build(void) {
  int _sv0t0 = sv0_vec_new();
  int tags = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int names = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int fcounts = _sv0t2;
  sv0_vec_push(tags, 0);
  sv0_vec_push(names, 10);
  sv0_vec_push(fcounts, 0);
  sv0_vec_push(tags, 1);
  sv0_vec_push(names, 20);
  sv0_vec_push(fcounts, 3);
  sv0_vec_push(tags, 2);
  sv0_vec_push(names, 30);
  sv0_vec_push(fcounts, 0);
  sv0_vec_push(tags, 1);
  sv0_vec_push(names, 40);
  sv0_vec_push(fcounts, 2);
  int _sv0t3 = sv0_vec_new();
  int out_n = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int out_fc = _sv0t4;
  int _sv0t5 = struct_layouts_build(tags, names, fcounts, out_n, out_fc);
  int count = _sv0t5;
  if ((count != 2)) {
    return 1;
  } else {
  }
  int _sv0t6 = sv0_vec_get(out_n, 0);
  if ((_sv0t6 != 20)) {
    return 2;
  } else {
  }
  int _sv0t7 = sv0_vec_get(out_fc, 0);
  if ((_sv0t7 != 3)) {
    return 3;
  } else {
  }
  int _sv0t8 = sv0_vec_get(out_n, 1);
  if ((_sv0t8 != 40)) {
    return 4;
  } else {
  }
  int _sv0t9 = sv0_vec_get(out_fc, 1);
  if ((_sv0t9 != 2)) {
    return 5;
  } else {
  }
  return 0;
}

static int test_enum_layouts_build(void) {
  int _sv0t0 = sv0_vec_new();
  int tags = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int names = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int vcounts = _sv0t2;
  int _sv0t3 = sv0_vec_new();
  int maxp = _sv0t3;
  sv0_vec_push(tags, 0);
  sv0_vec_push(names, 10);
  sv0_vec_push(vcounts, 0);
  sv0_vec_push(maxp, 0);
  sv0_vec_push(tags, 2);
  sv0_vec_push(names, 20);
  sv0_vec_push(vcounts, 3);
  sv0_vec_push(maxp, 2);
  sv0_vec_push(tags, 1);
  sv0_vec_push(names, 30);
  sv0_vec_push(vcounts, 0);
  sv0_vec_push(maxp, 0);
  sv0_vec_push(tags, 2);
  sv0_vec_push(names, 40);
  sv0_vec_push(vcounts, 2);
  sv0_vec_push(maxp, 0);
  int _sv0t4 = sv0_vec_new();
  int out_n = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int out_w = _sv0t5;
  int _sv0t6 = enum_layouts_build(tags, names, vcounts, maxp, out_n, out_w);
  int count = _sv0t6;
  if ((count != 2)) {
    return 1;
  } else {
  }
  int _sv0t7 = sv0_vec_get(out_n, 0);
  if ((_sv0t7 != 20)) {
    return 2;
  } else {
  }
  int _sv0t8 = sv0_vec_get(out_w, 0);
  if ((_sv0t8 != 3)) {
    return 3;
  } else {
  }
  int _sv0t9 = sv0_vec_get(out_n, 1);
  if ((_sv0t9 != 40)) {
    return 4;
  } else {
  }
  int _sv0t10 = sv0_vec_get(out_w, 1);
  if ((_sv0t10 != 1)) {
    return 5;
  } else {
  }
  return 0;
}

static int test_emit_value(void) {
  int _sv0t0 = sv0_vec_new();
  int en = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int eb = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int ew = _sv0t2;
  int _sv0t3 = sv0_vec_new();
  int ec = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int pool = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int out = _sv0t5;
  Value vi;
  vi.tag = 0;
  vi.p0 = 42;
  int _sv0t6 = sv0_vec_new();
  int _sv0t7 = sv0_vec_new();
  int _sv0t8 = sv0_vec_new();
  int _sv0t9 = sv0_vec_new();
  int _sv0t10 = emit_value(vi, en, eb, ew, _sv0t6, _sv0t7, pool, "", _sv0t8, _sv0t9, out);
  int r1 = _sv0t10;
  if ((r1 != 2)) {
    return 1;
  } else {
  }
  int _sv0t11 = sv0_vec_get(out, 0);
  if ((_sv0t11 != 4)) {
    return 2;
  } else {
  }
  int _sv0t12 = sv0_vec_get(out, 1);
  if ((_sv0t12 != 42)) {
    return 3;
  } else {
  }
  int _sv0t13 = sv0_vec_new();
  int out2 = _sv0t13;
  Value vb;
  vb.tag = 1;
  int _sv0t14 = sv0_vec_new();
  int _sv0t15 = sv0_vec_new();
  int _sv0t16 = sv0_vec_new();
  int _sv0t17 = sv0_vec_new();
  int _sv0t18 = emit_value(vb, en, eb, ew, _sv0t14, _sv0t15, pool, "", _sv0t16, _sv0t17, out2);
  int r2 = _sv0t18;
  if ((r2 != 2)) {
    return 4;
  } else {
  }
  int _sv0t19 = sv0_vec_get(out2, 0);
  if ((_sv0t19 != 7)) {
    return 5;
  } else {
  }
  int _sv0t20 = sv0_vec_get(out2, 1);
  if ((_sv0t20 != 1)) {
    return 6;
  } else {
  }
  int _sv0t21 = sv0_vec_new();
  int out3 = _sv0t21;
  Value vbf;
  vbf.tag = 2;
  int _sv0t22 = sv0_vec_new();
  int _sv0t23 = sv0_vec_new();
  int _sv0t24 = sv0_vec_new();
  int _sv0t25 = sv0_vec_new();
  int _sv0t26 = emit_value(vbf, en, eb, ew, _sv0t22, _sv0t23, pool, "", _sv0t24, _sv0t25, out3);
  int r2b = _sv0t26;
  if ((r2b != 2)) {
    return 7;
  } else {
  }
  int _sv0t27 = sv0_vec_get(out3, 1);
  if ((_sv0t27 != 0)) {
    return 8;
  } else {
  }
  int _sv0t28 = sv0_vec_new();
  int out4 = _sv0t28;
  Value vu;
  vu.tag = 4;
  int _sv0t29 = sv0_vec_new();
  int _sv0t30 = sv0_vec_new();
  int _sv0t31 = sv0_vec_new();
  int _sv0t32 = sv0_vec_new();
  int _sv0t33 = emit_value(vu, en, eb, ew, _sv0t29, _sv0t30, pool, "", _sv0t31, _sv0t32, out4);
  int r3 = _sv0t33;
  if ((r3 != 1)) {
    return 9;
  } else {
  }
  int _sv0t34 = sv0_vec_get(out4, 0);
  if ((_sv0t34 != 3)) {
    return 10;
  } else {
  }
  int _sv0t35 = sv0_vec_new();
  int out5 = _sv0t35;
  Value vs;
  vs.tag = 5;
  vs.p0 = 99;
  int _sv0t36 = sv0_vec_new();
  int _sv0t37 = sv0_vec_new();
  int _sv0t38 = sv0_vec_new();
  int _sv0t39 = sv0_vec_new();
  int _sv0t40 = emit_value(vs, en, eb, ew, _sv0t36, _sv0t37, pool, "", _sv0t38, _sv0t39, out5);
  int r4 = _sv0t40;
  if ((r4 != 2)) {
    return 11;
  } else {
  }
  int _sv0t41 = sv0_vec_get(out5, 0);
  if ((_sv0t41 != 8)) {
    return 12;
  } else {
  }
  int _sv0t42 = sv0_vec_get(out5, 1);
  if ((_sv0t42 != 0)) {
    return 13;
  } else {
  }
  sv0_vec_push(en, 50);
  sv0_vec_push(eb, 2);
  sv0_vec_push(ew, 1);
  int _sv0t43 = sv0_vec_new();
  int out6 = _sv0t43;
  Value vv;
  vv.tag = 3;
  vv.p0 = 50;
  int _sv0t44 = sv0_vec_new();
  int _sv0t45 = sv0_vec_new();
  int _sv0t46 = sv0_vec_new();
  int _sv0t47 = sv0_vec_new();
  int _sv0t48 = emit_value(vv, en, eb, ew, _sv0t44, _sv0t45, pool, "", _sv0t46, _sv0t47, out6);
  int r5 = _sv0t48;
  if ((r5 != 2)) {
    return 14;
  } else {
  }
  int _sv0t49 = sv0_vec_get(out6, 0);
  if ((_sv0t49 != 96)) {
    return 15;
  } else {
  }
  int _sv0t50 = sv0_vec_get(out6, 1);
  if ((_sv0t50 != 2)) {
    return 16;
  } else {
  }
  sv0_vec_push(en, 60);
  sv0_vec_push(eb, 5);
  sv0_vec_push(ew, 3);
  int _sv0t51 = sv0_vec_new();
  int out7 = _sv0t51;
  Value vv2;
  vv2.tag = 3;
  vv2.p0 = 60;
  int _sv0t52 = sv0_vec_new();
  int _sv0t53 = sv0_vec_new();
  int _sv0t54 = sv0_vec_new();
  int _sv0t55 = sv0_vec_new();
  int _sv0t56 = emit_value(vv2, en, eb, ew, _sv0t52, _sv0t53, pool, "", _sv0t54, _sv0t55, out7);
  int r6 = _sv0t56;
  if ((r6 != 6)) {
    return 17;
  } else {
  }
  int _sv0t57 = sv0_vec_get(out7, 0);
  if ((_sv0t57 != 96)) {
    return 18;
  } else {
  }
  int _sv0t58 = sv0_vec_get(out7, 1);
  if ((_sv0t58 != 5)) {
    return 19;
  } else {
  }
  int _sv0t59 = sv0_vec_get(out7, 2);
  if ((_sv0t59 != 96)) {
    return 20;
  } else {
  }
  int _sv0t60 = sv0_vec_get(out7, 3);
  if ((_sv0t60 != 6)) {
    return 21;
  } else {
  }
  int _sv0t61 = sv0_vec_get(out7, 4);
  if ((_sv0t61 != 96)) {
    return 22;
  } else {
  }
  int _sv0t62 = sv0_vec_get(out7, 5);
  if ((_sv0t62 != 7)) {
    return 23;
  } else {
  }
  int _sv0t63 = sv0_vec_new();
  int out8 = _sv0t63;
  Value va;
  va.tag = 7;
  va.p0 = 50;
  int _sv0t64 = sv0_vec_new();
  int _sv0t65 = sv0_vec_new();
  int _sv0t66 = sv0_vec_new();
  int _sv0t67 = sv0_vec_new();
  int _sv0t68 = emit_value(va, en, eb, ew, _sv0t64, _sv0t65, pool, "", _sv0t66, _sv0t67, out8);
  int r7 = _sv0t68;
  if ((r7 != 2)) {
    return 24;
  } else {
  }
  int _sv0t69 = sv0_vec_get(out8, 0);
  if ((_sv0t69 != 4)) {
    return 25;
  } else {
  }
  int _sv0t70 = sv0_vec_get(out8, 1);
  if ((_sv0t70 != 2)) {
    return 26;
  } else {
  }
  int _sv0t71 = sv0_vec_new();
  int out9 = _sv0t71;
  Value vm0;
  Value _sv0t72;
  _sv0t72.tag = 3;
  _sv0t72.p0 = 60;
  int _sv0t73 = sv0_box_alloc(3);
  sv0_box_store(_sv0t73, 0, _sv0t72.tag);
  sv0_box_store(_sv0t73, 1, _sv0t72.p0);
  sv0_box_store(_sv0t73, 2, _sv0t72.p1);
  vm0.tag = 6;
  vm0.p0 = _sv0t73;
  vm0.p1 = 0;
  int _sv0t74 = sv0_vec_new();
  int _sv0t75 = sv0_vec_new();
  int _sv0t76 = sv0_vec_new();
  int _sv0t77 = sv0_vec_new();
  int _sv0t78 = emit_value(vm0, en, eb, ew, _sv0t74, _sv0t75, pool, "", _sv0t76, _sv0t77, out9);
  int r8 = _sv0t78;
  if ((r8 != 2)) {
    return 27;
  } else {
  }
  int _sv0t79 = sv0_vec_get(out9, 0);
  if ((_sv0t79 != 96)) {
    return 28;
  } else {
  }
  int _sv0t80 = sv0_vec_get(out9, 1);
  if ((_sv0t80 != 5)) {
    return 29;
  } else {
  }
  int _sv0t81 = sv0_vec_new();
  int out10 = _sv0t81;
  Value vm_tag;
  Value _sv0t82;
  _sv0t82.tag = 3;
  _sv0t82.p0 = 60;
  int _sv0t83 = sv0_box_alloc(3);
  sv0_box_store(_sv0t83, 0, _sv0t82.tag);
  sv0_box_store(_sv0t83, 1, _sv0t82.p0);
  sv0_box_store(_sv0t83, 2, _sv0t82.p1);
  int _sv0t84 = (0 - 1);
  vm_tag.tag = 6;
  vm_tag.p0 = _sv0t83;
  vm_tag.p1 = _sv0t84;
  int _sv0t85 = sv0_vec_new();
  int _sv0t86 = sv0_vec_new();
  int _sv0t87 = sv0_vec_new();
  int _sv0t88 = sv0_vec_new();
  int _sv0t89 = emit_value(vm_tag, en, eb, ew, _sv0t85, _sv0t86, pool, "", _sv0t87, _sv0t88, out10);
  int r9 = _sv0t89;
  if ((r9 != 2)) {
    return 30;
  } else {
  }
  int _sv0t90 = sv0_vec_get(out10, 0);
  if ((_sv0t90 != 96)) {
    return 31;
  } else {
  }
  int _sv0t91 = sv0_vec_get(out10, 1);
  if ((_sv0t91 != 5)) {
    return 32;
  } else {
  }
  int _sv0t92 = sv0_vec_new();
  int out11 = _sv0t92;
  Value vm_oob;
  Value _sv0t93;
  _sv0t93.tag = 3;
  _sv0t93.p0 = 60;
  int _sv0t94 = sv0_box_alloc(3);
  sv0_box_store(_sv0t94, 0, _sv0t93.tag);
  sv0_box_store(_sv0t94, 1, _sv0t93.p0);
  sv0_box_store(_sv0t94, 2, _sv0t93.p1);
  vm_oob.tag = 6;
  vm_oob.p0 = _sv0t94;
  vm_oob.p1 = 3;
  int _sv0t95 = sv0_vec_new();
  int _sv0t96 = sv0_vec_new();
  int _sv0t97 = sv0_vec_new();
  int _sv0t98 = sv0_vec_new();
  int _sv0t99 = emit_value(vm_oob, en, eb, ew, _sv0t95, _sv0t96, pool, "", _sv0t97, _sv0t98, out11);
  int r10 = _sv0t99;
  int _sv0t100 = (0 - 1);
  if ((r10 != _sv0t100)) {
    return 33;
  } else {
  }
  return 0;
}

static int test_emit_expr(void) {
  int _sv0t0 = sv0_vec_new();
  int en = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int eb = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int ew = _sv0t2;
  int _sv0t3 = sv0_vec_new();
  int ec = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int pool = _sv0t4;
  const char* source;
  source = "+-!i32";
  int _sv0t5 = sv0_vec_new();
  int starts = _sv0t5;
  int _sv0t6 = sv0_vec_new();
  int ends = _sv0t6;
  sv0_vec_push(starts, 0);
  sv0_vec_push(ends, 1);
  sv0_vec_push(starts, 1);
  sv0_vec_push(ends, 2);
  sv0_vec_push(starts, 2);
  sv0_vec_push(ends, 3);
  sv0_vec_push(starts, 3);
  sv0_vec_push(ends, 6);
  int _sv0t7 = sv0_vec_new();
  int out1 = _sv0t7;
  Expr lit;
  Value _sv0t8;
  _sv0t8.tag = 0;
  _sv0t8.p0 = 7;
  int _sv0t9 = sv0_box_alloc(3);
  sv0_box_store(_sv0t9, 0, _sv0t8.tag);
  sv0_box_store(_sv0t9, 1, _sv0t8.p0);
  sv0_box_store(_sv0t9, 2, _sv0t8.p1);
  lit.tag = 0;
  lit.p0 = _sv0t9;
  int _sv0t10 = sv0_vec_new();
  int _sv0t11 = sv0_vec_new();
  int _sv0t12 = emit_expr(lit, en, eb, ew, ec, _sv0t10, _sv0t11, pool, source, starts, ends, out1);
  int r1 = _sv0t12;
  if ((r1 != 2)) {
    return 1;
  } else {
  }
  int _sv0t13 = sv0_vec_get(out1, 0);
  if ((_sv0t13 != 4)) {
    return 2;
  } else {
  }
  int _sv0t14 = sv0_vec_get(out1, 1);
  if ((_sv0t14 != 7)) {
    return 3;
  } else {
  }
  int _sv0t15 = sv0_vec_new();
  int out2 = _sv0t15;
  Expr binop;
  Value _sv0t16;
  _sv0t16.tag = 0;
  _sv0t16.p0 = 3;
  int _sv0t17 = sv0_box_alloc(3);
  sv0_box_store(_sv0t17, 0, _sv0t16.tag);
  sv0_box_store(_sv0t17, 1, _sv0t16.p0);
  sv0_box_store(_sv0t17, 2, _sv0t16.p1);
  Value _sv0t18;
  _sv0t18.tag = 0;
  _sv0t18.p0 = 4;
  int _sv0t19 = sv0_box_alloc(3);
  sv0_box_store(_sv0t19, 0, _sv0t18.tag);
  sv0_box_store(_sv0t19, 1, _sv0t18.p0);
  sv0_box_store(_sv0t19, 2, _sv0t18.p1);
  binop.tag = 2;
  binop.p0 = 0;
  binop.p1 = _sv0t17;
  binop.p2 = _sv0t19;
  int _sv0t20 = sv0_vec_new();
  int _sv0t21 = sv0_vec_new();
  int _sv0t22 = emit_expr(binop, en, eb, ew, ec, _sv0t20, _sv0t21, pool, source, starts, ends, out2);
  int r2 = _sv0t22;
  if ((r2 != 5)) {
    return 4;
  } else {
  }
  int _sv0t23 = sv0_vec_get(out2, 0);
  if ((_sv0t23 != 4)) {
    return 5;
  } else {
  }
  int _sv0t24 = sv0_vec_get(out2, 1);
  if ((_sv0t24 != 3)) {
    return 6;
  } else {
  }
  int _sv0t25 = sv0_vec_get(out2, 2);
  if ((_sv0t25 != 4)) {
    return 7;
  } else {
  }
  int _sv0t26 = sv0_vec_get(out2, 3);
  if ((_sv0t26 != 4)) {
    return 8;
  } else {
  }
  int _sv0t27 = sv0_vec_get(out2, 4);
  if ((_sv0t27 != 16)) {
    return 9;
  } else {
  }
  int _sv0t28 = sv0_vec_new();
  int out3 = _sv0t28;
  Expr unop_not;
  Value _sv0t29;
  _sv0t29.tag = 1;
  int _sv0t30 = sv0_box_alloc(3);
  sv0_box_store(_sv0t30, 0, _sv0t29.tag);
  sv0_box_store(_sv0t30, 1, _sv0t29.p0);
  sv0_box_store(_sv0t30, 2, _sv0t29.p1);
  unop_not.tag = 3;
  unop_not.p0 = 1;
  unop_not.p1 = _sv0t30;
  int _sv0t31 = sv0_vec_new();
  int _sv0t32 = sv0_vec_new();
  int _sv0t33 = emit_expr(unop_not, en, eb, ew, ec, _sv0t31, _sv0t32, pool, source, starts, ends, out3);
  int r3 = _sv0t33;
  if ((r3 != 3)) {
    return 10;
  } else {
  }
  int _sv0t34 = sv0_vec_get(out3, 0);
  if ((_sv0t34 != 7)) {
    return 11;
  } else {
  }
  int _sv0t35 = sv0_vec_get(out3, 1);
  if ((_sv0t35 != 1)) {
    return 12;
  } else {
  }
  int _sv0t36 = sv0_vec_get(out3, 2);
  if ((_sv0t36 != 82)) {
    return 13;
  } else {
  }
  int _sv0t37 = sv0_vec_new();
  int out4 = _sv0t37;
  Expr unop_neg;
  Value _sv0t38;
  _sv0t38.tag = 0;
  _sv0t38.p0 = 5;
  int _sv0t39 = sv0_box_alloc(3);
  sv0_box_store(_sv0t39, 0, _sv0t38.tag);
  sv0_box_store(_sv0t39, 1, _sv0t38.p0);
  sv0_box_store(_sv0t39, 2, _sv0t38.p1);
  unop_neg.tag = 3;
  unop_neg.p0 = 0;
  unop_neg.p1 = _sv0t39;
  int _sv0t40 = sv0_vec_new();
  int _sv0t41 = sv0_vec_new();
  int _sv0t42 = emit_expr(unop_neg, en, eb, ew, ec, _sv0t40, _sv0t41, pool, source, starts, ends, out4);
  int r4 = _sv0t42;
  if ((r4 != 3)) {
    return 14;
  } else {
  }
  int _sv0t43 = sv0_vec_get(out4, 0);
  if ((_sv0t43 != 4)) {
    return 15;
  } else {
  }
  int _sv0t44 = sv0_vec_get(out4, 1);
  if ((_sv0t44 != 5)) {
    return 16;
  } else {
  }
  int _sv0t45 = sv0_vec_get(out4, 2);
  if ((_sv0t45 != 21)) {
    return 17;
  } else {
  }
  sv0_vec_push(en, 70);
  sv0_vec_push(eb, 0);
  sv0_vec_push(ew, 1);
  int _sv0t46 = sv0_vec_new();
  int out5 = _sv0t46;
  Expr load;
  load.tag = 1;
  load.p0 = 70;
  int _sv0t47 = sv0_vec_new();
  int _sv0t48 = sv0_vec_new();
  int _sv0t49 = emit_expr(load, en, eb, ew, ec, _sv0t47, _sv0t48, pool, source, starts, ends, out5);
  int r5 = _sv0t49;
  if ((r5 != 2)) {
    return 18;
  } else {
  }
  int _sv0t50 = sv0_vec_get(out5, 0);
  if ((_sv0t50 != 96)) {
    return 19;
  } else {
  }
  int _sv0t51 = sv0_vec_get(out5, 1);
  if ((_sv0t51 != 0)) {
    return 20;
  } else {
  }
  int _sv0t52 = sv0_vec_new();
  int out6 = _sv0t52;
  Expr unop_cast;
  Value _sv0t53;
  _sv0t53.tag = 0;
  _sv0t53.p0 = 9;
  int _sv0t54 = sv0_box_alloc(3);
  sv0_box_store(_sv0t54, 0, _sv0t53.tag);
  sv0_box_store(_sv0t54, 1, _sv0t53.p0);
  sv0_box_store(_sv0t54, 2, _sv0t53.p1);
  unop_cast.tag = 3;
  unop_cast.p0 = 99;
  unop_cast.p1 = _sv0t54;
  int _sv0t55 = sv0_vec_new();
  int _sv0t56 = sv0_vec_new();
  int _sv0t57 = emit_expr(unop_cast, en, eb, ew, ec, _sv0t55, _sv0t56, pool, source, starts, ends, out6);
  int r6 = _sv0t57;
  if ((r6 != 2)) {
    return 21;
  } else {
  }
  int _sv0t58 = sv0_vec_get(out6, 0);
  if ((_sv0t58 != 4)) {
    return 22;
  } else {
  }
  int _sv0t59 = sv0_vec_get(out6, 1);
  if ((_sv0t59 != 9)) {
    return 23;
  } else {
  }
  sv0_vec_push(en, 80);
  sv0_vec_push(eb, 4);
  sv0_vec_push(ew, 3);
  int _sv0t60 = sv0_vec_new();
  int out7 = _sv0t60;
  Expr f0;
  Value _sv0t61;
  _sv0t61.tag = 3;
  _sv0t61.p0 = 80;
  int _sv0t62 = sv0_box_alloc(3);
  sv0_box_store(_sv0t62, 0, _sv0t61.tag);
  sv0_box_store(_sv0t62, 1, _sv0t61.p0);
  sv0_box_store(_sv0t62, 2, _sv0t61.p1);
  f0.tag = 4;
  f0.p0 = _sv0t62;
  f0.p1 = 0;
  int _sv0t63 = sv0_vec_new();
  int _sv0t64 = sv0_vec_new();
  int _sv0t65 = emit_expr(f0, en, eb, ew, ec, _sv0t63, _sv0t64, pool, source, starts, ends, out7);
  int r7 = _sv0t65;
  if ((r7 != 2)) {
    return 24;
  } else {
  }
  int _sv0t66 = sv0_vec_get(out7, 0);
  if ((_sv0t66 != 96)) {
    return 25;
  } else {
  }
  int _sv0t67 = sv0_vec_get(out7, 1);
  if ((_sv0t67 != 4)) {
    return 26;
  } else {
  }
  int _sv0t68 = sv0_vec_new();
  int out8 = _sv0t68;
  Expr ftag;
  Value _sv0t69;
  _sv0t69.tag = 3;
  _sv0t69.p0 = 80;
  int _sv0t70 = sv0_box_alloc(3);
  sv0_box_store(_sv0t70, 0, _sv0t69.tag);
  sv0_box_store(_sv0t70, 1, _sv0t69.p0);
  sv0_box_store(_sv0t70, 2, _sv0t69.p1);
  int _sv0t71 = (0 - 1);
  ftag.tag = 4;
  ftag.p0 = _sv0t70;
  ftag.p1 = _sv0t71;
  int _sv0t72 = sv0_vec_new();
  int _sv0t73 = sv0_vec_new();
  int _sv0t74 = emit_expr(ftag, en, eb, ew, ec, _sv0t72, _sv0t73, pool, source, starts, ends, out8);
  int r8 = _sv0t74;
  if ((r8 != 2)) {
    return 27;
  } else {
  }
  int _sv0t75 = sv0_vec_get(out8, 0);
  if ((_sv0t75 != 96)) {
    return 28;
  } else {
  }
  int _sv0t76 = sv0_vec_get(out8, 1);
  if ((_sv0t76 != 4)) {
    return 29;
  } else {
  }
  int _sv0t77 = sv0_vec_new();
  int out9 = _sv0t77;
  Expr foob;
  Value _sv0t78;
  _sv0t78.tag = 3;
  _sv0t78.p0 = 80;
  int _sv0t79 = sv0_box_alloc(3);
  sv0_box_store(_sv0t79, 0, _sv0t78.tag);
  sv0_box_store(_sv0t79, 1, _sv0t78.p0);
  sv0_box_store(_sv0t79, 2, _sv0t78.p1);
  foob.tag = 4;
  foob.p0 = _sv0t79;
  foob.p1 = 3;
  int _sv0t80 = sv0_vec_new();
  int _sv0t81 = sv0_vec_new();
  int _sv0t82 = emit_expr(foob, en, eb, ew, ec, _sv0t80, _sv0t81, pool, source, starts, ends, out9);
  int r9 = _sv0t82;
  int _sv0t83 = (0 - 1);
  if ((r9 != _sv0t83)) {
    return 30;
  } else {
  }
  int _sv0t84 = sv0_vec_new();
  int enf = _sv0t84;
  int _sv0t85 = sv0_vec_new();
  int ebf = _sv0t85;
  int _sv0t86 = sv0_vec_new();
  int ewf = _sv0t86;
  int _sv0t87 = sv0_vec_new();
  int ecf = _sv0t87;
  sv0_vec_push(enf, 60);
  sv0_vec_push(ebf, 0);
  sv0_vec_push(ewf, 1);
  sv0_vec_push(ecf, 1);
  sv0_vec_push(enf, 61);
  sv0_vec_push(ebf, 1);
  sv0_vec_push(ewf, 1);
  sv0_vec_push(ecf, 1);
  int _sv0t88 = sv0_vec_new();
  int out10 = _sv0t88;
  Expr fadd;
  Value _sv0t89;
  _sv0t89.tag = 3;
  _sv0t89.p0 = 60;
  int _sv0t90 = sv0_box_alloc(3);
  sv0_box_store(_sv0t90, 0, _sv0t89.tag);
  sv0_box_store(_sv0t90, 1, _sv0t89.p0);
  sv0_box_store(_sv0t90, 2, _sv0t89.p1);
  Value _sv0t91;
  _sv0t91.tag = 3;
  _sv0t91.p0 = 61;
  int _sv0t92 = sv0_box_alloc(3);
  sv0_box_store(_sv0t92, 0, _sv0t91.tag);
  sv0_box_store(_sv0t92, 1, _sv0t91.p0);
  sv0_box_store(_sv0t92, 2, _sv0t91.p1);
  fadd.tag = 2;
  fadd.p0 = 0;
  fadd.p1 = _sv0t90;
  fadd.p2 = _sv0t92;
  int _sv0t93 = sv0_vec_new();
  int _sv0t94 = sv0_vec_new();
  int _sv0t95 = emit_expr(fadd, enf, ebf, ewf, ecf, _sv0t93, _sv0t94, pool, source, starts, ends, out10);
  int r10 = _sv0t95;
  if ((r10 != 5)) {
    return 31;
  } else {
  }
  int _sv0t96 = sv0_vec_get(out10, 4);
  if ((_sv0t96 != 48)) {
    return 32;
  } else {
  }
  int _sv0t97 = sv0_vec_new();
  int out11 = _sv0t97;
  Expr fneg;
  Value _sv0t98;
  _sv0t98.tag = 3;
  _sv0t98.p0 = 60;
  int _sv0t99 = sv0_box_alloc(3);
  sv0_box_store(_sv0t99, 0, _sv0t98.tag);
  sv0_box_store(_sv0t99, 1, _sv0t98.p0);
  sv0_box_store(_sv0t99, 2, _sv0t98.p1);
  fneg.tag = 3;
  fneg.p0 = 0;
  fneg.p1 = _sv0t99;
  int _sv0t100 = sv0_vec_new();
  int _sv0t101 = sv0_vec_new();
  int _sv0t102 = emit_expr(fneg, enf, ebf, ewf, ecf, _sv0t100, _sv0t101, pool, source, starts, ends, out11);
  int r11 = _sv0t102;
  if ((r11 != 3)) {
    return 33;
  } else {
  }
  int _sv0t103 = sv0_vec_get(out11, 2);
  if ((_sv0t103 != 52)) {
    return 34;
  } else {
  }
  return 0;
}

static int test_emit_instr(void) {
  int _sv0t0 = sv0_vec_new();
  int en = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int eb = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int ew = _sv0t2;
  int _sv0t3 = sv0_vec_new();
  int ec = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int efs = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int eff = _sv0t5;
  int _sv0t6 = sv0_vec_new();
  int pool = _sv0t6;
  int _sv0t7 = sv0_vec_new();
  int fns = _sv0t7;
  const char* source;
  source = "+-!";
  int _sv0t8 = sv0_vec_new();
  int starts = _sv0t8;
  int _sv0t9 = sv0_vec_new();
  int ends = _sv0t9;
  sv0_vec_push(starts, 0);
  sv0_vec_push(ends, 1);
  sv0_vec_push(starts, 1);
  sv0_vec_push(ends, 2);
  sv0_vec_push(starts, 2);
  sv0_vec_push(ends, 3);
  sv0_vec_push(en, 50);
  sv0_vec_push(eb, 0);
  sv0_vec_push(ew, 1);
  sv0_vec_push(efs, 0);
  int _sv0t10 = sv0_vec_new();
  int out1 = _sv0t10;
  Instr _sv0t11;
  _sv0t11.tag = 0;
  int _sv0t12 = emit_instr(_sv0t11, en, eb, ew, ec, efs, eff, pool, source, starts, ends, fns, out1);
  int r1 = _sv0t12;
  if ((r1 != 0)) {
    return 1;
  } else {
  }
  int _sv0t13 = sv0_vec_len(out1);
  if ((_sv0t13 != 0)) {
    return 2;
  } else {
  }
  int _sv0t14 = sv0_vec_new();
  int out2 = _sv0t14;
  Instr _sv0t15;
  _sv0t15.tag = 1;
  _sv0t15.p0 = 99;
  int _sv0t16 = emit_instr(_sv0t15, en, eb, ew, ec, efs, eff, pool, source, starts, ends, fns, out2);
  int r2 = _sv0t16;
  if ((r2 != 0)) {
    return 3;
  } else {
  }
  int _sv0t17 = sv0_vec_len(out2);
  if ((_sv0t17 != 0)) {
    return 4;
  } else {
  }
  int _sv0t18 = sv0_vec_new();
  int out3 = _sv0t18;
  Instr _sv0t19;
  _sv0t19.tag = 2;
  _sv0t19.p0 = 1;
  _sv0t19.p1 = 2;
  int _sv0t20 = emit_instr(_sv0t19, en, eb, ew, ec, efs, eff, pool, source, starts, ends, fns, out3);
  int r3 = _sv0t20;
  if ((r3 != 0)) {
    return 5;
  } else {
  }
  int _sv0t21 = sv0_vec_new();
  int out4 = _sv0t21;
  Expr asgn_e;
  Value _sv0t22;
  _sv0t22.tag = 0;
  _sv0t22.p0 = 42;
  int _sv0t23 = sv0_box_alloc(3);
  sv0_box_store(_sv0t23, 0, _sv0t22.tag);
  sv0_box_store(_sv0t23, 1, _sv0t22.p0);
  sv0_box_store(_sv0t23, 2, _sv0t22.p1);
  asgn_e.tag = 0;
  asgn_e.p0 = _sv0t23;
  Instr _sv0t24;
  int _sv0t25 = sv0_box_alloc(4);
  sv0_box_store(_sv0t25, 0, asgn_e.tag);
  sv0_box_store(_sv0t25, 1, asgn_e.p0);
  sv0_box_store(_sv0t25, 2, asgn_e.p1);
  sv0_box_store(_sv0t25, 3, asgn_e.p2);
  _sv0t24.tag = 3;
  _sv0t24.p0 = 50;
  _sv0t24.p1 = _sv0t25;
  int _sv0t26 = emit_instr(_sv0t24, en, eb, ew, ec, efs, eff, pool, source, starts, ends, fns, out4);
  int r4 = _sv0t26;
  if ((r4 != 4)) {
    return 6;
  } else {
  }
  int _sv0t27 = sv0_vec_get(out4, 0);
  if ((_sv0t27 != 4)) {
    return 7;
  } else {
  }
  int _sv0t28 = sv0_vec_get(out4, 1);
  if ((_sv0t28 != 42)) {
    return 8;
  } else {
  }
  int _sv0t29 = sv0_vec_get(out4, 2);
  if ((_sv0t29 != 97)) {
    return 9;
  } else {
  }
  int _sv0t30 = sv0_vec_get(out4, 3);
  if ((_sv0t30 != 0)) {
    return 10;
  } else {
  }
  int _sv0t31 = sv0_vec_new();
  int out5 = _sv0t31;
  Expr store_e;
  Value _sv0t32;
  _sv0t32.tag = 1;
  int _sv0t33 = sv0_box_alloc(3);
  sv0_box_store(_sv0t33, 0, _sv0t32.tag);
  sv0_box_store(_sv0t33, 1, _sv0t32.p0);
  sv0_box_store(_sv0t33, 2, _sv0t32.p1);
  store_e.tag = 0;
  store_e.p0 = _sv0t33;
  Instr _sv0t34;
  int _sv0t35 = sv0_box_alloc(4);
  sv0_box_store(_sv0t35, 0, store_e.tag);
  sv0_box_store(_sv0t35, 1, store_e.p0);
  sv0_box_store(_sv0t35, 2, store_e.p1);
  sv0_box_store(_sv0t35, 3, store_e.p2);
  _sv0t34.tag = 4;
  _sv0t34.p0 = 50;
  _sv0t34.p1 = _sv0t35;
  int _sv0t36 = emit_instr(_sv0t34, en, eb, ew, ec, efs, eff, pool, source, starts, ends, fns, out5);
  int r5 = _sv0t36;
  if ((r5 != 4)) {
    return 11;
  } else {
  }
  int _sv0t37 = sv0_vec_get(out5, 0);
  if ((_sv0t37 != 7)) {
    return 12;
  } else {
  }
  int _sv0t38 = sv0_vec_get(out5, 1);
  if ((_sv0t38 != 1)) {
    return 13;
  } else {
  }
  int _sv0t39 = sv0_vec_get(out5, 2);
  if ((_sv0t39 != 97)) {
    return 14;
  } else {
  }
  int _sv0t40 = sv0_vec_get(out5, 3);
  if ((_sv0t40 != 0)) {
    return 15;
  } else {
  }
  int _sv0t41 = sv0_vec_new();
  int out6 = _sv0t41;
  Instr _sv0t42;
  _sv0t42.tag = 14;
  int _sv0t43 = emit_instr(_sv0t42, en, eb, ew, ec, efs, eff, pool, source, starts, ends, fns, out6);
  int r6 = _sv0t43;
  if ((r6 != 2)) {
    return 16;
  } else {
  }
  int _sv0t44 = sv0_vec_get(out6, 0);
  if ((_sv0t44 != 118)) {
    return 17;
  } else {
  }
  int _sv0t45 = sv0_vec_get(out6, 1);
  if ((_sv0t45 != 0)) {
    return 18;
  } else {
  }
  int _sv0t46 = sv0_vec_new();
  int out7 = _sv0t46;
  Instr _sv0t47;
  Value _sv0t48;
  _sv0t48.tag = 0;
  _sv0t48.p0 = 7;
  int _sv0t49 = sv0_box_alloc(3);
  sv0_box_store(_sv0t49, 0, _sv0t48.tag);
  sv0_box_store(_sv0t49, 1, _sv0t48.p0);
  sv0_box_store(_sv0t49, 2, _sv0t48.p1);
  _sv0t47.tag = 15;
  _sv0t47.p0 = _sv0t49;
  int _sv0t50 = emit_instr(_sv0t47, en, eb, ew, ec, efs, eff, pool, source, starts, ends, fns, out7);
  int r7 = _sv0t50;
  if ((r7 != 4)) {
    return 19;
  } else {
  }
  int _sv0t51 = sv0_vec_get(out7, 0);
  if ((_sv0t51 != 4)) {
    return 20;
  } else {
  }
  int _sv0t52 = sv0_vec_get(out7, 1);
  if ((_sv0t52 != 7)) {
    return 21;
  } else {
  }
  int _sv0t53 = sv0_vec_get(out7, 2);
  if ((_sv0t53 != 118)) {
    return 22;
  } else {
  }
  int _sv0t54 = sv0_vec_get(out7, 3);
  if ((_sv0t54 != 1)) {
    return 23;
  } else {
  }
  int _sv0t55 = sv0_vec_new();
  int out8 = _sv0t55;
  Instr _sv0t56;
  Value _sv0t57;
  _sv0t57.tag = 3;
  _sv0t57.p0 = 50;
  int _sv0t58 = sv0_box_alloc(3);
  sv0_box_store(_sv0t58, 0, _sv0t57.tag);
  sv0_box_store(_sv0t58, 1, _sv0t57.p0);
  sv0_box_store(_sv0t58, 2, _sv0t57.p1);
  _sv0t56.tag = 15;
  _sv0t56.p0 = _sv0t58;
  int _sv0t59 = emit_instr(_sv0t56, en, eb, ew, ec, efs, eff, pool, source, starts, ends, fns, out8);
  int r8 = _sv0t59;
  if ((r8 != 4)) {
    return 24;
  } else {
  }
  int _sv0t60 = sv0_vec_get(out8, 0);
  if ((_sv0t60 != 96)) {
    return 25;
  } else {
  }
  int _sv0t61 = sv0_vec_get(out8, 1);
  if ((_sv0t61 != 0)) {
    return 26;
  } else {
  }
  int _sv0t62 = sv0_vec_get(out8, 2);
  if ((_sv0t62 != 118)) {
    return 27;
  } else {
  }
  int _sv0t63 = sv0_vec_get(out8, 3);
  if ((_sv0t63 != 1)) {
    return 28;
  } else {
  }
  sv0_vec_push(en, 60);
  sv0_vec_push(eb, 1);
  sv0_vec_push(ew, 3);
  sv0_vec_push(efs, 0);
  int _sv0t64 = sv0_vec_new();
  int out9 = _sv0t64;
  Expr asgn2_e;
  asgn2_e.tag = 1;
  asgn2_e.p0 = 60;
  Instr _sv0t65;
  int _sv0t66 = sv0_box_alloc(4);
  sv0_box_store(_sv0t66, 0, asgn2_e.tag);
  sv0_box_store(_sv0t66, 1, asgn2_e.p0);
  sv0_box_store(_sv0t66, 2, asgn2_e.p1);
  sv0_box_store(_sv0t66, 3, asgn2_e.p2);
  _sv0t65.tag = 3;
  _sv0t65.p0 = 60;
  _sv0t65.p1 = _sv0t66;
  int _sv0t67 = emit_instr(_sv0t65, en, eb, ew, ec, efs, eff, pool, source, starts, ends, fns, out9);
  int r9 = _sv0t67;
  if ((r9 != 12)) {
    return 29;
  } else {
  }
  int _sv0t68 = sv0_vec_get(out9, 0);
  if ((_sv0t68 != 96)) {
    return 30;
  } else {
  }
  int _sv0t69 = sv0_vec_get(out9, 1);
  if ((_sv0t69 != 1)) {
    return 31;
  } else {
  }
  int _sv0t70 = sv0_vec_get(out9, 2);
  if ((_sv0t70 != 96)) {
    return 32;
  } else {
  }
  int _sv0t71 = sv0_vec_get(out9, 3);
  if ((_sv0t71 != 2)) {
    return 33;
  } else {
  }
  int _sv0t72 = sv0_vec_get(out9, 4);
  if ((_sv0t72 != 96)) {
    return 34;
  } else {
  }
  int _sv0t73 = sv0_vec_get(out9, 5);
  if ((_sv0t73 != 3)) {
    return 35;
  } else {
  }
  int _sv0t74 = sv0_vec_get(out9, 6);
  if ((_sv0t74 != 97)) {
    return 36;
  } else {
  }
  int _sv0t75 = sv0_vec_get(out9, 7);
  if ((_sv0t75 != 3)) {
    return 37;
  } else {
  }
  int _sv0t76 = sv0_vec_get(out9, 8);
  if ((_sv0t76 != 97)) {
    return 38;
  } else {
  }
  int _sv0t77 = sv0_vec_get(out9, 9);
  if ((_sv0t77 != 2)) {
    return 39;
  } else {
  }
  int _sv0t78 = sv0_vec_get(out9, 10);
  if ((_sv0t78 != 97)) {
    return 40;
  } else {
  }
  int _sv0t79 = sv0_vec_get(out9, 11);
  if ((_sv0t79 != 1)) {
    return 41;
  } else {
  }
  int _sv0t80 = sv0_vec_new();
  int out10 = _sv0t80;
  Instr _sv0t81;
  Value _sv0t82;
  _sv0t82.tag = 3;
  _sv0t82.p0 = 60;
  int _sv0t83 = sv0_box_alloc(3);
  sv0_box_store(_sv0t83, 0, _sv0t82.tag);
  sv0_box_store(_sv0t83, 1, _sv0t82.p0);
  sv0_box_store(_sv0t83, 2, _sv0t82.p1);
  _sv0t81.tag = 15;
  _sv0t81.p0 = _sv0t83;
  int _sv0t84 = emit_instr(_sv0t81, en, eb, ew, ec, efs, eff, pool, source, starts, ends, fns, out10);
  int r10 = _sv0t84;
  if ((r10 != 8)) {
    return 42;
  } else {
  }
  int _sv0t85 = sv0_vec_get(out10, 6);
  if ((_sv0t85 != 118)) {
    return 43;
  } else {
  }
  int _sv0t86 = sv0_vec_get(out10, 7);
  if ((_sv0t86 != 3)) {
    return 44;
  } else {
  }
  return 0;
}

static int test_emit_instrs(void) {
  int _sv0t0 = sv0_vec_new();
  int en = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int eb = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int ew = _sv0t2;
  int _sv0t3 = sv0_vec_new();
  int ec = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int efs = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int eff = _sv0t5;
  int _sv0t6 = sv0_vec_new();
  int pool = _sv0t6;
  int _sv0t7 = sv0_vec_new();
  int fns = _sv0t7;
  const char* source;
  source = "+";
  int _sv0t8 = sv0_vec_new();
  int starts = _sv0t8;
  int _sv0t9 = sv0_vec_new();
  int ends = _sv0t9;
  sv0_vec_push(starts, 0);
  sv0_vec_push(ends, 1);
  int _sv0t10 = sv0_vec_new();
  int empty = _sv0t10;
  int _sv0t11 = sv0_vec_new();
  int out1 = _sv0t11;
  int _sv0t12 = emit_instrs(empty, en, eb, ew, ec, efs, eff, pool, source, starts, ends, fns, out1);
  int r1 = _sv0t12;
  if ((r1 != 0)) {
    return 1;
  } else {
  }
  int _sv0t13 = sv0_vec_len(out1);
  if ((_sv0t13 != 0)) {
    return 2;
  } else {
  }
  int _sv0t14 = sv0_vec_new();
  int instrs = _sv0t14;
  Instr _sv0t15;
  _sv0t15.tag = 0;
  int _sv0t16 = sv0_box_alloc(5);
  sv0_box_store(_sv0t16, 0, _sv0t15.tag);
  sv0_box_store(_sv0t16, 1, _sv0t15.p0);
  sv0_box_store(_sv0t16, 2, _sv0t15.p1);
  sv0_box_store(_sv0t16, 3, _sv0t15.p2);
  sv0_box_store(_sv0t16, 4, _sv0t15.p3);
  sv0_vec_push(instrs, _sv0t16);
  Instr _sv0t17;
  _sv0t17.tag = 14;
  int _sv0t18 = sv0_box_alloc(5);
  sv0_box_store(_sv0t18, 0, _sv0t17.tag);
  sv0_box_store(_sv0t18, 1, _sv0t17.p0);
  sv0_box_store(_sv0t18, 2, _sv0t17.p1);
  sv0_box_store(_sv0t18, 3, _sv0t17.p2);
  sv0_box_store(_sv0t18, 4, _sv0t17.p3);
  sv0_vec_push(instrs, _sv0t18);
  int _sv0t19 = sv0_vec_new();
  int out2 = _sv0t19;
  int _sv0t20 = emit_instrs(instrs, en, eb, ew, ec, efs, eff, pool, source, starts, ends, fns, out2);
  int r2 = _sv0t20;
  if ((r2 != 2)) {
    return 3;
  } else {
  }
  int _sv0t21 = sv0_vec_len(out2);
  if ((_sv0t21 != 2)) {
    return 4;
  } else {
  }
  int _sv0t22 = sv0_vec_get(out2, 0);
  if ((_sv0t22 != 118)) {
    return 5;
  } else {
  }
  int _sv0t23 = sv0_vec_get(out2, 1);
  if ((_sv0t23 != 0)) {
    return 6;
  } else {
  }
  sv0_vec_push(en, 70);
  sv0_vec_push(eb, 0);
  sv0_vec_push(ew, 1);
  sv0_vec_push(efs, 0);
  int _sv0t24 = sv0_vec_new();
  int instrs2 = _sv0t24;
  Expr a_e;
  Value _sv0t25;
  _sv0t25.tag = 0;
  _sv0t25.p0 = 99;
  int _sv0t26 = sv0_box_alloc(3);
  sv0_box_store(_sv0t26, 0, _sv0t25.tag);
  sv0_box_store(_sv0t26, 1, _sv0t25.p0);
  sv0_box_store(_sv0t26, 2, _sv0t25.p1);
  a_e.tag = 0;
  a_e.p0 = _sv0t26;
  Instr _sv0t27;
  int _sv0t28 = sv0_box_alloc(4);
  sv0_box_store(_sv0t28, 0, a_e.tag);
  sv0_box_store(_sv0t28, 1, a_e.p0);
  sv0_box_store(_sv0t28, 2, a_e.p1);
  sv0_box_store(_sv0t28, 3, a_e.p2);
  _sv0t27.tag = 3;
  _sv0t27.p0 = 70;
  _sv0t27.p1 = _sv0t28;
  int _sv0t29 = sv0_box_alloc(5);
  sv0_box_store(_sv0t29, 0, _sv0t27.tag);
  sv0_box_store(_sv0t29, 1, _sv0t27.p0);
  sv0_box_store(_sv0t29, 2, _sv0t27.p1);
  sv0_box_store(_sv0t29, 3, _sv0t27.p2);
  sv0_box_store(_sv0t29, 4, _sv0t27.p3);
  sv0_vec_push(instrs2, _sv0t29);
  Instr _sv0t30;
  Value _sv0t31;
  _sv0t31.tag = 3;
  _sv0t31.p0 = 70;
  int _sv0t32 = sv0_box_alloc(3);
  sv0_box_store(_sv0t32, 0, _sv0t31.tag);
  sv0_box_store(_sv0t32, 1, _sv0t31.p0);
  sv0_box_store(_sv0t32, 2, _sv0t31.p1);
  _sv0t30.tag = 15;
  _sv0t30.p0 = _sv0t32;
  int _sv0t33 = sv0_box_alloc(5);
  sv0_box_store(_sv0t33, 0, _sv0t30.tag);
  sv0_box_store(_sv0t33, 1, _sv0t30.p0);
  sv0_box_store(_sv0t33, 2, _sv0t30.p1);
  sv0_box_store(_sv0t33, 3, _sv0t30.p2);
  sv0_box_store(_sv0t33, 4, _sv0t30.p3);
  sv0_vec_push(instrs2, _sv0t33);
  int _sv0t34 = sv0_vec_new();
  int out3 = _sv0t34;
  int _sv0t35 = emit_instrs(instrs2, en, eb, ew, ec, efs, eff, pool, source, starts, ends, fns, out3);
  int r3 = _sv0t35;
  if ((r3 != 8)) {
    return 7;
  } else {
  }
  int _sv0t36 = sv0_vec_get(out3, 0);
  if ((_sv0t36 != 4)) {
    return 8;
  } else {
  }
  int _sv0t37 = sv0_vec_get(out3, 1);
  if ((_sv0t37 != 99)) {
    return 9;
  } else {
  }
  int _sv0t38 = sv0_vec_get(out3, 2);
  if ((_sv0t38 != 97)) {
    return 10;
  } else {
  }
  int _sv0t39 = sv0_vec_get(out3, 3);
  if ((_sv0t39 != 0)) {
    return 11;
  } else {
  }
  int _sv0t40 = sv0_vec_get(out3, 4);
  if ((_sv0t40 != 96)) {
    return 12;
  } else {
  }
  int _sv0t41 = sv0_vec_get(out3, 5);
  if ((_sv0t41 != 0)) {
    return 13;
  } else {
  }
  int _sv0t42 = sv0_vec_get(out3, 6);
  if ((_sv0t42 != 118)) {
    return 14;
  } else {
  }
  int _sv0t43 = sv0_vec_get(out3, 7);
  if ((_sv0t43 != 1)) {
    return 15;
  } else {
  }
  return 0;
}

static int test_vec_append(void) {
  int _sv0t0 = sv0_vec_new();
  int dst = _sv0t0;
  sv0_vec_push(dst, 1);
  int _sv0t1 = sv0_vec_new();
  int src = _sv0t1;
  sv0_vec_push(src, 2);
  sv0_vec_push(src, 3);
  int _sv0t2 = vec_append(dst, src);
  int n = _sv0t2;
  if ((n != 2)) {
    return 1;
  } else {
  }
  int _sv0t3 = sv0_vec_len(dst);
  if ((_sv0t3 != 3)) {
    return 2;
  } else {
  }
  int _sv0t4 = sv0_vec_get(dst, 0);
  if ((_sv0t4 != 1)) {
    return 3;
  } else {
  }
  int _sv0t5 = sv0_vec_get(dst, 1);
  if ((_sv0t5 != 2)) {
    return 4;
  } else {
  }
  int _sv0t6 = sv0_vec_get(dst, 2);
  if ((_sv0t6 != 3)) {
    return 5;
  } else {
  }
  int _sv0t7 = sv0_vec_new();
  int e = _sv0t7;
  int _sv0t8 = vec_append(dst, e);
  int n2 = _sv0t8;
  if ((n2 != 0)) {
    return 6;
  } else {
  }
  int _sv0t9 = sv0_vec_len(dst);
  if ((_sv0t9 != 3)) {
    return 7;
  } else {
  }
  return 0;
}

static int test_flat_byte_size(void) {
  int _sv0t0 = sv0_vec_new();
  int v1 = _sv0t0;
  int _sv0t1 = flat_byte_size(v1);
  if ((_sv0t1 != 0)) {
    return 1;
  } else {
  }
  int _sv0t2 = sv0_vec_new();
  int v2 = _sv0t2;
  sv0_vec_push(v2, 118);
  sv0_vec_push(v2, 0);
  int _sv0t3 = flat_byte_size(v2);
  if ((_sv0t3 != 2)) {
    return 2;
  } else {
  }
  int _sv0t4 = sv0_vec_new();
  int v3 = _sv0t4;
  sv0_vec_push(v3, 4);
  sv0_vec_push(v3, 42);
  int _sv0t5 = flat_byte_size(v3);
  if ((_sv0t5 != 5)) {
    return 3;
  } else {
  }
  int _sv0t6 = sv0_vec_new();
  int v4 = _sv0t6;
  sv0_vec_push(v4, 4);
  sv0_vec_push(v4, 42);
  sv0_vec_push(v4, 97);
  sv0_vec_push(v4, 0);
  sv0_vec_push(v4, 118);
  sv0_vec_push(v4, 0);
  int _sv0t7 = flat_byte_size(v4);
  if ((_sv0t7 != 12)) {
    return 4;
  } else {
  }
  int _sv0t8 = sv0_vec_new();
  int v5 = _sv0t8;
  sv0_vec_push(v5, 7);
  sv0_vec_push(v5, 1);
  int _sv0t9 = flat_byte_size(v5);
  if ((_sv0t9 != 2)) {
    return 5;
  } else {
  }
  int _sv0t10 = sv0_vec_new();
  int v6 = _sv0t10;
  sv0_vec_push(v6, 3);
  int _sv0t11 = flat_byte_size(v6);
  if ((_sv0t11 != 1)) {
    return 6;
  } else {
  }
  return 0;
}

static int test_emit_ifelse_block(void) {
  int _sv0t0 = sv0_vec_new();
  int en = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int eb = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int ew = _sv0t2;
  int _sv0t3 = sv0_vec_new();
  int ec = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int efs = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int eff = _sv0t5;
  int _sv0t6 = sv0_vec_new();
  int pool = _sv0t6;
  int _sv0t7 = sv0_vec_new();
  int fns = _sv0t7;
  const char* source;
  source = "+";
  int _sv0t8 = sv0_vec_new();
  int starts = _sv0t8;
  int _sv0t9 = sv0_vec_new();
  int ends = _sv0t9;
  sv0_vec_push(starts, 0);
  sv0_vec_push(ends, 1);
  sv0_vec_push(en, 50);
  sv0_vec_push(eb, 0);
  sv0_vec_push(ew, 1);
  sv0_vec_push(efs, 0);
  Expr _sv0t10;
  Value _sv0t11;
  _sv0t11.tag = 1;
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
  int cond1 = _sv0t13;
  int _sv0t14 = sv0_vec_new();
  int th1 = _sv0t14;
  Instr _sv0t15;
  _sv0t15.tag = 14;
  int _sv0t16 = sv0_box_alloc(5);
  sv0_box_store(_sv0t16, 0, _sv0t15.tag);
  sv0_box_store(_sv0t16, 1, _sv0t15.p0);
  sv0_box_store(_sv0t16, 2, _sv0t15.p1);
  sv0_box_store(_sv0t16, 3, _sv0t15.p2);
  sv0_box_store(_sv0t16, 4, _sv0t15.p3);
  sv0_vec_push(th1, _sv0t16);
  int _sv0t17 = sv0_vec_new();
  int el1 = _sv0t17;
  Instr _sv0t18;
  _sv0t18.tag = 14;
  int _sv0t19 = sv0_box_alloc(5);
  sv0_box_store(_sv0t19, 0, _sv0t18.tag);
  sv0_box_store(_sv0t19, 1, _sv0t18.p0);
  sv0_box_store(_sv0t19, 2, _sv0t18.p1);
  sv0_box_store(_sv0t19, 3, _sv0t18.p2);
  sv0_box_store(_sv0t19, 4, _sv0t18.p3);
  sv0_vec_push(el1, _sv0t19);
  int _sv0t20 = sv0_vec_new();
  int out1 = _sv0t20;
  Instr _sv0t21;
  _sv0t21.tag = 6;
  _sv0t21.p0 = cond1;
  _sv0t21.p1 = th1;
  _sv0t21.p2 = el1;
  int _sv0t22 = emit_instr(_sv0t21, en, eb, ew, ec, efs, eff, pool, source, starts, ends, fns, out1);
  int r1 = _sv0t22;
  if ((r1 < 0)) {
    return 1;
  } else {
  }
  int _sv0t23 = sv0_vec_len(out1);
  if ((_sv0t23 != 10)) {
    return 2;
  } else {
  }
  int _sv0t24 = sv0_vec_get(out1, 0);
  if ((_sv0t24 != 7)) {
    return 3;
  } else {
  }
  int _sv0t25 = sv0_vec_get(out1, 1);
  if ((_sv0t25 != 1)) {
    return 4;
  } else {
  }
  int _sv0t26 = sv0_vec_get(out1, 2);
  if ((_sv0t26 != 114)) {
    return 5;
  } else {
  }
  int _sv0t27 = sv0_vec_get(out1, 3);
  if ((_sv0t27 != 7)) {
    return 6;
  } else {
  }
  int _sv0t28 = sv0_vec_get(out1, 4);
  if ((_sv0t28 != 118)) {
    return 7;
  } else {
  }
  int _sv0t29 = sv0_vec_get(out1, 5);
  if ((_sv0t29 != 0)) {
    return 8;
  } else {
  }
  int _sv0t30 = sv0_vec_get(out1, 6);
  if ((_sv0t30 != 112)) {
    return 9;
  } else {
  }
  int _sv0t31 = sv0_vec_get(out1, 7);
  if ((_sv0t31 != 2)) {
    return 10;
  } else {
  }
  int _sv0t32 = sv0_vec_get(out1, 8);
  if ((_sv0t32 != 118)) {
    return 11;
  } else {
  }
  int _sv0t33 = sv0_vec_get(out1, 9);
  if ((_sv0t33 != 0)) {
    return 12;
  } else {
  }
  Expr _sv0t34;
  Value _sv0t35;
  _sv0t35.tag = 0;
  _sv0t35.p0 = 1;
  int _sv0t36 = sv0_box_alloc(3);
  sv0_box_store(_sv0t36, 0, _sv0t35.tag);
  sv0_box_store(_sv0t36, 1, _sv0t35.p0);
  sv0_box_store(_sv0t36, 2, _sv0t35.p1);
  _sv0t34.tag = 0;
  _sv0t34.p0 = _sv0t36;
  int _sv0t37 = sv0_box_alloc(4);
  sv0_box_store(_sv0t37, 0, _sv0t34.tag);
  sv0_box_store(_sv0t37, 1, _sv0t34.p0);
  sv0_box_store(_sv0t37, 2, _sv0t34.p1);
  sv0_box_store(_sv0t37, 3, _sv0t34.p2);
  int cond2 = _sv0t37;
  int _sv0t38 = sv0_vec_new();
  int th2 = _sv0t38;
  Expr asgn_e;
  Value _sv0t39;
  _sv0t39.tag = 0;
  _sv0t39.p0 = 42;
  int _sv0t40 = sv0_box_alloc(3);
  sv0_box_store(_sv0t40, 0, _sv0t39.tag);
  sv0_box_store(_sv0t40, 1, _sv0t39.p0);
  sv0_box_store(_sv0t40, 2, _sv0t39.p1);
  asgn_e.tag = 0;
  asgn_e.p0 = _sv0t40;
  Instr _sv0t41;
  int _sv0t42 = sv0_box_alloc(4);
  sv0_box_store(_sv0t42, 0, asgn_e.tag);
  sv0_box_store(_sv0t42, 1, asgn_e.p0);
  sv0_box_store(_sv0t42, 2, asgn_e.p1);
  sv0_box_store(_sv0t42, 3, asgn_e.p2);
  _sv0t41.tag = 3;
  _sv0t41.p0 = 50;
  _sv0t41.p1 = _sv0t42;
  int _sv0t43 = sv0_box_alloc(5);
  sv0_box_store(_sv0t43, 0, _sv0t41.tag);
  sv0_box_store(_sv0t43, 1, _sv0t41.p0);
  sv0_box_store(_sv0t43, 2, _sv0t41.p1);
  sv0_box_store(_sv0t43, 3, _sv0t41.p2);
  sv0_box_store(_sv0t43, 4, _sv0t41.p3);
  sv0_vec_push(th2, _sv0t43);
  Instr _sv0t44;
  _sv0t44.tag = 14;
  int _sv0t45 = sv0_box_alloc(5);
  sv0_box_store(_sv0t45, 0, _sv0t44.tag);
  sv0_box_store(_sv0t45, 1, _sv0t44.p0);
  sv0_box_store(_sv0t45, 2, _sv0t44.p1);
  sv0_box_store(_sv0t45, 3, _sv0t44.p2);
  sv0_box_store(_sv0t45, 4, _sv0t44.p3);
  sv0_vec_push(th2, _sv0t45);
  int _sv0t46 = sv0_vec_new();
  int el2 = _sv0t46;
  Instr _sv0t47;
  _sv0t47.tag = 14;
  int _sv0t48 = sv0_box_alloc(5);
  sv0_box_store(_sv0t48, 0, _sv0t47.tag);
  sv0_box_store(_sv0t48, 1, _sv0t47.p0);
  sv0_box_store(_sv0t48, 2, _sv0t47.p1);
  sv0_box_store(_sv0t48, 3, _sv0t47.p2);
  sv0_box_store(_sv0t48, 4, _sv0t47.p3);
  sv0_vec_push(el2, _sv0t48);
  int _sv0t49 = sv0_vec_new();
  int out2 = _sv0t49;
  Instr _sv0t50;
  _sv0t50.tag = 6;
  _sv0t50.p0 = cond2;
  _sv0t50.p1 = th2;
  _sv0t50.p2 = el2;
  int _sv0t51 = emit_instr(_sv0t50, en, eb, ew, ec, efs, eff, pool, source, starts, ends, fns, out2);
  int r2 = _sv0t51;
  if ((r2 < 0)) {
    return 13;
  } else {
  }
  int _sv0t52 = sv0_vec_len(out2);
  if ((_sv0t52 != 14)) {
    return 14;
  } else {
  }
  int _sv0t53 = sv0_vec_get(out2, 0);
  if ((_sv0t53 != 4)) {
    return 15;
  } else {
  }
  int _sv0t54 = sv0_vec_get(out2, 1);
  if ((_sv0t54 != 1)) {
    return 16;
  } else {
  }
  int _sv0t55 = sv0_vec_get(out2, 2);
  if ((_sv0t55 != 114)) {
    return 17;
  } else {
  }
  int _sv0t56 = sv0_vec_get(out2, 3);
  if ((_sv0t56 != 17)) {
    return 18;
  } else {
  }
  int _sv0t57 = sv0_vec_get(out2, 4);
  if ((_sv0t57 != 4)) {
    return 19;
  } else {
  }
  int _sv0t58 = sv0_vec_get(out2, 5);
  if ((_sv0t58 != 42)) {
    return 20;
  } else {
  }
  int _sv0t59 = sv0_vec_get(out2, 6);
  if ((_sv0t59 != 97)) {
    return 21;
  } else {
  }
  int _sv0t60 = sv0_vec_get(out2, 7);
  if ((_sv0t60 != 0)) {
    return 22;
  } else {
  }
  int _sv0t61 = sv0_vec_get(out2, 8);
  if ((_sv0t61 != 118)) {
    return 23;
  } else {
  }
  int _sv0t62 = sv0_vec_get(out2, 9);
  if ((_sv0t62 != 0)) {
    return 24;
  } else {
  }
  int _sv0t63 = sv0_vec_get(out2, 10);
  if ((_sv0t63 != 112)) {
    return 25;
  } else {
  }
  int _sv0t64 = sv0_vec_get(out2, 11);
  if ((_sv0t64 != 2)) {
    return 26;
  } else {
  }
  int _sv0t65 = sv0_vec_get(out2, 12);
  if ((_sv0t65 != 118)) {
    return 27;
  } else {
  }
  int _sv0t66 = sv0_vec_get(out2, 13);
  if ((_sv0t66 != 0)) {
    return 28;
  } else {
  }
  int _sv0t67 = sv0_vec_new();
  int blk_body = _sv0t67;
  Instr _sv0t68;
  _sv0t68.tag = 0;
  int _sv0t69 = sv0_box_alloc(5);
  sv0_box_store(_sv0t69, 0, _sv0t68.tag);
  sv0_box_store(_sv0t69, 1, _sv0t68.p0);
  sv0_box_store(_sv0t69, 2, _sv0t68.p1);
  sv0_box_store(_sv0t69, 3, _sv0t68.p2);
  sv0_box_store(_sv0t69, 4, _sv0t68.p3);
  sv0_vec_push(blk_body, _sv0t69);
  Instr _sv0t70;
  _sv0t70.tag = 14;
  int _sv0t71 = sv0_box_alloc(5);
  sv0_box_store(_sv0t71, 0, _sv0t70.tag);
  sv0_box_store(_sv0t71, 1, _sv0t70.p0);
  sv0_box_store(_sv0t71, 2, _sv0t70.p1);
  sv0_box_store(_sv0t71, 3, _sv0t70.p2);
  sv0_box_store(_sv0t71, 4, _sv0t70.p3);
  sv0_vec_push(blk_body, _sv0t71);
  int _sv0t72 = sv0_vec_new();
  int out3 = _sv0t72;
  Instr _sv0t73;
  _sv0t73.tag = 8;
  _sv0t73.p0 = blk_body;
  int _sv0t74 = emit_instr(_sv0t73, en, eb, ew, ec, efs, eff, pool, source, starts, ends, fns, out3);
  int r3 = _sv0t74;
  if ((r3 != 2)) {
    return 29;
  } else {
  }
  int _sv0t75 = sv0_vec_get(out3, 0);
  if ((_sv0t75 != 118)) {
    return 30;
  } else {
  }
  int _sv0t76 = sv0_vec_get(out3, 1);
  if ((_sv0t76 != 0)) {
    return 31;
  } else {
  }
  int _sv0t77 = sv0_vec_new();
  int empty_blk = _sv0t77;
  int _sv0t78 = sv0_vec_new();
  int out4 = _sv0t78;
  Instr _sv0t79;
  _sv0t79.tag = 8;
  _sv0t79.p0 = empty_blk;
  int _sv0t80 = emit_instr(_sv0t79, en, eb, ew, ec, efs, eff, pool, source, starts, ends, fns, out4);
  int r4 = _sv0t80;
  if ((r4 != 0)) {
    return 32;
  } else {
  }
  int _sv0t81 = sv0_vec_len(out4);
  if ((_sv0t81 != 0)) {
    return 33;
  } else {
  }
  return 0;
}

static int test_emit_while_loop(void) {
  int _sv0t0 = sv0_vec_new();
  int en = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int eb = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int ew = _sv0t2;
  int _sv0t3 = sv0_vec_new();
  int ec = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int efs = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int eff = _sv0t5;
  int _sv0t6 = sv0_vec_new();
  int pool = _sv0t6;
  int _sv0t7 = sv0_vec_new();
  int fns = _sv0t7;
  const char* source;
  source = "+";
  int _sv0t8 = sv0_vec_new();
  int starts = _sv0t8;
  int _sv0t9 = sv0_vec_new();
  int ends = _sv0t9;
  sv0_vec_push(starts, 0);
  sv0_vec_push(ends, 1);
  sv0_vec_push(en, 50);
  sv0_vec_push(eb, 0);
  sv0_vec_push(ew, 1);
  sv0_vec_push(efs, 0);
  Expr _sv0t10;
  Value _sv0t11;
  _sv0t11.tag = 1;
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
  int cond1 = _sv0t13;
  int _sv0t14 = sv0_vec_new();
  int body1 = _sv0t14;
  Instr _sv0t15;
  _sv0t15.tag = 14;
  int _sv0t16 = sv0_box_alloc(5);
  sv0_box_store(_sv0t16, 0, _sv0t15.tag);
  sv0_box_store(_sv0t16, 1, _sv0t15.p0);
  sv0_box_store(_sv0t16, 2, _sv0t15.p1);
  sv0_box_store(_sv0t16, 3, _sv0t15.p2);
  sv0_box_store(_sv0t16, 4, _sv0t15.p3);
  sv0_vec_push(body1, _sv0t16);
  int _sv0t17 = sv0_vec_new();
  int out1 = _sv0t17;
  Instr _sv0t18;
  _sv0t18.tag = 7;
  _sv0t18.p0 = cond1;
  _sv0t18.p1 = body1;
  int _sv0t19 = emit_instr(_sv0t18, en, eb, ew, ec, efs, eff, pool, source, starts, ends, fns, out1);
  int r1 = _sv0t19;
  if ((r1 < 0)) {
    return 1;
  } else {
  }
  int _sv0t20 = sv0_vec_len(out1);
  if ((_sv0t20 != 8)) {
    return 2;
  } else {
  }
  int _sv0t21 = sv0_vec_get(out1, 0);
  if ((_sv0t21 != 7)) {
    return 3;
  } else {
  }
  int _sv0t22 = sv0_vec_get(out1, 1);
  if ((_sv0t22 != 1)) {
    return 4;
  } else {
  }
  int _sv0t23 = sv0_vec_get(out1, 2);
  if ((_sv0t23 != 114)) {
    return 5;
  } else {
  }
  int _sv0t24 = sv0_vec_get(out1, 3);
  if ((_sv0t24 != 7)) {
    return 6;
  } else {
  }
  int _sv0t25 = sv0_vec_get(out1, 4);
  if ((_sv0t25 != 118)) {
    return 7;
  } else {
  }
  int _sv0t26 = sv0_vec_get(out1, 5);
  if ((_sv0t26 != 0)) {
    return 8;
  } else {
  }
  int _sv0t27 = sv0_vec_get(out1, 6);
  if ((_sv0t27 != 112)) {
    return 9;
  } else {
  }
  int _sv0t28 = sv0_vec_get(out1, 7);
  int _sv0t29 = (0 - 14);
  if ((_sv0t28 != _sv0t29)) {
    return 10;
  } else {
  }
  Expr _sv0t30;
  Value _sv0t31;
  _sv0t31.tag = 1;
  int _sv0t32 = sv0_box_alloc(3);
  sv0_box_store(_sv0t32, 0, _sv0t31.tag);
  sv0_box_store(_sv0t32, 1, _sv0t31.p0);
  sv0_box_store(_sv0t32, 2, _sv0t31.p1);
  _sv0t30.tag = 0;
  _sv0t30.p0 = _sv0t32;
  int _sv0t33 = sv0_box_alloc(4);
  sv0_box_store(_sv0t33, 0, _sv0t30.tag);
  sv0_box_store(_sv0t33, 1, _sv0t30.p0);
  sv0_box_store(_sv0t33, 2, _sv0t30.p1);
  sv0_box_store(_sv0t33, 3, _sv0t30.p2);
  int cond2 = _sv0t33;
  int _sv0t34 = sv0_vec_new();
  int body2 = _sv0t34;
  int _sv0t35 = sv0_vec_new();
  int out2 = _sv0t35;
  Instr _sv0t36;
  _sv0t36.tag = 7;
  _sv0t36.p0 = cond2;
  _sv0t36.p1 = body2;
  int _sv0t37 = emit_instr(_sv0t36, en, eb, ew, ec, efs, eff, pool, source, starts, ends, fns, out2);
  int r2 = _sv0t37;
  if ((r2 < 0)) {
    return 11;
  } else {
  }
  int _sv0t38 = sv0_vec_len(out2);
  if ((_sv0t38 != 6)) {
    return 12;
  } else {
  }
  int _sv0t39 = sv0_vec_get(out2, 0);
  if ((_sv0t39 != 7)) {
    return 13;
  } else {
  }
  int _sv0t40 = sv0_vec_get(out2, 1);
  if ((_sv0t40 != 1)) {
    return 14;
  } else {
  }
  int _sv0t41 = sv0_vec_get(out2, 2);
  if ((_sv0t41 != 114)) {
    return 15;
  } else {
  }
  int _sv0t42 = sv0_vec_get(out2, 3);
  if ((_sv0t42 != 5)) {
    return 16;
  } else {
  }
  int _sv0t43 = sv0_vec_get(out2, 4);
  if ((_sv0t43 != 112)) {
    return 17;
  } else {
  }
  int _sv0t44 = sv0_vec_get(out2, 5);
  int _sv0t45 = (0 - 12);
  if ((_sv0t44 != _sv0t45)) {
    return 18;
  } else {
  }
  Expr _sv0t46;
  Value _sv0t47;
  _sv0t47.tag = 1;
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
  int cond3 = _sv0t49;
  int _sv0t50 = sv0_vec_new();
  int body3 = _sv0t50;
  Instr _sv0t51;
  _sv0t51.tag = 9;
  int _sv0t52 = sv0_box_alloc(5);
  sv0_box_store(_sv0t52, 0, _sv0t51.tag);
  sv0_box_store(_sv0t52, 1, _sv0t51.p0);
  sv0_box_store(_sv0t52, 2, _sv0t51.p1);
  sv0_box_store(_sv0t52, 3, _sv0t51.p2);
  sv0_box_store(_sv0t52, 4, _sv0t51.p3);
  sv0_vec_push(body3, _sv0t52);
  int _sv0t53 = sv0_vec_new();
  int out3 = _sv0t53;
  Instr _sv0t54;
  _sv0t54.tag = 7;
  _sv0t54.p0 = cond3;
  _sv0t54.p1 = body3;
  int _sv0t55 = emit_instr(_sv0t54, en, eb, ew, ec, efs, eff, pool, source, starts, ends, fns, out3);
  int r3 = _sv0t55;
  if ((r3 < 0)) {
    return 19;
  } else {
  }
  int _sv0t56 = sv0_vec_len(out3);
  if ((_sv0t56 != 8)) {
    return 20;
  } else {
  }
  int _sv0t57 = sv0_vec_get(out3, 4);
  if ((_sv0t57 != 112)) {
    return 21;
  } else {
  }
  int _sv0t58 = sv0_vec_get(out3, 5);
  if ((_sv0t58 != 5)) {
    return 22;
  } else {
  }
  int _sv0t59 = sv0_vec_get(out3, 6);
  if ((_sv0t59 != 112)) {
    return 23;
  } else {
  }
  int _sv0t60 = sv0_vec_get(out3, 7);
  int _sv0t61 = (0 - 17);
  if ((_sv0t60 != _sv0t61)) {
    return 24;
  } else {
  }
  Expr _sv0t62;
  Value _sv0t63;
  _sv0t63.tag = 1;
  int _sv0t64 = sv0_box_alloc(3);
  sv0_box_store(_sv0t64, 0, _sv0t63.tag);
  sv0_box_store(_sv0t64, 1, _sv0t63.p0);
  sv0_box_store(_sv0t64, 2, _sv0t63.p1);
  _sv0t62.tag = 0;
  _sv0t62.p0 = _sv0t64;
  int _sv0t65 = sv0_box_alloc(4);
  sv0_box_store(_sv0t65, 0, _sv0t62.tag);
  sv0_box_store(_sv0t65, 1, _sv0t62.p0);
  sv0_box_store(_sv0t65, 2, _sv0t62.p1);
  sv0_box_store(_sv0t65, 3, _sv0t62.p2);
  int cond4 = _sv0t65;
  int _sv0t66 = sv0_vec_new();
  int body4 = _sv0t66;
  Instr _sv0t67;
  _sv0t67.tag = 10;
  int _sv0t68 = sv0_box_alloc(5);
  sv0_box_store(_sv0t68, 0, _sv0t67.tag);
  sv0_box_store(_sv0t68, 1, _sv0t67.p0);
  sv0_box_store(_sv0t68, 2, _sv0t67.p1);
  sv0_box_store(_sv0t68, 3, _sv0t67.p2);
  sv0_box_store(_sv0t68, 4, _sv0t67.p3);
  sv0_vec_push(body4, _sv0t68);
  int _sv0t69 = sv0_vec_new();
  int out4 = _sv0t69;
  Instr _sv0t70;
  _sv0t70.tag = 7;
  _sv0t70.p0 = cond4;
  _sv0t70.p1 = body4;
  int _sv0t71 = emit_instr(_sv0t70, en, eb, ew, ec, efs, eff, pool, source, starts, ends, fns, out4);
  int r4 = _sv0t71;
  if ((r4 < 0)) {
    return 25;
  } else {
  }
  int _sv0t72 = sv0_vec_len(out4);
  if ((_sv0t72 != 8)) {
    return 26;
  } else {
  }
  int _sv0t73 = sv0_vec_get(out4, 4);
  if ((_sv0t73 != 112)) {
    return 27;
  } else {
  }
  int _sv0t74 = sv0_vec_get(out4, 5);
  int _sv0t75 = (0 - 12);
  if ((_sv0t74 != _sv0t75)) {
    return 28;
  } else {
  }
  int _sv0t76 = sv0_vec_get(out4, 6);
  if ((_sv0t76 != 112)) {
    return 29;
  } else {
  }
  int _sv0t77 = sv0_vec_get(out4, 7);
  int _sv0t78 = (0 - 17);
  if ((_sv0t77 != _sv0t78)) {
    return 30;
  } else {
  }
  Expr _sv0t79;
  Value _sv0t80;
  _sv0t80.tag = 1;
  int _sv0t81 = sv0_box_alloc(3);
  sv0_box_store(_sv0t81, 0, _sv0t80.tag);
  sv0_box_store(_sv0t81, 1, _sv0t80.p0);
  sv0_box_store(_sv0t81, 2, _sv0t80.p1);
  _sv0t79.tag = 0;
  _sv0t79.p0 = _sv0t81;
  int _sv0t82 = sv0_box_alloc(4);
  sv0_box_store(_sv0t82, 0, _sv0t79.tag);
  sv0_box_store(_sv0t82, 1, _sv0t79.p0);
  sv0_box_store(_sv0t82, 2, _sv0t79.p1);
  sv0_box_store(_sv0t82, 3, _sv0t79.p2);
  int cond5 = _sv0t82;
  int _sv0t83 = sv0_vec_new();
  int body5 = _sv0t83;
  Expr asgn_e;
  Value _sv0t84;
  _sv0t84.tag = 0;
  _sv0t84.p0 = 42;
  int _sv0t85 = sv0_box_alloc(3);
  sv0_box_store(_sv0t85, 0, _sv0t84.tag);
  sv0_box_store(_sv0t85, 1, _sv0t84.p0);
  sv0_box_store(_sv0t85, 2, _sv0t84.p1);
  asgn_e.tag = 0;
  asgn_e.p0 = _sv0t85;
  Instr _sv0t86;
  int _sv0t87 = sv0_box_alloc(4);
  sv0_box_store(_sv0t87, 0, asgn_e.tag);
  sv0_box_store(_sv0t87, 1, asgn_e.p0);
  sv0_box_store(_sv0t87, 2, asgn_e.p1);
  sv0_box_store(_sv0t87, 3, asgn_e.p2);
  _sv0t86.tag = 3;
  _sv0t86.p0 = 50;
  _sv0t86.p1 = _sv0t87;
  int _sv0t88 = sv0_box_alloc(5);
  sv0_box_store(_sv0t88, 0, _sv0t86.tag);
  sv0_box_store(_sv0t88, 1, _sv0t86.p0);
  sv0_box_store(_sv0t88, 2, _sv0t86.p1);
  sv0_box_store(_sv0t88, 3, _sv0t86.p2);
  sv0_box_store(_sv0t88, 4, _sv0t86.p3);
  sv0_vec_push(body5, _sv0t88);
  Instr _sv0t89;
  _sv0t89.tag = 14;
  int _sv0t90 = sv0_box_alloc(5);
  sv0_box_store(_sv0t90, 0, _sv0t89.tag);
  sv0_box_store(_sv0t90, 1, _sv0t89.p0);
  sv0_box_store(_sv0t90, 2, _sv0t89.p1);
  sv0_box_store(_sv0t90, 3, _sv0t89.p2);
  sv0_box_store(_sv0t90, 4, _sv0t89.p3);
  sv0_vec_push(body5, _sv0t90);
  int _sv0t91 = sv0_vec_new();
  int out5 = _sv0t91;
  Instr _sv0t92;
  _sv0t92.tag = 7;
  _sv0t92.p0 = cond5;
  _sv0t92.p1 = body5;
  int _sv0t93 = emit_instr(_sv0t92, en, eb, ew, ec, efs, eff, pool, source, starts, ends, fns, out5);
  int r5 = _sv0t93;
  if ((r5 < 0)) {
    return 31;
  } else {
  }
  int _sv0t94 = sv0_vec_len(out5);
  if ((_sv0t94 != 12)) {
    return 32;
  } else {
  }
  int _sv0t95 = sv0_vec_get(out5, 2);
  if ((_sv0t95 != 114)) {
    return 33;
  } else {
  }
  int _sv0t96 = sv0_vec_get(out5, 3);
  if ((_sv0t96 != 17)) {
    return 34;
  } else {
  }
  int _sv0t97 = sv0_vec_get(out5, 4);
  if ((_sv0t97 != 4)) {
    return 35;
  } else {
  }
  int _sv0t98 = sv0_vec_get(out5, 5);
  if ((_sv0t98 != 42)) {
    return 36;
  } else {
  }
  int _sv0t99 = sv0_vec_get(out5, 10);
  if ((_sv0t99 != 112)) {
    return 37;
  } else {
  }
  int _sv0t100 = sv0_vec_get(out5, 11);
  int _sv0t101 = (0 - 24);
  if ((_sv0t100 != _sv0t101)) {
    return 38;
  } else {
  }
  return 0;
}

static int test_emit_call(void) {
  int _sv0t0 = sv0_vec_new();
  int en = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int eb = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int ew = _sv0t2;
  int _sv0t3 = sv0_vec_new();
  int ec = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int efs = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int eff = _sv0t5;
  int _sv0t6 = sv0_vec_new();
  int pool = _sv0t6;
  const char* source;
  source = "sv0_printlnsv0_vec_newsv0_vec_pushsv0_vec_lenmyfunc";
  int _sv0t7 = sv0_vec_new();
  int starts = _sv0t7;
  int _sv0t8 = sv0_vec_new();
  int ends = _sv0t8;
  sv0_vec_push(starts, 0);
  sv0_vec_push(ends, 11);
  sv0_vec_push(starts, 11);
  sv0_vec_push(ends, 22);
  sv0_vec_push(starts, 22);
  sv0_vec_push(ends, 34);
  sv0_vec_push(starts, 34);
  sv0_vec_push(ends, 45);
  sv0_vec_push(starts, 45);
  sv0_vec_push(ends, 51);
  sv0_vec_push(en, 10);
  sv0_vec_push(eb, 0);
  sv0_vec_push(ew, 1);
  sv0_vec_push(efs, 0);
  sv0_vec_push(en, 20);
  sv0_vec_push(eb, 1);
  sv0_vec_push(ew, 1);
  sv0_vec_push(efs, 0);
  int _sv0t9 = sv0_vec_new();
  int fns = _sv0t9;
  sv0_vec_push(fns, 4);
  int _sv0t10 = sv0_vec_new();
  int args1 = _sv0t10;
  Value _sv0t11;
  _sv0t11.tag = 0;
  _sv0t11.p0 = 42;
  int _sv0t12 = sv0_box_alloc(3);
  sv0_box_store(_sv0t12, 0, _sv0t11.tag);
  sv0_box_store(_sv0t12, 1, _sv0t11.p0);
  sv0_box_store(_sv0t12, 2, _sv0t11.p1);
  sv0_vec_push(args1, _sv0t12);
  int _sv0t13 = sv0_vec_new();
  int out1 = _sv0t13;
  Instr _sv0t14;
  _sv0t14.tag = 11;
  _sv0t14.p0 = 0;
  _sv0t14.p1 = 0;
  _sv0t14.p2 = args1;
  _sv0t14.p3 = 0;
  int _sv0t15 = emit_instr(_sv0t14, en, eb, ew, ec, efs, eff, pool, source, starts, ends, fns, out1);
  int r1 = _sv0t15;
  if ((r1 != 4)) {
    return 1;
  } else {
  }
  int _sv0t16 = sv0_vec_get(out1, 0);
  if ((_sv0t16 != 4)) {
    return 2;
  } else {
  }
  int _sv0t17 = sv0_vec_get(out1, 1);
  if ((_sv0t17 != 42)) {
    return 3;
  } else {
  }
  int _sv0t18 = sv0_vec_get(out1, 2);
  if ((_sv0t18 != 117)) {
    return 4;
  } else {
  }
  int _sv0t19 = sv0_vec_get(out1, 3);
  if ((_sv0t19 != 0)) {
    return 5;
  } else {
  }
  int _sv0t20 = sv0_vec_new();
  int args2 = _sv0t20;
  int _sv0t21 = sv0_vec_new();
  int out2 = _sv0t21;
  Instr _sv0t22;
  _sv0t22.tag = 11;
  _sv0t22.p0 = 10;
  _sv0t22.p1 = 1;
  _sv0t22.p2 = args2;
  _sv0t22.p3 = 0;
  int _sv0t23 = emit_instr(_sv0t22, en, eb, ew, ec, efs, eff, pool, source, starts, ends, fns, out2);
  int r2 = _sv0t23;
  if ((r2 != 4)) {
    return 6;
  } else {
  }
  int _sv0t24 = sv0_vec_get(out2, 0);
  if ((_sv0t24 != 117)) {
    return 7;
  } else {
  }
  int _sv0t25 = sv0_vec_get(out2, 1);
  if ((_sv0t25 != 7)) {
    return 8;
  } else {
  }
  int _sv0t26 = sv0_vec_get(out2, 2);
  if ((_sv0t26 != 97)) {
    return 9;
  } else {
  }
  int _sv0t27 = sv0_vec_get(out2, 3);
  if ((_sv0t27 != 0)) {
    return 10;
  } else {
  }
  int _sv0t28 = sv0_vec_new();
  int args3 = _sv0t28;
  Value _sv0t29;
  _sv0t29.tag = 3;
  _sv0t29.p0 = 10;
  int _sv0t30 = sv0_box_alloc(3);
  sv0_box_store(_sv0t30, 0, _sv0t29.tag);
  sv0_box_store(_sv0t30, 1, _sv0t29.p0);
  sv0_box_store(_sv0t30, 2, _sv0t29.p1);
  sv0_vec_push(args3, _sv0t30);
  Value _sv0t31;
  _sv0t31.tag = 0;
  _sv0t31.p0 = 5;
  int _sv0t32 = sv0_box_alloc(3);
  sv0_box_store(_sv0t32, 0, _sv0t31.tag);
  sv0_box_store(_sv0t32, 1, _sv0t31.p0);
  sv0_box_store(_sv0t32, 2, _sv0t31.p1);
  sv0_vec_push(args3, _sv0t32);
  int _sv0t33 = sv0_vec_new();
  int out3 = _sv0t33;
  Instr _sv0t34;
  _sv0t34.tag = 11;
  _sv0t34.p0 = 0;
  _sv0t34.p1 = 2;
  _sv0t34.p2 = args3;
  _sv0t34.p3 = 0;
  int _sv0t35 = emit_instr(_sv0t34, en, eb, ew, ec, efs, eff, pool, source, starts, ends, fns, out3);
  int r3 = _sv0t35;
  if ((r3 != 6)) {
    return 11;
  } else {
  }
  int _sv0t36 = sv0_vec_get(out3, 0);
  if ((_sv0t36 != 96)) {
    return 12;
  } else {
  }
  int _sv0t37 = sv0_vec_get(out3, 1);
  if ((_sv0t37 != 0)) {
    return 13;
  } else {
  }
  int _sv0t38 = sv0_vec_get(out3, 2);
  if ((_sv0t38 != 4)) {
    return 14;
  } else {
  }
  int _sv0t39 = sv0_vec_get(out3, 3);
  if ((_sv0t39 != 5)) {
    return 15;
  } else {
  }
  int _sv0t40 = sv0_vec_get(out3, 4);
  if ((_sv0t40 != 117)) {
    return 16;
  } else {
  }
  int _sv0t41 = sv0_vec_get(out3, 5);
  if ((_sv0t41 != 8)) {
    return 17;
  } else {
  }
  int _sv0t42 = sv0_vec_new();
  int args4 = _sv0t42;
  Value _sv0t43;
  _sv0t43.tag = 3;
  _sv0t43.p0 = 10;
  int _sv0t44 = sv0_box_alloc(3);
  sv0_box_store(_sv0t44, 0, _sv0t43.tag);
  sv0_box_store(_sv0t44, 1, _sv0t43.p0);
  sv0_box_store(_sv0t44, 2, _sv0t43.p1);
  sv0_vec_push(args4, _sv0t44);
  int _sv0t45 = sv0_vec_new();
  int out4 = _sv0t45;
  Instr _sv0t46;
  _sv0t46.tag = 11;
  _sv0t46.p0 = 20;
  _sv0t46.p1 = 3;
  _sv0t46.p2 = args4;
  _sv0t46.p3 = 0;
  int _sv0t47 = emit_instr(_sv0t46, en, eb, ew, ec, efs, eff, pool, source, starts, ends, fns, out4);
  int r4 = _sv0t47;
  if ((r4 != 6)) {
    return 18;
  } else {
  }
  int _sv0t48 = sv0_vec_get(out4, 0);
  if ((_sv0t48 != 96)) {
    return 19;
  } else {
  }
  int _sv0t49 = sv0_vec_get(out4, 1);
  if ((_sv0t49 != 0)) {
    return 20;
  } else {
  }
  int _sv0t50 = sv0_vec_get(out4, 2);
  if ((_sv0t50 != 117)) {
    return 21;
  } else {
  }
  int _sv0t51 = sv0_vec_get(out4, 3);
  if ((_sv0t51 != 9)) {
    return 22;
  } else {
  }
  int _sv0t52 = sv0_vec_get(out4, 4);
  if ((_sv0t52 != 97)) {
    return 23;
  } else {
  }
  int _sv0t53 = sv0_vec_get(out4, 5);
  if ((_sv0t53 != 1)) {
    return 24;
  } else {
  }
  int _sv0t54 = sv0_vec_new();
  int args5 = _sv0t54;
  Value _sv0t55;
  _sv0t55.tag = 0;
  _sv0t55.p0 = 7;
  int _sv0t56 = sv0_box_alloc(3);
  sv0_box_store(_sv0t56, 0, _sv0t55.tag);
  sv0_box_store(_sv0t56, 1, _sv0t55.p0);
  sv0_box_store(_sv0t56, 2, _sv0t55.p1);
  sv0_vec_push(args5, _sv0t56);
  int _sv0t57 = sv0_vec_new();
  int out5 = _sv0t57;
  Instr _sv0t58;
  _sv0t58.tag = 11;
  _sv0t58.p0 = 20;
  _sv0t58.p1 = 4;
  _sv0t58.p2 = args5;
  _sv0t58.p3 = 0;
  int _sv0t59 = emit_instr(_sv0t58, en, eb, ew, ec, efs, eff, pool, source, starts, ends, fns, out5);
  int r5 = _sv0t59;
  if ((r5 != 7)) {
    return 25;
  } else {
  }
  int _sv0t60 = sv0_vec_get(out5, 0);
  if ((_sv0t60 != 4)) {
    return 26;
  } else {
  }
  int _sv0t61 = sv0_vec_get(out5, 1);
  if ((_sv0t61 != 7)) {
    return 27;
  } else {
  }
  int _sv0t62 = sv0_vec_get(out5, 2);
  if ((_sv0t62 != 115)) {
    return 28;
  } else {
  }
  int _sv0t63 = sv0_vec_get(out5, 3);
  if ((_sv0t63 != 0)) {
    return 29;
  } else {
  }
  int _sv0t64 = sv0_vec_get(out5, 4);
  if ((_sv0t64 != 1)) {
    return 30;
  } else {
  }
  int _sv0t65 = sv0_vec_get(out5, 5);
  if ((_sv0t65 != 97)) {
    return 31;
  } else {
  }
  int _sv0t66 = sv0_vec_get(out5, 6);
  if ((_sv0t66 != 1)) {
    return 32;
  } else {
  }
  int _sv0t67 = sv0_vec_new();
  int args6 = _sv0t67;
  int _sv0t68 = sv0_vec_new();
  int out6 = _sv0t68;
  Instr _sv0t69;
  _sv0t69.tag = 11;
  _sv0t69.p0 = 0;
  _sv0t69.p1 = 4;
  _sv0t69.p2 = args6;
  _sv0t69.p3 = 0;
  int _sv0t70 = emit_instr(_sv0t69, en, eb, ew, ec, efs, eff, pool, source, starts, ends, fns, out6);
  int r6 = _sv0t70;
  if ((r6 != 3)) {
    return 33;
  } else {
  }
  int _sv0t71 = sv0_vec_get(out6, 0);
  if ((_sv0t71 != 115)) {
    return 34;
  } else {
  }
  int _sv0t72 = sv0_vec_get(out6, 1);
  if ((_sv0t72 != 0)) {
    return 35;
  } else {
  }
  int _sv0t73 = sv0_vec_get(out6, 2);
  if ((_sv0t73 != 0)) {
    return 36;
  } else {
  }
  return 0;
}

static int test_emit_store_field(void) {
  int _sv0t0 = sv0_vec_new();
  int en = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int eb = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int ew = _sv0t2;
  int _sv0t3 = sv0_vec_new();
  int ec = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int efs = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int eff = _sv0t5;
  int _sv0t6 = sv0_vec_new();
  int pool = _sv0t6;
  int _sv0t7 = sv0_vec_new();
  int fns = _sv0t7;
  const char* source;
  source = "+";
  int _sv0t8 = sv0_vec_new();
  int starts = _sv0t8;
  int _sv0t9 = sv0_vec_new();
  int ends = _sv0t9;
  sv0_vec_push(starts, 0);
  sv0_vec_push(ends, 1);
  sv0_vec_push(en, 50);
  sv0_vec_push(eb, 0);
  sv0_vec_push(ew, 3);
  sv0_vec_push(efs, 0);
  sv0_vec_push(eff, 100);
  sv0_vec_push(eff, 200);
  sv0_vec_push(eff, 300);
  int _sv0t10 = sv0_vec_new();
  int out1 = _sv0t10;
  Instr sf1;
  Expr _sv0t11;
  Value _sv0t12;
  _sv0t12.tag = 0;
  _sv0t12.p0 = 7;
  int _sv0t13 = sv0_box_alloc(3);
  sv0_box_store(_sv0t13, 0, _sv0t12.tag);
  sv0_box_store(_sv0t13, 1, _sv0t12.p0);
  sv0_box_store(_sv0t13, 2, _sv0t12.p1);
  _sv0t11.tag = 0;
  _sv0t11.p0 = _sv0t13;
  int _sv0t14 = sv0_box_alloc(4);
  sv0_box_store(_sv0t14, 0, _sv0t11.tag);
  sv0_box_store(_sv0t14, 1, _sv0t11.p0);
  sv0_box_store(_sv0t14, 2, _sv0t11.p1);
  sv0_box_store(_sv0t14, 3, _sv0t11.p2);
  sf1.tag = 5;
  sf1.p0 = 50;
  sf1.p1 = 100;
  sf1.p2 = _sv0t14;
  int _sv0t15 = emit_instr(sf1, en, eb, ew, ec, efs, eff, pool, source, starts, ends, fns, out1);
  int r1 = _sv0t15;
  if ((r1 != 4)) {
    return 1;
  } else {
  }
  int _sv0t16 = sv0_vec_get(out1, 0);
  if ((_sv0t16 != 4)) {
    return 2;
  } else {
  }
  int _sv0t17 = sv0_vec_get(out1, 1);
  if ((_sv0t17 != 7)) {
    return 3;
  } else {
  }
  int _sv0t18 = sv0_vec_get(out1, 2);
  if ((_sv0t18 != 97)) {
    return 4;
  } else {
  }
  int _sv0t19 = sv0_vec_get(out1, 3);
  if ((_sv0t19 != 0)) {
    return 5;
  } else {
  }
  int _sv0t20 = sv0_vec_new();
  int out2 = _sv0t20;
  Instr sf2;
  Expr _sv0t21;
  Value _sv0t22;
  _sv0t22.tag = 0;
  _sv0t22.p0 = 99;
  int _sv0t23 = sv0_box_alloc(3);
  sv0_box_store(_sv0t23, 0, _sv0t22.tag);
  sv0_box_store(_sv0t23, 1, _sv0t22.p0);
  sv0_box_store(_sv0t23, 2, _sv0t22.p1);
  _sv0t21.tag = 0;
  _sv0t21.p0 = _sv0t23;
  int _sv0t24 = sv0_box_alloc(4);
  sv0_box_store(_sv0t24, 0, _sv0t21.tag);
  sv0_box_store(_sv0t24, 1, _sv0t21.p0);
  sv0_box_store(_sv0t24, 2, _sv0t21.p1);
  sv0_box_store(_sv0t24, 3, _sv0t21.p2);
  sf2.tag = 5;
  sf2.p0 = 50;
  sf2.p1 = 200;
  sf2.p2 = _sv0t24;
  int _sv0t25 = emit_instr(sf2, en, eb, ew, ec, efs, eff, pool, source, starts, ends, fns, out2);
  int r2 = _sv0t25;
  if ((r2 != 4)) {
    return 6;
  } else {
  }
  int _sv0t26 = sv0_vec_get(out2, 2);
  if ((_sv0t26 != 97)) {
    return 7;
  } else {
  }
  int _sv0t27 = sv0_vec_get(out2, 3);
  if ((_sv0t27 != 1)) {
    return 8;
  } else {
  }
  int _sv0t28 = sv0_vec_new();
  int out3 = _sv0t28;
  Instr sf3;
  Expr _sv0t29;
  Value _sv0t30;
  _sv0t30.tag = 1;
  int _sv0t31 = sv0_box_alloc(3);
  sv0_box_store(_sv0t31, 0, _sv0t30.tag);
  sv0_box_store(_sv0t31, 1, _sv0t30.p0);
  sv0_box_store(_sv0t31, 2, _sv0t30.p1);
  _sv0t29.tag = 0;
  _sv0t29.p0 = _sv0t31;
  int _sv0t32 = sv0_box_alloc(4);
  sv0_box_store(_sv0t32, 0, _sv0t29.tag);
  sv0_box_store(_sv0t32, 1, _sv0t29.p0);
  sv0_box_store(_sv0t32, 2, _sv0t29.p1);
  sv0_box_store(_sv0t32, 3, _sv0t29.p2);
  sf3.tag = 5;
  sf3.p0 = 50;
  sf3.p1 = 300;
  sf3.p2 = _sv0t32;
  int _sv0t33 = emit_instr(sf3, en, eb, ew, ec, efs, eff, pool, source, starts, ends, fns, out3);
  int r3 = _sv0t33;
  if ((r3 != 4)) {
    return 9;
  } else {
  }
  int _sv0t34 = sv0_vec_get(out3, 0);
  if ((_sv0t34 != 7)) {
    return 10;
  } else {
  }
  int _sv0t35 = sv0_vec_get(out3, 1);
  if ((_sv0t35 != 1)) {
    return 11;
  } else {
  }
  int _sv0t36 = sv0_vec_get(out3, 2);
  if ((_sv0t36 != 97)) {
    return 12;
  } else {
  }
  int _sv0t37 = sv0_vec_get(out3, 3);
  if ((_sv0t37 != 2)) {
    return 13;
  } else {
  }
  sv0_vec_push(en, 60);
  sv0_vec_push(eb, 3);
  sv0_vec_push(ew, 2);
  sv0_vec_push(efs, 3);
  sv0_vec_push(eff, 400);
  sv0_vec_push(eff, 500);
  int _sv0t38 = sv0_vec_new();
  int out4 = _sv0t38;
  Instr sf4;
  Expr _sv0t39;
  Value _sv0t40;
  _sv0t40.tag = 0;
  _sv0t40.p0 = 1;
  int _sv0t41 = sv0_box_alloc(3);
  sv0_box_store(_sv0t41, 0, _sv0t40.tag);
  sv0_box_store(_sv0t41, 1, _sv0t40.p0);
  sv0_box_store(_sv0t41, 2, _sv0t40.p1);
  _sv0t39.tag = 0;
  _sv0t39.p0 = _sv0t41;
  int _sv0t42 = sv0_box_alloc(4);
  sv0_box_store(_sv0t42, 0, _sv0t39.tag);
  sv0_box_store(_sv0t42, 1, _sv0t39.p0);
  sv0_box_store(_sv0t42, 2, _sv0t39.p1);
  sv0_box_store(_sv0t42, 3, _sv0t39.p2);
  sf4.tag = 5;
  sf4.p0 = 60;
  sf4.p1 = 500;
  sf4.p2 = _sv0t42;
  int _sv0t43 = emit_instr(sf4, en, eb, ew, ec, efs, eff, pool, source, starts, ends, fns, out4);
  int r4 = _sv0t43;
  if ((r4 != 4)) {
    return 14;
  } else {
  }
  int _sv0t44 = sv0_vec_get(out4, 2);
  if ((_sv0t44 != 97)) {
    return 15;
  } else {
  }
  int _sv0t45 = sv0_vec_get(out4, 3);
  if ((_sv0t45 != 4)) {
    return 16;
  } else {
  }
  int _sv0t46 = sv0_vec_new();
  int out5 = _sv0t46;
  Instr sf5;
  Expr _sv0t47;
  Value _sv0t48;
  _sv0t48.tag = 0;
  _sv0t48.p0 = 2;
  int _sv0t49 = sv0_box_alloc(3);
  sv0_box_store(_sv0t49, 0, _sv0t48.tag);
  sv0_box_store(_sv0t49, 1, _sv0t48.p0);
  sv0_box_store(_sv0t49, 2, _sv0t48.p1);
  _sv0t47.tag = 0;
  _sv0t47.p0 = _sv0t49;
  int _sv0t50 = sv0_box_alloc(4);
  sv0_box_store(_sv0t50, 0, _sv0t47.tag);
  sv0_box_store(_sv0t50, 1, _sv0t47.p0);
  sv0_box_store(_sv0t50, 2, _sv0t47.p1);
  sv0_box_store(_sv0t50, 3, _sv0t47.p2);
  sf5.tag = 5;
  sf5.p0 = 60;
  sf5.p1 = 400;
  sf5.p2 = _sv0t50;
  int _sv0t51 = emit_instr(sf5, en, eb, ew, ec, efs, eff, pool, source, starts, ends, fns, out5);
  int r5 = _sv0t51;
  if ((r5 != 4)) {
    return 17;
  } else {
  }
  int _sv0t52 = sv0_vec_get(out5, 2);
  if ((_sv0t52 != 97)) {
    return 18;
  } else {
  }
  int _sv0t53 = sv0_vec_get(out5, 3);
  if ((_sv0t53 != 3)) {
    return 19;
  } else {
  }
  int _sv0t54 = sv0_vec_new();
  int out6 = _sv0t54;
  Instr sf6;
  Expr _sv0t55;
  Value _sv0t56;
  _sv0t56.tag = 0;
  _sv0t56.p0 = 0;
  int _sv0t57 = sv0_box_alloc(3);
  sv0_box_store(_sv0t57, 0, _sv0t56.tag);
  sv0_box_store(_sv0t57, 1, _sv0t56.p0);
  sv0_box_store(_sv0t57, 2, _sv0t56.p1);
  _sv0t55.tag = 0;
  _sv0t55.p0 = _sv0t57;
  int _sv0t58 = sv0_box_alloc(4);
  sv0_box_store(_sv0t58, 0, _sv0t55.tag);
  sv0_box_store(_sv0t58, 1, _sv0t55.p0);
  sv0_box_store(_sv0t58, 2, _sv0t55.p1);
  sv0_box_store(_sv0t58, 3, _sv0t55.p2);
  sf6.tag = 5;
  sf6.p0 = 50;
  sf6.p1 = 999;
  sf6.p2 = _sv0t58;
  int _sv0t59 = emit_instr(sf6, en, eb, ew, ec, efs, eff, pool, source, starts, ends, fns, out6);
  int r6 = _sv0t59;
  int _sv0t60 = (0 - 1);
  if ((r6 != _sv0t60)) {
    return 20;
  } else {
  }
  return 0;
}

static int test_emit_contract(void) {
  int _sv0t0 = sv0_vec_new();
  int en = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int eb = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int ew = _sv0t2;
  int _sv0t3 = sv0_vec_new();
  int ec = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int efs = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int eff = _sv0t5;
  int _sv0t6 = sv0_vec_new();
  int pool = _sv0t6;
  int _sv0t7 = sv0_vec_new();
  int fns = _sv0t7;
  const char* source;
  source = "+";
  int _sv0t8 = sv0_vec_new();
  int starts = _sv0t8;
  int _sv0t9 = sv0_vec_new();
  int ends = _sv0t9;
  sv0_vec_push(starts, 0);
  sv0_vec_push(ends, 1);
  int _sv0t10 = sv0_vec_new();
  int out1 = _sv0t10;
  Instr req1;
  Expr _sv0t11;
  Value _sv0t12;
  _sv0t12.tag = 1;
  int _sv0t13 = sv0_box_alloc(3);
  sv0_box_store(_sv0t13, 0, _sv0t12.tag);
  sv0_box_store(_sv0t13, 1, _sv0t12.p0);
  sv0_box_store(_sv0t13, 2, _sv0t12.p1);
  _sv0t11.tag = 0;
  _sv0t11.p0 = _sv0t13;
  int _sv0t14 = sv0_box_alloc(4);
  sv0_box_store(_sv0t14, 0, _sv0t11.tag);
  sv0_box_store(_sv0t14, 1, _sv0t11.p0);
  sv0_box_store(_sv0t14, 2, _sv0t11.p1);
  sv0_box_store(_sv0t14, 3, _sv0t11.p2);
  req1.tag = 12;
  req1.p0 = _sv0t14;
  req1.p1 = 42;
  int _sv0t15 = emit_instr(req1, en, eb, ew, ec, efs, eff, pool, source, starts, ends, fns, out1);
  int r1 = _sv0t15;
  if ((r1 != 4)) {
    return 1;
  } else {
  }
  int _sv0t16 = sv0_vec_get(out1, 0);
  if ((_sv0t16 != 7)) {
    return 2;
  } else {
  }
  int _sv0t17 = sv0_vec_get(out1, 1);
  if ((_sv0t17 != 1)) {
    return 3;
  } else {
  }
  int _sv0t18 = sv0_vec_get(out1, 2);
  if ((_sv0t18 != 160)) {
    return 4;
  } else {
  }
  int _sv0t19 = sv0_vec_get(out1, 3);
  if ((_sv0t19 != 0)) {
    return 5;
  } else {
  }
  int _sv0t20 = pool_size(pool);
  if ((_sv0t20 != 1)) {
    return 6;
  } else {
  }
  int _sv0t21 = sv0_vec_new();
  int out2 = _sv0t21;
  Instr ens1;
  Expr _sv0t22;
  Value _sv0t23;
  _sv0t23.tag = 2;
  int _sv0t24 = sv0_box_alloc(3);
  sv0_box_store(_sv0t24, 0, _sv0t23.tag);
  sv0_box_store(_sv0t24, 1, _sv0t23.p0);
  sv0_box_store(_sv0t24, 2, _sv0t23.p1);
  _sv0t22.tag = 0;
  _sv0t22.p0 = _sv0t24;
  int _sv0t25 = sv0_box_alloc(4);
  sv0_box_store(_sv0t25, 0, _sv0t22.tag);
  sv0_box_store(_sv0t25, 1, _sv0t22.p0);
  sv0_box_store(_sv0t25, 2, _sv0t22.p1);
  sv0_box_store(_sv0t25, 3, _sv0t22.p2);
  ens1.tag = 13;
  ens1.p0 = _sv0t25;
  ens1.p1 = 42;
  int _sv0t26 = emit_instr(ens1, en, eb, ew, ec, efs, eff, pool, source, starts, ends, fns, out2);
  int r2 = _sv0t26;
  if ((r2 != 4)) {
    return 7;
  } else {
  }
  int _sv0t27 = sv0_vec_get(out2, 0);
  if ((_sv0t27 != 7)) {
    return 8;
  } else {
  }
  int _sv0t28 = sv0_vec_get(out2, 1);
  if ((_sv0t28 != 0)) {
    return 9;
  } else {
  }
  int _sv0t29 = sv0_vec_get(out2, 2);
  if ((_sv0t29 != 160)) {
    return 10;
  } else {
  }
  int _sv0t30 = sv0_vec_get(out2, 3);
  if ((_sv0t30 != 1)) {
    return 11;
  } else {
  }
  int _sv0t31 = pool_size(pool);
  if ((_sv0t31 != 2)) {
    return 12;
  } else {
  }
  int _sv0t32 = sv0_vec_new();
  int out3 = _sv0t32;
  Instr req2;
  Expr _sv0t33;
  Value _sv0t34;
  _sv0t34.tag = 1;
  int _sv0t35 = sv0_box_alloc(3);
  sv0_box_store(_sv0t35, 0, _sv0t34.tag);
  sv0_box_store(_sv0t35, 1, _sv0t34.p0);
  sv0_box_store(_sv0t35, 2, _sv0t34.p1);
  _sv0t33.tag = 0;
  _sv0t33.p0 = _sv0t35;
  int _sv0t36 = sv0_box_alloc(4);
  sv0_box_store(_sv0t36, 0, _sv0t33.tag);
  sv0_box_store(_sv0t36, 1, _sv0t33.p0);
  sv0_box_store(_sv0t36, 2, _sv0t33.p1);
  sv0_box_store(_sv0t36, 3, _sv0t33.p2);
  req2.tag = 12;
  req2.p0 = _sv0t36;
  req2.p1 = 42;
  int _sv0t37 = emit_instr(req2, en, eb, ew, ec, efs, eff, pool, source, starts, ends, fns, out3);
  int r3 = _sv0t37;
  if ((r3 != 4)) {
    return 13;
  } else {
  }
  int _sv0t38 = sv0_vec_get(out3, 3);
  if ((_sv0t38 != 0)) {
    return 14;
  } else {
  }
  int _sv0t39 = pool_size(pool);
  if ((_sv0t39 != 2)) {
    return 15;
  } else {
  }
  int _sv0t40 = sv0_vec_new();
  int out4 = _sv0t40;
  Instr req3;
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
  req3.tag = 12;
  req3.p0 = _sv0t44;
  req3.p1 = 99;
  int _sv0t45 = emit_instr(req3, en, eb, ew, ec, efs, eff, pool, source, starts, ends, fns, out4);
  int r4 = _sv0t45;
  if ((r4 != 4)) {
    return 16;
  } else {
  }
  int _sv0t46 = sv0_vec_get(out4, 3);
  if ((_sv0t46 != 2)) {
    return 17;
  } else {
  }
  int _sv0t47 = pool_size(pool);
  if ((_sv0t47 != 3)) {
    return 18;
  } else {
  }
  int _sv0t48 = contract_pool_key(42, 0);
  int k0 = _sv0t48;
  int _sv0t49 = contract_pool_key(42, 1);
  int k1 = _sv0t49;
  int _sv0t50 = contract_pool_key(99, 0);
  int k2 = _sv0t50;
  if ((k0 == k1)) {
    return 19;
  } else {
  }
  if ((k0 == k2)) {
    return 20;
  } else {
  }
  if ((k0 >= 0)) {
    return 21;
  } else {
  }
  if ((k1 >= 0)) {
    return 22;
  } else {
  }
  return 0;
}

static int test_compute_arity(void) {
  int _sv0t0 = sv0_vec_new();
  int sn = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int sfc = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int enn = _sv0t2;
  int _sv0t3 = sv0_vec_new();
  int enw = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int pc0 = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int _sv0t6 = sv0_vec_new();
  int _sv0t7 = compute_arity(pc0, sn, sfc, enn, enw, "", _sv0t5, _sv0t6);
  if ((_sv0t7 != 0)) {
    return 1;
  } else {
  }
  int _sv0t8 = sv0_vec_new();
  int pc1 = _sv0t8;
  sv0_vec_push(pc1, 777);
  int _sv0t9 = sv0_vec_new();
  int _sv0t10 = sv0_vec_new();
  int _sv0t11 = compute_arity(pc1, sn, sfc, enn, enw, "", _sv0t9, _sv0t10);
  if ((_sv0t11 != 1)) {
    return 2;
  } else {
  }
  int _sv0t12 = sv0_vec_new();
  int pc2 = _sv0t12;
  sv0_vec_push(pc2, 777);
  sv0_vec_push(pc2, 778);
  int _sv0t13 = sv0_vec_new();
  int _sv0t14 = sv0_vec_new();
  int _sv0t15 = compute_arity(pc2, sn, sfc, enn, enw, "", _sv0t13, _sv0t14);
  if ((_sv0t15 != 2)) {
    return 3;
  } else {
  }
  int _sv0t16 = sv0_vec_new();
  int sn2 = _sv0t16;
  int _sv0t17 = sv0_vec_new();
  int sfc2 = _sv0t17;
  sv0_vec_push(sn2, 500);
  sv0_vec_push(sfc2, 3);
  int _sv0t18 = sv0_vec_new();
  int pc3 = _sv0t18;
  sv0_vec_push(pc3, 500);
  int _sv0t19 = sv0_vec_new();
  int _sv0t20 = sv0_vec_new();
  int _sv0t21 = compute_arity(pc3, sn2, sfc2, enn, enw, "", _sv0t19, _sv0t20);
  if ((_sv0t21 != 3)) {
    return 4;
  } else {
  }
  int _sv0t22 = sv0_vec_new();
  int pc4 = _sv0t22;
  sv0_vec_push(pc4, 777);
  sv0_vec_push(pc4, 500);
  int _sv0t23 = sv0_vec_new();
  int _sv0t24 = sv0_vec_new();
  int _sv0t25 = compute_arity(pc4, sn2, sfc2, enn, enw, "", _sv0t23, _sv0t24);
  if ((_sv0t25 != 4)) {
    return 5;
  } else {
  }
  int _sv0t26 = sv0_vec_new();
  int enn2 = _sv0t26;
  int _sv0t27 = sv0_vec_new();
  int enw2 = _sv0t27;
  sv0_vec_push(enn2, 600);
  sv0_vec_push(enw2, 2);
  int _sv0t28 = sv0_vec_new();
  int pc5 = _sv0t28;
  sv0_vec_push(pc5, 600);
  int _sv0t29 = sv0_vec_new();
  int _sv0t30 = sv0_vec_new();
  int _sv0t31 = compute_arity(pc5, sn, sfc, enn2, enw2, "", _sv0t29, _sv0t30);
  if ((_sv0t31 != 2)) {
    return 6;
  } else {
  }
  return 0;
}

static int test_emit_fn(void) {
  int _sv0t0 = sv0_vec_new();
  int sn = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int sfc = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int enn = _sv0t2;
  int _sv0t3 = sv0_vec_new();
  int enw = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int pool = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int fns = _sv0t5;
  const char* source;
  source = "+";
  int _sv0t6 = sv0_vec_new();
  int starts = _sv0t6;
  int _sv0t7 = sv0_vec_new();
  int ends = _sv0t7;
  sv0_vec_push(starts, 0);
  sv0_vec_push(ends, 1);
  int _sv0t8 = sv0_vec_new();
  int ft = _sv0t8;
  int _sv0t9 = sv0_vec_new();
  int pn0 = _sv0t9;
  int _sv0t10 = sv0_vec_new();
  int pc0 = _sv0t10;
  int _sv0t11 = sv0_vec_new();
  int ins0 = _sv0t11;
  Instr _sv0t12;
  _sv0t12.tag = 14;
  int _sv0t13 = sv0_box_alloc(5);
  sv0_box_store(_sv0t13, 0, _sv0t12.tag);
  sv0_box_store(_sv0t13, 1, _sv0t12.p0);
  sv0_box_store(_sv0t13, 2, _sv0t12.p1);
  sv0_box_store(_sv0t13, 3, _sv0t12.p2);
  sv0_box_store(_sv0t13, 4, _sv0t12.p3);
  sv0_vec_push(ins0, _sv0t13);
  int _sv0t14 = sv0_vec_new();
  int _sv0t15 = sv0_vec_new();
  int _sv0t16 = emit_fn(42, pn0, pc0, ins0, sn, sfc, enn, enw, _sv0t14, _sv0t15, pool, source, starts, ends, fns, ft);
  int r0 = _sv0t16;
  if ((r0 != 0)) {
    return 1;
  } else {
  }
  int _sv0t17 = sv0_vec_len(ft);
  if ((_sv0t17 != 4)) {
    return 2;
  } else {
  }
  int _sv0t18 = sv0_vec_get(ft, 0);
  int ni0 = _sv0t18;
  if ((ni0 != 0)) {
    return 3;
  } else {
  }
  int _sv0t19 = sv0_vec_get(ft, 1);
  if ((_sv0t19 != 0)) {
    return 4;
  } else {
  }
  int _sv0t20 = sv0_vec_get(ft, 2);
  if ((_sv0t20 != 0)) {
    return 5;
  } else {
  }
  int _sv0t21 = sv0_vec_get(ft, 3);
  int code0 = _sv0t21;
  int _sv0t22 = sv0_vec_len(code0);
  if ((_sv0t22 != 2)) {
    return 6;
  } else {
  }
  int _sv0t23 = sv0_vec_get(code0, 0);
  if ((_sv0t23 != 118)) {
    return 7;
  } else {
  }
  int _sv0t24 = sv0_vec_get(code0, 1);
  if ((_sv0t24 != 0)) {
    return 8;
  } else {
  }
  int _sv0t25 = sv0_vec_new();
  int pn1 = _sv0t25;
  int _sv0t26 = sv0_vec_new();
  int pc1 = _sv0t26;
  sv0_vec_push(pn1, 10);
  sv0_vec_push(pc1, 777);
  int _sv0t27 = sv0_vec_new();
  int ins1 = _sv0t27;
  Instr _sv0t28;
  Expr _sv0t29;
  Value _sv0t30;
  _sv0t30.tag = 0;
  _sv0t30.p0 = 99;
  int _sv0t31 = sv0_box_alloc(3);
  sv0_box_store(_sv0t31, 0, _sv0t30.tag);
  sv0_box_store(_sv0t31, 1, _sv0t30.p0);
  sv0_box_store(_sv0t31, 2, _sv0t30.p1);
  _sv0t29.tag = 0;
  _sv0t29.p0 = _sv0t31;
  int _sv0t32 = sv0_box_alloc(4);
  sv0_box_store(_sv0t32, 0, _sv0t29.tag);
  sv0_box_store(_sv0t32, 1, _sv0t29.p0);
  sv0_box_store(_sv0t32, 2, _sv0t29.p1);
  sv0_box_store(_sv0t32, 3, _sv0t29.p2);
  _sv0t28.tag = 3;
  _sv0t28.p0 = 20;
  _sv0t28.p1 = _sv0t32;
  int _sv0t33 = sv0_box_alloc(5);
  sv0_box_store(_sv0t33, 0, _sv0t28.tag);
  sv0_box_store(_sv0t33, 1, _sv0t28.p0);
  sv0_box_store(_sv0t33, 2, _sv0t28.p1);
  sv0_box_store(_sv0t33, 3, _sv0t28.p2);
  sv0_box_store(_sv0t33, 4, _sv0t28.p3);
  sv0_vec_push(ins1, _sv0t33);
  Instr _sv0t34;
  Value _sv0t35;
  _sv0t35.tag = 3;
  _sv0t35.p0 = 20;
  int _sv0t36 = sv0_box_alloc(3);
  sv0_box_store(_sv0t36, 0, _sv0t35.tag);
  sv0_box_store(_sv0t36, 1, _sv0t35.p0);
  sv0_box_store(_sv0t36, 2, _sv0t35.p1);
  _sv0t34.tag = 15;
  _sv0t34.p0 = _sv0t36;
  int _sv0t37 = sv0_box_alloc(5);
  sv0_box_store(_sv0t37, 0, _sv0t34.tag);
  sv0_box_store(_sv0t37, 1, _sv0t34.p0);
  sv0_box_store(_sv0t37, 2, _sv0t34.p1);
  sv0_box_store(_sv0t37, 3, _sv0t34.p2);
  sv0_box_store(_sv0t37, 4, _sv0t34.p3);
  sv0_vec_push(ins1, _sv0t37);
  int _sv0t38 = sv0_vec_new();
  int _sv0t39 = sv0_vec_new();
  int _sv0t40 = emit_fn(43, pn1, pc1, ins1, sn, sfc, enn, enw, _sv0t38, _sv0t39, pool, source, starts, ends, fns, ft);
  int r1 = _sv0t40;
  if ((r1 != 0)) {
    return 9;
  } else {
  }
  int _sv0t41 = sv0_vec_len(ft);
  if ((_sv0t41 != 8)) {
    return 10;
  } else {
  }
  int _sv0t42 = sv0_vec_get(ft, 5);
  if ((_sv0t42 != 1)) {
    return 11;
  } else {
  }
  int _sv0t43 = sv0_vec_get(ft, 6);
  int lc1 = _sv0t43;
  if ((lc1 != 2)) {
    return 12;
  } else {
  }
  int _sv0t44 = sv0_vec_get(ft, 7);
  int code1 = _sv0t44;
  int _sv0t45 = sv0_vec_len(code1);
  if ((_sv0t45 < 2)) {
    return 13;
  } else {
  }
  int _sv0t46 = pool_size(pool);
  if ((_sv0t46 != 2)) {
    return 14;
  } else {
  }
  int _sv0t47 = sv0_vec_new();
  int sn3 = _sv0t47;
  int _sv0t48 = sv0_vec_new();
  int sfc3 = _sv0t48;
  sv0_vec_push(sn3, 500);
  sv0_vec_push(sfc3, 3);
  int _sv0t49 = sv0_vec_new();
  int pn2 = _sv0t49;
  int _sv0t50 = sv0_vec_new();
  int pc2 = _sv0t50;
  sv0_vec_push(pn2, 30);
  sv0_vec_push(pc2, 500);
  int _sv0t51 = sv0_vec_new();
  int ins2 = _sv0t51;
  Instr _sv0t52;
  _sv0t52.tag = 14;
  int _sv0t53 = sv0_box_alloc(5);
  sv0_box_store(_sv0t53, 0, _sv0t52.tag);
  sv0_box_store(_sv0t53, 1, _sv0t52.p0);
  sv0_box_store(_sv0t53, 2, _sv0t52.p1);
  sv0_box_store(_sv0t53, 3, _sv0t52.p2);
  sv0_box_store(_sv0t53, 4, _sv0t52.p3);
  sv0_vec_push(ins2, _sv0t53);
  int _sv0t54 = sv0_vec_new();
  int pool2 = _sv0t54;
  int _sv0t55 = sv0_vec_new();
  int ft2 = _sv0t55;
  int _sv0t56 = sv0_vec_new();
  int _sv0t57 = sv0_vec_new();
  int _sv0t58 = emit_fn(44, pn2, pc2, ins2, sn3, sfc3, enn, enw, _sv0t56, _sv0t57, pool2, source, starts, ends, fns, ft2);
  int r2 = _sv0t58;
  if ((r2 != 0)) {
    return 15;
  } else {
  }
  int _sv0t59 = sv0_vec_get(ft2, 1);
  if ((_sv0t59 != 3)) {
    return 16;
  } else {
  }
  int _sv0t60 = sv0_vec_get(ft2, 2);
  if ((_sv0t60 != 3)) {
    return 17;
  } else {
  }
  int _sv0t61 = sv0_vec_new();
  int pn3 = _sv0t61;
  int _sv0t62 = sv0_vec_new();
  int pc3 = _sv0t62;
  int _sv0t63 = sv0_vec_new();
  int ins3 = _sv0t63;
  Instr _sv0t64;
  _sv0t64.tag = 14;
  int _sv0t65 = sv0_box_alloc(5);
  sv0_box_store(_sv0t65, 0, _sv0t64.tag);
  sv0_box_store(_sv0t65, 1, _sv0t64.p0);
  sv0_box_store(_sv0t65, 2, _sv0t64.p1);
  sv0_box_store(_sv0t65, 3, _sv0t64.p2);
  sv0_box_store(_sv0t65, 4, _sv0t64.p3);
  sv0_vec_push(ins3, _sv0t65);
  int _sv0t66 = sv0_vec_new();
  int _sv0t67 = sv0_vec_new();
  int _sv0t68 = emit_fn(45, pn3, pc3, ins3, sn, sfc, enn, enw, _sv0t66, _sv0t67, pool2, source, starts, ends, fns, ft2);
  int r3 = _sv0t68;
  if ((r3 != 0)) {
    return 18;
  } else {
  }
  int _sv0t69 = sv0_vec_len(ft2);
  if ((_sv0t69 != 8)) {
    return 19;
  } else {
  }
  int _sv0t70 = sv0_vec_get(ft2, 4);
  int _sv0t71 = sv0_vec_get(ft2, 0);
  int _sv0t72 = (_sv0t71 + 1);
  if ((_sv0t70 != _sv0t72)) {
    return 20;
  } else {
  }
  return 0;
}

static int test_local_count_and_env(void) {
  int _sv0t0 = sv0_vec_new();
  int sn = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int sfc = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int enn = _sv0t2;
  int _sv0t3 = sv0_vec_new();
  int enw = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int en0 = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int eb0 = _sv0t5;
  int _sv0t6 = sv0_vec_new();
  int ew0 = _sv0t6;
  int _sv0t7 = sv0_vec_new();
  int ec0 = _sv0t7;
  int _sv0t8 = sv0_vec_new();
  int efs0 = _sv0t8;
  int _sv0t9 = sv0_vec_new();
  int pn0 = _sv0t9;
  int _sv0t10 = sv0_vec_new();
  int pc0 = _sv0t10;
  int _sv0t11 = sv0_vec_new();
  int ins0 = _sv0t11;
  int _sv0t12 = sv0_vec_new();
  int _sv0t13 = sv0_vec_new();
  int _sv0t14 = sv0_vec_new();
  int _sv0t15 = sv0_vec_new();
  int _sv0t16 = sv0_vec_new();
  int _sv0t17 = local_count_and_env(pn0, pc0, ins0, en0, eb0, ew0, ec0, efs0, sn, sfc, enn, enw, _sv0t12, _sv0t13, _sv0t14, "", _sv0t15, _sv0t16);
  int lc0 = _sv0t17;
  if ((lc0 != 0)) {
    return 1;
  } else {
  }
  int _sv0t18 = sv0_vec_len(en0);
  if ((_sv0t18 != 0)) {
    return 2;
  } else {
  }
  int _sv0t19 = sv0_vec_new();
  int en1 = _sv0t19;
  int _sv0t20 = sv0_vec_new();
  int eb1 = _sv0t20;
  int _sv0t21 = sv0_vec_new();
  int ew1 = _sv0t21;
  int _sv0t22 = sv0_vec_new();
  int ec1 = _sv0t22;
  int _sv0t23 = sv0_vec_new();
  int efs1 = _sv0t23;
  int _sv0t24 = sv0_vec_new();
  int pn1 = _sv0t24;
  int _sv0t25 = sv0_vec_new();
  int pc1 = _sv0t25;
  sv0_vec_push(pn1, 10);
  sv0_vec_push(pc1, 777);
  sv0_vec_push(pn1, 20);
  sv0_vec_push(pc1, 778);
  int _sv0t26 = sv0_vec_new();
  int ins1 = _sv0t26;
  int _sv0t27 = sv0_vec_new();
  int _sv0t28 = sv0_vec_new();
  int _sv0t29 = sv0_vec_new();
  int _sv0t30 = sv0_vec_new();
  int _sv0t31 = sv0_vec_new();
  int _sv0t32 = local_count_and_env(pn1, pc1, ins1, en1, eb1, ew1, ec1, efs1, sn, sfc, enn, enw, _sv0t27, _sv0t28, _sv0t29, "", _sv0t30, _sv0t31);
  int lc1 = _sv0t32;
  if ((lc1 != 2)) {
    return 3;
  } else {
  }
  int _sv0t33 = sv0_vec_len(en1);
  if ((_sv0t33 != 2)) {
    return 4;
  } else {
  }
  int _sv0t34 = sv0_vec_get(en1, 0);
  if ((_sv0t34 != 10)) {
    return 5;
  } else {
  }
  int _sv0t35 = sv0_vec_get(eb1, 0);
  if ((_sv0t35 != 0)) {
    return 6;
  } else {
  }
  int _sv0t36 = sv0_vec_get(ew1, 0);
  if ((_sv0t36 != 1)) {
    return 7;
  } else {
  }
  int _sv0t37 = sv0_vec_get(en1, 1);
  if ((_sv0t37 != 20)) {
    return 8;
  } else {
  }
  int _sv0t38 = sv0_vec_get(eb1, 1);
  if ((_sv0t38 != 1)) {
    return 9;
  } else {
  }
  int _sv0t39 = sv0_vec_new();
  int sn2 = _sv0t39;
  int _sv0t40 = sv0_vec_new();
  int sfc2 = _sv0t40;
  sv0_vec_push(sn2, 500);
  sv0_vec_push(sfc2, 3);
  int _sv0t41 = sv0_vec_new();
  int en2 = _sv0t41;
  int _sv0t42 = sv0_vec_new();
  int eb2 = _sv0t42;
  int _sv0t43 = sv0_vec_new();
  int ew2 = _sv0t43;
  int _sv0t44 = sv0_vec_new();
  int ec2 = _sv0t44;
  int _sv0t45 = sv0_vec_new();
  int efs2 = _sv0t45;
  int _sv0t46 = sv0_vec_new();
  int pn2 = _sv0t46;
  int _sv0t47 = sv0_vec_new();
  int pc2 = _sv0t47;
  sv0_vec_push(pn2, 30);
  sv0_vec_push(pc2, 500);
  int _sv0t48 = sv0_vec_new();
  int ins2 = _sv0t48;
  int _sv0t49 = sv0_vec_new();
  int _sv0t50 = sv0_vec_new();
  int _sv0t51 = sv0_vec_new();
  int _sv0t52 = sv0_vec_new();
  int _sv0t53 = sv0_vec_new();
  int _sv0t54 = local_count_and_env(pn2, pc2, ins2, en2, eb2, ew2, ec2, efs2, sn2, sfc2, enn, enw, _sv0t49, _sv0t50, _sv0t51, "", _sv0t52, _sv0t53);
  int lc2 = _sv0t54;
  if ((lc2 != 3)) {
    return 10;
  } else {
  }
  int _sv0t55 = sv0_vec_get(ew2, 0);
  if ((_sv0t55 != 3)) {
    return 11;
  } else {
  }
  int _sv0t56 = sv0_vec_get(eb2, 0);
  if ((_sv0t56 != 0)) {
    return 12;
  } else {
  }
  int _sv0t57 = sv0_vec_new();
  int en3 = _sv0t57;
  int _sv0t58 = sv0_vec_new();
  int eb3 = _sv0t58;
  int _sv0t59 = sv0_vec_new();
  int ew3 = _sv0t59;
  int _sv0t60 = sv0_vec_new();
  int ec3 = _sv0t60;
  int _sv0t61 = sv0_vec_new();
  int efs3 = _sv0t61;
  int _sv0t62 = sv0_vec_new();
  int pn3 = _sv0t62;
  int _sv0t63 = sv0_vec_new();
  int pc3 = _sv0t63;
  int _sv0t64 = sv0_vec_new();
  int ins3 = _sv0t64;
  Instr _sv0t65;
  _sv0t65.tag = 1;
  _sv0t65.p0 = 40;
  int _sv0t66 = sv0_box_alloc(5);
  sv0_box_store(_sv0t66, 0, _sv0t65.tag);
  sv0_box_store(_sv0t66, 1, _sv0t65.p0);
  sv0_box_store(_sv0t66, 2, _sv0t65.p1);
  sv0_box_store(_sv0t66, 3, _sv0t65.p2);
  sv0_box_store(_sv0t66, 4, _sv0t65.p3);
  sv0_vec_push(ins3, _sv0t66);
  int _sv0t67 = sv0_vec_new();
  int _sv0t68 = sv0_vec_new();
  int _sv0t69 = sv0_vec_new();
  int _sv0t70 = sv0_vec_new();
  int _sv0t71 = sv0_vec_new();
  int _sv0t72 = local_count_and_env(pn3, pc3, ins3, en3, eb3, ew3, ec3, efs3, sn, sfc, enn, enw, _sv0t67, _sv0t68, _sv0t69, "", _sv0t70, _sv0t71);
  int lc3 = _sv0t72;
  if ((lc3 != 1)) {
    return 13;
  } else {
  }
  int _sv0t73 = sv0_vec_get(en3, 0);
  if ((_sv0t73 != 40)) {
    return 14;
  } else {
  }
  int _sv0t74 = sv0_vec_get(eb3, 0);
  if ((_sv0t74 != 0)) {
    return 15;
  } else {
  }
  int _sv0t75 = sv0_vec_get(ew3, 0);
  if ((_sv0t75 != 1)) {
    return 16;
  } else {
  }
  int _sv0t76 = sv0_vec_new();
  int en4 = _sv0t76;
  int _sv0t77 = sv0_vec_new();
  int eb4 = _sv0t77;
  int _sv0t78 = sv0_vec_new();
  int ew4 = _sv0t78;
  int _sv0t79 = sv0_vec_new();
  int ec4 = _sv0t79;
  int _sv0t80 = sv0_vec_new();
  int efs4 = _sv0t80;
  int _sv0t81 = sv0_vec_new();
  int pn4 = _sv0t81;
  int _sv0t82 = sv0_vec_new();
  int pc4 = _sv0t82;
  int _sv0t83 = sv0_vec_new();
  int ins4 = _sv0t83;
  Instr _sv0t84;
  _sv0t84.tag = 1;
  _sv0t84.p0 = 50;
  int _sv0t85 = sv0_box_alloc(5);
  sv0_box_store(_sv0t85, 0, _sv0t84.tag);
  sv0_box_store(_sv0t85, 1, _sv0t84.p0);
  sv0_box_store(_sv0t85, 2, _sv0t84.p1);
  sv0_box_store(_sv0t85, 3, _sv0t84.p2);
  sv0_box_store(_sv0t85, 4, _sv0t84.p3);
  sv0_vec_push(ins4, _sv0t85);
  Instr _sv0t86;
  _sv0t86.tag = 1;
  _sv0t86.p0 = 50;
  int _sv0t87 = sv0_box_alloc(5);
  sv0_box_store(_sv0t87, 0, _sv0t86.tag);
  sv0_box_store(_sv0t87, 1, _sv0t86.p0);
  sv0_box_store(_sv0t87, 2, _sv0t86.p1);
  sv0_box_store(_sv0t87, 3, _sv0t86.p2);
  sv0_box_store(_sv0t87, 4, _sv0t86.p3);
  sv0_vec_push(ins4, _sv0t87);
  int _sv0t88 = sv0_vec_new();
  int _sv0t89 = sv0_vec_new();
  int _sv0t90 = sv0_vec_new();
  int _sv0t91 = sv0_vec_new();
  int _sv0t92 = sv0_vec_new();
  int _sv0t93 = local_count_and_env(pn4, pc4, ins4, en4, eb4, ew4, ec4, efs4, sn, sfc, enn, enw, _sv0t88, _sv0t89, _sv0t90, "", _sv0t91, _sv0t92);
  int lc4 = _sv0t93;
  if ((lc4 != 1)) {
    return 17;
  } else {
  }
  int _sv0t94 = sv0_vec_len(en4);
  if ((_sv0t94 != 1)) {
    return 18;
  } else {
  }
  int _sv0t95 = sv0_vec_new();
  int en5 = _sv0t95;
  int _sv0t96 = sv0_vec_new();
  int eb5 = _sv0t96;
  int _sv0t97 = sv0_vec_new();
  int ew5 = _sv0t97;
  int _sv0t98 = sv0_vec_new();
  int ec5 = _sv0t98;
  int _sv0t99 = sv0_vec_new();
  int efs5 = _sv0t99;
  int _sv0t100 = sv0_vec_new();
  int pn5 = _sv0t100;
  int _sv0t101 = sv0_vec_new();
  int pc5 = _sv0t101;
  int _sv0t102 = sv0_vec_new();
  int ins5 = _sv0t102;
  Instr _sv0t103;
  Expr _sv0t104;
  Value _sv0t105;
  _sv0t105.tag = 0;
  _sv0t105.p0 = 7;
  int _sv0t106 = sv0_box_alloc(3);
  sv0_box_store(_sv0t106, 0, _sv0t105.tag);
  sv0_box_store(_sv0t106, 1, _sv0t105.p0);
  sv0_box_store(_sv0t106, 2, _sv0t105.p1);
  _sv0t104.tag = 0;
  _sv0t104.p0 = _sv0t106;
  int _sv0t107 = sv0_box_alloc(4);
  sv0_box_store(_sv0t107, 0, _sv0t104.tag);
  sv0_box_store(_sv0t107, 1, _sv0t104.p0);
  sv0_box_store(_sv0t107, 2, _sv0t104.p1);
  sv0_box_store(_sv0t107, 3, _sv0t104.p2);
  _sv0t103.tag = 3;
  _sv0t103.p0 = 60;
  _sv0t103.p1 = _sv0t107;
  int _sv0t108 = sv0_box_alloc(5);
  sv0_box_store(_sv0t108, 0, _sv0t103.tag);
  sv0_box_store(_sv0t108, 1, _sv0t103.p0);
  sv0_box_store(_sv0t108, 2, _sv0t103.p1);
  sv0_box_store(_sv0t108, 3, _sv0t103.p2);
  sv0_box_store(_sv0t108, 4, _sv0t103.p3);
  sv0_vec_push(ins5, _sv0t108);
  int _sv0t109 = sv0_vec_new();
  int _sv0t110 = sv0_vec_new();
  int _sv0t111 = sv0_vec_new();
  int _sv0t112 = sv0_vec_new();
  int _sv0t113 = sv0_vec_new();
  int _sv0t114 = local_count_and_env(pn5, pc5, ins5, en5, eb5, ew5, ec5, efs5, sn, sfc, enn, enw, _sv0t109, _sv0t110, _sv0t111, "", _sv0t112, _sv0t113);
  int lc5 = _sv0t114;
  if ((lc5 != 1)) {
    return 19;
  } else {
  }
  int _sv0t115 = sv0_vec_get(en5, 0);
  if ((_sv0t115 != 60)) {
    return 20;
  } else {
  }
  int _sv0t116 = sv0_vec_new();
  int en6 = _sv0t116;
  int _sv0t117 = sv0_vec_new();
  int eb6 = _sv0t117;
  int _sv0t118 = sv0_vec_new();
  int ew6 = _sv0t118;
  int _sv0t119 = sv0_vec_new();
  int ec6 = _sv0t119;
  int _sv0t120 = sv0_vec_new();
  int efs6 = _sv0t120;
  int _sv0t121 = sv0_vec_new();
  int pn6 = _sv0t121;
  int _sv0t122 = sv0_vec_new();
  int pc6 = _sv0t122;
  int _sv0t123 = sv0_vec_new();
  int ins6 = _sv0t123;
  int _sv0t124 = sv0_vec_new();
  int cargs = _sv0t124;
  Instr _sv0t125;
  _sv0t125.tag = 11;
  _sv0t125.p0 = 70;
  _sv0t125.p1 = 100;
  _sv0t125.p2 = cargs;
  _sv0t125.p3 = 999;
  int _sv0t126 = sv0_box_alloc(5);
  sv0_box_store(_sv0t126, 0, _sv0t125.tag);
  sv0_box_store(_sv0t126, 1, _sv0t125.p0);
  sv0_box_store(_sv0t126, 2, _sv0t125.p1);
  sv0_box_store(_sv0t126, 3, _sv0t125.p2);
  sv0_box_store(_sv0t126, 4, _sv0t125.p3);
  sv0_vec_push(ins6, _sv0t126);
  int _sv0t127 = sv0_vec_new();
  int _sv0t128 = sv0_vec_new();
  int _sv0t129 = sv0_vec_new();
  int _sv0t130 = sv0_vec_new();
  int _sv0t131 = sv0_vec_new();
  int _sv0t132 = local_count_and_env(pn6, pc6, ins6, en6, eb6, ew6, ec6, efs6, sn, sfc, enn, enw, _sv0t127, _sv0t128, _sv0t129, "", _sv0t130, _sv0t131);
  int lc6 = _sv0t132;
  if ((lc6 != 1)) {
    return 21;
  } else {
  }
  int _sv0t133 = sv0_vec_get(en6, 0);
  if ((_sv0t133 != 70)) {
    return 22;
  } else {
  }
  int _sv0t134 = sv0_vec_new();
  int en7 = _sv0t134;
  int _sv0t135 = sv0_vec_new();
  int eb7 = _sv0t135;
  int _sv0t136 = sv0_vec_new();
  int ew7 = _sv0t136;
  int _sv0t137 = sv0_vec_new();
  int ec7 = _sv0t137;
  int _sv0t138 = sv0_vec_new();
  int efs7 = _sv0t138;
  int _sv0t139 = sv0_vec_new();
  int pn7 = _sv0t139;
  int _sv0t140 = sv0_vec_new();
  int pc7 = _sv0t140;
  int _sv0t141 = sv0_vec_new();
  int ins7 = _sv0t141;
  int _sv0t142 = sv0_vec_new();
  int cargs2 = _sv0t142;
  Instr _sv0t143;
  _sv0t143.tag = 11;
  _sv0t143.p0 = 0;
  _sv0t143.p1 = 100;
  _sv0t143.p2 = cargs2;
  _sv0t143.p3 = 999;
  int _sv0t144 = sv0_box_alloc(5);
  sv0_box_store(_sv0t144, 0, _sv0t143.tag);
  sv0_box_store(_sv0t144, 1, _sv0t143.p0);
  sv0_box_store(_sv0t144, 2, _sv0t143.p1);
  sv0_box_store(_sv0t144, 3, _sv0t143.p2);
  sv0_box_store(_sv0t144, 4, _sv0t143.p3);
  sv0_vec_push(ins7, _sv0t144);
  int _sv0t145 = sv0_vec_new();
  int _sv0t146 = sv0_vec_new();
  int _sv0t147 = sv0_vec_new();
  int _sv0t148 = sv0_vec_new();
  int _sv0t149 = sv0_vec_new();
  int _sv0t150 = local_count_and_env(pn7, pc7, ins7, en7, eb7, ew7, ec7, efs7, sn, sfc, enn, enw, _sv0t145, _sv0t146, _sv0t147, "", _sv0t148, _sv0t149);
  int lc7 = _sv0t150;
  if ((lc7 != 0)) {
    return 23;
  } else {
  }
  int _sv0t151 = sv0_vec_len(en7);
  if ((_sv0t151 != 0)) {
    return 24;
  } else {
  }
  int _sv0t152 = sv0_vec_new();
  int en8 = _sv0t152;
  int _sv0t153 = sv0_vec_new();
  int eb8 = _sv0t153;
  int _sv0t154 = sv0_vec_new();
  int ew8 = _sv0t154;
  int _sv0t155 = sv0_vec_new();
  int ec8 = _sv0t155;
  int _sv0t156 = sv0_vec_new();
  int efs8 = _sv0t156;
  int _sv0t157 = sv0_vec_new();
  int pn8 = _sv0t157;
  int _sv0t158 = sv0_vec_new();
  int pc8 = _sv0t158;
  int _sv0t159 = sv0_vec_new();
  int ins8 = _sv0t159;
  int _sv0t160 = sv0_vec_new();
  int cargs3 = _sv0t160;
  Instr _sv0t161;
  _sv0t161.tag = 11;
  _sv0t161.p0 = 80;
  _sv0t161.p1 = 100;
  _sv0t161.p2 = cargs3;
  _sv0t161.p3 = 500;
  int _sv0t162 = sv0_box_alloc(5);
  sv0_box_store(_sv0t162, 0, _sv0t161.tag);
  sv0_box_store(_sv0t162, 1, _sv0t161.p0);
  sv0_box_store(_sv0t162, 2, _sv0t161.p1);
  sv0_box_store(_sv0t162, 3, _sv0t161.p2);
  sv0_box_store(_sv0t162, 4, _sv0t161.p3);
  sv0_vec_push(ins8, _sv0t162);
  int _sv0t163 = sv0_vec_new();
  int _sv0t164 = sv0_vec_new();
  int _sv0t165 = sv0_vec_new();
  int _sv0t166 = sv0_vec_new();
  int _sv0t167 = sv0_vec_new();
  int _sv0t168 = local_count_and_env(pn8, pc8, ins8, en8, eb8, ew8, ec8, efs8, sn2, sfc2, enn, enw, _sv0t163, _sv0t164, _sv0t165, "", _sv0t166, _sv0t167);
  int lc8 = _sv0t168;
  if ((lc8 != 3)) {
    return 25;
  } else {
  }
  int _sv0t169 = sv0_vec_get(ew8, 0);
  if ((_sv0t169 != 3)) {
    return 26;
  } else {
  }
  int _sv0t170 = sv0_vec_new();
  int en9 = _sv0t170;
  int _sv0t171 = sv0_vec_new();
  int eb9 = _sv0t171;
  int _sv0t172 = sv0_vec_new();
  int ew9 = _sv0t172;
  int _sv0t173 = sv0_vec_new();
  int ec9 = _sv0t173;
  int _sv0t174 = sv0_vec_new();
  int efs9 = _sv0t174;
  int _sv0t175 = sv0_vec_new();
  int pn9 = _sv0t175;
  int _sv0t176 = sv0_vec_new();
  int pc9 = _sv0t176;
  int _sv0t177 = sv0_vec_new();
  int ins9 = _sv0t177;
  int _sv0t178 = sv0_vec_new();
  int th9 = _sv0t178;
  int _sv0t179 = sv0_vec_new();
  int el9 = _sv0t179;
  Instr _sv0t180;
  _sv0t180.tag = 1;
  _sv0t180.p0 = 90;
  int _sv0t181 = sv0_box_alloc(5);
  sv0_box_store(_sv0t181, 0, _sv0t180.tag);
  sv0_box_store(_sv0t181, 1, _sv0t180.p0);
  sv0_box_store(_sv0t181, 2, _sv0t180.p1);
  sv0_box_store(_sv0t181, 3, _sv0t180.p2);
  sv0_box_store(_sv0t181, 4, _sv0t180.p3);
  sv0_vec_push(th9, _sv0t181);
  Instr _sv0t182;
  _sv0t182.tag = 1;
  _sv0t182.p0 = 91;
  int _sv0t183 = sv0_box_alloc(5);
  sv0_box_store(_sv0t183, 0, _sv0t182.tag);
  sv0_box_store(_sv0t183, 1, _sv0t182.p0);
  sv0_box_store(_sv0t183, 2, _sv0t182.p1);
  sv0_box_store(_sv0t183, 3, _sv0t182.p2);
  sv0_box_store(_sv0t183, 4, _sv0t182.p3);
  sv0_vec_push(el9, _sv0t183);
  Instr _sv0t184;
  Expr _sv0t185;
  Value _sv0t186;
  _sv0t186.tag = 1;
  int _sv0t187 = sv0_box_alloc(3);
  sv0_box_store(_sv0t187, 0, _sv0t186.tag);
  sv0_box_store(_sv0t187, 1, _sv0t186.p0);
  sv0_box_store(_sv0t187, 2, _sv0t186.p1);
  _sv0t185.tag = 0;
  _sv0t185.p0 = _sv0t187;
  int _sv0t188 = sv0_box_alloc(4);
  sv0_box_store(_sv0t188, 0, _sv0t185.tag);
  sv0_box_store(_sv0t188, 1, _sv0t185.p0);
  sv0_box_store(_sv0t188, 2, _sv0t185.p1);
  sv0_box_store(_sv0t188, 3, _sv0t185.p2);
  _sv0t184.tag = 6;
  _sv0t184.p0 = _sv0t188;
  _sv0t184.p1 = th9;
  _sv0t184.p2 = el9;
  int _sv0t189 = sv0_box_alloc(5);
  sv0_box_store(_sv0t189, 0, _sv0t184.tag);
  sv0_box_store(_sv0t189, 1, _sv0t184.p0);
  sv0_box_store(_sv0t189, 2, _sv0t184.p1);
  sv0_box_store(_sv0t189, 3, _sv0t184.p2);
  sv0_box_store(_sv0t189, 4, _sv0t184.p3);
  sv0_vec_push(ins9, _sv0t189);
  int _sv0t190 = sv0_vec_new();
  int _sv0t191 = sv0_vec_new();
  int _sv0t192 = sv0_vec_new();
  int _sv0t193 = sv0_vec_new();
  int _sv0t194 = sv0_vec_new();
  int _sv0t195 = local_count_and_env(pn9, pc9, ins9, en9, eb9, ew9, ec9, efs9, sn, sfc, enn, enw, _sv0t190, _sv0t191, _sv0t192, "", _sv0t193, _sv0t194);
  int lc9 = _sv0t195;
  if ((lc9 != 2)) {
    return 27;
  } else {
  }
  int _sv0t196 = sv0_vec_get(en9, 0);
  if ((_sv0t196 != 90)) {
    return 28;
  } else {
  }
  int _sv0t197 = sv0_vec_get(en9, 1);
  if ((_sv0t197 != 91)) {
    return 29;
  } else {
  }
  int _sv0t198 = sv0_vec_new();
  int en10 = _sv0t198;
  int _sv0t199 = sv0_vec_new();
  int eb10 = _sv0t199;
  int _sv0t200 = sv0_vec_new();
  int ew10 = _sv0t200;
  int _sv0t201 = sv0_vec_new();
  int ec10 = _sv0t201;
  int _sv0t202 = sv0_vec_new();
  int efs10 = _sv0t202;
  int _sv0t203 = sv0_vec_new();
  int pn10 = _sv0t203;
  int _sv0t204 = sv0_vec_new();
  int pc10 = _sv0t204;
  int _sv0t205 = sv0_vec_new();
  int ins10 = _sv0t205;
  int _sv0t206 = sv0_vec_new();
  int wbod = _sv0t206;
  Instr _sv0t207;
  _sv0t207.tag = 1;
  _sv0t207.p0 = 95;
  int _sv0t208 = sv0_box_alloc(5);
  sv0_box_store(_sv0t208, 0, _sv0t207.tag);
  sv0_box_store(_sv0t208, 1, _sv0t207.p0);
  sv0_box_store(_sv0t208, 2, _sv0t207.p1);
  sv0_box_store(_sv0t208, 3, _sv0t207.p2);
  sv0_box_store(_sv0t208, 4, _sv0t207.p3);
  sv0_vec_push(wbod, _sv0t208);
  Instr _sv0t209;
  Expr _sv0t210;
  Value _sv0t211;
  _sv0t211.tag = 1;
  int _sv0t212 = sv0_box_alloc(3);
  sv0_box_store(_sv0t212, 0, _sv0t211.tag);
  sv0_box_store(_sv0t212, 1, _sv0t211.p0);
  sv0_box_store(_sv0t212, 2, _sv0t211.p1);
  _sv0t210.tag = 0;
  _sv0t210.p0 = _sv0t212;
  int _sv0t213 = sv0_box_alloc(4);
  sv0_box_store(_sv0t213, 0, _sv0t210.tag);
  sv0_box_store(_sv0t213, 1, _sv0t210.p0);
  sv0_box_store(_sv0t213, 2, _sv0t210.p1);
  sv0_box_store(_sv0t213, 3, _sv0t210.p2);
  _sv0t209.tag = 7;
  _sv0t209.p0 = _sv0t213;
  _sv0t209.p1 = wbod;
  int _sv0t214 = sv0_box_alloc(5);
  sv0_box_store(_sv0t214, 0, _sv0t209.tag);
  sv0_box_store(_sv0t214, 1, _sv0t209.p0);
  sv0_box_store(_sv0t214, 2, _sv0t209.p1);
  sv0_box_store(_sv0t214, 3, _sv0t209.p2);
  sv0_box_store(_sv0t214, 4, _sv0t209.p3);
  sv0_vec_push(ins10, _sv0t214);
  int _sv0t215 = sv0_vec_new();
  int _sv0t216 = sv0_vec_new();
  int _sv0t217 = sv0_vec_new();
  int _sv0t218 = sv0_vec_new();
  int _sv0t219 = sv0_vec_new();
  int _sv0t220 = local_count_and_env(pn10, pc10, ins10, en10, eb10, ew10, ec10, efs10, sn, sfc, enn, enw, _sv0t215, _sv0t216, _sv0t217, "", _sv0t218, _sv0t219);
  int lc10 = _sv0t220;
  if ((lc10 != 1)) {
    return 30;
  } else {
  }
  int _sv0t221 = sv0_vec_get(en10, 0);
  if ((_sv0t221 != 95)) {
    return 31;
  } else {
  }
  int _sv0t222 = sv0_vec_new();
  int en11 = _sv0t222;
  int _sv0t223 = sv0_vec_new();
  int eb11 = _sv0t223;
  int _sv0t224 = sv0_vec_new();
  int ew11 = _sv0t224;
  int _sv0t225 = sv0_vec_new();
  int ec11 = _sv0t225;
  int _sv0t226 = sv0_vec_new();
  int efs11 = _sv0t226;
  int _sv0t227 = sv0_vec_new();
  int pn11 = _sv0t227;
  int _sv0t228 = sv0_vec_new();
  int pc11 = _sv0t228;
  sv0_vec_push(pn11, 200);
  sv0_vec_push(pc11, 777);
  int _sv0t229 = sv0_vec_new();
  int ins11 = _sv0t229;
  Instr _sv0t230;
  _sv0t230.tag = 1;
  _sv0t230.p0 = 201;
  int _sv0t231 = sv0_box_alloc(5);
  sv0_box_store(_sv0t231, 0, _sv0t230.tag);
  sv0_box_store(_sv0t231, 1, _sv0t230.p0);
  sv0_box_store(_sv0t231, 2, _sv0t230.p1);
  sv0_box_store(_sv0t231, 3, _sv0t230.p2);
  sv0_box_store(_sv0t231, 4, _sv0t230.p3);
  sv0_vec_push(ins11, _sv0t231);
  Instr _sv0t232;
  _sv0t232.tag = 1;
  _sv0t232.p0 = 202;
  int _sv0t233 = sv0_box_alloc(5);
  sv0_box_store(_sv0t233, 0, _sv0t232.tag);
  sv0_box_store(_sv0t233, 1, _sv0t232.p0);
  sv0_box_store(_sv0t233, 2, _sv0t232.p1);
  sv0_box_store(_sv0t233, 3, _sv0t232.p2);
  sv0_box_store(_sv0t233, 4, _sv0t232.p3);
  sv0_vec_push(ins11, _sv0t233);
  int _sv0t234 = sv0_vec_new();
  int _sv0t235 = sv0_vec_new();
  int _sv0t236 = sv0_vec_new();
  int _sv0t237 = sv0_vec_new();
  int _sv0t238 = sv0_vec_new();
  int _sv0t239 = local_count_and_env(pn11, pc11, ins11, en11, eb11, ew11, ec11, efs11, sn, sfc, enn, enw, _sv0t234, _sv0t235, _sv0t236, "", _sv0t237, _sv0t238);
  int lc11 = _sv0t239;
  if ((lc11 != 3)) {
    return 32;
  } else {
  }
  int _sv0t240 = sv0_vec_len(en11);
  if ((_sv0t240 != 3)) {
    return 33;
  } else {
  }
  int _sv0t241 = sv0_vec_get(eb11, 0);
  if ((_sv0t241 != 0)) {
    return 34;
  } else {
  }
  int _sv0t242 = sv0_vec_get(eb11, 1);
  if ((_sv0t242 != 1)) {
    return 35;
  } else {
  }
  int _sv0t243 = sv0_vec_get(eb11, 2);
  if ((_sv0t243 != 2)) {
    return 36;
  } else {
  }
  int _sv0t244 = sv0_vec_new();
  int en12 = _sv0t244;
  int _sv0t245 = sv0_vec_new();
  int eb12 = _sv0t245;
  int _sv0t246 = sv0_vec_new();
  int ew12 = _sv0t246;
  int _sv0t247 = sv0_vec_new();
  int ec12 = _sv0t247;
  int _sv0t248 = sv0_vec_new();
  int efs12 = _sv0t248;
  int _sv0t249 = sv0_vec_new();
  int pn12 = _sv0t249;
  int _sv0t250 = sv0_vec_new();
  int pc12 = _sv0t250;
  int _sv0t251 = sv0_vec_new();
  int ins12 = _sv0t251;
  Instr _sv0t252;
  _sv0t252.tag = 0;
  int _sv0t253 = sv0_box_alloc(5);
  sv0_box_store(_sv0t253, 0, _sv0t252.tag);
  sv0_box_store(_sv0t253, 1, _sv0t252.p0);
  sv0_box_store(_sv0t253, 2, _sv0t252.p1);
  sv0_box_store(_sv0t253, 3, _sv0t252.p2);
  sv0_box_store(_sv0t253, 4, _sv0t252.p3);
  sv0_vec_push(ins12, _sv0t253);
  Instr _sv0t254;
  _sv0t254.tag = 9;
  int _sv0t255 = sv0_box_alloc(5);
  sv0_box_store(_sv0t255, 0, _sv0t254.tag);
  sv0_box_store(_sv0t255, 1, _sv0t254.p0);
  sv0_box_store(_sv0t255, 2, _sv0t254.p1);
  sv0_box_store(_sv0t255, 3, _sv0t254.p2);
  sv0_box_store(_sv0t255, 4, _sv0t254.p3);
  sv0_vec_push(ins12, _sv0t255);
  Instr _sv0t256;
  _sv0t256.tag = 10;
  int _sv0t257 = sv0_box_alloc(5);
  sv0_box_store(_sv0t257, 0, _sv0t256.tag);
  sv0_box_store(_sv0t257, 1, _sv0t256.p0);
  sv0_box_store(_sv0t257, 2, _sv0t256.p1);
  sv0_box_store(_sv0t257, 3, _sv0t256.p2);
  sv0_box_store(_sv0t257, 4, _sv0t256.p3);
  sv0_vec_push(ins12, _sv0t257);
  Instr _sv0t258;
  _sv0t258.tag = 14;
  int _sv0t259 = sv0_box_alloc(5);
  sv0_box_store(_sv0t259, 0, _sv0t258.tag);
  sv0_box_store(_sv0t259, 1, _sv0t258.p0);
  sv0_box_store(_sv0t259, 2, _sv0t258.p1);
  sv0_box_store(_sv0t259, 3, _sv0t258.p2);
  sv0_box_store(_sv0t259, 4, _sv0t258.p3);
  sv0_vec_push(ins12, _sv0t259);
  int _sv0t260 = sv0_vec_new();
  int _sv0t261 = sv0_vec_new();
  int _sv0t262 = sv0_vec_new();
  int _sv0t263 = sv0_vec_new();
  int _sv0t264 = sv0_vec_new();
  int _sv0t265 = local_count_and_env(pn12, pc12, ins12, en12, eb12, ew12, ec12, efs12, sn, sfc, enn, enw, _sv0t260, _sv0t261, _sv0t262, "", _sv0t263, _sv0t264);
  int lc12 = _sv0t265;
  if ((lc12 != 0)) {
    return 37;
  } else {
  }
  int _sv0t266 = sv0_vec_len(en12);
  if ((_sv0t266 != 0)) {
    return 38;
  } else {
  }
  return 0;
}

static int test_is_main_label(void) {
  const char* src;
  src = "main foo";
  int _sv0t0 = sv0_vec_new();
  int st = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int en = _sv0t1;
  sv0_vec_push(st, 0);
  sv0_vec_push(en, 4);
  sv0_vec_push(st, 5);
  sv0_vec_push(en, 8);
  int _sv0t2 = is_main_label(0, src, st, en);
  if ((_sv0t2 != 1)) {
    return 1;
  } else {
  }
  int _sv0t3 = is_main_label(1, src, st, en);
  if ((_sv0t3 != 0)) {
    return 2;
  } else {
  }
  return 0;
}

static int test_build_func_order(void) {
  const char* src;
  src = "main foo";
  int _sv0t0 = sv0_vec_new();
  int st = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int en = _sv0t1;
  sv0_vec_push(st, 0);
  sv0_vec_push(en, 4);
  sv0_vec_push(st, 5);
  sv0_vec_push(en, 8);
  int _sv0t2 = sv0_vec_new();
  int labs = _sv0t2;
  sv0_vec_push(labs, 0);
  sv0_vec_push(labs, 1);
  int _sv0t3 = sv0_vec_new();
  int ord = _sv0t3;
  int _sv0t4 = build_func_order(labs, src, st, en, ord);
  int fc = _sv0t4;
  if ((fc != 2)) {
    return 1;
  } else {
  }
  int _sv0t5 = sv0_vec_get(ord, 0);
  if ((_sv0t5 != 1)) {
    return 2;
  } else {
  }
  int _sv0t6 = sv0_vec_get(ord, 1);
  if ((_sv0t6 != 0)) {
    return 3;
  } else {
  }
  return 0;
}

static int emit_program_1fn(const char* source, int starts, int ends, int lab, int pn, int pc, int ins, int pool, int ft) {
  int _sv0t0 = sv0_vec_new();
  int it = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int in2 = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int ifc = _sv0t2;
  int _sv0t3 = sv0_vec_new();
  int ivp = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int bl = _sv0t4;
  sv0_vec_push(bl, lab);
  int _sv0t5 = sv0_vec_new();
  int bpn = _sv0t5;
  sv0_vec_push(bpn, pn);
  int _sv0t6 = sv0_vec_new();
  int bpc = _sv0t6;
  sv0_vec_push(bpc, pc);
  int _sv0t7 = sv0_vec_new();
  int bi = _sv0t7;
  sv0_vec_push(bi, ins);
  int _sv0t8 = sv0_vec_new();
  int _sv0t9 = sv0_vec_new();
  int _sv0t10 = emit_program(it, in2, ifc, ivp, _sv0t8, _sv0t9, bl, bpn, bpc, bi, source, starts, ends, pool, ft);
  return _sv0t10;
}

static int test_emit_program(void) {
  const char* src;
  src = "main";
  int _sv0t0 = sv0_vec_new();
  int st = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int en = _sv0t1;
  sv0_vec_push(st, 0);
  sv0_vec_push(en, 4);
  int _sv0t2 = sv0_vec_new();
  int pn = _sv0t2;
  int _sv0t3 = sv0_vec_new();
  int pc = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int ins = _sv0t4;
  Instr _sv0t5;
  Value _sv0t6;
  _sv0t6.tag = 0;
  _sv0t6.p0 = 42;
  int _sv0t7 = sv0_box_alloc(3);
  sv0_box_store(_sv0t7, 0, _sv0t6.tag);
  sv0_box_store(_sv0t7, 1, _sv0t6.p0);
  sv0_box_store(_sv0t7, 2, _sv0t6.p1);
  _sv0t5.tag = 15;
  _sv0t5.p0 = _sv0t7;
  int _sv0t8 = sv0_box_alloc(5);
  sv0_box_store(_sv0t8, 0, _sv0t5.tag);
  sv0_box_store(_sv0t8, 1, _sv0t5.p0);
  sv0_box_store(_sv0t8, 2, _sv0t5.p1);
  sv0_box_store(_sv0t8, 3, _sv0t5.p2);
  sv0_box_store(_sv0t8, 4, _sv0t5.p3);
  sv0_vec_push(ins, _sv0t8);
  int _sv0t9 = sv0_vec_new();
  int pool = _sv0t9;
  int _sv0t10 = sv0_vec_new();
  int ft = _sv0t10;
  int _sv0t11 = emit_program_1fn(src, st, en, 0, pn, pc, ins, pool, ft);
  int fc = _sv0t11;
  if ((fc != 1)) {
    return 1;
  } else {
  }
  int _sv0t12 = sv0_vec_len(ft);
  if ((_sv0t12 != 4)) {
    return 2;
  } else {
  }
  int _sv0t13 = sv0_vec_get(ft, 1);
  if ((_sv0t13 != 0)) {
    return 3;
  } else {
  }
  return 0;
}

static int bi_from(int v) {
  int _sv0t0 = bi_zero();
  int a = _sv0t0;
  int _sv0t1 = bi_set_u32(a, v);
  return a;
}

static int test_bigint(void) {
  int _sv0t0 = bi_from(5);
  int _sv0t1 = bi_from(7);
  int _sv0t2 = bi_cmp(_sv0t0, _sv0t1);
  int _sv0t3 = (0 - 1);
  if ((_sv0t2 != _sv0t3)) {
    return 1;
  } else {
  }
  int _sv0t4 = bi_from(7);
  int _sv0t5 = bi_from(5);
  int _sv0t6 = bi_cmp(_sv0t4, _sv0t5);
  if ((_sv0t6 != 1)) {
    return 2;
  } else {
  }
  int _sv0t7 = bi_from(42);
  int _sv0t8 = bi_from(42);
  int _sv0t9 = bi_cmp(_sv0t7, _sv0t8);
  if ((_sv0t9 != 0)) {
    return 3;
  } else {
  }
  int _sv0t10 = bi_from(0);
  int _sv0t11 = bi_is_zero(_sv0t10);
  if ((_sv0t11 != 1)) {
    return 4;
  } else {
  }
  int _sv0t12 = bi_from(1);
  int _sv0t13 = bi_is_zero(_sv0t12);
  if ((_sv0t13 != 0)) {
    return 5;
  } else {
  }
  int _sv0t14 = bi_from(12345);
  int _sv0t15 = bi_bitlen(_sv0t14);
  if ((_sv0t15 != 14)) {
    return 6;
  } else {
  }
  int _sv0t16 = bi_from(0);
  int _sv0t17 = bi_bitlen(_sv0t16);
  if ((_sv0t17 != 0)) {
    return 7;
  } else {
  }
  int _sv0t18 = bi_from(1);
  int _sv0t19 = bi_bitlen(_sv0t18);
  if ((_sv0t19 != 1)) {
    return 8;
  } else {
  }
  int _sv0t20 = bi_from(100);
  int m1 = _sv0t20;
  int _sv0t21 = bi_mul_small(m1, 10);
  int _sv0t22 = bi_from(1000);
  int _sv0t23 = bi_cmp(m1, _sv0t22);
  if ((_sv0t23 != 0)) {
    return 9;
  } else {
  }
  int _sv0t24 = bi_from(5);
  int m2 = _sv0t24;
  int _sv0t25 = bi_mul_pow10(m2, 4);
  int _sv0t26 = bi_from(50000);
  int _sv0t27 = bi_cmp(m2, _sv0t26);
  if ((_sv0t27 != 0)) {
    return 10;
  } else {
  }
  int _sv0t28 = bi_from(1);
  int s1 = _sv0t28;
  int _sv0t29 = bi_shl_bits(s1, 40);
  int _sv0t30 = bi_bitlen(s1);
  if ((_sv0t30 != 41)) {
    return 11;
  } else {
  }
  int _sv0t31 = bi_bit(s1, 40);
  if ((_sv0t31 != 1)) {
    return 12;
  } else {
  }
  int _sv0t32 = bi_bit(s1, 39);
  if ((_sv0t32 != 0)) {
    return 13;
  } else {
  }
  int _sv0t33 = bi_from(3);
  int s2 = _sv0t33;
  int _sv0t34 = bi_shl_bits(s2, 15);
  int _sv0t35 = bi_from(98304);
  int _sv0t36 = bi_cmp(s2, _sv0t35);
  if ((_sv0t36 != 0)) {
    return 14;
  } else {
  }
  int _sv0t37 = bi_from(1000);
  int d1 = _sv0t37;
  int _sv0t38 = bi_from(1);
  int _sv0t39 = bi_sub(d1, _sv0t38);
  int _sv0t40 = bi_from(999);
  int _sv0t41 = bi_cmp(d1, _sv0t40);
  if ((_sv0t41 != 0)) {
    return 15;
  } else {
  }
  int _sv0t42 = bi_from(65536);
  int d2 = _sv0t42;
  int _sv0t43 = bi_from(1);
  int _sv0t44 = bi_sub(d2, _sv0t43);
  int _sv0t45 = bi_from(65535);
  int _sv0t46 = bi_cmp(d2, _sv0t45);
  if ((_sv0t46 != 0)) {
    return 16;
  } else {
  }
  int _sv0t47 = bi_from(7);
  int base = _sv0t47;
  int _sv0t48 = bi_copy(base);
  int cp = _sv0t48;
  int _sv0t49 = bi_mul_small(cp, 3);
  int _sv0t50 = bi_from(7);
  int _sv0t51 = bi_cmp(base, _sv0t50);
  if ((_sv0t51 != 0)) {
    return 17;
  } else {
  }
  int _sv0t52 = bi_from(21);
  int _sv0t53 = bi_cmp(cp, _sv0t52);
  if ((_sv0t53 != 0)) {
    return 18;
  } else {
  }
  return 0;
}

static int pfl_check(const char* s, int want_neg, int want_exp, int want_digs) {
  int _sv0t0 = sv0_vec_new();
  int dg = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int mt = _sv0t1;
  int _sv0t2 = parse_f64_literal(s, dg, mt);
  if ((_sv0t2 != 0)) {
    return 1;
  } else {
  }
  int _sv0t3 = sv0_vec_get(mt, 0);
  if ((_sv0t3 != want_neg)) {
    return 2;
  } else {
  }
  int _sv0t4 = sv0_vec_get(mt, 1);
  if ((_sv0t4 != want_exp)) {
    return 3;
  } else {
  }
  int _sv0t5 = sv0_vec_len(dg);
  int _sv0t6 = sv0_vec_len(want_digs);
  if ((_sv0t5 != _sv0t6)) {
    return 4;
  } else {
  }
  int i = 0;
  while (1) {
    int _sv0t7 = sv0_vec_len(dg);
    int _sv0t10 = (i < _sv0t7);
    if ((!_sv0t10)) {
      break;
    } else {
    }
    int _sv0t8 = sv0_vec_get(dg, i);
    int _sv0t9 = sv0_vec_get(want_digs, i);
    if ((_sv0t8 != _sv0t9)) {
      return 5;
    } else {
    }
    i = (i + 1);
  }
  return 0;
}

static int dv(int a, int b, int c, int d, int e, int f, int cnt) {
  int _sv0t0 = sv0_vec_new();
  int v = _sv0t0;
  if ((cnt >= 1)) {
    sv0_vec_push(v, a);
  } else {
  }
  if ((cnt >= 2)) {
    sv0_vec_push(v, b);
  } else {
  }
  if ((cnt >= 3)) {
    sv0_vec_push(v, c);
  } else {
  }
  if ((cnt >= 4)) {
    sv0_vec_push(v, d);
  } else {
  }
  if ((cnt >= 5)) {
    sv0_vec_push(v, e);
  } else {
  }
  if ((cnt >= 6)) {
    sv0_vec_push(v, f);
  } else {
  }
  return v;
}

static int test_parse_f64_literal(void) {
  int _sv0t0 = (0 - 5);
  int _sv0t1 = dv(3, 1, 4, 1, 5, 9, 6);
  int _sv0t2 = pfl_check("3.14159", 0, _sv0t0, _sv0t1);
  if ((_sv0t2 != 0)) {
    return 1;
  } else {
  }
  int _sv0t3 = dv(1, 0, 0, 0, 0, 0, 1);
  int _sv0t4 = pfl_check("1e10", 0, 10, _sv0t3);
  if ((_sv0t4 != 0)) {
    return 2;
  } else {
  }
  int _sv0t5 = (0 - 3);
  int _sv0t6 = dv(1, 0, 0, 0, 0, 0, 1);
  int _sv0t7 = pfl_check("0.001", 0, _sv0t5, _sv0t6);
  if ((_sv0t7 != 0)) {
    return 3;
  } else {
  }
  int _sv0t8 = (0 - 1);
  int _sv0t9 = dv(2, 5, 0, 0, 0, 0, 2);
  int _sv0t10 = pfl_check("2.5", 0, _sv0t8, _sv0t9);
  if ((_sv0t10 != 0)) {
    return 4;
  } else {
  }
  int _sv0t11 = (0 - 1);
  int _sv0t12 = dv(2, 5, 0, 0, 0, 0, 2);
  int _sv0t13 = pfl_check("-2.5", 1, _sv0t11, _sv0t12);
  if ((_sv0t13 != 0)) {
    return 5;
  } else {
  }
  int _sv0t14 = (0 - 1);
  int _sv0t15 = dv(1, 0, 0, 0, 0, 0, 4);
  int _sv0t16 = pfl_check("100.0", 0, _sv0t14, _sv0t15);
  if ((_sv0t16 != 0)) {
    return 6;
  } else {
  }
  int _sv0t17 = (0 - 1);
  int _sv0t18 = dv(0, 0, 0, 0, 0, 0, 1);
  int _sv0t19 = pfl_check("0.0", 0, _sv0t17, _sv0t18);
  if ((_sv0t19 != 0)) {
    return 7;
  } else {
  }
  int _sv0t20 = dv(1, 0, 0, 0, 0, 0, 1);
  int _sv0t21 = pfl_check("1E3", 0, 3, _sv0t20);
  if ((_sv0t21 != 0)) {
    return 8;
  } else {
  }
  int _sv0t22 = (0 - 4);
  int _sv0t23 = dv(2, 5, 0, 0, 0, 0, 2);
  int _sv0t24 = pfl_check("2.5e-3", 0, _sv0t22, _sv0t23);
  if ((_sv0t24 != 0)) {
    return 9;
  } else {
  }
  int _sv0t25 = sv0_vec_new();
  int dg = _sv0t25;
  int _sv0t26 = sv0_vec_new();
  int mt = _sv0t26;
  int _sv0t27 = parse_f64_literal("1.2.3", dg, mt);
  int _sv0t28 = (0 - 1);
  if ((_sv0t27 != _sv0t28)) {
    return 10;
  } else {
  }
  int _sv0t29 = sv0_vec_new();
  int dg2 = _sv0t29;
  int _sv0t30 = sv0_vec_new();
  int mt2 = _sv0t30;
  int _sv0t31 = parse_f64_literal("1e", dg2, mt2);
  int _sv0t32 = (0 - 1);
  if ((_sv0t31 != _sv0t32)) {
    return 11;
  } else {
  }
  int _sv0t33 = sv0_vec_new();
  int dg3 = _sv0t33;
  int _sv0t34 = sv0_vec_new();
  int mt3 = _sv0t34;
  int _sv0t35 = parse_f64_literal("abc", dg3, mt3);
  int _sv0t36 = (0 - 1);
  if ((_sv0t35 != _sv0t36)) {
    return 12;
  } else {
  }
  int _sv0t37 = sv0_vec_new();
  int dg4 = _sv0t37;
  int _sv0t38 = sv0_vec_new();
  int mt4 = _sv0t38;
  int _sv0t39 = parse_f64_literal("", dg4, mt4);
  int _sv0t40 = (0 - 1);
  if ((_sv0t39 != _sv0t40)) {
    return 13;
  } else {
  }
  return 0;
}

static int test_wide_int_literal(void) {
  const char* src;
  src = "4294967296 9223372036854775807 18446744073709551615";
  int _sv0t0 = sv0_vec_new();
  int st = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int en = _sv0t1;
  sv0_vec_push(st, 0);
  sv0_vec_push(en, 10);
  sv0_vec_push(st, 11);
  sv0_vec_push(en, 30);
  sv0_vec_push(st, 31);
  sv0_vec_push(en, 51);
  int _sv0t2 = sv0_vec_new();
  int o1 = _sv0t2;
  int _sv0t3 = push_wide_int_literal(0, src, st, en, o1);
  int r1 = _sv0t3;
  if ((r1 != 3)) {
    return 1;
  } else {
  }
  int _sv0t4 = sv0_vec_get(o1, 0);
  if ((_sv0t4 != 5)) {
    return 2;
  } else {
  }
  int _sv0t5 = sv0_vec_get(o1, 1);
  if ((_sv0t5 != 0)) {
    return 3;
  } else {
  }
  int _sv0t6 = sv0_vec_get(o1, 2);
  if ((_sv0t6 != 1)) {
    return 4;
  } else {
  }
  int _sv0t7 = sv0_vec_new();
  int o2 = _sv0t7;
  int _sv0t8 = push_wide_int_literal(1, src, st, en, o2);
  int r2 = _sv0t8;
  int _sv0t9 = sv0_vec_get(o2, 1);
  int _sv0t10 = (0 - 1);
  if ((_sv0t9 != _sv0t10)) {
    return 5;
  } else {
  }
  int _sv0t11 = sv0_vec_get(o2, 2);
  if ((_sv0t11 != 2147483647)) {
    return 6;
  } else {
  }
  int _sv0t12 = sv0_vec_new();
  int o3 = _sv0t12;
  int _sv0t13 = push_wide_int_literal(2, src, st, en, o3);
  int r3 = _sv0t13;
  int _sv0t14 = sv0_vec_get(o3, 1);
  int _sv0t15 = (0 - 1);
  if ((_sv0t14 != _sv0t15)) {
    return 7;
  } else {
  }
  int _sv0t16 = sv0_vec_get(o3, 2);
  int _sv0t17 = (0 - 1);
  if ((_sv0t16 != _sv0t17)) {
    return 8;
  } else {
  }
  int _sv0t18 = sv0_vec_new();
  int o4 = _sv0t18;
  int _sv0t19 = sv0_vec_new();
  int pool = _sv0t19;
  Value _sv0t20;
  _sv0t20.tag = 9;
  _sv0t20.p0 = 0;
  int _sv0t21 = sv0_vec_new();
  int _sv0t22 = sv0_vec_new();
  int _sv0t23 = sv0_vec_new();
  int _sv0t24 = sv0_vec_new();
  int _sv0t25 = sv0_vec_new();
  int _sv0t26 = emit_value(_sv0t20, _sv0t21, _sv0t22, _sv0t23, _sv0t24, _sv0t25, pool, src, st, en, o4);
  int r4 = _sv0t26;
  if ((r4 != 3)) {
    return 9;
  } else {
  }
  int _sv0t27 = sv0_vec_get(o4, 0);
  if ((_sv0t27 != 5)) {
    return 10;
  } else {
  }
  int _sv0t28 = sv0_vec_get(o4, 2);
  if ((_sv0t28 != 1)) {
    return 11;
  } else {
  }
  const char* bad;
  bad = "12x4";
  int _sv0t29 = sv0_vec_new();
  int bst = _sv0t29;
  int _sv0t30 = sv0_vec_new();
  int ben = _sv0t30;
  sv0_vec_push(bst, 0);
  sv0_vec_push(ben, 4);
  int _sv0t31 = sv0_vec_new();
  int o5 = _sv0t31;
  int _sv0t32 = push_wide_int_literal(0, bad, bst, ben, o5);
  int _sv0t33 = (0 - 1);
  if ((_sv0t32 != _sv0t33)) {
    return 12;
  } else {
  }
  return 0;
}

static int test_typed_opcode_select(void) {
  int _sv0t0 = combine_cat(0, 0);
  if ((_sv0t0 != 0)) {
    return 1;
  } else {
  }
  int _sv0t1 = combine_cat(0, 2);
  if ((_sv0t1 != 2)) {
    return 2;
  } else {
  }
  int _sv0t2 = combine_cat(2, 1);
  if ((_sv0t2 != 1)) {
    return 3;
  } else {
  }
  int _sv0t3 = combine_cat(1, 0);
  if ((_sv0t3 != 1)) {
    return 4;
  } else {
  }
  int _sv0t4 = binop_tag_to_insn_typed(0, 0);
  if ((_sv0t4 != 16)) {
    return 10;
  } else {
  }
  int _sv0t5 = binop_tag_to_insn_typed(4, 0);
  if ((_sv0t5 != 20)) {
    return 11;
  } else {
  }
  int _sv0t6 = binop_tag_to_insn_typed(13, 0);
  if ((_sv0t6 != 88)) {
    return 12;
  } else {
  }
  int _sv0t7 = binop_tag_to_insn_typed(0, 1);
  if ((_sv0t7 != 48)) {
    return 20;
  } else {
  }
  int _sv0t8 = binop_tag_to_insn_typed(1, 1);
  if ((_sv0t8 != 49)) {
    return 21;
  } else {
  }
  int _sv0t9 = binop_tag_to_insn_typed(2, 1);
  if ((_sv0t9 != 50)) {
    return 22;
  } else {
  }
  int _sv0t10 = binop_tag_to_insn_typed(3, 1);
  if ((_sv0t10 != 51)) {
    return 23;
  } else {
  }
  int _sv0t11 = binop_tag_to_insn_typed(4, 1);
  int _sv0t12 = (0 - 1);
  if ((_sv0t11 != _sv0t12)) {
    return 24;
  } else {
  }
  int _sv0t13 = binop_tag_to_insn_typed(7, 1);
  if ((_sv0t13 != 66)) {
    return 25;
  } else {
  }
  int _sv0t14 = binop_tag_to_insn_typed(13, 1);
  int _sv0t15 = (0 - 1);
  if ((_sv0t14 != _sv0t15)) {
    return 26;
  } else {
  }
  int _sv0t16 = binop_tag_to_insn_typed(0, 2);
  if ((_sv0t16 != 32)) {
    return 30;
  } else {
  }
  int _sv0t17 = binop_tag_to_insn_typed(3, 2);
  if ((_sv0t17 != 35)) {
    return 31;
  } else {
  }
  int _sv0t18 = binop_tag_to_insn_typed(4, 2);
  if ((_sv0t18 != 36)) {
    return 32;
  } else {
  }
  int _sv0t19 = binop_tag_to_insn_typed(8, 2);
  if ((_sv0t19 != 67)) {
    return 33;
  } else {
  }
  int _sv0t20 = unop_tag_to_insn_typed(0, 0);
  if ((_sv0t20 != 21)) {
    return 40;
  } else {
  }
  int _sv0t21 = unop_tag_to_insn_typed(0, 1);
  if ((_sv0t21 != 52)) {
    return 41;
  } else {
  }
  int _sv0t22 = unop_tag_to_insn_typed(0, 2);
  if ((_sv0t22 != 37)) {
    return 42;
  } else {
  }
  int _sv0t23 = unop_tag_to_insn_typed(1, 1);
  if ((_sv0t23 != 82)) {
    return 43;
  } else {
  }
  int _sv0t24 = sv0_vec_new();
  int ec = _sv0t24;
  sv0_vec_push(ec, 0);
  sv0_vec_push(ec, 1);
  sv0_vec_push(ec, 2);
  int _sv0t25 = (0 - 1);
  int _sv0t26 = slot_cat(ec, _sv0t25);
  if ((_sv0t26 != 0)) {
    return 50;
  } else {
  }
  int _sv0t27 = slot_cat(ec, 1);
  if ((_sv0t27 != 1)) {
    return 51;
  } else {
  }
  int _sv0t28 = slot_cat(ec, 2);
  if ((_sv0t28 != 2)) {
    return 52;
  } else {
  }
  int _sv0t29 = slot_cat(ec, 9);
  if ((_sv0t29 != 0)) {
    return 53;
  } else {
  }
  int _sv0t30 = sv0_vec_new();
  int en = _sv0t30;
  int _sv0t31 = sv0_vec_new();
  int eb = _sv0t31;
  int _sv0t32 = sv0_vec_new();
  int ew = _sv0t32;
  Value _sv0t33;
  _sv0t33.tag = 8;
  _sv0t33.p0 = 0;
  int _sv0t34 = sv0_vec_new();
  int _sv0t35 = sv0_vec_new();
  int _sv0t36 = operand_cat(_sv0t33, en, eb, ew, ec, "", _sv0t34, _sv0t35);
  if ((_sv0t36 != 1)) {
    return 60;
  } else {
  }
  Value _sv0t37;
  _sv0t37.tag = 9;
  _sv0t37.p0 = 0;
  int _sv0t38 = sv0_vec_new();
  int _sv0t39 = sv0_vec_new();
  int _sv0t40 = operand_cat(_sv0t37, en, eb, ew, ec, "", _sv0t38, _sv0t39);
  if ((_sv0t40 != 2)) {
    return 61;
  } else {
  }
  Value _sv0t41;
  _sv0t41.tag = 0;
  _sv0t41.p0 = 7;
  int _sv0t42 = sv0_vec_new();
  int _sv0t43 = sv0_vec_new();
  int _sv0t44 = operand_cat(_sv0t41, en, eb, ew, ec, "", _sv0t42, _sv0t43);
  if ((_sv0t44 != 0)) {
    return 62;
  } else {
  }
  sv0_vec_push(en, 50);
  sv0_vec_push(eb, 0);
  sv0_vec_push(ew, 1);
  int _sv0t45 = sv0_vec_new();
  int ec2 = _sv0t45;
  sv0_vec_push(ec2, 1);
  Value _sv0t46;
  _sv0t46.tag = 3;
  _sv0t46.p0 = 50;
  int _sv0t47 = sv0_vec_new();
  int _sv0t48 = sv0_vec_new();
  int _sv0t49 = operand_cat(_sv0t46, en, eb, ew, ec2, "", _sv0t47, _sv0t48);
  if ((_sv0t49 != 1)) {
    return 63;
  } else {
  }
  return 0;
}

int main(void) {
  int _sv0t0 = test_variant_slots();
  int r1 = _sv0t0;
  if ((r1 != 0)) {
    return r1;
  } else {
  }
  int _sv0t1 = test_cty_classify();
  int r2 = _sv0t1;
  if ((r2 != 0)) {
    int _sv0t2 = (10 + r2);
    return _sv0t2;
  } else {
  }
  int _sv0t3 = test_index_of_field();
  int r3 = _sv0t3;
  if ((r3 != 0)) {
    int _sv0t4 = (20 + r3);
    return _sv0t4;
  } else {
  }
  int _sv0t5 = test_binop_to_insn();
  int r4 = _sv0t5;
  if ((r4 != 0)) {
    int _sv0t6 = (30 + r4);
    return _sv0t6;
  } else {
  }
  int _sv0t7 = test_unop_to_insn();
  int r5 = _sv0t7;
  if ((r5 != 0)) {
    int _sv0t8 = (50 + r5);
    return _sv0t8;
  } else {
  }
  int _sv0t9 = test_enc_len();
  int r6 = _sv0t9;
  if ((r6 != 0)) {
    int _sv0t10 = (60 + r6);
    return _sv0t10;
  } else {
  }
  int _sv0t11 = test_sentinels();
  int r7 = _sv0t11;
  if ((r7 != 0)) {
    int _sv0t12 = (70 + r7);
    return _sv0t12;
  } else {
  }
  int _sv0t13 = test_pool();
  int r8 = _sv0t13;
  if ((r8 != 0)) {
    int _sv0t14 = (80 + r8);
    return _sv0t14;
  } else {
  }
  int _sv0t15 = test_builtin_id();
  int r9 = _sv0t15;
  if ((r9 != 0)) {
    int _sv0t16 = (90 + r9);
    return _sv0t16;
  } else {
  }
  int _sv0t17 = test_store_ordering();
  int r10 = _sv0t17;
  if ((r10 != 0)) {
    int _sv0t18 = (110 + r10);
    return _sv0t18;
  } else {
  }
  int _sv0t19 = test_width_of_cty();
  int r11 = _sv0t19;
  if ((r11 != 0)) {
    int _sv0t20 = (120 + r11);
    return _sv0t20;
  } else {
  }
  int _sv0t21 = test_enum_field_helpers();
  int r12 = _sv0t21;
  if ((r12 != 0)) {
    int _sv0t22 = (130 + r12);
    return _sv0t22;
  } else {
  }
  int _sv0t23 = test_layout_lookup();
  int r13 = _sv0t23;
  if ((r13 != 0)) {
    int _sv0t24 = (140 + r13);
    return _sv0t24;
  } else {
  }
  int _sv0t25 = test_lookup_slot();
  int r14 = _sv0t25;
  if ((r14 != 0)) {
    int _sv0t26 = (150 + r14);
    return _sv0t26;
  } else {
  }
  int _sv0t27 = test_alloc_local();
  int r15 = _sv0t27;
  if ((r15 != 0)) {
    int _sv0t28 = (160 + r15);
    return _sv0t28;
  } else {
  }
  int _sv0t29 = test_replace_loop_exit();
  int r16 = _sv0t29;
  if ((r16 != 0)) {
    int _sv0t30 = (170 + r16);
    return _sv0t30;
  } else {
  }
  int _sv0t31 = test_enum_field_names();
  int r17 = _sv0t31;
  if ((r17 != 0)) {
    int _sv0t32 = (180 + r17);
    return _sv0t32;
  } else {
  }
  int _sv0t33 = test_value_width();
  int r18 = _sv0t33;
  if ((r18 != 0)) {
    int _sv0t34 = (190 + r18);
    return _sv0t34;
  } else {
  }
  int _sv0t35 = test_bind_param();
  int r19 = _sv0t35;
  if ((r19 != 0)) {
    int _sv0t36 = (200 + r19);
    return _sv0t36;
  } else {
  }
  int _sv0t37 = test_patch_continue_jumps();
  int r20 = _sv0t37;
  if ((r20 != 0)) {
    int _sv0t38 = (210 + r20);
    return _sv0t38;
  } else {
  }
  int _sv0t39 = test_struct_layouts_build();
  int r21 = _sv0t39;
  if ((r21 != 0)) {
    int _sv0t40 = (220 + r21);
    return _sv0t40;
  } else {
  }
  int _sv0t41 = test_enum_layouts_build();
  int r22 = _sv0t41;
  if ((r22 != 0)) {
    int _sv0t42 = (230 + r22);
    return _sv0t42;
  } else {
  }
  int _sv0t43 = test_emit_value();
  int r23 = _sv0t43;
  if ((r23 != 0)) {
    int _sv0t44 = (240 + r23);
    return _sv0t44;
  } else {
  }
  int _sv0t45 = test_emit_expr();
  int r24 = _sv0t45;
  if ((r24 != 0)) {
    int _sv0t46 = (270 + r24);
    return _sv0t46;
  } else {
  }
  int _sv0t47 = test_emit_instr();
  int r25 = _sv0t47;
  if ((r25 != 0)) {
    int _sv0t48 = (300 + r25);
    return _sv0t48;
  } else {
  }
  int _sv0t49 = test_emit_instrs();
  int r26 = _sv0t49;
  if ((r26 != 0)) {
    int _sv0t50 = (350 + r26);
    return _sv0t50;
  } else {
  }
  int _sv0t51 = test_vec_append();
  int r27 = _sv0t51;
  if ((r27 != 0)) {
    int _sv0t52 = (370 + r27);
    return _sv0t52;
  } else {
  }
  int _sv0t53 = test_flat_byte_size();
  int r28 = _sv0t53;
  if ((r28 != 0)) {
    int _sv0t54 = (380 + r28);
    return _sv0t54;
  } else {
  }
  int _sv0t55 = test_emit_ifelse_block();
  int r29 = _sv0t55;
  if ((r29 != 0)) {
    int _sv0t56 = (390 + r29);
    return _sv0t56;
  } else {
  }
  int _sv0t57 = test_emit_call();
  int r30 = _sv0t57;
  if ((r30 != 0)) {
    int _sv0t58 = (430 + r30);
    return _sv0t58;
  } else {
  }
  int _sv0t59 = test_emit_while_loop();
  int r31 = _sv0t59;
  if ((r31 != 0)) {
    int _sv0t60 = (470 + r31);
    return _sv0t60;
  } else {
  }
  int _sv0t61 = test_emit_store_field();
  int r32 = _sv0t61;
  if ((r32 != 0)) {
    int _sv0t62 = (510 + r32);
    return _sv0t62;
  } else {
  }
  int _sv0t63 = test_emit_contract();
  int r33 = _sv0t63;
  if ((r33 != 0)) {
    int _sv0t64 = (540 + r33);
    return _sv0t64;
  } else {
  }
  int _sv0t65 = test_local_count_and_env();
  int r34 = _sv0t65;
  if ((r34 != 0)) {
    int _sv0t66 = (570 + r34);
    return _sv0t66;
  } else {
  }
  int _sv0t67 = test_compute_arity();
  int r35 = _sv0t67;
  if ((r35 != 0)) {
    int _sv0t68 = (610 + r35);
    return _sv0t68;
  } else {
  }
  int _sv0t69 = test_emit_fn();
  int r36 = _sv0t69;
  if ((r36 != 0)) {
    int _sv0t70 = (620 + r36);
    return _sv0t70;
  } else {
  }
  int _sv0t71 = test_is_main_label();
  int r37 = _sv0t71;
  if ((r37 != 0)) {
    int _sv0t72 = (650 + r37);
    return _sv0t72;
  } else {
  }
  int _sv0t73 = test_build_func_order();
  int r38 = _sv0t73;
  if ((r38 != 0)) {
    int _sv0t74 = (660 + r38);
    return _sv0t74;
  } else {
  }
  int _sv0t75 = test_emit_program();
  int r39 = _sv0t75;
  if ((r39 != 0)) {
    int _sv0t76 = (670 + r39);
    return _sv0t76;
  } else {
  }
  int _sv0t77 = test_typed_opcode_select();
  int r40 = _sv0t77;
  if ((r40 != 0)) {
    int _sv0t78 = (690 + r40);
    return _sv0t78;
  } else {
  }
  int _sv0t79 = test_wide_int_literal();
  int r41 = _sv0t79;
  if ((r41 != 0)) {
    int _sv0t80 = (710 + r41);
    return _sv0t80;
  } else {
  }
  int _sv0t81 = test_bigint();
  int r42 = _sv0t81;
  if ((r42 != 0)) {
    int _sv0t82 = (730 + r42);
    return _sv0t82;
  } else {
  }
  int _sv0t83 = test_parse_f64_literal();
  int r43 = _sv0t83;
  if ((r43 != 0)) {
    int _sv0t84 = (760 + r43);
    return _sv0t84;
  } else {
  }
  return 0;
}


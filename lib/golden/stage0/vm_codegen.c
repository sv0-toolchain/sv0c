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
static int width_of_cty(int cty_handle, int structs_names, int structs_field_counts, int enums_names, int enums_widths);
static int width_of_cty_scalar(const char* cty);
static int width_of_cty_simple(const char* cty);
static int index_of_field(int fields, int field_name);
static int index_of_field_range(int fields_flat, int start, int count, int name);
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
static int pool_add(int pool, int s);
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
static int lookup_slot(int env_names, int env_bases, int env_widths, int name);
static int slot_base(int env_bases, int idx);
static int slot_width(int env_widths, int idx);
static int alloc_local(int env_names, int env_bases, int env_widths, int name, int width, int slot);
static int bind_param(int name, int cty_handle, int slot, int env_names, int env_bases, int env_widths, int env_fields, int structs_names, int structs_field_counts, int enums_names, int enums_widths);
static int replace_loop_exit_jump(int instrs, int back_offset);
static int insn_vec_stride(int opc);
static int patch_continue_jumps(int instrs, int from_pos, int target_abs);
static int patch_break_jumps(int instrs, int from_pos, int target_abs);
static int struct_layouts_build(int item_tags, int item_names, int item_field_counts, int out_names, int out_field_counts);
static int enum_layouts_build(int item_tags, int item_names, int item_variant_counts, int item_variant_max_payload, int out_names, int out_widths);
static int binop_handle_to_insn(int op_h, const char* source, int starts, int ends);
static int unop_handle_to_insn(int op_h, const char* source, int starts, int ends);
static const char* handle_to_str(int h, const char* source, int starts, int ends);
static int index_of_fn(int fn_h, int fn_names, const char* source, int starts, int ends);
static int member_offset_from_field(int width, int field_h);
static int emit_member_load_from_var(int x, int field_h, int env_names, int env_bases, int env_widths, int out);
static int emit_value(Value v, int env_names, int env_bases, int env_widths, int pool, int out);
static int emit_expr(Expr e, int env_names, int env_bases, int env_widths, int pool, const char* source, int starts, int ends, int out);
static int vec_append(int dst, int src);
static int flat_byte_size(int instrs);
static int emit_ret_width(Value v, int env_names, int env_bases, int env_widths);
static int emit_instr(Instr ins, int env_names, int env_bases, int env_widths, int env_field_starts, int env_fields_flat, int pool, const char* source, int starts, int ends, int fn_names, int out);
static int emit_instrs(int instrs, int env_names, int env_bases, int env_widths, int env_field_starts, int env_fields_flat, int pool, const char* source, int starts, int ends, int fn_names, int out);
static int ensure_var_int(int name, int env_names, int env_bases, int env_widths, int env_field_starts, int next_slot);
static int scan_instr_env(Instr ins, int env_names, int env_bases, int env_widths, int env_field_starts, int next_slot, int structs_names, int structs_field_counts, int enums_names, int enums_widths);
static int scan_instrs_env(int instrs, int env_names, int env_bases, int env_widths, int env_field_starts, int next_slot, int structs_names, int structs_field_counts, int enums_names, int enums_widths);
static int local_count_and_env(int param_names, int param_ctys, int instrs, int env_names, int env_bases, int env_widths, int env_field_starts, int structs_names, int structs_field_counts, int enums_names, int enums_widths);
static int compute_arity(int param_ctys, int structs_names, int structs_field_counts, int enums_names, int enums_widths);
static int emit_fn(int label, int param_names, int param_ctys, int instrs, int structs_names, int structs_field_counts, int enums_names, int enums_widths, int pool, const char* source, int starts, int ends, int fn_names, int ft_entries);
static int is_main_label(int label, const char* source, int starts, int ends);
static int build_func_order(int block_labels, const char* source, int starts, int ends, int order);
static int emit_program(int item_tags, int item_names, int item_field_counts, int item_variant_max_payload, int block_labels, int block_param_names, int block_param_ctys, int block_instrs, const char* source, int starts, int ends, int out_pool, int out_ft);
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

static int width_of_cty(int cty_handle, int structs_names, int structs_field_counts, int enums_names, int enums_widths) {
  int _sv0t0 = sv0_vec_len(structs_names);
  int sn = _sv0t0;
  int si = 0;
  while ((si < sn)) {
    int _sv0t1 = sv0_vec_get(structs_names, si);
    if ((_sv0t1 == cty_handle)) {
      int _sv0t2 = sv0_vec_get(structs_field_counts, si);
      return _sv0t2;
    } else {
    }
    si = (si + 1);
  }
  int _sv0t3 = sv0_vec_len(enums_names);
  int en = _sv0t3;
  int ei = 0;
  while ((ei < en)) {
    int _sv0t4 = sv0_vec_get(enums_names, ei);
    if ((_sv0t4 == cty_handle)) {
      int _sv0t5 = sv0_vec_get(enums_widths, ei);
      return _sv0t5;
    } else {
    }
    ei = (ei + 1);
  }
  return 1;
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

static int index_of_field_range(int fields_flat, int start, int count, int name) {
  int i = 0;
  while ((i < count)) {
    int _sv0t0 = (start + i);
    int _sv0t1 = sv0_vec_get(fields_flat, _sv0t0);
    if ((_sv0t1 == name)) {
      return i;
    } else {
    }
    i = (i + 1);
  }
  int _sv0t2 = (0 - 1);
  return _sv0t2;
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

static int pool_add(int pool, int s) {
  int _sv0t0 = sv0_vec_len(pool);
  int n = _sv0t0;
  int i = 0;
  while ((i < n)) {
    int _sv0t1 = sv0_vec_get(pool, i);
    if ((_sv0t1 == s)) {
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
    int _sv0t0 = lookup_slot(env_names, env_bases, env_widths, var_name);
    int idx = _sv0t0;
    if ((idx >= 0)) {
      int _sv0t1 = slot_width(env_widths, idx);
      return _sv0t1;
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

static int lookup_slot(int env_names, int env_bases, int env_widths, int name) {
  int _sv0t0 = sv0_vec_len(env_names);
  int n = _sv0t0;
  int i = (n - 1);
  while ((i >= 0)) {
    int _sv0t1 = sv0_vec_get(env_names, i);
    if ((_sv0t1 == name)) {
      return i;
    } else {
    }
    i = (i - 1);
  }
  int _sv0t2 = (0 - 1);
  return _sv0t2;
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
  int _sv0t0 = width_of_cty(cty_handle, structs_names, structs_field_counts, enums_names, enums_widths);
  int w = _sv0t0;
  sv0_vec_push(env_names, name);
  sv0_vec_push(env_bases, slot);
  sv0_vec_push(env_widths, w);
  int _sv0t1 = sv0_vec_len(env_fields);
  sv0_vec_push(env_fields, _sv0t1);
  int _sv0t2 = (slot + w);
  return _sv0t2;
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
  if ((off < 0)) {
    int _sv0t2 = (0 - 1);
    return _sv0t2;
  } else {
  }
  if ((off >= width)) {
    int _sv0t3 = (0 - 1);
    return _sv0t3;
  } else {
  }
  return off;
}

static int emit_member_load_from_var(int x, int field_h, int env_names, int env_bases, int env_widths, int out) {
  int _sv0t0 = lookup_slot(env_names, env_bases, env_widths, x);
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
  if ((off < 0)) {
    int _sv0t5 = (0 - 1);
    return _sv0t5;
  } else {
  }
  sv0_vec_push(out, 96);
  int _sv0t6 = (base + off);
  sv0_vec_push(out, _sv0t6);
  return 2;
}

static int emit_value(Value v, int env_names, int env_bases, int env_widths, int pool, int out) {
  int _sv0t0;
  if ((v.tag == 0)) {
    int n = v.p0;
    sv0_vec_push(out, 4);
    sv0_vec_push(out, n);
    return 2;
    _sv0t0 = 0;
  } else {
    if ((v.tag == 1)) {
      int h = v.p0;
      int _sv0t25 = (0 - 1);
      return _sv0t25;
      _sv0t0 = 0;
    } else {
      if ((v.tag == 2)) {
        int b = v.p0;
        sv0_vec_push(out, 7);
        if (b) {
          sv0_vec_push(out, 1);
        } else {
          sv0_vec_push(out, 0);
        }
        return 2;
        _sv0t0 = 0;
      } else {
        if ((v.tag == 3)) {
          int s = v.p0;
          int _sv0t24 = pool_add(pool, s);
          int idx = _sv0t24;
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
            if ((v.tag == 5)) {
              int x = v.p0;
              int _sv0t18 = lookup_slot(env_names, env_bases, env_widths, x);
              int si = _sv0t18;
              if ((si < 0)) {
                int _sv0t19 = (0 - 1);
                return _sv0t19;
              } else {
              }
              int _sv0t20 = slot_base(env_bases, si);
              int base = _sv0t20;
              int _sv0t21 = slot_width(env_widths, si);
              int width = _sv0t21;
              int k = 0;
              while ((k < width)) {
                sv0_vec_push(out, 96);
                int _sv0t22 = (base + k);
                sv0_vec_push(out, _sv0t22);
                k = (k + 1);
              }
              int _sv0t23 = (width * 2);
              return _sv0t23;
              _sv0t0 = 0;
            } else {
              if ((v.tag == 6)) {
                int bv = v.p0;
                int f = v.p1;
                Value _sv0t4;
                int _sv0t5 = sv0_box_load(bv, 0);
                _sv0t4.tag = _sv0t5;
                int _sv0t6 = sv0_box_load(bv, 1);
                _sv0t4.p0 = _sv0t6;
                int _sv0t7 = sv0_box_load(bv, 2);
                _sv0t4.p1 = _sv0t7;
                Value base_v;
                base_v.tag = (_sv0t4).tag;
                base_v.p0 = (_sv0t4).p0;
                base_v.p1 = (_sv0t4).p1;
                int _sv0t8;
                if ((base_v.tag == 5)) {
                  int x = base_v.p0;
                  int _sv0t16 = emit_member_load_from_var(x, f, env_names, env_bases, env_widths, out);
                  return _sv0t16;
                  _sv0t8 = 0;
                } else {
                  if ((base_v.tag == 0)) {
                    int n = base_v.p0;
                    int _sv0t15 = (0 - 1);
                    return _sv0t15;
                    _sv0t8 = 0;
                  } else {
                    if ((base_v.tag == 1)) {
                      int h = base_v.p0;
                      int _sv0t14 = (0 - 1);
                      return _sv0t14;
                      _sv0t8 = 0;
                    } else {
                      if ((base_v.tag == 2)) {
                        int b = base_v.p0;
                        int _sv0t13 = (0 - 1);
                        return _sv0t13;
                        _sv0t8 = 0;
                      } else {
                        if ((base_v.tag == 3)) {
                          int s = base_v.p0;
                          int _sv0t12 = (0 - 1);
                          return _sv0t12;
                          _sv0t8 = 0;
                        } else {
                          if ((base_v.tag == 4)) {
                            int _sv0t11 = (0 - 1);
                            return _sv0t11;
                            _sv0t8 = 0;
                          } else {
                            if ((base_v.tag == 6)) {
                              int bv2 = base_v.p0;
                              int f2 = base_v.p1;
                              int _sv0t10 = (0 - 1);
                              return _sv0t10;
                              _sv0t8 = 0;
                            } else {
                              if ((base_v.tag == 7)) {
                                int h2 = base_v.p0;
                                int _sv0t9 = (0 - 1);
                                return _sv0t9;
                                _sv0t8 = 0;
                              } else {
                              }
                            }
                          }
                        }
                      }
                    }
                  }
                }
                int _sv0t17 = (0 - 1);
                return _sv0t17;
                _sv0t0 = 0;
              } else {
                if ((v.tag == 7)) {
                  int x = v.p0;
                  int _sv0t1 = lookup_slot(env_names, env_bases, env_widths, x);
                  int si = _sv0t1;
                  if ((si < 0)) {
                    int _sv0t2 = (0 - 1);
                    return _sv0t2;
                  } else {
                  }
                  int _sv0t3 = slot_base(env_bases, si);
                  int base = _sv0t3;
                  sv0_vec_push(out, 4);
                  sv0_vec_push(out, base);
                  return 2;
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
  int _sv0t26 = (0 - 1);
  return _sv0t26;
}

static int emit_expr(Expr e, int env_names, int env_bases, int env_widths, int pool, const char* source, int starts, int ends, int out) {
  int _sv0t0;
  if ((e.tag == 0)) {
    int bv = e.p0;
    Value _sv0t45;
    int _sv0t46 = sv0_box_load(bv, 0);
    _sv0t45.tag = _sv0t46;
    int _sv0t47 = sv0_box_load(bv, 1);
    _sv0t45.p0 = _sv0t47;
    int _sv0t48 = sv0_box_load(bv, 2);
    _sv0t45.p1 = _sv0t48;
    Value v;
    v.tag = (_sv0t45).tag;
    v.p0 = (_sv0t45).p0;
    v.p1 = (_sv0t45).p1;
    int _sv0t49 = emit_value(v, env_names, env_bases, env_widths, pool, out);
    return _sv0t49;
    _sv0t0 = 0;
  } else {
    if ((e.tag == 3)) {
      int x = e.p0;
      int _sv0t39 = lookup_slot(env_names, env_bases, env_widths, x);
      int si = _sv0t39;
      if ((si < 0)) {
        int _sv0t40 = (0 - 1);
        return _sv0t40;
      } else {
      }
      int _sv0t41 = slot_base(env_bases, si);
      int base = _sv0t41;
      int _sv0t42 = slot_width(env_widths, si);
      int width = _sv0t42;
      int k = 0;
      while ((k < width)) {
        sv0_vec_push(out, 96);
        int _sv0t43 = (base + k);
        sv0_vec_push(out, _sv0t43);
        k = (k + 1);
      }
      int _sv0t44 = (width * 2);
      return _sv0t44;
      _sv0t0 = 0;
    } else {
      if ((e.tag == 1)) {
        int op_h = e.p0;
        int bv1 = e.p1;
        int bv2 = e.p2;
        Value _sv0t24;
        int _sv0t25 = sv0_box_load(bv1, 0);
        _sv0t24.tag = _sv0t25;
        int _sv0t26 = sv0_box_load(bv1, 1);
        _sv0t24.p0 = _sv0t26;
        int _sv0t27 = sv0_box_load(bv1, 2);
        _sv0t24.p1 = _sv0t27;
        Value v1;
        v1.tag = (_sv0t24).tag;
        v1.p0 = (_sv0t24).p0;
        v1.p1 = (_sv0t24).p1;
        Value _sv0t28;
        int _sv0t29 = sv0_box_load(bv2, 0);
        _sv0t28.tag = _sv0t29;
        int _sv0t30 = sv0_box_load(bv2, 1);
        _sv0t28.p0 = _sv0t30;
        int _sv0t31 = sv0_box_load(bv2, 2);
        _sv0t28.p1 = _sv0t31;
        Value v2;
        v2.tag = (_sv0t28).tag;
        v2.p0 = (_sv0t28).p0;
        v2.p1 = (_sv0t28).p1;
        int _sv0t32 = emit_value(v1, env_names, env_bases, env_widths, pool, out);
        int n1 = _sv0t32;
        if ((n1 < 0)) {
          int _sv0t33 = (0 - 1);
          return _sv0t33;
        } else {
        }
        int _sv0t34 = emit_value(v2, env_names, env_bases, env_widths, pool, out);
        int n2 = _sv0t34;
        if ((n2 < 0)) {
          int _sv0t35 = (0 - 1);
          return _sv0t35;
        } else {
        }
        int _sv0t36 = binop_handle_to_insn(op_h, source, starts, ends);
        int opc = _sv0t36;
        sv0_vec_push(out, opc);
        int _sv0t37 = (n1 + n2);
        int _sv0t38 = (_sv0t37 + 1);
        return _sv0t38;
        _sv0t0 = 0;
      } else {
        if ((e.tag == 2)) {
          int op_h = e.p0;
          int bv = e.p1;
          Value _sv0t16;
          int _sv0t17 = sv0_box_load(bv, 0);
          _sv0t16.tag = _sv0t17;
          int _sv0t18 = sv0_box_load(bv, 1);
          _sv0t16.p0 = _sv0t18;
          int _sv0t19 = sv0_box_load(bv, 2);
          _sv0t16.p1 = _sv0t19;
          Value v;
          v.tag = (_sv0t16).tag;
          v.p0 = (_sv0t16).p0;
          v.p1 = (_sv0t16).p1;
          int _sv0t20 = emit_value(v, env_names, env_bases, env_widths, pool, out);
          int n = _sv0t20;
          if ((n < 0)) {
            int _sv0t21 = (0 - 1);
            return _sv0t21;
          } else {
          }
          int _sv0t22 = unop_handle_to_insn(op_h, source, starts, ends);
          int opc = _sv0t22;
          if ((opc >= 0)) {
            sv0_vec_push(out, opc);
            int _sv0t23 = (n + 1);
            return _sv0t23;
          } else {
          }
          return n;
          _sv0t0 = 0;
        } else {
          if ((e.tag == 4)) {
            int bv = e.p0;
            int f = e.p1;
            Value _sv0t2;
            int _sv0t3 = sv0_box_load(bv, 0);
            _sv0t2.tag = _sv0t3;
            int _sv0t4 = sv0_box_load(bv, 1);
            _sv0t2.p0 = _sv0t4;
            int _sv0t5 = sv0_box_load(bv, 2);
            _sv0t2.p1 = _sv0t5;
            Value base_v;
            base_v.tag = (_sv0t2).tag;
            base_v.p0 = (_sv0t2).p0;
            base_v.p1 = (_sv0t2).p1;
            int _sv0t6;
            if ((base_v.tag == 5)) {
              int x = base_v.p0;
              int _sv0t14 = emit_member_load_from_var(x, f, env_names, env_bases, env_widths, out);
              return _sv0t14;
              _sv0t6 = 0;
            } else {
              if ((base_v.tag == 0)) {
                int n = base_v.p0;
                int _sv0t13 = (0 - 1);
                return _sv0t13;
                _sv0t6 = 0;
              } else {
                if ((base_v.tag == 1)) {
                  int h = base_v.p0;
                  int _sv0t12 = (0 - 1);
                  return _sv0t12;
                  _sv0t6 = 0;
                } else {
                  if ((base_v.tag == 2)) {
                    int b = base_v.p0;
                    int _sv0t11 = (0 - 1);
                    return _sv0t11;
                    _sv0t6 = 0;
                  } else {
                    if ((base_v.tag == 3)) {
                      int s = base_v.p0;
                      int _sv0t10 = (0 - 1);
                      return _sv0t10;
                      _sv0t6 = 0;
                    } else {
                      if ((base_v.tag == 4)) {
                        int _sv0t9 = (0 - 1);
                        return _sv0t9;
                        _sv0t6 = 0;
                      } else {
                        if ((base_v.tag == 6)) {
                          int bv2 = base_v.p0;
                          int f2 = base_v.p1;
                          int _sv0t8 = (0 - 1);
                          return _sv0t8;
                          _sv0t6 = 0;
                        } else {
                          if ((base_v.tag == 7)) {
                            int h2 = base_v.p0;
                            int _sv0t7 = (0 - 1);
                            return _sv0t7;
                            _sv0t6 = 0;
                          } else {
                          }
                        }
                      }
                    }
                  }
                }
              }
            }
            int _sv0t15 = (0 - 1);
            return _sv0t15;
            _sv0t0 = 0;
          } else {
            if ((e.tag == 5)) {
              int bv1 = e.p0;
              int bv2 = e.p1;
              int _sv0t1 = (0 - 1);
              return _sv0t1;
              _sv0t0 = 0;
            } else {
            }
          }
        }
      }
    }
  }
  int _sv0t50 = (0 - 1);
  return _sv0t50;
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

static int emit_ret_width(Value v, int env_names, int env_bases, int env_widths) {
  int _sv0t0;
  if ((v.tag == 5)) {
    int x = v.p0;
    int _sv0t1 = lookup_slot(env_names, env_bases, env_widths, x);
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
        int h = v.p0;
        return 1;
        _sv0t0 = 0;
      } else {
        if ((v.tag == 2)) {
          int b = v.p0;
          return 1;
          _sv0t0 = 0;
        } else {
          if ((v.tag == 3)) {
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

static int emit_instr(Instr ins, int env_names, int env_bases, int env_widths, int env_field_starts, int env_fields_flat, int pool, const char* source, int starts, int ends, int fn_names, int out) {
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
          Expr _sv0t147;
          int _sv0t148 = sv0_box_load(be, 0);
          _sv0t147.tag = _sv0t148;
          int _sv0t149 = sv0_box_load(be, 1);
          _sv0t147.p0 = _sv0t149;
          int _sv0t150 = sv0_box_load(be, 2);
          _sv0t147.p1 = _sv0t150;
          int _sv0t151 = sv0_box_load(be, 3);
          _sv0t147.p2 = _sv0t151;
          Expr e;
          e.tag = (_sv0t147).tag;
          e.p0 = (_sv0t147).p0;
          e.p1 = (_sv0t147).p1;
          e.p2 = (_sv0t147).p2;
          int _sv0t152 = emit_expr(e, env_names, env_bases, env_widths, pool, source, starts, ends, out);
          int n = _sv0t152;
          if ((n < 0)) {
            int _sv0t153 = (0 - 1);
            return _sv0t153;
          } else {
          }
          int _sv0t154 = lookup_slot(env_names, env_bases, env_widths, x);
          int si = _sv0t154;
          if ((si < 0)) {
            int _sv0t155 = (0 - 1);
            return _sv0t155;
          } else {
          }
          int _sv0t156 = slot_base(env_bases, si);
          int base = _sv0t156;
          int _sv0t157 = slot_width(env_widths, si);
          int width = _sv0t157;
          int k = 0;
          while ((k < width)) {
            sv0_vec_push(out, 97);
            int _sv0t158 = (base + width);
            int _sv0t159 = (_sv0t158 - 1);
            int _sv0t160 = (_sv0t159 - k);
            sv0_vec_push(out, _sv0t160);
            k = (k + 1);
          }
          int _sv0t161 = (width * 2);
          int _sv0t162 = (n + _sv0t161);
          return _sv0t162;
          _sv0t0 = 0;
        } else {
          if ((ins.tag == 4)) {
            int x = ins.p0;
            int be = ins.p1;
            Expr _sv0t131;
            int _sv0t132 = sv0_box_load(be, 0);
            _sv0t131.tag = _sv0t132;
            int _sv0t133 = sv0_box_load(be, 1);
            _sv0t131.p0 = _sv0t133;
            int _sv0t134 = sv0_box_load(be, 2);
            _sv0t131.p1 = _sv0t134;
            int _sv0t135 = sv0_box_load(be, 3);
            _sv0t131.p2 = _sv0t135;
            Expr e;
            e.tag = (_sv0t131).tag;
            e.p0 = (_sv0t131).p0;
            e.p1 = (_sv0t131).p1;
            e.p2 = (_sv0t131).p2;
            int _sv0t136 = emit_expr(e, env_names, env_bases, env_widths, pool, source, starts, ends, out);
            int n = _sv0t136;
            if ((n < 0)) {
              int _sv0t137 = (0 - 1);
              return _sv0t137;
            } else {
            }
            int _sv0t138 = lookup_slot(env_names, env_bases, env_widths, x);
            int si = _sv0t138;
            if ((si < 0)) {
              int _sv0t139 = (0 - 1);
              return _sv0t139;
            } else {
            }
            int _sv0t140 = slot_base(env_bases, si);
            int base = _sv0t140;
            int _sv0t141 = slot_width(env_widths, si);
            int width = _sv0t141;
            int k = 0;
            while ((k < width)) {
              sv0_vec_push(out, 97);
              int _sv0t142 = (base + width);
              int _sv0t143 = (_sv0t142 - 1);
              int _sv0t144 = (_sv0t143 - k);
              sv0_vec_push(out, _sv0t144);
              k = (k + 1);
            }
            int _sv0t145 = (width * 2);
            int _sv0t146 = (n + _sv0t145);
            return _sv0t146;
            _sv0t0 = 0;
          } else {
            if ((ins.tag == 5)) {
              int bh = ins.p0;
              int fh = ins.p1;
              int be = ins.p2;
              Expr _sv0t115;
              int _sv0t116 = sv0_box_load(be, 0);
              _sv0t115.tag = _sv0t116;
              int _sv0t117 = sv0_box_load(be, 1);
              _sv0t115.p0 = _sv0t117;
              int _sv0t118 = sv0_box_load(be, 2);
              _sv0t115.p1 = _sv0t118;
              int _sv0t119 = sv0_box_load(be, 3);
              _sv0t115.p2 = _sv0t119;
              Expr e;
              e.tag = (_sv0t115).tag;
              e.p0 = (_sv0t115).p0;
              e.p1 = (_sv0t115).p1;
              e.p2 = (_sv0t115).p2;
              int _sv0t120 = emit_expr(e, env_names, env_bases, env_widths, pool, source, starts, ends, out);
              int ne = _sv0t120;
              if ((ne < 0)) {
                int _sv0t121 = (0 - 1);
                return _sv0t121;
              } else {
              }
              int _sv0t122 = lookup_slot(env_names, env_bases, env_widths, bh);
              int si = _sv0t122;
              if ((si < 0)) {
                int _sv0t123 = (0 - 1);
                return _sv0t123;
              } else {
              }
              int _sv0t124 = slot_base(env_bases, si);
              int b0 = _sv0t124;
              int _sv0t125 = sv0_vec_get(env_field_starts, si);
              int fs = _sv0t125;
              int _sv0t126 = slot_width(env_widths, si);
              int w = _sv0t126;
              int _sv0t127 = index_of_field_range(env_fields_flat, fs, w, fh);
              int off = _sv0t127;
              if ((off < 0)) {
                int _sv0t128 = (0 - 1);
                return _sv0t128;
              } else {
              }
              sv0_vec_push(out, 97);
              int _sv0t129 = (b0 + off);
              sv0_vec_push(out, _sv0t129);
              int _sv0t130 = (ne + 2);
              return _sv0t130;
              _sv0t0 = 0;
            } else {
              if ((ins.tag == 6)) {
                int be = ins.p0;
                int th = ins.p1;
                int el = ins.p2;
                Expr _sv0t93;
                int _sv0t94 = sv0_box_load(be, 0);
                _sv0t93.tag = _sv0t94;
                int _sv0t95 = sv0_box_load(be, 1);
                _sv0t93.p0 = _sv0t95;
                int _sv0t96 = sv0_box_load(be, 2);
                _sv0t93.p1 = _sv0t96;
                int _sv0t97 = sv0_box_load(be, 3);
                _sv0t93.p2 = _sv0t97;
                Expr cond;
                cond.tag = (_sv0t93).tag;
                cond.p0 = (_sv0t93).p0;
                cond.p1 = (_sv0t93).p1;
                cond.p2 = (_sv0t93).p2;
                int _sv0t98 = emit_expr(cond, env_names, env_bases, env_widths, pool, source, starts, ends, out);
                int nc = _sv0t98;
                if ((nc < 0)) {
                  int _sv0t99 = (0 - 1);
                  return _sv0t99;
                } else {
                }
                int _sv0t100 = sv0_vec_new();
                int then_out = _sv0t100;
                int _sv0t101 = emit_instrs(th, env_names, env_bases, env_widths, env_field_starts, env_fields_flat, pool, source, starts, ends, fn_names, then_out);
                int nt = _sv0t101;
                if ((nt < 0)) {
                  int _sv0t102 = (0 - 1);
                  return _sv0t102;
                } else {
                }
                int _sv0t103 = sv0_vec_new();
                int else_out = _sv0t103;
                int _sv0t104 = emit_instrs(el, env_names, env_bases, env_widths, env_field_starts, env_fields_flat, pool, source, starts, ends, fn_names, else_out);
                int ne = _sv0t104;
                if ((ne < 0)) {
                  int _sv0t105 = (0 - 1);
                  return _sv0t105;
                } else {
                }
                int _sv0t106 = flat_byte_size(then_out);
                int _sv0t107 = insn_size(112);
                int off_else = (_sv0t106 + _sv0t107);
                int _sv0t108 = flat_byte_size(else_out);
                int off_end = _sv0t108;
                sv0_vec_push(out, 113);
                sv0_vec_push(out, off_else);
                int _sv0t109 = vec_append(out, then_out);
                int at = _sv0t109;
                sv0_vec_push(out, 112);
                sv0_vec_push(out, off_end);
                int _sv0t110 = vec_append(out, else_out);
                int ae = _sv0t110;
                int _sv0t111 = (nc + 2);
                int _sv0t112 = (_sv0t111 + at);
                int _sv0t113 = (_sv0t112 + 2);
                int _sv0t114 = (_sv0t113 + ae);
                return _sv0t114;
                _sv0t0 = 0;
              } else {
                if ((ins.tag == 7)) {
                  int be = ins.p0;
                  int body = ins.p1;
                  Expr _sv0t68;
                  int _sv0t69 = sv0_box_load(be, 0);
                  _sv0t68.tag = _sv0t69;
                  int _sv0t70 = sv0_box_load(be, 1);
                  _sv0t68.p0 = _sv0t70;
                  int _sv0t71 = sv0_box_load(be, 2);
                  _sv0t68.p1 = _sv0t71;
                  int _sv0t72 = sv0_box_load(be, 3);
                  _sv0t68.p2 = _sv0t72;
                  Expr cond;
                  cond.tag = (_sv0t68).tag;
                  cond.p0 = (_sv0t68).p0;
                  cond.p1 = (_sv0t68).p1;
                  cond.p2 = (_sv0t68).p2;
                  int _sv0t73 = sv0_vec_new();
                  int cond_out = _sv0t73;
                  int _sv0t74 = emit_expr(cond, env_names, env_bases, env_widths, pool, source, starts, ends, cond_out);
                  int nc = _sv0t74;
                  if ((nc < 0)) {
                    int _sv0t75 = (0 - 1);
                    return _sv0t75;
                  } else {
                  }
                  int _sv0t76 = sv0_vec_new();
                  int body_out = _sv0t76;
                  int _sv0t77 = emit_instrs(body, env_names, env_bases, env_widths, env_field_starts, env_fields_flat, pool, source, starts, ends, fn_names, body_out);
                  int nb = _sv0t77;
                  if ((nb < 0)) {
                    int _sv0t78 = (0 - 1);
                    return _sv0t78;
                  } else {
                  }
                  sv0_vec_push(body_out, 112);
                  int _sv0t79 = loop_exit_sentinel();
                  sv0_vec_push(body_out, _sv0t79);
                  int _sv0t80 = flat_byte_size(cond_out);
                  int cond_bytes = _sv0t80;
                  int _sv0t81 = insn_size(113);
                  int jif_bytes = _sv0t81;
                  int _sv0t82 = flat_byte_size(body_out);
                  int body_bytes = _sv0t82;
                  int forward = body_bytes;
                  int _sv0t83 = (cond_bytes + jif_bytes);
                  int _sv0t84 = (_sv0t83 + body_bytes);
                  int back = (0 - _sv0t84);
                  int _sv0t85 = replace_loop_exit_jump(body_out, back);
                  int _sv0t86 = sv0_vec_new();
                  int loop_out = _sv0t86;
                  int _sv0t87 = vec_append(loop_out, cond_out);
                  sv0_vec_push(loop_out, 113);
                  sv0_vec_push(loop_out, forward);
                  int _sv0t88 = vec_append(loop_out, body_out);
                  int _sv0t89 = flat_byte_size(loop_out);
                  int total_bytes = _sv0t89;
                  int _sv0t90 = patch_continue_jumps(loop_out, 0, 0);
                  int _sv0t91 = patch_break_jumps(loop_out, 0, total_bytes);
                  int _sv0t92 = vec_append(out, loop_out);
                  int added = _sv0t92;
                  return added;
                  _sv0t0 = 0;
                } else {
                  if ((ins.tag == 8)) {
                    int body = ins.p0;
                    int _sv0t67 = emit_instrs(body, env_names, env_bases, env_widths, env_field_starts, env_fields_flat, pool, source, starts, ends, fn_names, out);
                    return _sv0t67;
                    _sv0t0 = 0;
                  } else {
                    if ((ins.tag == 9)) {
                      sv0_vec_push(out, 112);
                      int _sv0t66 = loop_break_sentinel();
                      sv0_vec_push(out, _sv0t66);
                      return 2;
                      _sv0t0 = 0;
                    } else {
                      if ((ins.tag == 10)) {
                        sv0_vec_push(out, 112);
                        int _sv0t65 = loop_continue_sentinel();
                        sv0_vec_push(out, _sv0t65);
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
                          } else {
                            const char* _sv0t29 = handle_to_str(fn_h, source, starts, ends);
                            const char* fn_name;
                            fn_name = _sv0t29;
                            int _sv0t30 = builtin_id(fn_name);
                            bid = _sv0t30;
                          }
                          int _sv0t31 = sv0_vec_len(args);
                          int nargs_vec = _sv0t31;
                          int total = 0;
                          if ((bid >= 0)) {
                            int ai = 0;
                            while ((ai < nargs_vec)) {
                              int _sv0t32 = sv0_vec_get(args, ai);
                              int bv = _sv0t32;
                              Value _sv0t33;
                              int _sv0t34 = sv0_box_load(bv, 0);
                              _sv0t33.tag = _sv0t34;
                              int _sv0t35 = sv0_box_load(bv, 1);
                              _sv0t33.p0 = _sv0t35;
                              int _sv0t36 = sv0_box_load(bv, 2);
                              _sv0t33.p1 = _sv0t36;
                              Value v;
                              v.tag = (_sv0t33).tag;
                              v.p0 = (_sv0t33).p0;
                              v.p1 = (_sv0t33).p1;
                              int _sv0t37 = emit_value(v, env_names, env_bases, env_widths, pool, out);
                              int nv = _sv0t37;
                              if ((nv < 0)) {
                                int _sv0t38 = (0 - 1);
                                return _sv0t38;
                              } else {
                              }
                              total = (total + nv);
                              ai = (ai + 1);
                            }
                            sv0_vec_push(out, 117);
                            sv0_vec_push(out, bid);
                            total = (total + 2);
                            if ((dst_h != 0)) {
                              int _sv0t39 = lookup_slot(env_names, env_bases, env_widths, dst_h);
                              int si = _sv0t39;
                              if ((si < 0)) {
                                int _sv0t40 = (0 - 1);
                                return _sv0t40;
                              } else {
                              }
                              int _sv0t41 = slot_base(env_bases, si);
                              int base = _sv0t41;
                              int _sv0t42 = slot_width(env_widths, si);
                              int width = _sv0t42;
                              int k = 0;
                              while ((k < width)) {
                                sv0_vec_push(out, 97);
                                int _sv0t43 = (base + width);
                                int _sv0t44 = (_sv0t43 - 1);
                                int _sv0t45 = (_sv0t44 - k);
                                sv0_vec_push(out, _sv0t45);
                                k = (k + 1);
                              }
                              int _sv0t46 = (width * 2);
                              total = (total + _sv0t46);
                            } else {
                            }
                            return total;
                          } else {
                          }
                          int nargs_total = 0;
                          int ai = 0;
                          while ((ai < nargs_vec)) {
                            int _sv0t47 = sv0_vec_get(args, ai);
                            int bv = _sv0t47;
                            Value _sv0t48;
                            int _sv0t49 = sv0_box_load(bv, 0);
                            _sv0t48.tag = _sv0t49;
                            int _sv0t50 = sv0_box_load(bv, 1);
                            _sv0t48.p0 = _sv0t50;
                            int _sv0t51 = sv0_box_load(bv, 2);
                            _sv0t48.p1 = _sv0t51;
                            Value v;
                            v.tag = (_sv0t48).tag;
                            v.p0 = (_sv0t48).p0;
                            v.p1 = (_sv0t48).p1;
                            int _sv0t52 = emit_ret_width(v, env_names, env_bases, env_widths);
                            int w = _sv0t52;
                            nargs_total = (nargs_total + w);
                            int _sv0t53 = emit_value(v, env_names, env_bases, env_widths, pool, out);
                            int nv = _sv0t53;
                            if ((nv < 0)) {
                              int _sv0t54 = (0 - 1);
                              return _sv0t54;
                            } else {
                            }
                            total = (total + nv);
                            ai = (ai + 1);
                          }
                          int _sv0t55 = index_of_fn(fn_h, fn_names, source, starts, ends);
                          int fix = _sv0t55;
                          if ((fix < 0)) {
                            int _sv0t56 = (0 - 1);
                            return _sv0t56;
                          } else {
                          }
                          sv0_vec_push(out, 115);
                          sv0_vec_push(out, fix);
                          sv0_vec_push(out, nargs_total);
                          total = (total + 3);
                          if ((dst_h != 0)) {
                            int _sv0t57 = lookup_slot(env_names, env_bases, env_widths, dst_h);
                            int si = _sv0t57;
                            if ((si < 0)) {
                              int _sv0t58 = (0 - 1);
                              return _sv0t58;
                            } else {
                            }
                            int _sv0t59 = slot_base(env_bases, si);
                            int base = _sv0t59;
                            int _sv0t60 = slot_width(env_widths, si);
                            int width = _sv0t60;
                            int k = 0;
                            while ((k < width)) {
                              sv0_vec_push(out, 97);
                              int _sv0t61 = (base + width);
                              int _sv0t62 = (_sv0t61 - 1);
                              int _sv0t63 = (_sv0t62 - k);
                              sv0_vec_push(out, _sv0t63);
                              k = (k + 1);
                            }
                            int _sv0t64 = (width * 2);
                            total = (total + _sv0t64);
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
                            int _sv0t24 = emit_expr(e, env_names, env_bases, env_widths, pool, source, starts, ends, out);
                            int ne = _sv0t24;
                            if ((ne < 0)) {
                              int _sv0t25 = (0 - 1);
                              return _sv0t25;
                            } else {
                            }
                            int _sv0t26 = contract_pool_key(fn_h, 0);
                            int key = _sv0t26;
                            int _sv0t27 = pool_add(pool, key);
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
                              int _sv0t14 = emit_expr(e, env_names, env_bases, env_widths, pool, source, starts, ends, out);
                              int ne = _sv0t14;
                              if ((ne < 0)) {
                                int _sv0t15 = (0 - 1);
                                return _sv0t15;
                              } else {
                              }
                              int _sv0t16 = contract_pool_key(fn_h, 1);
                              int key = _sv0t16;
                              int _sv0t17 = pool_add(pool, key);
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
                                  int _sv0t5 = emit_ret_width(v, env_names, env_bases, env_widths);
                                  int w = _sv0t5;
                                  int _sv0t6 = emit_value(v, env_names, env_bases, env_widths, pool, out);
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
  int _sv0t163 = (0 - 1);
  return _sv0t163;
}

static int emit_instrs(int instrs, int env_names, int env_bases, int env_widths, int env_field_starts, int env_fields_flat, int pool, const char* source, int starts, int ends, int fn_names, int out) {
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
    int _sv0t8 = emit_instr(ins, env_names, env_bases, env_widths, env_field_starts, env_fields_flat, pool, source, starts, ends, fn_names, out);
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

static int ensure_var_int(int name, int env_names, int env_bases, int env_widths, int env_field_starts, int next_slot) {
  int _sv0t0 = lookup_slot(env_names, env_bases, env_widths, name);
  int si = _sv0t0;
  if ((si >= 0)) {
    return next_slot;
  } else {
  }
  sv0_vec_push(env_names, name);
  sv0_vec_push(env_bases, next_slot);
  sv0_vec_push(env_widths, 1);
  sv0_vec_push(env_field_starts, 0);
  int _sv0t1 = (next_slot + 1);
  return _sv0t1;
}

static int scan_instr_env(Instr ins, int env_names, int env_bases, int env_widths, int env_field_starts, int next_slot, int structs_names, int structs_field_counts, int enums_names, int enums_widths) {
  int _sv0t0;
  if ((ins.tag == 1)) {
    int x = ins.p0;
    int _sv0t12 = ensure_var_int(x, env_names, env_bases, env_widths, env_field_starts, next_slot);
    return _sv0t12;
    _sv0t0 = 0;
  } else {
    if ((ins.tag == 2)) {
      int cty = ins.p0;
      int x = ins.p1;
      int _sv0t10 = width_of_cty(cty, structs_names, structs_field_counts, enums_names, enums_widths);
      int w = _sv0t10;
      sv0_vec_push(env_names, x);
      sv0_vec_push(env_bases, next_slot);
      sv0_vec_push(env_widths, w);
      sv0_vec_push(env_field_starts, 0);
      int _sv0t11 = (next_slot + w);
      return _sv0t11;
      _sv0t0 = 0;
    } else {
      if ((ins.tag == 3)) {
        int x = ins.p0;
        int be = ins.p1;
        int _sv0t9 = ensure_var_int(x, env_names, env_bases, env_widths, env_field_starts, next_slot);
        return _sv0t9;
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
          int _sv0t5 = width_of_cty(rty, structs_names, structs_field_counts, enums_names, enums_widths);
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
          sv0_vec_push(env_field_starts, 0);
          int _sv0t8 = (next_slot + w);
          return _sv0t8;
          _sv0t0 = 0;
        } else {
          if ((ins.tag == 6)) {
            int be = ins.p0;
            int th = ins.p1;
            int el = ins.p2;
            int _sv0t3 = scan_instrs_env(th, env_names, env_bases, env_widths, env_field_starts, next_slot, structs_names, structs_field_counts, enums_names, enums_widths);
            int ns1 = _sv0t3;
            int _sv0t4 = scan_instrs_env(el, env_names, env_bases, env_widths, env_field_starts, ns1, structs_names, structs_field_counts, enums_names, enums_widths);
            return _sv0t4;
            _sv0t0 = 0;
          } else {
            if ((ins.tag == 7)) {
              int be = ins.p0;
              int body = ins.p1;
              int _sv0t2 = scan_instrs_env(body, env_names, env_bases, env_widths, env_field_starts, next_slot, structs_names, structs_field_counts, enums_names, enums_widths);
              return _sv0t2;
              _sv0t0 = 0;
            } else {
              if ((ins.tag == 8)) {
                int body = ins.p0;
                int _sv0t1 = scan_instrs_env(body, env_names, env_bases, env_widths, env_field_starts, next_slot, structs_names, structs_field_counts, enums_names, enums_widths);
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

static int scan_instrs_env(int instrs, int env_names, int env_bases, int env_widths, int env_field_starts, int next_slot, int structs_names, int structs_field_counts, int enums_names, int enums_widths) {
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
    int _sv0t8 = scan_instr_env(ins, env_names, env_bases, env_widths, env_field_starts, ns, structs_names, structs_field_counts, enums_names, enums_widths);
    ns = _sv0t8;
    i = (i + 1);
  }
  return ns;
}

static int local_count_and_env(int param_names, int param_ctys, int instrs, int env_names, int env_bases, int env_widths, int env_field_starts, int structs_names, int structs_field_counts, int enums_names, int enums_widths) {
  int _sv0t0 = sv0_vec_len(param_names);
  int np = _sv0t0;
  int ns = 0;
  int pi = 0;
  while ((pi < np)) {
    int _sv0t1 = sv0_vec_get(param_names, pi);
    int pn = _sv0t1;
    int _sv0t2 = sv0_vec_get(param_ctys, pi);
    int pc = _sv0t2;
    int _sv0t3 = width_of_cty(pc, structs_names, structs_field_counts, enums_names, enums_widths);
    int w = _sv0t3;
    sv0_vec_push(env_names, pn);
    sv0_vec_push(env_bases, ns);
    sv0_vec_push(env_widths, w);
    sv0_vec_push(env_field_starts, 0);
    ns = (ns + w);
    pi = (pi + 1);
  }
  int _sv0t4 = scan_instrs_env(instrs, env_names, env_bases, env_widths, env_field_starts, ns, structs_names, structs_field_counts, enums_names, enums_widths);
  return _sv0t4;
}

static int compute_arity(int param_ctys, int structs_names, int structs_field_counts, int enums_names, int enums_widths) {
  int _sv0t0 = sv0_vec_len(param_ctys);
  int np = _sv0t0;
  int total = 0;
  int i = 0;
  while ((i < np)) {
    int _sv0t1 = sv0_vec_get(param_ctys, i);
    int _sv0t2 = width_of_cty(_sv0t1, structs_names, structs_field_counts, enums_names, enums_widths);
    total = (total + _sv0t2);
    i = (i + 1);
  }
  return total;
}

static int emit_fn(int label, int param_names, int param_ctys, int instrs, int structs_names, int structs_field_counts, int enums_names, int enums_widths, int pool, const char* source, int starts, int ends, int fn_names, int ft_entries) {
  int _sv0t0 = sv0_vec_new();
  int en = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int eb = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int ew = _sv0t2;
  int _sv0t3 = sv0_vec_new();
  int efs = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int eff = _sv0t4;
  int _sv0t5 = local_count_and_env(param_names, param_ctys, instrs, en, eb, ew, efs, structs_names, structs_field_counts, enums_names, enums_widths);
  int lc = _sv0t5;
  int _sv0t6 = sv0_vec_new();
  int code = _sv0t6;
  int _sv0t7 = emit_instrs(instrs, en, eb, ew, efs, eff, pool, source, starts, ends, fn_names, code);
  int n = _sv0t7;
  if ((n < 0)) {
    int _sv0t8 = (0 - 1);
    return _sv0t8;
  } else {
  }
  int _sv0t9 = compute_arity(param_ctys, structs_names, structs_field_counts, enums_names, enums_widths);
  int arity = _sv0t9;
  int _sv0t10 = pool_add(pool, label);
  int name_idx = _sv0t10;
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

static int emit_program(int item_tags, int item_names, int item_field_counts, int item_variant_max_payload, int block_labels, int block_param_names, int block_param_ctys, int block_instrs, const char* source, int starts, int ends, int out_pool, int out_ft) {
  int _sv0t0 = sv0_vec_new();
  int sn = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int sfc = _sv0t1;
  int _sv0t2 = struct_layouts_build(item_tags, item_names, item_field_counts, sn, sfc);
  int _sv0t3 = sv0_vec_new();
  int enn = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int enw = _sv0t4;
  int _sv0t5 = enum_layouts_build(item_tags, item_names, item_field_counts, item_variant_max_payload, enn, enw);
  int _sv0t6 = sv0_vec_new();
  int func_order = _sv0t6;
  int _sv0t7 = build_func_order(block_labels, source, starts, ends, func_order);
  int fc = _sv0t7;
  int _sv0t8 = sv0_vec_new();
  int fn_names = _sv0t8;
  int fi = 0;
  while ((fi < fc)) {
    int _sv0t9 = sv0_vec_get(func_order, fi);
    int bi = _sv0t9;
    int _sv0t10 = sv0_vec_get(block_labels, bi);
    sv0_vec_push(fn_names, _sv0t10);
    fi = (fi + 1);
  }
  fi = 0;
  while ((fi < fc)) {
    int _sv0t11 = sv0_vec_get(func_order, fi);
    int bi2 = _sv0t11;
    int _sv0t12 = sv0_vec_get(block_param_names, bi2);
    int pn = _sv0t12;
    int _sv0t13 = sv0_vec_get(block_param_ctys, bi2);
    int pc = _sv0t13;
    int _sv0t14 = sv0_vec_get(block_instrs, bi2);
    int ins = _sv0t14;
    int _sv0t15 = sv0_vec_get(block_labels, bi2);
    int lab = _sv0t15;
    int _sv0t16 = emit_fn(lab, pn, pc, ins, sn, sfc, enn, enw, out_pool, source, starts, ends, fn_names, out_ft);
    int r = _sv0t16;
    if ((r < 0)) {
      int _sv0t17 = (0 - 1);
      return _sv0t17;
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
  sv0_vec_push(sn, 100);
  sv0_vec_push(sf, 3);
  sv0_vec_push(sn, 200);
  sv0_vec_push(sf, 5);
  sv0_vec_push(en, 300);
  sv0_vec_push(ew, 4);
  int _sv0t4 = width_of_cty(100, sn, sf, en, ew);
  if ((_sv0t4 != 3)) {
    return 1;
  } else {
  }
  int _sv0t5 = width_of_cty(200, sn, sf, en, ew);
  if ((_sv0t5 != 5)) {
    return 2;
  } else {
  }
  int _sv0t6 = width_of_cty(300, sn, sf, en, ew);
  if ((_sv0t6 != 4)) {
    return 3;
  } else {
  }
  int _sv0t7 = width_of_cty(999, sn, sf, en, ew);
  if ((_sv0t7 != 1)) {
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
  int _sv0t1 = pool_add(pool, 100);
  int i0 = _sv0t1;
  if ((i0 != 0)) {
    return 1;
  } else {
  }
  int _sv0t2 = pool_add(pool, 200);
  int i1 = _sv0t2;
  if ((i1 != 1)) {
    return 2;
  } else {
  }
  int _sv0t3 = pool_add(pool, 100);
  int i0b = _sv0t3;
  if ((i0b != 0)) {
    return 3;
  } else {
  }
  int _sv0t4 = pool_size(pool);
  if ((_sv0t4 != 2)) {
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
  int _sv0t3 = lookup_slot(names, bases, widths, 10);
  if ((_sv0t3 != 0)) {
    return 1;
  } else {
  }
  int _sv0t4 = lookup_slot(names, bases, widths, 20);
  if ((_sv0t4 != 1)) {
    return 2;
  } else {
  }
  int _sv0t5 = lookup_slot(names, bases, widths, 99);
  int _sv0t6 = (0 - 1);
  if ((_sv0t5 != _sv0t6)) {
    return 3;
  } else {
  }
  int _sv0t7 = slot_base(bases, 1);
  if ((_sv0t7 != 1)) {
    return 4;
  } else {
  }
  int _sv0t8 = slot_width(widths, 1);
  if ((_sv0t8 != 3)) {
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
  int ef = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int sn = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int sf = _sv0t5;
  int _sv0t6 = sv0_vec_new();
  int enn = _sv0t6;
  int _sv0t7 = sv0_vec_new();
  int enw = _sv0t7;
  sv0_vec_push(sn, 100);
  sv0_vec_push(sf, 3);
  sv0_vec_push(enn, 200);
  sv0_vec_push(enw, 4);
  int _sv0t8 = bind_param(10, 100, 0, en, eb, ew, ef, sn, sf, enn, enw);
  int s1 = _sv0t8;
  if ((s1 != 3)) {
    return 1;
  } else {
  }
  int _sv0t9 = bind_param(20, 200, 3, en, eb, ew, ef, sn, sf, enn, enw);
  int s2 = _sv0t9;
  if ((s2 != 7)) {
    return 2;
  } else {
  }
  int _sv0t10 = bind_param(30, 999, 7, en, eb, ew, ef, sn, sf, enn, enw);
  int s3 = _sv0t10;
  if ((s3 != 8)) {
    return 3;
  } else {
  }
  int _sv0t11 = sv0_vec_len(en);
  if ((_sv0t11 != 3)) {
    return 4;
  } else {
  }
  int _sv0t12 = sv0_vec_get(ew, 0);
  if ((_sv0t12 != 3)) {
    return 5;
  } else {
  }
  int _sv0t13 = sv0_vec_get(ew, 1);
  if ((_sv0t13 != 4)) {
    return 6;
  } else {
  }
  int _sv0t14 = sv0_vec_get(ew, 2);
  if ((_sv0t14 != 1)) {
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
  int pool = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int out = _sv0t4;
  Value vi;
  vi.tag = 0;
  vi.p0 = 42;
  int _sv0t5 = emit_value(vi, en, eb, ew, pool, out);
  int r1 = _sv0t5;
  if ((r1 != 2)) {
    return 1;
  } else {
  }
  int _sv0t6 = sv0_vec_get(out, 0);
  if ((_sv0t6 != 4)) {
    return 2;
  } else {
  }
  int _sv0t7 = sv0_vec_get(out, 1);
  if ((_sv0t7 != 42)) {
    return 3;
  } else {
  }
  int _sv0t8 = sv0_vec_new();
  int out2 = _sv0t8;
  Value vb;
  vb.tag = 2;
  vb.p0 = 1;
  int _sv0t9 = emit_value(vb, en, eb, ew, pool, out2);
  int r2 = _sv0t9;
  if ((r2 != 2)) {
    return 4;
  } else {
  }
  int _sv0t10 = sv0_vec_get(out2, 0);
  if ((_sv0t10 != 7)) {
    return 5;
  } else {
  }
  int _sv0t11 = sv0_vec_get(out2, 1);
  if ((_sv0t11 != 1)) {
    return 6;
  } else {
  }
  int _sv0t12 = sv0_vec_new();
  int out3 = _sv0t12;
  Value vbf;
  vbf.tag = 2;
  vbf.p0 = 0;
  int _sv0t13 = emit_value(vbf, en, eb, ew, pool, out3);
  int r2b = _sv0t13;
  if ((r2b != 2)) {
    return 7;
  } else {
  }
  int _sv0t14 = sv0_vec_get(out3, 1);
  if ((_sv0t14 != 0)) {
    return 8;
  } else {
  }
  int _sv0t15 = sv0_vec_new();
  int out4 = _sv0t15;
  Value vu;
  vu.tag = 4;
  int _sv0t16 = emit_value(vu, en, eb, ew, pool, out4);
  int r3 = _sv0t16;
  if ((r3 != 1)) {
    return 9;
  } else {
  }
  int _sv0t17 = sv0_vec_get(out4, 0);
  if ((_sv0t17 != 3)) {
    return 10;
  } else {
  }
  int _sv0t18 = sv0_vec_new();
  int out5 = _sv0t18;
  Value vs;
  vs.tag = 3;
  vs.p0 = 99;
  int _sv0t19 = emit_value(vs, en, eb, ew, pool, out5);
  int r4 = _sv0t19;
  if ((r4 != 2)) {
    return 11;
  } else {
  }
  int _sv0t20 = sv0_vec_get(out5, 0);
  if ((_sv0t20 != 8)) {
    return 12;
  } else {
  }
  int _sv0t21 = sv0_vec_get(out5, 1);
  if ((_sv0t21 != 0)) {
    return 13;
  } else {
  }
  sv0_vec_push(en, 50);
  sv0_vec_push(eb, 2);
  sv0_vec_push(ew, 1);
  int _sv0t22 = sv0_vec_new();
  int out6 = _sv0t22;
  Value vv;
  vv.tag = 5;
  vv.p0 = 50;
  int _sv0t23 = emit_value(vv, en, eb, ew, pool, out6);
  int r5 = _sv0t23;
  if ((r5 != 2)) {
    return 14;
  } else {
  }
  int _sv0t24 = sv0_vec_get(out6, 0);
  if ((_sv0t24 != 96)) {
    return 15;
  } else {
  }
  int _sv0t25 = sv0_vec_get(out6, 1);
  if ((_sv0t25 != 2)) {
    return 16;
  } else {
  }
  sv0_vec_push(en, 60);
  sv0_vec_push(eb, 5);
  sv0_vec_push(ew, 3);
  int _sv0t26 = sv0_vec_new();
  int out7 = _sv0t26;
  Value vv2;
  vv2.tag = 5;
  vv2.p0 = 60;
  int _sv0t27 = emit_value(vv2, en, eb, ew, pool, out7);
  int r6 = _sv0t27;
  if ((r6 != 6)) {
    return 17;
  } else {
  }
  int _sv0t28 = sv0_vec_get(out7, 0);
  if ((_sv0t28 != 96)) {
    return 18;
  } else {
  }
  int _sv0t29 = sv0_vec_get(out7, 1);
  if ((_sv0t29 != 5)) {
    return 19;
  } else {
  }
  int _sv0t30 = sv0_vec_get(out7, 2);
  if ((_sv0t30 != 96)) {
    return 20;
  } else {
  }
  int _sv0t31 = sv0_vec_get(out7, 3);
  if ((_sv0t31 != 6)) {
    return 21;
  } else {
  }
  int _sv0t32 = sv0_vec_get(out7, 4);
  if ((_sv0t32 != 96)) {
    return 22;
  } else {
  }
  int _sv0t33 = sv0_vec_get(out7, 5);
  if ((_sv0t33 != 7)) {
    return 23;
  } else {
  }
  int _sv0t34 = sv0_vec_new();
  int out8 = _sv0t34;
  Value va;
  va.tag = 7;
  va.p0 = 50;
  int _sv0t35 = emit_value(va, en, eb, ew, pool, out8);
  int r7 = _sv0t35;
  if ((r7 != 2)) {
    return 24;
  } else {
  }
  int _sv0t36 = sv0_vec_get(out8, 0);
  if ((_sv0t36 != 4)) {
    return 25;
  } else {
  }
  int _sv0t37 = sv0_vec_get(out8, 1);
  if ((_sv0t37 != 2)) {
    return 26;
  } else {
  }
  int _sv0t38 = sv0_vec_new();
  int out9 = _sv0t38;
  Value vm0;
  Value _sv0t39;
  _sv0t39.tag = 5;
  _sv0t39.p0 = 60;
  int _sv0t40 = sv0_box_alloc(3);
  sv0_box_store(_sv0t40, 0, _sv0t39.tag);
  sv0_box_store(_sv0t40, 1, _sv0t39.p0);
  sv0_box_store(_sv0t40, 2, _sv0t39.p1);
  vm0.tag = 6;
  vm0.p0 = _sv0t40;
  vm0.p1 = 0;
  int _sv0t41 = emit_value(vm0, en, eb, ew, pool, out9);
  int r8 = _sv0t41;
  if ((r8 != 2)) {
    return 27;
  } else {
  }
  int _sv0t42 = sv0_vec_get(out9, 0);
  if ((_sv0t42 != 96)) {
    return 28;
  } else {
  }
  int _sv0t43 = sv0_vec_get(out9, 1);
  if ((_sv0t43 != 5)) {
    return 29;
  } else {
  }
  int _sv0t44 = sv0_vec_new();
  int out10 = _sv0t44;
  Value vm_tag;
  Value _sv0t45;
  _sv0t45.tag = 5;
  _sv0t45.p0 = 60;
  int _sv0t46 = sv0_box_alloc(3);
  sv0_box_store(_sv0t46, 0, _sv0t45.tag);
  sv0_box_store(_sv0t46, 1, _sv0t45.p0);
  sv0_box_store(_sv0t46, 2, _sv0t45.p1);
  int _sv0t47 = (0 - 1);
  vm_tag.tag = 6;
  vm_tag.p0 = _sv0t46;
  vm_tag.p1 = _sv0t47;
  int _sv0t48 = emit_value(vm_tag, en, eb, ew, pool, out10);
  int r9 = _sv0t48;
  if ((r9 != 2)) {
    return 30;
  } else {
  }
  int _sv0t49 = sv0_vec_get(out10, 0);
  if ((_sv0t49 != 96)) {
    return 31;
  } else {
  }
  int _sv0t50 = sv0_vec_get(out10, 1);
  if ((_sv0t50 != 5)) {
    return 32;
  } else {
  }
  int _sv0t51 = sv0_vec_new();
  int out11 = _sv0t51;
  Value vm_oob;
  Value _sv0t52;
  _sv0t52.tag = 5;
  _sv0t52.p0 = 60;
  int _sv0t53 = sv0_box_alloc(3);
  sv0_box_store(_sv0t53, 0, _sv0t52.tag);
  sv0_box_store(_sv0t53, 1, _sv0t52.p0);
  sv0_box_store(_sv0t53, 2, _sv0t52.p1);
  vm_oob.tag = 6;
  vm_oob.p0 = _sv0t53;
  vm_oob.p1 = 3;
  int _sv0t54 = emit_value(vm_oob, en, eb, ew, pool, out11);
  int r10 = _sv0t54;
  int _sv0t55 = (0 - 1);
  if ((r10 != _sv0t55)) {
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
  int pool = _sv0t3;
  const char* source;
  source = "+-!i32";
  int _sv0t4 = sv0_vec_new();
  int starts = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int ends = _sv0t5;
  sv0_vec_push(starts, 0);
  sv0_vec_push(ends, 1);
  sv0_vec_push(starts, 1);
  sv0_vec_push(ends, 2);
  sv0_vec_push(starts, 2);
  sv0_vec_push(ends, 3);
  sv0_vec_push(starts, 3);
  sv0_vec_push(ends, 6);
  int _sv0t6 = sv0_vec_new();
  int out1 = _sv0t6;
  Expr lit;
  Value _sv0t7;
  _sv0t7.tag = 0;
  _sv0t7.p0 = 7;
  int _sv0t8 = sv0_box_alloc(3);
  sv0_box_store(_sv0t8, 0, _sv0t7.tag);
  sv0_box_store(_sv0t8, 1, _sv0t7.p0);
  sv0_box_store(_sv0t8, 2, _sv0t7.p1);
  lit.tag = 0;
  lit.p0 = _sv0t8;
  int _sv0t9 = emit_expr(lit, en, eb, ew, pool, source, starts, ends, out1);
  int r1 = _sv0t9;
  if ((r1 != 2)) {
    return 1;
  } else {
  }
  int _sv0t10 = sv0_vec_get(out1, 0);
  if ((_sv0t10 != 4)) {
    return 2;
  } else {
  }
  int _sv0t11 = sv0_vec_get(out1, 1);
  if ((_sv0t11 != 7)) {
    return 3;
  } else {
  }
  int _sv0t12 = sv0_vec_new();
  int out2 = _sv0t12;
  Expr binop;
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
  binop.tag = 1;
  binop.p0 = 0;
  binop.p1 = _sv0t14;
  binop.p2 = _sv0t16;
  int _sv0t17 = emit_expr(binop, en, eb, ew, pool, source, starts, ends, out2);
  int r2 = _sv0t17;
  if ((r2 != 5)) {
    return 4;
  } else {
  }
  int _sv0t18 = sv0_vec_get(out2, 0);
  if ((_sv0t18 != 4)) {
    return 5;
  } else {
  }
  int _sv0t19 = sv0_vec_get(out2, 1);
  if ((_sv0t19 != 3)) {
    return 6;
  } else {
  }
  int _sv0t20 = sv0_vec_get(out2, 2);
  if ((_sv0t20 != 4)) {
    return 7;
  } else {
  }
  int _sv0t21 = sv0_vec_get(out2, 3);
  if ((_sv0t21 != 4)) {
    return 8;
  } else {
  }
  int _sv0t22 = sv0_vec_get(out2, 4);
  if ((_sv0t22 != 16)) {
    return 9;
  } else {
  }
  int _sv0t23 = sv0_vec_new();
  int out3 = _sv0t23;
  Expr unop_not;
  Value _sv0t24;
  _sv0t24.tag = 2;
  _sv0t24.p0 = 1;
  int _sv0t25 = sv0_box_alloc(3);
  sv0_box_store(_sv0t25, 0, _sv0t24.tag);
  sv0_box_store(_sv0t25, 1, _sv0t24.p0);
  sv0_box_store(_sv0t25, 2, _sv0t24.p1);
  unop_not.tag = 2;
  unop_not.p0 = 2;
  unop_not.p1 = _sv0t25;
  int _sv0t26 = emit_expr(unop_not, en, eb, ew, pool, source, starts, ends, out3);
  int r3 = _sv0t26;
  if ((r3 != 3)) {
    return 10;
  } else {
  }
  int _sv0t27 = sv0_vec_get(out3, 0);
  if ((_sv0t27 != 7)) {
    return 11;
  } else {
  }
  int _sv0t28 = sv0_vec_get(out3, 1);
  if ((_sv0t28 != 1)) {
    return 12;
  } else {
  }
  int _sv0t29 = sv0_vec_get(out3, 2);
  if ((_sv0t29 != 82)) {
    return 13;
  } else {
  }
  int _sv0t30 = sv0_vec_new();
  int out4 = _sv0t30;
  Expr unop_neg;
  Value _sv0t31;
  _sv0t31.tag = 0;
  _sv0t31.p0 = 5;
  int _sv0t32 = sv0_box_alloc(3);
  sv0_box_store(_sv0t32, 0, _sv0t31.tag);
  sv0_box_store(_sv0t32, 1, _sv0t31.p0);
  sv0_box_store(_sv0t32, 2, _sv0t31.p1);
  unop_neg.tag = 2;
  unop_neg.p0 = 1;
  unop_neg.p1 = _sv0t32;
  int _sv0t33 = emit_expr(unop_neg, en, eb, ew, pool, source, starts, ends, out4);
  int r4 = _sv0t33;
  if ((r4 != 3)) {
    return 14;
  } else {
  }
  int _sv0t34 = sv0_vec_get(out4, 0);
  if ((_sv0t34 != 4)) {
    return 15;
  } else {
  }
  int _sv0t35 = sv0_vec_get(out4, 1);
  if ((_sv0t35 != 5)) {
    return 16;
  } else {
  }
  int _sv0t36 = sv0_vec_get(out4, 2);
  if ((_sv0t36 != 21)) {
    return 17;
  } else {
  }
  sv0_vec_push(en, 70);
  sv0_vec_push(eb, 0);
  sv0_vec_push(ew, 1);
  int _sv0t37 = sv0_vec_new();
  int out5 = _sv0t37;
  Expr load;
  load.tag = 3;
  load.p0 = 70;
  int _sv0t38 = emit_expr(load, en, eb, ew, pool, source, starts, ends, out5);
  int r5 = _sv0t38;
  if ((r5 != 2)) {
    return 18;
  } else {
  }
  int _sv0t39 = sv0_vec_get(out5, 0);
  if ((_sv0t39 != 96)) {
    return 19;
  } else {
  }
  int _sv0t40 = sv0_vec_get(out5, 1);
  if ((_sv0t40 != 0)) {
    return 20;
  } else {
  }
  int _sv0t41 = sv0_vec_new();
  int out6 = _sv0t41;
  Expr unop_cast;
  Value _sv0t42;
  _sv0t42.tag = 0;
  _sv0t42.p0 = 9;
  int _sv0t43 = sv0_box_alloc(3);
  sv0_box_store(_sv0t43, 0, _sv0t42.tag);
  sv0_box_store(_sv0t43, 1, _sv0t42.p0);
  sv0_box_store(_sv0t43, 2, _sv0t42.p1);
  unop_cast.tag = 2;
  unop_cast.p0 = 3;
  unop_cast.p1 = _sv0t43;
  int _sv0t44 = emit_expr(unop_cast, en, eb, ew, pool, source, starts, ends, out6);
  int r6 = _sv0t44;
  if ((r6 != 2)) {
    return 21;
  } else {
  }
  int _sv0t45 = sv0_vec_get(out6, 0);
  if ((_sv0t45 != 4)) {
    return 22;
  } else {
  }
  int _sv0t46 = sv0_vec_get(out6, 1);
  if ((_sv0t46 != 9)) {
    return 23;
  } else {
  }
  sv0_vec_push(en, 80);
  sv0_vec_push(eb, 4);
  sv0_vec_push(ew, 3);
  int _sv0t47 = sv0_vec_new();
  int out7 = _sv0t47;
  Expr f0;
  Value _sv0t48;
  _sv0t48.tag = 5;
  _sv0t48.p0 = 80;
  int _sv0t49 = sv0_box_alloc(3);
  sv0_box_store(_sv0t49, 0, _sv0t48.tag);
  sv0_box_store(_sv0t49, 1, _sv0t48.p0);
  sv0_box_store(_sv0t49, 2, _sv0t48.p1);
  f0.tag = 4;
  f0.p0 = _sv0t49;
  f0.p1 = 0;
  int _sv0t50 = emit_expr(f0, en, eb, ew, pool, source, starts, ends, out7);
  int r7 = _sv0t50;
  if ((r7 != 2)) {
    return 24;
  } else {
  }
  int _sv0t51 = sv0_vec_get(out7, 0);
  if ((_sv0t51 != 96)) {
    return 25;
  } else {
  }
  int _sv0t52 = sv0_vec_get(out7, 1);
  if ((_sv0t52 != 4)) {
    return 26;
  } else {
  }
  int _sv0t53 = sv0_vec_new();
  int out8 = _sv0t53;
  Expr ftag;
  Value _sv0t54;
  _sv0t54.tag = 5;
  _sv0t54.p0 = 80;
  int _sv0t55 = sv0_box_alloc(3);
  sv0_box_store(_sv0t55, 0, _sv0t54.tag);
  sv0_box_store(_sv0t55, 1, _sv0t54.p0);
  sv0_box_store(_sv0t55, 2, _sv0t54.p1);
  int _sv0t56 = (0 - 1);
  ftag.tag = 4;
  ftag.p0 = _sv0t55;
  ftag.p1 = _sv0t56;
  int _sv0t57 = emit_expr(ftag, en, eb, ew, pool, source, starts, ends, out8);
  int r8 = _sv0t57;
  if ((r8 != 2)) {
    return 27;
  } else {
  }
  int _sv0t58 = sv0_vec_get(out8, 0);
  if ((_sv0t58 != 96)) {
    return 28;
  } else {
  }
  int _sv0t59 = sv0_vec_get(out8, 1);
  if ((_sv0t59 != 4)) {
    return 29;
  } else {
  }
  int _sv0t60 = sv0_vec_new();
  int out9 = _sv0t60;
  Expr foob;
  Value _sv0t61;
  _sv0t61.tag = 5;
  _sv0t61.p0 = 80;
  int _sv0t62 = sv0_box_alloc(3);
  sv0_box_store(_sv0t62, 0, _sv0t61.tag);
  sv0_box_store(_sv0t62, 1, _sv0t61.p0);
  sv0_box_store(_sv0t62, 2, _sv0t61.p1);
  foob.tag = 4;
  foob.p0 = _sv0t62;
  foob.p1 = 3;
  int _sv0t63 = emit_expr(foob, en, eb, ew, pool, source, starts, ends, out9);
  int r9 = _sv0t63;
  int _sv0t64 = (0 - 1);
  if ((r9 != _sv0t64)) {
    return 30;
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
  int efs = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int eff = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int pool = _sv0t5;
  int _sv0t6 = sv0_vec_new();
  int fns = _sv0t6;
  const char* source;
  source = "+-!";
  int _sv0t7 = sv0_vec_new();
  int starts = _sv0t7;
  int _sv0t8 = sv0_vec_new();
  int ends = _sv0t8;
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
  int _sv0t9 = sv0_vec_new();
  int out1 = _sv0t9;
  Instr _sv0t10;
  _sv0t10.tag = 0;
  int _sv0t11 = emit_instr(_sv0t10, en, eb, ew, efs, eff, pool, source, starts, ends, fns, out1);
  int r1 = _sv0t11;
  if ((r1 != 0)) {
    return 1;
  } else {
  }
  int _sv0t12 = sv0_vec_len(out1);
  if ((_sv0t12 != 0)) {
    return 2;
  } else {
  }
  int _sv0t13 = sv0_vec_new();
  int out2 = _sv0t13;
  Instr _sv0t14;
  _sv0t14.tag = 1;
  _sv0t14.p0 = 99;
  int _sv0t15 = emit_instr(_sv0t14, en, eb, ew, efs, eff, pool, source, starts, ends, fns, out2);
  int r2 = _sv0t15;
  if ((r2 != 0)) {
    return 3;
  } else {
  }
  int _sv0t16 = sv0_vec_len(out2);
  if ((_sv0t16 != 0)) {
    return 4;
  } else {
  }
  int _sv0t17 = sv0_vec_new();
  int out3 = _sv0t17;
  Instr _sv0t18;
  _sv0t18.tag = 2;
  _sv0t18.p0 = 1;
  _sv0t18.p1 = 2;
  int _sv0t19 = emit_instr(_sv0t18, en, eb, ew, efs, eff, pool, source, starts, ends, fns, out3);
  int r3 = _sv0t19;
  if ((r3 != 0)) {
    return 5;
  } else {
  }
  int _sv0t20 = sv0_vec_new();
  int out4 = _sv0t20;
  Expr asgn_e;
  Value _sv0t21;
  _sv0t21.tag = 0;
  _sv0t21.p0 = 42;
  int _sv0t22 = sv0_box_alloc(3);
  sv0_box_store(_sv0t22, 0, _sv0t21.tag);
  sv0_box_store(_sv0t22, 1, _sv0t21.p0);
  sv0_box_store(_sv0t22, 2, _sv0t21.p1);
  asgn_e.tag = 0;
  asgn_e.p0 = _sv0t22;
  Instr _sv0t23;
  int _sv0t24 = sv0_box_alloc(4);
  sv0_box_store(_sv0t24, 0, asgn_e.tag);
  sv0_box_store(_sv0t24, 1, asgn_e.p0);
  sv0_box_store(_sv0t24, 2, asgn_e.p1);
  sv0_box_store(_sv0t24, 3, asgn_e.p2);
  _sv0t23.tag = 3;
  _sv0t23.p0 = 50;
  _sv0t23.p1 = _sv0t24;
  int _sv0t25 = emit_instr(_sv0t23, en, eb, ew, efs, eff, pool, source, starts, ends, fns, out4);
  int r4 = _sv0t25;
  if ((r4 != 4)) {
    return 6;
  } else {
  }
  int _sv0t26 = sv0_vec_get(out4, 0);
  if ((_sv0t26 != 4)) {
    return 7;
  } else {
  }
  int _sv0t27 = sv0_vec_get(out4, 1);
  if ((_sv0t27 != 42)) {
    return 8;
  } else {
  }
  int _sv0t28 = sv0_vec_get(out4, 2);
  if ((_sv0t28 != 97)) {
    return 9;
  } else {
  }
  int _sv0t29 = sv0_vec_get(out4, 3);
  if ((_sv0t29 != 0)) {
    return 10;
  } else {
  }
  int _sv0t30 = sv0_vec_new();
  int out5 = _sv0t30;
  Expr store_e;
  Value _sv0t31;
  _sv0t31.tag = 2;
  _sv0t31.p0 = 1;
  int _sv0t32 = sv0_box_alloc(3);
  sv0_box_store(_sv0t32, 0, _sv0t31.tag);
  sv0_box_store(_sv0t32, 1, _sv0t31.p0);
  sv0_box_store(_sv0t32, 2, _sv0t31.p1);
  store_e.tag = 0;
  store_e.p0 = _sv0t32;
  Instr _sv0t33;
  int _sv0t34 = sv0_box_alloc(4);
  sv0_box_store(_sv0t34, 0, store_e.tag);
  sv0_box_store(_sv0t34, 1, store_e.p0);
  sv0_box_store(_sv0t34, 2, store_e.p1);
  sv0_box_store(_sv0t34, 3, store_e.p2);
  _sv0t33.tag = 4;
  _sv0t33.p0 = 50;
  _sv0t33.p1 = _sv0t34;
  int _sv0t35 = emit_instr(_sv0t33, en, eb, ew, efs, eff, pool, source, starts, ends, fns, out5);
  int r5 = _sv0t35;
  if ((r5 != 4)) {
    return 11;
  } else {
  }
  int _sv0t36 = sv0_vec_get(out5, 0);
  if ((_sv0t36 != 7)) {
    return 12;
  } else {
  }
  int _sv0t37 = sv0_vec_get(out5, 1);
  if ((_sv0t37 != 1)) {
    return 13;
  } else {
  }
  int _sv0t38 = sv0_vec_get(out5, 2);
  if ((_sv0t38 != 97)) {
    return 14;
  } else {
  }
  int _sv0t39 = sv0_vec_get(out5, 3);
  if ((_sv0t39 != 0)) {
    return 15;
  } else {
  }
  int _sv0t40 = sv0_vec_new();
  int out6 = _sv0t40;
  Instr _sv0t41;
  _sv0t41.tag = 14;
  int _sv0t42 = emit_instr(_sv0t41, en, eb, ew, efs, eff, pool, source, starts, ends, fns, out6);
  int r6 = _sv0t42;
  if ((r6 != 2)) {
    return 16;
  } else {
  }
  int _sv0t43 = sv0_vec_get(out6, 0);
  if ((_sv0t43 != 118)) {
    return 17;
  } else {
  }
  int _sv0t44 = sv0_vec_get(out6, 1);
  if ((_sv0t44 != 0)) {
    return 18;
  } else {
  }
  int _sv0t45 = sv0_vec_new();
  int out7 = _sv0t45;
  Instr _sv0t46;
  Value _sv0t47;
  _sv0t47.tag = 0;
  _sv0t47.p0 = 7;
  int _sv0t48 = sv0_box_alloc(3);
  sv0_box_store(_sv0t48, 0, _sv0t47.tag);
  sv0_box_store(_sv0t48, 1, _sv0t47.p0);
  sv0_box_store(_sv0t48, 2, _sv0t47.p1);
  _sv0t46.tag = 15;
  _sv0t46.p0 = _sv0t48;
  int _sv0t49 = emit_instr(_sv0t46, en, eb, ew, efs, eff, pool, source, starts, ends, fns, out7);
  int r7 = _sv0t49;
  if ((r7 != 4)) {
    return 19;
  } else {
  }
  int _sv0t50 = sv0_vec_get(out7, 0);
  if ((_sv0t50 != 4)) {
    return 20;
  } else {
  }
  int _sv0t51 = sv0_vec_get(out7, 1);
  if ((_sv0t51 != 7)) {
    return 21;
  } else {
  }
  int _sv0t52 = sv0_vec_get(out7, 2);
  if ((_sv0t52 != 118)) {
    return 22;
  } else {
  }
  int _sv0t53 = sv0_vec_get(out7, 3);
  if ((_sv0t53 != 1)) {
    return 23;
  } else {
  }
  int _sv0t54 = sv0_vec_new();
  int out8 = _sv0t54;
  Instr _sv0t55;
  Value _sv0t56;
  _sv0t56.tag = 5;
  _sv0t56.p0 = 50;
  int _sv0t57 = sv0_box_alloc(3);
  sv0_box_store(_sv0t57, 0, _sv0t56.tag);
  sv0_box_store(_sv0t57, 1, _sv0t56.p0);
  sv0_box_store(_sv0t57, 2, _sv0t56.p1);
  _sv0t55.tag = 15;
  _sv0t55.p0 = _sv0t57;
  int _sv0t58 = emit_instr(_sv0t55, en, eb, ew, efs, eff, pool, source, starts, ends, fns, out8);
  int r8 = _sv0t58;
  if ((r8 != 4)) {
    return 24;
  } else {
  }
  int _sv0t59 = sv0_vec_get(out8, 0);
  if ((_sv0t59 != 96)) {
    return 25;
  } else {
  }
  int _sv0t60 = sv0_vec_get(out8, 1);
  if ((_sv0t60 != 0)) {
    return 26;
  } else {
  }
  int _sv0t61 = sv0_vec_get(out8, 2);
  if ((_sv0t61 != 118)) {
    return 27;
  } else {
  }
  int _sv0t62 = sv0_vec_get(out8, 3);
  if ((_sv0t62 != 1)) {
    return 28;
  } else {
  }
  sv0_vec_push(en, 60);
  sv0_vec_push(eb, 1);
  sv0_vec_push(ew, 3);
  sv0_vec_push(efs, 0);
  int _sv0t63 = sv0_vec_new();
  int out9 = _sv0t63;
  Expr asgn2_e;
  asgn2_e.tag = 3;
  asgn2_e.p0 = 60;
  Instr _sv0t64;
  int _sv0t65 = sv0_box_alloc(4);
  sv0_box_store(_sv0t65, 0, asgn2_e.tag);
  sv0_box_store(_sv0t65, 1, asgn2_e.p0);
  sv0_box_store(_sv0t65, 2, asgn2_e.p1);
  sv0_box_store(_sv0t65, 3, asgn2_e.p2);
  _sv0t64.tag = 3;
  _sv0t64.p0 = 60;
  _sv0t64.p1 = _sv0t65;
  int _sv0t66 = emit_instr(_sv0t64, en, eb, ew, efs, eff, pool, source, starts, ends, fns, out9);
  int r9 = _sv0t66;
  if ((r9 != 12)) {
    return 29;
  } else {
  }
  int _sv0t67 = sv0_vec_get(out9, 0);
  if ((_sv0t67 != 96)) {
    return 30;
  } else {
  }
  int _sv0t68 = sv0_vec_get(out9, 1);
  if ((_sv0t68 != 1)) {
    return 31;
  } else {
  }
  int _sv0t69 = sv0_vec_get(out9, 2);
  if ((_sv0t69 != 96)) {
    return 32;
  } else {
  }
  int _sv0t70 = sv0_vec_get(out9, 3);
  if ((_sv0t70 != 2)) {
    return 33;
  } else {
  }
  int _sv0t71 = sv0_vec_get(out9, 4);
  if ((_sv0t71 != 96)) {
    return 34;
  } else {
  }
  int _sv0t72 = sv0_vec_get(out9, 5);
  if ((_sv0t72 != 3)) {
    return 35;
  } else {
  }
  int _sv0t73 = sv0_vec_get(out9, 6);
  if ((_sv0t73 != 97)) {
    return 36;
  } else {
  }
  int _sv0t74 = sv0_vec_get(out9, 7);
  if ((_sv0t74 != 3)) {
    return 37;
  } else {
  }
  int _sv0t75 = sv0_vec_get(out9, 8);
  if ((_sv0t75 != 97)) {
    return 38;
  } else {
  }
  int _sv0t76 = sv0_vec_get(out9, 9);
  if ((_sv0t76 != 2)) {
    return 39;
  } else {
  }
  int _sv0t77 = sv0_vec_get(out9, 10);
  if ((_sv0t77 != 97)) {
    return 40;
  } else {
  }
  int _sv0t78 = sv0_vec_get(out9, 11);
  if ((_sv0t78 != 1)) {
    return 41;
  } else {
  }
  int _sv0t79 = sv0_vec_new();
  int out10 = _sv0t79;
  Instr _sv0t80;
  Value _sv0t81;
  _sv0t81.tag = 5;
  _sv0t81.p0 = 60;
  int _sv0t82 = sv0_box_alloc(3);
  sv0_box_store(_sv0t82, 0, _sv0t81.tag);
  sv0_box_store(_sv0t82, 1, _sv0t81.p0);
  sv0_box_store(_sv0t82, 2, _sv0t81.p1);
  _sv0t80.tag = 15;
  _sv0t80.p0 = _sv0t82;
  int _sv0t83 = emit_instr(_sv0t80, en, eb, ew, efs, eff, pool, source, starts, ends, fns, out10);
  int r10 = _sv0t83;
  if ((r10 != 8)) {
    return 42;
  } else {
  }
  int _sv0t84 = sv0_vec_get(out10, 6);
  if ((_sv0t84 != 118)) {
    return 43;
  } else {
  }
  int _sv0t85 = sv0_vec_get(out10, 7);
  if ((_sv0t85 != 3)) {
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
  int efs = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int eff = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int pool = _sv0t5;
  int _sv0t6 = sv0_vec_new();
  int fns = _sv0t6;
  const char* source;
  source = "+";
  int _sv0t7 = sv0_vec_new();
  int starts = _sv0t7;
  int _sv0t8 = sv0_vec_new();
  int ends = _sv0t8;
  sv0_vec_push(starts, 0);
  sv0_vec_push(ends, 1);
  int _sv0t9 = sv0_vec_new();
  int empty = _sv0t9;
  int _sv0t10 = sv0_vec_new();
  int out1 = _sv0t10;
  int _sv0t11 = emit_instrs(empty, en, eb, ew, efs, eff, pool, source, starts, ends, fns, out1);
  int r1 = _sv0t11;
  if ((r1 != 0)) {
    return 1;
  } else {
  }
  int _sv0t12 = sv0_vec_len(out1);
  if ((_sv0t12 != 0)) {
    return 2;
  } else {
  }
  int _sv0t13 = sv0_vec_new();
  int instrs = _sv0t13;
  Instr _sv0t14;
  _sv0t14.tag = 0;
  int _sv0t15 = sv0_box_alloc(5);
  sv0_box_store(_sv0t15, 0, _sv0t14.tag);
  sv0_box_store(_sv0t15, 1, _sv0t14.p0);
  sv0_box_store(_sv0t15, 2, _sv0t14.p1);
  sv0_box_store(_sv0t15, 3, _sv0t14.p2);
  sv0_box_store(_sv0t15, 4, _sv0t14.p3);
  sv0_vec_push(instrs, _sv0t15);
  Instr _sv0t16;
  _sv0t16.tag = 14;
  int _sv0t17 = sv0_box_alloc(5);
  sv0_box_store(_sv0t17, 0, _sv0t16.tag);
  sv0_box_store(_sv0t17, 1, _sv0t16.p0);
  sv0_box_store(_sv0t17, 2, _sv0t16.p1);
  sv0_box_store(_sv0t17, 3, _sv0t16.p2);
  sv0_box_store(_sv0t17, 4, _sv0t16.p3);
  sv0_vec_push(instrs, _sv0t17);
  int _sv0t18 = sv0_vec_new();
  int out2 = _sv0t18;
  int _sv0t19 = emit_instrs(instrs, en, eb, ew, efs, eff, pool, source, starts, ends, fns, out2);
  int r2 = _sv0t19;
  if ((r2 != 2)) {
    return 3;
  } else {
  }
  int _sv0t20 = sv0_vec_len(out2);
  if ((_sv0t20 != 2)) {
    return 4;
  } else {
  }
  int _sv0t21 = sv0_vec_get(out2, 0);
  if ((_sv0t21 != 118)) {
    return 5;
  } else {
  }
  int _sv0t22 = sv0_vec_get(out2, 1);
  if ((_sv0t22 != 0)) {
    return 6;
  } else {
  }
  sv0_vec_push(en, 70);
  sv0_vec_push(eb, 0);
  sv0_vec_push(ew, 1);
  sv0_vec_push(efs, 0);
  int _sv0t23 = sv0_vec_new();
  int instrs2 = _sv0t23;
  Expr a_e;
  Value _sv0t24;
  _sv0t24.tag = 0;
  _sv0t24.p0 = 99;
  int _sv0t25 = sv0_box_alloc(3);
  sv0_box_store(_sv0t25, 0, _sv0t24.tag);
  sv0_box_store(_sv0t25, 1, _sv0t24.p0);
  sv0_box_store(_sv0t25, 2, _sv0t24.p1);
  a_e.tag = 0;
  a_e.p0 = _sv0t25;
  Instr _sv0t26;
  int _sv0t27 = sv0_box_alloc(4);
  sv0_box_store(_sv0t27, 0, a_e.tag);
  sv0_box_store(_sv0t27, 1, a_e.p0);
  sv0_box_store(_sv0t27, 2, a_e.p1);
  sv0_box_store(_sv0t27, 3, a_e.p2);
  _sv0t26.tag = 3;
  _sv0t26.p0 = 70;
  _sv0t26.p1 = _sv0t27;
  int _sv0t28 = sv0_box_alloc(5);
  sv0_box_store(_sv0t28, 0, _sv0t26.tag);
  sv0_box_store(_sv0t28, 1, _sv0t26.p0);
  sv0_box_store(_sv0t28, 2, _sv0t26.p1);
  sv0_box_store(_sv0t28, 3, _sv0t26.p2);
  sv0_box_store(_sv0t28, 4, _sv0t26.p3);
  sv0_vec_push(instrs2, _sv0t28);
  Instr _sv0t29;
  Value _sv0t30;
  _sv0t30.tag = 5;
  _sv0t30.p0 = 70;
  int _sv0t31 = sv0_box_alloc(3);
  sv0_box_store(_sv0t31, 0, _sv0t30.tag);
  sv0_box_store(_sv0t31, 1, _sv0t30.p0);
  sv0_box_store(_sv0t31, 2, _sv0t30.p1);
  _sv0t29.tag = 15;
  _sv0t29.p0 = _sv0t31;
  int _sv0t32 = sv0_box_alloc(5);
  sv0_box_store(_sv0t32, 0, _sv0t29.tag);
  sv0_box_store(_sv0t32, 1, _sv0t29.p0);
  sv0_box_store(_sv0t32, 2, _sv0t29.p1);
  sv0_box_store(_sv0t32, 3, _sv0t29.p2);
  sv0_box_store(_sv0t32, 4, _sv0t29.p3);
  sv0_vec_push(instrs2, _sv0t32);
  int _sv0t33 = sv0_vec_new();
  int out3 = _sv0t33;
  int _sv0t34 = emit_instrs(instrs2, en, eb, ew, efs, eff, pool, source, starts, ends, fns, out3);
  int r3 = _sv0t34;
  if ((r3 != 8)) {
    return 7;
  } else {
  }
  int _sv0t35 = sv0_vec_get(out3, 0);
  if ((_sv0t35 != 4)) {
    return 8;
  } else {
  }
  int _sv0t36 = sv0_vec_get(out3, 1);
  if ((_sv0t36 != 99)) {
    return 9;
  } else {
  }
  int _sv0t37 = sv0_vec_get(out3, 2);
  if ((_sv0t37 != 97)) {
    return 10;
  } else {
  }
  int _sv0t38 = sv0_vec_get(out3, 3);
  if ((_sv0t38 != 0)) {
    return 11;
  } else {
  }
  int _sv0t39 = sv0_vec_get(out3, 4);
  if ((_sv0t39 != 96)) {
    return 12;
  } else {
  }
  int _sv0t40 = sv0_vec_get(out3, 5);
  if ((_sv0t40 != 0)) {
    return 13;
  } else {
  }
  int _sv0t41 = sv0_vec_get(out3, 6);
  if ((_sv0t41 != 118)) {
    return 14;
  } else {
  }
  int _sv0t42 = sv0_vec_get(out3, 7);
  if ((_sv0t42 != 1)) {
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
  int efs = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int eff = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int pool = _sv0t5;
  int _sv0t6 = sv0_vec_new();
  int fns = _sv0t6;
  const char* source;
  source = "+";
  int _sv0t7 = sv0_vec_new();
  int starts = _sv0t7;
  int _sv0t8 = sv0_vec_new();
  int ends = _sv0t8;
  sv0_vec_push(starts, 0);
  sv0_vec_push(ends, 1);
  sv0_vec_push(en, 50);
  sv0_vec_push(eb, 0);
  sv0_vec_push(ew, 1);
  sv0_vec_push(efs, 0);
  Expr _sv0t9;
  Value _sv0t10;
  _sv0t10.tag = 2;
  _sv0t10.p0 = 1;
  int _sv0t11 = sv0_box_alloc(3);
  sv0_box_store(_sv0t11, 0, _sv0t10.tag);
  sv0_box_store(_sv0t11, 1, _sv0t10.p0);
  sv0_box_store(_sv0t11, 2, _sv0t10.p1);
  _sv0t9.tag = 0;
  _sv0t9.p0 = _sv0t11;
  int _sv0t12 = sv0_box_alloc(4);
  sv0_box_store(_sv0t12, 0, _sv0t9.tag);
  sv0_box_store(_sv0t12, 1, _sv0t9.p0);
  sv0_box_store(_sv0t12, 2, _sv0t9.p1);
  sv0_box_store(_sv0t12, 3, _sv0t9.p2);
  int cond1 = _sv0t12;
  int _sv0t13 = sv0_vec_new();
  int th1 = _sv0t13;
  Instr _sv0t14;
  _sv0t14.tag = 14;
  int _sv0t15 = sv0_box_alloc(5);
  sv0_box_store(_sv0t15, 0, _sv0t14.tag);
  sv0_box_store(_sv0t15, 1, _sv0t14.p0);
  sv0_box_store(_sv0t15, 2, _sv0t14.p1);
  sv0_box_store(_sv0t15, 3, _sv0t14.p2);
  sv0_box_store(_sv0t15, 4, _sv0t14.p3);
  sv0_vec_push(th1, _sv0t15);
  int _sv0t16 = sv0_vec_new();
  int el1 = _sv0t16;
  Instr _sv0t17;
  _sv0t17.tag = 14;
  int _sv0t18 = sv0_box_alloc(5);
  sv0_box_store(_sv0t18, 0, _sv0t17.tag);
  sv0_box_store(_sv0t18, 1, _sv0t17.p0);
  sv0_box_store(_sv0t18, 2, _sv0t17.p1);
  sv0_box_store(_sv0t18, 3, _sv0t17.p2);
  sv0_box_store(_sv0t18, 4, _sv0t17.p3);
  sv0_vec_push(el1, _sv0t18);
  int _sv0t19 = sv0_vec_new();
  int out1 = _sv0t19;
  Instr _sv0t20;
  _sv0t20.tag = 6;
  _sv0t20.p0 = cond1;
  _sv0t20.p1 = th1;
  _sv0t20.p2 = el1;
  int _sv0t21 = emit_instr(_sv0t20, en, eb, ew, efs, eff, pool, source, starts, ends, fns, out1);
  int r1 = _sv0t21;
  if ((r1 < 0)) {
    return 1;
  } else {
  }
  int _sv0t22 = sv0_vec_len(out1);
  if ((_sv0t22 != 10)) {
    return 2;
  } else {
  }
  int _sv0t23 = sv0_vec_get(out1, 0);
  if ((_sv0t23 != 7)) {
    return 3;
  } else {
  }
  int _sv0t24 = sv0_vec_get(out1, 1);
  if ((_sv0t24 != 1)) {
    return 4;
  } else {
  }
  int _sv0t25 = sv0_vec_get(out1, 2);
  if ((_sv0t25 != 113)) {
    return 5;
  } else {
  }
  int _sv0t26 = sv0_vec_get(out1, 3);
  if ((_sv0t26 != 7)) {
    return 6;
  } else {
  }
  int _sv0t27 = sv0_vec_get(out1, 4);
  if ((_sv0t27 != 118)) {
    return 7;
  } else {
  }
  int _sv0t28 = sv0_vec_get(out1, 5);
  if ((_sv0t28 != 0)) {
    return 8;
  } else {
  }
  int _sv0t29 = sv0_vec_get(out1, 6);
  if ((_sv0t29 != 112)) {
    return 9;
  } else {
  }
  int _sv0t30 = sv0_vec_get(out1, 7);
  if ((_sv0t30 != 2)) {
    return 10;
  } else {
  }
  int _sv0t31 = sv0_vec_get(out1, 8);
  if ((_sv0t31 != 118)) {
    return 11;
  } else {
  }
  int _sv0t32 = sv0_vec_get(out1, 9);
  if ((_sv0t32 != 0)) {
    return 12;
  } else {
  }
  Expr _sv0t33;
  Value _sv0t34;
  _sv0t34.tag = 0;
  _sv0t34.p0 = 1;
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
  int cond2 = _sv0t36;
  int _sv0t37 = sv0_vec_new();
  int th2 = _sv0t37;
  Expr asgn_e;
  Value _sv0t38;
  _sv0t38.tag = 0;
  _sv0t38.p0 = 42;
  int _sv0t39 = sv0_box_alloc(3);
  sv0_box_store(_sv0t39, 0, _sv0t38.tag);
  sv0_box_store(_sv0t39, 1, _sv0t38.p0);
  sv0_box_store(_sv0t39, 2, _sv0t38.p1);
  asgn_e.tag = 0;
  asgn_e.p0 = _sv0t39;
  Instr _sv0t40;
  int _sv0t41 = sv0_box_alloc(4);
  sv0_box_store(_sv0t41, 0, asgn_e.tag);
  sv0_box_store(_sv0t41, 1, asgn_e.p0);
  sv0_box_store(_sv0t41, 2, asgn_e.p1);
  sv0_box_store(_sv0t41, 3, asgn_e.p2);
  _sv0t40.tag = 3;
  _sv0t40.p0 = 50;
  _sv0t40.p1 = _sv0t41;
  int _sv0t42 = sv0_box_alloc(5);
  sv0_box_store(_sv0t42, 0, _sv0t40.tag);
  sv0_box_store(_sv0t42, 1, _sv0t40.p0);
  sv0_box_store(_sv0t42, 2, _sv0t40.p1);
  sv0_box_store(_sv0t42, 3, _sv0t40.p2);
  sv0_box_store(_sv0t42, 4, _sv0t40.p3);
  sv0_vec_push(th2, _sv0t42);
  Instr _sv0t43;
  _sv0t43.tag = 14;
  int _sv0t44 = sv0_box_alloc(5);
  sv0_box_store(_sv0t44, 0, _sv0t43.tag);
  sv0_box_store(_sv0t44, 1, _sv0t43.p0);
  sv0_box_store(_sv0t44, 2, _sv0t43.p1);
  sv0_box_store(_sv0t44, 3, _sv0t43.p2);
  sv0_box_store(_sv0t44, 4, _sv0t43.p3);
  sv0_vec_push(th2, _sv0t44);
  int _sv0t45 = sv0_vec_new();
  int el2 = _sv0t45;
  Instr _sv0t46;
  _sv0t46.tag = 14;
  int _sv0t47 = sv0_box_alloc(5);
  sv0_box_store(_sv0t47, 0, _sv0t46.tag);
  sv0_box_store(_sv0t47, 1, _sv0t46.p0);
  sv0_box_store(_sv0t47, 2, _sv0t46.p1);
  sv0_box_store(_sv0t47, 3, _sv0t46.p2);
  sv0_box_store(_sv0t47, 4, _sv0t46.p3);
  sv0_vec_push(el2, _sv0t47);
  int _sv0t48 = sv0_vec_new();
  int out2 = _sv0t48;
  Instr _sv0t49;
  _sv0t49.tag = 6;
  _sv0t49.p0 = cond2;
  _sv0t49.p1 = th2;
  _sv0t49.p2 = el2;
  int _sv0t50 = emit_instr(_sv0t49, en, eb, ew, efs, eff, pool, source, starts, ends, fns, out2);
  int r2 = _sv0t50;
  if ((r2 < 0)) {
    return 13;
  } else {
  }
  int _sv0t51 = sv0_vec_len(out2);
  if ((_sv0t51 != 14)) {
    return 14;
  } else {
  }
  int _sv0t52 = sv0_vec_get(out2, 0);
  if ((_sv0t52 != 4)) {
    return 15;
  } else {
  }
  int _sv0t53 = sv0_vec_get(out2, 1);
  if ((_sv0t53 != 1)) {
    return 16;
  } else {
  }
  int _sv0t54 = sv0_vec_get(out2, 2);
  if ((_sv0t54 != 113)) {
    return 17;
  } else {
  }
  int _sv0t55 = sv0_vec_get(out2, 3);
  if ((_sv0t55 != 17)) {
    return 18;
  } else {
  }
  int _sv0t56 = sv0_vec_get(out2, 4);
  if ((_sv0t56 != 4)) {
    return 19;
  } else {
  }
  int _sv0t57 = sv0_vec_get(out2, 5);
  if ((_sv0t57 != 42)) {
    return 20;
  } else {
  }
  int _sv0t58 = sv0_vec_get(out2, 6);
  if ((_sv0t58 != 97)) {
    return 21;
  } else {
  }
  int _sv0t59 = sv0_vec_get(out2, 7);
  if ((_sv0t59 != 0)) {
    return 22;
  } else {
  }
  int _sv0t60 = sv0_vec_get(out2, 8);
  if ((_sv0t60 != 118)) {
    return 23;
  } else {
  }
  int _sv0t61 = sv0_vec_get(out2, 9);
  if ((_sv0t61 != 0)) {
    return 24;
  } else {
  }
  int _sv0t62 = sv0_vec_get(out2, 10);
  if ((_sv0t62 != 112)) {
    return 25;
  } else {
  }
  int _sv0t63 = sv0_vec_get(out2, 11);
  if ((_sv0t63 != 2)) {
    return 26;
  } else {
  }
  int _sv0t64 = sv0_vec_get(out2, 12);
  if ((_sv0t64 != 118)) {
    return 27;
  } else {
  }
  int _sv0t65 = sv0_vec_get(out2, 13);
  if ((_sv0t65 != 0)) {
    return 28;
  } else {
  }
  int _sv0t66 = sv0_vec_new();
  int blk_body = _sv0t66;
  Instr _sv0t67;
  _sv0t67.tag = 0;
  int _sv0t68 = sv0_box_alloc(5);
  sv0_box_store(_sv0t68, 0, _sv0t67.tag);
  sv0_box_store(_sv0t68, 1, _sv0t67.p0);
  sv0_box_store(_sv0t68, 2, _sv0t67.p1);
  sv0_box_store(_sv0t68, 3, _sv0t67.p2);
  sv0_box_store(_sv0t68, 4, _sv0t67.p3);
  sv0_vec_push(blk_body, _sv0t68);
  Instr _sv0t69;
  _sv0t69.tag = 14;
  int _sv0t70 = sv0_box_alloc(5);
  sv0_box_store(_sv0t70, 0, _sv0t69.tag);
  sv0_box_store(_sv0t70, 1, _sv0t69.p0);
  sv0_box_store(_sv0t70, 2, _sv0t69.p1);
  sv0_box_store(_sv0t70, 3, _sv0t69.p2);
  sv0_box_store(_sv0t70, 4, _sv0t69.p3);
  sv0_vec_push(blk_body, _sv0t70);
  int _sv0t71 = sv0_vec_new();
  int out3 = _sv0t71;
  Instr _sv0t72;
  _sv0t72.tag = 8;
  _sv0t72.p0 = blk_body;
  int _sv0t73 = emit_instr(_sv0t72, en, eb, ew, efs, eff, pool, source, starts, ends, fns, out3);
  int r3 = _sv0t73;
  if ((r3 != 2)) {
    return 29;
  } else {
  }
  int _sv0t74 = sv0_vec_get(out3, 0);
  if ((_sv0t74 != 118)) {
    return 30;
  } else {
  }
  int _sv0t75 = sv0_vec_get(out3, 1);
  if ((_sv0t75 != 0)) {
    return 31;
  } else {
  }
  int _sv0t76 = sv0_vec_new();
  int empty_blk = _sv0t76;
  int _sv0t77 = sv0_vec_new();
  int out4 = _sv0t77;
  Instr _sv0t78;
  _sv0t78.tag = 8;
  _sv0t78.p0 = empty_blk;
  int _sv0t79 = emit_instr(_sv0t78, en, eb, ew, efs, eff, pool, source, starts, ends, fns, out4);
  int r4 = _sv0t79;
  if ((r4 != 0)) {
    return 32;
  } else {
  }
  int _sv0t80 = sv0_vec_len(out4);
  if ((_sv0t80 != 0)) {
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
  int efs = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int eff = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int pool = _sv0t5;
  int _sv0t6 = sv0_vec_new();
  int fns = _sv0t6;
  const char* source;
  source = "+";
  int _sv0t7 = sv0_vec_new();
  int starts = _sv0t7;
  int _sv0t8 = sv0_vec_new();
  int ends = _sv0t8;
  sv0_vec_push(starts, 0);
  sv0_vec_push(ends, 1);
  sv0_vec_push(en, 50);
  sv0_vec_push(eb, 0);
  sv0_vec_push(ew, 1);
  sv0_vec_push(efs, 0);
  Expr _sv0t9;
  Value _sv0t10;
  _sv0t10.tag = 2;
  _sv0t10.p0 = 1;
  int _sv0t11 = sv0_box_alloc(3);
  sv0_box_store(_sv0t11, 0, _sv0t10.tag);
  sv0_box_store(_sv0t11, 1, _sv0t10.p0);
  sv0_box_store(_sv0t11, 2, _sv0t10.p1);
  _sv0t9.tag = 0;
  _sv0t9.p0 = _sv0t11;
  int _sv0t12 = sv0_box_alloc(4);
  sv0_box_store(_sv0t12, 0, _sv0t9.tag);
  sv0_box_store(_sv0t12, 1, _sv0t9.p0);
  sv0_box_store(_sv0t12, 2, _sv0t9.p1);
  sv0_box_store(_sv0t12, 3, _sv0t9.p2);
  int cond1 = _sv0t12;
  int _sv0t13 = sv0_vec_new();
  int body1 = _sv0t13;
  Instr _sv0t14;
  _sv0t14.tag = 14;
  int _sv0t15 = sv0_box_alloc(5);
  sv0_box_store(_sv0t15, 0, _sv0t14.tag);
  sv0_box_store(_sv0t15, 1, _sv0t14.p0);
  sv0_box_store(_sv0t15, 2, _sv0t14.p1);
  sv0_box_store(_sv0t15, 3, _sv0t14.p2);
  sv0_box_store(_sv0t15, 4, _sv0t14.p3);
  sv0_vec_push(body1, _sv0t15);
  int _sv0t16 = sv0_vec_new();
  int out1 = _sv0t16;
  Instr _sv0t17;
  _sv0t17.tag = 7;
  _sv0t17.p0 = cond1;
  _sv0t17.p1 = body1;
  int _sv0t18 = emit_instr(_sv0t17, en, eb, ew, efs, eff, pool, source, starts, ends, fns, out1);
  int r1 = _sv0t18;
  if ((r1 < 0)) {
    return 1;
  } else {
  }
  int _sv0t19 = sv0_vec_len(out1);
  if ((_sv0t19 != 8)) {
    return 2;
  } else {
  }
  int _sv0t20 = sv0_vec_get(out1, 0);
  if ((_sv0t20 != 7)) {
    return 3;
  } else {
  }
  int _sv0t21 = sv0_vec_get(out1, 1);
  if ((_sv0t21 != 1)) {
    return 4;
  } else {
  }
  int _sv0t22 = sv0_vec_get(out1, 2);
  if ((_sv0t22 != 113)) {
    return 5;
  } else {
  }
  int _sv0t23 = sv0_vec_get(out1, 3);
  if ((_sv0t23 != 7)) {
    return 6;
  } else {
  }
  int _sv0t24 = sv0_vec_get(out1, 4);
  if ((_sv0t24 != 118)) {
    return 7;
  } else {
  }
  int _sv0t25 = sv0_vec_get(out1, 5);
  if ((_sv0t25 != 0)) {
    return 8;
  } else {
  }
  int _sv0t26 = sv0_vec_get(out1, 6);
  if ((_sv0t26 != 112)) {
    return 9;
  } else {
  }
  int _sv0t27 = sv0_vec_get(out1, 7);
  int _sv0t28 = (0 - 14);
  if ((_sv0t27 != _sv0t28)) {
    return 10;
  } else {
  }
  Expr _sv0t29;
  Value _sv0t30;
  _sv0t30.tag = 2;
  _sv0t30.p0 = 1;
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
  int cond2 = _sv0t32;
  int _sv0t33 = sv0_vec_new();
  int body2 = _sv0t33;
  int _sv0t34 = sv0_vec_new();
  int out2 = _sv0t34;
  Instr _sv0t35;
  _sv0t35.tag = 7;
  _sv0t35.p0 = cond2;
  _sv0t35.p1 = body2;
  int _sv0t36 = emit_instr(_sv0t35, en, eb, ew, efs, eff, pool, source, starts, ends, fns, out2);
  int r2 = _sv0t36;
  if ((r2 < 0)) {
    return 11;
  } else {
  }
  int _sv0t37 = sv0_vec_len(out2);
  if ((_sv0t37 != 6)) {
    return 12;
  } else {
  }
  int _sv0t38 = sv0_vec_get(out2, 0);
  if ((_sv0t38 != 7)) {
    return 13;
  } else {
  }
  int _sv0t39 = sv0_vec_get(out2, 1);
  if ((_sv0t39 != 1)) {
    return 14;
  } else {
  }
  int _sv0t40 = sv0_vec_get(out2, 2);
  if ((_sv0t40 != 113)) {
    return 15;
  } else {
  }
  int _sv0t41 = sv0_vec_get(out2, 3);
  if ((_sv0t41 != 5)) {
    return 16;
  } else {
  }
  int _sv0t42 = sv0_vec_get(out2, 4);
  if ((_sv0t42 != 112)) {
    return 17;
  } else {
  }
  int _sv0t43 = sv0_vec_get(out2, 5);
  int _sv0t44 = (0 - 12);
  if ((_sv0t43 != _sv0t44)) {
    return 18;
  } else {
  }
  Expr _sv0t45;
  Value _sv0t46;
  _sv0t46.tag = 2;
  _sv0t46.p0 = 1;
  int _sv0t47 = sv0_box_alloc(3);
  sv0_box_store(_sv0t47, 0, _sv0t46.tag);
  sv0_box_store(_sv0t47, 1, _sv0t46.p0);
  sv0_box_store(_sv0t47, 2, _sv0t46.p1);
  _sv0t45.tag = 0;
  _sv0t45.p0 = _sv0t47;
  int _sv0t48 = sv0_box_alloc(4);
  sv0_box_store(_sv0t48, 0, _sv0t45.tag);
  sv0_box_store(_sv0t48, 1, _sv0t45.p0);
  sv0_box_store(_sv0t48, 2, _sv0t45.p1);
  sv0_box_store(_sv0t48, 3, _sv0t45.p2);
  int cond3 = _sv0t48;
  int _sv0t49 = sv0_vec_new();
  int body3 = _sv0t49;
  Instr _sv0t50;
  _sv0t50.tag = 9;
  int _sv0t51 = sv0_box_alloc(5);
  sv0_box_store(_sv0t51, 0, _sv0t50.tag);
  sv0_box_store(_sv0t51, 1, _sv0t50.p0);
  sv0_box_store(_sv0t51, 2, _sv0t50.p1);
  sv0_box_store(_sv0t51, 3, _sv0t50.p2);
  sv0_box_store(_sv0t51, 4, _sv0t50.p3);
  sv0_vec_push(body3, _sv0t51);
  int _sv0t52 = sv0_vec_new();
  int out3 = _sv0t52;
  Instr _sv0t53;
  _sv0t53.tag = 7;
  _sv0t53.p0 = cond3;
  _sv0t53.p1 = body3;
  int _sv0t54 = emit_instr(_sv0t53, en, eb, ew, efs, eff, pool, source, starts, ends, fns, out3);
  int r3 = _sv0t54;
  if ((r3 < 0)) {
    return 19;
  } else {
  }
  int _sv0t55 = sv0_vec_len(out3);
  if ((_sv0t55 != 8)) {
    return 20;
  } else {
  }
  int _sv0t56 = sv0_vec_get(out3, 4);
  if ((_sv0t56 != 112)) {
    return 21;
  } else {
  }
  int _sv0t57 = sv0_vec_get(out3, 5);
  if ((_sv0t57 != 5)) {
    return 22;
  } else {
  }
  int _sv0t58 = sv0_vec_get(out3, 6);
  if ((_sv0t58 != 112)) {
    return 23;
  } else {
  }
  int _sv0t59 = sv0_vec_get(out3, 7);
  int _sv0t60 = (0 - 17);
  if ((_sv0t59 != _sv0t60)) {
    return 24;
  } else {
  }
  Expr _sv0t61;
  Value _sv0t62;
  _sv0t62.tag = 2;
  _sv0t62.p0 = 1;
  int _sv0t63 = sv0_box_alloc(3);
  sv0_box_store(_sv0t63, 0, _sv0t62.tag);
  sv0_box_store(_sv0t63, 1, _sv0t62.p0);
  sv0_box_store(_sv0t63, 2, _sv0t62.p1);
  _sv0t61.tag = 0;
  _sv0t61.p0 = _sv0t63;
  int _sv0t64 = sv0_box_alloc(4);
  sv0_box_store(_sv0t64, 0, _sv0t61.tag);
  sv0_box_store(_sv0t64, 1, _sv0t61.p0);
  sv0_box_store(_sv0t64, 2, _sv0t61.p1);
  sv0_box_store(_sv0t64, 3, _sv0t61.p2);
  int cond4 = _sv0t64;
  int _sv0t65 = sv0_vec_new();
  int body4 = _sv0t65;
  Instr _sv0t66;
  _sv0t66.tag = 10;
  int _sv0t67 = sv0_box_alloc(5);
  sv0_box_store(_sv0t67, 0, _sv0t66.tag);
  sv0_box_store(_sv0t67, 1, _sv0t66.p0);
  sv0_box_store(_sv0t67, 2, _sv0t66.p1);
  sv0_box_store(_sv0t67, 3, _sv0t66.p2);
  sv0_box_store(_sv0t67, 4, _sv0t66.p3);
  sv0_vec_push(body4, _sv0t67);
  int _sv0t68 = sv0_vec_new();
  int out4 = _sv0t68;
  Instr _sv0t69;
  _sv0t69.tag = 7;
  _sv0t69.p0 = cond4;
  _sv0t69.p1 = body4;
  int _sv0t70 = emit_instr(_sv0t69, en, eb, ew, efs, eff, pool, source, starts, ends, fns, out4);
  int r4 = _sv0t70;
  if ((r4 < 0)) {
    return 25;
  } else {
  }
  int _sv0t71 = sv0_vec_len(out4);
  if ((_sv0t71 != 8)) {
    return 26;
  } else {
  }
  int _sv0t72 = sv0_vec_get(out4, 4);
  if ((_sv0t72 != 112)) {
    return 27;
  } else {
  }
  int _sv0t73 = sv0_vec_get(out4, 5);
  int _sv0t74 = (0 - 12);
  if ((_sv0t73 != _sv0t74)) {
    return 28;
  } else {
  }
  int _sv0t75 = sv0_vec_get(out4, 6);
  if ((_sv0t75 != 112)) {
    return 29;
  } else {
  }
  int _sv0t76 = sv0_vec_get(out4, 7);
  int _sv0t77 = (0 - 17);
  if ((_sv0t76 != _sv0t77)) {
    return 30;
  } else {
  }
  Expr _sv0t78;
  Value _sv0t79;
  _sv0t79.tag = 2;
  _sv0t79.p0 = 1;
  int _sv0t80 = sv0_box_alloc(3);
  sv0_box_store(_sv0t80, 0, _sv0t79.tag);
  sv0_box_store(_sv0t80, 1, _sv0t79.p0);
  sv0_box_store(_sv0t80, 2, _sv0t79.p1);
  _sv0t78.tag = 0;
  _sv0t78.p0 = _sv0t80;
  int _sv0t81 = sv0_box_alloc(4);
  sv0_box_store(_sv0t81, 0, _sv0t78.tag);
  sv0_box_store(_sv0t81, 1, _sv0t78.p0);
  sv0_box_store(_sv0t81, 2, _sv0t78.p1);
  sv0_box_store(_sv0t81, 3, _sv0t78.p2);
  int cond5 = _sv0t81;
  int _sv0t82 = sv0_vec_new();
  int body5 = _sv0t82;
  Expr asgn_e;
  Value _sv0t83;
  _sv0t83.tag = 0;
  _sv0t83.p0 = 42;
  int _sv0t84 = sv0_box_alloc(3);
  sv0_box_store(_sv0t84, 0, _sv0t83.tag);
  sv0_box_store(_sv0t84, 1, _sv0t83.p0);
  sv0_box_store(_sv0t84, 2, _sv0t83.p1);
  asgn_e.tag = 0;
  asgn_e.p0 = _sv0t84;
  Instr _sv0t85;
  int _sv0t86 = sv0_box_alloc(4);
  sv0_box_store(_sv0t86, 0, asgn_e.tag);
  sv0_box_store(_sv0t86, 1, asgn_e.p0);
  sv0_box_store(_sv0t86, 2, asgn_e.p1);
  sv0_box_store(_sv0t86, 3, asgn_e.p2);
  _sv0t85.tag = 3;
  _sv0t85.p0 = 50;
  _sv0t85.p1 = _sv0t86;
  int _sv0t87 = sv0_box_alloc(5);
  sv0_box_store(_sv0t87, 0, _sv0t85.tag);
  sv0_box_store(_sv0t87, 1, _sv0t85.p0);
  sv0_box_store(_sv0t87, 2, _sv0t85.p1);
  sv0_box_store(_sv0t87, 3, _sv0t85.p2);
  sv0_box_store(_sv0t87, 4, _sv0t85.p3);
  sv0_vec_push(body5, _sv0t87);
  Instr _sv0t88;
  _sv0t88.tag = 14;
  int _sv0t89 = sv0_box_alloc(5);
  sv0_box_store(_sv0t89, 0, _sv0t88.tag);
  sv0_box_store(_sv0t89, 1, _sv0t88.p0);
  sv0_box_store(_sv0t89, 2, _sv0t88.p1);
  sv0_box_store(_sv0t89, 3, _sv0t88.p2);
  sv0_box_store(_sv0t89, 4, _sv0t88.p3);
  sv0_vec_push(body5, _sv0t89);
  int _sv0t90 = sv0_vec_new();
  int out5 = _sv0t90;
  Instr _sv0t91;
  _sv0t91.tag = 7;
  _sv0t91.p0 = cond5;
  _sv0t91.p1 = body5;
  int _sv0t92 = emit_instr(_sv0t91, en, eb, ew, efs, eff, pool, source, starts, ends, fns, out5);
  int r5 = _sv0t92;
  if ((r5 < 0)) {
    return 31;
  } else {
  }
  int _sv0t93 = sv0_vec_len(out5);
  if ((_sv0t93 != 12)) {
    return 32;
  } else {
  }
  int _sv0t94 = sv0_vec_get(out5, 2);
  if ((_sv0t94 != 113)) {
    return 33;
  } else {
  }
  int _sv0t95 = sv0_vec_get(out5, 3);
  if ((_sv0t95 != 17)) {
    return 34;
  } else {
  }
  int _sv0t96 = sv0_vec_get(out5, 4);
  if ((_sv0t96 != 4)) {
    return 35;
  } else {
  }
  int _sv0t97 = sv0_vec_get(out5, 5);
  if ((_sv0t97 != 42)) {
    return 36;
  } else {
  }
  int _sv0t98 = sv0_vec_get(out5, 10);
  if ((_sv0t98 != 112)) {
    return 37;
  } else {
  }
  int _sv0t99 = sv0_vec_get(out5, 11);
  int _sv0t100 = (0 - 24);
  if ((_sv0t99 != _sv0t100)) {
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
  int efs = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int eff = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int pool = _sv0t5;
  const char* source;
  source = "sv0_printlnsv0_vec_newsv0_vec_pushsv0_vec_lenmyfunc";
  int _sv0t6 = sv0_vec_new();
  int starts = _sv0t6;
  int _sv0t7 = sv0_vec_new();
  int ends = _sv0t7;
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
  int _sv0t8 = sv0_vec_new();
  int fns = _sv0t8;
  sv0_vec_push(fns, 4);
  int _sv0t9 = sv0_vec_new();
  int args1 = _sv0t9;
  Value _sv0t10;
  _sv0t10.tag = 0;
  _sv0t10.p0 = 42;
  int _sv0t11 = sv0_box_alloc(3);
  sv0_box_store(_sv0t11, 0, _sv0t10.tag);
  sv0_box_store(_sv0t11, 1, _sv0t10.p0);
  sv0_box_store(_sv0t11, 2, _sv0t10.p1);
  sv0_vec_push(args1, _sv0t11);
  int _sv0t12 = sv0_vec_new();
  int out1 = _sv0t12;
  Instr _sv0t13;
  _sv0t13.tag = 11;
  _sv0t13.p0 = 0;
  _sv0t13.p1 = 0;
  _sv0t13.p2 = args1;
  _sv0t13.p3 = 0;
  int _sv0t14 = emit_instr(_sv0t13, en, eb, ew, efs, eff, pool, source, starts, ends, fns, out1);
  int r1 = _sv0t14;
  if ((r1 != 4)) {
    return 1;
  } else {
  }
  int _sv0t15 = sv0_vec_get(out1, 0);
  if ((_sv0t15 != 4)) {
    return 2;
  } else {
  }
  int _sv0t16 = sv0_vec_get(out1, 1);
  if ((_sv0t16 != 42)) {
    return 3;
  } else {
  }
  int _sv0t17 = sv0_vec_get(out1, 2);
  if ((_sv0t17 != 117)) {
    return 4;
  } else {
  }
  int _sv0t18 = sv0_vec_get(out1, 3);
  if ((_sv0t18 != 0)) {
    return 5;
  } else {
  }
  int _sv0t19 = sv0_vec_new();
  int args2 = _sv0t19;
  int _sv0t20 = sv0_vec_new();
  int out2 = _sv0t20;
  Instr _sv0t21;
  _sv0t21.tag = 11;
  _sv0t21.p0 = 10;
  _sv0t21.p1 = 1;
  _sv0t21.p2 = args2;
  _sv0t21.p3 = 0;
  int _sv0t22 = emit_instr(_sv0t21, en, eb, ew, efs, eff, pool, source, starts, ends, fns, out2);
  int r2 = _sv0t22;
  if ((r2 != 4)) {
    return 6;
  } else {
  }
  int _sv0t23 = sv0_vec_get(out2, 0);
  if ((_sv0t23 != 117)) {
    return 7;
  } else {
  }
  int _sv0t24 = sv0_vec_get(out2, 1);
  if ((_sv0t24 != 7)) {
    return 8;
  } else {
  }
  int _sv0t25 = sv0_vec_get(out2, 2);
  if ((_sv0t25 != 97)) {
    return 9;
  } else {
  }
  int _sv0t26 = sv0_vec_get(out2, 3);
  if ((_sv0t26 != 0)) {
    return 10;
  } else {
  }
  int _sv0t27 = sv0_vec_new();
  int args3 = _sv0t27;
  Value _sv0t28;
  _sv0t28.tag = 5;
  _sv0t28.p0 = 10;
  int _sv0t29 = sv0_box_alloc(3);
  sv0_box_store(_sv0t29, 0, _sv0t28.tag);
  sv0_box_store(_sv0t29, 1, _sv0t28.p0);
  sv0_box_store(_sv0t29, 2, _sv0t28.p1);
  sv0_vec_push(args3, _sv0t29);
  Value _sv0t30;
  _sv0t30.tag = 0;
  _sv0t30.p0 = 5;
  int _sv0t31 = sv0_box_alloc(3);
  sv0_box_store(_sv0t31, 0, _sv0t30.tag);
  sv0_box_store(_sv0t31, 1, _sv0t30.p0);
  sv0_box_store(_sv0t31, 2, _sv0t30.p1);
  sv0_vec_push(args3, _sv0t31);
  int _sv0t32 = sv0_vec_new();
  int out3 = _sv0t32;
  Instr _sv0t33;
  _sv0t33.tag = 11;
  _sv0t33.p0 = 0;
  _sv0t33.p1 = 2;
  _sv0t33.p2 = args3;
  _sv0t33.p3 = 0;
  int _sv0t34 = emit_instr(_sv0t33, en, eb, ew, efs, eff, pool, source, starts, ends, fns, out3);
  int r3 = _sv0t34;
  if ((r3 != 6)) {
    return 11;
  } else {
  }
  int _sv0t35 = sv0_vec_get(out3, 0);
  if ((_sv0t35 != 96)) {
    return 12;
  } else {
  }
  int _sv0t36 = sv0_vec_get(out3, 1);
  if ((_sv0t36 != 0)) {
    return 13;
  } else {
  }
  int _sv0t37 = sv0_vec_get(out3, 2);
  if ((_sv0t37 != 4)) {
    return 14;
  } else {
  }
  int _sv0t38 = sv0_vec_get(out3, 3);
  if ((_sv0t38 != 5)) {
    return 15;
  } else {
  }
  int _sv0t39 = sv0_vec_get(out3, 4);
  if ((_sv0t39 != 117)) {
    return 16;
  } else {
  }
  int _sv0t40 = sv0_vec_get(out3, 5);
  if ((_sv0t40 != 8)) {
    return 17;
  } else {
  }
  int _sv0t41 = sv0_vec_new();
  int args4 = _sv0t41;
  Value _sv0t42;
  _sv0t42.tag = 5;
  _sv0t42.p0 = 10;
  int _sv0t43 = sv0_box_alloc(3);
  sv0_box_store(_sv0t43, 0, _sv0t42.tag);
  sv0_box_store(_sv0t43, 1, _sv0t42.p0);
  sv0_box_store(_sv0t43, 2, _sv0t42.p1);
  sv0_vec_push(args4, _sv0t43);
  int _sv0t44 = sv0_vec_new();
  int out4 = _sv0t44;
  Instr _sv0t45;
  _sv0t45.tag = 11;
  _sv0t45.p0 = 20;
  _sv0t45.p1 = 3;
  _sv0t45.p2 = args4;
  _sv0t45.p3 = 0;
  int _sv0t46 = emit_instr(_sv0t45, en, eb, ew, efs, eff, pool, source, starts, ends, fns, out4);
  int r4 = _sv0t46;
  if ((r4 != 6)) {
    return 18;
  } else {
  }
  int _sv0t47 = sv0_vec_get(out4, 0);
  if ((_sv0t47 != 96)) {
    return 19;
  } else {
  }
  int _sv0t48 = sv0_vec_get(out4, 1);
  if ((_sv0t48 != 0)) {
    return 20;
  } else {
  }
  int _sv0t49 = sv0_vec_get(out4, 2);
  if ((_sv0t49 != 117)) {
    return 21;
  } else {
  }
  int _sv0t50 = sv0_vec_get(out4, 3);
  if ((_sv0t50 != 9)) {
    return 22;
  } else {
  }
  int _sv0t51 = sv0_vec_get(out4, 4);
  if ((_sv0t51 != 97)) {
    return 23;
  } else {
  }
  int _sv0t52 = sv0_vec_get(out4, 5);
  if ((_sv0t52 != 1)) {
    return 24;
  } else {
  }
  int _sv0t53 = sv0_vec_new();
  int args5 = _sv0t53;
  Value _sv0t54;
  _sv0t54.tag = 0;
  _sv0t54.p0 = 7;
  int _sv0t55 = sv0_box_alloc(3);
  sv0_box_store(_sv0t55, 0, _sv0t54.tag);
  sv0_box_store(_sv0t55, 1, _sv0t54.p0);
  sv0_box_store(_sv0t55, 2, _sv0t54.p1);
  sv0_vec_push(args5, _sv0t55);
  int _sv0t56 = sv0_vec_new();
  int out5 = _sv0t56;
  Instr _sv0t57;
  _sv0t57.tag = 11;
  _sv0t57.p0 = 20;
  _sv0t57.p1 = 4;
  _sv0t57.p2 = args5;
  _sv0t57.p3 = 0;
  int _sv0t58 = emit_instr(_sv0t57, en, eb, ew, efs, eff, pool, source, starts, ends, fns, out5);
  int r5 = _sv0t58;
  if ((r5 != 7)) {
    return 25;
  } else {
  }
  int _sv0t59 = sv0_vec_get(out5, 0);
  if ((_sv0t59 != 4)) {
    return 26;
  } else {
  }
  int _sv0t60 = sv0_vec_get(out5, 1);
  if ((_sv0t60 != 7)) {
    return 27;
  } else {
  }
  int _sv0t61 = sv0_vec_get(out5, 2);
  if ((_sv0t61 != 115)) {
    return 28;
  } else {
  }
  int _sv0t62 = sv0_vec_get(out5, 3);
  if ((_sv0t62 != 0)) {
    return 29;
  } else {
  }
  int _sv0t63 = sv0_vec_get(out5, 4);
  if ((_sv0t63 != 1)) {
    return 30;
  } else {
  }
  int _sv0t64 = sv0_vec_get(out5, 5);
  if ((_sv0t64 != 97)) {
    return 31;
  } else {
  }
  int _sv0t65 = sv0_vec_get(out5, 6);
  if ((_sv0t65 != 1)) {
    return 32;
  } else {
  }
  int _sv0t66 = sv0_vec_new();
  int args6 = _sv0t66;
  int _sv0t67 = sv0_vec_new();
  int out6 = _sv0t67;
  Instr _sv0t68;
  _sv0t68.tag = 11;
  _sv0t68.p0 = 0;
  _sv0t68.p1 = 4;
  _sv0t68.p2 = args6;
  _sv0t68.p3 = 0;
  int _sv0t69 = emit_instr(_sv0t68, en, eb, ew, efs, eff, pool, source, starts, ends, fns, out6);
  int r6 = _sv0t69;
  if ((r6 != 3)) {
    return 33;
  } else {
  }
  int _sv0t70 = sv0_vec_get(out6, 0);
  if ((_sv0t70 != 115)) {
    return 34;
  } else {
  }
  int _sv0t71 = sv0_vec_get(out6, 1);
  if ((_sv0t71 != 0)) {
    return 35;
  } else {
  }
  int _sv0t72 = sv0_vec_get(out6, 2);
  if ((_sv0t72 != 0)) {
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
  int efs = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int eff = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int pool = _sv0t5;
  int _sv0t6 = sv0_vec_new();
  int fns = _sv0t6;
  const char* source;
  source = "+";
  int _sv0t7 = sv0_vec_new();
  int starts = _sv0t7;
  int _sv0t8 = sv0_vec_new();
  int ends = _sv0t8;
  sv0_vec_push(starts, 0);
  sv0_vec_push(ends, 1);
  sv0_vec_push(en, 50);
  sv0_vec_push(eb, 0);
  sv0_vec_push(ew, 3);
  sv0_vec_push(efs, 0);
  sv0_vec_push(eff, 100);
  sv0_vec_push(eff, 200);
  sv0_vec_push(eff, 300);
  int _sv0t9 = sv0_vec_new();
  int out1 = _sv0t9;
  Instr sf1;
  Expr _sv0t10;
  Value _sv0t11;
  _sv0t11.tag = 0;
  _sv0t11.p0 = 7;
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
  sf1.tag = 5;
  sf1.p0 = 50;
  sf1.p1 = 100;
  sf1.p2 = _sv0t13;
  int _sv0t14 = emit_instr(sf1, en, eb, ew, efs, eff, pool, source, starts, ends, fns, out1);
  int r1 = _sv0t14;
  if ((r1 != 4)) {
    return 1;
  } else {
  }
  int _sv0t15 = sv0_vec_get(out1, 0);
  if ((_sv0t15 != 4)) {
    return 2;
  } else {
  }
  int _sv0t16 = sv0_vec_get(out1, 1);
  if ((_sv0t16 != 7)) {
    return 3;
  } else {
  }
  int _sv0t17 = sv0_vec_get(out1, 2);
  if ((_sv0t17 != 97)) {
    return 4;
  } else {
  }
  int _sv0t18 = sv0_vec_get(out1, 3);
  if ((_sv0t18 != 0)) {
    return 5;
  } else {
  }
  int _sv0t19 = sv0_vec_new();
  int out2 = _sv0t19;
  Instr sf2;
  Expr _sv0t20;
  Value _sv0t21;
  _sv0t21.tag = 0;
  _sv0t21.p0 = 99;
  int _sv0t22 = sv0_box_alloc(3);
  sv0_box_store(_sv0t22, 0, _sv0t21.tag);
  sv0_box_store(_sv0t22, 1, _sv0t21.p0);
  sv0_box_store(_sv0t22, 2, _sv0t21.p1);
  _sv0t20.tag = 0;
  _sv0t20.p0 = _sv0t22;
  int _sv0t23 = sv0_box_alloc(4);
  sv0_box_store(_sv0t23, 0, _sv0t20.tag);
  sv0_box_store(_sv0t23, 1, _sv0t20.p0);
  sv0_box_store(_sv0t23, 2, _sv0t20.p1);
  sv0_box_store(_sv0t23, 3, _sv0t20.p2);
  sf2.tag = 5;
  sf2.p0 = 50;
  sf2.p1 = 200;
  sf2.p2 = _sv0t23;
  int _sv0t24 = emit_instr(sf2, en, eb, ew, efs, eff, pool, source, starts, ends, fns, out2);
  int r2 = _sv0t24;
  if ((r2 != 4)) {
    return 6;
  } else {
  }
  int _sv0t25 = sv0_vec_get(out2, 2);
  if ((_sv0t25 != 97)) {
    return 7;
  } else {
  }
  int _sv0t26 = sv0_vec_get(out2, 3);
  if ((_sv0t26 != 1)) {
    return 8;
  } else {
  }
  int _sv0t27 = sv0_vec_new();
  int out3 = _sv0t27;
  Instr sf3;
  Expr _sv0t28;
  Value _sv0t29;
  _sv0t29.tag = 2;
  _sv0t29.p0 = 1;
  int _sv0t30 = sv0_box_alloc(3);
  sv0_box_store(_sv0t30, 0, _sv0t29.tag);
  sv0_box_store(_sv0t30, 1, _sv0t29.p0);
  sv0_box_store(_sv0t30, 2, _sv0t29.p1);
  _sv0t28.tag = 0;
  _sv0t28.p0 = _sv0t30;
  int _sv0t31 = sv0_box_alloc(4);
  sv0_box_store(_sv0t31, 0, _sv0t28.tag);
  sv0_box_store(_sv0t31, 1, _sv0t28.p0);
  sv0_box_store(_sv0t31, 2, _sv0t28.p1);
  sv0_box_store(_sv0t31, 3, _sv0t28.p2);
  sf3.tag = 5;
  sf3.p0 = 50;
  sf3.p1 = 300;
  sf3.p2 = _sv0t31;
  int _sv0t32 = emit_instr(sf3, en, eb, ew, efs, eff, pool, source, starts, ends, fns, out3);
  int r3 = _sv0t32;
  if ((r3 != 4)) {
    return 9;
  } else {
  }
  int _sv0t33 = sv0_vec_get(out3, 0);
  if ((_sv0t33 != 7)) {
    return 10;
  } else {
  }
  int _sv0t34 = sv0_vec_get(out3, 1);
  if ((_sv0t34 != 1)) {
    return 11;
  } else {
  }
  int _sv0t35 = sv0_vec_get(out3, 2);
  if ((_sv0t35 != 97)) {
    return 12;
  } else {
  }
  int _sv0t36 = sv0_vec_get(out3, 3);
  if ((_sv0t36 != 2)) {
    return 13;
  } else {
  }
  sv0_vec_push(en, 60);
  sv0_vec_push(eb, 3);
  sv0_vec_push(ew, 2);
  sv0_vec_push(efs, 3);
  sv0_vec_push(eff, 400);
  sv0_vec_push(eff, 500);
  int _sv0t37 = sv0_vec_new();
  int out4 = _sv0t37;
  Instr sf4;
  Expr _sv0t38;
  Value _sv0t39;
  _sv0t39.tag = 0;
  _sv0t39.p0 = 1;
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
  sf4.tag = 5;
  sf4.p0 = 60;
  sf4.p1 = 500;
  sf4.p2 = _sv0t41;
  int _sv0t42 = emit_instr(sf4, en, eb, ew, efs, eff, pool, source, starts, ends, fns, out4);
  int r4 = _sv0t42;
  if ((r4 != 4)) {
    return 14;
  } else {
  }
  int _sv0t43 = sv0_vec_get(out4, 2);
  if ((_sv0t43 != 97)) {
    return 15;
  } else {
  }
  int _sv0t44 = sv0_vec_get(out4, 3);
  if ((_sv0t44 != 4)) {
    return 16;
  } else {
  }
  int _sv0t45 = sv0_vec_new();
  int out5 = _sv0t45;
  Instr sf5;
  Expr _sv0t46;
  Value _sv0t47;
  _sv0t47.tag = 0;
  _sv0t47.p0 = 2;
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
  sf5.tag = 5;
  sf5.p0 = 60;
  sf5.p1 = 400;
  sf5.p2 = _sv0t49;
  int _sv0t50 = emit_instr(sf5, en, eb, ew, efs, eff, pool, source, starts, ends, fns, out5);
  int r5 = _sv0t50;
  if ((r5 != 4)) {
    return 17;
  } else {
  }
  int _sv0t51 = sv0_vec_get(out5, 2);
  if ((_sv0t51 != 97)) {
    return 18;
  } else {
  }
  int _sv0t52 = sv0_vec_get(out5, 3);
  if ((_sv0t52 != 3)) {
    return 19;
  } else {
  }
  int _sv0t53 = sv0_vec_new();
  int out6 = _sv0t53;
  Instr sf6;
  Expr _sv0t54;
  Value _sv0t55;
  _sv0t55.tag = 0;
  _sv0t55.p0 = 0;
  int _sv0t56 = sv0_box_alloc(3);
  sv0_box_store(_sv0t56, 0, _sv0t55.tag);
  sv0_box_store(_sv0t56, 1, _sv0t55.p0);
  sv0_box_store(_sv0t56, 2, _sv0t55.p1);
  _sv0t54.tag = 0;
  _sv0t54.p0 = _sv0t56;
  int _sv0t57 = sv0_box_alloc(4);
  sv0_box_store(_sv0t57, 0, _sv0t54.tag);
  sv0_box_store(_sv0t57, 1, _sv0t54.p0);
  sv0_box_store(_sv0t57, 2, _sv0t54.p1);
  sv0_box_store(_sv0t57, 3, _sv0t54.p2);
  sf6.tag = 5;
  sf6.p0 = 50;
  sf6.p1 = 999;
  sf6.p2 = _sv0t57;
  int _sv0t58 = emit_instr(sf6, en, eb, ew, efs, eff, pool, source, starts, ends, fns, out6);
  int r6 = _sv0t58;
  int _sv0t59 = (0 - 1);
  if ((r6 != _sv0t59)) {
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
  int efs = _sv0t3;
  int _sv0t4 = sv0_vec_new();
  int eff = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int pool = _sv0t5;
  int _sv0t6 = sv0_vec_new();
  int fns = _sv0t6;
  const char* source;
  source = "+";
  int _sv0t7 = sv0_vec_new();
  int starts = _sv0t7;
  int _sv0t8 = sv0_vec_new();
  int ends = _sv0t8;
  sv0_vec_push(starts, 0);
  sv0_vec_push(ends, 1);
  int _sv0t9 = sv0_vec_new();
  int out1 = _sv0t9;
  Instr req1;
  Expr _sv0t10;
  Value _sv0t11;
  _sv0t11.tag = 2;
  _sv0t11.p0 = 1;
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
  req1.tag = 12;
  req1.p0 = _sv0t13;
  req1.p1 = 42;
  int _sv0t14 = emit_instr(req1, en, eb, ew, efs, eff, pool, source, starts, ends, fns, out1);
  int r1 = _sv0t14;
  if ((r1 != 4)) {
    return 1;
  } else {
  }
  int _sv0t15 = sv0_vec_get(out1, 0);
  if ((_sv0t15 != 7)) {
    return 2;
  } else {
  }
  int _sv0t16 = sv0_vec_get(out1, 1);
  if ((_sv0t16 != 1)) {
    return 3;
  } else {
  }
  int _sv0t17 = sv0_vec_get(out1, 2);
  if ((_sv0t17 != 160)) {
    return 4;
  } else {
  }
  int _sv0t18 = sv0_vec_get(out1, 3);
  if ((_sv0t18 != 0)) {
    return 5;
  } else {
  }
  int _sv0t19 = pool_size(pool);
  if ((_sv0t19 != 1)) {
    return 6;
  } else {
  }
  int _sv0t20 = sv0_vec_new();
  int out2 = _sv0t20;
  Instr ens1;
  Expr _sv0t21;
  Value _sv0t22;
  _sv0t22.tag = 2;
  _sv0t22.p0 = 0;
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
  ens1.tag = 13;
  ens1.p0 = _sv0t24;
  ens1.p1 = 42;
  int _sv0t25 = emit_instr(ens1, en, eb, ew, efs, eff, pool, source, starts, ends, fns, out2);
  int r2 = _sv0t25;
  if ((r2 != 4)) {
    return 7;
  } else {
  }
  int _sv0t26 = sv0_vec_get(out2, 0);
  if ((_sv0t26 != 7)) {
    return 8;
  } else {
  }
  int _sv0t27 = sv0_vec_get(out2, 1);
  if ((_sv0t27 != 0)) {
    return 9;
  } else {
  }
  int _sv0t28 = sv0_vec_get(out2, 2);
  if ((_sv0t28 != 160)) {
    return 10;
  } else {
  }
  int _sv0t29 = sv0_vec_get(out2, 3);
  if ((_sv0t29 != 1)) {
    return 11;
  } else {
  }
  int _sv0t30 = pool_size(pool);
  if ((_sv0t30 != 2)) {
    return 12;
  } else {
  }
  int _sv0t31 = sv0_vec_new();
  int out3 = _sv0t31;
  Instr req2;
  Expr _sv0t32;
  Value _sv0t33;
  _sv0t33.tag = 2;
  _sv0t33.p0 = 1;
  int _sv0t34 = sv0_box_alloc(3);
  sv0_box_store(_sv0t34, 0, _sv0t33.tag);
  sv0_box_store(_sv0t34, 1, _sv0t33.p0);
  sv0_box_store(_sv0t34, 2, _sv0t33.p1);
  _sv0t32.tag = 0;
  _sv0t32.p0 = _sv0t34;
  int _sv0t35 = sv0_box_alloc(4);
  sv0_box_store(_sv0t35, 0, _sv0t32.tag);
  sv0_box_store(_sv0t35, 1, _sv0t32.p0);
  sv0_box_store(_sv0t35, 2, _sv0t32.p1);
  sv0_box_store(_sv0t35, 3, _sv0t32.p2);
  req2.tag = 12;
  req2.p0 = _sv0t35;
  req2.p1 = 42;
  int _sv0t36 = emit_instr(req2, en, eb, ew, efs, eff, pool, source, starts, ends, fns, out3);
  int r3 = _sv0t36;
  if ((r3 != 4)) {
    return 13;
  } else {
  }
  int _sv0t37 = sv0_vec_get(out3, 3);
  if ((_sv0t37 != 0)) {
    return 14;
  } else {
  }
  int _sv0t38 = pool_size(pool);
  if ((_sv0t38 != 2)) {
    return 15;
  } else {
  }
  int _sv0t39 = sv0_vec_new();
  int out4 = _sv0t39;
  Instr req3;
  Expr _sv0t40;
  Value _sv0t41;
  _sv0t41.tag = 2;
  _sv0t41.p0 = 1;
  int _sv0t42 = sv0_box_alloc(3);
  sv0_box_store(_sv0t42, 0, _sv0t41.tag);
  sv0_box_store(_sv0t42, 1, _sv0t41.p0);
  sv0_box_store(_sv0t42, 2, _sv0t41.p1);
  _sv0t40.tag = 0;
  _sv0t40.p0 = _sv0t42;
  int _sv0t43 = sv0_box_alloc(4);
  sv0_box_store(_sv0t43, 0, _sv0t40.tag);
  sv0_box_store(_sv0t43, 1, _sv0t40.p0);
  sv0_box_store(_sv0t43, 2, _sv0t40.p1);
  sv0_box_store(_sv0t43, 3, _sv0t40.p2);
  req3.tag = 12;
  req3.p0 = _sv0t43;
  req3.p1 = 99;
  int _sv0t44 = emit_instr(req3, en, eb, ew, efs, eff, pool, source, starts, ends, fns, out4);
  int r4 = _sv0t44;
  if ((r4 != 4)) {
    return 16;
  } else {
  }
  int _sv0t45 = sv0_vec_get(out4, 3);
  if ((_sv0t45 != 2)) {
    return 17;
  } else {
  }
  int _sv0t46 = pool_size(pool);
  if ((_sv0t46 != 3)) {
    return 18;
  } else {
  }
  int _sv0t47 = contract_pool_key(42, 0);
  int k0 = _sv0t47;
  int _sv0t48 = contract_pool_key(42, 1);
  int k1 = _sv0t48;
  int _sv0t49 = contract_pool_key(99, 0);
  int k2 = _sv0t49;
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
  int _sv0t5 = compute_arity(pc0, sn, sfc, enn, enw);
  if ((_sv0t5 != 0)) {
    return 1;
  } else {
  }
  int _sv0t6 = sv0_vec_new();
  int pc1 = _sv0t6;
  sv0_vec_push(pc1, 777);
  int _sv0t7 = compute_arity(pc1, sn, sfc, enn, enw);
  if ((_sv0t7 != 1)) {
    return 2;
  } else {
  }
  int _sv0t8 = sv0_vec_new();
  int pc2 = _sv0t8;
  sv0_vec_push(pc2, 777);
  sv0_vec_push(pc2, 778);
  int _sv0t9 = compute_arity(pc2, sn, sfc, enn, enw);
  if ((_sv0t9 != 2)) {
    return 3;
  } else {
  }
  int _sv0t10 = sv0_vec_new();
  int sn2 = _sv0t10;
  int _sv0t11 = sv0_vec_new();
  int sfc2 = _sv0t11;
  sv0_vec_push(sn2, 500);
  sv0_vec_push(sfc2, 3);
  int _sv0t12 = sv0_vec_new();
  int pc3 = _sv0t12;
  sv0_vec_push(pc3, 500);
  int _sv0t13 = compute_arity(pc3, sn2, sfc2, enn, enw);
  if ((_sv0t13 != 3)) {
    return 4;
  } else {
  }
  int _sv0t14 = sv0_vec_new();
  int pc4 = _sv0t14;
  sv0_vec_push(pc4, 777);
  sv0_vec_push(pc4, 500);
  int _sv0t15 = compute_arity(pc4, sn2, sfc2, enn, enw);
  if ((_sv0t15 != 4)) {
    return 5;
  } else {
  }
  int _sv0t16 = sv0_vec_new();
  int enn2 = _sv0t16;
  int _sv0t17 = sv0_vec_new();
  int enw2 = _sv0t17;
  sv0_vec_push(enn2, 600);
  sv0_vec_push(enw2, 2);
  int _sv0t18 = sv0_vec_new();
  int pc5 = _sv0t18;
  sv0_vec_push(pc5, 600);
  int _sv0t19 = compute_arity(pc5, sn, sfc, enn2, enw2);
  if ((_sv0t19 != 2)) {
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
  int _sv0t14 = emit_fn(42, pn0, pc0, ins0, sn, sfc, enn, enw, pool, source, starts, ends, fns, ft);
  int r0 = _sv0t14;
  if ((r0 != 0)) {
    return 1;
  } else {
  }
  int _sv0t15 = sv0_vec_len(ft);
  if ((_sv0t15 != 4)) {
    return 2;
  } else {
  }
  int _sv0t16 = sv0_vec_get(ft, 0);
  int ni0 = _sv0t16;
  if ((ni0 != 0)) {
    return 3;
  } else {
  }
  int _sv0t17 = sv0_vec_get(ft, 1);
  if ((_sv0t17 != 0)) {
    return 4;
  } else {
  }
  int _sv0t18 = sv0_vec_get(ft, 2);
  if ((_sv0t18 != 0)) {
    return 5;
  } else {
  }
  int _sv0t19 = sv0_vec_get(ft, 3);
  int code0 = _sv0t19;
  int _sv0t20 = sv0_vec_len(code0);
  if ((_sv0t20 != 2)) {
    return 6;
  } else {
  }
  int _sv0t21 = sv0_vec_get(code0, 0);
  if ((_sv0t21 != 118)) {
    return 7;
  } else {
  }
  int _sv0t22 = sv0_vec_get(code0, 1);
  if ((_sv0t22 != 0)) {
    return 8;
  } else {
  }
  int _sv0t23 = sv0_vec_new();
  int pn1 = _sv0t23;
  int _sv0t24 = sv0_vec_new();
  int pc1 = _sv0t24;
  sv0_vec_push(pn1, 10);
  sv0_vec_push(pc1, 777);
  int _sv0t25 = sv0_vec_new();
  int ins1 = _sv0t25;
  Instr _sv0t26;
  Expr _sv0t27;
  Value _sv0t28;
  _sv0t28.tag = 0;
  _sv0t28.p0 = 99;
  int _sv0t29 = sv0_box_alloc(3);
  sv0_box_store(_sv0t29, 0, _sv0t28.tag);
  sv0_box_store(_sv0t29, 1, _sv0t28.p0);
  sv0_box_store(_sv0t29, 2, _sv0t28.p1);
  _sv0t27.tag = 0;
  _sv0t27.p0 = _sv0t29;
  int _sv0t30 = sv0_box_alloc(4);
  sv0_box_store(_sv0t30, 0, _sv0t27.tag);
  sv0_box_store(_sv0t30, 1, _sv0t27.p0);
  sv0_box_store(_sv0t30, 2, _sv0t27.p1);
  sv0_box_store(_sv0t30, 3, _sv0t27.p2);
  _sv0t26.tag = 3;
  _sv0t26.p0 = 20;
  _sv0t26.p1 = _sv0t30;
  int _sv0t31 = sv0_box_alloc(5);
  sv0_box_store(_sv0t31, 0, _sv0t26.tag);
  sv0_box_store(_sv0t31, 1, _sv0t26.p0);
  sv0_box_store(_sv0t31, 2, _sv0t26.p1);
  sv0_box_store(_sv0t31, 3, _sv0t26.p2);
  sv0_box_store(_sv0t31, 4, _sv0t26.p3);
  sv0_vec_push(ins1, _sv0t31);
  Instr _sv0t32;
  Value _sv0t33;
  _sv0t33.tag = 5;
  _sv0t33.p0 = 20;
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
  sv0_vec_push(ins1, _sv0t35);
  int _sv0t36 = emit_fn(43, pn1, pc1, ins1, sn, sfc, enn, enw, pool, source, starts, ends, fns, ft);
  int r1 = _sv0t36;
  if ((r1 != 0)) {
    return 9;
  } else {
  }
  int _sv0t37 = sv0_vec_len(ft);
  if ((_sv0t37 != 8)) {
    return 10;
  } else {
  }
  int _sv0t38 = sv0_vec_get(ft, 5);
  if ((_sv0t38 != 1)) {
    return 11;
  } else {
  }
  int _sv0t39 = sv0_vec_get(ft, 6);
  int lc1 = _sv0t39;
  if ((lc1 != 2)) {
    return 12;
  } else {
  }
  int _sv0t40 = sv0_vec_get(ft, 7);
  int code1 = _sv0t40;
  int _sv0t41 = sv0_vec_len(code1);
  if ((_sv0t41 < 2)) {
    return 13;
  } else {
  }
  int _sv0t42 = pool_size(pool);
  if ((_sv0t42 != 2)) {
    return 14;
  } else {
  }
  int _sv0t43 = sv0_vec_new();
  int sn3 = _sv0t43;
  int _sv0t44 = sv0_vec_new();
  int sfc3 = _sv0t44;
  sv0_vec_push(sn3, 500);
  sv0_vec_push(sfc3, 3);
  int _sv0t45 = sv0_vec_new();
  int pn2 = _sv0t45;
  int _sv0t46 = sv0_vec_new();
  int pc2 = _sv0t46;
  sv0_vec_push(pn2, 30);
  sv0_vec_push(pc2, 500);
  int _sv0t47 = sv0_vec_new();
  int ins2 = _sv0t47;
  Instr _sv0t48;
  _sv0t48.tag = 14;
  int _sv0t49 = sv0_box_alloc(5);
  sv0_box_store(_sv0t49, 0, _sv0t48.tag);
  sv0_box_store(_sv0t49, 1, _sv0t48.p0);
  sv0_box_store(_sv0t49, 2, _sv0t48.p1);
  sv0_box_store(_sv0t49, 3, _sv0t48.p2);
  sv0_box_store(_sv0t49, 4, _sv0t48.p3);
  sv0_vec_push(ins2, _sv0t49);
  int _sv0t50 = sv0_vec_new();
  int pool2 = _sv0t50;
  int _sv0t51 = sv0_vec_new();
  int ft2 = _sv0t51;
  int _sv0t52 = emit_fn(44, pn2, pc2, ins2, sn3, sfc3, enn, enw, pool2, source, starts, ends, fns, ft2);
  int r2 = _sv0t52;
  if ((r2 != 0)) {
    return 15;
  } else {
  }
  int _sv0t53 = sv0_vec_get(ft2, 1);
  if ((_sv0t53 != 3)) {
    return 16;
  } else {
  }
  int _sv0t54 = sv0_vec_get(ft2, 2);
  if ((_sv0t54 != 3)) {
    return 17;
  } else {
  }
  int _sv0t55 = sv0_vec_new();
  int pn3 = _sv0t55;
  int _sv0t56 = sv0_vec_new();
  int pc3 = _sv0t56;
  int _sv0t57 = sv0_vec_new();
  int ins3 = _sv0t57;
  Instr _sv0t58;
  _sv0t58.tag = 14;
  int _sv0t59 = sv0_box_alloc(5);
  sv0_box_store(_sv0t59, 0, _sv0t58.tag);
  sv0_box_store(_sv0t59, 1, _sv0t58.p0);
  sv0_box_store(_sv0t59, 2, _sv0t58.p1);
  sv0_box_store(_sv0t59, 3, _sv0t58.p2);
  sv0_box_store(_sv0t59, 4, _sv0t58.p3);
  sv0_vec_push(ins3, _sv0t59);
  int _sv0t60 = emit_fn(45, pn3, pc3, ins3, sn, sfc, enn, enw, pool2, source, starts, ends, fns, ft2);
  int r3 = _sv0t60;
  if ((r3 != 0)) {
    return 18;
  } else {
  }
  int _sv0t61 = sv0_vec_len(ft2);
  if ((_sv0t61 != 8)) {
    return 19;
  } else {
  }
  int _sv0t62 = sv0_vec_get(ft2, 4);
  int _sv0t63 = sv0_vec_get(ft2, 0);
  int _sv0t64 = (_sv0t63 + 1);
  if ((_sv0t62 != _sv0t64)) {
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
  int efs0 = _sv0t7;
  int _sv0t8 = sv0_vec_new();
  int pn0 = _sv0t8;
  int _sv0t9 = sv0_vec_new();
  int pc0 = _sv0t9;
  int _sv0t10 = sv0_vec_new();
  int ins0 = _sv0t10;
  int _sv0t11 = local_count_and_env(pn0, pc0, ins0, en0, eb0, ew0, efs0, sn, sfc, enn, enw);
  int lc0 = _sv0t11;
  if ((lc0 != 0)) {
    return 1;
  } else {
  }
  int _sv0t12 = sv0_vec_len(en0);
  if ((_sv0t12 != 0)) {
    return 2;
  } else {
  }
  int _sv0t13 = sv0_vec_new();
  int en1 = _sv0t13;
  int _sv0t14 = sv0_vec_new();
  int eb1 = _sv0t14;
  int _sv0t15 = sv0_vec_new();
  int ew1 = _sv0t15;
  int _sv0t16 = sv0_vec_new();
  int efs1 = _sv0t16;
  int _sv0t17 = sv0_vec_new();
  int pn1 = _sv0t17;
  int _sv0t18 = sv0_vec_new();
  int pc1 = _sv0t18;
  sv0_vec_push(pn1, 10);
  sv0_vec_push(pc1, 777);
  sv0_vec_push(pn1, 20);
  sv0_vec_push(pc1, 778);
  int _sv0t19 = sv0_vec_new();
  int ins1 = _sv0t19;
  int _sv0t20 = local_count_and_env(pn1, pc1, ins1, en1, eb1, ew1, efs1, sn, sfc, enn, enw);
  int lc1 = _sv0t20;
  if ((lc1 != 2)) {
    return 3;
  } else {
  }
  int _sv0t21 = sv0_vec_len(en1);
  if ((_sv0t21 != 2)) {
    return 4;
  } else {
  }
  int _sv0t22 = sv0_vec_get(en1, 0);
  if ((_sv0t22 != 10)) {
    return 5;
  } else {
  }
  int _sv0t23 = sv0_vec_get(eb1, 0);
  if ((_sv0t23 != 0)) {
    return 6;
  } else {
  }
  int _sv0t24 = sv0_vec_get(ew1, 0);
  if ((_sv0t24 != 1)) {
    return 7;
  } else {
  }
  int _sv0t25 = sv0_vec_get(en1, 1);
  if ((_sv0t25 != 20)) {
    return 8;
  } else {
  }
  int _sv0t26 = sv0_vec_get(eb1, 1);
  if ((_sv0t26 != 1)) {
    return 9;
  } else {
  }
  int _sv0t27 = sv0_vec_new();
  int sn2 = _sv0t27;
  int _sv0t28 = sv0_vec_new();
  int sfc2 = _sv0t28;
  sv0_vec_push(sn2, 500);
  sv0_vec_push(sfc2, 3);
  int _sv0t29 = sv0_vec_new();
  int en2 = _sv0t29;
  int _sv0t30 = sv0_vec_new();
  int eb2 = _sv0t30;
  int _sv0t31 = sv0_vec_new();
  int ew2 = _sv0t31;
  int _sv0t32 = sv0_vec_new();
  int efs2 = _sv0t32;
  int _sv0t33 = sv0_vec_new();
  int pn2 = _sv0t33;
  int _sv0t34 = sv0_vec_new();
  int pc2 = _sv0t34;
  sv0_vec_push(pn2, 30);
  sv0_vec_push(pc2, 500);
  int _sv0t35 = sv0_vec_new();
  int ins2 = _sv0t35;
  int _sv0t36 = local_count_and_env(pn2, pc2, ins2, en2, eb2, ew2, efs2, sn2, sfc2, enn, enw);
  int lc2 = _sv0t36;
  if ((lc2 != 3)) {
    return 10;
  } else {
  }
  int _sv0t37 = sv0_vec_get(ew2, 0);
  if ((_sv0t37 != 3)) {
    return 11;
  } else {
  }
  int _sv0t38 = sv0_vec_get(eb2, 0);
  if ((_sv0t38 != 0)) {
    return 12;
  } else {
  }
  int _sv0t39 = sv0_vec_new();
  int en3 = _sv0t39;
  int _sv0t40 = sv0_vec_new();
  int eb3 = _sv0t40;
  int _sv0t41 = sv0_vec_new();
  int ew3 = _sv0t41;
  int _sv0t42 = sv0_vec_new();
  int efs3 = _sv0t42;
  int _sv0t43 = sv0_vec_new();
  int pn3 = _sv0t43;
  int _sv0t44 = sv0_vec_new();
  int pc3 = _sv0t44;
  int _sv0t45 = sv0_vec_new();
  int ins3 = _sv0t45;
  Instr _sv0t46;
  _sv0t46.tag = 1;
  _sv0t46.p0 = 40;
  int _sv0t47 = sv0_box_alloc(5);
  sv0_box_store(_sv0t47, 0, _sv0t46.tag);
  sv0_box_store(_sv0t47, 1, _sv0t46.p0);
  sv0_box_store(_sv0t47, 2, _sv0t46.p1);
  sv0_box_store(_sv0t47, 3, _sv0t46.p2);
  sv0_box_store(_sv0t47, 4, _sv0t46.p3);
  sv0_vec_push(ins3, _sv0t47);
  int _sv0t48 = local_count_and_env(pn3, pc3, ins3, en3, eb3, ew3, efs3, sn, sfc, enn, enw);
  int lc3 = _sv0t48;
  if ((lc3 != 1)) {
    return 13;
  } else {
  }
  int _sv0t49 = sv0_vec_get(en3, 0);
  if ((_sv0t49 != 40)) {
    return 14;
  } else {
  }
  int _sv0t50 = sv0_vec_get(eb3, 0);
  if ((_sv0t50 != 0)) {
    return 15;
  } else {
  }
  int _sv0t51 = sv0_vec_get(ew3, 0);
  if ((_sv0t51 != 1)) {
    return 16;
  } else {
  }
  int _sv0t52 = sv0_vec_new();
  int en4 = _sv0t52;
  int _sv0t53 = sv0_vec_new();
  int eb4 = _sv0t53;
  int _sv0t54 = sv0_vec_new();
  int ew4 = _sv0t54;
  int _sv0t55 = sv0_vec_new();
  int efs4 = _sv0t55;
  int _sv0t56 = sv0_vec_new();
  int pn4 = _sv0t56;
  int _sv0t57 = sv0_vec_new();
  int pc4 = _sv0t57;
  int _sv0t58 = sv0_vec_new();
  int ins4 = _sv0t58;
  Instr _sv0t59;
  _sv0t59.tag = 1;
  _sv0t59.p0 = 50;
  int _sv0t60 = sv0_box_alloc(5);
  sv0_box_store(_sv0t60, 0, _sv0t59.tag);
  sv0_box_store(_sv0t60, 1, _sv0t59.p0);
  sv0_box_store(_sv0t60, 2, _sv0t59.p1);
  sv0_box_store(_sv0t60, 3, _sv0t59.p2);
  sv0_box_store(_sv0t60, 4, _sv0t59.p3);
  sv0_vec_push(ins4, _sv0t60);
  Instr _sv0t61;
  _sv0t61.tag = 1;
  _sv0t61.p0 = 50;
  int _sv0t62 = sv0_box_alloc(5);
  sv0_box_store(_sv0t62, 0, _sv0t61.tag);
  sv0_box_store(_sv0t62, 1, _sv0t61.p0);
  sv0_box_store(_sv0t62, 2, _sv0t61.p1);
  sv0_box_store(_sv0t62, 3, _sv0t61.p2);
  sv0_box_store(_sv0t62, 4, _sv0t61.p3);
  sv0_vec_push(ins4, _sv0t62);
  int _sv0t63 = local_count_and_env(pn4, pc4, ins4, en4, eb4, ew4, efs4, sn, sfc, enn, enw);
  int lc4 = _sv0t63;
  if ((lc4 != 1)) {
    return 17;
  } else {
  }
  int _sv0t64 = sv0_vec_len(en4);
  if ((_sv0t64 != 1)) {
    return 18;
  } else {
  }
  int _sv0t65 = sv0_vec_new();
  int en5 = _sv0t65;
  int _sv0t66 = sv0_vec_new();
  int eb5 = _sv0t66;
  int _sv0t67 = sv0_vec_new();
  int ew5 = _sv0t67;
  int _sv0t68 = sv0_vec_new();
  int efs5 = _sv0t68;
  int _sv0t69 = sv0_vec_new();
  int pn5 = _sv0t69;
  int _sv0t70 = sv0_vec_new();
  int pc5 = _sv0t70;
  int _sv0t71 = sv0_vec_new();
  int ins5 = _sv0t71;
  Instr _sv0t72;
  Expr _sv0t73;
  Value _sv0t74;
  _sv0t74.tag = 0;
  _sv0t74.p0 = 7;
  int _sv0t75 = sv0_box_alloc(3);
  sv0_box_store(_sv0t75, 0, _sv0t74.tag);
  sv0_box_store(_sv0t75, 1, _sv0t74.p0);
  sv0_box_store(_sv0t75, 2, _sv0t74.p1);
  _sv0t73.tag = 0;
  _sv0t73.p0 = _sv0t75;
  int _sv0t76 = sv0_box_alloc(4);
  sv0_box_store(_sv0t76, 0, _sv0t73.tag);
  sv0_box_store(_sv0t76, 1, _sv0t73.p0);
  sv0_box_store(_sv0t76, 2, _sv0t73.p1);
  sv0_box_store(_sv0t76, 3, _sv0t73.p2);
  _sv0t72.tag = 3;
  _sv0t72.p0 = 60;
  _sv0t72.p1 = _sv0t76;
  int _sv0t77 = sv0_box_alloc(5);
  sv0_box_store(_sv0t77, 0, _sv0t72.tag);
  sv0_box_store(_sv0t77, 1, _sv0t72.p0);
  sv0_box_store(_sv0t77, 2, _sv0t72.p1);
  sv0_box_store(_sv0t77, 3, _sv0t72.p2);
  sv0_box_store(_sv0t77, 4, _sv0t72.p3);
  sv0_vec_push(ins5, _sv0t77);
  int _sv0t78 = local_count_and_env(pn5, pc5, ins5, en5, eb5, ew5, efs5, sn, sfc, enn, enw);
  int lc5 = _sv0t78;
  if ((lc5 != 1)) {
    return 19;
  } else {
  }
  int _sv0t79 = sv0_vec_get(en5, 0);
  if ((_sv0t79 != 60)) {
    return 20;
  } else {
  }
  int _sv0t80 = sv0_vec_new();
  int en6 = _sv0t80;
  int _sv0t81 = sv0_vec_new();
  int eb6 = _sv0t81;
  int _sv0t82 = sv0_vec_new();
  int ew6 = _sv0t82;
  int _sv0t83 = sv0_vec_new();
  int efs6 = _sv0t83;
  int _sv0t84 = sv0_vec_new();
  int pn6 = _sv0t84;
  int _sv0t85 = sv0_vec_new();
  int pc6 = _sv0t85;
  int _sv0t86 = sv0_vec_new();
  int ins6 = _sv0t86;
  int _sv0t87 = sv0_vec_new();
  int cargs = _sv0t87;
  Instr _sv0t88;
  _sv0t88.tag = 11;
  _sv0t88.p0 = 70;
  _sv0t88.p1 = 100;
  _sv0t88.p2 = cargs;
  _sv0t88.p3 = 999;
  int _sv0t89 = sv0_box_alloc(5);
  sv0_box_store(_sv0t89, 0, _sv0t88.tag);
  sv0_box_store(_sv0t89, 1, _sv0t88.p0);
  sv0_box_store(_sv0t89, 2, _sv0t88.p1);
  sv0_box_store(_sv0t89, 3, _sv0t88.p2);
  sv0_box_store(_sv0t89, 4, _sv0t88.p3);
  sv0_vec_push(ins6, _sv0t89);
  int _sv0t90 = local_count_and_env(pn6, pc6, ins6, en6, eb6, ew6, efs6, sn, sfc, enn, enw);
  int lc6 = _sv0t90;
  if ((lc6 != 1)) {
    return 21;
  } else {
  }
  int _sv0t91 = sv0_vec_get(en6, 0);
  if ((_sv0t91 != 70)) {
    return 22;
  } else {
  }
  int _sv0t92 = sv0_vec_new();
  int en7 = _sv0t92;
  int _sv0t93 = sv0_vec_new();
  int eb7 = _sv0t93;
  int _sv0t94 = sv0_vec_new();
  int ew7 = _sv0t94;
  int _sv0t95 = sv0_vec_new();
  int efs7 = _sv0t95;
  int _sv0t96 = sv0_vec_new();
  int pn7 = _sv0t96;
  int _sv0t97 = sv0_vec_new();
  int pc7 = _sv0t97;
  int _sv0t98 = sv0_vec_new();
  int ins7 = _sv0t98;
  int _sv0t99 = sv0_vec_new();
  int cargs2 = _sv0t99;
  Instr _sv0t100;
  _sv0t100.tag = 11;
  _sv0t100.p0 = 0;
  _sv0t100.p1 = 100;
  _sv0t100.p2 = cargs2;
  _sv0t100.p3 = 999;
  int _sv0t101 = sv0_box_alloc(5);
  sv0_box_store(_sv0t101, 0, _sv0t100.tag);
  sv0_box_store(_sv0t101, 1, _sv0t100.p0);
  sv0_box_store(_sv0t101, 2, _sv0t100.p1);
  sv0_box_store(_sv0t101, 3, _sv0t100.p2);
  sv0_box_store(_sv0t101, 4, _sv0t100.p3);
  sv0_vec_push(ins7, _sv0t101);
  int _sv0t102 = local_count_and_env(pn7, pc7, ins7, en7, eb7, ew7, efs7, sn, sfc, enn, enw);
  int lc7 = _sv0t102;
  if ((lc7 != 0)) {
    return 23;
  } else {
  }
  int _sv0t103 = sv0_vec_len(en7);
  if ((_sv0t103 != 0)) {
    return 24;
  } else {
  }
  int _sv0t104 = sv0_vec_new();
  int en8 = _sv0t104;
  int _sv0t105 = sv0_vec_new();
  int eb8 = _sv0t105;
  int _sv0t106 = sv0_vec_new();
  int ew8 = _sv0t106;
  int _sv0t107 = sv0_vec_new();
  int efs8 = _sv0t107;
  int _sv0t108 = sv0_vec_new();
  int pn8 = _sv0t108;
  int _sv0t109 = sv0_vec_new();
  int pc8 = _sv0t109;
  int _sv0t110 = sv0_vec_new();
  int ins8 = _sv0t110;
  int _sv0t111 = sv0_vec_new();
  int cargs3 = _sv0t111;
  Instr _sv0t112;
  _sv0t112.tag = 11;
  _sv0t112.p0 = 80;
  _sv0t112.p1 = 100;
  _sv0t112.p2 = cargs3;
  _sv0t112.p3 = 500;
  int _sv0t113 = sv0_box_alloc(5);
  sv0_box_store(_sv0t113, 0, _sv0t112.tag);
  sv0_box_store(_sv0t113, 1, _sv0t112.p0);
  sv0_box_store(_sv0t113, 2, _sv0t112.p1);
  sv0_box_store(_sv0t113, 3, _sv0t112.p2);
  sv0_box_store(_sv0t113, 4, _sv0t112.p3);
  sv0_vec_push(ins8, _sv0t113);
  int _sv0t114 = local_count_and_env(pn8, pc8, ins8, en8, eb8, ew8, efs8, sn2, sfc2, enn, enw);
  int lc8 = _sv0t114;
  if ((lc8 != 3)) {
    return 25;
  } else {
  }
  int _sv0t115 = sv0_vec_get(ew8, 0);
  if ((_sv0t115 != 3)) {
    return 26;
  } else {
  }
  int _sv0t116 = sv0_vec_new();
  int en9 = _sv0t116;
  int _sv0t117 = sv0_vec_new();
  int eb9 = _sv0t117;
  int _sv0t118 = sv0_vec_new();
  int ew9 = _sv0t118;
  int _sv0t119 = sv0_vec_new();
  int efs9 = _sv0t119;
  int _sv0t120 = sv0_vec_new();
  int pn9 = _sv0t120;
  int _sv0t121 = sv0_vec_new();
  int pc9 = _sv0t121;
  int _sv0t122 = sv0_vec_new();
  int ins9 = _sv0t122;
  int _sv0t123 = sv0_vec_new();
  int th9 = _sv0t123;
  int _sv0t124 = sv0_vec_new();
  int el9 = _sv0t124;
  Instr _sv0t125;
  _sv0t125.tag = 1;
  _sv0t125.p0 = 90;
  int _sv0t126 = sv0_box_alloc(5);
  sv0_box_store(_sv0t126, 0, _sv0t125.tag);
  sv0_box_store(_sv0t126, 1, _sv0t125.p0);
  sv0_box_store(_sv0t126, 2, _sv0t125.p1);
  sv0_box_store(_sv0t126, 3, _sv0t125.p2);
  sv0_box_store(_sv0t126, 4, _sv0t125.p3);
  sv0_vec_push(th9, _sv0t126);
  Instr _sv0t127;
  _sv0t127.tag = 1;
  _sv0t127.p0 = 91;
  int _sv0t128 = sv0_box_alloc(5);
  sv0_box_store(_sv0t128, 0, _sv0t127.tag);
  sv0_box_store(_sv0t128, 1, _sv0t127.p0);
  sv0_box_store(_sv0t128, 2, _sv0t127.p1);
  sv0_box_store(_sv0t128, 3, _sv0t127.p2);
  sv0_box_store(_sv0t128, 4, _sv0t127.p3);
  sv0_vec_push(el9, _sv0t128);
  Instr _sv0t129;
  Expr _sv0t130;
  Value _sv0t131;
  _sv0t131.tag = 2;
  _sv0t131.p0 = 1;
  int _sv0t132 = sv0_box_alloc(3);
  sv0_box_store(_sv0t132, 0, _sv0t131.tag);
  sv0_box_store(_sv0t132, 1, _sv0t131.p0);
  sv0_box_store(_sv0t132, 2, _sv0t131.p1);
  _sv0t130.tag = 0;
  _sv0t130.p0 = _sv0t132;
  int _sv0t133 = sv0_box_alloc(4);
  sv0_box_store(_sv0t133, 0, _sv0t130.tag);
  sv0_box_store(_sv0t133, 1, _sv0t130.p0);
  sv0_box_store(_sv0t133, 2, _sv0t130.p1);
  sv0_box_store(_sv0t133, 3, _sv0t130.p2);
  _sv0t129.tag = 6;
  _sv0t129.p0 = _sv0t133;
  _sv0t129.p1 = th9;
  _sv0t129.p2 = el9;
  int _sv0t134 = sv0_box_alloc(5);
  sv0_box_store(_sv0t134, 0, _sv0t129.tag);
  sv0_box_store(_sv0t134, 1, _sv0t129.p0);
  sv0_box_store(_sv0t134, 2, _sv0t129.p1);
  sv0_box_store(_sv0t134, 3, _sv0t129.p2);
  sv0_box_store(_sv0t134, 4, _sv0t129.p3);
  sv0_vec_push(ins9, _sv0t134);
  int _sv0t135 = local_count_and_env(pn9, pc9, ins9, en9, eb9, ew9, efs9, sn, sfc, enn, enw);
  int lc9 = _sv0t135;
  if ((lc9 != 2)) {
    return 27;
  } else {
  }
  int _sv0t136 = sv0_vec_get(en9, 0);
  if ((_sv0t136 != 90)) {
    return 28;
  } else {
  }
  int _sv0t137 = sv0_vec_get(en9, 1);
  if ((_sv0t137 != 91)) {
    return 29;
  } else {
  }
  int _sv0t138 = sv0_vec_new();
  int en10 = _sv0t138;
  int _sv0t139 = sv0_vec_new();
  int eb10 = _sv0t139;
  int _sv0t140 = sv0_vec_new();
  int ew10 = _sv0t140;
  int _sv0t141 = sv0_vec_new();
  int efs10 = _sv0t141;
  int _sv0t142 = sv0_vec_new();
  int pn10 = _sv0t142;
  int _sv0t143 = sv0_vec_new();
  int pc10 = _sv0t143;
  int _sv0t144 = sv0_vec_new();
  int ins10 = _sv0t144;
  int _sv0t145 = sv0_vec_new();
  int wbod = _sv0t145;
  Instr _sv0t146;
  _sv0t146.tag = 1;
  _sv0t146.p0 = 95;
  int _sv0t147 = sv0_box_alloc(5);
  sv0_box_store(_sv0t147, 0, _sv0t146.tag);
  sv0_box_store(_sv0t147, 1, _sv0t146.p0);
  sv0_box_store(_sv0t147, 2, _sv0t146.p1);
  sv0_box_store(_sv0t147, 3, _sv0t146.p2);
  sv0_box_store(_sv0t147, 4, _sv0t146.p3);
  sv0_vec_push(wbod, _sv0t147);
  Instr _sv0t148;
  Expr _sv0t149;
  Value _sv0t150;
  _sv0t150.tag = 2;
  _sv0t150.p0 = 1;
  int _sv0t151 = sv0_box_alloc(3);
  sv0_box_store(_sv0t151, 0, _sv0t150.tag);
  sv0_box_store(_sv0t151, 1, _sv0t150.p0);
  sv0_box_store(_sv0t151, 2, _sv0t150.p1);
  _sv0t149.tag = 0;
  _sv0t149.p0 = _sv0t151;
  int _sv0t152 = sv0_box_alloc(4);
  sv0_box_store(_sv0t152, 0, _sv0t149.tag);
  sv0_box_store(_sv0t152, 1, _sv0t149.p0);
  sv0_box_store(_sv0t152, 2, _sv0t149.p1);
  sv0_box_store(_sv0t152, 3, _sv0t149.p2);
  _sv0t148.tag = 7;
  _sv0t148.p0 = _sv0t152;
  _sv0t148.p1 = wbod;
  int _sv0t153 = sv0_box_alloc(5);
  sv0_box_store(_sv0t153, 0, _sv0t148.tag);
  sv0_box_store(_sv0t153, 1, _sv0t148.p0);
  sv0_box_store(_sv0t153, 2, _sv0t148.p1);
  sv0_box_store(_sv0t153, 3, _sv0t148.p2);
  sv0_box_store(_sv0t153, 4, _sv0t148.p3);
  sv0_vec_push(ins10, _sv0t153);
  int _sv0t154 = local_count_and_env(pn10, pc10, ins10, en10, eb10, ew10, efs10, sn, sfc, enn, enw);
  int lc10 = _sv0t154;
  if ((lc10 != 1)) {
    return 30;
  } else {
  }
  int _sv0t155 = sv0_vec_get(en10, 0);
  if ((_sv0t155 != 95)) {
    return 31;
  } else {
  }
  int _sv0t156 = sv0_vec_new();
  int en11 = _sv0t156;
  int _sv0t157 = sv0_vec_new();
  int eb11 = _sv0t157;
  int _sv0t158 = sv0_vec_new();
  int ew11 = _sv0t158;
  int _sv0t159 = sv0_vec_new();
  int efs11 = _sv0t159;
  int _sv0t160 = sv0_vec_new();
  int pn11 = _sv0t160;
  int _sv0t161 = sv0_vec_new();
  int pc11 = _sv0t161;
  sv0_vec_push(pn11, 200);
  sv0_vec_push(pc11, 777);
  int _sv0t162 = sv0_vec_new();
  int ins11 = _sv0t162;
  Instr _sv0t163;
  _sv0t163.tag = 1;
  _sv0t163.p0 = 201;
  int _sv0t164 = sv0_box_alloc(5);
  sv0_box_store(_sv0t164, 0, _sv0t163.tag);
  sv0_box_store(_sv0t164, 1, _sv0t163.p0);
  sv0_box_store(_sv0t164, 2, _sv0t163.p1);
  sv0_box_store(_sv0t164, 3, _sv0t163.p2);
  sv0_box_store(_sv0t164, 4, _sv0t163.p3);
  sv0_vec_push(ins11, _sv0t164);
  Instr _sv0t165;
  _sv0t165.tag = 1;
  _sv0t165.p0 = 202;
  int _sv0t166 = sv0_box_alloc(5);
  sv0_box_store(_sv0t166, 0, _sv0t165.tag);
  sv0_box_store(_sv0t166, 1, _sv0t165.p0);
  sv0_box_store(_sv0t166, 2, _sv0t165.p1);
  sv0_box_store(_sv0t166, 3, _sv0t165.p2);
  sv0_box_store(_sv0t166, 4, _sv0t165.p3);
  sv0_vec_push(ins11, _sv0t166);
  int _sv0t167 = local_count_and_env(pn11, pc11, ins11, en11, eb11, ew11, efs11, sn, sfc, enn, enw);
  int lc11 = _sv0t167;
  if ((lc11 != 3)) {
    return 32;
  } else {
  }
  int _sv0t168 = sv0_vec_len(en11);
  if ((_sv0t168 != 3)) {
    return 33;
  } else {
  }
  int _sv0t169 = sv0_vec_get(eb11, 0);
  if ((_sv0t169 != 0)) {
    return 34;
  } else {
  }
  int _sv0t170 = sv0_vec_get(eb11, 1);
  if ((_sv0t170 != 1)) {
    return 35;
  } else {
  }
  int _sv0t171 = sv0_vec_get(eb11, 2);
  if ((_sv0t171 != 2)) {
    return 36;
  } else {
  }
  int _sv0t172 = sv0_vec_new();
  int en12 = _sv0t172;
  int _sv0t173 = sv0_vec_new();
  int eb12 = _sv0t173;
  int _sv0t174 = sv0_vec_new();
  int ew12 = _sv0t174;
  int _sv0t175 = sv0_vec_new();
  int efs12 = _sv0t175;
  int _sv0t176 = sv0_vec_new();
  int pn12 = _sv0t176;
  int _sv0t177 = sv0_vec_new();
  int pc12 = _sv0t177;
  int _sv0t178 = sv0_vec_new();
  int ins12 = _sv0t178;
  Instr _sv0t179;
  _sv0t179.tag = 0;
  int _sv0t180 = sv0_box_alloc(5);
  sv0_box_store(_sv0t180, 0, _sv0t179.tag);
  sv0_box_store(_sv0t180, 1, _sv0t179.p0);
  sv0_box_store(_sv0t180, 2, _sv0t179.p1);
  sv0_box_store(_sv0t180, 3, _sv0t179.p2);
  sv0_box_store(_sv0t180, 4, _sv0t179.p3);
  sv0_vec_push(ins12, _sv0t180);
  Instr _sv0t181;
  _sv0t181.tag = 9;
  int _sv0t182 = sv0_box_alloc(5);
  sv0_box_store(_sv0t182, 0, _sv0t181.tag);
  sv0_box_store(_sv0t182, 1, _sv0t181.p0);
  sv0_box_store(_sv0t182, 2, _sv0t181.p1);
  sv0_box_store(_sv0t182, 3, _sv0t181.p2);
  sv0_box_store(_sv0t182, 4, _sv0t181.p3);
  sv0_vec_push(ins12, _sv0t182);
  Instr _sv0t183;
  _sv0t183.tag = 10;
  int _sv0t184 = sv0_box_alloc(5);
  sv0_box_store(_sv0t184, 0, _sv0t183.tag);
  sv0_box_store(_sv0t184, 1, _sv0t183.p0);
  sv0_box_store(_sv0t184, 2, _sv0t183.p1);
  sv0_box_store(_sv0t184, 3, _sv0t183.p2);
  sv0_box_store(_sv0t184, 4, _sv0t183.p3);
  sv0_vec_push(ins12, _sv0t184);
  Instr _sv0t185;
  _sv0t185.tag = 14;
  int _sv0t186 = sv0_box_alloc(5);
  sv0_box_store(_sv0t186, 0, _sv0t185.tag);
  sv0_box_store(_sv0t186, 1, _sv0t185.p0);
  sv0_box_store(_sv0t186, 2, _sv0t185.p1);
  sv0_box_store(_sv0t186, 3, _sv0t185.p2);
  sv0_box_store(_sv0t186, 4, _sv0t185.p3);
  sv0_vec_push(ins12, _sv0t186);
  int _sv0t187 = local_count_and_env(pn12, pc12, ins12, en12, eb12, ew12, efs12, sn, sfc, enn, enw);
  int lc12 = _sv0t187;
  if ((lc12 != 0)) {
    return 37;
  } else {
  }
  int _sv0t188 = sv0_vec_len(en12);
  if ((_sv0t188 != 0)) {
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
  int _sv0t8 = emit_program(it, in2, ifc, ivp, bl, bpn, bpc, bi, source, starts, ends, pool, ft);
  return _sv0t8;
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
  return 0;
}


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
static int enum_field_count(int width);
static int enum_field_names(int width);
static int binop_to_insn(const char* op);
static int unop_to_insn(const char* op);
static int insn_size(int opc);
static int enc_len(int opcode);
static int enc_lens(int opcodes);
static int loop_exit_sentinel(void);
static int loop_continue_sentinel(void);
static int is_loop_exit_sentinel(int v);
static int is_loop_continue_sentinel(int v);
static int pool_new(void);
static int pool_add(int pool, int s);
static int pool_size(int pool);
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
static int struct_layouts_build(int item_tags, int item_names, int item_field_counts, int out_names, int out_field_counts);
static int enum_layouts_build(int item_tags, int item_names, int item_variant_counts, int item_variant_max_payload, int out_names, int out_widths);
static int binop_handle_to_insn(int op_h, const char* source, int starts, int ends);
static int unop_handle_to_insn(int op_h, const char* source, int starts, int ends);
static int emit_value(Value v, int env_names, int env_bases, int env_widths, int pool, int out);
static int emit_expr(Expr e, int env_names, int env_bases, int env_widths, int pool, const char* source, int starts, int ends, int out);
static int vec_append(int dst, int src);
static int emit_ret_width(Value v, int env_names, int env_bases, int env_widths);
static int emit_instr(Instr ins, int env_names, int env_bases, int env_widths, int pool, const char* source, int starts, int ends, int out);
static int emit_instrs(int instrs, int env_names, int env_bases, int env_widths, int pool, const char* source, int starts, int ends, int out);
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
  int _sv0t15 = (0 - 1);
  return _sv0t15;
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
      int _sv0t12 = (0 - 1);
      return _sv0t12;
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
          int _sv0t11 = pool_add(pool, s);
          int idx = _sv0t11;
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
              int _sv0t5 = lookup_slot(env_names, env_bases, env_widths, x);
              int si = _sv0t5;
              if ((si < 0)) {
                int _sv0t6 = (0 - 1);
                return _sv0t6;
              } else {
              }
              int _sv0t7 = slot_base(env_bases, si);
              int base = _sv0t7;
              int _sv0t8 = slot_width(env_widths, si);
              int width = _sv0t8;
              int k = 0;
              while ((k < width)) {
                sv0_vec_push(out, 96);
                int _sv0t9 = (base + k);
                sv0_vec_push(out, _sv0t9);
                k = (k + 1);
              }
              int _sv0t10 = (width * 2);
              return _sv0t10;
              _sv0t0 = 0;
            } else {
              if ((v.tag == 6)) {
                int bv = v.p0;
                int f = v.p1;
                int _sv0t4 = (0 - 1);
                return _sv0t4;
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
  int _sv0t13 = (0 - 1);
  return _sv0t13;
}

static int emit_expr(Expr e, int env_names, int env_bases, int env_widths, int pool, const char* source, int starts, int ends, int out) {
  int _sv0t0;
  if ((e.tag == 0)) {
    int bv = e.p0;
    Value _sv0t32;
    int _sv0t33 = sv0_box_load(bv, 0);
    _sv0t32.tag = _sv0t33;
    int _sv0t34 = sv0_box_load(bv, 1);
    _sv0t32.p0 = _sv0t34;
    int _sv0t35 = sv0_box_load(bv, 2);
    _sv0t32.p1 = _sv0t35;
    Value v;
    v.tag = (_sv0t32).tag;
    v.p0 = (_sv0t32).p0;
    v.p1 = (_sv0t32).p1;
    int _sv0t36 = emit_value(v, env_names, env_bases, env_widths, pool, out);
    return _sv0t36;
    _sv0t0 = 0;
  } else {
    if ((e.tag == 3)) {
      int x = e.p0;
      int _sv0t26 = lookup_slot(env_names, env_bases, env_widths, x);
      int si = _sv0t26;
      if ((si < 0)) {
        int _sv0t27 = (0 - 1);
        return _sv0t27;
      } else {
      }
      int _sv0t28 = slot_base(env_bases, si);
      int base = _sv0t28;
      int _sv0t29 = slot_width(env_widths, si);
      int width = _sv0t29;
      int k = 0;
      while ((k < width)) {
        sv0_vec_push(out, 96);
        int _sv0t30 = (base + k);
        sv0_vec_push(out, _sv0t30);
        k = (k + 1);
      }
      int _sv0t31 = (width * 2);
      return _sv0t31;
      _sv0t0 = 0;
    } else {
      if ((e.tag == 1)) {
        int op_h = e.p0;
        int bv1 = e.p1;
        int bv2 = e.p2;
        Value _sv0t11;
        int _sv0t12 = sv0_box_load(bv1, 0);
        _sv0t11.tag = _sv0t12;
        int _sv0t13 = sv0_box_load(bv1, 1);
        _sv0t11.p0 = _sv0t13;
        int _sv0t14 = sv0_box_load(bv1, 2);
        _sv0t11.p1 = _sv0t14;
        Value v1;
        v1.tag = (_sv0t11).tag;
        v1.p0 = (_sv0t11).p0;
        v1.p1 = (_sv0t11).p1;
        Value _sv0t15;
        int _sv0t16 = sv0_box_load(bv2, 0);
        _sv0t15.tag = _sv0t16;
        int _sv0t17 = sv0_box_load(bv2, 1);
        _sv0t15.p0 = _sv0t17;
        int _sv0t18 = sv0_box_load(bv2, 2);
        _sv0t15.p1 = _sv0t18;
        Value v2;
        v2.tag = (_sv0t15).tag;
        v2.p0 = (_sv0t15).p0;
        v2.p1 = (_sv0t15).p1;
        int _sv0t19 = emit_value(v1, env_names, env_bases, env_widths, pool, out);
        int n1 = _sv0t19;
        if ((n1 < 0)) {
          int _sv0t20 = (0 - 1);
          return _sv0t20;
        } else {
        }
        int _sv0t21 = emit_value(v2, env_names, env_bases, env_widths, pool, out);
        int n2 = _sv0t21;
        if ((n2 < 0)) {
          int _sv0t22 = (0 - 1);
          return _sv0t22;
        } else {
        }
        int _sv0t23 = binop_handle_to_insn(op_h, source, starts, ends);
        int opc = _sv0t23;
        sv0_vec_push(out, opc);
        int _sv0t24 = (n1 + n2);
        int _sv0t25 = (_sv0t24 + 1);
        return _sv0t25;
        _sv0t0 = 0;
      } else {
        if ((e.tag == 2)) {
          int op_h = e.p0;
          int bv = e.p1;
          Value _sv0t3;
          int _sv0t4 = sv0_box_load(bv, 0);
          _sv0t3.tag = _sv0t4;
          int _sv0t5 = sv0_box_load(bv, 1);
          _sv0t3.p0 = _sv0t5;
          int _sv0t6 = sv0_box_load(bv, 2);
          _sv0t3.p1 = _sv0t6;
          Value v;
          v.tag = (_sv0t3).tag;
          v.p0 = (_sv0t3).p0;
          v.p1 = (_sv0t3).p1;
          int _sv0t7 = emit_value(v, env_names, env_bases, env_widths, pool, out);
          int n = _sv0t7;
          if ((n < 0)) {
            int _sv0t8 = (0 - 1);
            return _sv0t8;
          } else {
          }
          int _sv0t9 = unop_handle_to_insn(op_h, source, starts, ends);
          int opc = _sv0t9;
          if ((opc >= 0)) {
            sv0_vec_push(out, opc);
            int _sv0t10 = (n + 1);
            return _sv0t10;
          } else {
          }
          return n;
          _sv0t0 = 0;
        } else {
          if ((e.tag == 4)) {
            int bv = e.p0;
            int f = e.p1;
            int _sv0t2 = (0 - 1);
            return _sv0t2;
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
  int _sv0t37 = (0 - 1);
  return _sv0t37;
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

static int emit_instr(Instr ins, int env_names, int env_bases, int env_widths, int pool, const char* source, int starts, int ends, int out) {
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
          Expr _sv0t34;
          int _sv0t35 = sv0_box_load(be, 0);
          _sv0t34.tag = _sv0t35;
          int _sv0t36 = sv0_box_load(be, 1);
          _sv0t34.p0 = _sv0t36;
          int _sv0t37 = sv0_box_load(be, 2);
          _sv0t34.p1 = _sv0t37;
          int _sv0t38 = sv0_box_load(be, 3);
          _sv0t34.p2 = _sv0t38;
          Expr e;
          e.tag = (_sv0t34).tag;
          e.p0 = (_sv0t34).p0;
          e.p1 = (_sv0t34).p1;
          e.p2 = (_sv0t34).p2;
          int _sv0t39 = emit_expr(e, env_names, env_bases, env_widths, pool, source, starts, ends, out);
          int n = _sv0t39;
          if ((n < 0)) {
            int _sv0t40 = (0 - 1);
            return _sv0t40;
          } else {
          }
          int _sv0t41 = lookup_slot(env_names, env_bases, env_widths, x);
          int si = _sv0t41;
          if ((si < 0)) {
            int _sv0t42 = (0 - 1);
            return _sv0t42;
          } else {
          }
          int _sv0t43 = slot_base(env_bases, si);
          int base = _sv0t43;
          int _sv0t44 = slot_width(env_widths, si);
          int width = _sv0t44;
          int k = 0;
          while ((k < width)) {
            sv0_vec_push(out, 97);
            int _sv0t45 = (base + width);
            int _sv0t46 = (_sv0t45 - 1);
            int _sv0t47 = (_sv0t46 - k);
            sv0_vec_push(out, _sv0t47);
            k = (k + 1);
          }
          int _sv0t48 = (width * 2);
          int _sv0t49 = (n + _sv0t48);
          return _sv0t49;
          _sv0t0 = 0;
        } else {
          if ((ins.tag == 4)) {
            int x = ins.p0;
            int be = ins.p1;
            Expr _sv0t18;
            int _sv0t19 = sv0_box_load(be, 0);
            _sv0t18.tag = _sv0t19;
            int _sv0t20 = sv0_box_load(be, 1);
            _sv0t18.p0 = _sv0t20;
            int _sv0t21 = sv0_box_load(be, 2);
            _sv0t18.p1 = _sv0t21;
            int _sv0t22 = sv0_box_load(be, 3);
            _sv0t18.p2 = _sv0t22;
            Expr e;
            e.tag = (_sv0t18).tag;
            e.p0 = (_sv0t18).p0;
            e.p1 = (_sv0t18).p1;
            e.p2 = (_sv0t18).p2;
            int _sv0t23 = emit_expr(e, env_names, env_bases, env_widths, pool, source, starts, ends, out);
            int n = _sv0t23;
            if ((n < 0)) {
              int _sv0t24 = (0 - 1);
              return _sv0t24;
            } else {
            }
            int _sv0t25 = lookup_slot(env_names, env_bases, env_widths, x);
            int si = _sv0t25;
            if ((si < 0)) {
              int _sv0t26 = (0 - 1);
              return _sv0t26;
            } else {
            }
            int _sv0t27 = slot_base(env_bases, si);
            int base = _sv0t27;
            int _sv0t28 = slot_width(env_widths, si);
            int width = _sv0t28;
            int k = 0;
            while ((k < width)) {
              sv0_vec_push(out, 97);
              int _sv0t29 = (base + width);
              int _sv0t30 = (_sv0t29 - 1);
              int _sv0t31 = (_sv0t30 - k);
              sv0_vec_push(out, _sv0t31);
              k = (k + 1);
            }
            int _sv0t32 = (width * 2);
            int _sv0t33 = (n + _sv0t32);
            return _sv0t33;
            _sv0t0 = 0;
          } else {
            if ((ins.tag == 5)) {
              int bh = ins.p0;
              int fh = ins.p1;
              int be = ins.p2;
              int _sv0t17 = (0 - 1);
              return _sv0t17;
              _sv0t0 = 0;
            } else {
              if ((ins.tag == 6)) {
                int be = ins.p0;
                int th = ins.p1;
                int el = ins.p2;
                int _sv0t16 = (0 - 1);
                return _sv0t16;
                _sv0t0 = 0;
              } else {
                if ((ins.tag == 7)) {
                  int be = ins.p0;
                  int body = ins.p1;
                  int _sv0t15 = (0 - 1);
                  return _sv0t15;
                  _sv0t0 = 0;
                } else {
                  if ((ins.tag == 8)) {
                    int body = ins.p0;
                    int _sv0t14 = (0 - 1);
                    return _sv0t14;
                    _sv0t0 = 0;
                  } else {
                    if ((ins.tag == 9)) {
                      int _sv0t13 = (0 - 1);
                      return _sv0t13;
                      _sv0t0 = 0;
                    } else {
                      if ((ins.tag == 10)) {
                        int _sv0t12 = (0 - 1);
                        return _sv0t12;
                        _sv0t0 = 0;
                      } else {
                        if ((ins.tag == 11)) {
                          int dst = ins.p0;
                          int fn_h = ins.p1;
                          int args = ins.p2;
                          int rt = ins.p3;
                          int _sv0t11 = (0 - 1);
                          return _sv0t11;
                          _sv0t0 = 0;
                        } else {
                          if ((ins.tag == 12)) {
                            int be = ins.p0;
                            int fn_h = ins.p1;
                            int _sv0t10 = (0 - 1);
                            return _sv0t10;
                            _sv0t0 = 0;
                          } else {
                            if ((ins.tag == 13)) {
                              int be = ins.p0;
                              int fn_h = ins.p1;
                              int _sv0t9 = (0 - 1);
                              return _sv0t9;
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
  int _sv0t50 = (0 - 1);
  return _sv0t50;
}

static int emit_instrs(int instrs, int env_names, int env_bases, int env_widths, int pool, const char* source, int starts, int ends, int out) {
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
    int _sv0t8 = emit_instr(ins, env_names, env_bases, env_widths, pool, source, starts, ends, out);
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
  int _sv0t4 = builtin_id("unknown");
  int _sv0t5 = (0 - 1);
  if ((_sv0t4 != _sv0t5)) {
    return 5;
  } else {
  }
  int _sv0t6 = is_builtin("sv0_println");
  if ((_sv0t6 != 1)) {
    return 6;
  } else {
  }
  int _sv0t7 = is_builtin("foo");
  if ((_sv0t7 != 0)) {
    return 7;
  } else {
  }
  int _sv0t8 = builtin_has_result("sv0_println");
  if ((_sv0t8 != 0)) {
    return 8;
  } else {
  }
  int _sv0t9 = builtin_has_result("sv0_string_len");
  if ((_sv0t9 != 1)) {
    return 9;
  } else {
  }
  int _sv0t10 = builtin_arg_count("sv0_println");
  if ((_sv0t10 != 1)) {
    return 10;
  } else {
  }
  int _sv0t11 = builtin_arg_count("sv0_string_substr");
  if ((_sv0t11 != 3)) {
    return 11;
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
  int pool = _sv0t3;
  const char* source;
  source = "+-!";
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
  sv0_vec_push(en, 50);
  sv0_vec_push(eb, 0);
  sv0_vec_push(ew, 1);
  int _sv0t6 = sv0_vec_new();
  int out1 = _sv0t6;
  Instr _sv0t7;
  _sv0t7.tag = 0;
  int _sv0t8 = emit_instr(_sv0t7, en, eb, ew, pool, source, starts, ends, out1);
  int r1 = _sv0t8;
  if ((r1 != 0)) {
    return 1;
  } else {
  }
  int _sv0t9 = sv0_vec_len(out1);
  if ((_sv0t9 != 0)) {
    return 2;
  } else {
  }
  int _sv0t10 = sv0_vec_new();
  int out2 = _sv0t10;
  Instr _sv0t11;
  _sv0t11.tag = 1;
  _sv0t11.p0 = 99;
  int _sv0t12 = emit_instr(_sv0t11, en, eb, ew, pool, source, starts, ends, out2);
  int r2 = _sv0t12;
  if ((r2 != 0)) {
    return 3;
  } else {
  }
  int _sv0t13 = sv0_vec_len(out2);
  if ((_sv0t13 != 0)) {
    return 4;
  } else {
  }
  int _sv0t14 = sv0_vec_new();
  int out3 = _sv0t14;
  Instr _sv0t15;
  _sv0t15.tag = 2;
  _sv0t15.p0 = 1;
  _sv0t15.p1 = 2;
  int _sv0t16 = emit_instr(_sv0t15, en, eb, ew, pool, source, starts, ends, out3);
  int r3 = _sv0t16;
  if ((r3 != 0)) {
    return 5;
  } else {
  }
  int _sv0t17 = sv0_vec_new();
  int out4 = _sv0t17;
  Expr asgn_e;
  Value _sv0t18;
  _sv0t18.tag = 0;
  _sv0t18.p0 = 42;
  int _sv0t19 = sv0_box_alloc(3);
  sv0_box_store(_sv0t19, 0, _sv0t18.tag);
  sv0_box_store(_sv0t19, 1, _sv0t18.p0);
  sv0_box_store(_sv0t19, 2, _sv0t18.p1);
  asgn_e.tag = 0;
  asgn_e.p0 = _sv0t19;
  Instr _sv0t20;
  int _sv0t21 = sv0_box_alloc(4);
  sv0_box_store(_sv0t21, 0, asgn_e.tag);
  sv0_box_store(_sv0t21, 1, asgn_e.p0);
  sv0_box_store(_sv0t21, 2, asgn_e.p1);
  sv0_box_store(_sv0t21, 3, asgn_e.p2);
  _sv0t20.tag = 3;
  _sv0t20.p0 = 50;
  _sv0t20.p1 = _sv0t21;
  int _sv0t22 = emit_instr(_sv0t20, en, eb, ew, pool, source, starts, ends, out4);
  int r4 = _sv0t22;
  if ((r4 != 4)) {
    return 6;
  } else {
  }
  int _sv0t23 = sv0_vec_get(out4, 0);
  if ((_sv0t23 != 4)) {
    return 7;
  } else {
  }
  int _sv0t24 = sv0_vec_get(out4, 1);
  if ((_sv0t24 != 42)) {
    return 8;
  } else {
  }
  int _sv0t25 = sv0_vec_get(out4, 2);
  if ((_sv0t25 != 97)) {
    return 9;
  } else {
  }
  int _sv0t26 = sv0_vec_get(out4, 3);
  if ((_sv0t26 != 0)) {
    return 10;
  } else {
  }
  int _sv0t27 = sv0_vec_new();
  int out5 = _sv0t27;
  Expr store_e;
  Value _sv0t28;
  _sv0t28.tag = 2;
  _sv0t28.p0 = 1;
  int _sv0t29 = sv0_box_alloc(3);
  sv0_box_store(_sv0t29, 0, _sv0t28.tag);
  sv0_box_store(_sv0t29, 1, _sv0t28.p0);
  sv0_box_store(_sv0t29, 2, _sv0t28.p1);
  store_e.tag = 0;
  store_e.p0 = _sv0t29;
  Instr _sv0t30;
  int _sv0t31 = sv0_box_alloc(4);
  sv0_box_store(_sv0t31, 0, store_e.tag);
  sv0_box_store(_sv0t31, 1, store_e.p0);
  sv0_box_store(_sv0t31, 2, store_e.p1);
  sv0_box_store(_sv0t31, 3, store_e.p2);
  _sv0t30.tag = 4;
  _sv0t30.p0 = 50;
  _sv0t30.p1 = _sv0t31;
  int _sv0t32 = emit_instr(_sv0t30, en, eb, ew, pool, source, starts, ends, out5);
  int r5 = _sv0t32;
  if ((r5 != 4)) {
    return 11;
  } else {
  }
  int _sv0t33 = sv0_vec_get(out5, 0);
  if ((_sv0t33 != 7)) {
    return 12;
  } else {
  }
  int _sv0t34 = sv0_vec_get(out5, 1);
  if ((_sv0t34 != 1)) {
    return 13;
  } else {
  }
  int _sv0t35 = sv0_vec_get(out5, 2);
  if ((_sv0t35 != 97)) {
    return 14;
  } else {
  }
  int _sv0t36 = sv0_vec_get(out5, 3);
  if ((_sv0t36 != 0)) {
    return 15;
  } else {
  }
  int _sv0t37 = sv0_vec_new();
  int out6 = _sv0t37;
  Instr _sv0t38;
  _sv0t38.tag = 14;
  int _sv0t39 = emit_instr(_sv0t38, en, eb, ew, pool, source, starts, ends, out6);
  int r6 = _sv0t39;
  if ((r6 != 2)) {
    return 16;
  } else {
  }
  int _sv0t40 = sv0_vec_get(out6, 0);
  if ((_sv0t40 != 118)) {
    return 17;
  } else {
  }
  int _sv0t41 = sv0_vec_get(out6, 1);
  if ((_sv0t41 != 0)) {
    return 18;
  } else {
  }
  int _sv0t42 = sv0_vec_new();
  int out7 = _sv0t42;
  Instr _sv0t43;
  Value _sv0t44;
  _sv0t44.tag = 0;
  _sv0t44.p0 = 7;
  int _sv0t45 = sv0_box_alloc(3);
  sv0_box_store(_sv0t45, 0, _sv0t44.tag);
  sv0_box_store(_sv0t45, 1, _sv0t44.p0);
  sv0_box_store(_sv0t45, 2, _sv0t44.p1);
  _sv0t43.tag = 15;
  _sv0t43.p0 = _sv0t45;
  int _sv0t46 = emit_instr(_sv0t43, en, eb, ew, pool, source, starts, ends, out7);
  int r7 = _sv0t46;
  if ((r7 != 4)) {
    return 19;
  } else {
  }
  int _sv0t47 = sv0_vec_get(out7, 0);
  if ((_sv0t47 != 4)) {
    return 20;
  } else {
  }
  int _sv0t48 = sv0_vec_get(out7, 1);
  if ((_sv0t48 != 7)) {
    return 21;
  } else {
  }
  int _sv0t49 = sv0_vec_get(out7, 2);
  if ((_sv0t49 != 118)) {
    return 22;
  } else {
  }
  int _sv0t50 = sv0_vec_get(out7, 3);
  if ((_sv0t50 != 1)) {
    return 23;
  } else {
  }
  int _sv0t51 = sv0_vec_new();
  int out8 = _sv0t51;
  Instr _sv0t52;
  Value _sv0t53;
  _sv0t53.tag = 5;
  _sv0t53.p0 = 50;
  int _sv0t54 = sv0_box_alloc(3);
  sv0_box_store(_sv0t54, 0, _sv0t53.tag);
  sv0_box_store(_sv0t54, 1, _sv0t53.p0);
  sv0_box_store(_sv0t54, 2, _sv0t53.p1);
  _sv0t52.tag = 15;
  _sv0t52.p0 = _sv0t54;
  int _sv0t55 = emit_instr(_sv0t52, en, eb, ew, pool, source, starts, ends, out8);
  int r8 = _sv0t55;
  if ((r8 != 4)) {
    return 24;
  } else {
  }
  int _sv0t56 = sv0_vec_get(out8, 0);
  if ((_sv0t56 != 96)) {
    return 25;
  } else {
  }
  int _sv0t57 = sv0_vec_get(out8, 1);
  if ((_sv0t57 != 0)) {
    return 26;
  } else {
  }
  int _sv0t58 = sv0_vec_get(out8, 2);
  if ((_sv0t58 != 118)) {
    return 27;
  } else {
  }
  int _sv0t59 = sv0_vec_get(out8, 3);
  if ((_sv0t59 != 1)) {
    return 28;
  } else {
  }
  sv0_vec_push(en, 60);
  sv0_vec_push(eb, 1);
  sv0_vec_push(ew, 3);
  int _sv0t60 = sv0_vec_new();
  int out9 = _sv0t60;
  Expr asgn2_e;
  asgn2_e.tag = 3;
  asgn2_e.p0 = 60;
  Instr _sv0t61;
  int _sv0t62 = sv0_box_alloc(4);
  sv0_box_store(_sv0t62, 0, asgn2_e.tag);
  sv0_box_store(_sv0t62, 1, asgn2_e.p0);
  sv0_box_store(_sv0t62, 2, asgn2_e.p1);
  sv0_box_store(_sv0t62, 3, asgn2_e.p2);
  _sv0t61.tag = 3;
  _sv0t61.p0 = 60;
  _sv0t61.p1 = _sv0t62;
  int _sv0t63 = emit_instr(_sv0t61, en, eb, ew, pool, source, starts, ends, out9);
  int r9 = _sv0t63;
  if ((r9 != 12)) {
    return 29;
  } else {
  }
  int _sv0t64 = sv0_vec_get(out9, 0);
  if ((_sv0t64 != 96)) {
    return 30;
  } else {
  }
  int _sv0t65 = sv0_vec_get(out9, 1);
  if ((_sv0t65 != 1)) {
    return 31;
  } else {
  }
  int _sv0t66 = sv0_vec_get(out9, 2);
  if ((_sv0t66 != 96)) {
    return 32;
  } else {
  }
  int _sv0t67 = sv0_vec_get(out9, 3);
  if ((_sv0t67 != 2)) {
    return 33;
  } else {
  }
  int _sv0t68 = sv0_vec_get(out9, 4);
  if ((_sv0t68 != 96)) {
    return 34;
  } else {
  }
  int _sv0t69 = sv0_vec_get(out9, 5);
  if ((_sv0t69 != 3)) {
    return 35;
  } else {
  }
  int _sv0t70 = sv0_vec_get(out9, 6);
  if ((_sv0t70 != 97)) {
    return 36;
  } else {
  }
  int _sv0t71 = sv0_vec_get(out9, 7);
  if ((_sv0t71 != 3)) {
    return 37;
  } else {
  }
  int _sv0t72 = sv0_vec_get(out9, 8);
  if ((_sv0t72 != 97)) {
    return 38;
  } else {
  }
  int _sv0t73 = sv0_vec_get(out9, 9);
  if ((_sv0t73 != 2)) {
    return 39;
  } else {
  }
  int _sv0t74 = sv0_vec_get(out9, 10);
  if ((_sv0t74 != 97)) {
    return 40;
  } else {
  }
  int _sv0t75 = sv0_vec_get(out9, 11);
  if ((_sv0t75 != 1)) {
    return 41;
  } else {
  }
  int _sv0t76 = sv0_vec_new();
  int out10 = _sv0t76;
  Instr _sv0t77;
  Value _sv0t78;
  _sv0t78.tag = 5;
  _sv0t78.p0 = 60;
  int _sv0t79 = sv0_box_alloc(3);
  sv0_box_store(_sv0t79, 0, _sv0t78.tag);
  sv0_box_store(_sv0t79, 1, _sv0t78.p0);
  sv0_box_store(_sv0t79, 2, _sv0t78.p1);
  _sv0t77.tag = 15;
  _sv0t77.p0 = _sv0t79;
  int _sv0t80 = emit_instr(_sv0t77, en, eb, ew, pool, source, starts, ends, out10);
  int r10 = _sv0t80;
  if ((r10 != 8)) {
    return 42;
  } else {
  }
  int _sv0t81 = sv0_vec_get(out10, 6);
  if ((_sv0t81 != 118)) {
    return 43;
  } else {
  }
  int _sv0t82 = sv0_vec_get(out10, 7);
  if ((_sv0t82 != 3)) {
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
  int pool = _sv0t3;
  const char* source;
  source = "+";
  int _sv0t4 = sv0_vec_new();
  int starts = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int ends = _sv0t5;
  sv0_vec_push(starts, 0);
  sv0_vec_push(ends, 1);
  int _sv0t6 = sv0_vec_new();
  int empty = _sv0t6;
  int _sv0t7 = sv0_vec_new();
  int out1 = _sv0t7;
  int _sv0t8 = emit_instrs(empty, en, eb, ew, pool, source, starts, ends, out1);
  int r1 = _sv0t8;
  if ((r1 != 0)) {
    return 1;
  } else {
  }
  int _sv0t9 = sv0_vec_len(out1);
  if ((_sv0t9 != 0)) {
    return 2;
  } else {
  }
  int _sv0t10 = sv0_vec_new();
  int instrs = _sv0t10;
  Instr _sv0t11;
  _sv0t11.tag = 0;
  int _sv0t12 = sv0_box_alloc(5);
  sv0_box_store(_sv0t12, 0, _sv0t11.tag);
  sv0_box_store(_sv0t12, 1, _sv0t11.p0);
  sv0_box_store(_sv0t12, 2, _sv0t11.p1);
  sv0_box_store(_sv0t12, 3, _sv0t11.p2);
  sv0_box_store(_sv0t12, 4, _sv0t11.p3);
  sv0_vec_push(instrs, _sv0t12);
  Instr _sv0t13;
  _sv0t13.tag = 14;
  int _sv0t14 = sv0_box_alloc(5);
  sv0_box_store(_sv0t14, 0, _sv0t13.tag);
  sv0_box_store(_sv0t14, 1, _sv0t13.p0);
  sv0_box_store(_sv0t14, 2, _sv0t13.p1);
  sv0_box_store(_sv0t14, 3, _sv0t13.p2);
  sv0_box_store(_sv0t14, 4, _sv0t13.p3);
  sv0_vec_push(instrs, _sv0t14);
  int _sv0t15 = sv0_vec_new();
  int out2 = _sv0t15;
  int _sv0t16 = emit_instrs(instrs, en, eb, ew, pool, source, starts, ends, out2);
  int r2 = _sv0t16;
  if ((r2 != 2)) {
    return 3;
  } else {
  }
  int _sv0t17 = sv0_vec_len(out2);
  if ((_sv0t17 != 2)) {
    return 4;
  } else {
  }
  int _sv0t18 = sv0_vec_get(out2, 0);
  if ((_sv0t18 != 118)) {
    return 5;
  } else {
  }
  int _sv0t19 = sv0_vec_get(out2, 1);
  if ((_sv0t19 != 0)) {
    return 6;
  } else {
  }
  sv0_vec_push(en, 70);
  sv0_vec_push(eb, 0);
  sv0_vec_push(ew, 1);
  int _sv0t20 = sv0_vec_new();
  int instrs2 = _sv0t20;
  Expr a_e;
  Value _sv0t21;
  _sv0t21.tag = 0;
  _sv0t21.p0 = 99;
  int _sv0t22 = sv0_box_alloc(3);
  sv0_box_store(_sv0t22, 0, _sv0t21.tag);
  sv0_box_store(_sv0t22, 1, _sv0t21.p0);
  sv0_box_store(_sv0t22, 2, _sv0t21.p1);
  a_e.tag = 0;
  a_e.p0 = _sv0t22;
  Instr _sv0t23;
  int _sv0t24 = sv0_box_alloc(4);
  sv0_box_store(_sv0t24, 0, a_e.tag);
  sv0_box_store(_sv0t24, 1, a_e.p0);
  sv0_box_store(_sv0t24, 2, a_e.p1);
  sv0_box_store(_sv0t24, 3, a_e.p2);
  _sv0t23.tag = 3;
  _sv0t23.p0 = 70;
  _sv0t23.p1 = _sv0t24;
  int _sv0t25 = sv0_box_alloc(5);
  sv0_box_store(_sv0t25, 0, _sv0t23.tag);
  sv0_box_store(_sv0t25, 1, _sv0t23.p0);
  sv0_box_store(_sv0t25, 2, _sv0t23.p1);
  sv0_box_store(_sv0t25, 3, _sv0t23.p2);
  sv0_box_store(_sv0t25, 4, _sv0t23.p3);
  sv0_vec_push(instrs2, _sv0t25);
  Instr _sv0t26;
  Value _sv0t27;
  _sv0t27.tag = 5;
  _sv0t27.p0 = 70;
  int _sv0t28 = sv0_box_alloc(3);
  sv0_box_store(_sv0t28, 0, _sv0t27.tag);
  sv0_box_store(_sv0t28, 1, _sv0t27.p0);
  sv0_box_store(_sv0t28, 2, _sv0t27.p1);
  _sv0t26.tag = 15;
  _sv0t26.p0 = _sv0t28;
  int _sv0t29 = sv0_box_alloc(5);
  sv0_box_store(_sv0t29, 0, _sv0t26.tag);
  sv0_box_store(_sv0t29, 1, _sv0t26.p0);
  sv0_box_store(_sv0t29, 2, _sv0t26.p1);
  sv0_box_store(_sv0t29, 3, _sv0t26.p2);
  sv0_box_store(_sv0t29, 4, _sv0t26.p3);
  sv0_vec_push(instrs2, _sv0t29);
  int _sv0t30 = sv0_vec_new();
  int out3 = _sv0t30;
  int _sv0t31 = emit_instrs(instrs2, en, eb, ew, pool, source, starts, ends, out3);
  int r3 = _sv0t31;
  if ((r3 != 8)) {
    return 7;
  } else {
  }
  int _sv0t32 = sv0_vec_get(out3, 0);
  if ((_sv0t32 != 4)) {
    return 8;
  } else {
  }
  int _sv0t33 = sv0_vec_get(out3, 1);
  if ((_sv0t33 != 99)) {
    return 9;
  } else {
  }
  int _sv0t34 = sv0_vec_get(out3, 2);
  if ((_sv0t34 != 97)) {
    return 10;
  } else {
  }
  int _sv0t35 = sv0_vec_get(out3, 3);
  if ((_sv0t35 != 0)) {
    return 11;
  } else {
  }
  int _sv0t36 = sv0_vec_get(out3, 4);
  if ((_sv0t36 != 96)) {
    return 12;
  } else {
  }
  int _sv0t37 = sv0_vec_get(out3, 5);
  if ((_sv0t37 != 0)) {
    return 13;
  } else {
  }
  int _sv0t38 = sv0_vec_get(out3, 6);
  if ((_sv0t38 != 118)) {
    return 14;
  } else {
  }
  int _sv0t39 = sv0_vec_get(out3, 7);
  if ((_sv0t39 != 1)) {
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
  return 0;
}


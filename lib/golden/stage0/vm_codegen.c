#include "sv0_runtime.h"

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
static int store_slot_offset(int base, int width, int k);
static int lookup_slot(int env_names, int env_bases, int env_widths, int name);
static int slot_base(int env_bases, int idx);
static int slot_width(int env_widths, int idx);
static int alloc_local(int env_names, int env_bases, int env_widths, int name, int width, int slot);
static int replace_loop_exit_jump(int instrs, int back_offset);
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
  return 0;
}


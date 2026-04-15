#include "sv0_runtime.h"

static int magic_byte_0(void);
static int magic_byte_1(void);
static int magic_byte_2(void);
static int magic_byte_3(void);
static int format_version(void);
static int OP_HALT(void);
static int OP_POP(void);
static int OP_DUP(void);
static int OP_PUSH_UNIT(void);
static int OP_PUSH_I32(void);
static int OP_PUSH_I64(void);
static int OP_PUSH_F64(void);
static int OP_PUSH_BOOL(void);
static int OP_PUSH_STRING(void);
static int OP_ADD_I32(void);
static int OP_SUB_I32(void);
static int OP_MUL_I32(void);
static int OP_DIV_I32(void);
static int OP_MOD_I32(void);
static int OP_NEG_I32(void);
static int OP_ADD_I64(void);
static int OP_SUB_I64(void);
static int OP_MUL_I64(void);
static int OP_DIV_I64(void);
static int OP_MOD_I64(void);
static int OP_NEG_I64(void);
static int OP_ADD_F64(void);
static int OP_SUB_F64(void);
static int OP_MUL_F64(void);
static int OP_DIV_F64(void);
static int OP_NEG_F64(void);
static int OP_EQ(void);
static int OP_NEQ(void);
static int OP_LT(void);
static int OP_GT(void);
static int OP_LTE(void);
static int OP_GTE(void);
static int OP_AND(void);
static int OP_OR(void);
static int OP_NOT(void);
static int OP_BIT_AND(void);
static int OP_BIT_OR(void);
static int OP_BIT_XOR(void);
static int OP_BIT_NOT(void);
static int OP_SHL(void);
static int OP_SHR(void);
static int OP_LOAD_LOCAL(void);
static int OP_STORE_LOCAL(void);
static int OP_JUMP(void);
static int OP_JUMP_IF(void);
static int OP_JUMP_IF_NOT(void);
static int OP_CALL(void);
static int OP_RETURN(void);
static int OP_CALL_BUILTIN(void);
static int OP_RETURN_SLOTS(void);
static int OP_ALLOC_STRUCT(void);
static int OP_GET_FIELD(void);
static int OP_SET_FIELD(void);
static int OP_ALLOC_ARRAY(void);
static int OP_GET_INDEX(void);
static int OP_SET_INDEX(void);
static int OP_CONSTRUCT_VARIANT(void);
static int OP_GET_TAG(void);
static int OP_GET_VARIANT_FIELD(void);
static int OP_CONTRACT_CHECK(void);
static int OP_CAST(void);
static int insn_encoded_size(int opc);
static int is_simple_opcode(int opc);
static int opcode_has_i32_payload(int opc);
static int opcode_has_u32_payload(int opc);
static int is_arithmetic_opcode(int opc);
static int is_compare_opcode(int opc);
static int is_logic_opcode(int opc);
static int is_bitwise_opcode(int opc);
static int is_control_flow_opcode(int opc);
static int is_struct_opcode(int opc);
static int is_variant_opcode(int opc);
static int le_byte0(int val);
static int le_byte1(int val);
static int le_byte2(int val);
static int le_byte3(int val);
static int from_le_u16(int b0, int b1);
static int from_le_u32(int b0, int b1, int b2, int b3);
static int encode_u16_le(int val, int out);
static int encode_u32_le(int val, int out);
static int encode_i32_le(int val, int out);
static int encode_header(int version, int out);
static int decode_u16_at(int buf, int pos);
static int decode_u32_at(int buf, int pos);
static int verify_magic(int buf);
static int file_header_size(void);
static int func_entry_size(void);
static int string_entry_overhead(void);
static int file_header_magic_len(void);
static int file_header_version_len(void);
static int string_section_size(int count, int total_bytes);
static int func_table_size(int count);
static int encode_strings(int strings, const char* source, int starts, int ends, int out);
static int encode_string_literals(int strs, int out);
static int decode_strings(int buf, int pos, int out_starts, int out_lens);
static int test_magic(void);
static int test_opcodes(void);
static int test_insn_sizes(void);
static int test_opcode_classify(void);
static int test_le_bytes(void);
static int test_file_layout(void);
static int test_encode_le(void);
static int test_encode_header(void);
static int test_decode_at(void);
static int test_verify_magic(void);
static int test_encode_decode_strings(void);
static int test_encode_string_literals(void);

static int magic_byte_0(void) {
  return 83;
}

static int magic_byte_1(void) {
  return 86;
}

static int magic_byte_2(void) {
  return 48;
}

static int magic_byte_3(void) {
  return 66;
}

static int format_version(void) {
  return 1;
}

static int OP_HALT(void) {
  return 0;
}

static int OP_POP(void) {
  return 1;
}

static int OP_DUP(void) {
  return 2;
}

static int OP_PUSH_UNIT(void) {
  return 3;
}

static int OP_PUSH_I32(void) {
  return 4;
}

static int OP_PUSH_I64(void) {
  return 5;
}

static int OP_PUSH_F64(void) {
  return 6;
}

static int OP_PUSH_BOOL(void) {
  return 7;
}

static int OP_PUSH_STRING(void) {
  return 8;
}

static int OP_ADD_I32(void) {
  return 16;
}

static int OP_SUB_I32(void) {
  return 17;
}

static int OP_MUL_I32(void) {
  return 18;
}

static int OP_DIV_I32(void) {
  return 19;
}

static int OP_MOD_I32(void) {
  return 20;
}

static int OP_NEG_I32(void) {
  return 21;
}

static int OP_ADD_I64(void) {
  return 32;
}

static int OP_SUB_I64(void) {
  return 33;
}

static int OP_MUL_I64(void) {
  return 34;
}

static int OP_DIV_I64(void) {
  return 35;
}

static int OP_MOD_I64(void) {
  return 36;
}

static int OP_NEG_I64(void) {
  return 37;
}

static int OP_ADD_F64(void) {
  return 48;
}

static int OP_SUB_F64(void) {
  return 49;
}

static int OP_MUL_F64(void) {
  return 50;
}

static int OP_DIV_F64(void) {
  return 51;
}

static int OP_NEG_F64(void) {
  return 52;
}

static int OP_EQ(void) {
  return 64;
}

static int OP_NEQ(void) {
  return 65;
}

static int OP_LT(void) {
  return 66;
}

static int OP_GT(void) {
  return 67;
}

static int OP_LTE(void) {
  return 68;
}

static int OP_GTE(void) {
  return 69;
}

static int OP_AND(void) {
  return 80;
}

static int OP_OR(void) {
  return 81;
}

static int OP_NOT(void) {
  return 82;
}

static int OP_BIT_AND(void) {
  return 88;
}

static int OP_BIT_OR(void) {
  return 89;
}

static int OP_BIT_XOR(void) {
  return 90;
}

static int OP_BIT_NOT(void) {
  return 91;
}

static int OP_SHL(void) {
  return 92;
}

static int OP_SHR(void) {
  return 93;
}

static int OP_LOAD_LOCAL(void) {
  return 96;
}

static int OP_STORE_LOCAL(void) {
  return 97;
}

static int OP_JUMP(void) {
  return 112;
}

static int OP_JUMP_IF(void) {
  return 113;
}

static int OP_JUMP_IF_NOT(void) {
  return 114;
}

static int OP_CALL(void) {
  return 115;
}

static int OP_RETURN(void) {
  return 116;
}

static int OP_CALL_BUILTIN(void) {
  return 117;
}

static int OP_RETURN_SLOTS(void) {
  return 118;
}

static int OP_ALLOC_STRUCT(void) {
  return 128;
}

static int OP_GET_FIELD(void) {
  return 129;
}

static int OP_SET_FIELD(void) {
  return 130;
}

static int OP_ALLOC_ARRAY(void) {
  return 131;
}

static int OP_GET_INDEX(void) {
  return 132;
}

static int OP_SET_INDEX(void) {
  return 133;
}

static int OP_CONSTRUCT_VARIANT(void) {
  return 144;
}

static int OP_GET_TAG(void) {
  return 145;
}

static int OP_GET_VARIANT_FIELD(void) {
  return 146;
}

static int OP_CONTRACT_CHECK(void) {
  return 160;
}

static int OP_CAST(void) {
  return 161;
}

static int insn_encoded_size(int opc) {
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

static int is_simple_opcode(int opc) {
  int _sv0t0 = insn_encoded_size(opc);
  int _sv0t1 = (_sv0t0 == 1);
  return _sv0t1;
}

static int opcode_has_i32_payload(int opc) {
  if ((opc == 4)) {
    return 1;
  } else {
  }
  if ((opc == 112)) {
    return 1;
  } else {
  }
  if ((opc == 113)) {
    return 1;
  } else {
  }
  if ((opc == 114)) {
    return 1;
  } else {
  }
  return 0;
}

static int opcode_has_u32_payload(int opc) {
  if ((opc == 8)) {
    return 1;
  } else {
  }
  if ((opc == 96)) {
    return 1;
  } else {
  }
  if ((opc == 97)) {
    return 1;
  } else {
  }
  if ((opc == 117)) {
    return 1;
  } else {
  }
  if ((opc == 128)) {
    return 1;
  } else {
  }
  if ((opc == 129)) {
    return 1;
  } else {
  }
  if ((opc == 130)) {
    return 1;
  } else {
  }
  if ((opc == 131)) {
    return 1;
  } else {
  }
  if ((opc == 146)) {
    return 1;
  } else {
  }
  if ((opc == 160)) {
    return 1;
  } else {
  }
  return 0;
}

static int is_arithmetic_opcode(int opc) {
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
  return 0;
}

static int is_compare_opcode(int opc) {
  if ((opc >= 64)) {
    if ((opc <= 69)) {
      return 1;
    } else {
    }
  } else {
  }
  return 0;
}

static int is_logic_opcode(int opc) {
  if ((opc >= 80)) {
    if ((opc <= 82)) {
      return 1;
    } else {
    }
  } else {
  }
  return 0;
}

static int is_bitwise_opcode(int opc) {
  if ((opc >= 88)) {
    if ((opc <= 93)) {
      return 1;
    } else {
    }
  } else {
  }
  return 0;
}

static int is_control_flow_opcode(int opc) {
  if ((opc >= 112)) {
    if ((opc <= 118)) {
      return 1;
    } else {
    }
  } else {
  }
  return 0;
}

static int is_struct_opcode(int opc) {
  if ((opc >= 128)) {
    if ((opc <= 133)) {
      return 1;
    } else {
    }
  } else {
  }
  return 0;
}

static int is_variant_opcode(int opc) {
  if ((opc >= 144)) {
    if ((opc <= 146)) {
      return 1;
    } else {
    }
  } else {
  }
  return 0;
}

static int le_byte0(int val) {
  int v = val;
  if ((v < 0)) {
    int _sv0t0 = (256 * 256);
    int _sv0t1 = (_sv0t0 * 256);
    int _sv0t2 = (_sv0t1 * 256);
    v = (v + _sv0t2);
  } else {
  }
  int _sv0t3 = (v / 256);
  int _sv0t4 = (_sv0t3 * 256);
  int _sv0t5 = (v - _sv0t4);
  return _sv0t5;
}

static int le_byte1(int val) {
  int v = val;
  if ((v < 0)) {
    int _sv0t0 = (256 * 256);
    int _sv0t1 = (_sv0t0 * 256);
    int _sv0t2 = (_sv0t1 * 256);
    v = (v + _sv0t2);
  } else {
  }
  int shifted = (v / 256);
  int _sv0t3 = (shifted / 256);
  int _sv0t4 = (_sv0t3 * 256);
  int _sv0t5 = (shifted - _sv0t4);
  return _sv0t5;
}

static int le_byte2(int val) {
  int v = val;
  if ((v < 0)) {
    int _sv0t0 = (256 * 256);
    int _sv0t1 = (_sv0t0 * 256);
    int _sv0t2 = (_sv0t1 * 256);
    v = (v + _sv0t2);
  } else {
  }
  int _sv0t3 = (256 * 256);
  int shifted = (v / _sv0t3);
  int _sv0t4 = (shifted / 256);
  int _sv0t5 = (_sv0t4 * 256);
  int _sv0t6 = (shifted - _sv0t5);
  return _sv0t6;
}

static int le_byte3(int val) {
  int v = val;
  if ((v < 0)) {
    int _sv0t0 = (256 * 256);
    int _sv0t1 = (_sv0t0 * 256);
    int _sv0t2 = (_sv0t1 * 256);
    v = (v + _sv0t2);
  } else {
  }
  int _sv0t3 = (256 * 256);
  int _sv0t4 = (_sv0t3 * 256);
  int _sv0t5 = (v / _sv0t4);
  return _sv0t5;
}

static int from_le_u16(int b0, int b1) {
  int _sv0t0 = (b1 * 256);
  int _sv0t1 = (b0 + _sv0t0);
  return _sv0t1;
}

static int from_le_u32(int b0, int b1, int b2, int b3) {
  int _sv0t0 = (b1 * 256);
  int _sv0t1 = (b0 + _sv0t0);
  int _sv0t2 = (b2 * 256);
  int _sv0t3 = (_sv0t2 * 256);
  int _sv0t4 = (_sv0t1 + _sv0t3);
  int _sv0t5 = (b3 * 256);
  int _sv0t6 = (_sv0t5 * 256);
  int _sv0t7 = (_sv0t6 * 256);
  int _sv0t8 = (_sv0t4 + _sv0t7);
  return _sv0t8;
}

static int encode_u16_le(int val, int out) {
  int _sv0t0 = le_byte0(val);
  sv0_vec_push(out, _sv0t0);
  int _sv0t1 = le_byte1(val);
  sv0_vec_push(out, _sv0t1);
  return 2;
}

static int encode_u32_le(int val, int out) {
  int _sv0t0 = le_byte0(val);
  sv0_vec_push(out, _sv0t0);
  int _sv0t1 = le_byte1(val);
  sv0_vec_push(out, _sv0t1);
  int _sv0t2 = le_byte2(val);
  sv0_vec_push(out, _sv0t2);
  int _sv0t3 = le_byte3(val);
  sv0_vec_push(out, _sv0t3);
  return 4;
}

static int encode_i32_le(int val, int out) {
  int _sv0t0 = le_byte0(val);
  sv0_vec_push(out, _sv0t0);
  int _sv0t1 = le_byte1(val);
  sv0_vec_push(out, _sv0t1);
  int _sv0t2 = le_byte2(val);
  sv0_vec_push(out, _sv0t2);
  int _sv0t3 = le_byte3(val);
  sv0_vec_push(out, _sv0t3);
  return 4;
}

static int encode_header(int version, int out) {
  int _sv0t0 = magic_byte_0();
  sv0_vec_push(out, _sv0t0);
  int _sv0t1 = magic_byte_1();
  sv0_vec_push(out, _sv0t1);
  int _sv0t2 = magic_byte_2();
  sv0_vec_push(out, _sv0t2);
  int _sv0t3 = magic_byte_3();
  sv0_vec_push(out, _sv0t3);
  int _sv0t4 = encode_u16_le(version, out);
  return 6;
}

static int decode_u16_at(int buf, int pos) {
  int _sv0t0 = sv0_vec_get(buf, pos);
  int b0 = _sv0t0;
  int _sv0t1 = (pos + 1);
  int _sv0t2 = sv0_vec_get(buf, _sv0t1);
  int b1 = _sv0t2;
  int _sv0t3 = from_le_u16(b0, b1);
  return _sv0t3;
}

static int decode_u32_at(int buf, int pos) {
  int _sv0t0 = sv0_vec_get(buf, pos);
  int b0 = _sv0t0;
  int _sv0t1 = (pos + 1);
  int _sv0t2 = sv0_vec_get(buf, _sv0t1);
  int b1 = _sv0t2;
  int _sv0t3 = (pos + 2);
  int _sv0t4 = sv0_vec_get(buf, _sv0t3);
  int b2 = _sv0t4;
  int _sv0t5 = (pos + 3);
  int _sv0t6 = sv0_vec_get(buf, _sv0t5);
  int b3 = _sv0t6;
  int _sv0t7 = from_le_u32(b0, b1, b2, b3);
  return _sv0t7;
}

static int verify_magic(int buf) {
  int _sv0t0 = sv0_vec_len(buf);
  if ((_sv0t0 < 6)) {
    return 0;
  } else {
  }
  int _sv0t1 = sv0_vec_get(buf, 0);
  int _sv0t2 = magic_byte_0();
  if ((_sv0t1 != _sv0t2)) {
    return 0;
  } else {
  }
  int _sv0t3 = sv0_vec_get(buf, 1);
  int _sv0t4 = magic_byte_1();
  if ((_sv0t3 != _sv0t4)) {
    return 0;
  } else {
  }
  int _sv0t5 = sv0_vec_get(buf, 2);
  int _sv0t6 = magic_byte_2();
  if ((_sv0t5 != _sv0t6)) {
    return 0;
  } else {
  }
  int _sv0t7 = sv0_vec_get(buf, 3);
  int _sv0t8 = magic_byte_3();
  if ((_sv0t7 != _sv0t8)) {
    return 0;
  } else {
  }
  return 1;
}

static int file_header_size(void) {
  return 6;
}

static int func_entry_size(void) {
  return 20;
}

static int string_entry_overhead(void) {
  return 4;
}

static int file_header_magic_len(void) {
  return 4;
}

static int file_header_version_len(void) {
  return 2;
}

static int string_section_size(int count, int total_bytes) {
  int _sv0t0 = (count * 4);
  int _sv0t1 = (4 + _sv0t0);
  int _sv0t2 = (_sv0t1 + total_bytes);
  return _sv0t2;
}

static int func_table_size(int count) {
  int _sv0t0 = (count * 20);
  int _sv0t1 = (4 + _sv0t0);
  return _sv0t1;
}

static int encode_strings(int strings, const char* source, int starts, int ends, int out) {
  int _sv0t0 = sv0_vec_len(strings);
  int count = _sv0t0;
  int _sv0t1 = encode_u32_le(count, out);
  int total = 4;
  int i = 0;
  while ((i < count)) {
    int _sv0t2 = sv0_vec_get(strings, i);
    int idx = _sv0t2;
    int _sv0t3 = sv0_vec_get(starts, idx);
    int s = _sv0t3;
    int _sv0t4 = sv0_vec_get(ends, idx);
    int e = _sv0t4;
    int slen = (e - s);
    int _sv0t5 = encode_u32_le(slen, out);
    total = (total + 4);
    int j = 0;
    while ((j < slen)) {
      int _sv0t6 = (s + j);
      int _sv0t7 = sv0_string_char_at(source, _sv0t6);
      sv0_vec_push(out, _sv0t7);
      j = (j + 1);
    }
    total = (total + slen);
    i = (i + 1);
  }
  return total;
}

static int encode_string_literals(int strs, int out) {
  int _sv0t0 = sv0_vec_len(strs);
  int count = _sv0t0;
  int _sv0t1 = encode_u32_le(count, out);
  int total = 4;
  int i = 0;
  while ((i < count)) {
    int _sv0t2 = sv0_vec_get(strs, i);
    int handle = _sv0t2;
    int _sv0t3 = encode_u32_le(handle, out);
    total = (total + 4);
    i = (i + 1);
  }
  return total;
}

static int decode_strings(int buf, int pos, int out_starts, int out_lens) {
  int _sv0t0 = decode_u32_at(buf, pos);
  int count = _sv0t0;
  int p = (pos + 4);
  int i = 0;
  while ((i < count)) {
    int _sv0t1 = decode_u32_at(buf, p);
    int slen = _sv0t1;
    p = (p + 4);
    sv0_vec_push(out_starts, p);
    sv0_vec_push(out_lens, slen);
    p = (p + slen);
    i = (i + 1);
  }
  return p;
}

static int test_magic(void) {
  int _sv0t0 = magic_byte_0();
  if ((_sv0t0 != 83)) {
    return 1;
  } else {
  }
  int _sv0t1 = magic_byte_1();
  if ((_sv0t1 != 86)) {
    return 2;
  } else {
  }
  int _sv0t2 = magic_byte_2();
  if ((_sv0t2 != 48)) {
    return 3;
  } else {
  }
  int _sv0t3 = magic_byte_3();
  if ((_sv0t3 != 66)) {
    return 4;
  } else {
  }
  int _sv0t4 = format_version();
  if ((_sv0t4 != 1)) {
    return 5;
  } else {
  }
  return 0;
}

static int test_opcodes(void) {
  int _sv0t0 = OP_HALT();
  if ((_sv0t0 != 0)) {
    return 1;
  } else {
  }
  int _sv0t1 = OP_PUSH_I32();
  if ((_sv0t1 != 4)) {
    return 2;
  } else {
  }
  int _sv0t2 = OP_ADD_I32();
  if ((_sv0t2 != 16)) {
    return 3;
  } else {
  }
  int _sv0t3 = OP_ADD_I64();
  if ((_sv0t3 != 32)) {
    return 4;
  } else {
  }
  int _sv0t4 = OP_ADD_F64();
  if ((_sv0t4 != 48)) {
    return 5;
  } else {
  }
  int _sv0t5 = OP_EQ();
  if ((_sv0t5 != 64)) {
    return 6;
  } else {
  }
  int _sv0t6 = OP_AND();
  if ((_sv0t6 != 80)) {
    return 7;
  } else {
  }
  int _sv0t7 = OP_BIT_AND();
  if ((_sv0t7 != 88)) {
    return 8;
  } else {
  }
  int _sv0t8 = OP_LOAD_LOCAL();
  if ((_sv0t8 != 96)) {
    return 9;
  } else {
  }
  int _sv0t9 = OP_JUMP();
  if ((_sv0t9 != 112)) {
    return 10;
  } else {
  }
  int _sv0t10 = OP_CALL();
  if ((_sv0t10 != 115)) {
    return 11;
  } else {
  }
  int _sv0t11 = OP_RETURN();
  if ((_sv0t11 != 116)) {
    return 12;
  } else {
  }
  int _sv0t12 = OP_ALLOC_STRUCT();
  if ((_sv0t12 != 128)) {
    return 13;
  } else {
  }
  int _sv0t13 = OP_CONSTRUCT_VARIANT();
  if ((_sv0t13 != 144)) {
    return 14;
  } else {
  }
  int _sv0t14 = OP_CONTRACT_CHECK();
  if ((_sv0t14 != 160)) {
    return 15;
  } else {
  }
  int _sv0t15 = OP_CAST();
  if ((_sv0t15 != 161)) {
    return 16;
  } else {
  }
  int _sv0t16 = OP_RETURN_SLOTS();
  if ((_sv0t16 != 118)) {
    return 17;
  } else {
  }
  return 0;
}

static int test_insn_sizes(void) {
  int _sv0t0 = insn_encoded_size(0);
  if ((_sv0t0 != 1)) {
    return 1;
  } else {
  }
  int _sv0t1 = insn_encoded_size(1);
  if ((_sv0t1 != 1)) {
    return 2;
  } else {
  }
  int _sv0t2 = insn_encoded_size(4);
  if ((_sv0t2 != 5)) {
    return 3;
  } else {
  }
  int _sv0t3 = insn_encoded_size(5);
  if ((_sv0t3 != 9)) {
    return 4;
  } else {
  }
  int _sv0t4 = insn_encoded_size(6);
  if ((_sv0t4 != 9)) {
    return 5;
  } else {
  }
  int _sv0t5 = insn_encoded_size(7);
  if ((_sv0t5 != 2)) {
    return 6;
  } else {
  }
  int _sv0t6 = insn_encoded_size(8);
  if ((_sv0t6 != 5)) {
    return 7;
  } else {
  }
  int _sv0t7 = insn_encoded_size(16);
  if ((_sv0t7 != 1)) {
    return 8;
  } else {
  }
  int _sv0t8 = insn_encoded_size(64);
  if ((_sv0t8 != 1)) {
    return 9;
  } else {
  }
  int _sv0t9 = insn_encoded_size(96);
  if ((_sv0t9 != 5)) {
    return 10;
  } else {
  }
  int _sv0t10 = insn_encoded_size(112);
  if ((_sv0t10 != 5)) {
    return 11;
  } else {
  }
  int _sv0t11 = insn_encoded_size(115);
  if ((_sv0t11 != 9)) {
    return 12;
  } else {
  }
  int _sv0t12 = insn_encoded_size(116);
  if ((_sv0t12 != 1)) {
    return 13;
  } else {
  }
  int _sv0t13 = insn_encoded_size(118);
  if ((_sv0t13 != 2)) {
    return 14;
  } else {
  }
  int _sv0t14 = insn_encoded_size(144);
  if ((_sv0t14 != 13)) {
    return 15;
  } else {
  }
  int _sv0t15 = insn_encoded_size(161);
  if ((_sv0t15 != 5)) {
    return 16;
  } else {
  }
  return 0;
}

static int test_opcode_classify(void) {
  int _sv0t0 = is_simple_opcode(0);
  if ((_sv0t0 != 1)) {
    return 1;
  } else {
  }
  int _sv0t1 = is_simple_opcode(4);
  if ((_sv0t1 != 0)) {
    return 2;
  } else {
  }
  int _sv0t2 = opcode_has_i32_payload(4);
  if ((_sv0t2 != 1)) {
    return 3;
  } else {
  }
  int _sv0t3 = opcode_has_i32_payload(112);
  if ((_sv0t3 != 1)) {
    return 4;
  } else {
  }
  int _sv0t4 = opcode_has_i32_payload(8);
  if ((_sv0t4 != 0)) {
    return 5;
  } else {
  }
  int _sv0t5 = opcode_has_u32_payload(8);
  if ((_sv0t5 != 1)) {
    return 6;
  } else {
  }
  int _sv0t6 = opcode_has_u32_payload(96);
  if ((_sv0t6 != 1)) {
    return 7;
  } else {
  }
  int _sv0t7 = is_arithmetic_opcode(16);
  if ((_sv0t7 != 1)) {
    return 8;
  } else {
  }
  int _sv0t8 = is_arithmetic_opcode(52);
  if ((_sv0t8 != 1)) {
    return 9;
  } else {
  }
  int _sv0t9 = is_arithmetic_opcode(64);
  if ((_sv0t9 != 0)) {
    return 10;
  } else {
  }
  int _sv0t10 = is_compare_opcode(64);
  if ((_sv0t10 != 1)) {
    return 11;
  } else {
  }
  int _sv0t11 = is_compare_opcode(69);
  if ((_sv0t11 != 1)) {
    return 12;
  } else {
  }
  int _sv0t12 = is_logic_opcode(80);
  if ((_sv0t12 != 1)) {
    return 13;
  } else {
  }
  int _sv0t13 = is_bitwise_opcode(88);
  if ((_sv0t13 != 1)) {
    return 14;
  } else {
  }
  int _sv0t14 = is_bitwise_opcode(93);
  if ((_sv0t14 != 1)) {
    return 15;
  } else {
  }
  int _sv0t15 = is_control_flow_opcode(112);
  if ((_sv0t15 != 1)) {
    return 16;
  } else {
  }
  int _sv0t16 = is_control_flow_opcode(118);
  if ((_sv0t16 != 1)) {
    return 17;
  } else {
  }
  int _sv0t17 = is_struct_opcode(128);
  if ((_sv0t17 != 1)) {
    return 18;
  } else {
  }
  int _sv0t18 = is_variant_opcode(144);
  if ((_sv0t18 != 1)) {
    return 19;
  } else {
  }
  return 0;
}

static int test_le_bytes(void) {
  int _sv0t0 = le_byte0(258);
  if ((_sv0t0 != 2)) {
    return 1;
  } else {
  }
  int _sv0t1 = le_byte1(258);
  if ((_sv0t1 != 1)) {
    return 2;
  } else {
  }
  int _sv0t2 = le_byte2(258);
  if ((_sv0t2 != 0)) {
    return 3;
  } else {
  }
  int _sv0t3 = le_byte3(258);
  if ((_sv0t3 != 0)) {
    return 4;
  } else {
  }
  int _sv0t4 = le_byte0(65536);
  if ((_sv0t4 != 0)) {
    return 5;
  } else {
  }
  int _sv0t5 = le_byte2(65536);
  if ((_sv0t5 != 1)) {
    return 6;
  } else {
  }
  int _sv0t6 = from_le_u16(2, 1);
  if ((_sv0t6 != 258)) {
    return 7;
  } else {
  }
  int _sv0t7 = from_le_u32(1, 0, 1, 0);
  if ((_sv0t7 != 65537)) {
    return 8;
  } else {
  }
  int _sv0t8 = from_le_u32(0, 0, 0, 1);
  int _sv0t9 = (256 * 256);
  int _sv0t10 = (_sv0t9 * 256);
  if ((_sv0t8 != _sv0t10)) {
    return 9;
  } else {
  }
  return 0;
}

static int test_file_layout(void) {
  int _sv0t0 = file_header_size();
  if ((_sv0t0 != 6)) {
    return 1;
  } else {
  }
  int _sv0t1 = func_entry_size();
  if ((_sv0t1 != 20)) {
    return 2;
  } else {
  }
  int _sv0t2 = string_entry_overhead();
  if ((_sv0t2 != 4)) {
    return 3;
  } else {
  }
  int _sv0t3 = string_section_size(2, 10);
  if ((_sv0t3 != 22)) {
    return 4;
  } else {
  }
  int _sv0t4 = func_table_size(3);
  if ((_sv0t4 != 64)) {
    return 5;
  } else {
  }
  return 0;
}

static int test_encode_le(void) {
  int _sv0t0 = sv0_vec_new();
  int buf = _sv0t0;
  int _sv0t1 = encode_u16_le(258, buf);
  int _sv0t2 = sv0_vec_len(buf);
  if ((_sv0t2 != 2)) {
    return 1;
  } else {
  }
  int _sv0t3 = sv0_vec_get(buf, 0);
  if ((_sv0t3 != 2)) {
    return 2;
  } else {
  }
  int _sv0t4 = sv0_vec_get(buf, 1);
  if ((_sv0t4 != 1)) {
    return 3;
  } else {
  }
  int _sv0t5 = sv0_vec_new();
  int buf2 = _sv0t5;
  int _sv0t6 = encode_u32_le(65537, buf2);
  int _sv0t7 = sv0_vec_len(buf2);
  if ((_sv0t7 != 4)) {
    return 4;
  } else {
  }
  int _sv0t8 = sv0_vec_get(buf2, 0);
  if ((_sv0t8 != 1)) {
    return 5;
  } else {
  }
  int _sv0t9 = sv0_vec_get(buf2, 1);
  if ((_sv0t9 != 0)) {
    return 6;
  } else {
  }
  int _sv0t10 = sv0_vec_get(buf2, 2);
  if ((_sv0t10 != 1)) {
    return 7;
  } else {
  }
  int _sv0t11 = sv0_vec_get(buf2, 3);
  if ((_sv0t11 != 0)) {
    return 8;
  } else {
  }
  int _sv0t12 = sv0_vec_new();
  int buf3 = _sv0t12;
  int _sv0t13 = encode_i32_le(256, buf3);
  int _sv0t14 = sv0_vec_len(buf3);
  if ((_sv0t14 != 4)) {
    return 9;
  } else {
  }
  int _sv0t15 = sv0_vec_get(buf3, 0);
  if ((_sv0t15 != 0)) {
    return 10;
  } else {
  }
  int _sv0t16 = sv0_vec_get(buf3, 1);
  if ((_sv0t16 != 1)) {
    return 11;
  } else {
  }
  return 0;
}

static int test_encode_header(void) {
  int _sv0t0 = sv0_vec_new();
  int buf = _sv0t0;
  int _sv0t1 = encode_header(1, buf);
  int sz = _sv0t1;
  if ((sz != 6)) {
    return 1;
  } else {
  }
  int _sv0t2 = sv0_vec_len(buf);
  if ((_sv0t2 != 6)) {
    return 2;
  } else {
  }
  int _sv0t3 = sv0_vec_get(buf, 0);
  if ((_sv0t3 != 83)) {
    return 3;
  } else {
  }
  int _sv0t4 = sv0_vec_get(buf, 1);
  if ((_sv0t4 != 86)) {
    return 4;
  } else {
  }
  int _sv0t5 = sv0_vec_get(buf, 2);
  if ((_sv0t5 != 48)) {
    return 5;
  } else {
  }
  int _sv0t6 = sv0_vec_get(buf, 3);
  if ((_sv0t6 != 66)) {
    return 6;
  } else {
  }
  int _sv0t7 = sv0_vec_get(buf, 4);
  if ((_sv0t7 != 1)) {
    return 7;
  } else {
  }
  int _sv0t8 = sv0_vec_get(buf, 5);
  if ((_sv0t8 != 0)) {
    return 8;
  } else {
  }
  return 0;
}

static int test_decode_at(void) {
  int _sv0t0 = sv0_vec_new();
  int buf = _sv0t0;
  int _sv0t1 = encode_u32_le(65537, buf);
  int _sv0t2 = decode_u32_at(buf, 0);
  int decoded = _sv0t2;
  if ((decoded != 65537)) {
    return 1;
  } else {
  }
  int _sv0t3 = sv0_vec_new();
  int buf2 = _sv0t3;
  int _sv0t4 = encode_u16_le(258, buf2);
  int _sv0t5 = decode_u16_at(buf2, 0);
  int d2 = _sv0t5;
  if ((d2 != 258)) {
    return 2;
  } else {
  }
  return 0;
}

static int test_verify_magic(void) {
  int _sv0t0 = sv0_vec_new();
  int buf = _sv0t0;
  int _sv0t1 = encode_header(1, buf);
  int _sv0t2 = verify_magic(buf);
  if ((_sv0t2 != 1)) {
    return 1;
  } else {
  }
  int _sv0t3 = sv0_vec_new();
  int bad = _sv0t3;
  sv0_vec_push(bad, 0);
  sv0_vec_push(bad, 0);
  sv0_vec_push(bad, 0);
  sv0_vec_push(bad, 0);
  sv0_vec_push(bad, 0);
  sv0_vec_push(bad, 0);
  int _sv0t4 = verify_magic(bad);
  if ((_sv0t4 != 0)) {
    return 2;
  } else {
  }
  int _sv0t5 = sv0_vec_new();
  int tiny = _sv0t5;
  sv0_vec_push(tiny, 83);
  int _sv0t6 = verify_magic(tiny);
  if ((_sv0t6 != 0)) {
    return 3;
  } else {
  }
  return 0;
}

static int test_encode_decode_strings(void) {
  const char* source;
  source = "hello world";
  int _sv0t0 = sv0_vec_new();
  int strings = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int starts = _sv0t1;
  int _sv0t2 = sv0_vec_new();
  int ends = _sv0t2;
  sv0_vec_push(starts, 0);
  sv0_vec_push(ends, 5);
  sv0_vec_push(starts, 6);
  sv0_vec_push(ends, 11);
  sv0_vec_push(strings, 0);
  sv0_vec_push(strings, 1);
  int _sv0t3 = sv0_vec_new();
  int out = _sv0t3;
  int _sv0t4 = encode_strings(strings, source, starts, ends, out);
  int total = _sv0t4;
  int _sv0t5 = (4 + 4);
  int _sv0t6 = (_sv0t5 + 5);
  int _sv0t7 = (_sv0t6 + 4);
  int _sv0t8 = (_sv0t7 + 5);
  if ((total != _sv0t8)) {
    return 1;
  } else {
  }
  int _sv0t9 = sv0_vec_new();
  int d_starts = _sv0t9;
  int _sv0t10 = sv0_vec_new();
  int d_lens = _sv0t10;
  int _sv0t11 = decode_strings(out, 0, d_starts, d_lens);
  int end_pos = _sv0t11;
  if ((end_pos != total)) {
    return 2;
  } else {
  }
  int _sv0t12 = sv0_vec_len(d_starts);
  if ((_sv0t12 != 2)) {
    return 3;
  } else {
  }
  int _sv0t13 = sv0_vec_get(d_lens, 0);
  if ((_sv0t13 != 5)) {
    return 4;
  } else {
  }
  int _sv0t14 = sv0_vec_get(d_lens, 1);
  if ((_sv0t14 != 5)) {
    return 5;
  } else {
  }
  int _sv0t15 = sv0_vec_get(d_starts, 0);
  int _sv0t16 = sv0_vec_get(out, _sv0t15);
  if ((_sv0t16 != 104)) {
    return 6;
  } else {
  }
  int _sv0t17 = sv0_vec_get(d_starts, 1);
  int _sv0t18 = sv0_vec_get(out, _sv0t17);
  if ((_sv0t18 != 119)) {
    return 7;
  } else {
  }
  return 0;
}

static int test_encode_string_literals(void) {
  int _sv0t0 = sv0_vec_new();
  int strs = _sv0t0;
  sv0_vec_push(strs, 42);
  sv0_vec_push(strs, 99);
  int _sv0t1 = sv0_vec_new();
  int out = _sv0t1;
  int _sv0t2 = encode_string_literals(strs, out);
  int total = _sv0t2;
  if ((total != 12)) {
    return 1;
  } else {
  }
  int _sv0t3 = sv0_vec_len(out);
  if ((_sv0t3 != 12)) {
    return 2;
  } else {
  }
  int _sv0t4 = decode_u32_at(out, 0);
  int count = _sv0t4;
  if ((count != 2)) {
    return 3;
  } else {
  }
  int _sv0t5 = decode_u32_at(out, 4);
  int h0 = _sv0t5;
  if ((h0 != 42)) {
    return 4;
  } else {
  }
  int _sv0t6 = decode_u32_at(out, 8);
  int h1 = _sv0t6;
  if ((h1 != 99)) {
    return 5;
  } else {
  }
  return 0;
}

int main(void) {
  int _sv0t0 = test_magic();
  int r1 = _sv0t0;
  if ((r1 != 0)) {
    return r1;
  } else {
  }
  int _sv0t1 = test_opcodes();
  int r2 = _sv0t1;
  if ((r2 != 0)) {
    int _sv0t2 = (10 + r2);
    return _sv0t2;
  } else {
  }
  int _sv0t3 = test_insn_sizes();
  int r3 = _sv0t3;
  if ((r3 != 0)) {
    int _sv0t4 = (30 + r3);
    return _sv0t4;
  } else {
  }
  int _sv0t5 = test_opcode_classify();
  int r4 = _sv0t5;
  if ((r4 != 0)) {
    int _sv0t6 = (50 + r4);
    return _sv0t6;
  } else {
  }
  int _sv0t7 = test_le_bytes();
  int r5 = _sv0t7;
  if ((r5 != 0)) {
    int _sv0t8 = (70 + r5);
    return _sv0t8;
  } else {
  }
  int _sv0t9 = test_file_layout();
  int r6 = _sv0t9;
  if ((r6 != 0)) {
    int _sv0t10 = (80 + r6);
    return _sv0t10;
  } else {
  }
  int _sv0t11 = test_encode_le();
  int r7 = _sv0t11;
  if ((r7 != 0)) {
    int _sv0t12 = (90 + r7);
    return _sv0t12;
  } else {
  }
  int _sv0t13 = test_encode_header();
  int r8 = _sv0t13;
  if ((r8 != 0)) {
    int _sv0t14 = (110 + r8);
    return _sv0t14;
  } else {
  }
  int _sv0t15 = test_decode_at();
  int r9 = _sv0t15;
  if ((r9 != 0)) {
    int _sv0t16 = (120 + r9);
    return _sv0t16;
  } else {
  }
  int _sv0t17 = test_verify_magic();
  int r10 = _sv0t17;
  if ((r10 != 0)) {
    int _sv0t18 = (130 + r10);
    return _sv0t18;
  } else {
  }
  int _sv0t19 = test_encode_decode_strings();
  int r11 = _sv0t19;
  if ((r11 != 0)) {
    int _sv0t20 = (140 + r11);
    return _sv0t20;
  } else {
  }
  int _sv0t21 = test_encode_string_literals();
  int r12 = _sv0t21;
  if ((r12 != 0)) {
    int _sv0t22 = (150 + r12);
    return _sv0t22;
  } else {
  }
  return 0;
}


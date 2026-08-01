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
static int encode_insn_at(int instrs, int idx, int out);
static int decode_insn_at(int buf, int pos, int out);
static int encode_all_insns(int instrs, int out);
static int decode_all_insns(int buf, int buf_len, int out);
static int encode_file(int str_sec, int str_sec_len, int ft_entries, int out);
static int decode_file(int buf, int buf_len, int out_str_starts, int out_str_lens, int out_ft);
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
static int test_encode_insn_simple(void);
static int test_encode_insn_payload(void);
static int test_encode_insn_multi(void);
static int test_decode_insn_simple(void);
static int test_decode_insn_payload(void);
static int test_encode_decode_roundtrip(void);
static int test_encode_decode_all(void);
static int test_encode_file(void);
static int test_decode_file_roundtrip(void);
static int test_logical_opcode_cluster(void);

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
  int _sv0t0 = (val & 255);
  return _sv0t0;
}

static int le_byte1(int val) {
  int _sv0t0 = (val >> 8);
  int _sv0t1 = (_sv0t0 & 255);
  return _sv0t1;
}

static int le_byte2(int val) {
  int _sv0t0 = (val >> 16);
  int _sv0t1 = (_sv0t0 & 255);
  return _sv0t1;
}

static int le_byte3(int val) {
  int _sv0t0 = (val >> 24);
  int _sv0t1 = (_sv0t0 & 255);
  return _sv0t1;
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

static int encode_insn_at(int instrs, int idx, int out) {
  int _sv0t0 = sv0_vec_get(instrs, idx);
  int opc = _sv0t0;
  sv0_vec_push(out, opc);
  if ((opc == 0)) {
    int _sv0t1 = (idx + 1);
    return _sv0t1;
  } else {
  }
  if ((opc == 1)) {
    int _sv0t2 = (idx + 1);
    return _sv0t2;
  } else {
  }
  if ((opc == 2)) {
    int _sv0t3 = (idx + 1);
    return _sv0t3;
  } else {
  }
  if ((opc == 3)) {
    int _sv0t4 = (idx + 1);
    return _sv0t4;
  } else {
  }
  if ((opc == 4)) {
    int _sv0t5 = (idx + 1);
    int _sv0t6 = sv0_vec_get(instrs, _sv0t5);
    int _sv0t7 = encode_i32_le(_sv0t6, out);
    int _sv0t8 = (idx + 2);
    return _sv0t8;
  } else {
  }
  if ((opc == 5)) {
    int _sv0t9 = (idx + 1);
    int _sv0t10 = sv0_vec_get(instrs, _sv0t9);
    int _sv0t11 = encode_i32_le(_sv0t10, out);
    int _sv0t12 = (idx + 2);
    int _sv0t13 = sv0_vec_get(instrs, _sv0t12);
    int _sv0t14 = encode_i32_le(_sv0t13, out);
    int _sv0t15 = (idx + 3);
    return _sv0t15;
  } else {
  }
  if ((opc == 6)) {
    int _sv0t16 = (idx + 1);
    int _sv0t17 = sv0_vec_get(instrs, _sv0t16);
    int _sv0t18 = encode_i32_le(_sv0t17, out);
    int _sv0t19 = (idx + 2);
    int _sv0t20 = sv0_vec_get(instrs, _sv0t19);
    int _sv0t21 = encode_i32_le(_sv0t20, out);
    int _sv0t22 = (idx + 3);
    return _sv0t22;
  } else {
  }
  if ((opc == 7)) {
    int _sv0t23 = (idx + 1);
    int _sv0t24 = sv0_vec_get(instrs, _sv0t23);
    int bval = _sv0t24;
    if ((bval != 0)) {
      sv0_vec_push(out, 1);
    } else {
    }
    if ((bval == 0)) {
      sv0_vec_push(out, 0);
    } else {
    }
    int _sv0t25 = (idx + 2);
    return _sv0t25;
  } else {
  }
  if ((opc == 8)) {
    int _sv0t26 = (idx + 1);
    int _sv0t27 = sv0_vec_get(instrs, _sv0t26);
    int _sv0t28 = encode_u32_le(_sv0t27, out);
    int _sv0t29 = (idx + 2);
    return _sv0t29;
  } else {
  }
  if ((opc >= 16)) {
    if ((opc <= 21)) {
      int _sv0t30 = (idx + 1);
      return _sv0t30;
    } else {
    }
  } else {
  }
  if ((opc >= 32)) {
    if ((opc <= 37)) {
      int _sv0t31 = (idx + 1);
      return _sv0t31;
    } else {
    }
  } else {
  }
  if ((opc >= 48)) {
    if ((opc <= 52)) {
      int _sv0t32 = (idx + 1);
      return _sv0t32;
    } else {
    }
  } else {
  }
  if ((opc >= 64)) {
    if ((opc <= 69)) {
      int _sv0t33 = (idx + 1);
      return _sv0t33;
    } else {
    }
  } else {
  }
  if ((opc >= 80)) {
    if ((opc <= 82)) {
      int _sv0t34 = (idx + 1);
      return _sv0t34;
    } else {
    }
  } else {
  }
  if ((opc >= 88)) {
    if ((opc <= 93)) {
      int _sv0t35 = (idx + 1);
      return _sv0t35;
    } else {
    }
  } else {
  }
  if ((opc == 96)) {
    int _sv0t36 = (idx + 1);
    int _sv0t37 = sv0_vec_get(instrs, _sv0t36);
    int _sv0t38 = encode_u32_le(_sv0t37, out);
    int _sv0t39 = (idx + 2);
    return _sv0t39;
  } else {
  }
  if ((opc == 97)) {
    int _sv0t40 = (idx + 1);
    int _sv0t41 = sv0_vec_get(instrs, _sv0t40);
    int _sv0t42 = encode_u32_le(_sv0t41, out);
    int _sv0t43 = (idx + 2);
    return _sv0t43;
  } else {
  }
  if ((opc == 112)) {
    int _sv0t44 = (idx + 1);
    int _sv0t45 = sv0_vec_get(instrs, _sv0t44);
    int _sv0t46 = encode_i32_le(_sv0t45, out);
    int _sv0t47 = (idx + 2);
    return _sv0t47;
  } else {
  }
  if ((opc == 113)) {
    int _sv0t48 = (idx + 1);
    int _sv0t49 = sv0_vec_get(instrs, _sv0t48);
    int _sv0t50 = encode_i32_le(_sv0t49, out);
    int _sv0t51 = (idx + 2);
    return _sv0t51;
  } else {
  }
  if ((opc == 114)) {
    int _sv0t52 = (idx + 1);
    int _sv0t53 = sv0_vec_get(instrs, _sv0t52);
    int _sv0t54 = encode_i32_le(_sv0t53, out);
    int _sv0t55 = (idx + 2);
    return _sv0t55;
  } else {
  }
  if ((opc == 115)) {
    int _sv0t56 = (idx + 1);
    int _sv0t57 = sv0_vec_get(instrs, _sv0t56);
    int _sv0t58 = encode_u32_le(_sv0t57, out);
    int _sv0t59 = (idx + 2);
    int _sv0t60 = sv0_vec_get(instrs, _sv0t59);
    int _sv0t61 = encode_u32_le(_sv0t60, out);
    int _sv0t62 = (idx + 3);
    return _sv0t62;
  } else {
  }
  if ((opc == 116)) {
    int _sv0t63 = (idx + 1);
    return _sv0t63;
  } else {
  }
  if ((opc == 117)) {
    int _sv0t64 = (idx + 1);
    int _sv0t65 = sv0_vec_get(instrs, _sv0t64);
    int _sv0t66 = encode_u32_le(_sv0t65, out);
    int _sv0t67 = (idx + 2);
    return _sv0t67;
  } else {
  }
  if ((opc == 118)) {
    int _sv0t68 = (idx + 1);
    int _sv0t69 = sv0_vec_get(instrs, _sv0t68);
    int cnt = _sv0t69;
    sv0_vec_push(out, cnt);
    int _sv0t70 = (idx + 2);
    return _sv0t70;
  } else {
  }
  if ((opc == 128)) {
    int _sv0t71 = (idx + 1);
    int _sv0t72 = sv0_vec_get(instrs, _sv0t71);
    int _sv0t73 = encode_u32_le(_sv0t72, out);
    int _sv0t74 = (idx + 2);
    return _sv0t74;
  } else {
  }
  if ((opc == 129)) {
    int _sv0t75 = (idx + 1);
    int _sv0t76 = sv0_vec_get(instrs, _sv0t75);
    int _sv0t77 = encode_u32_le(_sv0t76, out);
    int _sv0t78 = (idx + 2);
    return _sv0t78;
  } else {
  }
  if ((opc == 130)) {
    int _sv0t79 = (idx + 1);
    int _sv0t80 = sv0_vec_get(instrs, _sv0t79);
    int _sv0t81 = encode_u32_le(_sv0t80, out);
    int _sv0t82 = (idx + 2);
    return _sv0t82;
  } else {
  }
  if ((opc == 131)) {
    int _sv0t83 = (idx + 1);
    int _sv0t84 = sv0_vec_get(instrs, _sv0t83);
    int _sv0t85 = encode_u32_le(_sv0t84, out);
    int _sv0t86 = (idx + 2);
    return _sv0t86;
  } else {
  }
  if ((opc == 132)) {
    int _sv0t87 = (idx + 1);
    return _sv0t87;
  } else {
  }
  if ((opc == 133)) {
    int _sv0t88 = (idx + 1);
    return _sv0t88;
  } else {
  }
  if ((opc == 144)) {
    int _sv0t89 = (idx + 1);
    int _sv0t90 = sv0_vec_get(instrs, _sv0t89);
    int _sv0t91 = encode_u32_le(_sv0t90, out);
    int _sv0t92 = (idx + 2);
    int _sv0t93 = sv0_vec_get(instrs, _sv0t92);
    int _sv0t94 = encode_u32_le(_sv0t93, out);
    int _sv0t95 = (idx + 3);
    int _sv0t96 = sv0_vec_get(instrs, _sv0t95);
    int _sv0t97 = encode_u32_le(_sv0t96, out);
    int _sv0t98 = (idx + 4);
    return _sv0t98;
  } else {
  }
  if ((opc == 145)) {
    int _sv0t99 = (idx + 1);
    return _sv0t99;
  } else {
  }
  if ((opc == 146)) {
    int _sv0t100 = (idx + 1);
    int _sv0t101 = sv0_vec_get(instrs, _sv0t100);
    int _sv0t102 = encode_u32_le(_sv0t101, out);
    int _sv0t103 = (idx + 2);
    return _sv0t103;
  } else {
  }
  if ((opc == 160)) {
    int _sv0t104 = (idx + 1);
    int _sv0t105 = sv0_vec_get(instrs, _sv0t104);
    int _sv0t106 = encode_u32_le(_sv0t105, out);
    int _sv0t107 = (idx + 2);
    return _sv0t107;
  } else {
  }
  if ((opc == 161)) {
    int _sv0t108 = (idx + 1);
    int _sv0t109 = sv0_vec_get(instrs, _sv0t108);
    int _sv0t110 = encode_u16_le(_sv0t109, out);
    int _sv0t111 = (idx + 2);
    int _sv0t112 = sv0_vec_get(instrs, _sv0t111);
    int _sv0t113 = encode_u16_le(_sv0t112, out);
    int _sv0t114 = (idx + 3);
    return _sv0t114;
  } else {
  }
  int _sv0t115 = (idx + 1);
  return _sv0t115;
}

static int decode_insn_at(int buf, int pos, int out) {
  int _sv0t0 = sv0_vec_get(buf, pos);
  int opc = _sv0t0;
  sv0_vec_push(out, opc);
  if ((opc == 0)) {
    int _sv0t1 = (pos + 1);
    return _sv0t1;
  } else {
  }
  if ((opc == 1)) {
    int _sv0t2 = (pos + 1);
    return _sv0t2;
  } else {
  }
  if ((opc == 2)) {
    int _sv0t3 = (pos + 1);
    return _sv0t3;
  } else {
  }
  if ((opc == 3)) {
    int _sv0t4 = (pos + 1);
    return _sv0t4;
  } else {
  }
  if ((opc == 4)) {
    int _sv0t5 = (pos + 1);
    int _sv0t6 = decode_u32_at(buf, _sv0t5);
    int v = _sv0t6;
    sv0_vec_push(out, v);
    int _sv0t7 = (pos + 5);
    return _sv0t7;
  } else {
  }
  if ((opc == 5)) {
    int _sv0t8 = (pos + 1);
    int _sv0t9 = decode_u32_at(buf, _sv0t8);
    int lo = _sv0t9;
    int _sv0t10 = (pos + 5);
    int _sv0t11 = decode_u32_at(buf, _sv0t10);
    int hi = _sv0t11;
    sv0_vec_push(out, lo);
    sv0_vec_push(out, hi);
    int _sv0t12 = (pos + 9);
    return _sv0t12;
  } else {
  }
  if ((opc == 6)) {
    int _sv0t13 = (pos + 1);
    int _sv0t14 = decode_u32_at(buf, _sv0t13);
    int lo = _sv0t14;
    int _sv0t15 = (pos + 5);
    int _sv0t16 = decode_u32_at(buf, _sv0t15);
    int hi = _sv0t16;
    sv0_vec_push(out, lo);
    sv0_vec_push(out, hi);
    int _sv0t17 = (pos + 9);
    return _sv0t17;
  } else {
  }
  if ((opc == 7)) {
    int _sv0t18 = (pos + 1);
    int _sv0t19 = sv0_vec_get(buf, _sv0t18);
    int bval = _sv0t19;
    sv0_vec_push(out, bval);
    int _sv0t20 = (pos + 2);
    return _sv0t20;
  } else {
  }
  if ((opc == 8)) {
    int _sv0t21 = (pos + 1);
    int _sv0t22 = decode_u32_at(buf, _sv0t21);
    int v = _sv0t22;
    sv0_vec_push(out, v);
    int _sv0t23 = (pos + 5);
    return _sv0t23;
  } else {
  }
  if ((opc >= 16)) {
    if ((opc <= 21)) {
      int _sv0t24 = (pos + 1);
      return _sv0t24;
    } else {
    }
  } else {
  }
  if ((opc >= 32)) {
    if ((opc <= 37)) {
      int _sv0t25 = (pos + 1);
      return _sv0t25;
    } else {
    }
  } else {
  }
  if ((opc >= 48)) {
    if ((opc <= 52)) {
      int _sv0t26 = (pos + 1);
      return _sv0t26;
    } else {
    }
  } else {
  }
  if ((opc >= 64)) {
    if ((opc <= 69)) {
      int _sv0t27 = (pos + 1);
      return _sv0t27;
    } else {
    }
  } else {
  }
  if ((opc >= 80)) {
    if ((opc <= 82)) {
      int _sv0t28 = (pos + 1);
      return _sv0t28;
    } else {
    }
  } else {
  }
  if ((opc >= 88)) {
    if ((opc <= 93)) {
      int _sv0t29 = (pos + 1);
      return _sv0t29;
    } else {
    }
  } else {
  }
  if ((opc == 96)) {
    int _sv0t30 = (pos + 1);
    int _sv0t31 = decode_u32_at(buf, _sv0t30);
    int v = _sv0t31;
    sv0_vec_push(out, v);
    int _sv0t32 = (pos + 5);
    return _sv0t32;
  } else {
  }
  if ((opc == 97)) {
    int _sv0t33 = (pos + 1);
    int _sv0t34 = decode_u32_at(buf, _sv0t33);
    int v = _sv0t34;
    sv0_vec_push(out, v);
    int _sv0t35 = (pos + 5);
    return _sv0t35;
  } else {
  }
  if ((opc == 112)) {
    int _sv0t36 = (pos + 1);
    int _sv0t37 = decode_u32_at(buf, _sv0t36);
    int v = _sv0t37;
    sv0_vec_push(out, v);
    int _sv0t38 = (pos + 5);
    return _sv0t38;
  } else {
  }
  if ((opc == 113)) {
    int _sv0t39 = (pos + 1);
    int _sv0t40 = decode_u32_at(buf, _sv0t39);
    int v = _sv0t40;
    sv0_vec_push(out, v);
    int _sv0t41 = (pos + 5);
    return _sv0t41;
  } else {
  }
  if ((opc == 114)) {
    int _sv0t42 = (pos + 1);
    int _sv0t43 = decode_u32_at(buf, _sv0t42);
    int v = _sv0t43;
    sv0_vec_push(out, v);
    int _sv0t44 = (pos + 5);
    return _sv0t44;
  } else {
  }
  if ((opc == 115)) {
    int _sv0t45 = (pos + 1);
    int _sv0t46 = decode_u32_at(buf, _sv0t45);
    int f = _sv0t46;
    int _sv0t47 = (pos + 5);
    int _sv0t48 = decode_u32_at(buf, _sv0t47);
    int n = _sv0t48;
    sv0_vec_push(out, f);
    sv0_vec_push(out, n);
    int _sv0t49 = (pos + 9);
    return _sv0t49;
  } else {
  }
  if ((opc == 116)) {
    int _sv0t50 = (pos + 1);
    return _sv0t50;
  } else {
  }
  if ((opc == 117)) {
    int _sv0t51 = (pos + 1);
    int _sv0t52 = decode_u32_at(buf, _sv0t51);
    int v = _sv0t52;
    sv0_vec_push(out, v);
    int _sv0t53 = (pos + 5);
    return _sv0t53;
  } else {
  }
  if ((opc == 118)) {
    int _sv0t54 = (pos + 1);
    int _sv0t55 = sv0_vec_get(buf, _sv0t54);
    int cnt = _sv0t55;
    sv0_vec_push(out, cnt);
    int _sv0t56 = (pos + 2);
    return _sv0t56;
  } else {
  }
  if ((opc == 128)) {
    int _sv0t57 = (pos + 1);
    int _sv0t58 = decode_u32_at(buf, _sv0t57);
    int v = _sv0t58;
    sv0_vec_push(out, v);
    int _sv0t59 = (pos + 5);
    return _sv0t59;
  } else {
  }
  if ((opc == 129)) {
    int _sv0t60 = (pos + 1);
    int _sv0t61 = decode_u32_at(buf, _sv0t60);
    int v = _sv0t61;
    sv0_vec_push(out, v);
    int _sv0t62 = (pos + 5);
    return _sv0t62;
  } else {
  }
  if ((opc == 130)) {
    int _sv0t63 = (pos + 1);
    int _sv0t64 = decode_u32_at(buf, _sv0t63);
    int v = _sv0t64;
    sv0_vec_push(out, v);
    int _sv0t65 = (pos + 5);
    return _sv0t65;
  } else {
  }
  if ((opc == 131)) {
    int _sv0t66 = (pos + 1);
    int _sv0t67 = decode_u32_at(buf, _sv0t66);
    int v = _sv0t67;
    sv0_vec_push(out, v);
    int _sv0t68 = (pos + 5);
    return _sv0t68;
  } else {
  }
  if ((opc == 132)) {
    int _sv0t69 = (pos + 1);
    return _sv0t69;
  } else {
  }
  if ((opc == 133)) {
    int _sv0t70 = (pos + 1);
    return _sv0t70;
  } else {
  }
  if ((opc == 144)) {
    int _sv0t71 = (pos + 1);
    int _sv0t72 = decode_u32_at(buf, _sv0t71);
    int t = _sv0t72;
    int _sv0t73 = (pos + 5);
    int _sv0t74 = decode_u32_at(buf, _sv0t73);
    int vv = _sv0t74;
    int _sv0t75 = (pos + 9);
    int _sv0t76 = decode_u32_at(buf, _sv0t75);
    int fc = _sv0t76;
    sv0_vec_push(out, t);
    sv0_vec_push(out, vv);
    sv0_vec_push(out, fc);
    int _sv0t77 = (pos + 13);
    return _sv0t77;
  } else {
  }
  if ((opc == 145)) {
    int _sv0t78 = (pos + 1);
    return _sv0t78;
  } else {
  }
  if ((opc == 146)) {
    int _sv0t79 = (pos + 1);
    int _sv0t80 = decode_u32_at(buf, _sv0t79);
    int v = _sv0t80;
    sv0_vec_push(out, v);
    int _sv0t81 = (pos + 5);
    return _sv0t81;
  } else {
  }
  if ((opc == 160)) {
    int _sv0t82 = (pos + 1);
    int _sv0t83 = decode_u32_at(buf, _sv0t82);
    int v = _sv0t83;
    sv0_vec_push(out, v);
    int _sv0t84 = (pos + 5);
    return _sv0t84;
  } else {
  }
  if ((opc == 161)) {
    int _sv0t85 = (pos + 1);
    int _sv0t86 = decode_u16_at(buf, _sv0t85);
    int a = _sv0t86;
    int _sv0t87 = (pos + 3);
    int _sv0t88 = decode_u16_at(buf, _sv0t87);
    int b = _sv0t88;
    sv0_vec_push(out, a);
    sv0_vec_push(out, b);
    int _sv0t89 = (pos + 5);
    return _sv0t89;
  } else {
  }
  int _sv0t90 = (pos + 1);
  return _sv0t90;
}

static int encode_all_insns(int instrs, int out) {
  int _sv0t0 = sv0_vec_len(instrs);
  int n = _sv0t0;
  int _sv0t1 = sv0_vec_len(out);
  int start_len = _sv0t1;
  int idx = 0;
  while ((idx < n)) {
    int _sv0t2 = encode_insn_at(instrs, idx, out);
    idx = _sv0t2;
  }
  int _sv0t3 = sv0_vec_len(out);
  int _sv0t4 = (_sv0t3 - start_len);
  return _sv0t4;
}

static int decode_all_insns(int buf, int buf_len, int out) {
  int _sv0t0 = sv0_vec_len(out);
  int start_len = _sv0t0;
  int pos = 0;
  while ((pos < buf_len)) {
    int _sv0t1 = decode_insn_at(buf, pos, out);
    pos = _sv0t1;
  }
  int _sv0t2 = sv0_vec_len(out);
  int _sv0t3 = (_sv0t2 - start_len);
  return _sv0t3;
}

static int encode_file(int str_sec, int str_sec_len, int ft_entries, int out) {
  int _sv0t0 = encode_header(1, out);
  int _sv0t1 = sv0_vec_len(ft_entries);
  int func_count = (_sv0t1 / 4);
  int _sv0t2 = encode_u32_le(str_sec_len, out);
  int si = 0;
  while ((si < str_sec_len)) {
    int _sv0t3 = sv0_vec_get(str_sec, si);
    sv0_vec_push(out, _sv0t3);
    si = (si + 1);
  }
  int _sv0t4 = sv0_vec_new();
  int code_sizes = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int code_bufs = _sv0t5;
  int fi = 0;
  while ((fi < func_count)) {
    int _sv0t6 = (fi * 4);
    int _sv0t7 = (_sv0t6 + 3);
    int _sv0t8 = sv0_vec_get(ft_entries, _sv0t7);
    int code_vec = _sv0t8;
    int _sv0t9 = sv0_vec_new();
    int cbuf = _sv0t9;
    int _sv0t10 = encode_all_insns(code_vec, cbuf);
    int bcount = _sv0t10;
    sv0_vec_push(code_sizes, bcount);
    sv0_vec_push(code_bufs, cbuf);
    fi = (fi + 1);
  }
  int _sv0t11 = (func_count * 20);
  int func_sec_data_len = (4 + _sv0t11);
  int _sv0t12 = encode_u32_le(func_sec_data_len, out);
  int _sv0t13 = encode_u32_le(func_count, out);
  int code_offset = 0;
  fi = 0;
  while ((fi < func_count)) {
    int _sv0t14 = (fi * 4);
    int _sv0t15 = (_sv0t14 + 0);
    int _sv0t16 = sv0_vec_get(ft_entries, _sv0t15);
    int _sv0t17 = encode_u32_le(_sv0t16, out);
    int _sv0t18 = (fi * 4);
    int _sv0t19 = (_sv0t18 + 1);
    int _sv0t20 = sv0_vec_get(ft_entries, _sv0t19);
    int _sv0t21 = encode_u32_le(_sv0t20, out);
    int _sv0t22 = (fi * 4);
    int _sv0t23 = (_sv0t22 + 2);
    int _sv0t24 = sv0_vec_get(ft_entries, _sv0t23);
    int _sv0t25 = encode_u32_le(_sv0t24, out);
    int _sv0t26 = encode_u32_le(code_offset, out);
    int _sv0t27 = sv0_vec_get(code_sizes, fi);
    int _sv0t28 = encode_u32_le(_sv0t27, out);
    int _sv0t29 = sv0_vec_get(code_sizes, fi);
    code_offset = (code_offset + _sv0t29);
    fi = (fi + 1);
  }
  int _sv0t30 = encode_u32_le(code_offset, out);
  fi = 0;
  while ((fi < func_count)) {
    int _sv0t31 = sv0_vec_get(code_bufs, fi);
    int fb = _sv0t31;
    int _sv0t32 = sv0_vec_len(fb);
    int flen = _sv0t32;
    int ci = 0;
    while ((ci < flen)) {
      int _sv0t33 = sv0_vec_get(fb, ci);
      sv0_vec_push(out, _sv0t33);
      ci = (ci + 1);
    }
    fi = (fi + 1);
  }
  int _sv0t34 = sv0_vec_len(out);
  return _sv0t34;
}

static int decode_file(int buf, int buf_len, int out_str_starts, int out_str_lens, int out_ft) {
  int _sv0t0 = verify_magic(buf);
  if ((_sv0t0 == 0)) {
    int _sv0t1 = (0 - 1);
    return _sv0t1;
  } else {
  }
  int _sv0t2 = decode_u16_at(buf, 4);
  int ver = _sv0t2;
  if ((ver != 1)) {
    int _sv0t3 = (0 - 2);
    return _sv0t3;
  } else {
  }
  int pos = 6;
  int _sv0t4 = decode_u32_at(buf, pos);
  int str_sec_len = _sv0t4;
  pos = (pos + 4);
  int str_end = (pos + str_sec_len);
  int _sv0t5 = decode_strings(buf, pos, out_str_starts, out_str_lens);
  pos = str_end;
  int _sv0t6 = decode_u32_at(buf, pos);
  int func_sec_len = _sv0t6;
  pos = (pos + 4);
  int func_sec_end = (pos + func_sec_len);
  int _sv0t7 = decode_u32_at(buf, pos);
  int func_count = _sv0t7;
  pos = (pos + 4);
  int code_sec_start = (func_sec_end + 4);
  int fi = 0;
  while ((fi < func_count)) {
    int _sv0t8 = decode_u32_at(buf, pos);
    int nameIdx = _sv0t8;
    pos = (pos + 4);
    int _sv0t9 = decode_u32_at(buf, pos);
    int arity = _sv0t9;
    pos = (pos + 4);
    int _sv0t10 = decode_u32_at(buf, pos);
    int localCount = _sv0t10;
    pos = (pos + 4);
    int _sv0t11 = decode_u32_at(buf, pos);
    int code_off = _sv0t11;
    pos = (pos + 4);
    int _sv0t12 = decode_u32_at(buf, pos);
    int code_len = _sv0t12;
    pos = (pos + 4);
    int _sv0t13 = sv0_vec_new();
    int code_buf = _sv0t13;
    int ci = 0;
    while ((ci < code_len)) {
      int _sv0t14 = (code_sec_start + code_off);
      int _sv0t15 = (_sv0t14 + ci);
      int _sv0t16 = sv0_vec_get(buf, _sv0t15);
      sv0_vec_push(code_buf, _sv0t16);
      ci = (ci + 1);
    }
    int _sv0t17 = sv0_vec_new();
    int code_vec = _sv0t17;
    int _sv0t18 = decode_all_insns(code_buf, code_len, code_vec);
    sv0_vec_push(out_ft, nameIdx);
    sv0_vec_push(out_ft, arity);
    sv0_vec_push(out_ft, localCount);
    sv0_vec_push(out_ft, code_vec);
    fi = (fi + 1);
  }
  return 0;
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

static int test_encode_insn_simple(void) {
  int _sv0t0 = sv0_vec_new();
  int instrs = _sv0t0;
  sv0_vec_push(instrs, 0);
  sv0_vec_push(instrs, 1);
  sv0_vec_push(instrs, 2);
  sv0_vec_push(instrs, 3);
  sv0_vec_push(instrs, 116);
  sv0_vec_push(instrs, 132);
  sv0_vec_push(instrs, 133);
  sv0_vec_push(instrs, 145);
  int _sv0t1 = sv0_vec_new();
  int out = _sv0t1;
  int idx = 0;
  int _sv0t2 = encode_insn_at(instrs, idx, out);
  idx = _sv0t2;
  if ((idx != 1)) {
    return 1;
  } else {
  }
  int _sv0t3 = encode_insn_at(instrs, idx, out);
  idx = _sv0t3;
  int _sv0t4 = encode_insn_at(instrs, idx, out);
  idx = _sv0t4;
  int _sv0t5 = encode_insn_at(instrs, idx, out);
  idx = _sv0t5;
  int _sv0t6 = encode_insn_at(instrs, idx, out);
  idx = _sv0t6;
  int _sv0t7 = encode_insn_at(instrs, idx, out);
  idx = _sv0t7;
  int _sv0t8 = encode_insn_at(instrs, idx, out);
  idx = _sv0t8;
  int _sv0t9 = encode_insn_at(instrs, idx, out);
  idx = _sv0t9;
  if ((idx != 8)) {
    return 2;
  } else {
  }
  int _sv0t10 = sv0_vec_len(out);
  if ((_sv0t10 != 8)) {
    return 3;
  } else {
  }
  int _sv0t11 = sv0_vec_get(out, 0);
  if ((_sv0t11 != 0)) {
    return 4;
  } else {
  }
  int _sv0t12 = sv0_vec_get(out, 1);
  if ((_sv0t12 != 1)) {
    return 5;
  } else {
  }
  int _sv0t13 = sv0_vec_get(out, 4);
  if ((_sv0t13 != 116)) {
    return 6;
  } else {
  }
  int _sv0t14 = sv0_vec_get(out, 7);
  if ((_sv0t14 != 145)) {
    return 7;
  } else {
  }
  return 0;
}

static int test_encode_insn_payload(void) {
  int _sv0t0 = sv0_vec_new();
  int instrs = _sv0t0;
  sv0_vec_push(instrs, 4);
  sv0_vec_push(instrs, 42);
  sv0_vec_push(instrs, 7);
  sv0_vec_push(instrs, 1);
  sv0_vec_push(instrs, 7);
  sv0_vec_push(instrs, 0);
  sv0_vec_push(instrs, 96);
  sv0_vec_push(instrs, 5);
  sv0_vec_push(instrs, 118);
  sv0_vec_push(instrs, 3);
  int _sv0t1 = sv0_vec_new();
  int out = _sv0t1;
  int idx = 0;
  int _sv0t2 = encode_insn_at(instrs, idx, out);
  idx = _sv0t2;
  if ((idx != 2)) {
    return 1;
  } else {
  }
  int _sv0t3 = sv0_vec_len(out);
  if ((_sv0t3 != 5)) {
    return 2;
  } else {
  }
  int _sv0t4 = sv0_vec_get(out, 0);
  if ((_sv0t4 != 4)) {
    return 3;
  } else {
  }
  int _sv0t5 = sv0_vec_get(out, 1);
  if ((_sv0t5 != 42)) {
    return 4;
  } else {
  }
  int _sv0t6 = sv0_vec_get(out, 2);
  if ((_sv0t6 != 0)) {
    return 5;
  } else {
  }
  int _sv0t7 = sv0_vec_get(out, 3);
  if ((_sv0t7 != 0)) {
    return 6;
  } else {
  }
  int _sv0t8 = sv0_vec_get(out, 4);
  if ((_sv0t8 != 0)) {
    return 7;
  } else {
  }
  int _sv0t9 = encode_insn_at(instrs, idx, out);
  idx = _sv0t9;
  if ((idx != 4)) {
    return 8;
  } else {
  }
  int _sv0t10 = sv0_vec_get(out, 5);
  if ((_sv0t10 != 7)) {
    return 9;
  } else {
  }
  int _sv0t11 = sv0_vec_get(out, 6);
  if ((_sv0t11 != 1)) {
    return 10;
  } else {
  }
  int _sv0t12 = encode_insn_at(instrs, idx, out);
  idx = _sv0t12;
  if ((idx != 6)) {
    return 11;
  } else {
  }
  int _sv0t13 = sv0_vec_get(out, 7);
  if ((_sv0t13 != 7)) {
    return 12;
  } else {
  }
  int _sv0t14 = sv0_vec_get(out, 8);
  if ((_sv0t14 != 0)) {
    return 13;
  } else {
  }
  int _sv0t15 = encode_insn_at(instrs, idx, out);
  idx = _sv0t15;
  if ((idx != 8)) {
    return 14;
  } else {
  }
  int _sv0t16 = sv0_vec_len(out);
  if ((_sv0t16 != 14)) {
    return 15;
  } else {
  }
  int _sv0t17 = encode_insn_at(instrs, idx, out);
  idx = _sv0t17;
  if ((idx != 10)) {
    return 16;
  } else {
  }
  int _sv0t18 = sv0_vec_get(out, 14);
  if ((_sv0t18 != 118)) {
    return 17;
  } else {
  }
  int _sv0t19 = sv0_vec_get(out, 15);
  if ((_sv0t19 != 3)) {
    return 18;
  } else {
  }
  return 0;
}

static int test_encode_insn_multi(void) {
  int _sv0t0 = sv0_vec_new();
  int instrs = _sv0t0;
  sv0_vec_push(instrs, 115);
  sv0_vec_push(instrs, 2);
  sv0_vec_push(instrs, 3);
  sv0_vec_push(instrs, 144);
  sv0_vec_push(instrs, 10);
  sv0_vec_push(instrs, 20);
  sv0_vec_push(instrs, 30);
  sv0_vec_push(instrs, 161);
  sv0_vec_push(instrs, 1);
  sv0_vec_push(instrs, 2);
  int _sv0t1 = sv0_vec_new();
  int out = _sv0t1;
  int idx = 0;
  int _sv0t2 = encode_insn_at(instrs, idx, out);
  idx = _sv0t2;
  if ((idx != 3)) {
    return 1;
  } else {
  }
  int _sv0t3 = sv0_vec_len(out);
  if ((_sv0t3 != 9)) {
    return 2;
  } else {
  }
  int _sv0t4 = sv0_vec_get(out, 0);
  if ((_sv0t4 != 115)) {
    return 3;
  } else {
  }
  int _sv0t5 = decode_u32_at(out, 1);
  int d_func = _sv0t5;
  if ((d_func != 2)) {
    return 4;
  } else {
  }
  int _sv0t6 = decode_u32_at(out, 5);
  int d_arity = _sv0t6;
  if ((d_arity != 3)) {
    return 5;
  } else {
  }
  int _sv0t7 = encode_insn_at(instrs, idx, out);
  idx = _sv0t7;
  if ((idx != 7)) {
    return 6;
  } else {
  }
  int _sv0t8 = sv0_vec_len(out);
  if ((_sv0t8 != 22)) {
    return 7;
  } else {
  }
  int _sv0t9 = sv0_vec_get(out, 9);
  if ((_sv0t9 != 144)) {
    return 8;
  } else {
  }
  int _sv0t10 = encode_insn_at(instrs, idx, out);
  idx = _sv0t10;
  if ((idx != 10)) {
    return 9;
  } else {
  }
  int _sv0t11 = sv0_vec_len(out);
  if ((_sv0t11 != 27)) {
    return 10;
  } else {
  }
  int _sv0t12 = sv0_vec_get(out, 22);
  if ((_sv0t12 != 161)) {
    return 11;
  } else {
  }
  int _sv0t13 = decode_u16_at(out, 23);
  int d_from = _sv0t13;
  if ((d_from != 1)) {
    return 12;
  } else {
  }
  int _sv0t14 = decode_u16_at(out, 25);
  int d_to = _sv0t14;
  if ((d_to != 2)) {
    return 13;
  } else {
  }
  return 0;
}

static int test_decode_insn_simple(void) {
  int _sv0t0 = sv0_vec_new();
  int buf = _sv0t0;
  sv0_vec_push(buf, 0);
  sv0_vec_push(buf, 1);
  sv0_vec_push(buf, 116);
  int _sv0t1 = sv0_vec_new();
  int out = _sv0t1;
  int pos = 0;
  int _sv0t2 = decode_insn_at(buf, pos, out);
  pos = _sv0t2;
  if ((pos != 1)) {
    return 1;
  } else {
  }
  int _sv0t3 = sv0_vec_get(out, 0);
  if ((_sv0t3 != 0)) {
    return 2;
  } else {
  }
  int _sv0t4 = decode_insn_at(buf, pos, out);
  pos = _sv0t4;
  if ((pos != 2)) {
    return 3;
  } else {
  }
  int _sv0t5 = sv0_vec_get(out, 1);
  if ((_sv0t5 != 1)) {
    return 4;
  } else {
  }
  int _sv0t6 = decode_insn_at(buf, pos, out);
  pos = _sv0t6;
  if ((pos != 3)) {
    return 5;
  } else {
  }
  int _sv0t7 = sv0_vec_get(out, 2);
  if ((_sv0t7 != 116)) {
    return 6;
  } else {
  }
  return 0;
}

static int test_decode_insn_payload(void) {
  int _sv0t0 = sv0_vec_new();
  int buf = _sv0t0;
  sv0_vec_push(buf, 4);
  int _sv0t1 = encode_i32_le(42, buf);
  sv0_vec_push(buf, 7);
  sv0_vec_push(buf, 1);
  sv0_vec_push(buf, 96);
  int _sv0t2 = encode_u32_le(99, buf);
  int _sv0t3 = sv0_vec_new();
  int out = _sv0t3;
  int pos = 0;
  int _sv0t4 = decode_insn_at(buf, pos, out);
  pos = _sv0t4;
  if ((pos != 5)) {
    return 1;
  } else {
  }
  int _sv0t5 = sv0_vec_get(out, 0);
  if ((_sv0t5 != 4)) {
    return 2;
  } else {
  }
  int _sv0t6 = sv0_vec_get(out, 1);
  if ((_sv0t6 != 42)) {
    return 3;
  } else {
  }
  int _sv0t7 = decode_insn_at(buf, pos, out);
  pos = _sv0t7;
  if ((pos != 7)) {
    return 4;
  } else {
  }
  int _sv0t8 = sv0_vec_get(out, 2);
  if ((_sv0t8 != 7)) {
    return 5;
  } else {
  }
  int _sv0t9 = sv0_vec_get(out, 3);
  if ((_sv0t9 != 1)) {
    return 6;
  } else {
  }
  int _sv0t10 = decode_insn_at(buf, pos, out);
  pos = _sv0t10;
  if ((pos != 12)) {
    return 7;
  } else {
  }
  int _sv0t11 = sv0_vec_get(out, 4);
  if ((_sv0t11 != 96)) {
    return 8;
  } else {
  }
  int _sv0t12 = sv0_vec_get(out, 5);
  if ((_sv0t12 != 99)) {
    return 9;
  } else {
  }
  return 0;
}

static int test_encode_decode_roundtrip(void) {
  int _sv0t0 = sv0_vec_new();
  int instrs = _sv0t0;
  sv0_vec_push(instrs, 0);
  sv0_vec_push(instrs, 4);
  sv0_vec_push(instrs, 100);
  sv0_vec_push(instrs, 7);
  sv0_vec_push(instrs, 1);
  sv0_vec_push(instrs, 115);
  sv0_vec_push(instrs, 5);
  sv0_vec_push(instrs, 2);
  sv0_vec_push(instrs, 116);
  sv0_vec_push(instrs, 118);
  sv0_vec_push(instrs, 4);
  int _sv0t1 = sv0_vec_new();
  int bytes = _sv0t1;
  int _sv0t2 = encode_all_insns(instrs, bytes);
  int bcount = _sv0t2;
  int _sv0t3 = (1 + 5);
  int _sv0t4 = (_sv0t3 + 2);
  int _sv0t5 = (_sv0t4 + 9);
  int _sv0t6 = (_sv0t5 + 1);
  int _sv0t7 = (_sv0t6 + 2);
  if ((bcount != _sv0t7)) {
    return 1;
  } else {
  }
  int _sv0t8 = sv0_vec_new();
  int decoded = _sv0t8;
  int _sv0t9 = decode_all_insns(bytes, bcount, decoded);
  int scount = _sv0t9;
  int _sv0t10 = sv0_vec_len(decoded);
  int _sv0t11 = sv0_vec_len(instrs);
  if ((_sv0t10 != _sv0t11)) {
    return 2;
  } else {
  }
  int i = 0;
  while (1) {
    int _sv0t12 = sv0_vec_len(instrs);
    int _sv0t16 = (i < _sv0t12);
    if ((!_sv0t16)) {
      break;
    } else {
    }
    int _sv0t13 = sv0_vec_get(instrs, i);
    int _sv0t14 = sv0_vec_get(decoded, i);
    if ((_sv0t13 != _sv0t14)) {
      int _sv0t15 = (3 + i);
      return _sv0t15;
    } else {
    }
    i = (i + 1);
  }
  return 0;
}

static int test_encode_decode_all(void) {
  int _sv0t0 = sv0_vec_new();
  int instrs = _sv0t0;
  sv0_vec_push(instrs, 3);
  sv0_vec_push(instrs, 16);
  sv0_vec_push(instrs, 64);
  sv0_vec_push(instrs, 112);
  sv0_vec_push(instrs, 10);
  sv0_vec_push(instrs, 144);
  sv0_vec_push(instrs, 1);
  sv0_vec_push(instrs, 2);
  sv0_vec_push(instrs, 3);
  sv0_vec_push(instrs, 161);
  sv0_vec_push(instrs, 4);
  sv0_vec_push(instrs, 8);
  int _sv0t1 = sv0_vec_new();
  int bytes = _sv0t1;
  int _sv0t2 = encode_all_insns(instrs, bytes);
  int bcount = _sv0t2;
  int _sv0t3 = (1 + 1);
  int _sv0t4 = (_sv0t3 + 1);
  int _sv0t5 = (_sv0t4 + 5);
  int _sv0t6 = (_sv0t5 + 13);
  int _sv0t7 = (_sv0t6 + 5);
  if ((bcount != _sv0t7)) {
    return 1;
  } else {
  }
  int _sv0t8 = sv0_vec_new();
  int decoded = _sv0t8;
  int _sv0t9 = decode_all_insns(bytes, bcount, decoded);
  int _sv0t10 = sv0_vec_len(decoded);
  int _sv0t11 = sv0_vec_len(instrs);
  if ((_sv0t10 != _sv0t11)) {
    return 2;
  } else {
  }
  int _sv0t12 = sv0_vec_get(decoded, 0);
  if ((_sv0t12 != 3)) {
    return 3;
  } else {
  }
  int _sv0t13 = sv0_vec_get(decoded, 1);
  if ((_sv0t13 != 16)) {
    return 4;
  } else {
  }
  int _sv0t14 = sv0_vec_get(decoded, 2);
  if ((_sv0t14 != 64)) {
    return 5;
  } else {
  }
  int _sv0t15 = sv0_vec_get(decoded, 3);
  if ((_sv0t15 != 112)) {
    return 6;
  } else {
  }
  int _sv0t16 = sv0_vec_get(decoded, 4);
  if ((_sv0t16 != 10)) {
    return 7;
  } else {
  }
  int _sv0t17 = sv0_vec_get(decoded, 5);
  if ((_sv0t17 != 144)) {
    return 8;
  } else {
  }
  int _sv0t18 = sv0_vec_get(decoded, 6);
  if ((_sv0t18 != 1)) {
    return 9;
  } else {
  }
  int _sv0t19 = sv0_vec_get(decoded, 7);
  if ((_sv0t19 != 2)) {
    return 10;
  } else {
  }
  int _sv0t20 = sv0_vec_get(decoded, 8);
  if ((_sv0t20 != 3)) {
    return 11;
  } else {
  }
  int _sv0t21 = sv0_vec_get(decoded, 9);
  if ((_sv0t21 != 161)) {
    return 12;
  } else {
  }
  int _sv0t22 = sv0_vec_get(decoded, 10);
  if ((_sv0t22 != 4)) {
    return 13;
  } else {
  }
  int _sv0t23 = sv0_vec_get(decoded, 11);
  if ((_sv0t23 != 8)) {
    return 14;
  } else {
  }
  return 0;
}

static int test_encode_file(void) {
  const char* source;
  source = "main";
  int _sv0t0 = sv0_vec_new();
  int starts = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int ends = _sv0t1;
  sv0_vec_push(starts, 0);
  sv0_vec_push(ends, 4);
  int _sv0t2 = sv0_vec_new();
  int pool = _sv0t2;
  sv0_vec_push(pool, 0);
  int _sv0t3 = sv0_vec_new();
  int str_buf = _sv0t3;
  int _sv0t4 = encode_strings(pool, source, starts, ends, str_buf);
  int str_len = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int code0 = _sv0t5;
  sv0_vec_push(code0, 0);
  int _sv0t6 = sv0_vec_new();
  int ft = _sv0t6;
  sv0_vec_push(ft, 0);
  sv0_vec_push(ft, 0);
  sv0_vec_push(ft, 1);
  sv0_vec_push(ft, code0);
  int _sv0t7 = sv0_vec_new();
  int out = _sv0t7;
  int _sv0t8 = encode_file(str_buf, str_len, ft, out);
  int total = _sv0t8;
  if ((total <= 0)) {
    return 1;
  } else {
  }
  int _sv0t9 = sv0_vec_get(out, 0);
  if ((_sv0t9 != 83)) {
    return 2;
  } else {
  }
  int _sv0t10 = sv0_vec_get(out, 1);
  if ((_sv0t10 != 86)) {
    return 3;
  } else {
  }
  int _sv0t11 = sv0_vec_get(out, 2);
  if ((_sv0t11 != 48)) {
    return 4;
  } else {
  }
  int _sv0t12 = sv0_vec_get(out, 3);
  if ((_sv0t12 != 66)) {
    return 5;
  } else {
  }
  return 0;
}

static int test_decode_file_roundtrip(void) {
  const char* source;
  source = "main add";
  int _sv0t0 = sv0_vec_new();
  int starts = _sv0t0;
  int _sv0t1 = sv0_vec_new();
  int ends = _sv0t1;
  sv0_vec_push(starts, 0);
  sv0_vec_push(ends, 4);
  sv0_vec_push(starts, 5);
  sv0_vec_push(ends, 8);
  int _sv0t2 = sv0_vec_new();
  int pool = _sv0t2;
  sv0_vec_push(pool, 0);
  sv0_vec_push(pool, 1);
  int _sv0t3 = sv0_vec_new();
  int str_buf = _sv0t3;
  int _sv0t4 = encode_strings(pool, source, starts, ends, str_buf);
  int str_len = _sv0t4;
  int _sv0t5 = sv0_vec_new();
  int code0 = _sv0t5;
  sv0_vec_push(code0, 4);
  sv0_vec_push(code0, 42);
  sv0_vec_push(code0, 118);
  sv0_vec_push(code0, 1);
  int _sv0t6 = sv0_vec_new();
  int code1 = _sv0t6;
  sv0_vec_push(code1, 0);
  int _sv0t7 = sv0_vec_new();
  int ft = _sv0t7;
  sv0_vec_push(ft, 0);
  sv0_vec_push(ft, 0);
  sv0_vec_push(ft, 2);
  sv0_vec_push(ft, code0);
  sv0_vec_push(ft, 1);
  sv0_vec_push(ft, 2);
  sv0_vec_push(ft, 0);
  sv0_vec_push(ft, code1);
  int _sv0t8 = sv0_vec_new();
  int out = _sv0t8;
  int _sv0t9 = encode_file(str_buf, str_len, ft, out);
  int _ef = _sv0t9;
  int _sv0t10 = sv0_vec_len(out);
  int buf_len = _sv0t10;
  int _sv0t11 = sv0_vec_new();
  int ds = _sv0t11;
  int _sv0t12 = sv0_vec_new();
  int dl = _sv0t12;
  int _sv0t13 = sv0_vec_new();
  int dft = _sv0t13;
  int _sv0t14 = decode_file(out, buf_len, ds, dl, dft);
  int rc = _sv0t14;
  if ((rc != 0)) {
    return 1;
  } else {
  }
  int _sv0t15 = sv0_vec_len(ds);
  if ((_sv0t15 != 2)) {
    return 2;
  } else {
  }
  int _sv0t16 = sv0_vec_get(dl, 0);
  if ((_sv0t16 != 4)) {
    return 3;
  } else {
  }
  int _sv0t17 = sv0_vec_get(ds, 0);
  int s0 = _sv0t17;
  int _sv0t18 = sv0_vec_get(out, s0);
  if ((_sv0t18 != 109)) {
    return 4;
  } else {
  }
  int _sv0t19 = (s0 + 3);
  int _sv0t20 = sv0_vec_get(out, _sv0t19);
  if ((_sv0t20 != 110)) {
    return 5;
  } else {
  }
  int _sv0t21 = sv0_vec_get(dl, 1);
  if ((_sv0t21 != 3)) {
    return 6;
  } else {
  }
  int _sv0t22 = sv0_vec_len(dft);
  if ((_sv0t22 != 8)) {
    return 7;
  } else {
  }
  int _sv0t23 = sv0_vec_get(dft, 0);
  if ((_sv0t23 != 0)) {
    return 8;
  } else {
  }
  int _sv0t24 = sv0_vec_get(dft, 1);
  if ((_sv0t24 != 0)) {
    return 9;
  } else {
  }
  int _sv0t25 = sv0_vec_get(dft, 2);
  if ((_sv0t25 != 2)) {
    return 10;
  } else {
  }
  int _sv0t26 = sv0_vec_get(dft, 3);
  int dc0 = _sv0t26;
  int _sv0t27 = sv0_vec_len(dc0);
  if ((_sv0t27 != 4)) {
    return 11;
  } else {
  }
  int _sv0t28 = sv0_vec_get(dc0, 0);
  if ((_sv0t28 != 4)) {
    return 12;
  } else {
  }
  int _sv0t29 = sv0_vec_get(dc0, 1);
  if ((_sv0t29 != 42)) {
    return 13;
  } else {
  }
  int _sv0t30 = sv0_vec_get(dc0, 2);
  if ((_sv0t30 != 118)) {
    return 14;
  } else {
  }
  int _sv0t31 = sv0_vec_get(dc0, 3);
  if ((_sv0t31 != 1)) {
    return 15;
  } else {
  }
  int _sv0t32 = sv0_vec_get(dft, 4);
  if ((_sv0t32 != 1)) {
    return 16;
  } else {
  }
  int _sv0t33 = sv0_vec_get(dft, 5);
  if ((_sv0t33 != 2)) {
    return 17;
  } else {
  }
  int _sv0t34 = sv0_vec_get(dft, 6);
  if ((_sv0t34 != 0)) {
    return 18;
  } else {
  }
  int _sv0t35 = sv0_vec_get(dft, 7);
  int dc1 = _sv0t35;
  int _sv0t36 = sv0_vec_len(dc1);
  if ((_sv0t36 != 1)) {
    return 19;
  } else {
  }
  int _sv0t37 = sv0_vec_get(dc1, 0);
  if ((_sv0t37 != 0)) {
    return 20;
  } else {
  }
  return 0;
}

static int test_logical_opcode_cluster(void) {
  int _sv0t0 = OP_AND();
  if ((_sv0t0 != 80)) {
    return 1;
  } else {
  }
  int _sv0t1 = OP_OR();
  if ((_sv0t1 != 81)) {
    return 2;
  } else {
  }
  int _sv0t2 = OP_NOT();
  if ((_sv0t2 != 82)) {
    return 3;
  } else {
  }
  int _sv0t3 = OP_EQ();
  if ((_sv0t3 != 64)) {
    return 4;
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
  int _sv0t3 = test_logical_opcode_cluster();
  int r2b = _sv0t3;
  if ((r2b != 0)) {
    int _sv0t4 = (15 + r2b);
    return _sv0t4;
  } else {
  }
  int _sv0t5 = test_insn_sizes();
  int r3 = _sv0t5;
  if ((r3 != 0)) {
    int _sv0t6 = (30 + r3);
    return _sv0t6;
  } else {
  }
  int _sv0t7 = test_opcode_classify();
  int r4 = _sv0t7;
  if ((r4 != 0)) {
    int _sv0t8 = (50 + r4);
    return _sv0t8;
  } else {
  }
  int _sv0t9 = test_le_bytes();
  int r5 = _sv0t9;
  if ((r5 != 0)) {
    int _sv0t10 = (70 + r5);
    return _sv0t10;
  } else {
  }
  int _sv0t11 = test_file_layout();
  int r6 = _sv0t11;
  if ((r6 != 0)) {
    int _sv0t12 = (80 + r6);
    return _sv0t12;
  } else {
  }
  int _sv0t13 = test_encode_le();
  int r7 = _sv0t13;
  if ((r7 != 0)) {
    int _sv0t14 = (90 + r7);
    return _sv0t14;
  } else {
  }
  int _sv0t15 = test_encode_header();
  int r8 = _sv0t15;
  if ((r8 != 0)) {
    int _sv0t16 = (110 + r8);
    return _sv0t16;
  } else {
  }
  int _sv0t17 = test_decode_at();
  int r9 = _sv0t17;
  if ((r9 != 0)) {
    int _sv0t18 = (120 + r9);
    return _sv0t18;
  } else {
  }
  int _sv0t19 = test_verify_magic();
  int r10 = _sv0t19;
  if ((r10 != 0)) {
    int _sv0t20 = (130 + r10);
    return _sv0t20;
  } else {
  }
  int _sv0t21 = test_encode_decode_strings();
  int r11 = _sv0t21;
  if ((r11 != 0)) {
    int _sv0t22 = (140 + r11);
    return _sv0t22;
  } else {
  }
  int _sv0t23 = test_encode_string_literals();
  int r12 = _sv0t23;
  if ((r12 != 0)) {
    int _sv0t24 = (150 + r12);
    return _sv0t24;
  } else {
  }
  int _sv0t25 = test_encode_insn_simple();
  int r13 = _sv0t25;
  if ((r13 != 0)) {
    int _sv0t26 = (160 + r13);
    return _sv0t26;
  } else {
  }
  int _sv0t27 = test_encode_insn_payload();
  int r14 = _sv0t27;
  if ((r14 != 0)) {
    int _sv0t28 = (170 + r14);
    return _sv0t28;
  } else {
  }
  int _sv0t29 = test_encode_insn_multi();
  int r15 = _sv0t29;
  if ((r15 != 0)) {
    int _sv0t30 = (190 + r15);
    return _sv0t30;
  } else {
  }
  int _sv0t31 = test_decode_insn_simple();
  int r16 = _sv0t31;
  if ((r16 != 0)) {
    int _sv0t32 = (204 + r16);
    return _sv0t32;
  } else {
  }
  int _sv0t33 = test_decode_insn_payload();
  int r17 = _sv0t33;
  if ((r17 != 0)) {
    int _sv0t34 = (212 + r17);
    return _sv0t34;
  } else {
  }
  int _sv0t35 = test_encode_decode_roundtrip();
  int r18 = _sv0t35;
  if ((r18 != 0)) {
    int _sv0t36 = (222 + r18);
    return _sv0t36;
  } else {
  }
  int _sv0t37 = test_encode_decode_all();
  int r19 = _sv0t37;
  if ((r19 != 0)) {
    int _sv0t38 = (236 + r19);
    return _sv0t38;
  } else {
  }
  int _sv0t39 = test_encode_file();
  int r20 = _sv0t39;
  if ((r20 != 0)) {
    int _sv0t40 = (246 + r20);
    return _sv0t40;
  } else {
  }
  int _sv0t41 = test_decode_file_roundtrip();
  int r21 = _sv0t41;
  if ((r21 != 0)) {
    int _sv0t42 = (10 + r21);
    return _sv0t42;
  } else {
  }
  return 0;
}


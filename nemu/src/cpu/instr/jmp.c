#include "cpu/instr.h"

make_instr_func(jmp_near) {
    OPERAND rel;
    rel.type = OPR_IMM;
    rel.sreg = SREG_CS;
    rel.data_size = data_size;
    rel.addr = eip + 1;

    operand_read(&rel);

    int offset = sign_ext(rel.val, data_size);
    
    cpu.eip += offset;

    // thank Ting Xu from CS'17 for finding this bug
    print_asm_1("jmp", "", 1 + data_size / 8, &rel);

    return 1 + data_size / 8;
}

make_instr_func(jmp_short) {
    OPERAND rel;
    rel.type = OPR_IMM;
    rel.sreg = SREG_CS;
    rel.data_size = 8;
    rel.addr = eip + 1;

    operand_read(&rel);

    int offset = sign_ext(rel.val, 8);

    cpu.eip += offset;

    print_asm_1("jmp", "", 2, &rel);

    return 2;
}

make_instr_func(jmp_rm) {
    OPERAND rm;
    rm.data_size = data_size;
    modrm_rm(eip + 1, &rm);

    operand_read(&rm);

    cpu.eip = rm.val;

    print_asm_1("jmp", "", 1 + data_size / 8, &rm);

    return 0;
}

make_instr_func(jmp_ptr) {
    OPERAND ptr;
    ptr.data_size = data_size;
    ptr.type = OPR_IMM;
    ptr.sreg = SREG_CS;
    ptr.addr = eip + 1;

    operand_read(&ptr);

    cpu.eip = ptr.val;

    print_asm_1("jmp", "", 1 + data_size / 8, &ptr);

    return 0;
}
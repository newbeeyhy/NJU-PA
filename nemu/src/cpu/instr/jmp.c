#include "cpu/instr.h"

make_instr_func(jmp_near) {
    OPERAND rel;
    rel.type = OPR_IMM;
    rel.sreg = SREG_CS;
    rel.data_size = data_size;
    rel.addr = eip + 1;

    operand_read(&rel);

    int offset = sign_ext(rel.val, data_size);
    
    // thank Ting Xu from CS'17 for finding this bug
    print_asm_1("jmp", "", 1 + data_size / 8, &rel);

    cpu.eip += offset;

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

    print_asm_1("jmp", "", 2, &rel);

    cpu.eip += offset;

    return 2;
}

make_instr_func(jmp_rm) {
    OPERAND rm;
    rm.data_size = data_size;
    modrm_rm(eip + 1, &rm);

    operand_read(&rm);

    print_asm_1("jmp", "", 1 + data_size / 8, &rm);

    cpu.eip = rm.val;

    return 0;
}

make_instr_func(jmp_ptr) {
#ifdef IA32_SEG
    cpu.cs.val = paddr_read(cpu.eip + 5, 2);
    load_sreg(1);
#endif    

    OPERAND ptr;
    ptr.data_size = data_size;
    ptr.type = OPR_IMM;
    ptr.sreg = SREG_CS;
    ptr.addr = eip + 1;

    operand_read(&ptr);

    print_asm_1("jmp", "", 1 + data_size / 8, &ptr);

    cpu.eip = ptr.val;

    return 0;
}
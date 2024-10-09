#include "cpu/instr.h"

make_instr_func(call_instr_direct) {
    OPERAND rel;
    rel.addr = eip + 1;
    rel.data_size = data_size;
    rel.sreg = SREG_CS;
    rel.type = OPR_IMM;
    rel.val = vaddr_read(eip + 1, SREG_CS, data_size / 8);

    cpu.esp -= 4;
    vaddr_write(cpu.esp, SREG_SS, 4, eip + 1 + data_size / 8);
    cpu.eip = eip + 1 + data_size / 8 + rel.val;

    print_asm_1("call", "", 1 + data_size / 8, &rel);

    return 0;
}

make_instr_func(call_instr_indirect) {
    OPERAND rm;
    rm.data_size = data_size;
    int len = modrm_rm(eip + 1, &rm);
    operand_read(&rm);

    cpu.esp -= 4;
    vaddr_write(cpu.esp, SREG_SS, 4, eip + 1 + len);
    cpu.eip = rm.val;

    print_asm_1("call", "", 1 + len, &rm);

    return 0;
}
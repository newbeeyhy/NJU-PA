#include "cpu/instr.h"

make_instr_func(call_instr_direct) {
    OPERAND imm;
    imm.addr = eip + 1;
    imm.data_size = data_size;
    imm.sreg = SREG_CS;
    imm.type = OPR_IMM;
    
    operand_read(&imm);

    cpu.esp -= 4;
    vaddr_write(cpu.esp, SREG_SS, 4, eip + 1 + data_size / 8);
    cpu.eip = eip + 1 + data_size / 8 + imm.val;

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

    return 0;
}
#include "cpu/instr.h"

make_instr_func(ret_near) {
    cpu.eip = vaddr_read(cpu.esp, SREG_SS, data_size / 8);
    cpu.esp += data_size / 8;

    print_asm_0("ret", "", 1);

    return 0;
}

make_instr_func(ret_near_i16) {
    OPERAND imm;
    imm.data_size = 16;
    imm.type = OPR_IMM;
    imm.addr = cpu.eip + 1;
    imm.sreg = SREG_CS;

    operand_read(&imm);

    cpu.eip = vaddr_read(cpu.esp, SREG_SS, imm.val);
    cpu.esp += imm.val;

    print_asm_1("ret", "", 3, &imm);
    
    return 0;
}
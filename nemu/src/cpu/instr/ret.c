#include "cpu/instr.h"

make_instr_func(ret_near) {
    cpu.eip = vaddr_read(cpu.esp, SREG_SS, data_size / 8);
    cpu.esp += data_size / 8;
    return 0;
}

make_instr_func(ret_near_i16) {
    uint16_t imm16 = vaddr_read(cpu.eip + 1, SREG_CS, 2);
    cpu.eip = vaddr_read(cpu.esp, SREG_SS, 2);
    cpu.esp += 2 + imm16;
    return 0;
}
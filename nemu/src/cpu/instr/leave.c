#include "cpu/instr.h"

make_instr_func(leave) {
    cpu.esp = cpu.ebp;
    cpu.ebp = vaddr_read(cpu.esp, SREG_SS, data_size / 8);
    cpu.esp += data_size / 8;
    return 1;
}
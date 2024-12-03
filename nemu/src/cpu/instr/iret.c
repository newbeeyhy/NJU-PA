#include "cpu/instr.h"

static uint32_t pop() {
    uint32_t ret = vaddr_read(cpu.esp, SREG_SS, data_size / 8);
    cpu.esp += data_size / 8;
    return ret;
}

make_instr_func(iret) {
    cpu.eip = pop();
    cpu.cs.val = pop();
    load_sreg(1);
    cpu.eflags.val = pop();
    return 1;
}

#include "cpu/instr.h"

static uint32_t pop() {
    uint32_t ret = vaddr_read(cpu.esp, SREG_SS, data_size / 8);
    cpu.esp += data_size / 8;
    return ret;
}

make_instr_func(iret) {
    cpu.eip = pop();
    cpu.cs.val = pop();
    cpu.eflags.val = pop();
    cpu.es.val = pop();
    cpu.ds.val = pop();
    cpu.fs.val = pop();
    cpu.gs.val = pop();
    return 1;
}

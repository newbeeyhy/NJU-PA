#include "cpu/instr.h"

static uint32_t pop() {
    uint32_t ret = vaddr_read(cpu.esp, SREG_SS, 4);
    cpu.esp += 4;
    return ret;
}

make_instr_func(iret) {
    cpu.eip = pop();
    cpu.cs.val = pop();
    load_sreg(SREG_CS);
    cpu.eflags.val = pop();
    return 0;
}

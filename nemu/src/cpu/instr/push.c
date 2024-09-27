#include "cpu/instr.h"

instr_execute_1op() {
    uint32_t rel = paddr_read(eip + 1, data_size / 8);
    cpu.esp -= 4;
    paddr_write(cpu.esp, data_size / 8, rel);
}

make_instr_impl_1op(push, r, l)
#include "cpu/instr.h"

make_instr_func(call_instr_direct) {
    uint32_t rel = vaddr_read(eip + 1, SREG_CS, data_size / 8);

    cpu.esp -= 4;
    vaddr_write(cpu.esp, SREG_SS, 4, eip + 1 + data_size / 8);
    cpu.eip = eip + 1 + data_size / 8 + rel;

    print_asm_1("call", "", 1 + data_size / 8, &rel);

    return 0;
}
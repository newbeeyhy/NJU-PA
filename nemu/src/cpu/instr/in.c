#include "cpu/instr.h"
#include "device/port_io.h"

make_instr_func(in_b) {
    uint8_t data = pio_read(cpu.gpr[2]._16, 1);
    cpu.gpr[0]._8[0] = data;

    print_asm_0("in", "", 1);

    return 1;
}

make_instr_func(in_v) {
    uint32_t data = pio_read(cpu.gpr[2]._16, data_size / 8);
    if (data_size == 16) {
        cpu.gpr[0]._16 = data;
    } else {
        cpu.eax = data;
    }

    print_asm_0("in", "", 1);

    return 1;
}


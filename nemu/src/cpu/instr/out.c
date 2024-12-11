#include "cpu/instr.h"
#include "device/port_io.h"

make_instr_func(out_b) {
    uint8_t data = cpu.gpr[0]._8[0];
    pio_write(cpu.gpr[2]._16, 1, data);

    print_asm_0("out", "", 1);

    return 1;
}

make_instr_func(out_v) {
    uint32_t data;
    if (data_size == 16) {
        data = cpu.gpr[0]._16;
    } else {
        data = cpu.eax;
    }
    pio_write(cpu.gpr[2]._16, data_size / 8, data);

    print_asm_0("out", "", 1);

    return 1;
}

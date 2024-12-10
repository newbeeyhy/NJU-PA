#include "cpu/instr.h"
#include "device/port_io.h"

make_instr_func(out_eax2i_b) {
    OPERAND i;
    i.data_size = 8;
    i.type = OPR_IMM;
	i.sreg = SREG_CS;
	i.addr = eip + 1;
	operand_read(&i);
    uint8_t data = cpu.eax & 0xff;
    write_io_port(i.val, 1, data);

    print_asm_1("out", "", 2, &i);

    return 2;
}

make_instr_func(out_eax2i_v) {
    OPERAND i;
    i.data_size = 8;
    i.type = OPR_IMM;
	i.sreg = SREG_CS;
	i.addr = eip + 1;
	operand_read(&i);
    uint32_t data = cpu.eax;
    write_io_port(i.val, 4, data);

    print_asm_1("out", "", 2, &i);

    return 2;
}

make_instr_func(out_eax2edx_b) {
    uint8_t data = cpu.eax & 0xff;
    write_io_port(cpu.edx, 1, data);

    print_asm_0("out", "", 1);

    return 1;
}

make_instr_func(out_eax2edx_v) {
    uint32_t data = cpu.eax;
    write_io_port(cpu.edx, 4, data);

    print_asm_0("out", "", 1);

    return 1;
}

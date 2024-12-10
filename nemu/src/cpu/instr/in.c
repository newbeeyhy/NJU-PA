#include "cpu/instr.h"
#include "device/port_io.h"

make_instr_func(in_i2eax_b) {
    OPERAND i;
    i.data_size = 8;
    i.type = OPR_IMM;
	i.sreg = SREG_CS;
	i.addr = eip + 1;
	operand_read(&i);
    uint8_t data = read_io_port(i.val, 1);
    cpu.eax = data;

    print_asm_1("in", "", 2, &i);

    return 2;
}

make_instr_func(in_i2eax_v) {
    OPERAND i;
    i.data_size = 8;
    i.type = OPR_IMM;
	i.sreg = SREG_CS;
	i.addr = eip + 1;
	operand_read(&i);
    uint32_t data = read_io_port(i.val, 4);
    cpu.eax = data;

    print_asm_1("in", "", 2, &i);

    return 2;
}

make_instr_func(in_edx2eax_b) {
    uint8_t data = read_io_port(cpu.edx, 1);
    cpu.eax = data;

    print_asm_0("in", "", 1);

    return 1;
}

make_instr_func(in_edx2eax_v) {
    uint32_t data = read_io_port(cpu.edx, 4);
    cpu.eax = data;

    print_asm_0("in", "", 1);

    return 1;
}


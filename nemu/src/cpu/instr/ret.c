#include "cpu/instr.h"

make_instr_func(ret_near) {
    cpu.eip = vaddr_read(cpu.esp, SREG_SS, data_size / 8);
    cpu.esp += data_size / 8;

    print_asm_0("ret", "", 1);

    return 0;
}

make_instr_func(ret_near_i16) {
    uint32_t dest_addr = vaddr_read(cpu.esp, SREG_CS, 4);
	cpu.esp += data_size / 8;

	cpu.eip = dest_addr;
	
	uint16_t imm = vaddr_read(eip + 1, SREG_CS, 2);
	cpu.esp += imm;

    print_asm_1("ret", "", 3, &imm);
    
    return 0;
}
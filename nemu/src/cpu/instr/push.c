#include "cpu/instr.h"

static void instr_execute_1op() {
    uint32_t old_esp = cpu.esp;
	cpu.esp -= data_size / 8;
	operand_read(&opr_src);
	if (opr_src.addr == 0x4) {
		vaddr_write(cpu.esp, SREG_CS, (data_size / 8), old_esp);
	} else {
		vaddr_write(cpu.esp, SREG_CS, (data_size / 8), opr_src.val);
    }
}

static void push(uint32_t dest) {
    cpu.esp -= 4;
    vaddr_write(cpu.esp, SREG_SS, 4, dest);
}

make_instr_impl_1op(push, r, v)
make_instr_impl_1op(push, i, v)
make_instr_impl_1op(push, i, b)
make_instr_impl_1op(push, rm, v)
make_instr_impl_1op(push, cs, w)
make_instr_impl_1op(push, ss, w)
make_instr_impl_1op(push, ds, w)
make_instr_impl_1op(push, es, w)
make_instr_impl_1op(push, fs, w)
make_instr_impl_1op(push, gs, w)

make_instr_func(pusha) {
    uint32_t temp = cpu.esp;
    push(cpu.eax);
    push(cpu.ecx);
    push(cpu.edx);
    push(cpu.ebx);
    push(temp);
    push(cpu.ebp);
    push(cpu.esi);
    push(cpu.edi);
    
    print_asm_0("pusha", "", 1);

    return 1;
}
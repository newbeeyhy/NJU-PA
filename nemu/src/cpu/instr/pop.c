#include "cpu/instr.h"

static void instr_execute_1op() {
    opr_src.val = vaddr_read(cpu.esp, SREG_SS, data_size / 8);
    cpu.esp += data_size / 8;
    operand_write(&opr_src);
}

static uint32_t pop() {
    uint32_t ret = vaddr_read(cpu.esp, SREG_SS, data_size / 8);
    cpu.esp += data_size / 8;
    return ret;
}

make_instr_impl_1op(pop, r, v)
make_instr_impl_1op(pop, rm, v)
make_instr_impl_1op(pop, ss, w)
make_instr_impl_1op(pop, ds, w)
make_instr_impl_1op(pop, es, w)
make_instr_impl_1op(pop, fs, w)
make_instr_impl_1op(pop, gs, w)

make_instr_func(popa) {
    cpu.edi = pop();
    cpu.esi = pop();
    cpu.ebp = pop();
    cpu.esp += data_size / 8;
    cpu.ebx = pop();
    cpu.edx = pop();
    cpu.ecx = pop();
    cpu.eax = pop();
    
    print_asm_0("popa", "", 1);

    return 1;
}
#include "cpu/instr.h"

static void instr_execute_1op() {
    operand_read(&opr_src);
    cpu.esp -= data_size / 8;
    vaddr_write(cpu.esp, SREG_SS, data_size / 8, opr_src.val);
}

static void push(uint32_t dest) {
    cpu.esp -= data_size / 8;
    vaddr_write(cpu.esp, SREG_SS, data_size / 8, dest);
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
    push(cpu.edi);
    push(cpu.esi);
    push(cpu.ebp);
    cpu.esp -= data_size / 8;
    push(cpu.ebx);
    push(cpu.edx);
    push(cpu.ecx);
    push(cpu.eax);
    
    print_asm_0("pusha", "", 1);

    return 1;
}
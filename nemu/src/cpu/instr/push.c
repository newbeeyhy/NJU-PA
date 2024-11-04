#include "cpu/instr.h"

static void instr_execute_1op() {
    operand_read(&opr_src);
    cpu.esp -= data_size / 8;
    vaddr_write(cpu.esp, SREG_SS, data_size / 8, opr_src.val);
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

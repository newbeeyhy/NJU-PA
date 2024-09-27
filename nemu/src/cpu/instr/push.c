#include "cpu/instr.h"

instr_execute_1op() {
    operand_read(&opr_src);
    cpu.esp -= data_size / 8;
    paddr_write(cpu.esp, data_size / 8, opr_src.val);
}

make_instr_impl_1op(push, r, l)
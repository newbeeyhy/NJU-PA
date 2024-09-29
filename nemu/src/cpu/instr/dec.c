#include "cpu/instr.h"

static void instr_execute_1op() {
    operand_read(&opr_dest);
    opr_dest.val = alu_sub(1, opr_dest.val, data_size);
    operand_write(&opr_dest);
}

make_instr_impl_1op(dec, r, v)
#include "cpu/intr.h"
#include "cpu/instr.h"

static void instr_execute_1op() {
    operand_read(&opr_src);
    raise_sw_intr(opr_src.val);
}

make_instr_impl_1op(int_, i, b)

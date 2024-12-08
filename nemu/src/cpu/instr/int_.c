#include "cpu/intr.h"
#include "cpu/instr.h"

make_instr_func(int__i_b) {
    OPERAND i;
    i.data_size = 8;
    i.type = OPR_IMM;
	i.sreg = SREG_CS;
	i.addr = eip + 1;
	operand_read(&i);
    raise_sw_intr(i.val);
    return 0;
}

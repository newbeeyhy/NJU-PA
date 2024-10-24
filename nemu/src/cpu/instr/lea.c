#include "cpu/instr.h"

make_instr_func(lea) {
    OPERAND r, rm;

    r.data_size = rm.data_size = data_size;

    int len = 1;
    len += modrm_r_rm(cpu.eip + 1, &r, &rm);
    r.val = rm.addr;

    operand_write(&r);

    print_asm_2("lea", "", len, &rm, &r);

    return len;
}
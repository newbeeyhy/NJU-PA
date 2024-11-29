#include "cpu/instr.h"

make_instr_func(lgdt) {
#ifdef IA32_SEG
    int len = 1;
    OPERAND m;

    m.data_size = 16;
    len += modrm_rm(eip + 1, &m);

    operand_read(&m);
    cpu.gdtr.limit = m.val;

    m.data_size = 32;
    m.addr += 2;

    operand_read(&m);
    cpu.gdtr.base = m.val;

    print_asm_0("lgdt", "", len);

    return len;
#else
    assert(0);
#endif
}
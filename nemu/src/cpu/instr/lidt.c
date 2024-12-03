#include "cpu/instr.h"

make_instr_func(lidt) {
#ifdef IA32_INTR
    int len = 1;
    OPERAND m;

    m.data_size = 16;
    len += modrm_rm(eip + 1, &m);

    operand_read(&m);
    cpu.idtr.limit = m.val;

    m.data_size = 32;
    m.addr += 2;

    operand_read(&m);
    cpu.idtr.base = m.val;

    print_asm_0("lidt", "", len);

    return len;
#else
    assert(0);
#endif
}

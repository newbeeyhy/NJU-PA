#include "cpu/instr.h"

static void instr_execute_1op() {
    operand_read(&opr_src);
    cpu.esp -= data_size / 8;
    vaddr_write(cpu.esp, SREG_SS, data_size / 8, opr_src.val);
}

make_instr_impl_1op(push, r, v)
make_instr_impl_1op(push, i, v)
make_instr_impl_1op(push, i, b)

make_instr_func(push_es) {
    OPERAND r;

    r.type = OPR_SREG;
    r.sreg = SREG_ES;
    r.addr = 0;
    r.data_size = 16;

    operand_read(&r);
    cpu.esp -= 2;
    vaddr_write(cpu.esp, SREG_SS, 2, r.val);

    print_asm_1("push", "", 3, &r);

    return 3;
}

make_instr_func(push_cs) {
    OPERAND r;

    r.type = OPR_SREG;
    r.sreg = SREG_CS;
    r.addr = 1;
    r.data_size = 16;

    operand_read(&r);
    cpu.esp -= 2;
    vaddr_write(cpu.esp, SREG_SS, 2, r.val);

    print_asm_1("push", "", 3, &r);

    return 3;
}

make_instr_func(push_ss) {
    OPERAND r;

    r.type = OPR_SREG;
    r.sreg = SREG_SS;
    r.addr = 2;
    r.data_size = 16;

    operand_read(&r);
    cpu.esp -= 2;
    vaddr_write(cpu.esp, SREG_SS, 2, r.val);

    print_asm_1("push", "", 3, &r);

    return 3;
}

make_instr_func(push_ds) {
    OPERAND r;

    r.type = OPR_SREG;
    r.sreg = SREG_DS;
    r.addr = 3;
    r.data_size = 16;

    operand_read(&r);
    cpu.esp -= 2;
    vaddr_write(cpu.esp, SREG_SS, 2, r.val);

    print_asm_1("push", "", 3, &r);

    return 3;
}

make_instr_func(push_fs) {
    OPERAND r;

    r.type = OPR_SREG;
    r.sreg = SREG_FS;
    r.addr = 4;
    r.data_size = 16;

    operand_read(&r);
    cpu.esp -= 2;
    vaddr_write(cpu.esp, SREG_SS, 2, r.val);

    print_asm_1("push", "", 3, &r);

    return 3;
}

make_instr_func(push_gs) {
    OPERAND r;

    r.type = OPR_SREG;
    r.sreg = SREG_GS;
    r.addr = 5;
    r.data_size = 16;

    operand_read(&r);
    cpu.esp -= 2;
    vaddr_write(cpu.esp, SREG_SS, 2, r.val);

    print_asm_1("push", "", 3, &r);

    return 3;
}
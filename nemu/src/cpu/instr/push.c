#include "cpu/instr.h"

instr_execute_1op() {
    operand_read(&opr_src);
    cpu.esp -= data_size / 8;
    vaddr_write(cpu.esp, SREG_SS, data_size / 8, opr_src.val);
}

make_instr_impl_1op(push, r, l)

make_instr_func(push_es) {
    OPERAND r;

    r.type = OPR_SREG;
    r.addr = 0;
    r.data_size = 16;

    operand_read(&r);
    cpu.esp -= 2;
    vaddr_write(cpu.esp, SREG_SS, 2, r.val);

    print_asm_1("push", "", len, &r);

    return 0;
}

make_instr_func(push_cs) {
    OPERAND r;

    r.type = OPR_SREG;
    r.addr = 1;
    r.data_size = 16;

    operand_read(&r);
    cpu.esp -= 2;
    vaddr_write(cpu.esp, SREG_SS, 2, r.val);

    print_asm_1("push", "", len, &r);

    return 0;
}

make_instr_func(push_ss) {
    OPERAND r;

    r.type = OPR_SREG;
    r.addr = 2;
    r.data_size = 16;

    operand_read(&r);
    cpu.esp -= 2;
    vaddr_write(cpu.esp, SREG_SS, 2, r.val);

    print_asm_1("push", "", len, &r);

    return 0;
}

make_instr_func(push_ds) {
    OPERAND r;

    r.type = OPR_SREG;
    r.addr = 3;
    r.data_size = 16;

    operand_read(&r);
    cpu.esp -= 2;
    vaddr_write(cpu.esp, SREG_SS, 2, r.val);

    print_asm_1("push", "", len, &r);

    return 0;
}

make_instr_func(push_fs) {
    OPERAND r;

    r.type = OPR_SREG;
    r.addr = 4;
    r.data_size = 16;

    operand_read(&r);
    cpu.esp -= 2;
    vaddr_write(cpu.esp, SREG_SS, 2, r.val);

    print_asm_1("push", "", len, &r);

    return 0;
}

make_instr_func(push_gs) {
    OPERAND r;

    r.type = OPR_SREG;
    r.addr = 5;
    r.data_size = 16;

    operand_read(&r);
    cpu.esp -= 2;
    vaddr_write(cpu.esp, SREG_SS, 2, r.val);

    print_asm_1("push", "", len, &r);

    return 0;
}
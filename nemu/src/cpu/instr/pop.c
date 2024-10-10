#include "cpu/instr.h"

static void instr_execute_1op() {
    operand_read(&opr_dest);
    opr_dest.val = vaddr_read(cpu.esp, SREG_SS, data_size / 8);
    cpu.esp += data_size / 8;
    operand_write(&opr_dest);
}

make_instr_impl_1op(pop, r, v)
make_instr_impl_1op(pop, rm, v)

make_instr_func(pop_es) {
    OPERAND r;

    r.type = OPR_SREG;
    r.sreg = SREG_ES;
    r.addr = 0;
    r.data_size = 16;

    operand_read(&r);
    r.val = vaddr_read(cpu.esp, SREG_SS, 2);
    cpu.esp += 2;
    operand_write(&r);

    print_asm_1("pop", "", 3, &r);

    return 3;
}

make_instr_func(pop_ss) {
    OPERAND r;

    r.type = OPR_SREG;
    r.sreg = SREG_SS;
    r.addr = 2;
    r.data_size = 16;

    operand_read(&r);
    r.val = vaddr_read(cpu.esp, SREG_SS, 2);
    cpu.esp += 2;
    operand_write(&r);

    print_asm_1("pop", "", 3, &r);

    return 3;
}

make_instr_func(pop_ds) {
    OPERAND r;

    r.type = OPR_SREG;
    r.sreg = SREG_DS;
    r.addr = 3;
    r.data_size = 16;

    operand_read(&r);
    r.val = vaddr_read(cpu.esp, SREG_SS, 2);
    cpu.esp += 2;
    operand_write(&r);

    print_asm_1("pop", "", 3, &r);

    return 3;
}

make_instr_func(pop_fs) {
    OPERAND r;

    r.type = OPR_SREG;
    r.sreg = SREG_FS;
    r.addr = 4;
    r.data_size = 16;

    operand_read(&r);
    r.val = vaddr_read(cpu.esp, SREG_SS, 2);
    cpu.esp += 2;
    operand_write(&r);

    print_asm_1("pop", "", 3, &r);

    return 3;
}

make_instr_func(pop_gs) {
    OPERAND r;

    r.type = OPR_SREG;
    r.sreg = SREG_GS;
    r.addr = 5;
    r.data_size = 16;

    operand_read(&r);
    r.val = vaddr_read(cpu.esp, SREG_SS, 2);
    cpu.esp += 2;
    operand_write(&r);

    print_asm_1("pop", "", 3, &r);

    return 3;
}
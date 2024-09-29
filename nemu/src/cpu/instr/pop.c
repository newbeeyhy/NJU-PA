#include "cpu/instr.h"

static void instr_execute_1op() {
    operand_read(&opr_dest);
    opr_dest.val = vaddr_read(cpu.esp, SREG_SS, data_size / 8);
    cpu.esp += data_size / 8;
    operand_write(&opr_dest);
}

make_instr_impl_1op(pop, r, l)
make_instr_impl_1op(pop, rm, v)

make_instr_func(pop_es) {
    OPERAND r;

    r.type = OPR_SREG;
    r.addr = 0;
    r.data_size = 16;

    operand_read(&r);
    r.val = vaddr_read(cpu.esp, SREG_SS, 2);
    cpu.esp += 2;
    operand_write(&r);

    print_asm_1("pop", "", len, &r);

    return 3;
}

make_instr_func(pop_ss) {
    OPERAND r;

    r.type = OPR_SREG;
    r.addr = 2;
    r.data_size = 16;

    operand_read(&r);
    r.val = vaddr_read(cpu.esp, SREG_SS, 2);
    cpu.esp += 2;
    operand_write(&r);

    print_asm_1("pop", "", len, &r);

    return 3;
}

make_instr_func(pop_ds) {
    OPERAND r;

    r.type = OPR_SREG;
    r.addr = 3;
    r.data_size = 16;

    operand_read(&r);
    r.val = vaddr_read(cpu.esp, SREG_SS, 2);
    cpu.esp += 2;
    operand_write(&r);

    print_asm_1("pop", "", len, &r);

    return 3;
}
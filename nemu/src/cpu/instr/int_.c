#include "cpu/intr.h"
#include "cpu/instr.h"

static void push(uint32_t dest) {
    cpu.esp -= data_size / 8;
    vaddr_write(cpu.esp, SREG_SS, data_size / 8, dest);
}

static void instr_execute_1op() {
    operand_read(&opr_src);
    push(cpu.eflags.val);
    cpu.eflags.IF = 0;
    cpu.eflags.TF = 0;
    push(cpu.cs.val);
    push(cpu.eip);
    GateDesc gate;
    gate.val[0] = laddr_read(cpu.idtr.base + opr_src.val * 4, 4);
    gate.val[1] = laddr_read(cpu.idtr.base + opr_src.val * 4 + 4, 4);
    cpu.cs.val = gate.selector;
    cpu.eip = (gate.offset_31_16 << 16) | gate.offset_15_0;
}

make_instr_impl_1op(int_, i, b)

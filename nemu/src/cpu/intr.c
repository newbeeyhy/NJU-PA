#include "cpu/intr.h"
#include "cpu/instr.h"
#include "memory/memory.h"

static void push(uint32_t dest) {
    cpu.esp -= data_size / 8;
    vaddr_write(cpu.esp, SREG_SS, data_size / 8, dest);
}

void raise_intr(uint8_t intr_no)
{
#ifdef IA32_INTR
    push(cpu.eflags.val);
    push(cpu.cs.val);
    push(cpu.eip);
    GateDesc gate;
    gate.val[0] = laddr_read(cpu.idtr.base + intr_no * 4, 4);
    gate.val[1] = laddr_read(cpu.idtr.base + intr_no * 4 + 4, 4);
    if (gate.type == 0xe) {
        cpu.eflags.IF = 0;
    }
    cpu.cs.val = gate.selector;
	load_sreg(1);
	uint32_t offset = (gate.offset_31_16 << 16) | gate.offset_15_0;
    cpu.eip = vaddr_read(cpu.cs.base + offset, SREG_CS, 4);
#endif
}

void raise_sw_intr(uint8_t intr_no)
{
	// return address is the next instruction
	cpu.eip += 2;
	raise_intr(intr_no);
}

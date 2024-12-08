#include "cpu/intr.h"
#include "cpu/instr.h"
#include "memory/memory.h"

#ifdef IA32_INTR
static void push(uint32_t dest) {
    cpu.esp -= 4;
    vaddr_write(cpu.esp, SREG_SS, 4, dest);
}
#endif

void raise_intr(uint8_t intr_no)
{
#ifdef IA32_INTR
    push(cpu.eflags.val);
    push(cpu.cs.val);
    push(cpu.eip);
    GateDesc gate;
    gate.val[0] = laddr_read(cpu.idtr.base + intr_no * 8, 4);
    gate.val[1] = laddr_read(cpu.idtr.base + intr_no * 8 + 4, 4);
    if (gate.type == 0xe) {
        cpu.eflags.IF = 0;
    };
    cpu.cs.val = gate.selector;
    load_sreg(SREG_CS);
	uint32_t offset = (gate.offset_31_16 << 16) | gate.offset_15_0;
    cpu.eip = segment_translate(offset, SREG_CS);
#endif
}

void raise_sw_intr(uint8_t intr_no)
{
	// return address is the next instruction
	cpu.eip += 2;
	raise_intr(intr_no);
}

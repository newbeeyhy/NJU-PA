#include "cpu/intr.h"
#include "cpu/instr.h"
#include "memory/memory.h"

void raise_intr(uint8_t intr_no)
{
#ifdef IA32_INTR
	// TODO: Trigger an exception/interrupt with 'intr_no'
    // TODO: 'intr_no' is the index to the IDT
    // TODO: Push EFLAGS, CS, and EIP
    // TODO: Find the IDT entry using 'intr_no'
    // TODO: Clear IF if it is an interrupt
    // TODO: Set EIP to the entry of the interrupt handler
#endif
}

void raise_sw_intr(uint8_t intr_no)
{
	// return address is the next instruction
	cpu.eip += 2;
	raise_intr(intr_no);
}

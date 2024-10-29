#include "cpu/cpu.h"
#include "memory/memory.h"

// return the linear address from the virtual address and segment selector
uint32_t segment_translate(uint32_t offset, uint8_t sreg) {
	/* TODO: perform segment translation from virtual address to linear address
	 * by reading the invisible part of the segment register 'sreg'
	 */
	return cpu.segReg[sreg].base + offset;
}

// load the invisible part of a segment register
void load_sreg(uint8_t sreg) {
	/* TODO: load the invisibile part of the segment register 'sreg' by reading the GDT.
	 * The visible part of 'sreg' should be assigned by mov or ljmp already.
	 */
	uint32_t gdtr_base = cpu.gdtr.base;
	uint32_t index = cpu.segReg[sreg].index;
	SegDesc segDesc;
	segDesc.val[0] = laddr_read(gdtr_base, index * 8, 4);
	segDesc.val[1] = laddr_read(gdtr_base, index * 8 + 4, 4);
	cpu.segReg[sreg].base = (segDesc.base_31_24 << 24) | (segDesc.base_23_16 << 16) | segDesc.base_15_0;
	cpu.segReg[sreg].limit = (segDesc.limit_19_16 << 16) | segDesc.limit_15_0;
	cpu.segReg[sreg].type = segDesc.type;
	cpu.segReg[sreg].privilege_level = segDesc.privilege_level;
	cpu.segReg[sreg].soft_use = segDesc.soft_use;
	assert(cpu.segReg[sreg].base == 0);
	assert(cpu.segReg[sreg].limit == 0xfffff);
	assert(segDesc.granularity == 1);
}

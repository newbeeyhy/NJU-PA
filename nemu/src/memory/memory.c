#include "nemu.h"
#include "cpu/cpu.h"
#include "memory/memory.h"
#include "device/mm_io.h"
#include <memory.h>
#include <stdio.h>

uint8_t hw_mem[MEM_SIZE_B];

uint32_t hw_mem_read(paddr_t paddr, size_t len) {
	uint32_t ret = 0;
	memcpy(&ret, hw_mem + paddr, len);
	return ret;
}

void hw_mem_write(paddr_t paddr, size_t len, uint32_t data) {
	memcpy(hw_mem + paddr, &data, len);
}

#ifdef HAS_DEVICE_VGA
uint32_t paddr_read_io(paddr_t paddr, size_t len, int map_NO) {
	return mmio_read(paddr, len, map_NO);
}
#endif

uint32_t paddr_read_mm(paddr_t paddr, size_t len) {
	uint32_t ret = 0;
#ifdef CACHE_ENABLED
	ret = cache_read(paddr, len);
	uint32_t tmp = hw_mem_read(paddr, len);
	assert(ret == tmp);
#else
	ret = hw_mem_read(paddr, len);
#endif
	return ret;
}

#ifdef HAS_DEVICE_VGA
void paddr_write_io(paddr_t paddr, size_t len, uint32_t data, int map_NO) {
	mmio_write(paddr, len, data, map_NO);
}
#endif

void paddr_write_mm(paddr_t paddr, size_t len, uint32_t data) {
#ifdef CACHE_ENABLED
	cache_write(paddr, len, data);
	uint32_t tmp = hw_mem_read(paddr, len);
	assert(data == tmp);
#else
	hw_mem_write(paddr, len, data);
#endif
}

uint32_t paddr_read(paddr_t paddr, size_t len) {
	uint32_t ret;
#ifdef HAS_DEVICE_VGA
	int map_NO = is_mmio(paddr);
	if (map_NO != -1) {
		ret = paddr_read_io(paddr, len, map_NO);
	} else {
		ret = paddr_read_mm(paddr, len);
	}
#else
	ret = paddr_read_mm(paddr, len);
#endif
	return ret;
}

void paddr_write(paddr_t paddr, size_t len, uint32_t data) {
#ifdef HAS_DEVICE_VGA
	int map_NO = is_mmio(paddr);
	if (map_NO != -1) {
		paddr_write_io(paddr, len, data, map_NO);
	} else {
		paddr_write_mm(paddr, len, data);
	}
#else
	paddr_write_mm(paddr, len, data);
#endif
}

uint32_t laddr_read(laddr_t laddr, size_t len) {
#ifdef IA32_PAGE
	if (cpu.cr0.pg == 1) {
		uint32_t ret = 0;
		uint32_t offset = laddr & 0xfff;
		if (offset + len > 0x1000) {
			for (uint32_t i = 0; i < len; i++) {
				uint32_t laddr_i = page_translate(offset + i);
				ret |= paddr_read(laddr_i, 1) << (8 * i);
			}
		} else {
			laddr = page_translate(laddr);
			ret = paddr_read(laddr, len);
		}
		return ret;
	}
#endif
	return paddr_read(laddr, len);
}

void laddr_write(laddr_t laddr, size_t len, uint32_t data) {
#ifdef IA32_PAGE
	if (cpu.cr0.pg == 1) {
		uint32_t offset = laddr & 0xfff;
		if (offset + len > 0x1000) {
			for (uint32_t i = 0; i < len; i++) {
				uint32_t laddr_i = page_translate(offset + i);
				paddr_write(laddr_i, 1, data >> (8 * i) & 0xff);
			}
		} else {
			laddr = page_translate(laddr);
			paddr_write(laddr, len, data);
		}
		return;
	}
#endif
	paddr_write(laddr, len, data);
}

uint32_t vaddr_read(vaddr_t vaddr, uint8_t sreg, size_t len) {
	assert(len == 1 || len == 2 || len == 4);
#ifdef IA32_SEG
	if (cpu.cr0.pe == 1) {
		vaddr = segment_translate(vaddr, sreg);
	}
#endif
	return laddr_read(vaddr, len);
}

void vaddr_write(vaddr_t vaddr, uint8_t sreg, size_t len, uint32_t data) {
	assert(len == 1 || len == 2 || len == 4);
#ifdef IA32_SEG
	if (cpu.cr0.pe == 1) {
		vaddr = segment_translate(vaddr, sreg);
	}
#endif
	laddr_write(vaddr, len, data);
}

void init_mem() {
	// clear the memory on initiation
	memset(hw_mem, 0, MEM_SIZE_B);
#ifdef CACHE_ENABLED
	init_cache();
#endif
#ifdef TLB_ENABLED
	make_all_tlb();
	init_all_tlb();
#endif
}

uint32_t instr_fetch(vaddr_t vaddr, size_t len) {
	assert(len == 1 || len == 2 || len == 4);
	return vaddr_read(vaddr, SREG_CS, len);
}

uint8_t *get_mem_addr() {
	return hw_mem;
}

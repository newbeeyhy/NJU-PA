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

uint32_t paddr_read(paddr_t paddr, size_t len) {
	assert(len == 1 || len == 2 || len == 4);
	uint32_t ret = 0;
#ifdef CACHE_ENABLED
	ret = cache_read(paddr, len);     // 通过cache进行读
	uint32_t tmp = hw_mem_read(paddr, len);
	assert(ret == tmp);		// cache和内存中的数据应该一致
#else
	ret = hw_mem_read(paddr, len);
#endif
	return ret;
}

void paddr_write(paddr_t paddr, size_t len, uint32_t data) {
	assert(len == 1 || len == 2 || len == 4);
#ifdef CACHE_ENABLED
	cache_write(paddr, len, data);    // 通过cache进行写
	uint32_t tmp = hw_mem_read(paddr, len);
	assert(data == tmp);	// cache和内存中的数据应该一致
#else
	hw_mem_write(paddr, len, data);
#endif
}

uint32_t laddr_read(laddr_t laddr, size_t len) {
	assert(len == 1 || len == 2 || len == 4);
#ifdef IA32_PAGE
	if (cpu.cr0.pe == 1) {
		uint32_t ret = 0;
		uint32_t offset = laddr & 0xfff;
		if (offset + len > 0x1000) {
			uint32_t laddr1 = laddr , laddr2 = (laddr + 0x1000 - offset);
			size_t len1 = 0x1000 - offset, len2 = len - len1;
			laddr1 = page_translate(laddr1);
			laddr2 = page_translate(laddr2);
			ret |= paddr_read(laddr1, len1);
			ret |= paddr_read(laddr2, len2) << (len1 << 3);
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
	assert(len == 1 || len == 2 || len == 4);
#ifdef IA32_PAGE
	if (cpu.cr0.pe == 1) {
		uint32_t offset = laddr & 0xfff;
		if (offset + len > 0x1000) {
			uint32_t laddr1 = laddr , laddr2 = (laddr + 0x1000 - offset);
			size_t len1 = 0x1000 - offset, len2 = len - len1;
			laddr1 = page_translate(laddr1);
			laddr2 = page_translate(laddr2);
			paddr_write(laddr1, len1, data & ((1 << (len1 << 3)) - 1));
			paddr_write(laddr2, len2, data >> (len1 << 3));
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
	init_cache();                             // 初始化cache
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

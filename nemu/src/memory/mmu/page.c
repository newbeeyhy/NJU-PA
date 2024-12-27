#include "cpu/cpu.h"
#include "memory/memory.h"

// translate from linear address to physical address
paddr_t page_translate(laddr_t laddr) {
#ifndef TLB_ENABLED
#ifdef IA32_PAGE
	uint32_t dir = (laddr >> 22) & 0x3ff;
	uint32_t page = (laddr >> 12) & 0x3ff;
	uint32_t offset = laddr & 0xfff;
	PDE pdir;
	pdir.val = paddr_read((cpu.cr3.page_directory_base << 12) | (dir << 2), 4);
	assert(pdir.present);
	PTE ptable;
	ptable.val = paddr_read((pdir.page_frame << 12) | (page << 2), 4);
	assert(ptable.present);
	uint32_t paddr = (ptable.page_frame << 12) | offset;
#ifdef HAS_DEVICE_VGA
	if (laddr >= 0xa0000 && laddr < 0xa0000 + 320 * 200) {
		assert(paddr == laddr);
	}
#endif
	return paddr;
#else
	assert(0);
#endif
#else
	return tlb_read(laddr) | (laddr & PAGE_MASK);
#endif
}

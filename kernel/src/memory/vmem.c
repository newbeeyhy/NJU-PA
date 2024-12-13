#include "common.h"
#include "memory.h"
#include <string.h>

#define VMEM_ADDR 0xa0000
#define SCR_SIZE (320 * 200)
#define NR_PT ((SCR_SIZE + PT_SIZE - 1) / PT_SIZE) // number of page tables to cover the vmem

PDE *get_updir();
// PTE vptable[NR_PT * NR_PTE] align_to_page;

void create_video_mapping()
{	
/*
	PDE *updir = get_updir();
	for (uint32_t dir = 0; dir < NR_PT; dir++) {
		for (uint32_t page = 0; page < NR_PTE; page++) {
			PTE *uptable = (PTE *)((updir[(VMEM_ADDR >> 22) + dir].page_frame << 12) | (page << 2));
			uptable->val = make_pte(VMEM_ADDR | (page << 12));
		}
	}
*/
}

void video_mapping_write_test()
{
	int i;
	uint32_t *buf = (void *)VMEM_ADDR;
	for (i = 0; i < SCR_SIZE / 4; i++)
	{
		buf[i] = i;
	}
}

void video_mapping_read_test()
{
	int i;
	uint32_t *buf = (void *)VMEM_ADDR;
	for (i = 0; i < SCR_SIZE / 4; i++)
	{
		assert(buf[i] == i);
	}
}

void video_mapping_clear()
{
	memset((void *)VMEM_ADDR, 0, SCR_SIZE);
}

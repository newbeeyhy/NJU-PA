#include "memory/mmu/cache.h"
#include "memory/memory.h"
#include <stdlib.h>

typedef struct {
	struct {
		struct {
			bool valid;
			uint32_t tag;
			uint8_t data[64];
		} slot[8];
	} group[128];
} CACHE;

CACHE cache;

uint32_t get_tag(paddr_t paddr) {
	return (paddr >> 13) & 0x3fff;
}

uint32_t get_group_id(paddr_t paddr) {
	return (paddr >> 6) & 0x7f;
}

uint32_t get_addr(paddr_t paddr) {
	return paddr & 0x3f;
}

// init the cache
void init_cache() {
	// implement me in PA 3-1
	srand(time(0));
	for (int i = 0; i < 128; i++) {
		for (int j = 0; j < 8; j++) {
			cache.group[i].slot[j].valid = false;
		}
	}
}

// write data to cache
void cache_write(paddr_t paddr, size_t len, uint32_t data) {
	// implement me in PA 3-1
	uint32_t addr = get_addr(paddr);
	if (addr + len > 64) {
		cache_write(paddr, 64 - addr, data);
		cache_write(paddr + 64 - addr, len - 64 + addr, data << ((64 - addr) * 8));
		return;
	}
	uint32_t tag = get_tag(paddr), group_id = get_group_id(paddr);
	int slot_id = -1;
	for (int i = 0; i < 8; i++) {
		if (cache.group[group_id].slot[i].valid && cache.group[group_id].slot[i].tag == tag) {
			slot_id = i;
			break;
		}
	}
	if (slot_id == -1) {
		memcpy(hw_mem + paddr, &data, len);
	} else {
		memcpy(cache.group[group_id].slot[slot_id].data + addr, &data, len);
		memcpy(hw_mem + paddr, &data, len);
	}
}

// read data from cache
uint32_t cache_read(paddr_t paddr, size_t len) {
	// implement me in PA 3-1
	uint32_t addr = get_addr(paddr);
	if (addr + len > 64) {
		uint32_t val1 = cache_read(paddr, 64 - addr);
		uint32_t val2 = cache_read(paddr + 64 - addr, len - 64 + addr);
		return (val2 << ((64 - addr) * 8)) | val1;
	}
	uint32_t tag = get_tag(paddr), group_id = get_group_id(paddr);
	uint32_t ret = 0;
	int slot_id = -1;
	for (int i = 0; i < 8; i++) {
		if (cache.group[group_id].slot[i].valid && cache.group[group_id].slot[i].tag == tag) {
			slot_id = i;
			break;
		}
	}
	if (slot_id == -1) {
		slot_id = rand() % 8;
		cache.group[group_id].slot[slot_id].valid = true;
		cache.group[group_id].slot[slot_id].tag = tag;
		memcpy(&ret, hw_mem + paddr, len);
		memcpy(cache.group[group_id].slot[slot_id].data, hw_mem + (paddr ^ addr), 64);
	} else {
		memcpy(&ret, cache.group[group_id].slot[slot_id].data + addr, len);
	}
	return ret;
}
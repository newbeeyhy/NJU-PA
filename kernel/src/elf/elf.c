#include "common.h"
#include "memory.h"
#include "string.h"

#include <elf.h>

#ifdef HAS_DEVICE_IDE
#define ELF_OFFSET_IN_DISK 0
#define ELF_SIZE (4096 * 512)
#endif

#define STACK_SIZE (1 << 20)

void ide_read(uint8_t *, uint32_t, uint32_t);
void ide_read_long(uint8_t *, uint32_t, uint32_t);
void ide_read_buf(uint8_t *, uint32_t, uint32_t);

void create_video_mapping();
uint32_t get_ucr3();

uint32_t loader() {
	Elf32_Ehdr *elf;
	Elf32_Phdr *ph, *eph;

#ifdef HAS_DEVICE_IDE
	uint8_t buf[ELF_SIZE];
	ide_read_buf(buf, ELF_OFFSET_IN_DISK, ELF_SIZE);
	elf = (void *)buf;
	Log("ELF loading from hard disk.");
#else
	elf = (void *)0x0;
	Log("ELF loading from ram disk.");
#endif

	/* Load each program segment */
	ph = (void *)elf + elf->e_phoff;
	eph = ph + elf->e_phnum;
	for (; ph < eph; ph++) {
		if (ph->p_type == PT_LOAD) {

			uint32_t paddr;
#ifdef IA32_PAGE
			paddr = mm_malloc(ph->p_vaddr, ph->p_memsz);
#else
			paddr = ph->p_vaddr;
#endif
			memcpy((void *)paddr, (void *)(elf) + ph->p_offset, ph->p_filesz);
			memset((void *)paddr + ph->p_filesz, 0, ph->p_memsz - ph->p_filesz);
			
#ifdef IA32_PAGE
			/* Record the program break for future use */
			extern uint32_t brk;
			uint32_t new_brk = ph->p_vaddr + ph->p_memsz - 1;
			if (brk < new_brk) {
				brk = new_brk;
			}
#endif
		}
	}

	volatile uint32_t entry = elf->e_entry;

#ifdef IA32_PAGE
	mm_malloc(KOFFSET - STACK_SIZE, STACK_SIZE);
#ifdef HAS_DEVICE_VGA
	create_video_mapping();
#endif
	write_cr3(get_ucr3());
#endif
    // assert(0);
	return entry;
}

#include "cpu/cpu.h"

void set_eflags(uint32_t res, size_t data_size) {
	int cnt = 0;
	for (size_t i = 0; i < 8; i++) {
		if (res >> i & 1) cnt++;
	}

	cpu.eflags.PF = !(cnt & 1);
	cpu.eflags.ZF = res == 0;
	cpu.eflags.SF = res >> (data_size - 1) & 1;
}

uint32_t alu_add(uint32_t src, uint32_t dest, size_t data_size) {
#ifdef NEMU_REF_ALU
	return __ref_alu_add(src, dest, data_size);
#else
    uint32_t full = (uint32_t)0xffffffff >> (32 - data_size);
    src &= full;
	dest &= full;

	uint32_t res = (src + dest) & full;
	cpu.eflags.CF = res < src || res < dest;

	bool b1 = src >> (data_size - 1);
    bool b2 = dest >> (data_size - 1);
    bool b3 = res >> (data_size - 1);
	cpu.eflags.OF = (!b1 && !b2 && b3) || (b1 && b2 && !b3);

	set_eflags(res, data_size);
	return res;
#endif
}

uint32_t alu_adc(uint32_t src, uint32_t dest, size_t data_size) {
#ifdef NEMU_REF_ALU
	return __ref_alu_adc(src, dest, data_size);
#else
	uint32_t full = (uint32_t)0xffffffff >> (32 - data_size);
    src &= full;
	dest &= full;

	uint32_t tmp = (dest + cpu.eflags.CF) & full;
	cpu.eflags.CF = tmp < dest;

	uint32_t res = (tmp + src) & full;
	cpu.eflags.CF |= res < src || res < tmp;

	bool b1 = src >> (data_size - 1);
    bool b2 = dest >> (data_size - 1);
    bool b3 = res >> (data_size - 1);
	cpu.eflags.OF = (!b1 && !b2 && b3) || (b1 && b2 && !b3);

	set_eflags(res, data_size);
	return res;
#endif
}

uint32_t alu_sub(uint32_t src, uint32_t dest, size_t data_size) {
#ifdef NEMU_REF_ALU
	return __ref_alu_sub(src, dest, data_size);
#else
	uint32_t full = (uint32_t)0xffffffff >> (32 - data_size);
    uint32_t tmp = (~src + 1) & full;
	src &= full;
	dest &= full;

	uint32_t res = (tmp + dest) & full;
	cpu.eflags.CF = src > dest;

	bool b1 = src >> (data_size - 1);
    bool b2 = dest >> (data_size - 1);
    bool b3 = res >> (data_size - 1);
	cpu.eflags.OF = (b1 && !b2 && b3) || (!b1 && b2 && !b3);

	set_eflags(res, data_size);
	return res;
#endif
}

uint32_t alu_sbb(uint32_t src, uint32_t dest, size_t data_size) {
#ifdef NEMU_REF_ALU
	return __ref_alu_sbb(src, dest, data_size);
#else
	uint32_t full = (uint32_t)0xffffffff >> (32 - data_size);
	uint32_t tmp = (~cpu.eflags.CF + 1) & full;
	dest &= full;

	uint32_t res = (tmp + dest) & full;
	cpu.eflags.CF = cpu.eflags.CF > dest;

    tmp = (~src + 1) & full;
	src &= full;

	cpu.eflags.CF |= src > res;
	res = (tmp + res) & full;

	bool b1 = src >> (data_size - 1);
    bool b2 = dest >> (data_size - 1);
    bool b3 = res >> (data_size - 1);
	cpu.eflags.OF = (b1 && !b2 && b3) || (!b1 && b2 && !b3);
	
	set_eflags(res, data_size);
	return res;
#endif
}

uint64_t alu_mul(uint32_t src, uint32_t dest, size_t data_size) {
#ifdef NEMU_REF_ALU
	return __ref_alu_mul(src, dest, data_size);
#else
	uint32_t full = (uint32_t)0xffffffff >> (32 - data_size);
	src &= full;
	dest &= full;

	uint64_t res = (uint64_t)src * dest;
	cpu.eflags.CF = cpu.eflags.OF = !((res >> data_size) == 0);

	return res;
#endif
}

int64_t alu_imul(int32_t src, int32_t dest, size_t data_size) {
#ifdef NEMU_REF_ALU
	return __ref_alu_imul(src, dest, data_size);
#else
	if (data_size == 8) {
		int8_t src8_t = src, dest8_t = dest;
		return src8_t * dest8_t;
	} else if (data_size == 16) {
		int16_t src16_t = src, dest16_t = dest;
		return src16_t * dest16_t;
	} else {
		return (int64_t)src * dest;
	}
#endif
}

// need to implement alu_mod before testing
uint32_t alu_div(uint64_t src, uint64_t dest, size_t data_size) {
#ifdef NEMU_REF_ALU
	return __ref_alu_div(src, dest, data_size);
#else
	uint64_t full = (uint64_t)0xffffffffffffffff >> (64 - data_size);
	src &= full;
	dest &= full;

	if (src == 0) {
		printf("Floating Point Exception!\n");
		assert(0);
	}

	uint32_t res = dest / src;
	return res;
#endif
}

// need to implement alu_imod before testing
int32_t alu_idiv(int64_t src, int64_t dest, size_t data_size) {
#ifdef NEMU_REF_ALU
	return __ref_alu_idiv(src, dest, data_size);
#else
	if (src == 0) {
		printf("Floating Point Exception!\n");
		assert(0);
	}
	if (data_size == 8) {
		int8_t src8_t = src, dest8_t = dest;
		return dest8_t / src8_t;
	} else if (data_size == 16) {
		int16_t src16_t = src, dest16_t = dest;
		return dest16_t / src16_t;
	} else if (data_size == 32) {
		int32_t src32_t = src, dest32_t = dest;
		return dest32_t / src32_t;
	} else {
	    return (int32_t)dest / src;
	}
#endif
}

uint32_t alu_mod(uint64_t src, uint64_t dest) {
#ifdef NEMU_REF_ALU
	return __ref_alu_mod(src, dest);
#else
	if (src == 0) {
		printf("Floating Point Exception!\n");
		assert(0);
	}
	return dest % src;
#endif
}

int32_t alu_imod(int64_t src, int64_t dest) {
#ifdef NEMU_REF_ALU
	return __ref_alu_imod(src, dest);
#else
	if (src == 0) {
		printf("Floating Point Exception!\n");
		assert(0);
	}
	return dest % src;
#endif
}

uint32_t alu_and(uint32_t src, uint32_t dest, size_t data_size) {
#ifdef NEMU_REF_ALU
	return __ref_alu_and(src, dest, data_size);
#else
	uint32_t full = (uint32_t)0xffffffff >> (32 - data_size);
    src &= full;
	dest &= full;

	uint32_t res = src & dest;
	cpu.eflags.CF = 0;
	cpu.eflags.OF = 0;

	set_eflags(res, data_size);
	return res;
#endif
}

uint32_t alu_xor(uint32_t src, uint32_t dest, size_t data_size) {
#ifdef NEMU_REF_ALU
	return __ref_alu_xor(src, dest, data_size);
#else
	uint32_t full = (uint32_t)0xffffffff >> (32 - data_size);
    src &= full;
	dest &= full;

	uint32_t res = src ^ dest;
	cpu.eflags.CF = 0;
	cpu.eflags.OF = 0;

	set_eflags(res, data_size);
	return res;
#endif
}

uint32_t alu_or(uint32_t src, uint32_t dest, size_t data_size) {
#ifdef NEMU_REF_ALU
	return __ref_alu_or(src, dest, data_size);
#else
	uint32_t full = (uint32_t)0xffffffff >> (32 - data_size);
    src &= full;
	dest &= full;

	uint32_t res = src | dest;
	cpu.eflags.CF = 0;
	cpu.eflags.OF = 0;

	set_eflags(res, data_size);
	return res;
#endif
}

uint32_t alu_shl(uint32_t src, uint32_t dest, size_t data_size) {
#ifdef NEMU_REF_ALU
	return __ref_alu_shl(src, dest, data_size);
#else
	uint32_t full = (uint32_t)0xffffffff >> (32 - data_size);
	src &= full;
	dest &= full;

	uint32_t res = (dest << src) & full;
	cpu.eflags.CF = src > data_size ? 0 : dest >> (data_size - src) & 1;

	set_eflags(res, data_size);
	return res;
#endif
}

uint32_t alu_shr(uint32_t src, uint32_t dest, size_t data_size) {
#ifdef NEMU_REF_ALU
	return __ref_alu_shr(src, dest, data_size);
#else
	uint32_t full = (uint32_t)0xffffffff >> (32 - data_size);
	src &= full;
	dest &= full;

	uint32_t res = (dest >> src) & full;
	cpu.eflags.CF = src > data_size ? 0 : dest >> (src - 1) & 1;

	set_eflags(res, data_size);
	return res;
#endif
}

uint32_t alu_sar(uint32_t src, uint32_t dest, size_t data_size) {
#ifdef NEMU_REF_ALU
	return __ref_alu_sar(src, dest, data_size);
#else
	uint32_t full = (uint32_t)0xffffffff >> (32 - data_size);
	src &= full;
	dest &= full;

	uint32_t res = dest >> src;
	uint32_t sign = dest >> (data_size - 1);
	if (sign) {
		for (size_t i = 0; i < src; i++) {
			res |= 1 << (data_size - 1 - i);
		}
	}
	cpu.eflags.CF = src > data_size ? 0 : dest >> (src - 1) & 1;

	set_eflags(res, data_size);
	return res;
#endif
}

uint32_t alu_sal(uint32_t src, uint32_t dest, size_t data_size) {
#ifdef NEMU_REF_ALU
	return __ref_alu_sal(src, dest, data_size);
#else
	uint32_t res = alu_shl(src, dest, data_size);
	return res;
#endif
}
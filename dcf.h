#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef SRAM_DATA_SIZE
#define SRAM_DATA_SIZE 0x10000
#endif

#if MOCK_DCF
static char sram[SRAM_DATA_SIZE];
#define SRAM_DATA_BASE ((unsigned int)sram)
#else

#ifndef SRAM_DATA_BASE
#define SRAM_DATA_BASE 0x10000
#endif

#endif

#define SRAM_DATA_END (SRAM_DATA_BASE + SRAM_DATA_SIZE)

#define MAX_NAME 20
#define MAX_DATA_ENTRY 20
#define MAX_REG_ENTRY MAX_DATA_ENTRY

#if MOCK_DCF
unsigned int reg_r0 = 0;
unsigned int reg_r1 = 0;

#define REG_R0 ((unsigned int)&reg_r0)
#define REG_R1 ((unsigned int)&reg_r1)
#else
#define REG_R0 0xfffffffe
#define REG_R1 0xffffffff
#endif

int lock_r0 = 0;
int lock_r1 = 0;

#define IS_REG(reg) (reg == REG_R0 || reg == REG_R1)
#define REG_NAME(reg) ((reg == REG_R0) ? "r0" : "r1")
#define STR_ISB_CMD(is_str) (is_str ? "str" : "isb")
#define AND_OR_CMD(is_and) (is_and ? "and" : "or")
#define AND_OR_OP(is_and) (is_and ? "&" : "|")
#define EQ_NEQ_CMD(eq) (eq ? "eq" : "ne")
#define EQ_NEQ_OP(eq) (eq ? "=" : "!")

int flag = 0;

#if MOCK_DCF
#define VALUE(var) (*((unsigned int*)var))
#else
#define VALUE(var) (-1)
#endif

#define _PCODE(...) { \
	printf("\n"); \
	PDBG(""); \
	printf("#[CODE]:" __VA_ARGS__); \
}

#define PCODE(...) { \
	if (cmd_ouput) _PCODE(__VA_ARGS__); \
}

#define PRINT(fmt, args...) { \
	if (cmd_ouput) fprintf(stderr, "#[LOG]%s(%d): "fmt, __func__, __LINE__, ##args); \
}

#if DEBUG
#define PDBG(...) PRINT(__VA_ARGS__)
#else
#define PDBG(...)
#endif

int cmd_counter = 0;
int cmd_ouput = 1;

#define CMD(fmt, args...) { \
	if (cmd_ouput) printf(fmt " (%08x)\n", ##args, cmd_counter << 3); \
	cmd_counter ++; \
}

typedef void (*JUMP_CALL)();

#define ASSERT(x) { \
	if (!(x)) { \
		PRINT("assert failed!"); \
		exit(-1); \
	} \
}

typedef struct {
	char _name[MAX_NAME];
	unsigned int _addr;
	unsigned int _size;
} DATA, *PDATA;

static DATA data_area[MAX_DATA_ENTRY];
static int data_area_cur = 0;

static DATA reg_area[MAX_REG_ENTRY];
static int reg_area_cur = 0;

static int sram_data_pos = 0;

#define DECLARE_DATA(name, size) { \
	ASSERT(data_area_cur < (MAX_DATA_ENTRY - 1)); \
	ASSERT(strlen(#name) < (MAX_NAME - 1)); \
	ASSERT((sram_data_pos + size) < SRAM_DATA_SIZE); \
	strcpy(data_area[data_area_cur]._name, #name); \
	data_area[data_area_cur]._addr = sram_data_pos + SRAM_DATA_BASE; \
	data_area[data_area_cur]._size = size; \
	data_area_cur ++; \
	sram_data_pos += size; \
}

#define DECLARE_REG(name, addr, num) { \
	ASSERT(reg_area_cur < (MAX_REG_ENTRY - 1)); \
	ASSERT(strlen(#name) < (MAX_NAME - 1)); \
	strcpy(reg_area[reg_area_cur]._name, #name); \
	reg_area[reg_area_cur]._addr = addr; \
	reg_area[reg_area_cur]._size = num; \
	reg_area_cur ++; \
}

#define DECLARE_U32(name) DECLARE_DATA(name, 4)

unsigned int get_addr(DATA *area, int area_cur, char* name) {
	int i = 0;
	for (i = 0; i < area_cur; i++) {
		if (!strcmp(area[i]._name, name))
			return area[i]._addr;
	}
	ASSERT(0);
	return 0;
}

#define DATA_ADDR(name) get_addr(data_area, data_area_cur, #name)

#define REG_ADDR(name, num) \
	(get_addr(reg_area, reg_area_cur, #name) + (num) * 4)

/////////////////////////////commands///////////////////////////////

void idle(unsigned int delay);

void end();

// r0 <> r1
void swap();

// *addr = ~*addr
void inv(unsigned int addr);

// *dst_addr = val
void set_val(unsigned int dst_addr, unsigned int val);

// *dst_addr = val; sync (isb)
void set_val_sync(unsigned int dst_addr, unsigned int val);

// *dst_addr = *src_addr
void set(unsigned int dst_addr, unsigned int src_addr);

// *dst_addr = *src_addr; sync (isb)
void set_sync(unsigned int dst_addr, unsigned int src_addr);

// *dst_addr = *src_addr | val
void or_val(unsigned int dst_addr, unsigned int src_addr, unsigned int val);

// *dst_addr = *src_reg | *src_addr (src_reg should be reg)
void or_reg(unsigned int dst_addr, unsigned int src_reg, unsigned int src_addr);

// *dst_addr = *src_addr & val
void and_val(unsigned int dst_addr, unsigned int src_addr, unsigned int val);

// *dst_addr = *src_reg & *src_addr (src_reg should be reg)
void and_reg(unsigned int dst_addr, unsigned int src_addr1, unsigned int src_addr2);

// *dst_addr = *src_addr << val
void lsr(unsigned int dst_addr, unsigned int src_addr, unsigned int val);

// while (!(*dst_addr & val))
void while_zero(unsigned int addr, unsigned int val);

// bl up/down offset (offset = cmds << 3)
void bl(unsigned int cmds, int up);

/**
 * do {
 *   cond();
 * } while (flag);
 */
void while_flag(JUMP_CALL cond);

/**
 * cond();
 * if (!flag)
 *   if_func();
 */
void if_flag(JUMP_CALL cond, JUMP_CALL if_func);

/**
 * cond();
 * if (!flag)
 *   if_func();
 * else
 *   else_func();
 */
void if_else_flag(JUMP_CALL cond, JUMP_CALL if_func, JUMP_CALL else_func);

// flag = (*addr != val) ^ eq
void cmp_val(unsigned int addr, unsigned int val, int eq);
#define eq_val(addr, val) \
	cmp_val(addr, val, 1)
#define neq_val(addr, val) \
	cmp_val(addr, val, 0)
#define ifcond_eq_val(addr, val) \
	neq_val(addr, val)
#define ifcond_neq_val(addr, val) \
	eq_val(addr, val)

// flag = (*addr1 != addr2) ^ eq
void cmp(unsigned int addr1, unsigned int addr2, int eq);
#define eq(addr1, addr2) \
	cmp(addr1, addr2, 1)
#define neq(addr1, addr2) \
	cmp(addr1, addr2, 0)
#define ifcond_eq(addr1, addr2) \
	neq(addr1, addr2)
#define ifcond_neq(addr1, addr2) \
	eq(addr1, addr2)

void _set_val(unsigned int dst_addr, unsigned int val, int sync);
void __set(unsigned int dst_addr, unsigned int src_addr, int sync);
void _set(unsigned int dst_addr, unsigned int src_addr);
void _lsr(unsigned int dst_addr, unsigned int src_addr, unsigned int val);
void _cmp(unsigned int addr1, unsigned int addr2, int eq);

/////////////////////////////commands helpers///////////////////////////////

char buf[MAX_DATA_ENTRY][MAX_NAME];
char* fix_addr(unsigned int addr, char* buf) {
	if (IS_REG(addr)) {
		sprintf(buf, "%s", REG_NAME(addr));
	} else {
		sprintf(buf, "$0x%x", addr);
	}
	return buf;
}

unsigned int get_reg(unsigned int reg_addr) {
	ASSERT(lock_r0 <= 1 || lock_r1 <= 1);

	//prefer r1
	if (reg_addr == REG_R1) {
		return (lock_r1 <= 1) ? REG_R1 : REG_R0;
	}

	//r1 is better than r0
	if (lock_r0 && !lock_r1)
		return REG_R1;

	return (lock_r0 <= 1) ? REG_R0 : REG_R1;
}

void lock_reg(unsigned int reg_addr) {
	ASSERT(IS_REG(reg_addr));

	if (reg_addr == REG_R0) {
		ASSERT(lock_r0 <= 1);
		if (lock_r0 == 1) {
			PDBG("lock r0\n");
			_set(DATA_ADDR(save_r0), REG_R0);
		}
		lock_r0 ++;
	} else {
		ASSERT(lock_r1 <= 1);
		if (lock_r1 == 1) {
			PDBG("lock r1\n");
			_set(DATA_ADDR(save_r1), REG_R1);
		}
		lock_r1 ++;
	}
}

void unlock_reg(unsigned int reg_addr) {
	ASSERT(IS_REG(reg_addr));

	if (reg_addr == REG_R0) {
		ASSERT(lock_r0 >= 1);
		if (lock_r0 > 1) {
			PDBG("unlock r0\n");
			_set(REG_R0, DATA_ADDR(save_r0));
		}
		lock_r0 --;
	} else {
		ASSERT(lock_r1 >= 1);
		if (lock_r1 > 1) {
			PDBG("unlock r1\n");
			_set(REG_R1, DATA_ADDR(save_r1));
		}
		lock_r1 --;
	}
}

/////////////////////////////commands///////////////////////////////

void idle(unsigned int delay) {
	CMD("idle 0x0, 0x%x;# idle(0x%x)", delay, delay);
}

void end() {
	idle(1);
	idle(1);
	idle(1);
	idle(1);
	idle(1);
	idle(1);
	idle(1);
	idle(1);
	idle(1);
	CMD("end;# end of all cmds");
}

void swap() {
	CMD("inv r0, r1;# r0 <=> r1");
#if MOCK_DCF
	VALUE(REG_R0) ^= VALUE(REG_R1);
	VALUE(REG_R1) ^= VALUE(REG_R0);
	VALUE(REG_R0) ^= VALUE(REG_R1);
#endif
}

void inv(unsigned int addr) {
	PDBG("inv %s\n", fix_addr(addr, buf[0]));
	unsigned int reg = IS_REG(addr) ? addr : get_reg(REG_R0);

	if (reg != addr) {
		lock_reg(reg);
		_set(reg, addr);
	}
	CMD("inv %s;# %s = ~%s", REG_NAME(reg), REG_NAME(reg), REG_NAME(reg));
#if MOCK_DCF
	VALUE(reg) = ~VALUE(reg);
#endif
	if (reg != addr) {
		_set(addr, reg);
		unlock_reg(reg);
	}
}

void clear_reg(unsigned int reg_addr) {
	ASSERT(IS_REG(reg_addr));

	CMD("and %s, 0x0;# %s = 0", REG_NAME(reg_addr), REG_NAME(reg_addr));
#if MOCK_DCF
	VALUE(reg_addr) = 0;
#endif
}

void set_val_sync(unsigned int dst_addr, unsigned int val) {
	PDBG("%s = 0x%x; sync\n", fix_addr(dst_addr, buf[0]), val);
	_set_val(dst_addr, val, 1);
}

void set_val(unsigned int dst_addr, unsigned int val) {
	PDBG("%s = 0x%x\n", fix_addr(dst_addr, buf[0]), val);
	_set_val(dst_addr, val, 0);
}

void _set_val(unsigned int dst_addr, unsigned int val, int sync) {
	if (IS_REG(dst_addr)) {
		// set val to reg

		clear_reg(dst_addr);
		CMD("or %s, 0x%x;# %s |= 0x%x", REG_NAME(dst_addr), val,
				REG_NAME(dst_addr), val);
#if MOCK_DCF
		VALUE(dst_addr) = val;
#endif
	} else {
		// set val to dst_addr

		CMD("%s 0x%x, 0x%x;# $0x%x = 0x%x", STR_ISB_CMD(!sync), dst_addr,
				val, dst_addr, val);
#if MOCK_DCF
		VALUE(dst_addr) = val;
#endif
	}
}

void set(unsigned int dst_addr, unsigned int src_addr) {
	PDBG("%s = %s\n", fix_addr(dst_addr, buf[0]), fix_addr(src_addr, buf[1]));
	_set(dst_addr, src_addr);
}

void set_sync(unsigned int dst_addr, unsigned int src_addr) {
	PDBG("%s = %s; sync\n", fix_addr(dst_addr, buf[0]), fix_addr(src_addr, buf[1]));
	__set(dst_addr, src_addr, 1);
}

void _set(unsigned int dst_addr, unsigned int src_addr) {
	__set(dst_addr, src_addr, 0);
}

void __set(unsigned int dst_addr, unsigned int src_addr, int sync) {
	if (dst_addr == src_addr)
		return;

	if (IS_REG(dst_addr)) {
		if (IS_REG(src_addr)) {
			// set from reg to reg

			__set(DATA_ADDR(tmp), src_addr, sync);
			__set(dst_addr, DATA_ADDR(tmp), sync);
		} else {
			// set from src_addr to reg

			CMD("or 0x%x, %s;# %s = $0x%x", src_addr,
					REG_NAME(dst_addr),
					REG_NAME(dst_addr),
					src_addr);
#if MOCK_DCF
			VALUE(dst_addr) = VALUE(src_addr);
#endif
		}
	} else {
		if (IS_REG(src_addr)) {
			// set from reg to dst_addr

			CMD("%s 0x%x, %s;# $0x%x = %s", STR_ISB_CMD(!sync),
					dst_addr, REG_NAME(src_addr),
					dst_addr, REG_NAME(src_addr));
#if MOCK_DCF
			VALUE(dst_addr) = VALUE(src_addr);
#endif
		} else {
			// set from src_addr to dst_addr

			unsigned int reg = get_reg(REG_R0);
			lock_reg(reg);
			__set(reg, src_addr, sync);
			__set(dst_addr, reg, sync);
			unlock_reg(reg);
		}
	}
}

void and_or_val(unsigned int dst_addr, unsigned int src_addr, unsigned int val,
		int is_and) {
	if (!IS_REG(dst_addr)) {
		if (IS_REG(src_addr)) {
			and_or_val(src_addr, src_addr, val, is_and);
			_set(dst_addr, src_addr);
		} else {
			unsigned int reg = get_reg(REG_R0);
			lock_reg(reg);
			and_or_val(reg, src_addr, val, is_and);
			_set(dst_addr, reg);
			unlock_reg(reg);
		}
	} else {
		if (IS_REG(src_addr)) {
			if (src_addr == dst_addr) {
				CMD("%s %s, 0x%x;# %s %s= 0x%x",
						AND_OR_CMD(is_and),
						REG_NAME(dst_addr), val,
						REG_NAME(dst_addr),
						AND_OR_OP(is_and), val);
#if MOCK_DCF
				if (is_and)
					VALUE(dst_addr) &= val;
				else
					VALUE(dst_addr) |= val;
#endif
			} else {
				_set(DATA_ADDR(tmp), src_addr);
				and_or_val(dst_addr, DATA_ADDR(tmp), val, is_and);
			}
		} else {
			CMD("%s 0x%x, %s, 0x%x;# %s = $0x%x %s 0x%x",
					AND_OR_CMD(is_and),
					src_addr, REG_NAME(dst_addr), val,
					REG_NAME(dst_addr), src_addr,
					AND_OR_OP(is_and), val);
#if MOCK_DCF
			if (is_and)
				VALUE(dst_addr) = VALUE(src_addr) & val;
			else
				VALUE(dst_addr) = VALUE(src_addr) | val;
#endif
		}
	}
}

void and_or_reg(unsigned int dst_addr, unsigned int src_reg, unsigned int src_addr,
		int is_and) {
	ASSERT(IS_REG(src_reg));
	if (src_reg == src_addr) {
		and_or_val(dst_addr, src_reg, 0, is_and);
	} else {
		if (IS_REG(dst_addr)) {
			if (IS_REG(src_addr)) {
				if (dst_addr == REG_R1) {
					CMD("%s r0, r1;# r1 %s= r0",
							AND_OR_CMD(is_and),
							AND_OR_OP(is_and));
#if MOCK_DCF
					if (is_and)
						VALUE(REG_R1) &= VALUE(REG_R0);
					else
						VALUE(REG_R1) |= VALUE(REG_R0);
#endif
				} else {
					_set(DATA_ADDR(tmp), REG_R1);
					CMD("%s 0x%x, r0;# r0 %s= $0x%x",
							AND_OR_CMD(is_and),
							DATA_ADDR(tmp),
							AND_OR_OP(is_and),
							DATA_ADDR(tmp));
#if MOCK_DCF
					if (is_and)
						VALUE(REG_R0) &= VALUE(DATA_ADDR(tmp));
					else
						VALUE(REG_R0) |= VALUE(DATA_ADDR(tmp));
#endif
				}
			} else {
				if (dst_addr == REG_R1) {
					CMD("%s 0x%x, r0, r1;# r1 = r0 %s $0x%x",
							AND_OR_CMD(is_and),
							src_addr,
							AND_OR_OP(is_and),
							src_addr);
#if MOCK_DCF
					if (is_and)
						VALUE(REG_R1) =
							VALUE(REG_R0) & VALUE(src_addr);
					else
						VALUE(REG_R1) =
							VALUE(REG_R0) | VALUE(src_addr);
#endif
				} else {
					if (dst_addr != src_reg) {
						_set(dst_addr, src_addr);
						and_or_reg(dst_addr, src_reg, dst_addr,
								is_and);
					} else {
						lock_reg(REG_R1);
						and_or_reg(REG_R1, src_reg, src_addr,
								is_and);
						_set(dst_addr, REG_R1);
						unlock_reg(REG_R1);
					}
				}
			}
		} else {
			unsigned int reg = get_reg(REG_R1);
			if (reg != REG_R1)
				swap();

			lock_reg(REG_R1);
			and_or_reg(REG_R1, src_reg, src_addr, is_and);
			_set(dst_addr, REG_R1);
			unlock_reg(REG_R1);

			if (reg != REG_R1)
				swap();
		}
	}
}

void or_val(unsigned int dst_addr, unsigned int src_addr, unsigned int val) {
	PDBG("%s = %s | 0x%x\n", fix_addr(dst_addr, buf[0]),
			fix_addr(src_addr, buf[1]), val);
	and_or_val(dst_addr, src_addr, val, 0);
}

void or_reg(unsigned int dst_addr, unsigned int src_reg, unsigned int src_addr) {
	PDBG("%s = %s | %s\n", fix_addr(dst_addr, buf[0]),
			fix_addr(src_reg, buf[1]), fix_addr(src_addr, buf[2]));
	and_or_reg(dst_addr, src_reg, src_addr, 0);
}

void and_val(unsigned int dst_addr, unsigned int src_addr, unsigned int val) {
	PDBG("%s = %s & 0x%x\n", fix_addr(dst_addr, buf[0]),
			fix_addr(src_addr, buf[1]), val);
	and_or_val(dst_addr, src_addr, val, 1);
}

void and_reg(unsigned int dst_addr, unsigned int src_reg, unsigned int src_addr) {
	PDBG("%s = %s & %s\n", fix_addr(dst_addr, buf[0]),
			fix_addr(src_reg, buf[1]), fix_addr(src_addr, buf[2]));
	and_or_reg(dst_addr, src_reg, src_addr, 1);
}

void lsr(unsigned int dst_addr, unsigned int src_addr, unsigned int val) {
	PDBG("%s = %s << 0x%x\n", fix_addr(dst_addr, buf[0]),
			fix_addr(src_addr, buf[1]), val);
	_lsr(dst_addr, src_addr, val);
}

void _lsr(unsigned int dst_addr, unsigned int src_addr, unsigned int val) {
	if (IS_REG(dst_addr)) {
		if (IS_REG(src_addr)) {
			if (dst_addr == src_addr) {
				CMD("lsr %s, 0x%x;# %s <<= 0x%x",
						REG_NAME(dst_addr), val,
						REG_NAME(dst_addr), val);
#if MOCK_DCF
				VALUE(dst_addr) <<= val;
#endif
			} else {
				_set(DATA_ADDR(tmp), src_addr);
				_lsr(dst_addr, DATA_ADDR(tmp), val);
			}
		} else {
			CMD("lsr 0x%x, %s, 0x%x;# %s = $0x%x << 0x%x",
					src_addr, REG_NAME(dst_addr), val,
					REG_NAME(dst_addr), src_addr, val);
#if MOCK_DCF
			VALUE(dst_addr) = VALUE(src_addr) << val;
#endif
		}
	} else {
		unsigned int reg = get_reg(IS_REG(src_addr) ? src_addr : REG_R0);
		lock_reg(reg);
		_lsr(reg, src_addr, val);
		_set(dst_addr, reg);
		unlock_reg(reg);
	}
}

void while_zero(unsigned int addr, unsigned int val) {
	PDBG("while (!(%s & 0x%x));\n", fix_addr(addr, buf[0]), val);
	ASSERT(get_reg(REG_R0) == REG_R0);

	lock_reg(REG_R0);

	idle(1);
	and_val(REG_R0, addr, val);
	CMD("polneq 0x0, r0, 0x0;# while(!r0) r0 = $0x%x & 0x%x;",
			addr, val);
#if MOCK_DCF
	while (!(VALUE(addr) & val));
#endif
	unlock_reg(REG_R0);
}

void bl(unsigned int cmds, int up) {
	unsigned int offset = cmds << 3; // 64 bits per cmd
	PDBG("%s 0x%x\n", up ? "u" : "d", offset);
	CMD("bl_%s 0x0, 0x%x;# if (flag) jump(%08x)", up ? "u" : "d", offset,
			(up ? cmd_counter - cmds : cmd_counter + cmds) << 3);
#if MOCK_DCF
	//TODO: if (flag) bl xxx;
#endif
}

void while_flag(JUMP_CALL cond) {
	int counter;
	unsigned int cmds;

	counter = cmd_counter;
	cond();

	cmds = cmd_counter - counter;
	bl(cmds, 1);
}

void if_flag(JUMP_CALL cond, JUMP_CALL if_func) {
	int counter, output;
	unsigned int cmds;

	cond();

	counter = cmd_counter;

	// fake run, just get how many cmds we will jump.
	output = cmd_ouput;
	cmd_ouput = 0;
	if_func();
	if (output)
		cmd_ouput = 1;

	cmds = cmd_counter - counter;
	cmd_counter = counter;
	bl(cmds + 1, 0);

	if_func();
}

void if_else_flag(JUMP_CALL cond, JUMP_CALL if_func, JUMP_CALL else_func) {
	int counter, output;
	unsigned int if_cmds, else_cmds = 0;

	cond();

	counter = cmd_counter;

	output = cmd_ouput;
	cmd_ouput = 0;
	if_func();
	clear_reg(REG_R0);
	cmp_val(REG_R0, 0, 1); bl(else_cmds, 0);

	if_cmds = cmd_counter - counter;

	else_func();

	else_cmds = cmd_counter - counter - if_cmds;
	if (output)
		cmd_ouput = 1;
	cmd_counter = counter;

	/**
	 * if (!flag) {
	 *   if_func();
	 *   goto skip_else;
	 * }
	 * else_func();
	 *
	 * skip_else:
	 */
	bl(if_cmds + 1, 0);
	if_func();
	clear_reg(REG_R0);
	cmp_val(REG_R0, 0, 1); bl(else_cmds + 1, 0);

	else_func();
}

void cmp_val(unsigned int addr, unsigned int val, int eq)
{
	PDBG("cmp%s %s, 0x%x\n", EQ_NEQ_CMD(eq), fix_addr(addr, buf[0]), val);
	if (IS_REG(addr)) {
		CMD("cmp%s %s, 0x%x;# flag = (%s %s= 0x%x)",
				EQ_NEQ_CMD(eq), REG_NAME(addr), val,
				REG_NAME(addr), EQ_NEQ_OP(eq), val);
#if MOCK_DCF
		flag = (VALUE(addr) != val) ^ eq;
#endif
	} else {
		unsigned int reg = get_reg(REG_R0);
		lock_reg(reg);
		CMD("cmp%s 0x%x, %s, 0x%x;# flag = ($0x%x %s= 0x%x)",
				EQ_NEQ_CMD(eq), addr, REG_NAME(reg), val,
				addr, EQ_NEQ_OP(eq), val);
#if MOCK_DCF
		flag = (VALUE(addr) != val) ^ eq;
#endif
		unlock_reg(reg);
	}
}

void cmp(unsigned int addr1, unsigned int addr2, int eq)
{
	PDBG("cmp%s %s, %s\n", EQ_NEQ_CMD(eq), fix_addr(addr1, buf[0]),
			fix_addr(addr2, buf[1]));
	_cmp(addr1, addr2, eq);
}

void _cmp(unsigned int addr1, unsigned int addr2, int eq)
{
	if (IS_REG(addr1) && IS_REG(addr2)) {
		ASSERT(addr1 != addr2);
		CMD("cmp%s %s, %s;# flag = %s %s= %s",
				EQ_NEQ_CMD(eq), REG_NAME(addr1), REG_NAME(addr2),
				REG_NAME(addr1), EQ_NEQ_OP(eq), REG_NAME(addr2));
#if MOCK_DCF
		flag = (VALUE(addr1) != VALUE(addr2)) ^ eq;
#endif
	} else if (IS_REG(addr1)) {
		if (addr1 == REG_R1)
			swap();

		lock_reg(REG_R1);
		CMD("cmp%s 0x%x, r0, r1;# flag = (r0 %s= $0x%x)",
				EQ_NEQ_CMD(eq), addr2,
				EQ_NEQ_OP(eq), addr2);
#if MOCK_DCF
		VALUE(REG_R1) = VALUE(addr2);
		flag = (VALUE(addr1) != VALUE(addr2)) ^ eq;
#endif
		unlock_reg(REG_R1);

		if (addr1 == REG_R1)
			swap();
	} else {
		ASSERT(!IS_REG(addr2));

		lock_reg(REG_R0);
		_set(REG_R0, addr1);
		_cmp(REG_R0, addr2, eq);
		unlock_reg(REG_R0);
	}
}

/////////////////////////////debug///////////////////////////////

void show_status(int data, int regs, int reg)
{
	int i = 0, j = 0;
	if (data) {
		PRINT("sram data area:0x%08x -> 0x%08x\n", SRAM_DATA_BASE,
				SRAM_DATA_BASE + sram_data_pos);
		PRINT("data entry:%d\n", data_area_cur);
		for (i = 0; i < data_area_cur; i ++) {
			PRINT("data[%d]: %s at 0x%08x-0x%08x\n", i, data_area[i]._name,
					data_area[i]._addr,
					data_area[i]._addr + data_area[i]._size);
		}
	}
	if (regs) {
		PRINT("reg entry:%d\n", reg_area_cur);
		for (i = 0; i < reg_area_cur; i ++) {
			PRINT("reg[%d]: %s at 0x%08x-0x%08x\n", i, reg_area[i]._name,
					reg_area[i]._addr,
					reg_area[i]._addr + reg_area[i]._size * 4);
#if MOCK_DCF
			for (j = 0; j < reg_area[i]._size; j++) {
				PRINT("\t0x%08x:0x%08x\n", reg_area[i]._addr + j * 4,
						VALUE(reg_area[i]._addr + j * 4));
			}
#endif
		}
	}
	if (reg) {
#if MOCK_DCF
		PRINT("reg r0:0x%08x, reg_r1:0x%08x\n", VALUE(REG_R0),
				VALUE(REG_R1));
#endif
	}
}

void setup() {
	DECLARE_U32(tmp);
	DECLARE_U32(save_r0);
	DECLARE_U32(save_r1);
}

#include "global.h"
#include "defines.h"
#include "IF.h"
#include "ID.h"
#include "EX.h"

void IF() {
	if (STALL) return;
	if (ID_EX.pc_src_out == 1) PC = ID_EX.pc_out;
	else PC = IF_ID.pc_plus_four_out;
	int i;
	for (i = 0; i < 4; i++)
		IF_ID.ins_reg_in = (IF_ID.ins_reg_in << 8) | (unsigned char) iMemory[PC + i];
	PC = PC + 4;
	IF_ID.pc_plus_four_in = PC;
	
	IF_ID.opcode_in = IF_ID.ins_reg_in >> 26;
	IF_ID.funct_in = IF_ID.ins_reg_in << 26 >> 26;
	IF_ID.rs_in = IF_ID.ins_reg_in << 6 >> 27;
	IF_ID.rt_in = IF_ID.ins_reg_in << 11 >> 27;
	
	if (IF_ID.ins_reg_in >> 26 == HALT) IF_HALT = 1;
	else IF_HALT = 0;
}

void IF_ID_READY() {
	if (STALL) return;
	IF_ID.pc_plus_four_out = IF_ID.pc_plus_four_in;
	IF_ID.ins_reg_out = IF_ID.ins_reg_in;
	
	IF_ID.opcode_out = IF_ID.opcode_in;
	IF_ID.funct_out = IF_ID.funct_in;
	IF_ID.rs_out = IF_ID.rs_in;
	IF_ID.rt_out = IF_ID.rt_in;
}

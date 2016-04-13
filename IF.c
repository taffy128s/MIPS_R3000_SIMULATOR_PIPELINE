#include "global.h"
#include "IF.h"

void IF() {
	int i;
	IF_ID.pc_plus_four_in = PC + 4;
	
	for (i = 0; i < 4; i++)
		IF_ID.ins_reg_in = (IF_ID.ins_reg_in << 8) | (unsigned char) iMemory[PC + i];
	
	//IF_ID.ins_reg_in = 0x0000FFFF;
}

void IF_ID_READY() {
	IF_ID.pc_plus_four_out = IF_ID.pc_plus_four_in;
	IF_ID.ins_reg_out = IF_ID.ins_reg_in;
}

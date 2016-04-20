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
	if (FWD_RS_TO_ID) IF_ID.$rs = reg[EX_DM.rs_out];
	else IF_ID.$rs = reg[IF_ID.rs_in];
	if (FWD_RT_TO_ID) IF_ID.$rt = reg[EX_DM.rt_out];
	else IF_ID.$rt = reg[IF_ID.rt_in];
	
	FLUSH = 0;
	if (IF_ID.opcode_in == BEQ) {
		if (IF_ID.$rs == IF_ID.$rt) FLUSH = 1;
	} else if (IF_ID.opcode_in == BNE) {
		if (IF_ID.$rs != IF_ID.$rt) FLUSH = 1;
	} else if (IF_ID.opcode_in == BGTZ) {
		int intRs = IF_ID.$rs;
		if (intRs > 0) FLUSH = 1;
	} else if (IF_ID.opcode_in == J || IF_ID.opcode_in == JAL) {
		FLUSH = 1;
	} else if (IF_ID.opcode_in == R && IF_ID.funct_in == JR) {
		FLUSH = 1;
	}
	
	/*if (FLUSH) {
		IF_ID.pc_plus_four_in = 0;
		IF_ID.ins_reg_in = 0;
			
		IF_ID.opcode_in = 0;
		IF_ID.funct_in = 0;
		IF_ID.rs_in = 0;
		IF_ID.rt_in = 0;
	}*/
	
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

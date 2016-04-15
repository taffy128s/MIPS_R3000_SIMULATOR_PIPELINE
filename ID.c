#include "defines.h"
#include "global.h"
#include "IF.h"
#include "ID.h"

static unsigned opcode, funct;

void ID() {
	opcode = IF_ID.ins_reg_out >> 26;
	funct = IF_ID.ins_reg_out << 26 >> 26;
	
	ID_EX.ins_reg_in = IF_ID.ins_reg_out;
	
	CONTROL();
	
	ID_EX.$rs_in = reg[IF_ID.ins_reg_out << 6 >> 27];
	ID_EX.$rt_in = reg[IF_ID.ins_reg_out << 11 >> 27];
	
	short temp = IF_ID.ins_reg_out << 16 >> 16;
	ID_EX.extended_imme_in = temp;
	ID_EX.rt_in = IF_ID.ins_reg_out << 11 >> 27;
	ID_EX.rd_in = IF_ID.ins_reg_out << 16 >> 27;
	
	if (opcode == BEQ) {
		if (ID_EX.$rs_in == ID_EX.$rt_in) {
			ID_EX.pc_src_in = 1;
			ID_EX.pc_in = IF_ID.pc_plus_four_out + 4 * ID_EX.extended_imme_in;
		} else ID_EX.pc_src_in = 0;
	} else if (opcode == BNE) {
		if (ID_EX.$rs_in != ID_EX.$rt_in) {
			ID_EX.pc_src_in = 1;
			ID_EX.pc_in = IF_ID.pc_plus_four_out + 4 * ID_EX.extended_imme_in;
		} else ID_EX.pc_src_in = 0;
	} else if (opcode == BGTZ) {
		int intRs = ID_EX.$rs_in;
		if (intRs > 0) {
			ID_EX.pc_src_in = 1;
			ID_EX.pc_in = IF_ID.pc_plus_four_out + 4 * ID_EX.extended_imme_in;
		} else ID_EX.pc_src_in = 0;
	} else if (opcode == J || opcode == JAL) {
		ID_EX.pc_src_in = 1;
		ID_EX.pc_in = (IF_ID.pc_plus_four_out << 28 >> 28) | (ID_EX.extended_imme_in << 6 >> 6);
	} else if (opcode == JR) {
		ID_EX.pc_src_in = 1;
		ID_EX.pc_in = ID_EX.$rs_in;
	} else ID_EX.pc_src_in = 0;
}

void CONTROL() {
	if (opcode == R) {
		ID_EX.reg_dst_in = 1;
		ID_EX.alu_src_in = 0;
	} else if (opcode == JAL) {
		ID_EX.reg_dst_in = 2;
		ID_EX.alu_src_in = 1;
	} else {
		ID_EX.reg_dst_in = 0;
		ID_EX.alu_src_in = 1;
	}
	
	if (opcode == LW || opcode == LH || opcode == LHU || opcode == LB || opcode == LBU) ID_EX.mem_read_in = 1;
	else ID_EX.mem_read_in = 0;
	if (opcode == SW || opcode == SH || opcode == SB) ID_EX.mem_write_in = 1;
	else ID_EX.mem_write_in = 0;
	if (opcode == LW || opcode == SW) ID_EX.mem_op_in = MEM_WORD;
	else if (opcode == LH || opcode == SH) ID_EX.mem_op_in = MEM_HALF;
	else if (opcode == LB || opcode == SB) ID_EX.mem_op_in = MEM_BYTE;
	else if (opcode == LHU) ID_EX.mem_op_in = MEM_HALF_UNSIGN;
	else if (opcode == LBU) ID_EX.mem_op_in = MEM_BYTE_UNSIGN;
	else ID_EX.mem_op_in = ERROR;
	
	if (opcode == R) {
		if (funct != JR) ID_EX.reg_write_in = 1;
		else ID_EX.reg_write_in = 0;
	} else if (opcode == ADDI || opcode == ADDIU || opcode == LW || opcode == LH || opcode == LHU || opcode == LB 
	|| opcode == LBU || opcode == LUI || opcode == ANDI || opcode == ORI || opcode == NORI || opcode == SLTI || opcode == JAL) {
		ID_EX.reg_write_in = 1;
	} else ID_EX.reg_write_in = 0;
	if (opcode == LW || opcode == LH || opcode == LHU || opcode == LB || opcode == LBU) ID_EX.mem_to_reg_in = 1;
	else ID_EX.mem_to_reg_in = 0;
}

void ID_EX_READY() {
	ID_EX.ins_reg_out = ID_EX.ins_reg_in;
	
	ID_EX.reg_dst_out = ID_EX.reg_dst_in;
	ID_EX.alu_src_out = ID_EX.alu_src_in;
	
	ID_EX.mem_read_out = ID_EX.mem_read_in;
	ID_EX.mem_write_out = ID_EX.mem_write_in;
	ID_EX.mem_op_out = ID_EX.mem_op_in;
	
	ID_EX.reg_write_out = ID_EX.reg_write_in;
	ID_EX.mem_to_reg_out = ID_EX.mem_to_reg_in;
	
	ID_EX.$rs_out = ID_EX.$rs_in;
	ID_EX.$rt_out = ID_EX.$rt_in;
	
	ID_EX.extended_imme_out = ID_EX.extended_imme_in;
	ID_EX.rt_out = ID_EX.rt_in;
	ID_EX.rd_out = ID_EX.rd_in;
	
	ID_EX.pc_src_out = ID_EX.pc_src_in;
	ID_EX.pc_out = ID_EX.pc_in;
}

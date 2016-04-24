#include "defines.h"
#include "global.h"
#include "IF.h"
#include "ID.h"
#include "EX.h"

static unsigned address;

void ID() {
	if (STALL) return;
	if (ID_EX.pc_src_out) {
		ID_EX.pc_plus_four_in = 0;
		ID_EX.opcode_in = 0;
		ID_EX.funct_in = 0;
		ID_EX.shamt_in = 0;
		ID_EX.rt_in = 0;
		ID_EX.rd_in = 0;
		ID_EX.rs_in = 0;
		
		ID_EX.reg_dst_in = 0;
		ID_EX.alu_src_in = 0;
		
		ID_EX.mem_read_in = 0;
		ID_EX.mem_write_in = 0;
		ID_EX.mem_op_in = 0;
		
		ID_EX.reg_write_in = 0;
		ID_EX.mem_to_reg_in = 0;
		
		ID_EX.$rs_in = 0;
		ID_EX.$rt_in = 0;
		ID_EX.extended_imme_in = 0;
		
		ID_EX.pc_src_in = 0;
		ID_EX.pc_in = 0;
		ID_EX.reg_to_write_in = 0;
		return;
	}
	ID_EX.pc_plus_four_in = IF_ID.pc_plus_four_out;
	ID_EX.opcode_in = IF_ID.opcode_out;
	ID_EX.funct_in = IF_ID.funct_out;
	ID_EX.shamt_in = IF_ID.ins_reg_out << 21 >> 27;
	ID_EX.rs_in = IF_ID.rs_out;
	ID_EX.rt_in = IF_ID.rt_out;
	ID_EX.rd_in = IF_ID.ins_reg_out << 16 >> 27;
	address = IF_ID.ins_reg_out << 6 >> 6;
	
	CONTROL();
	
	if (FWD_RS_TO_ID) ID_EX.$rs_in = EX_DM.alu_result_out;
	else ID_EX.$rs_in = reg[ID_EX.rs_in];
	if (FWD_RT_TO_ID) ID_EX.$rt_in = EX_DM.alu_result_out;
	else ID_EX.$rt_in = reg[ID_EX.rt_in];
	short temp = IF_ID.ins_reg_out << 16 >> 16;
	ID_EX.extended_imme_in = temp;
	
	if (ID_EX.reg_dst_in == 0) ID_EX.reg_to_write_in = ID_EX.rt_in;
	else if (ID_EX.reg_dst_in == 1) ID_EX.reg_to_write_in = ID_EX.rd_in;
	else if (ID_EX.reg_dst_in == 2) ID_EX.reg_to_write_in = 31;
	else printf("ID_EX.reg_dst_in error.\n");
	
	if (ID_EX.opcode_in == BEQ) {
		if (ID_EX.$rs_in == ID_EX.$rt_in) {
			ID_EX.pc_src_in = 1;
			ID_EX.pc_in = IF_ID.pc_plus_four_out + 4 * ID_EX.extended_imme_in;
		} else ID_EX.pc_src_in = 0;
	} else if (ID_EX.opcode_in == BNE) {
		if (ID_EX.$rs_in != ID_EX.$rt_in) {
			ID_EX.pc_src_in = 1;
			ID_EX.pc_in = IF_ID.pc_plus_four_out + 4 * ID_EX.extended_imme_in;
		} else ID_EX.pc_src_in = 0;
	} else if (ID_EX.opcode_in == BGTZ) {
		int intRs = ID_EX.$rs_in;
		if (intRs > 0) {
			ID_EX.pc_src_in = 1;
			ID_EX.pc_in = IF_ID.pc_plus_four_out + 4 * ID_EX.extended_imme_in;
		} else ID_EX.pc_src_in = 0;
	} else if (ID_EX.opcode_in == J || ID_EX.opcode_in == JAL) {
		ID_EX.pc_src_in = 1;
		ID_EX.pc_in = (IF_ID.pc_plus_four_out >> 28 << 28) | (address << 2);
	} else if (ID_EX.opcode_in == R && ID_EX.funct_in == JR) {
		ID_EX.pc_src_in = 1;
		ID_EX.pc_in = ID_EX.$rs_in;
	} else ID_EX.pc_src_in = 0;
	
	if (ID_EX.opcode_in == HALT) ID_HALT = 1;
	else ID_HALT = 0;
}

void CONTROL() {
	if (ID_EX.opcode_in == R) {
		ID_EX.reg_dst_in = 1;
		ID_EX.alu_src_in = 0;
	} else if (ID_EX.opcode_in == JAL) {
		ID_EX.reg_dst_in = 2;
		ID_EX.alu_src_in = 1;
	} else {
		ID_EX.reg_dst_in = 0;
		ID_EX.alu_src_in = 1;
	}
	
	if (ID_EX.opcode_in == LW || ID_EX.opcode_in == LH || ID_EX.opcode_in == LHU || ID_EX.opcode_in == LB || ID_EX.opcode_in == LBU) ID_EX.mem_read_in = 1;
	else ID_EX.mem_read_in = 0;
	if (ID_EX.opcode_in == SW || ID_EX.opcode_in == SH || ID_EX.opcode_in == SB) ID_EX.mem_write_in = 1;
	else ID_EX.mem_write_in = 0;
	if (ID_EX.opcode_in == LW || ID_EX.opcode_in == SW) ID_EX.mem_op_in = MEM_WORD;
	else if (ID_EX.opcode_in == LH || ID_EX.opcode_in == SH) ID_EX.mem_op_in = MEM_HALF;
	else if (ID_EX.opcode_in == LB || ID_EX.opcode_in == SB) ID_EX.mem_op_in = MEM_BYTE;
	else if (ID_EX.opcode_in == LHU) ID_EX.mem_op_in = MEM_HALF_UNSIGN;
	else if (ID_EX.opcode_in == LBU) ID_EX.mem_op_in = MEM_BYTE_UNSIGN;
	else ID_EX.mem_op_in = ERROR;
	
	if (ID_EX.opcode_in == R) {
		if (ID_EX.funct_in != JR) ID_EX.reg_write_in = 1;
		else ID_EX.reg_write_in = 0;
	} else if (ID_EX.opcode_in == ADDI || ID_EX.opcode_in == ADDIU || ID_EX.opcode_in == LW || ID_EX.opcode_in == LH || ID_EX.opcode_in == LHU || ID_EX.opcode_in == LB 
	|| ID_EX.opcode_in == LBU || ID_EX.opcode_in == LUI || ID_EX.opcode_in == ANDI || ID_EX.opcode_in == ORI || ID_EX.opcode_in == NORI || ID_EX.opcode_in == SLTI || ID_EX.opcode_in == JAL) {
		ID_EX.reg_write_in = 1;
	} else ID_EX.reg_write_in = 0;
	if (ID_EX.opcode_in == LW || ID_EX.opcode_in == LH || ID_EX.opcode_in == LHU || ID_EX.opcode_in == LB || ID_EX.opcode_in == LBU) ID_EX.mem_to_reg_in = 1;
	else ID_EX.mem_to_reg_in = 0;
}

void ID_EX_READY() {
	if (STALL) {
		ID_EX.pc_plus_four_out = 0;
		ID_EX.opcode_out = 0;
		ID_EX.funct_out = 0;
		ID_EX.shamt_out = 0;
		ID_EX.rt_out = 0;
		ID_EX.rd_out = 0;
		ID_EX.rs_out = 0;
		
		ID_EX.reg_dst_out = 0;
		ID_EX.alu_src_out = 0;
		
		ID_EX.mem_read_out = 0;
		ID_EX.mem_write_out = 0;
		ID_EX.mem_op_out = 0;
		
		ID_EX.reg_write_out = 0;
		ID_EX.mem_to_reg_out = 0;
		
		ID_EX.$rs_out = 0;
		ID_EX.$rt_out = 0;
		ID_EX.extended_imme_out = 0;
		
		ID_EX.pc_src_out = 0;
		ID_EX.pc_out = 0;
		ID_EX.reg_to_write_out = 0;
	} else {
		ID_EX.pc_plus_four_out = ID_EX.pc_plus_four_in;
		ID_EX.opcode_out = ID_EX.opcode_in;
		ID_EX.funct_out = ID_EX.funct_in;
		ID_EX.shamt_out = ID_EX.shamt_in;
		ID_EX.rt_out = ID_EX.rt_in;
		ID_EX.rd_out = ID_EX.rd_in;
		ID_EX.rs_out = ID_EX.rs_in;
		
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
		
		ID_EX.pc_src_out = ID_EX.pc_src_in;
		ID_EX.pc_out = ID_EX.pc_in;
		ID_EX.reg_to_write_out = ID_EX.reg_to_write_in;
	}
}

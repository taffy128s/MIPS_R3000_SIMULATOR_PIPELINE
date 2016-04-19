#include "defines.h"
#include "global.h"
#include "EX.h"
#include "ID.h"

static int intRight, intLeft, temp;

void EX() {
	EX_DM.opcode_in = ID_EX.opcode_out;
	EX_DM.rd_in = ID_EX.rd_out;
	if (FWD_RT_TO_EX) EX_DM.rt_in = EX_DM.rt_out;
	else EX_DM.rt_in = ID_EX.rt_out;
	if (FWD_RS_TO_EX) EX_DM.rs_in = EX_DM.rs_out;
	else EX_DM.rs_in = ID_EX.rs_out;
	EX_DM.funct_in = ID_EX.funct_out;
	EX_DM.shamt_in = ID_EX.shamt_out;
	
	EX_DM.mem_read_in = ID_EX.mem_read_out;
	EX_DM.mem_write_in = ID_EX.mem_write_out;
	EX_DM.mem_op_in = ID_EX.mem_op_out;
	
	EX_DM.reg_write_in = ID_EX.reg_write_out;
	EX_DM.mem_to_reg_in = ID_EX.mem_to_reg_out;
	
	unsigned left = ID_EX.$rs_out;
	unsigned right;
	if (ID_EX.alu_src_out == 0) right = ID_EX.$rt_out;
	else right = ID_EX.extended_imme_out;
	switch (EX_DM.opcode_in) {
		case R:
			switch (EX_DM.funct_in) {
				case ADD:
					EX_DM.alu_result_in = left + right;
					break;
				case ADDU:
					EX_DM.alu_result_in = left + right;
					break;
				case SUB:
					EX_DM.alu_result_in = left - right;
					break;
				case AND:
					EX_DM.alu_result_in = left & right;
					break;
				case OR:
					EX_DM.alu_result_in = left | right;
					break;
				case XOR:
					EX_DM.alu_result_in = left ^ right;
					break;
				case NOR:
					EX_DM.alu_result_in = ~(left | right);
					break;
				case NAND:
					EX_DM.alu_result_in = ~(left & right);
					break;
				case SLT:
					intRight = right, intLeft = left;
					EX_DM.alu_result_in = (intLeft < intRight) ? 1 : 0;
					break;
				case SLL:
					EX_DM.alu_result_in = right << EX_DM.shamt_in;
					break;
				case SRL:
					EX_DM.alu_result_in = right >> EX_DM.shamt_in;
					break;
				case SRA:
					temp = right;
					EX_DM.alu_result_in = right >> EX_DM.shamt_in;
					break;
				default:
					//printf("There's an error at EX.c.\n");
					break;
			}
			break;
		case ADDI:
			EX_DM.alu_result_in = left + right;
			break;
		case ADDIU:
			EX_DM.alu_result_in = left + right;
			break;
		case LW:
			EX_DM.alu_result_in = left + right;
			break;
		case LH:
			EX_DM.alu_result_in = left + right;
			break;
		case LHU:
			EX_DM.alu_result_in = left + right;
			break;
		case LB:
			EX_DM.alu_result_in = left + right;
			break;
		case LBU:
			EX_DM.alu_result_in = left + right;
			break;
		case SW:
			EX_DM.alu_result_in = left + right;
			break;
		case SH:
			EX_DM.alu_result_in = left + right;
			break;
		case SB:
			EX_DM.alu_result_in = left + right;
			break;
		case LUI:
			EX_DM.alu_result_in = right << 16;
			break;
		case ANDI:
			EX_DM.alu_result_in = left & right;
			break;
		case ORI:
			EX_DM.alu_result_in = left | right;
			break;
		case NORI:
			EX_DM.alu_result_in = ~(left | right);
			break;
		case SLTI:
			intRight = right, intLeft = left;
			EX_DM.alu_result_in = (intLeft < intRight) ? 1 : 0;
			break;
		default:
			//printf("There's an error at EX.c.\n");
			break;
	}
	
	EX_DM.$rt_in = ID_EX.$rt_out;
	if (ID_EX.reg_dst_out == 0) EX_DM.reg_to_write_in = ID_EX.rt_out;
	else if (ID_EX.reg_dst_out == 1) EX_DM.reg_to_write_in = ID_EX.rd_out;
	else if (ID_EX.reg_dst_out == 1) EX_DM.reg_to_write_in = 31;
	else printf("There's an error at EX.c.\n");
	
	if (EX_DM.opcode_in == HALT) EX_HALT = 1;
	else EX_HALT = 0;
}

void EX_DM_READY() {
	EX_DM.mem_read_out = EX_DM.mem_read_in;
	EX_DM.mem_write_out = EX_DM.mem_write_in;
	EX_DM.mem_op_out = EX_DM.mem_op_in;
	EX_DM.rd_out = EX_DM.rd_in;
	EX_DM.rs_out = EX_DM.rs_in;
	EX_DM.rt_out = EX_DM.rt_in;
	EX_DM.shamt_out = EX_DM.shamt_in;
	EX_DM.funct_out = EX_DM.funct_in;
	
	EX_DM.reg_write_out = EX_DM.reg_write_in;
	EX_DM.mem_to_reg_out = EX_DM.mem_to_reg_in;
	
	EX_DM.opcode_out = EX_DM.opcode_in;
	EX_DM.$rt_out = EX_DM.$rt_in;
	EX_DM.reg_to_write_out = EX_DM.reg_to_write_in;
	EX_DM.alu_result_out = EX_DM.alu_result_in;
}

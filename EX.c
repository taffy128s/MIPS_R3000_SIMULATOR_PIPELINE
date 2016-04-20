#include "defines.h"
#include "global.h"
#include "EX.h"
#include "ID.h"

static int intRight, intLeft, temp;
static unsigned left_sign, right_sign, result_sign;

void EX() {
	EX_DM.opcode_in = ID_EX.opcode_out;
	EX_DM.rd_in = ID_EX.rd_out;
	EX_DM.rt_in = ID_EX.rt_out;
	EX_DM.rs_in = ID_EX.rs_out;
	EX_DM.funct_in = ID_EX.funct_out;
	EX_DM.shamt_in = ID_EX.shamt_out;
	
	EX_DM.mem_read_in = ID_EX.mem_read_out;
	EX_DM.mem_write_in = ID_EX.mem_write_out;
	EX_DM.mem_op_in = ID_EX.mem_op_out;
	
	EX_DM.reg_write_in = ID_EX.reg_write_out;
	EX_DM.mem_to_reg_in = ID_EX.mem_to_reg_out;
	
	unsigned left, right;
	if (EX_DM.opcode_in == SW || EX_DM.opcode_in == SH || EX_DM.opcode_in == SB) {
		if (FWD_RS_TO_EX) left = EX_DM.alu_result_out;
		else left = ID_EX.$rs_out;
		right = ID_EX.extended_imme_out;
		if (FWD_RT_TO_EX) EX_DM.write_data_in = EX_DM.alu_result_out;
		else EX_DM.write_data_in = ID_EX.$rt_out;
	} else {
		if (FWD_RS_TO_EX) left = EX_DM.alu_result_out;
		else left = ID_EX.$rs_out;
		if (ID_EX.alu_src_out == 0) {
			if (FWD_RT_TO_EX) right = EX_DM.alu_result_out;
			else right = ID_EX.$rt_out;
		} else right = ID_EX.extended_imme_out;
	}
	left_sign = left >> 31;
	right_sign = right >> 31;
	switch (EX_DM.opcode_in) {
		case R:
			switch (EX_DM.funct_in) {
				case ADD:
					EX_DM.alu_result_in = left + right;
					result_sign = EX_DM.alu_result_in >> 31;
					if (left_sign == right_sign && left_sign != result_sign) num_overflow = 1;
					break;
				case ADDU:
					EX_DM.alu_result_in = left + right;
					break;
				case SUB:
					right_sign = (-right) >> 31;
					EX_DM.alu_result_in = left - right;
					result_sign = EX_DM.alu_result_in >> 31;
					if (left_sign == right_sign && left_sign != result_sign) num_overflow = 1;
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
					EX_DM.alu_result_in = temp >> EX_DM.shamt_in;
					break;
				default:
					break;
			}
			break;
		case ADDI:
			EX_DM.alu_result_in = left + right;
			result_sign = EX_DM.alu_result_in >> 31;
			if (left_sign == right_sign && left_sign != result_sign) num_overflow = 1;
			break;
		case ADDIU:
			EX_DM.alu_result_in = left + right;
			break;
		case LW:
			EX_DM.alu_result_in = left + right;
			result_sign = EX_DM.alu_result_in >> 31;
			if (left_sign == right_sign && left_sign != result_sign) num_overflow = 1;
			break;
		case LH:
			EX_DM.alu_result_in = left + right;
			result_sign = EX_DM.alu_result_in >> 31;
			if (left_sign == right_sign && left_sign != result_sign) num_overflow = 1;
			break;
		case LHU:
			EX_DM.alu_result_in = left + right;
			result_sign = EX_DM.alu_result_in >> 31;
			if (left_sign == right_sign && left_sign != result_sign) num_overflow = 1;
			break;
		case LB:
			EX_DM.alu_result_in = left + right;
			result_sign = EX_DM.alu_result_in >> 31;
			if (left_sign == right_sign && left_sign != result_sign) num_overflow = 1;
			break;
		case LBU:
			EX_DM.alu_result_in = left + right;
			result_sign = EX_DM.alu_result_in >> 31;
			if (left_sign == right_sign && left_sign != result_sign) num_overflow = 1;
			break;
		case SW:
			EX_DM.alu_result_in = left + right;
			result_sign = EX_DM.alu_result_in >> 31;
			if (left_sign == right_sign && left_sign != result_sign) num_overflow = 1;
			break;
		case SH:
			EX_DM.alu_result_in = left + right;
			result_sign = EX_DM.alu_result_in >> 31;
			if (left_sign == right_sign && left_sign != result_sign) num_overflow = 1;
			break;
		case SB:
			EX_DM.alu_result_in = left + right;
			result_sign = EX_DM.alu_result_in >> 31;
			if (left_sign == right_sign && left_sign != result_sign) num_overflow = 1;
			break;
		case LUI:
			EX_DM.alu_result_in = right << 16;
			break;
		case ANDI:
			EX_DM.alu_result_in = left & (unsigned short) right;
			break;
		case ORI:
			EX_DM.alu_result_in = left | (unsigned short) right;
			break;
		case NORI:
			EX_DM.alu_result_in = ~(left | (unsigned short) right);
			break;
		case SLTI:
			intRight = right, intLeft = left;
			EX_DM.alu_result_in = (intLeft < intRight) ? 1 : 0;
			break;
		case JAL:
			EX_DM.alu_result_in = ID_EX.pc_plus_four_out;
		default:
			break;
	}
	
	EX_DM.$rs_in = ID_EX.$rs_out;
	EX_DM.$rt_in = ID_EX.$rt_out;
	EX_DM.reg_to_write_in = ID_EX.reg_to_write_out;
	
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
	EX_DM.write_data_out = EX_DM.write_data_in;
	
	EX_DM.opcode_out = EX_DM.opcode_in;
	EX_DM.$rs_out = EX_DM.$rs_in;
	EX_DM.$rt_out = EX_DM.$rt_in;
	EX_DM.reg_to_write_out = EX_DM.reg_to_write_in;
	EX_DM.alu_result_out = EX_DM.alu_result_in;
}

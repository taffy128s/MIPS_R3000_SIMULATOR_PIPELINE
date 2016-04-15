#include "defines.h"
#include "global.h"
#include "EX.h"
#include "ID.h"

static unsigned opcode, funct, shamt;
static int intRight, intLeft, temp;

void EX() {
	opcode = ID_EX.ins_reg_out >> 26;
	funct = ID_EX.ins_reg_out << 26 >> 26;
	shamt = ID_EX.ins_reg_out << 21 >> 27;
	
	unsigned left = ID_EX.$rs_out;
	unsigned right;
	if (ID_EX.alu_src_out == 0) right = ID_EX.$rt_out;
	else right = ID_EX.extended_imme_out;
	switch (opcode) {
		case R:
			switch (funct) {
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
					EX_DM.alu_result_in = right << shamt;
					break;
				case SRL:
					EX_DM.alu_result_in = right >> shamt;
					break;
				case SRA:
					temp = right;
					EX_DM.alu_result_in = right >> shamt;
					break;
				default:
					printf("There's an error at EX.c.\n");
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
			printf("There's an error at EX.c.\n");
	}
	
	EX_DM.$rt_in = ID_EX.$rt_out;
	if (ID_EX.reg_dst_out == 0) EX_DM.reg_to_write_in = ID_EX.rt_out;
	else EX_DM.reg_to_write_in = ID_EX.rd_out;
}

void EX_DM_READY() {
	EX_DM.$rt_out = EX_DM.$rt_in;
	EX_DM.reg_to_write_out = EX_DM.reg_to_write_in;
	EX_DM.alu_result_out = EX_DM.alu_result_in;
}

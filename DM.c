#include "defines.h"
#include "global.h"
#include "DM.h"
#include "EX.h"

void DM() {
	int i;
	DM_WB.opcode_in = EX_DM.opcode_out;
	DM_WB.reg_write_in = EX_DM.reg_write_out;
	DM_WB.mem_to_reg_in = EX_DM.mem_to_reg_out;
	DM_WB.alu_result_in = EX_DM.alu_result_out;
	DM_WB.reg_to_write_in = EX_DM.reg_to_write_out;
	
	if (EX_DM.mem_write_out == 1 && EX_DM.mem_read_out == 1)
		printf("Fatal error at DM.c.\n");
	
	if (EX_DM.mem_write_out == 1) {
		if (EX_DM.mem_op_out == MEM_WORD) {
			dMemory[EX_DM.alu_result_out] = EX_DM.$rt_out >> 24;
			dMemory[EX_DM.alu_result_out + 1] = EX_DM.$rt_out << 8 >> 24;
			dMemory[EX_DM.alu_result_out + 2] = EX_DM.$rt_out << 16 >> 24;
			dMemory[EX_DM.alu_result_out + 3] = EX_DM.$rt_out << 24 >> 24;
		} else if (EX_DM.mem_op_out == MEM_HALF) {
			dMemory[EX_DM.alu_result_out] = EX_DM.$rt_out << 16 >> 24;
			dMemory[EX_DM.alu_result_out + 1] = EX_DM.$rt_out << 24 >> 24;
		} else if (EX_DM.mem_op_out == MEM_BYTE) {
			dMemory[EX_DM.alu_result_out] = EX_DM.$rt_out << 24 >> 24;
		} else printf("Error at DM.c.\n");
	}
	
	if (EX_DM.mem_read_out == 1) {
		if (EX_DM.mem_op_out == MEM_WORD) {
			for (i = 0; i < 4; i++) 
				DM_WB.read_data_in = (DM_WB.read_data_in << 8) | (unsigned char) dMemory[EX_DM.alu_result_out + i];
		} else if (EX_DM.mem_op_out == MEM_HALF) {
			DM_WB.read_data_in = dMemory[EX_DM.alu_result_out];
			DM_WB.read_data_in = (DM_WB.read_data_in << 8) | (unsigned char) dMemory[EX_DM.alu_result_out + 1];
		} else if (EX_DM.mem_op_out == MEM_HALF_UNSIGN) {
			DM_WB.read_data_in = (unsigned char) dMemory[EX_DM.alu_result_out];
			DM_WB.read_data_in = (DM_WB.read_data_in << 8) | (unsigned char) dMemory[EX_DM.alu_result_out + 1];
		} else if (EX_DM.mem_op_out == MEM_BYTE) {
			DM_WB.read_data_in = dMemory[EX_DM.alu_result_out];
		} else if (EX_DM.mem_op_out == MEM_BYTE_UNSIGN) {
			DM_WB.read_data_in = (unsigned char) dMemory[EX_DM.alu_result_out];
		} else printf("Error at DM.c.\n");
	} 
	
	if (DM_WB.opcode_in == HALT) DM_HALT = 1;
	else DM_HALT = 0;
}

void DM_WB_READY() {
	DM_WB.opcode_out = DM_WB.opcode_in;
	DM_WB.reg_write_out = DM_WB.reg_write_in;
	DM_WB.mem_to_reg_out = DM_WB.mem_to_reg_in;
	DM_WB.alu_result_out = DM_WB.alu_result_in;
	DM_WB.reg_to_write_out = DM_WB.reg_to_write_in;
	
	DM_WB.read_data_out = DM_WB.read_data_in;
}

#include "defines.h"
#include "global.h"
#include "DM.h"
#include "EX.h"

void DM() {
	int i;
	DM_WB.opcode_in = EX_DM.opcode_out;
	DM_WB.rd_in = EX_DM.rd_out;
	DM_WB.rt_in = EX_DM.rt_out;
	DM_WB.rs_in = EX_DM.rs_out;
	DM_WB.shamt_in = EX_DM.shamt_out;
	DM_WB.funct_in = EX_DM.funct_out;
	
	DM_WB.reg_write_in = EX_DM.reg_write_out;
	DM_WB.mem_to_reg_in = EX_DM.mem_to_reg_out;
	
	DM_WB.alu_result_in = EX_DM.alu_result_out;
	DM_WB.reg_to_write_in = EX_DM.reg_to_write_out;
	
	if (EX_DM.mem_write_out == 1 && EX_DM.mem_read_out == 1)
		printf("Fatal error at DM.c.\n");
	
	if (EX_DM.mem_write_out == 1) {
		if (EX_DM.mem_op_out == MEM_WORD) {
			if (EX_DM.alu_result_out >= 1024 || EX_DM.alu_result_out + 3 >= 1024) {
				mem_overflow = 1;
			}
			if (EX_DM.alu_result_out % 4) {
				mem_misalign = 1;
			}
			if (mem_overflow || mem_misalign) return;
			dMemory[EX_DM.alu_result_out] = EX_DM.write_data_out >> 24;
			dMemory[EX_DM.alu_result_out + 1] = EX_DM.write_data_out << 8 >> 24;
			dMemory[EX_DM.alu_result_out + 2] = EX_DM.write_data_out << 16 >> 24;
			dMemory[EX_DM.alu_result_out + 3] = EX_DM.write_data_out << 24 >> 24;
		} else if (EX_DM.mem_op_out == MEM_HALF) {
			if (EX_DM.alu_result_out >= 1024 || EX_DM.alu_result_out + 1 >= 1024) {
				mem_overflow = 1;
			}
			if (EX_DM.alu_result_out % 2) {
				mem_misalign = 1;
			}
			if (mem_overflow || mem_misalign) return;
			dMemory[EX_DM.alu_result_out] = EX_DM.write_data_out << 16 >> 24;
			dMemory[EX_DM.alu_result_out + 1] = EX_DM.write_data_out << 24 >> 24;
		} else if (EX_DM.mem_op_out == MEM_BYTE) {
			if (EX_DM.alu_result_out >= 1024) {
				mem_overflow = 1;
			}
			if (mem_overflow || mem_misalign) return;
			dMemory[EX_DM.alu_result_out] = EX_DM.write_data_out << 24 >> 24;
		} else printf("Error at DM.c.\n");
	}
	
	if (EX_DM.mem_read_out == 1) {
		if (EX_DM.mem_op_out == MEM_WORD) {
			if (EX_DM.alu_result_out >= 1024 || EX_DM.alu_result_out + 3 >= 1024) {
				mem_overflow = 1;
			}
			if (EX_DM.alu_result_out % 4) {
				mem_misalign = 1;
			}
			if (mem_overflow || mem_misalign) return;
			for (i = 0; i < 4; i++) 
				DM_WB.read_data_in = (DM_WB.read_data_in << 8) | (unsigned char) dMemory[EX_DM.alu_result_out + i];
		} else if (EX_DM.mem_op_out == MEM_HALF) {
			if (EX_DM.alu_result_out >= 1024 || EX_DM.alu_result_out + 1 >= 1024) {
				mem_overflow = 1;
			}
			if (EX_DM.alu_result_out % 2) {
				mem_misalign = 1;
			}
			if (mem_overflow || mem_misalign) return;
			DM_WB.read_data_in = dMemory[EX_DM.alu_result_out];
			DM_WB.read_data_in = (DM_WB.read_data_in << 8) | (unsigned char) dMemory[EX_DM.alu_result_out + 1];
		} else if (EX_DM.mem_op_out == MEM_HALF_UNSIGN) {
			if (EX_DM.alu_result_out >= 1024 || EX_DM.alu_result_out + 1 >= 1024) {
				mem_overflow = 1;
			}
			if (EX_DM.alu_result_out % 2) {
				mem_misalign = 1;
			}
			if (mem_overflow || mem_misalign) return;
			DM_WB.read_data_in = (unsigned char) dMemory[EX_DM.alu_result_out];
			DM_WB.read_data_in = (DM_WB.read_data_in << 8) | (unsigned char) dMemory[EX_DM.alu_result_out + 1];
		} else if (EX_DM.mem_op_out == MEM_BYTE) {
			if (EX_DM.alu_result_out >= 1024) {
				mem_overflow = 1;
			}
			DM_WB.read_data_in = dMemory[EX_DM.alu_result_out];
		} else if (EX_DM.mem_op_out == MEM_BYTE_UNSIGN) {
			if (EX_DM.alu_result_out >= 1024) {
				mem_overflow = 1;
			}
			if (mem_overflow || mem_misalign) return;
			DM_WB.read_data_in = (unsigned char) dMemory[EX_DM.alu_result_out];
		} else printf("Error at DM.c.\n");
	}
	
	if (DM_WB.opcode_in == HALT) DM_HALT = 1;
	else DM_HALT = 0;
}

void DM_WB_READY() {
	DM_WB.opcode_out = DM_WB.opcode_in;
	DM_WB.rd_out = DM_WB.rd_in;
	DM_WB.rs_out = DM_WB.rs_in;
	DM_WB.rt_out = DM_WB.rt_in;
	DM_WB.funct_out = DM_WB.funct_in;
	DM_WB.shamt_out = DM_WB.shamt_in;
	
	DM_WB.reg_write_out = DM_WB.reg_write_in;
	DM_WB.mem_to_reg_out = DM_WB.mem_to_reg_in;
	DM_WB.alu_result_out = DM_WB.alu_result_in;
	DM_WB.reg_to_write_out = DM_WB.reg_to_write_in;
	
	DM_WB.read_data_out = DM_WB.read_data_in;
}

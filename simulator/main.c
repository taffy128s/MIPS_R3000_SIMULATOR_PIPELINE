#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "defines.h"
#include "global.h"
#include "initial.h"
#include "IF.h"
#include "ID.h"
#include "EX.h"
#include "DM.h"
#include "WB.h"

static pthread_barrier_t barrier1_1, barrier1_2;
static pthread_barrier_t barrier2_1, barrier2_2;
static pthread_barrier_t barrier3_1, barrier3_2;
static pthread_barrier_t barrier4_1, barrier4_2;
static pthread_barrier_t barrier5_1, barrier5_2;
static pthread_barrier_t barrier_ID_WB;
static pthread_barrier_t barrier_RETURN;
static pthread_barrier_t barrier_CHECK;
static pthread_barrier_t barrier_PRINT;

void checkStall() {
	STALL = 0;
	if (ID_EX.pc_src_out == 1) return;
	// EX_DM to ID (not branches) stall 
	if (IF_ID.opcode_out == R && IF_ID.funct_out != SLL && IF_ID.funct_out != SRL && IF_ID.funct_out != SRA && IF_ID.funct_out != JR) {
		if (EX_DM.reg_write_out && (EX_DM.reg_to_write_out != 0) && (EX_DM.reg_to_write_out == IF_ID.rs_out) && !(ID_EX.reg_write_out && (ID_EX.reg_to_write_out != 0) && (ID_EX.reg_to_write_out == IF_ID.rs_out))) STALL = 1;
		if (EX_DM.reg_write_out && (EX_DM.reg_to_write_out != 0) && (EX_DM.reg_to_write_out == IF_ID.rt_out) && !(ID_EX.reg_write_out && (ID_EX.reg_to_write_out != 0) && (ID_EX.reg_to_write_out == IF_ID.rt_out))) STALL = 1;
	} else if (IF_ID.opcode_out == R && (IF_ID.funct_out == SLL || IF_ID.funct_out == SRL || IF_ID.funct_out == SRA)) {
		if (EX_DM.reg_write_out && (EX_DM.reg_to_write_out != 0) && (EX_DM.reg_to_write_out == IF_ID.rt_out) && !(ID_EX.reg_write_out && (ID_EX.reg_to_write_out != 0) && (ID_EX.reg_to_write_out == IF_ID.rt_out))) STALL = 1;
	} else if (IF_ID.opcode_out != R && IF_ID.opcode_out != LUI && IF_ID.opcode_out != BEQ && IF_ID.opcode_out != BNE && IF_ID.opcode_out != BGTZ
	&& IF_ID.opcode_out != J && IF_ID.opcode_out != JAL && IF_ID.opcode_out != HALT && IF_ID.opcode_out != SW && IF_ID.opcode_out != SH && IF_ID.opcode_out != SB) {
		if (EX_DM.reg_write_out && (EX_DM.reg_to_write_out != 0) && (EX_DM.reg_to_write_out == IF_ID.rs_out) && !(ID_EX.reg_write_out && (ID_EX.reg_to_write_out != 0) && (ID_EX.reg_to_write_out == IF_ID.rs_out))) STALL = 1;
	} else if (IF_ID.opcode_out == SW || IF_ID.opcode_out == SH || IF_ID.opcode_out == SB) {
		if (EX_DM.reg_write_out && (EX_DM.reg_to_write_out != 0) && (EX_DM.reg_to_write_out == IF_ID.rs_out) && !(ID_EX.reg_write_out && (ID_EX.reg_to_write_out != 0) && (ID_EX.reg_to_write_out == IF_ID.rs_out))) STALL = 1;
		if (EX_DM.reg_write_out && (EX_DM.reg_to_write_out != 0) && (EX_DM.reg_to_write_out == IF_ID.rt_out) && !(ID_EX.reg_write_out && (ID_EX.reg_to_write_out != 0) && (ID_EX.reg_to_write_out == IF_ID.rt_out))) STALL = 1;
	}
	// ID_EX to ID (branches) stall
	if (IF_ID.opcode_out == BEQ || IF_ID.opcode_out == BNE) {
		if (ID_EX.reg_write_out && (ID_EX.reg_to_write_out != 0) && (ID_EX.reg_to_write_out == IF_ID.rs_out)) STALL = 1;
		if (ID_EX.reg_write_out && (ID_EX.reg_to_write_out != 0) && (ID_EX.reg_to_write_out == IF_ID.rt_out)) STALL = 1;
	} else if (IF_ID.opcode_out == BGTZ) {
		if (ID_EX.reg_write_out && (ID_EX.reg_to_write_out != 0) && (ID_EX.reg_to_write_out == IF_ID.rs_out)) STALL = 1;
	} else if (IF_ID.opcode_out == R && IF_ID.funct_out == JR) {
		if (ID_EX.reg_write_out && (ID_EX.reg_to_write_out != 0) && (ID_EX.reg_to_write_out == IF_ID.rs_out)) STALL = 1;
	}
	// memory read stall
	if (IF_ID.opcode_out == R && IF_ID.funct_out != SLL && IF_ID.funct_out != SRL && IF_ID.funct_out != SRA && IF_ID.funct_out != JR) {
		if (ID_EX.mem_read_out && (ID_EX.reg_to_write_out != 0) && (ID_EX.reg_to_write_out == IF_ID.rs_out)) STALL = 1;
		if (ID_EX.mem_read_out && (ID_EX.reg_to_write_out != 0) && (ID_EX.reg_to_write_out == IF_ID.rt_out)) STALL = 1;
	} else if (IF_ID.opcode_out == R && (IF_ID.funct_out == SLL || IF_ID.funct_out == SRL || IF_ID.funct_out == SRA)) {
		if (ID_EX.mem_read_out && (ID_EX.reg_to_write_out != 0) && (ID_EX.reg_to_write_out == IF_ID.rt_out)) STALL = 1;
	} else if (IF_ID.opcode_out != R && IF_ID.opcode_out != LUI && IF_ID.opcode_out != BEQ && IF_ID.opcode_out != BNE && IF_ID.opcode_out != BGTZ
	&& IF_ID.opcode_out != J && IF_ID.opcode_out != JAL && IF_ID.opcode_out != HALT && IF_ID.opcode_out != SW && IF_ID.opcode_out != SH && IF_ID.opcode_out != SB) {
		if (ID_EX.mem_read_out && (ID_EX.reg_to_write_out != 0) && (ID_EX.reg_to_write_out == IF_ID.rs_out)) STALL = 1;
	} else if (IF_ID.opcode_out == BNE || IF_ID.opcode_out == BEQ || IF_ID.opcode_out == SW || IF_ID.opcode_out == SH || IF_ID.opcode_out == SB) {
		if (ID_EX.mem_read_out && (ID_EX.reg_to_write_out != 0) && (ID_EX.reg_to_write_out == IF_ID.rs_out)) STALL = 1;
		if (ID_EX.mem_read_out && (ID_EX.reg_to_write_out != 0) && (ID_EX.reg_to_write_out == IF_ID.rt_out)) STALL = 1;
	} else if (IF_ID.opcode_out == BGTZ || (IF_ID.opcode_out == R && IF_ID.funct_out == JR)) {
		if (ID_EX.mem_read_out && (ID_EX.reg_to_write_out != 0) && (ID_EX.reg_to_write_out == IF_ID.rs_out)) STALL = 1;
	}
	// memory read stall
	if (IF_ID.opcode_out == R && IF_ID.funct_out != SLL && IF_ID.funct_out != SRL && IF_ID.funct_out != SRA && IF_ID.funct_out != JR) {
		if (EX_DM.mem_read_out && (EX_DM.reg_to_write_out != 0) && (EX_DM.reg_to_write_out == IF_ID.rs_out) && !(ID_EX.reg_write_out && (ID_EX.reg_to_write_out != 0) && (ID_EX.reg_to_write_out == IF_ID.rs_out))) STALL = 1;
		if (EX_DM.mem_read_out && (EX_DM.reg_to_write_out != 0) && (EX_DM.reg_to_write_out == IF_ID.rt_out) && !(ID_EX.reg_write_out && (ID_EX.reg_to_write_out != 0) && (ID_EX.reg_to_write_out == IF_ID.rt_out))) STALL = 1;
	} else if (IF_ID.opcode_out == R && (IF_ID.funct_out == SLL || IF_ID.funct_out == SRL || IF_ID.funct_out == SRA)) {
		if (EX_DM.mem_read_out && (EX_DM.reg_to_write_out != 0) && (EX_DM.reg_to_write_out == IF_ID.rt_out) && !(ID_EX.reg_write_out && (ID_EX.reg_to_write_out != 0) && (ID_EX.reg_to_write_out == IF_ID.rt_out))) STALL = 1;
	} else if (IF_ID.opcode_out != R && IF_ID.opcode_out != LUI && IF_ID.opcode_out != BEQ && IF_ID.opcode_out != BNE && IF_ID.opcode_out != BGTZ
	&& IF_ID.opcode_out != J && IF_ID.opcode_out != JAL && IF_ID.opcode_out != HALT && IF_ID.opcode_out != SW && IF_ID.opcode_out != SH && IF_ID.opcode_out != SB) {
		if (EX_DM.mem_read_out && (EX_DM.reg_to_write_out != 0) && (EX_DM.reg_to_write_out == IF_ID.rs_out) && !(ID_EX.reg_write_out && (ID_EX.reg_to_write_out != 0) && (ID_EX.reg_to_write_out == IF_ID.rs_out))) STALL = 1;
	} else if (IF_ID.opcode_out == SW || IF_ID.opcode_out == SH || IF_ID.opcode_out == SB) {
		if (EX_DM.mem_read_out && (EX_DM.reg_to_write_out != 0) && (EX_DM.reg_to_write_out == IF_ID.rs_out) && !(ID_EX.reg_write_out && (ID_EX.reg_to_write_out != 0) && (ID_EX.reg_to_write_out == IF_ID.rs_out))) STALL = 1;
		if (EX_DM.mem_read_out && (EX_DM.reg_to_write_out != 0) && (EX_DM.reg_to_write_out == IF_ID.rt_out) && !(ID_EX.reg_write_out && (ID_EX.reg_to_write_out != 0) && (ID_EX.reg_to_write_out == IF_ID.rt_out))) STALL = 1;
	} else if (IF_ID.opcode_out == BNE || IF_ID.opcode_out == BEQ) {
		if (EX_DM.mem_read_out && (EX_DM.reg_to_write_out != 0) && (EX_DM.reg_to_write_out == IF_ID.rs_out)) STALL = 1;
		if (EX_DM.mem_read_out && (EX_DM.reg_to_write_out != 0) && (EX_DM.reg_to_write_out == IF_ID.rt_out)) STALL = 1;
	} else if (IF_ID.opcode_out == BGTZ || (IF_ID.opcode_out == R && IF_ID.funct_out == JR)) {
		if (EX_DM.mem_read_out && (EX_DM.reg_to_write_out != 0) && (EX_DM.reg_to_write_out == IF_ID.rs_out)) STALL = 1;
	}
}

void checkFwdToId() {
	FWD_RS_TO_ID = 0;
	FWD_RT_TO_ID = 0;
	if (IF_ID.opcode_out == BEQ || IF_ID.opcode_out == BNE) {
		if (EX_DM.reg_write_out && (EX_DM.reg_to_write_out != 0) && (EX_DM.reg_to_write_out == IF_ID.rs_out)) FWD_RS_TO_ID = 1;
		if (EX_DM.reg_write_out && (EX_DM.reg_to_write_out != 0) && (EX_DM.reg_to_write_out == IF_ID.rt_out)) FWD_RT_TO_ID = 1;
	} else if (IF_ID.opcode_out == BGTZ) {
		if (EX_DM.reg_write_out && (EX_DM.reg_to_write_out != 0) && (EX_DM.reg_to_write_out == IF_ID.rs_out)) FWD_RS_TO_ID = 1;
	} else if (IF_ID.opcode_out == R && IF_ID.funct_out == JR) {
		if (EX_DM.reg_write_out && (EX_DM.reg_to_write_out != 0) && (EX_DM.reg_to_write_out == IF_ID.rs_out)) FWD_RS_TO_ID = 1;
	}
}

void checkFwdToEx() {
	FWD_RS_TO_EX = 0;
	FWD_RT_TO_EX = 0;
	if (ID_EX.opcode_out == R && ID_EX.funct_out != SLL && ID_EX.funct_out != SRL && ID_EX.funct_out != SRA && ID_EX.funct_out != JR) {
		if (EX_DM.reg_write_out && (EX_DM.reg_to_write_out != 0) && (EX_DM.reg_to_write_out == ID_EX.rs_out)) FWD_RS_TO_EX = 1;
		if (EX_DM.reg_write_out && (EX_DM.reg_to_write_out != 0) && (EX_DM.reg_to_write_out == ID_EX.rt_out)) FWD_RT_TO_EX = 1;
	} else if (ID_EX.opcode_out == R && (ID_EX.funct_out == SLL || ID_EX.funct_out == SRL || ID_EX.funct_out == SRA)) {
		if (EX_DM.reg_write_out && (EX_DM.reg_to_write_out != 0) && (EX_DM.reg_to_write_out == ID_EX.rt_out)) FWD_RT_TO_EX = 1;
	} else if (ID_EX.opcode_out != R && ID_EX.opcode_out != LUI && ID_EX.opcode_out != BEQ && ID_EX.opcode_out != BNE && ID_EX.opcode_out != BGTZ
	&& ID_EX.opcode_out != J && ID_EX.opcode_out != JAL && ID_EX.opcode_out != HALT && ID_EX.opcode_out != SW && ID_EX.opcode_out != SH && ID_EX.opcode_out != SB) {
		if (EX_DM.reg_write_out && (EX_DM.reg_to_write_out != 0) && (EX_DM.reg_to_write_out == ID_EX.rs_out)) FWD_RS_TO_EX = 1;
	} else if (ID_EX.opcode_out == SW || ID_EX.opcode_out == SH || ID_EX.opcode_out == SB) {
		if (EX_DM.reg_write_out && (EX_DM.reg_to_write_out != 0) && (EX_DM.reg_to_write_out == ID_EX.rs_out)) FWD_RS_TO_EX = 1;
		if (EX_DM.reg_write_out && (EX_DM.reg_to_write_out != 0) && (EX_DM.reg_to_write_out == ID_EX.rt_out)) FWD_RT_TO_EX = 1;
	}
}

void printIns(unsigned opcode, unsigned rt, unsigned rd, unsigned funct, unsigned shamt) {
	switch (opcode) {
		case R:
			switch (funct) {
				case ADD:
					fprintf(snap, "ADD");
					break;
				case ADDU:
					fprintf(snap, "ADDU");
					break;
				case SUB:
					fprintf(snap, "SUB");
					break;
				case AND:
					fprintf(snap, "AND");
					break;
				case OR:
					fprintf(snap, "OR");
					break;
				case XOR:
					fprintf(snap, "XOR");
					break;
				case NOR:
					fprintf(snap, "NOR");
					break;
				case NAND:
					fprintf(snap, "NAND");
					break;
				case SLT:
					fprintf(snap, "SLT");
					break;
				case SLL:
					if (rt == 0 && rd == 0 && shamt == 0) fprintf(snap, "NOP");
					else fprintf(snap, "SLL");
					break;
				case SRL:
					fprintf(snap, "SRL");
					break;
				case SRA:
					fprintf(snap, "SRA");
					break;
				case JR:
					fprintf(snap, "JR");
					break;
				default:
					fprintf(snap, "ERROR!!");
					break;
			}
			break;
		case ADDI:
			fprintf(snap, "ADDI");
			break;
		case ADDIU:
			fprintf(snap, "ADDIU");
			break;
		case LW:
			fprintf(snap, "LW");
			break;
		case LH:
			fprintf(snap, "LH");
			break;
		case LHU:
			fprintf(snap, "LHU");
			break;
		case LB:
			fprintf(snap, "LB");
			break;
		case LBU:
			fprintf(snap, "LBU");
			break;
		case SW:
			fprintf(snap, "SW");
			break;
		case SH:
			fprintf(snap, "SH");
			break;
		case SB:
			fprintf(snap, "SB");
			break;
		case LUI:
			fprintf(snap, "LUI");
			break;
		case ANDI:
			fprintf(snap, "ANDI");
			break;
		case ORI:
			fprintf(snap, "ORI");
			break;
		case NORI:
			fprintf(snap, "NORI");
			break;
		case SLTI:
			fprintf(snap, "SLTI");
			break;
		case BEQ:
			fprintf(snap, "BEQ");
			break;
		case BNE:
			fprintf(snap, "BNE");
			break;
		case BGTZ:
			fprintf(snap, "BGTZ");
			break;
		case J:
			fprintf(snap, "J");
			break;
		case JAL:
			fprintf(snap, "JAL");
			break;
		case HALT:
			fprintf(snap, "HALT");
			break;
		default:
			fprintf(snap, "ERROR!!");
			break;
	}
}

void dumpSnap() {
	fprintf(snap, "cycle %u\n", cycle++);
	unsigned i;
	for (i = 0; i < 32; i++) fprintf(snap, "$%02u: 0x%08X\n", i, reg[i]);
	if (ID_EX.pc_src_out == 1) {
		fprintf(snap, "PC: 0x%08X\nIF: 0x", ID_EX.pc_out);
		for (i = 0; i < 4; i++) fprintf(snap, "%02X", iMemory[ID_EX.pc_out + i] & 0xff);
	} else {
		fprintf(snap, "PC: 0x%08X\nIF: 0x", PC);
		for (i = 0; i < 4; i++) fprintf(snap, "%02X", iMemory[PC + i] & 0xff);
	}
}

void dumpSnap2() {
	if (STALL) fprintf(snap, " to_be_stalled");
	else if (ID_EX.pc_src_in == 1) fprintf(snap, " to_be_flushed");
	fprintf(snap, "\n");
	
	fprintf(snap, "ID: ");
	if (ID_EX.pc_src_out) fprintf(snap, "NOP");
	else printIns(IF_ID.ins_reg_out >> 26, IF_ID.ins_reg_out << 11 >> 27, IF_ID.ins_reg_out << 16 >> 27, IF_ID.ins_reg_out << 26 >> 26, IF_ID.ins_reg_out << 21 >> 27);
	if (STALL) {
		fprintf(snap, " to_be_stalled");
	} else {
		if (FWD_RS_TO_ID) fprintf(snap, " fwd_EX-DM_rs_$%u", IF_ID.rs_out);
		if (FWD_RT_TO_ID) fprintf(snap, " fwd_EX-DM_rt_$%u", IF_ID.rt_out);
	}
	fprintf(snap, "\n");
	fprintf(snap, "EX: ");
	printIns(ID_EX.opcode_out, ID_EX.rt_out, ID_EX.rd_out, ID_EX.funct_out, ID_EX.shamt_out);
	
	if (FWD_RS_TO_EX) fprintf(snap, " fwd_EX-DM_rs_$%u", ID_EX.rs_out);
	if (FWD_RT_TO_EX) fprintf(snap, " fwd_EX-DM_rt_$%u", ID_EX.rt_out);
	fprintf(snap, "\n");
	
	fprintf(snap, "DM: ");
	printIns(EX_DM.opcode_out, EX_DM.rt_out, EX_DM.rd_out, EX_DM.funct_out, EX_DM.shamt_out);
	fprintf(snap, "\n");
	
	fprintf(snap, "WB: ");
	printIns(DM_WB.opcode_out, DM_WB.rt_out, DM_WB.rd_out, DM_WB.funct_out, DM_WB.shamt_out);
	fprintf(snap, "\n\n\n");
}

void printError() {
	if (write_to_zero) {
		write_to_zero = 0;
		fprintf(err, "In cycle %d: Write $0 Error\n", cycle);
	}
	
	if (mem_overflow) {
		fprintf(err, "In cycle %d: Address Overflow\n", cycle);
	}
	
	if (mem_misalign) {
		fprintf(err, "In cycle %d: Misalignment Error\n", cycle);
	}
	
	if (num_overflow) {
		num_overflow = 0;
		fprintf(err, "In cycle %d: Number Overflow\n", cycle);
	}
}

int detectHalt() {
	if ((IF_HALT && ID_HALT && EX_HALT && DM_HALT && WB_HALT) || (mem_overflow || mem_misalign)) return 1;
	else return 0;
}

void *thread5(void *input) {
	while (1) {
		pthread_barrier_wait(&barrier_CHECK);
		WB();
		pthread_barrier_wait(&barrier_ID_WB);
		pthread_barrier_wait(&barrier1_1);
		pthread_barrier_wait(&barrier_PRINT);
		if (detectHalt()) {
			pthread_barrier_wait(&barrier_RETURN);
			pthread_exit(NULL);
		}
		pthread_barrier_wait(&barrier1_2);
		/****/
		pthread_barrier_wait(&barrier_CHECK);
		IF();
		pthread_barrier_wait(&barrier2_1);
		pthread_barrier_wait(&barrier_PRINT);
		if (detectHalt()) {
			pthread_barrier_wait(&barrier_RETURN);
			pthread_exit(NULL);
		}
		IF_ID_READY();
		pthread_barrier_wait(&barrier2_2);
		/****/
		pthread_barrier_wait(&barrier_CHECK);
		pthread_barrier_wait(&barrier_ID_WB);
		ID();
		pthread_barrier_wait(&barrier3_1);
		pthread_barrier_wait(&barrier_PRINT);
		if (detectHalt()) {
			pthread_barrier_wait(&barrier_RETURN);
			pthread_exit(NULL);
		}
		ID_EX_READY();
		pthread_barrier_wait(&barrier3_2);
		/****/
		pthread_barrier_wait(&barrier_CHECK);
		EX();
		pthread_barrier_wait(&barrier4_1);
		pthread_barrier_wait(&barrier_PRINT);
		if (detectHalt()) {
			pthread_barrier_wait(&barrier_RETURN);
			pthread_exit(NULL);
		}
		EX_DM_READY();
		pthread_barrier_wait(&barrier4_2);
		/****/
		pthread_barrier_wait(&barrier_CHECK);
		DM();
		pthread_barrier_wait(&barrier5_1);
		pthread_barrier_wait(&barrier_PRINT);
		if (detectHalt()) {
			pthread_barrier_wait(&barrier_RETURN);
			pthread_exit(NULL);
		}
		DM_WB_READY();
		pthread_barrier_wait(&barrier5_2);
	}
}

void *thread4(void *input) {
	while (1) {
		pthread_barrier_wait(&barrier_CHECK);
		DM();
		pthread_barrier_wait(&barrier1_1);
		pthread_barrier_wait(&barrier_PRINT);
		if (detectHalt()) {
			pthread_barrier_wait(&barrier_RETURN);
			pthread_exit(NULL);
		}
		DM_WB_READY();
		pthread_barrier_wait(&barrier1_2);
		/****/
		pthread_barrier_wait(&barrier_CHECK);
		WB();
		pthread_barrier_wait(&barrier_ID_WB);
		pthread_barrier_wait(&barrier2_1);
		pthread_barrier_wait(&barrier_PRINT);
		if (detectHalt()) {
			pthread_barrier_wait(&barrier_RETURN);
			pthread_exit(NULL);
		}
		pthread_barrier_wait(&barrier2_2);
		/****/
		pthread_barrier_wait(&barrier_CHECK);
		IF();
		pthread_barrier_wait(&barrier3_1);
		pthread_barrier_wait(&barrier_PRINT);
		if (detectHalt()) {
			pthread_barrier_wait(&barrier_RETURN);
			pthread_exit(NULL);
		}
		IF_ID_READY();
		pthread_barrier_wait(&barrier3_2);
		/****/
		pthread_barrier_wait(&barrier_CHECK);
		pthread_barrier_wait(&barrier_ID_WB);
		ID();
		pthread_barrier_wait(&barrier4_1);
		pthread_barrier_wait(&barrier_PRINT);
		if (detectHalt()) {
			pthread_barrier_wait(&barrier_RETURN);
			pthread_exit(NULL);
		}
		ID_EX_READY();
		pthread_barrier_wait(&barrier4_2);
		/****/
		pthread_barrier_wait(&barrier_CHECK);
		EX();
		pthread_barrier_wait(&barrier5_1);
		pthread_barrier_wait(&barrier_PRINT);
		if (detectHalt()) {
			pthread_barrier_wait(&barrier_RETURN);
			pthread_exit(NULL);
		}
		EX_DM_READY();
		pthread_barrier_wait(&barrier5_2);
	}
}

void *thread3(void *input) {
	while (1) {
		pthread_barrier_wait(&barrier_CHECK);
		EX();
		pthread_barrier_wait(&barrier1_1);
		pthread_barrier_wait(&barrier_PRINT);
		if (detectHalt()) {
			pthread_barrier_wait(&barrier_RETURN);
			pthread_exit(NULL);
		}
		EX_DM_READY();
		pthread_barrier_wait(&barrier1_2);
		/****/
		pthread_barrier_wait(&barrier_CHECK);
		DM();
		pthread_barrier_wait(&barrier2_1);
		pthread_barrier_wait(&barrier_PRINT);
		if (detectHalt()) {
			pthread_barrier_wait(&barrier_RETURN);
			pthread_exit(NULL);
		}
		DM_WB_READY();
		pthread_barrier_wait(&barrier2_2);
		/****/
		pthread_barrier_wait(&barrier_CHECK);
		WB();
		pthread_barrier_wait(&barrier_ID_WB);
		pthread_barrier_wait(&barrier3_1);
		pthread_barrier_wait(&barrier_PRINT);
		if (detectHalt()) {
			pthread_barrier_wait(&barrier_RETURN);
			pthread_exit(NULL);
		}
		pthread_barrier_wait(&barrier3_2);
		/****/
		pthread_barrier_wait(&barrier_CHECK);
		IF();
		pthread_barrier_wait(&barrier4_1);
		pthread_barrier_wait(&barrier_PRINT);
		if (detectHalt()) {
			pthread_barrier_wait(&barrier_RETURN);
			pthread_exit(NULL);
		}
		IF_ID_READY();
		pthread_barrier_wait(&barrier4_2);
		/****/
		pthread_barrier_wait(&barrier_CHECK);
		pthread_barrier_wait(&barrier_ID_WB);
		ID();
		pthread_barrier_wait(&barrier5_1);
		pthread_barrier_wait(&barrier_PRINT);
		if (detectHalt()) {
			pthread_barrier_wait(&barrier_RETURN);
			pthread_exit(NULL);
		}
		ID_EX_READY();
		pthread_barrier_wait(&barrier5_2);
	}
}

void *thread2(void *input) {
	while (1) {
		pthread_barrier_wait(&barrier_CHECK);
		pthread_barrier_wait(&barrier_ID_WB);
		ID();
		pthread_barrier_wait(&barrier1_1);
		pthread_barrier_wait(&barrier_PRINT);
		if (detectHalt()) {
			pthread_barrier_wait(&barrier_RETURN);
			pthread_exit(NULL);
		}
		ID_EX_READY();
		pthread_barrier_wait(&barrier1_2);
		/****/
		pthread_barrier_wait(&barrier_CHECK);
		EX();
		pthread_barrier_wait(&barrier2_1);
		pthread_barrier_wait(&barrier_PRINT);
		if (detectHalt()) {
			pthread_barrier_wait(&barrier_RETURN);
			pthread_exit(NULL);
		}
		EX_DM_READY();
		pthread_barrier_wait(&barrier2_2);
		/****/
		pthread_barrier_wait(&barrier_CHECK);
		DM();
		pthread_barrier_wait(&barrier3_1);
		pthread_barrier_wait(&barrier_PRINT);
		if (detectHalt()) {
			pthread_barrier_wait(&barrier_RETURN);
			pthread_exit(NULL);
		}
		DM_WB_READY();
		pthread_barrier_wait(&barrier3_2);
		/****/
		pthread_barrier_wait(&barrier_CHECK);
		WB();
		pthread_barrier_wait(&barrier_ID_WB);
		pthread_barrier_wait(&barrier4_1);
		pthread_barrier_wait(&barrier_PRINT);
		if (detectHalt()) {
			pthread_barrier_wait(&barrier_RETURN);
			pthread_exit(NULL);
		}
		pthread_barrier_wait(&barrier4_2);
		/****/
		pthread_barrier_wait(&barrier_CHECK);
		IF();
		pthread_barrier_wait(&barrier5_1);
		pthread_barrier_wait(&barrier_PRINT);
		if (detectHalt()) {
			pthread_barrier_wait(&barrier_RETURN);
			pthread_exit(NULL);
		}
		IF_ID_READY();
		pthread_barrier_wait(&barrier5_2);
	}
}

void *thread1(void *input) {
	while (1) {
		checkFwdToId();
		checkFwdToEx();
		checkStall();
		dumpSnap();
		pthread_barrier_wait(&barrier_CHECK);
		IF();
		pthread_barrier_wait(&barrier1_1);
		dumpSnap2();
		printError();
		pthread_barrier_wait(&barrier_PRINT);
		if (detectHalt()) {
			pthread_barrier_wait(&barrier_RETURN);
			pthread_exit(NULL);
		}
		IF_ID_READY();
		pthread_barrier_wait(&barrier1_2);
		/****/
		checkFwdToId();
		checkFwdToEx();
		checkStall();
		dumpSnap();
		pthread_barrier_wait(&barrier_CHECK);
		pthread_barrier_wait(&barrier_ID_WB);
		ID();
		pthread_barrier_wait(&barrier2_1);
		dumpSnap2();
		printError();
		pthread_barrier_wait(&barrier_PRINT);
		if (detectHalt()) {
			pthread_barrier_wait(&barrier_RETURN);
			pthread_exit(NULL);
		}
		ID_EX_READY();
		pthread_barrier_wait(&barrier2_2);
		/****/
		checkFwdToId();
		checkFwdToEx();
		checkStall();
		dumpSnap();
		pthread_barrier_wait(&barrier_CHECK);
		EX();
		pthread_barrier_wait(&barrier3_1);
		dumpSnap2();
		printError();
		pthread_barrier_wait(&barrier_PRINT);
		if (detectHalt()) {
			pthread_barrier_wait(&barrier_RETURN);
			pthread_exit(NULL);
		}
		EX_DM_READY();
		pthread_barrier_wait(&barrier3_2);
		/****/
		checkFwdToId();
		checkFwdToEx();
		checkStall();
		dumpSnap();
		pthread_barrier_wait(&barrier_CHECK);
		DM();
		pthread_barrier_wait(&barrier4_1);
		dumpSnap2();
		printError();
		pthread_barrier_wait(&barrier_PRINT);
		if (detectHalt()) {
			pthread_barrier_wait(&barrier_RETURN);
			pthread_exit(NULL);
		}
		DM_WB_READY();
		pthread_barrier_wait(&barrier4_2);
		/****/
		checkFwdToId();
		checkFwdToEx();
		checkStall();
		dumpSnap();
		pthread_barrier_wait(&barrier_CHECK);
		WB();
		pthread_barrier_wait(&barrier_ID_WB);
		pthread_barrier_wait(&barrier5_1);
		dumpSnap2();
		printError();
		pthread_barrier_wait(&barrier_PRINT);
		if (detectHalt()) {
			pthread_barrier_wait(&barrier_RETURN);
			pthread_exit(NULL);
		}
		pthread_barrier_wait(&barrier5_2);
	}
}

int main() {
	openNLoadFiles();
	dealWithDImg();
	dealWithIImg();
	// TODO: pipeline
	pthread_barrier_init(&barrier1_1, NULL, 5);
	pthread_barrier_init(&barrier1_2, NULL, 5);
	pthread_barrier_init(&barrier2_1, NULL, 5);
	pthread_barrier_init(&barrier2_2, NULL, 5);
	pthread_barrier_init(&barrier3_1, NULL, 5);
	pthread_barrier_init(&barrier3_2, NULL, 5);
	pthread_barrier_init(&barrier4_1, NULL, 5);
	pthread_barrier_init(&barrier4_2, NULL, 5);
	pthread_barrier_init(&barrier5_1, NULL, 5);
	pthread_barrier_init(&barrier5_2, NULL, 5);
	pthread_barrier_init(&barrier_CHECK, NULL, 5);
	pthread_barrier_init(&barrier_PRINT, NULL, 5);
	pthread_barrier_init(&barrier_ID_WB, NULL, 2);
	pthread_barrier_init(&barrier_RETURN, NULL, 6);
	
	pthread_t t1, t2, t3, t4, t5;
	pthread_create(&t1, NULL, thread1, NULL);
	pthread_create(&t2, NULL, thread2, NULL);
	pthread_create(&t3, NULL, thread3, NULL);
	pthread_create(&t4, NULL, thread4, NULL);
	pthread_create(&t5, NULL, thread5, NULL);
	
	pthread_barrier_wait(&barrier_RETURN);
	
	return 0;
}

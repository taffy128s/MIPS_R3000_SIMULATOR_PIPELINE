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
static pthread_barrier_t barrier_PRINT;

void printIns(unsigned opcode, unsigned rt, unsigned rd, unsigned funct, unsigned shamt) {
	switch (opcode) {
		case R:
			switch (funct) {
				case ADD:
					fprintf(snap, "ADD\n");
					break;
				case ADDU:
					fprintf(snap, "ADDU\n");
					break;
				case SUB:
					fprintf(snap, "SUB\n");
					break;
				case AND:
					fprintf(snap, "AND\n");
					break;
				case OR:
					fprintf(snap, "OR\n");
					break;
				case XOR:
					fprintf(snap, "XOR\n");
					break;
				case NOR:
					fprintf(snap, "NOR\n");
					break;
				case NAND:
					fprintf(snap, "NAND\n");
					break;
				case SLT:
					fprintf(snap, "SLT\n");
					break;
				case SLL:
					if (rt == 0 && rd == 0 && shamt == 0) fprintf(snap, "NOP\n");
					else fprintf(snap, "SLL\n");
					break;
				case SRL:
					fprintf(snap, "SRL\n");
					break;
				case SRA:
					fprintf(snap, "SRA\n");
					break;
				case JR:
					fprintf(snap, "JR\n");
					break;
				default:
					fprintf(snap, "ERROR!!\n");
					break;
			}
			break;
		case ADDI:
			fprintf(snap, "ADDI\n");
			break;
		case ADDIU:
			fprintf(snap, "ADDIU\n");
			break;
		case LW:
			fprintf(snap, "LW\n");
			break;
		case LH:
			fprintf(snap, "LH\n");
			break;
		case LHU:
			fprintf(snap, "LHU\n");
			break;
		case LB:
			fprintf(snap, "LB\n");
			break;
		case LBU:
			fprintf(snap, "LBU\n");
			break;
		case SW:
			fprintf(snap, "SW\n");
			break;
		case SH:
			fprintf(snap, "SH\n");
			break;
		case SB:
			fprintf(snap, "SB\n");
			break;
		case LUI:
			fprintf(snap, "LUI\n");
			break;
		case ANDI:
			fprintf(snap, "ANDI\n");
			break;
		case ORI:
			fprintf(snap, "ORI\n");
			break;
		case NORI:
			fprintf(snap, "NORI\n");
			break;
		case SLTI:
			fprintf(snap, "SLTI\n");
			break;
		case BEQ:
			fprintf(snap, "BEQ\n");
			break;
		case BNE:
			fprintf(snap, "BNE\n");
			break;
		case BGTZ:
			fprintf(snap, "BGTZ\n");
			break;
		case J:
			fprintf(snap, "J\n");
			break;
		case JAL:
			fprintf(snap, "JAL\n");
			break;
		case HALT:
			fprintf(snap, "HALT\n");
			break;
		default:
			fprintf(snap, "ERROR!!\n");
			break;
	}
}

void dumpSnap() {
	fprintf(snap, "cycle %u\n", cycle++);
	unsigned i;
	for (i = 0; i < 32; i++) fprintf(snap, "$%02u: 0x%08X\n", i, reg[i]);
	fprintf(snap, "PC: 0x%08X\nIF: 0x", PC);
	for (i = 0; i < 4; i++) fprintf(snap, "%02X", iMemory[PC + i] & 0xff);
	fprintf(snap, "\n");
	fprintf(snap, "ID: ");
	printIns(IF_ID.ins_reg_in >> 26, IF_ID.ins_reg_in << 11 >> 27, IF_ID.ins_reg_in << 16 >> 27, IF_ID.ins_reg_in << 26 >> 26, IF_ID.ins_reg_in << 21 >> 27);
	fprintf(snap, "EX: ");
	printIns(ID_EX.opcode_out, ID_EX.rt_out, ID_EX.rd_out, ID_EX.funct_out, ID_EX.shamt_out);
	fprintf(snap, "DM: ");
	printIns(EX_DM.opcode_out, EX_DM.rt_out, EX_DM.rd_out, EX_DM.funct_out, EX_DM.shamt_out);
	fprintf(snap, "WB: ");
	printIns(DM_WB.opcode_out, DM_WB.rt_out, DM_WB.rd_out, DM_WB.funct_out, DM_WB.shamt_out);
	fprintf(snap, "\n\n");
}

int detectHalt() {
	if (IF_HALT && ID_HALT && EX_HALT && DM_HALT && WB_HALT) return 1;
	else return 0;
}

void *thread5(void *input) {
	while (1) {
		pthread_barrier_wait(&barrier_PRINT);
		WB();
		pthread_barrier_wait(&barrier_ID_WB);
		pthread_barrier_wait(&barrier1_1);
		if (detectHalt()) {
			pthread_barrier_wait(&barrier_RETURN);
			pthread_exit(NULL);
		}
		pthread_barrier_wait(&barrier1_2);
		/****/
		pthread_barrier_wait(&barrier_PRINT);
		IF();
		pthread_barrier_wait(&barrier2_1);
		if (detectHalt()) {
			pthread_barrier_wait(&barrier_RETURN);
			pthread_exit(NULL);
		}
		IF_ID_READY();
		pthread_barrier_wait(&barrier2_2);
		/****/
		pthread_barrier_wait(&barrier_PRINT);
		pthread_barrier_wait(&barrier_ID_WB);
		ID();
		pthread_barrier_wait(&barrier3_1);
		if (detectHalt()) {
			pthread_barrier_wait(&barrier_RETURN);
			pthread_exit(NULL);
		}
		ID_EX_READY();
		pthread_barrier_wait(&barrier3_2);
		/****/
		pthread_barrier_wait(&barrier_PRINT);
		EX();
		pthread_barrier_wait(&barrier4_1);
		if (detectHalt()) {
			pthread_barrier_wait(&barrier_RETURN);
			pthread_exit(NULL);
		}
		EX_DM_READY();
		pthread_barrier_wait(&barrier4_2);
		/****/
		pthread_barrier_wait(&barrier_PRINT);
		DM();
		pthread_barrier_wait(&barrier5_1);
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
		pthread_barrier_wait(&barrier_PRINT);
		DM();
		pthread_barrier_wait(&barrier1_1);
		if (detectHalt()) {
			pthread_barrier_wait(&barrier_RETURN);
			pthread_exit(NULL);
		}
		DM_WB_READY();
		pthread_barrier_wait(&barrier1_2);
		/****/
		pthread_barrier_wait(&barrier_PRINT);
		WB();
		pthread_barrier_wait(&barrier_ID_WB);
		pthread_barrier_wait(&barrier2_1);
		if (detectHalt()) {
			pthread_barrier_wait(&barrier_RETURN);
			pthread_exit(NULL);
		}
		pthread_barrier_wait(&barrier2_2);
		/****/
		pthread_barrier_wait(&barrier_PRINT);
		IF();
		pthread_barrier_wait(&barrier3_1);
		if (detectHalt()) {
			pthread_barrier_wait(&barrier_RETURN);
			pthread_exit(NULL);
		}
		IF_ID_READY();
		pthread_barrier_wait(&barrier3_2);
		/****/
		pthread_barrier_wait(&barrier_PRINT);
		pthread_barrier_wait(&barrier_ID_WB);
		ID();
		pthread_barrier_wait(&barrier4_1);
		if (detectHalt()) {
			pthread_barrier_wait(&barrier_RETURN);
			pthread_exit(NULL);
		}
		ID_EX_READY();
		pthread_barrier_wait(&barrier4_2);
		/****/
		pthread_barrier_wait(&barrier_PRINT);
		EX();
		pthread_barrier_wait(&barrier5_1);
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
		pthread_barrier_wait(&barrier_PRINT);
		EX();
		pthread_barrier_wait(&barrier1_1);
		if (detectHalt()) {
			pthread_barrier_wait(&barrier_RETURN);
			pthread_exit(NULL);
		}
		EX_DM_READY();
		pthread_barrier_wait(&barrier1_2);
		/****/
		pthread_barrier_wait(&barrier_PRINT);
		DM();
		pthread_barrier_wait(&barrier2_1);
		if (detectHalt()) {
			pthread_barrier_wait(&barrier_RETURN);
			pthread_exit(NULL);
		}
		DM_WB_READY();
		pthread_barrier_wait(&barrier2_2);
		/****/
		pthread_barrier_wait(&barrier_PRINT);
		WB();
		pthread_barrier_wait(&barrier_ID_WB);
		pthread_barrier_wait(&barrier3_1);
		if (detectHalt()) {
			pthread_barrier_wait(&barrier_RETURN);
			pthread_exit(NULL);
		}
		pthread_barrier_wait(&barrier3_2);
		/****/
		pthread_barrier_wait(&barrier_PRINT);
		IF();
		pthread_barrier_wait(&barrier4_1);
		if (detectHalt()) {
			pthread_barrier_wait(&barrier_RETURN);
			pthread_exit(NULL);
		}
		IF_ID_READY();
		pthread_barrier_wait(&barrier4_2);
		/****/
		pthread_barrier_wait(&barrier_PRINT);
		pthread_barrier_wait(&barrier_ID_WB);
		ID();
		pthread_barrier_wait(&barrier5_1);
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
		pthread_barrier_wait(&barrier_PRINT);
		pthread_barrier_wait(&barrier_ID_WB);
		ID();
		pthread_barrier_wait(&barrier1_1);
		if (detectHalt()) {
			pthread_barrier_wait(&barrier_RETURN);
			pthread_exit(NULL);
		}
		ID_EX_READY();
		pthread_barrier_wait(&barrier1_2);
		/****/
		pthread_barrier_wait(&barrier_PRINT);
		EX();
		pthread_barrier_wait(&barrier2_1);
		if (detectHalt()) {
			pthread_barrier_wait(&barrier_RETURN);
			pthread_exit(NULL);
		}
		EX_DM_READY();
		pthread_barrier_wait(&barrier2_2);
		/****/
		pthread_barrier_wait(&barrier_PRINT);
		DM();
		pthread_barrier_wait(&barrier3_1);
		if (detectHalt()) {
			pthread_barrier_wait(&barrier_RETURN);
			pthread_exit(NULL);
		}
		DM_WB_READY();
		pthread_barrier_wait(&barrier3_2);
		/****/
		pthread_barrier_wait(&barrier_PRINT);
		WB();
		pthread_barrier_wait(&barrier_ID_WB);
		pthread_barrier_wait(&barrier4_1);
		if (detectHalt()) {
			pthread_barrier_wait(&barrier_RETURN);
			pthread_exit(NULL);
		}
		pthread_barrier_wait(&barrier4_2);
		/****/
		pthread_barrier_wait(&barrier_PRINT);
		IF();
		pthread_barrier_wait(&barrier5_1);
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
		dumpSnap();
		pthread_barrier_wait(&barrier_PRINT);
		IF();
		pthread_barrier_wait(&barrier1_1);
		if (detectHalt()) {
			pthread_barrier_wait(&barrier_RETURN);
			pthread_exit(NULL);
		}
		IF_ID_READY();
		pthread_barrier_wait(&barrier1_2);
		/****/
		dumpSnap();
		pthread_barrier_wait(&barrier_PRINT);
		pthread_barrier_wait(&barrier_ID_WB);
		ID();
		pthread_barrier_wait(&barrier2_1);
		if (detectHalt()) {
			pthread_barrier_wait(&barrier_RETURN);
			pthread_exit(NULL);
		}
		ID_EX_READY();
		pthread_barrier_wait(&barrier2_2);
		/****/
		dumpSnap();
		pthread_barrier_wait(&barrier_PRINT);
		EX();
		pthread_barrier_wait(&barrier3_1);
		if (detectHalt()) {
			pthread_barrier_wait(&barrier_RETURN);
			pthread_exit(NULL);
		}
		EX_DM_READY();
		pthread_barrier_wait(&barrier3_2);
		/****/
		dumpSnap();
		pthread_barrier_wait(&barrier_PRINT);
		DM();
		pthread_barrier_wait(&barrier4_1);
		if (detectHalt()) {
			pthread_barrier_wait(&barrier_RETURN);
			pthread_exit(NULL);
		}
		DM_WB_READY();
		pthread_barrier_wait(&barrier4_2);
		/****/
		dumpSnap();
		pthread_barrier_wait(&barrier_PRINT);
		WB();
		pthread_barrier_wait(&barrier_ID_WB);
		pthread_barrier_wait(&barrier5_1);
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

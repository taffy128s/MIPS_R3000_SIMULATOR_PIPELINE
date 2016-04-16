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

int detectHalt() {
	if (IF_HALT && ID_HALT && EX_HALT && DM_HALT && WB_HALT) return 1;
	else return 0;
}

void *thread5(void *input) {
	while (1) {
		WB();
		pthread_barrier_wait(&barrier_ID_WB);
		pthread_barrier_wait(&barrier1_1);
		if (detectHalt()) {
			pthread_barrier_wait(&barrier_RETURN);
			pthread_exit(NULL);
		}
		pthread_barrier_wait(&barrier1_2);
		IF();
		pthread_barrier_wait(&barrier2_1);
		if (detectHalt()) {
			pthread_barrier_wait(&barrier_RETURN);
			pthread_exit(NULL);
		}
		IF_ID_READY();
		pthread_barrier_wait(&barrier2_2);
		pthread_barrier_wait(&barrier_ID_WB);
		ID();
		pthread_barrier_wait(&barrier3_1);
		if (detectHalt()) {
			pthread_barrier_wait(&barrier_RETURN);
			pthread_exit(NULL);
		}
		ID_EX_READY();
		pthread_barrier_wait(&barrier3_2);
		EX();
		pthread_barrier_wait(&barrier4_1);
		if (detectHalt()) {
			pthread_barrier_wait(&barrier_RETURN);
			pthread_exit(NULL);
		}
		EX_DM_READY();
		pthread_barrier_wait(&barrier4_2);
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
		DM();
		pthread_barrier_wait(&barrier1_1);
		if (detectHalt()) {
			pthread_barrier_wait(&barrier_RETURN);
			pthread_exit(NULL);
		}
		DM_WB_READY();
		pthread_barrier_wait(&barrier1_2);
		WB();
		pthread_barrier_wait(&barrier_ID_WB);
		pthread_barrier_wait(&barrier2_1);
		if (detectHalt()) {
			pthread_barrier_wait(&barrier_RETURN);
			pthread_exit(NULL);
		}
		pthread_barrier_wait(&barrier2_2);
		IF();
		pthread_barrier_wait(&barrier3_1);
		if (detectHalt()) {
			pthread_barrier_wait(&barrier_RETURN);
			pthread_exit(NULL);
		}
		IF_ID_READY();
		pthread_barrier_wait(&barrier3_2);
		pthread_barrier_wait(&barrier_ID_WB);
		ID();
		pthread_barrier_wait(&barrier4_1);
		if (detectHalt()) {
			pthread_barrier_wait(&barrier_RETURN);
			pthread_exit(NULL);
		}
		ID_EX_READY();
		pthread_barrier_wait(&barrier4_2);
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
		EX();
		pthread_barrier_wait(&barrier1_1);
		if (detectHalt()) {
			pthread_barrier_wait(&barrier_RETURN);
			pthread_exit(NULL);
		}
		EX_DM_READY();
		pthread_barrier_wait(&barrier1_2);
		DM();
		pthread_barrier_wait(&barrier2_1);
		if (detectHalt()) {
			pthread_barrier_wait(&barrier_RETURN);
			pthread_exit(NULL);
		}
		DM_WB_READY();
		pthread_barrier_wait(&barrier2_2);
		WB();
		pthread_barrier_wait(&barrier_ID_WB);
		pthread_barrier_wait(&barrier3_1);
		if (detectHalt()) {
			pthread_barrier_wait(&barrier_RETURN);
			pthread_exit(NULL);
		}
		pthread_barrier_wait(&barrier3_2);
		IF();
		pthread_barrier_wait(&barrier4_1);
		if (detectHalt()) {
			pthread_barrier_wait(&barrier_RETURN);
			pthread_exit(NULL);
		}
		IF_ID_READY();
		//printf("thread3: %x\n", IF_ID.ins_reg_out);
		pthread_barrier_wait(&barrier4_2);
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
		pthread_barrier_wait(&barrier_ID_WB);
		ID();
		pthread_barrier_wait(&barrier1_1);
		if (detectHalt()) {
			pthread_barrier_wait(&barrier_RETURN);
			pthread_exit(NULL);
		}
		ID_EX_READY();
		pthread_barrier_wait(&barrier1_2);
		EX();
		pthread_barrier_wait(&barrier2_1);
		if (detectHalt()) {
			pthread_barrier_wait(&barrier_RETURN);
			pthread_exit(NULL);
		}
		EX_DM_READY();
		pthread_barrier_wait(&barrier2_2);
		DM();
		pthread_barrier_wait(&barrier3_1);
		if (detectHalt()) {
			pthread_barrier_wait(&barrier_RETURN);
			pthread_exit(NULL);
		}
		DM_WB_READY();
		pthread_barrier_wait(&barrier3_2);
		WB();
		pthread_barrier_wait(&barrier_ID_WB);
		pthread_barrier_wait(&barrier4_1);
		if (detectHalt()) {
			pthread_barrier_wait(&barrier_RETURN);
			pthread_exit(NULL);
		}
		pthread_barrier_wait(&barrier4_2);
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
		IF();
		pthread_barrier_wait(&barrier1_1);
		if (detectHalt()) {
			pthread_barrier_wait(&barrier_RETURN);
			pthread_exit(NULL);
		}
		IF_ID_READY();
		pthread_barrier_wait(&barrier1_2);
		pthread_barrier_wait(&barrier_ID_WB);
		ID();
		pthread_barrier_wait(&barrier2_1);
		if (detectHalt()) {
			pthread_barrier_wait(&barrier_RETURN);
			pthread_exit(NULL);
		}
		ID_EX_READY();
		pthread_barrier_wait(&barrier2_2);
		EX();
		pthread_barrier_wait(&barrier3_1);
		if (detectHalt()) {
			pthread_barrier_wait(&barrier_RETURN);
			pthread_exit(NULL);
		}
		EX_DM_READY();
		pthread_barrier_wait(&barrier3_2);
		DM();
		pthread_barrier_wait(&barrier4_1);
		if (detectHalt()) {
			pthread_barrier_wait(&barrier_RETURN);
			pthread_exit(NULL);
		}
		DM_WB_READY();
		pthread_barrier_wait(&barrier4_2);
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

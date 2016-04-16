#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
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

void *thread5(void *input) {
	WB();
	pthread_barrier_wait(&barrier_ID_WB);
	pthread_barrier_wait(&barrier1_1);
	pthread_barrier_wait(&barrier1_2);
	IF();
	pthread_barrier_wait(&barrier2_1);
	IF_ID_READY();
	pthread_barrier_wait(&barrier2_2);
	pthread_barrier_wait(&barrier_ID_WB);
	ID();
	pthread_barrier_wait(&barrier3_1);
	ID_EX_READY();
	pthread_barrier_wait(&barrier3_2);
	EX();
	pthread_barrier_wait(&barrier4_1);
	EX_DM_READY();
	pthread_barrier_wait(&barrier4_2);
	DM();
	pthread_barrier_wait(&barrier5_1);
	DM_WB_READY();
	pthread_barrier_wait(&barrier5_2);
	
	pthread_exit(NULL);
}

void *thread4(void *input) {
	DM();
	pthread_barrier_wait(&barrier1_1);
	DM_WB_READY();
	pthread_barrier_wait(&barrier1_2);
	WB();
	pthread_barrier_wait(&barrier_ID_WB);
	pthread_barrier_wait(&barrier2_1);
	pthread_barrier_wait(&barrier2_2);
	IF();
	pthread_barrier_wait(&barrier3_1);
	IF_ID_READY();
	pthread_barrier_wait(&barrier3_2);
	pthread_barrier_wait(&barrier_ID_WB);
	ID();
	pthread_barrier_wait(&barrier4_1);
	ID_EX_READY();
	pthread_barrier_wait(&barrier4_2);
	EX();
	pthread_barrier_wait(&barrier5_1);
	EX_DM_READY();
	pthread_barrier_wait(&barrier5_2);
	
	pthread_exit(NULL);
}

void *thread3(void *input) {
	EX();
	pthread_barrier_wait(&barrier1_1);
	EX_DM_READY();
	pthread_barrier_wait(&barrier1_2);
	DM();
	pthread_barrier_wait(&barrier2_1);
	DM_WB_READY();
	pthread_barrier_wait(&barrier2_2);
	WB();
	pthread_barrier_wait(&barrier_ID_WB);
	pthread_barrier_wait(&barrier3_1);
	pthread_barrier_wait(&barrier3_2);
	IF();
	pthread_barrier_wait(&barrier4_1);
	IF_ID_READY();
	printf("thread3: %x\n", IF_ID.ins_reg_out);
	pthread_barrier_wait(&barrier4_2);
	pthread_barrier_wait(&barrier_ID_WB);
	ID();
	pthread_barrier_wait(&barrier5_1);
	ID_EX_READY();
	pthread_barrier_wait(&barrier5_2);
	
	pthread_exit(NULL);
}

void *thread2(void *input) {
	pthread_barrier_wait(&barrier_ID_WB);
	ID();
	pthread_barrier_wait(&barrier1_1);
	ID_EX_READY();
	pthread_barrier_wait(&barrier1_2);
	EX();
	pthread_barrier_wait(&barrier2_1);
	EX_DM_READY();
	pthread_barrier_wait(&barrier2_2);
	DM();
	pthread_barrier_wait(&barrier3_1);
	DM_WB_READY();
	pthread_barrier_wait(&barrier3_2);
	WB();
	pthread_barrier_wait(&barrier_ID_WB);
	pthread_barrier_wait(&barrier4_1);
	pthread_barrier_wait(&barrier4_2);
	IF();
	pthread_barrier_wait(&barrier5_1);
	IF_ID_READY();
	pthread_barrier_wait(&barrier5_2);
	
	pthread_exit(NULL);
}

void *thread1(void *input) {
	IF();
	pthread_barrier_wait(&barrier1_1);
	IF_ID_READY();
	pthread_barrier_wait(&barrier1_2);
	pthread_barrier_wait(&barrier_ID_WB);
	ID();
	pthread_barrier_wait(&barrier2_1);
	ID_EX_READY();
	pthread_barrier_wait(&barrier2_2);
	EX();
	pthread_barrier_wait(&barrier3_1);
	EX_DM_READY();
	pthread_barrier_wait(&barrier3_2);
	DM();
	pthread_barrier_wait(&barrier4_1);
	DM_WB_READY();
	pthread_barrier_wait(&barrier4_2);
	WB();
	pthread_barrier_wait(&barrier_ID_WB);
	pthread_barrier_wait(&barrier5_1);
	pthread_barrier_wait(&barrier5_2);
	
	pthread_exit(NULL);
}

int main() {
	openNLoadFiles();
	dealWithDImg();
	dealWithIImg();
	// TODO: pipeline
	pthread_barrier_init(&barrier1_1, NULL, 6);
	pthread_barrier_init(&barrier1_2, NULL, 6);
	pthread_barrier_init(&barrier2_1, NULL, 6);
	pthread_barrier_init(&barrier2_2, NULL, 6);
	pthread_barrier_init(&barrier3_1, NULL, 6);
	pthread_barrier_init(&barrier3_2, NULL, 6);
	pthread_barrier_init(&barrier4_1, NULL, 6);
	pthread_barrier_init(&barrier4_2, NULL, 6);
	pthread_barrier_init(&barrier5_1, NULL, 6);
	pthread_barrier_init(&barrier5_2, NULL, 6);
	pthread_barrier_init(&barrier_ID_WB, NULL, 2);
	
	pthread_t t1, t2, t3, t4, t5;
	pthread_create(&t1, NULL, thread1, NULL);
	pthread_create(&t2, NULL, thread2, NULL);
	pthread_create(&t3, NULL, thread3, NULL);
	pthread_create(&t4, NULL, thread4, NULL);
	pthread_create(&t5, NULL, thread5, NULL);
	
	pthread_barrier_wait(&barrier1_1);
	pthread_barrier_wait(&barrier1_2);
	pthread_barrier_wait(&barrier2_1);
	pthread_barrier_wait(&barrier2_2);
	pthread_barrier_wait(&barrier3_1);
	pthread_barrier_wait(&barrier3_2);
	pthread_barrier_wait(&barrier4_1);
	pthread_barrier_wait(&barrier4_2);
	pthread_barrier_wait(&barrier5_1);
	pthread_barrier_wait(&barrier5_2);
	
	return 0;
}

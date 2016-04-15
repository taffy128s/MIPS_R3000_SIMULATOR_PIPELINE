#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "initial.h"
#include "IF.h"
#include "ID.h"
#include "EX.h"

static pthread_barrier_t barrier1_1, barrier1_2;
static pthread_barrier_t barrier2_1, barrier2_2;
static pthread_barrier_t barrier3_1, barrier3_2;

void *thread2(void *input) {
	pthread_exit(NULL);
}

void *thread1(void *input) {
	IF();
	pthread_barrier_wait(&barrier1_1);
	IF_ID_READY();
	pthread_barrier_wait(&barrier1_2);
	ID();
	pthread_barrier_wait(&barrier2_1);
	ID_EX_READY();
	pthread_barrier_wait(&barrier2_2);
	EX();
	pthread_barrier_wait(&barrier3_1);
	EX_DM_READY();
	pthread_barrier_wait(&barrier3_2);
	
	pthread_exit(NULL);
}

int main() {
	openNLoadFiles();
	dealWithDImg();
	dealWithIImg();
	// TODO: pipeline
	pthread_barrier_init(&barrier1_1, NULL, 2);
	pthread_barrier_init(&barrier1_2, NULL, 2);
	pthread_barrier_init(&barrier2_1, NULL, 2);
	pthread_barrier_init(&barrier2_2, NULL, 2);
	pthread_barrier_init(&barrier3_1, NULL, 2);
	pthread_barrier_init(&barrier3_2, NULL, 2);
	
	pthread_t t1;
	pthread_create(&t1, NULL, thread1, NULL);
	
	pthread_barrier_wait(&barrier1_1);
	pthread_barrier_wait(&barrier1_2);
	pthread_barrier_wait(&barrier2_1);
	pthread_barrier_wait(&barrier2_2);
	pthread_barrier_wait(&barrier3_1);
	pthread_barrier_wait(&barrier3_2);
	
	printf("%d\n%x\n", IF_ID.pc_plus_four_out, IF_ID.ins_reg_out);
	printf("%u\n%u\n", ID_EX.rt_out, ID_EX.rd_out);
	printf("%u\n", ID_EX.extended_imme_out);
	printf("%u\n", ID_EX.reg_dst_out);
	return 0;
}

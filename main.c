#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "initial.h"
#include "IF.h"



int main() {
	openNLoadFiles();
    dealWithDImg();
    dealWithIImg();
	// TODO: pipeline
	ifout test = IF();
	printf("%d\n%x\n", test.pc_plus_four, test.ins_reg);
	return 0;
}

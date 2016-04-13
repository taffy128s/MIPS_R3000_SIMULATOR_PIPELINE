#include "global.h"
#include "IF.h"

ifout IF() {
	int i;
	ifout retVal;
	retVal.pc_plus_four = PC + 4;
	for (i = 0; i < 4; i++)
		retVal.ins_reg = (retVal.ins_reg << 8) | (unsigned char) iMemory[PC + i];
	return retVal;
}

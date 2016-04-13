#include "global.h"
#include "IF.h"
#include "ID.h"

void ID() {
	ID_EX.$rs_in = reg[IF_ID.ins_reg_out << 6 >> 27];
	ID_EX.$rt_in = reg[IF_ID.ins_reg_out << 11 >> 27];
	
	short temp = IF_ID.ins_reg_out << 16 >> 16;
	ID_EX.extended_imme_in = temp;
	ID_EX.rt_in = IF_ID.ins_reg_out << 11 >> 27;
	ID_EX.rd_in = IF_ID.ins_reg_out << 16 >> 27;
}

void CONTROL() {
	
}

void ID_EX_READY() {
	ID_EX.$rs_out = ID_EX.$rs_in;
	ID_EX.$rt_out = ID_EX.$rt_in;
	
	ID_EX.extended_imme_out = ID_EX.extended_imme_in;
	ID_EX.rt_out = ID_EX.rt_in;
	ID_EX.rd_out = ID_EX.rd_in;
}

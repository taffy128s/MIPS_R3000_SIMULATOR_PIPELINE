typedef struct {
	unsigned ins_reg_in, ins_reg_out;
	unsigned pc_plus_four_in, pc_plus_four_out;
	
	unsigned opcode_in, opcode_out;
	unsigned funct_in, funct_out;
	unsigned rs_in, rs_out;
	unsigned rt_in, rt_out;
	unsigned $rs, $rt;
} IF_STAGE; 

extern IF_STAGE IF_ID;
IF_STAGE IF_ID;

void IF();
void IF_ID_READY();

typedef struct {
	unsigned opcode_in, opcode_out;
	unsigned rd_in, rd_out;
	unsigned rs_in, rs_out;
	unsigned rt_in, rt_out;
	unsigned funct_in, funct_out;
	unsigned shamt_in, shamt_out;
	
	unsigned reg_write_in, reg_write_out;
	unsigned mem_to_reg_in, mem_to_reg_out;
	
	unsigned read_data_in, read_data_out;
	unsigned alu_result_in, alu_result_out;
	unsigned reg_to_write_in, reg_to_write_out;
} DM_STAGE;

extern DM_STAGE DM_WB;
DM_STAGE DM_WB;

void DM();
void DM_WB_READY();

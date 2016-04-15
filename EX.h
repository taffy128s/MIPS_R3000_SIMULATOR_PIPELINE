typedef struct {
	unsigned opcode_in, opcode_out;
	
	unsigned mem_read_in, mem_read_out;
	unsigned mem_write_in, mem_write_out;
	unsigned mem_op_in, mem_op_out;
	
	unsigned reg_write_in, reg_write_out;
	unsigned mem_to_reg_in, mem_to_reg_out;

	unsigned alu_result_in, alu_result_out;
	unsigned $rt_in, $rt_out;
	unsigned reg_to_write_in, reg_to_write_out;
} EX_STAGE;

extern EX_STAGE EX_DM;
EX_STAGE EX_DM;

void EX();
void EX_DM_READY();
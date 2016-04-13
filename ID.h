typedef struct {
	unsigned reg_write_in, reg_write_out;
	unsigned mem_to_reg_in, mem_to_reg_out;
	
	unsigned mem_read_in, mem_read_out;
	unsigned mem_write_in, mem_write_out;
	
	unsigned reg_dst_in, reg_dst_out;
	unsigned alu_op1_in, alu_op1_out;
	unsigned alu_op0_in, alu_op0_out;
	unsigned alu_src_in, alu_src_out;
	unsigned branch_in, branch_out;
	
	unsigned $rs_in, $rs_out;
	unsigned $rt_in, $rt_out;
	
	unsigned extended_imme_in, extended_imme_out;
	unsigned rt_in, rt_out;
	unsigned rd_in, rd_out;
} ID_STAGE;

extern ID_STAGE ID_EX;
ID_STAGE ID_EX;

void ID();
void CONTROL();
void ID_EX_READY();

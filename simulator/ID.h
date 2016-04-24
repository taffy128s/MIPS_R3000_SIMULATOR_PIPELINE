typedef struct {
	unsigned pc_plus_four_in, pc_plus_four_out;
	unsigned opcode_in, opcode_out;
	unsigned funct_in, funct_out;
	unsigned shamt_in, shamt_out;
	unsigned rt_in, rt_out;
	unsigned rd_in, rd_out;
	unsigned rs_in, rs_out;
	
	unsigned reg_dst_in, reg_dst_out;
	unsigned alu_src_in, alu_src_out;
	
	unsigned mem_read_in, mem_read_out;
	unsigned mem_write_in, mem_write_out;
	unsigned mem_op_in, mem_op_out;
	
	unsigned reg_write_in, reg_write_out;
	unsigned mem_to_reg_in, mem_to_reg_out;
	
	unsigned $rs_in, $rs_out;
	unsigned $rt_in, $rt_out;
	unsigned extended_imme_in, extended_imme_out;
	
	unsigned pc_src_in, pc_src_out;
	unsigned pc_in, pc_out;
	unsigned reg_to_write_in, reg_to_write_out;
} ID_STAGE;

extern ID_STAGE ID_EX;
ID_STAGE ID_EX;

void ID();
void CONTROL();
void ID_EX_READY();

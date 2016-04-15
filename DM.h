typedef struct {
	unsigned reg_write_in, reg_write_out;
	unsigned mem_to_reg_in, mem_to_reg_out;
	
	unsigned read_data_in, read_data_out;
	unsigned alu_result_in, alu_result_out;
} DM_STAGE;

extern DM_STAGE DM_WB;
DM_STAGE DM_WB;

void DM();
void DM_WB_READY();

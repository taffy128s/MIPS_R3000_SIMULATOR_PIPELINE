typedef struct {
	unsigned alu_result_in, alu_result_out;
	unsigned $rt_in, $rt_out;
	unsigned reg_to_write_in, reg_to_write_out;
} EX_STAGE;

extern EX_STAGE EX_DM;
EX_STAGE EX_DM;

void EX();
void EX_DM_READY();

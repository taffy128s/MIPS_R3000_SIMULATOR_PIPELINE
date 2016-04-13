typedef struct {
	unsigned pc_plus_four_in, pc_plus_four_out;
	unsigned ins_reg_in, ins_reg_out;
} IF_STAGE;

extern IF_STAGE IF_ID;
IF_STAGE IF_ID;

void IF();
void IF_ID_READY();

#include <stdio.h>
#include <stdlib.h>

extern unsigned IF_HALT, ID_HALT, EX_HALT, DM_HALT, WB_HALT, FWD_RS_TO_EX, FWD_RT_TO_EX, FWD_RS_TO_ID, FWD_RT_TO_ID, STALL;
extern unsigned iImgLen, dImgLen, iImgLenResult, dImgLenResult, reg[32], PC, cycle;
extern char *iImgBuffer, *dImgBuffer;
extern char dMemory[1024], iMemory[1024];
extern FILE *err, *snap, *iImg, *dImg;

unsigned IF_HALT, ID_HALT, EX_HALT, DM_HALT, WB_HALT, FWD_RS_TO_EX, FWD_RT_TO_EX, FWD_RS_TO_ID, FWD_RT_TO_ID, STALL;
unsigned iImgLen, dImgLen, iImgLenResult, dImgLenResult, reg[32], PC, cycle;
char *iImgBuffer, *dImgBuffer;
char dMemory[1024], iMemory[1024];
FILE *err, *snap, *iImg, *dImg;

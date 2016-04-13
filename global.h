#include <stdio.h>
#include <stdlib.h>

extern unsigned iImgLen, dImgLen, iImgLenResult, dImgLenResult, reg[32], PC;
extern char *iImgBuffer, *dImgBuffer;
extern char dMemory[1024], iMemory[1024];
extern FILE *err, *snap, *iImg, *dImg;

unsigned iImgLen, dImgLen, iImgLenResult, dImgLenResult, reg[32], PC;
char *iImgBuffer, *dImgBuffer;
char dMemory[1024], iMemory[1024];
FILE *err, *snap, *iImg, *dImg;

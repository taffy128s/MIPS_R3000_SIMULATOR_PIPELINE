#include "defines.h"
#include "global.h"
#include "initial.h"

void openNLoadFiles() {
    // Open the files.
    iImg = fopen("iimage.bin", "rb");
    dImg = fopen("dimage.bin", "rb");
    err = fopen("error_dump.rpt", "wb");
    snap = fopen("snapshot.rpt", "wb");

    // Load the files.
    if (iImg == NULL || dImg == NULL)
        fputs("Please put iimage.bin and dimage.bin here.\n", stderr), exit(1);

    // Get the length of the files.
    fseek(iImg, 0, SEEK_END), fseek(dImg, 0, SEEK_END);
    iImgLen = (unsigned) ftell(iImg), dImgLen = (unsigned) ftell(dImg);

    // Move the pointers to the beginning of the files.
    rewind(iImg), rewind(dImg);

    // Allocate memory to contain the files.
    iImgBuffer = (char *) malloc(sizeof(char) * iImgLen);
    dImgBuffer = (char *) malloc(sizeof(char) * dImgLen);
    if (iImgBuffer == NULL || dImgBuffer == NULL)
        fputs("Memory allocation error.\n", stderr), exit(2);

    // Copy the file to the buffers.
    iImgLenResult = fread(iImgBuffer, 1, iImgLen, iImg);
    dImgLenResult = fread(dImgBuffer, 1, dImgLen, dImg);
    if (iImgLen != iImgLenResult || dImgLen != dImgLenResult)
        fputs("Reading error.\n", stderr), exit(3);

    // Close the files.
    fclose(iImg), fclose(dImg);
}

void dealWithDImg() {
    unsigned i, temp, idx = 0;
    // Get the value of $sp.
    for (i = 0; i < 4; i++)
        temp = (temp << 8) + (unsigned char) dImgBuffer[i];
    reg[$SP] = temp;
    // Get the number for D memory.
    temp = 0;
    for (i = 4; i < 8; i++)
        temp = (temp << 8) + (unsigned char) dImgBuffer[i];
    // Write the value to D memory.
    for (i = 8; i < 8 + 4 * temp; i++)
        dMemory[idx++] = dImgBuffer[i];
}

void dealWithIImg() {
    unsigned i, temp, idx;
    // Get the value of PC.
    for (i = 0; i < 4; i++)
        temp = (temp << 8) + (unsigned char) iImgBuffer[i];
    PC = temp;
    // Get the number for I memory.
    temp = 0;
    for (i = 4; i < 8; i++)
        temp = (temp << 8) + (unsigned char) iImgBuffer[i];
    // Write the value to I memory.
    idx = PC;
    for (i = 8; i < 8 + 4 * temp; i++)
        iMemory[idx++] = iImgBuffer[i];
}

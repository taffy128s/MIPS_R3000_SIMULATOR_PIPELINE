#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "initial.h"

int main() {
	openNLoadFiles();
    dealWithDImg();
    dealWithIImg();
	// TODO: pipeline
	return 0;
}
/* stack.c */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int bof(char *str){

	char buffer[12];

	// strcpy presenta un problema de buffer overflow
	strcpy(buffer, str);
	return 1;
}

int main(int argc, char **argv){

	char str[517];
	FILE *badfile;

	badfile = fopen("badfile", "r");
	fread(str, sizeof(char), 517, badfile);
	bof(str);

	return 1;
}

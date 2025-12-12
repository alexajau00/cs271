/****************************************
 * C-ploration 6 for CS 271
 * 
 * [NAME] Alexa Jauregui
 * [TERM] FALL 2025
 * 
 ****************************************/
#include "parser.h"
#include "symtable.h"

int main(int argc, const char *argv[])
{		

	// your code here
	if(argc != 2) {
		printf("Usage: %s[filename]\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	FILE *fin = fopen(argv[1], "r");
	if(fin == NULL) {
		perror("Unable to open file!");
		exit(EXIT_FAILURE);
	}	
	parse(fin);
	symtable_print_labels();
	fclose(fin);	
}



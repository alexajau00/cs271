/****************************************
 * C-ploration 4 for CS 271
 * 
 * [NAME] Alexa Jauregui
 * [TERM] FALL 2025
 * 
 ****************************************/
#include "parser.h"

/* Function: strip
 * -------------
 * remove whitespace and comments from a line
 *
 * s: the char* string to strip
 *
 * returns: the stripped char* string
 */
char *strip(char *s){	

	// your code here
	char s_new[strlen(s) + 1];
	int i = 0;
	for(char *s2 = s; *s2; s2++) {
		if(*s2 == '/' && *(s2+1) == '/'){
			break;
		} else if (!isspace(*s2)){
			s_new[i++] = *s2;
		}
	}
	s_new[i] = '\0';
	strcpy(s, s_new);
    return s;	
}

/* Function: parse
 * -------------
 * iterate each line in the file and strip whitespace and comments. 
 *
 * file: pointer to FILE to parse
 *
 * returns: nothing
 */
void parse(FILE * file){
	
	// your code here
	char line[MAX_LINE_LENGTH] = {0};
	while(fgets(line, sizeof(line), file)){
		strip(line);
		if(!*line){
			continue;
		}
		char inst_type;
		if(is_Atype(line)) {
			inst_type = 'A';
		} else if(is_label(line)){
			inst_type = 'L';
		} else {
			inst_type = 'C';
		}
		printf("%c  %s\n", inst_type, line);
	}
}


/* Function: is_Atype
 * -------------
 * Determine whether the line is an address instruction depending on if it starts with @
 *
 * line: the line of an assembly file
 *
 * returns: boolean
 */
bool is_Atype(const char *line){
	if(line[0] == '@'){
		return true;
	} else {
		return false;
	}
}


/* Function: is_label
 * -------------
 * Determine whether the line is a label depending on if it starts with '(' and ends with ')'
 *
 * line: the line of an assembly file
 *
 * returns: boolean
 */
bool is_label(const char *line){
	size_t len = strlen(line);
	if(line[0] == '(' && line[len - 1] == ')'){
		return true;
	} else{
		return false;
	}
}


/* Function: is_Ctype
 * -------------
 * Determine whether the line is a C-type instruction depending on if it's not a label or address instruction
 *
 * line: the line of an assembly file
 *
 * returns: boolean
 */
bool is_Ctype(const char *line){
	if(is_label(line) == false && is_Atype(line) == false){
		return true;
	} else{
		return false;
	}
}
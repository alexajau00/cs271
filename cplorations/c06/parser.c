/****************************************
 * C-ploration 6 for CS 271
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
	char label[MAX_LABEL_LENGTH] = {0};
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
			extract_label(line, label);
			strcpy(line, label);
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


/* Function: extract_label
 * -------------
 * Extracts the name of the label and puts it into the variable label
 *
 * line: the line of an assembly file
 * label: the label in the line
 *
 * returns: label
 */
char *extract_label(const char *line, char* label){
	strcpy(label, line + 1);
	label[strlen(label) - 1] = '\0';
	return label;
}


enum instr_type{
	Invalid = -1,
	Atype,
	Ctype,
};


typedef struct c_instruction {
	opcode a:1;
	opcode comp:6;
	opcode dest:3;
	opcode jump:3;
} c_instruction;


typedef struct a_instruction {
	union {
		hack_addr address;
		char * label;
	};
	bool is_addr;
} a_instruction;


typedef struct instruction {
	union{
		a_instruction a;
		c_instruction c;
	};
	enum instr_type type;
}instruction;
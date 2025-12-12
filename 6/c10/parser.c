/****************************************
 * C-ploration 6 for CS 271
 * 
 * [NAME] Alexa Jauregui
 * [TERM] FALL 2025
 * 
 ****************************************/
#include "parser.h"
#include "symtable.h"
#include "error.h"


/* Function: strip
 * 
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
 * returns: integer
 */
int parse(FILE * file, instruction *instructions){
	
	// your code here
	char line[MAX_LINE_LENGTH] = {0};
	char label[MAX_LABEL_LENGTH] = {0};
	char tmp_line[MAX_LINE_LENGTH] = {0};

	hack_addr instr_addr = 0;
	unsigned int line_num = 0;
	unsigned int instr_num = 0;
	char inst_type;
	instruction instr;
	add_predefined_symbols();

	while(fgets(line, sizeof(line), file)){
		line_num++;
		if(instr_num > MAX_INSTRUCTIONS) {
			exit_program(EXIT_TOO_MANY_INSTRUCTIONS, MAX_INSTRUCTIONS + 1);
		}
		strip(line);
		if(!*line){
			continue;
		}
		if(is_Atype(line)) {
			if (!parse_A_instruction(line, &instr.instr.a)){
    			exit_program(EXIT_INVALID_A_INSTR, line_num, line);
 			}
 			instr.itype = Atype;
			inst_type = 'A';
			instr_addr++;
			if(instr.instr.a.is_addr){
				printf("A: %d\n", instr.instr.a.address);
			} else {
				printf("A: %s\n", instr.instr.a.label);
			}
		} else if(is_label(line)){
			inst_type = 'L';
			extract_label(line, label);
			if(!isalpha((unsigned char)label[0])){
				exit_program(EXIT_INVALID_LABEL, line_num, label);
			} else if (symtable_find(label) != NULL){
				exit_program(EXIT_SYMBOL_ALREADY_EXISTS, line_num, label);
			} else {
				symtable_insert(label, instr_num);
				continue;
			}
		} else {
			inst_type = 'C';
			strcpy(tmp_line, line);
			parse_C_instruction(tmp_line, &instr.instr.c);

			c_instruction *c = &instr.instr.c;
			if(c->dest == DEST_INVALID){
				exit_program(EXIT_INVALID_C_DEST, line_num, line);
			}
			if(c->comp == COMP_INVALID){
				exit_program(EXIT_INVALID_C_COMP, line_num, line);
			}
			if(c->jump == JMP_INVALID){
				exit_program(EXIT_INVALID_C_JUMP,line_num, line);
			}

			instr.itype = Ctype;
			instr_addr++;
			printf("C: d=%d, c=%d, j=%d\n", instr.instr.c.dest, instr.instr.c.comp, instr.instr.c.jump);
		}
		//printf("%u: %c  %s\n", instr_num, inst_type, line);
		instructions[instr_num++] = instr;
	}
	return instr_num;
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

void add_predefined_symbols(){
	for(int i = 0; i < NUM_PREDEFINED_SYMBOLS; i++){
		predefined_symbol pre_sym = predefined_symbols[i];
		symtable_insert(pre_sym.name, pre_sym.address);
	}
}


bool parse_A_instruction(const char *line, a_instruction *instr){
	size_t len = strlen(line);
	char *s = malloc(len); 
	strcpy(s, line + 1);
	char *s_end = NULL;
	long result = strtol(s, &s_end, 10);
	if(s == s_end){
		instr->label = (char *)malloc(strlen(s) + 1);
		strcpy(instr->label, s);
		instr->is_addr = false;
		return true;
	} else if(*s_end != 0){
		return false;
	}else{
		instr->address = result;
		instr->is_addr = true;
	}
	return true;
}

void parse_C_instruction(char *line, c_instruction *instr){
	char *temp = NULL;
	char *jump_inst = NULL;
	char *destination_inst = NULL;
	char *computation_inst = NULL;

	temp = strtok(line, ";");
	jump_inst = strtok(NULL, ";");
	destination_inst = strtok(temp, "=");
	computation_inst = strtok(NULL, "=");

	if(computation_inst == NULL){
		computation_inst = destination_inst;
		destination_inst = NULL;
	}

	instr->jump = str_to_jumpid(jump_inst);
	int a_bit = 0;
	instr->comp = str_to_compid(computation_inst, &a_bit);
	instr->a = a_bit ? 1 : 0;
	instr->dest = str_to_destid(destination_inst);
}

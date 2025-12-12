/****************************************
 * C-ploration 4 for CS 271
 * 
 * [NAME] Alexa Jauregui
 * [TERM] FALL 2025
 * 
 ****************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

#define MAX_LINE_LENGTH  200

/** function prototypes **/
char *strip(char *s);
bool is_Atype(const char *);
bool is_label(const char *);
bool is_Ctype(const char *);

void parse(FILE * file);

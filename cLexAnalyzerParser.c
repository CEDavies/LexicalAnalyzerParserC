#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
int charClass;
char lexeme[100];
char nextChar;
int lexLen;
int token;
int nextToken;
FILE *in_fp;
FILE *out_fp;
void expr();
void term();
void factor();
void addChar();
void getChar();
void getNonBlank();
int lex();
#define LETTER 0
#define DIGIT 1
#define UNKNOWN 99
#define INT_LIT 10
#define IDENT 11
#define ASSIGN_OP 20
#define ADD_OP 21
//#define SUB_OP 22
#define MULT_OP 23
//#define DIV_OP 24
#define LEFT_PAREN 25
#define RIGHT_PAREN 26

int main(void) {
out_fp = fopen("groot2.txt", "w"); // write file
if ((in_fp = fopen("groot.txt", "r")) == NULL) {
printf("ERROR");
} else {
getChar();
do {
lex();
expr();
} while (nextToken != EOF);
/*do expr, term, factor
do {
expr();
} while (nextToken != EOF);*/
}
fclose(out_fp); // close the file
return 0;
}

int lookup(char ch) {
switch (ch) {
case '(':
addChar();
nextToken = LEFT_PAREN;
break;
case ')':
addChar();
nextToken = RIGHT_PAREN;
break;
case '+':
addChar();
nextToken = ADD_OP;
break;
case '-':
addChar();
nextToken = ADD_OP;
break;
case '*':
addChar();
nextToken = MULT_OP;
break;
case '/':
addChar();
nextToken = MULT_OP;
break;
}
return nextToken;
}
void addChar() {
if (lexLen <= 98) {
lexeme[lexLen++] = nextChar;
lexeme[lexLen] = 0;
}
}

void getChar() {
if ((nextChar = getc(in_fp)) != EOF) {
if (isalpha(nextChar))
charClass = LETTER;
else if (isdigit(nextChar))
charClass = DIGIT;
else
charClass = UNKNOWN;
} else
charClass = EOF;
}
void getNonBlank() {
while (isspace(nextChar)) {
getChar();
}
}

int lex() {
lexLen = 0;
getNonBlank();
switch (charClass) {
case LETTER:
addChar();
getChar();
while (charClass == LETTER || charClass == DIGIT) {
addChar();
getChar();
}
nextToken = IDENT;
break;
case DIGIT:
addChar();
getChar();
while (charClass == DIGIT) {
addChar();
getChar();
}
nextToken = INT_LIT;
break;
case UNKNOWN:
lookup(nextChar);
getChar();
break;
case EOF:
nextToken = EOF;
lexeme[0] = 'E';
lexeme[1] = 'O';
lexeme[2] = 'F';
lexeme[3] = 0;
break;
}
return nextToken;
}

void expr() {
/*char name[100];*/
fprintf(out_fp, "[expr");
/*scanf("%s", name); */
term();
while (nextToken == ADD_OP) {
fprintf(out_fp, "\n[%s]", lexeme);
lex();
term();
}
fprintf(out_fp, "\n]");
}

void term() {
fprintf(out_fp, "\n[term\n");
factor();
while (nextToken == MULT_OP) {
fprintf(out_fp, "\n[%s]", lexeme);
lex();
factor();
}
fprintf(out_fp, "\n  ]");
}

void factor() {
fprintf(out_fp, "   [factor \n");
fprintf(out_fp, "\t [");
if (nextToken == IDENT || nextToken == INT_LIT) {
fprintf(out_fp, "id[%s]]\n", lexeme);
lex();
} else {
if (nextToken == LEFT_PAREN) {
fprintf(out_fp, "\n[%s]", lexeme);
lex();
expr();
if (nextToken == RIGHT_PAREN) {
fprintf(out_fp, "\n[%s]", lexeme);
lex();
} else
fprintf(out_fp, "error");
} else
fprintf(out_fp, "error");
}
fprintf(out_fp, "\t]");
}
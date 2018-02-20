#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

#define MAX_BUFFER_SIZE 1024

int flagBuffer = -1;
char fBuffer[MAX_BUFFER_SIZE+1];
char sBuffer[MAX_BUFFER_SIZE+1];
int currBufferIndex = -1;
int lineno = 0;

typedef struct {
	char token[21];
	char value[25];
	int lineno;
} tokenInfo;

void removeComments(char *testcaseFile, char *cleanFile);
FILE *getStream(FILE *fp);
char getNextCharacter(FILE *fp);
tokenInfo* setTokenInfo(char *token, char* value, int lineno);
void readTokenInfo(tokenInfo *ti);
tokenInfo* getNextToken(FILE *fp);
char viewNextChar();
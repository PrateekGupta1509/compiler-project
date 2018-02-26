// PRATEEK GUPTA
// 2014B4A70767P

#ifndef LEXER_DEF
#define LEXER_DEF

#define MAX_BUFFER_SIZE 1024

typedef struct {
	char token[21];
	char value[25];
	unsigned long long lineno;
} tokenInfo;

typedef struct keywordHT {
	char token[10];
	char value[10];
	struct keywordHT * next;
} keywordHT;

#endif
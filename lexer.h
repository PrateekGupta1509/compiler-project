// PRATEEK GUPTA
// 2014B4A70767P

#include "lexerDef.h"

void removeComments(char *testcaseFile);
FILE *getStream(FILE *fp);
char getNextCharacter(FILE *fp);
tokenInfo* setTokenInfo(char *token, char* value, unsigned long long lineno);
void readTokenInfo(tokenInfo *ti);
tokenInfo* getNextToken(FILE *fp);
char viewNextChar();

int hashKeyword(char *value);
void addKeyword(char *value, char *token);
void initKeywordMap();
tokenInfo * keywordMap(char* value, unsigned long long lineno);

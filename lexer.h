// PRATEEK GUPTA
// 2014B4A70767P

#include "lexerDef.h"

void removeComments(char *testcaseFile);
FILE *getStream(FILE *fp);
char getNextCharacter(FILE *fp);
tokenInfo* setTokenInfo(char *token, char* value, int lineno);
void readTokenInfo(tokenInfo *ti);
tokenInfo* getNextToken(FILE *fp);
char viewNextChar();
tokenInfo * keywordMap(char* value, int lineno);

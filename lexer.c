// PRATEEK GUPTA
// 2014B4A70767P

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "lexerDef.h"

int flagBuffer = -1;
char fBuffer[MAX_BUFFER_SIZE+1];
char sBuffer[MAX_BUFFER_SIZE+1];
int currBufferIndex = -1;
unsigned long long lineno = 0;

keywordHT *keywordTable[10];

tokenInfo* setTokenInfo(char *token, char* value, unsigned long long lineno) {
	tokenInfo *ti = (tokenInfo *) malloc (sizeof(tokenInfo));
	if(ti == NULL)
		return NULL;

	strcpy( ti->token, token);
	strcpy( ti->value, value);
	ti->lineno = lineno;
	return ti;
}

void readTokenInfo(tokenInfo *ti) {
	if(ti != NULL)
		printf("%-25s  %-25s  %-10llu\n", ti->token, ti->value, ti->lineno);
}

FILE* getStream(FILE *fp){
	int i = -1,j = -1;
	if(fp == NULL)
		return NULL;
	switch(flagBuffer){
		case -1:	
					i = fread(fBuffer, sizeof(char), MAX_BUFFER_SIZE, fp);
					j = fread(sBuffer, sizeof(char), MAX_BUFFER_SIZE, fp);
					fBuffer[i] = '\0';
					sBuffer[j] = '\0';
					flagBuffer = 0;
					lineno = 1;
					break;
		case 0:
					j = fread(sBuffer, sizeof(char), MAX_BUFFER_SIZE, fp);
					sBuffer[j] = '\0';
					break;
		case 1:
					i = fread(fBuffer, sizeof(char), MAX_BUFFER_SIZE, fp);
					fBuffer[i] = '\0';
					break;
	}
	return fp;
}

char getNextCharacter(FILE *fp){
	++currBufferIndex;
	if(currBufferIndex == MAX_BUFFER_SIZE){
		currBufferIndex = 0;
		switch(flagBuffer) {
			case 0:	
					flagBuffer = 1;
					fp = getStream(fp);
					break;
			case 1: flagBuffer = 0;
					fp = getStream(fp);
					break;
		}
	}
	switch(flagBuffer) {
		case -1: 	fp = getStream(fp);
		case 0: 	return fBuffer[currBufferIndex];
		case 1:		return sBuffer[currBufferIndex];
	}
	return -1;
}

char viewNextChar(){
	int i = currBufferIndex + 1;
	int flag = flagBuffer;
	if(i == MAX_BUFFER_SIZE){
		i = 0;
		if(flag == 0) flag = 1;
		else flag = 0;
	}
	switch(flag) {
		case 0: return fBuffer[i];
		case 1: return sBuffer[i];
	}
	return -1;
}

void removeComments(char *testcaseFile) {
	FILE *infp = fopen(testcaseFile, "r");
	if(infp == NULL){
		return;
	}
	char c;
	c = getNextCharacter(infp);
	while( c != '\0') {
		if(c == '#'){
			c = getNextCharacter(infp);
			while( c != '\n' && c != '\0'){
				c = getNextCharacter(infp);
			};
		}
		if(c != '\0') {
			printf("%c",c);
			c = getNextCharacter(infp);
		}
	};
	fclose(infp);
	currBufferIndex = -1;
	flagBuffer = -1;
}

int hashKeyword(char *value) {
	int hash = 0;
	int i = 0, a;
    while(value[i] != '\0'){  
        int a = value[i] - '5';
        hash = (hash * 17) + a;
        i++; 
    } 
    return hash % 10;
}

void addKeyword(char *value, char *token) {
	keywordHT * kht = (keywordHT *) malloc (sizeof(keywordHT));

	strcpy(kht->token,token);
	strcpy(kht->value,value);
	kht->next = NULL;

	int i = hashKeyword(value);
	// printf("%s %d\n",value,i );
	keywordHT *tmp = NULL;
	if(keywordTable[i] == NULL){
		keywordTable[i] = kht;
	}
	else{
		tmp = keywordTable[i];
		while(tmp->next != NULL){
			tmp = tmp->next;
		}
		tmp->next = kht;
	}
}

void initKeywordMap(){
	int i; 
	for(i = 0; i<10; i++){
		keywordTable[i] = NULL;
	}
	addKeyword("end","END");
	addKeyword("int","INT");
	addKeyword("real","REAL");
	addKeyword("string","STRING");
	addKeyword("matrix","MATRIX");
	addKeyword("if","IF");
	addKeyword("else","ELSE");
	addKeyword("endif","ENDIF");
	addKeyword("read","READ");
	addKeyword("print","PRINT");
	addKeyword("function","FUNCTION");
}

tokenInfo * keywordMap(char* value, unsigned long long lineno){
	int hash = -1;
	hash = hashKeyword(value);
	keywordHT * tmp = keywordTable[hash];
	while(tmp != NULL){
		if(strcmp(tmp->value, value) == 0){
			return setTokenInfo(tmp->token, tmp->value, lineno);
		}
		tmp = tmp->next;
	}
	return setTokenInfo("ID",value,lineno); 
}

tokenInfo * getNextToken(FILE *fp) {
	char ch;
	int i;
	char buff[25], tbuff[50];
	if(fp == NULL)
		return NULL;

	ch = getNextCharacter(fp);
	switch(ch) {
		case '\n':
		case '\r':
		case '\t':
		case ' ':	
					while( ch == '\n' || ch == '\r' || ch == ' ' || ch == '\t'){
						if(ch == '\n')
							++lineno;
						ch = getNextCharacter(fp);
					}
					--currBufferIndex;
					return getNextToken(fp);

		case '\0':	--currBufferIndex;
					return setTokenInfo("EOF","EOF",lineno);
		case '[':	return setTokenInfo("SQO","[",lineno);
		case ']':	return setTokenInfo("SQC","]",lineno);
		case '(':	return setTokenInfo("OP","(",lineno);
		case ')':	return setTokenInfo("CL",")",lineno);
		case ';':	return setTokenInfo("SEMICOLON",";",lineno);
		case ',':	return setTokenInfo("COMMA",",",lineno);
		case '+':	return setTokenInfo("PLUS","+",lineno);
		case '-':	return setTokenInfo("MINUS","-",lineno);
		case '*':	return setTokenInfo("MUL","*",lineno);
		case '/':	return setTokenInfo("DIV","/",lineno);
		case '@':	return setTokenInfo("SIZE","@",lineno);

		case '<':	
					ch = getNextCharacter(fp);
					if(ch == '='){
						return setTokenInfo("LE","<=",lineno);
					}
					--currBufferIndex;
					return setTokenInfo("LT","<",lineno);

		case '>':	
					ch = getNextCharacter(fp);
					if(ch == '='){
						return setTokenInfo("GE",">=",lineno);
					}
					--currBufferIndex;
					return setTokenInfo("GT",">",lineno);

		case '=':	
					ch = getNextCharacter(fp);
					if(ch == '='){
						return setTokenInfo("EQ","==",lineno);
					}
					else if(ch == '/'){
						ch = viewNextChar();
						if(ch == '='){
							ch = getNextCharacter(fp);
							return setTokenInfo("NE","=/=",lineno);
						}
						--currBufferIndex;
						return setTokenInfo("ASSIGNOP","=",lineno);
					}
					--currBufferIndex;
					return setTokenInfo("ASSIGNOP","=",lineno);

		case '"':	
					i = 0;
					buff[i++] = ch; 
					ch = getNextCharacter(fp);
					while(i<21){
						if(!isalpha(ch) && ch != ' '){
							--currBufferIndex;
							buff[i++] = ch;
							buff[i] = '\0';
							sprintf(tbuff,"UP %s",buff);
							return setTokenInfo("Lexical error",tbuff,lineno);
						}
						buff[i++] = ch;
						ch = getNextCharacter(fp);
						if(ch == '"'){
							buff[i++] = '"';
							buff[i] = '\0';
							return setTokenInfo("STR",buff,lineno);
						}
					}
					if(i >= 21){
						while(isalpha(ch) || ch == ' '){
							buff[i++] = ch;
							ch = getNextCharacter(fp);
						}
						if(ch == '"'){
							return setTokenInfo("Lexical error","Length exceeded",lineno);
						}
						--currBufferIndex;
						sprintf(tbuff,"UP %s",buff);
						return setTokenInfo("Lexical error",tbuff,lineno);
					}
					--currBufferIndex;
					return setTokenInfo("INVALID","INVALID",lineno);

		case '_':
					i = 0;
					tbuff[i] = ch;
					buff[i++] = ch; 
					ch = getNextCharacter(fp);
					if(isalpha(ch)){
						tbuff[i] = ch;
						buff[i++] = ch;
						ch = getNextCharacter(fp);
					}
					else{
						--currBufferIndex;
						tbuff[i] = '\0';
						sprintf(buff,"UP %c",tbuff);
						return setTokenInfo("Lexical error",buff,lineno);
					}

					while(i<21){
						if(!isalnum(ch)){
							buff[i] = '\0';
							--currBufferIndex;
							if(strcmp(buff,"_main") == 0)
								return setTokenInfo("MAIN","_main",lineno);
							else
								return setTokenInfo("FUNID",buff,lineno);
						}
						buff[i++] = ch;
						ch = getNextCharacter(fp);
					}

					if(i >= 21){
						while(isalnum(ch)){
							ch = getNextCharacter(fp);
						}
						--currBufferIndex;
						return setTokenInfo("Lexical error","Length exceeded",lineno);
					}
					// buff[i] = '\0';
					// --currBufferIndex;
					// return setTokenInfo("FUNID",buff,lineno);

		case 'a':
		case 'b':
		case 'c':
		case 'd':
		case 'e':
		case 'f':
		case 'g':
		case 'h':
		case 'i':
		case 'j':
		case 'k':
		case 'l':
		case 'm':
		case 'n':
		case 'o':
		case 'p':
		case 'q':
		case 'r':
		case 's':
		case 't':
		case 'u':
		case 'v':
		case 'w':
		case 'x':
		case 'y':
		case 'z':
		case 'A':
		case 'B':
		case 'C':
		case 'D':
		case 'E':
		case 'F':
		case 'G':
		case 'H':
		case 'I':
		case 'J':
		case 'K':
		case 'L':
		case 'M':
		case 'N':
		case 'O':
		case 'P':
		case 'Q':
		case 'R':
		case 'S':
		case 'T':
		case 'U':
		case 'V':
		case 'W':
		case 'X':
		case 'Y':
		case 'Z':	
					i = 0;
					buff[i++] = ch;
					ch = getNextCharacter(fp);
					while(isalpha(ch) && i < 20){
						buff[i++] = ch;
						ch = getNextCharacter(fp);
					}
					if(isdigit(ch) && i< 20){
						buff[i++] = ch;
						buff[i] = '\0';
						return keywordMap(buff,lineno);
					}
					if(!isalnum(ch)){
						buff[i] = '\0';
						--currBufferIndex;
						return keywordMap(buff,lineno);
					}
					if(i >= 20){
						while(isalpha(ch)){
							ch = getNextCharacter(fp);
						}
						if(isdigit(ch)){
							return setTokenInfo("Lexical error","Length exceeded", lineno);
						}
						--currBufferIndex;
						return setTokenInfo("Lexical error","Length exceeded", lineno);
					}
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
					i = 0;
					buff[i++] = ch;
					ch = getNextCharacter(fp); 
					while(isdigit(ch)){
						buff[i++] = ch;
						ch = getNextCharacter(fp);
					}
					if(ch != '.') {
						buff[i] = '\0';
						--currBufferIndex;
						return setTokenInfo("NUM",buff,lineno);
					}
					if(isdigit(viewNextChar())) {
						ch = getNextCharacter(fp);
						buff[i++] = '.';
						buff[i++] = ch;
						ch = getNextCharacter(fp);
						if(isdigit(ch)){
							buff[i++] = ch;
							buff[i] = '\0';
							return setTokenInfo("RNUM",buff,lineno);
						}
						--currBufferIndex;
						buff[i++] = ch;
						buff[i] = '\0';
						sprintf(tbuff,"UP %s", buff);
						return setTokenInfo("Lexical error",tbuff,lineno);
					}

					buff[i] = '\0';
					--currBufferIndex;
					return setTokenInfo("NUM2",buff,lineno);

		case '.':	i = 0;
					tbuff[i++] = '.';
					ch = getNextCharacter(fp);
					if( ch == 'n'){
						tbuff[i++] = 'n';
						ch = getNextCharacter(fp);
						if(ch == 'o'){
							tbuff[i++] = 'o';
							ch = getNextCharacter(fp);
							if (ch == 't'){
								tbuff[i++] = 't';
								ch = getNextCharacter(fp);
								if (ch == '.')
									return setTokenInfo("NOT",".not.",lineno);
							}
						}
					}
					else if( ch == 'a'){
						tbuff[i++] = 'a';
						ch = getNextCharacter(fp);
						if(ch == 'n'){
							tbuff[i++] = 'n';
							ch = getNextCharacter(fp);
							if (ch == 'd'){
								tbuff[i++] = 'd';
								ch = getNextCharacter(fp);
								if (ch == '.')
									return setTokenInfo("AND",".and.",lineno);
							}
						}
					}
					else if( ch == 'o'){
						tbuff[i++] = 'o';
						ch = getNextCharacter(fp);
						if(ch == 'r'){
							tbuff[i++] = 'r';
							ch = getNextCharacter(fp);
							if (ch == '.')
								return setTokenInfo("OR",".or.",lineno);
						}
					}
					tbuff[i++] = ch;
					tbuff[i] = '\0';
					sprintf(buff,"UP %s", tbuff);
					--currBufferIndex;
					return setTokenInfo("Lexical error",buff,lineno);

		case '#':	ch = getNextCharacter(fp);
					while( ch != '\n' && ch != '\0'){
						ch = getNextCharacter(fp);
					};
					--currBufferIndex;
					return setTokenInfo("COMMENT","COMMENT",lineno);
					
		default:	
					// printf("%c\n",ch );
					sprintf(buff,"Unknown Symbol %c",ch);
					return setTokenInfo("Lexical error", buff,lineno);
	};
};

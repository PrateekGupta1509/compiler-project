#include "lexer.h"

int main( int charc, char **charv){
	char *fileName = charv[1];
	FILE *fp = fopen(fileName, "r");
	tokenInfo *ti;
	ti = getNextToken(fp); 
	while( strcmp(ti->token,"EOF")  != 0){
		readTokenInfo(ti);
		ti = getNextToken(fp);
	}
	return 0;
}

tokenInfo * getNextToken(FILE *fp) {
	char ch;
	int i;
	char buff[25];
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
						if(!isalpha(ch)){
							--currBufferIndex;
							return setTokenInfo("INVALID","INVALID",lineno);
						}
						buff[i++] = ch;
						ch = getNextCharacter(fp);
						if(ch == '"'){
							buff[i++] = '"';
							buff[i] = '\0';
							return setTokenInfo("STR",buff,lineno);
						}
					}
					--currBufferIndex;
					return setTokenInfo("INVALID","INVALID",lineno);

		case '_':
					i = 0;
					buff[i++] = ch; 
					ch = getNextCharacter(fp);
					if(isalpha(ch)){
						buff[i++] = ch;
						ch = getNextCharacter(fp);
					}
					else{
						--currBufferIndex;
						return setTokenInfo("INVALID","INVALID",lineno);
					}

					while(i<21){
						if(!isalnum(ch)){
							buff[i] = '\0';
							--currBufferIndex;
							return setTokenInfo("FUNID",buff,lineno);
						}
						buff[i++] = ch;
						ch = getNextCharacter(fp);
					}
					buff[i] = '\0';
					--currBufferIndex;
					return setTokenInfo("FUNID",buff,lineno);

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
					while(isalpha(ch) && i < 21){
						buff[i++] = ch;
						ch = getNextCharacter(fp);
					}
					if(isdigit(ch) && i< 20){
						buff[i++] = ch;
						buff[i] = '\0';
						return setTokenInfo("ID",buff,lineno);
					}
					if(isalpha(ch) && i >= 21){
						--currBufferIndex;
						return setTokenInfo("INVALID","INVALID", lineno);
					}

					buff[i] = '\0';
					--currBufferIndex;
					return setTokenInfo("ID",buff,lineno);

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
						return setTokenInfo("INVALID","INVALID",lineno);
					}

					buff[i] = '\0';
					--currBufferIndex;
					return setTokenInfo("NUM2",buff,lineno);

		case '.':	
					ch = getNextCharacter(fp);
					if( ch == 'n'){
						ch = getNextCharacter(fp);
						if(ch == 'o'){
							ch = getNextCharacter(fp);
							if (ch == 't'){
								ch = getNextCharacter(fp);
								if (ch == '.')
									return setTokenInfo("NOT",".not.",lineno);
							}
						}
					}
					else if( ch == 'a'){
						ch = getNextCharacter(fp);
						if(ch == 'n'){
							ch = getNextCharacter(fp);
							if (ch == 'd'){
								ch = getNextCharacter(fp);
								if (ch == '.')
									return setTokenInfo("AND",".and.",lineno);
							}
						}
					}
					else if( ch == 'o'){
						ch = getNextCharacter(fp);
						if(ch == 'r'){
							ch = getNextCharacter(fp);
							if (ch == '.')
								return setTokenInfo("OR",".or.",lineno);
						}
					}
					--currBufferIndex;
					return setTokenInfo("INVALID","INVALID",lineno);

		case '#':	ch = getNextCharacter(fp);
					while( ch != '\n' && ch != '\0'){
						ch = getNextCharacter(fp);
					};
					--currBufferIndex;
					return setTokenInfo("COMMENT","COMMENT",lineno);
					
		default:	printf("%c\n",ch );
					return setTokenInfo("INVALID","INVALID",lineno);
	};
};

tokenInfo* setTokenInfo(char *token, char* value, int lineno) {
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
		printf("%s %s %d\n", ti->token, ti->value, ti->lineno);
}


void removeComments(char *testcaseFile, char *cleanFile) {
	FILE *infp = fopen(testcaseFile, "r");
	FILE *outfp = fopen(cleanFile, "w");
	char c;
	while( (c = fgetc(infp)) != EOF) {
		if(c == '#'){
			printf("ig %c", c);
			c = fgetc(infp);
			while( c != '\n' && c != EOF){
				c = fgetc(infp);
			};
			fputc('\n',outfp);
		}
		else{
			fputc(c,outfp);
		}
	};
	fclose(infp);
	fclose(outfp);
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
}
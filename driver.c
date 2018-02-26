#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "parser.h"

int main(int argc, char ** argv){
	int choice;

	printf("(a) FIRST is automated and FOLLOW set is hardcoded.\n");
	printf("(c) Both lexical and syntax analysis modules implemented.\n");

	// 	(a) FIRST and FOLLOW set automated
	// (b) Only Lexical analyzer module developed
	// (c) Both lexical and syntax analysis modules implemented
	// (d) modules compile but give segmentation fault
	// (e) modules work with test cases 2, 3 and 4 only
	// (f) parse tree could not be constructed

	printf("\nPress option for the defined task.\n");
	printf("1 : For removal of comments ‐ print the comment free code on the console.\n");
	printf("2 : For printing the token list (on the console) generated by the lexer.\n");
	printf("3 : For parsing to verify the syntactic correctness of the input source code.\n");
	printf("4 : For printing the parse tree appropriately.\n");
	printf("Enter your choice: ");
	scanf("%d", &choice);
	FILE *fp = NULL;
	FILE *outfp = NULL;
	tokenInfo *ti = NULL;

	nonTerminalList * ntlHead = NULL;
	parseTree * ptHead = NULL;
	char c;
	initKeywordMap();

	switch(choice){
		case 1:
				printf("%s\n", argv[1]);
				removeComments(argv[1]);
				break;
		case 2:
				fp = fopen(argv[1], "r");
				ti = getNextToken(fp);
				while( strcmp(ti->token,"EOF")  != 0){
					if(strcmp(ti->token, "Lexical error") == 0){
						if((ti->value)[0] == 'L'){
							printf("line no.:%d %s: %s\n",ti->lineno, ti->token, "Identifier is longer than the prescribed length");
						}
						else if((ti->token)[1] == 'P'){
							printf("line no.:%d %s: Unknown Pattern %s\n",ti->lineno, ti->token, ((ti->value)+3));
						}
						else{
							printf("line no.:%d %s: %s\n",ti->lineno, ti->token, ti->value);
						}
					}
					else{
						readTokenInfo(ti);
					}
					ti = getNextToken(fp);
				}
				fclose(fp);
				break;
		case 3:	
				ntlHead = readGrammar("grammar.txt");
				computeFirstSet(ntlHead);
				readFollowSet("follow.txt",ntlHead);
				generateParseTable(ntlHead);
				ptHead = initParseTree(ptHead);
				generateParseTree(ptHead, ntlHead, argv[1]);
				outfp = fopen(argv[2],"w");
				printParseTree(outfp,ptHead);
				fclose(outfp);
				break;
		case 4:	
				outfp = fopen(argv[2],"r");
				while((c=fgetc(outfp))!=EOF){
					printf("%c",c);
				}
				fclose(outfp);
				break;
		default:
				printf("Invalid Choice!\n");
	}
	return 0;
}
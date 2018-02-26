// PRATEEK GUPTA
// 2014B4A70767P
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "parserDef.h"
#include "lexer.h"

parseTable pt[50][50];
parseStack *psHead = NULL; 
parseStack *dummyStack = NULL; 

// int main(int charc, char ** charv){
// 	// printf("yo\n");
// 	nonTerminalList * ntl = readGrammar("grammar.txt");
// 	// printGrammar(ntl);
// 	computeFirstSet(ntl);
// 	// printFirstSet(ntl);
// 	// computeFollowSet(ntl);
// 	readFollowSet("follow.txt",ntl);
// 	// printFollowSet(ntl);
// 	generateParseTable(ntl);
// 	// printParseTable();
// 	generateParseTree(charv[1]);
// 	printParseTree(ptHead);
// 	return 0;
// 	// ask main ke end ke baad semicolon
// }

char * stripNonTerminal(char *value){
	++value;
	value[strlen(value)-1] = '\0';
	return value;
} 


ruleNode * createRuleNode(char * value){
	ruleNode * rn = (ruleNode *) malloc (sizeof(ruleNode));
	if(rn == NULL)
		return NULL;
	if(value[0] == '<'){
		strcpy( rn->value, stripNonTerminal(value));
		// rn->value = stripNonTerminal(value);
		rn->isTerminal = 0;
	}
	else{
		strcpy(rn->value, value);
		// rn->value = value;
		rn->isTerminal = 1;
	}
	rn->next = NULL;
	return rn;
}

ruleList* createRule(ruleNode * rn){
	ruleList *rlh = (ruleList *) malloc (sizeof(ruleList));
	if(rlh == NULL)
		return NULL;
	rlh->ruleHead = rn;
	rlh->firstHead = NULL;
	rlh->next = NULL;
	return rlh; 
}

nonTerminalList * createNTNode(char * nonTerminal){
	nonTerminalList * ntl = (nonTerminalList *) malloc (sizeof(nonTerminalList));
	if(ntl == NULL)
		return NULL;
	strcpy(ntl->value, stripNonTerminal(nonTerminal));
	// ntl->value = stripNonTerminal(nonTerminal) ;
	ntl->ruleListHead = NULL; 
	ntl->followHead = NULL;
	ntl->next = NULL; 
	return ntl;
}

nonTerminalList * readGrammar (char * fileName) {
	FILE * fp = fopen(fileName,"r");
	
	char line[30];
	char prevNT[25];
	int i = 0;

	nonTerminalList * ntlHead = NULL;
	nonTerminalList * ntl = NULL;
	ruleList * rule = NULL;
	ruleNode *rn = NULL;

	i = fscanf(fp,"%s",line);
	// printf("%s\n", line);
	while(i != -1){
		// read non terminal
		if(strcmp(line, prevNT) != 0){
			// printf("%s\n",line);
			strcpy(prevNT, line);
			if(ntlHead == NULL){
				ntlHead = createNTNode(line);
				ntl = ntlHead;
			}
			else {
				ntl->next = createNTNode(line);
				ntl = ntl->next;
			}
			rule = NULL;
			rn = NULL;
		}
		// printf("\n%s",prevNT);
		//read ===>
		i = fscanf(fp,"%s",line);
		// printf(" %s", line);
		//first Token
		i = fscanf(fp,"%s",line);
		// printf(" %s", line);
		rn = createRuleNode(line);

		if(ntl->ruleListHead == NULL){
			ntl->ruleListHead = createRule(rn);
			rule = ntl->ruleListHead;
		}
		else{
			rule->next = createRule(rn);
			rule = rule->next;
		}
		
		i = fscanf(fp,"%s",line);
		// printf(" %s", line);

		while(line[0] != '#' ){
			rn->next = createRuleNode(line);
			rn = rn->next;
			// printf(" %s", line);
			i = fscanf(fp,"%s",line);
		}
		i = fscanf(fp,"%s",line);
		// break;
	}
	fclose(fp);
	return ntlHead;
}

void printGrammar(nonTerminalList * ntlHead){
	nonTerminalList * ntl = ntlHead;
	ruleList * rl = NULL;
	ruleNode *rn = NULL;
	int i = 1;
	while(ntl != NULL){
		printf("%d. %s\n", i++, ntl->value);
		rl = ntl->ruleListHead;
		while(rl != NULL){
			rn = rl->ruleHead;
			printf("----->");
			while(rn != NULL){
				printf(" %s%d",rn->value,rn->isTerminal);
				rn = rn->next;
			}
			printf("\n");
			rl = rl->next;
		}
		ntl = ntl->next;
		printf("\n");
	}
}

nonTerminalList * getNonTerminal(nonTerminalList *ntlHead, char * value){
	nonTerminalList * ntl = ntlHead;
	while(ntl != NULL){
		if( strcmp(ntl->value, value) == 0){
			return ntl;
		}
		ntl = ntl->next;
	}
	return NULL;
}

setNode * createSetNode(char *value){
	setNode *fis = (setNode *) malloc (sizeof(setNode));
	// printf("--%s\n", value);
	strcpy(fis->value, value);
	fis->next = NULL; 
	return fis;
}

setNode * recurseFirstSet(nonTerminalList* ntlHead, ruleNode * ruleHead){
	if(ruleHead->isTerminal == 1)
		return createSetNode(ruleHead->value);
	else{
		ruleList * rl = (getNonTerminal(ntlHead, ruleHead->value))->ruleListHead;
		setNode * snh = NULL;
		setNode * sn = NULL;
		while(rl != NULL){
			rl->firstHead = recurseFirstSet(ntlHead, rl->ruleHead);
			setNode * tmp = rl->firstHead;
			while(tmp != NULL){
				if(snh == NULL){
					snh = createSetNode(tmp->value);
				}
				else{
					sn = snh;
					while(sn->next != NULL){
						if(strcmp(tmp->value,sn->value) == 0){
							sn = NULL;
							break;
						}
						sn = sn->next;
					}
					if(sn != NULL){
						if(strcmp(sn->value, tmp->value) != 0){
							sn->next = createSetNode(tmp->value);
						}
					}
				}
				tmp = tmp->next;
			}
			rl = rl->next;
		}
		return snh;
	}
}

void computeFirstSet(nonTerminalList *ntlHead) {
	nonTerminalList * ntl = ntlHead;
	ruleList * rl = NULL;

	while(ntl != NULL){
		rl = ntl->ruleListHead;
		// printf("<%s>\n", ntl->value);
		while(rl != NULL){
			if(rl->firstHead == NULL){
				rl->firstHead = recurseFirstSet(ntlHead, rl->ruleHead);
			}
			// break;
			rl = rl->next;
		}
		// break;
		ntl = ntl->next;
	}
}

void printFirstSet(nonTerminalList * ntlHead){
	nonTerminalList * ntl = ntlHead;
	ruleList * rl = NULL;
	setNode * sn = NULL;

	while(ntl != NULL){
		printf("%s\n", ntl->value);
		rl = ntl->ruleListHead;
		while(rl != NULL){
			sn = rl->firstHead;
			printf("-----");
			while(sn != NULL){
				printf(" %s;",sn->value);
				sn = sn->next;
			}
			printf("\n");
			rl = rl->next;
		}
		ntl = ntl->next;
		printf("\n");
	}
}


// void computeFollowSet(nonTerminalList * ntlHead){
// 	nonTerminalList * ntl = ntlHead;
// 	ruleList * rl = NULL;
// 	ruleNode * rn = NULL;
// 	setNode * sn = NULL;
// 	// for start symbol main function
// 	if(strcmp("mainFunction", ntlHead->value) == 0){
// 		ntl->followHead = createSetNode("EOF");
// 	}
// 	while(ntl != NULL){
// 		rl = ntl->ruleListHead;
// 		while(rl != NULL){
// 			rn = rl->ruleHead;
// 			while(rn != NULL){
// 				if(rn->isTerminal == 0){
// 					if(rn->next == NULL){
// 						if(strcmp("more_ids", rn->value) == 0){
// 							// printf("%s\n----------------\n",rn->value);
// 							// printSet(ntl->followHead );
// 						}
// 						appendFollowSet( getNonTerminal(ntlHead, rn->value), ntl->followHead);
// 					}
// 					else if(rn->next->isTerminal == 1){
// 						appendFollowSet( getNonTerminal(ntlHead, rn->value), createSetNode(rn->next->value));
// 					}
// 					else{
// 						// printf("%s\n----------------\n",rn->value);
// 						// printSet(ntl->followHead);
// 						recurseFollowSet(ntlHead, getNonTerminal(ntlHead, rn->value), ntl, rn->next);
// 					}
// 				}
// 				rn = rn->next;
// 			}
// 			rl = rl->next;
// 		}
// 		ntl = ntl->next;
// 	}
// }

// void appendFollowSet(nonTerminalList * ntl, setNode * followSet) {
// 	if(followSet == NULL){
// 		return;
// 	}
// 	setNode * snh = ntl->followHead;
// 	setNode * sn = NULL;
// 	setNode * fo = followSet;
// 	if(snh == NULL){
// 		ntl->followHead = createSetNode(fo->value);
// 		snh = ntl->followHead; 
// 		fo = fo->next;
// 	}
// 	while (fo != NULL){
// 		sn = snh;
// 		while(sn->next != NULL){
// 			if(strcmp(sn->value, fo->value) == 0){
// 				sn = NULL;
// 				break;
// 			}
// 			sn = sn->next;
// 		}
// 		if(sn != NULL){
// 			if(strcmp(sn->value, fo->value) != 0){
// 				sn->next = createSetNode(fo->value);
// 			}
// 		}
// 		fo = fo->next;
// 	}
// }

// void recurseFollowSet(nonTerminalList *ntlHead, nonTerminalList *fntl, nonTerminalList * lhsntl, ruleNode * nrn){
// 	nonTerminalList *ntl = getNonTerminal(ntlHead ,nrn->value);
// 	// printf("%s\n", ntl->value );
// 	ruleList * rl = ntl->ruleListHead;
// 	setNode *sn = NULL;
// 	while(rl != NULL){
// 		sn = rl->firstHead;
// 		while(sn != NULL){
// 			if(strcmp(sn->value, "EPSILON") == 0){
// 				if(nrn->next == NULL){
// 					appendFollowSet(fntl, lhsntl->followHead);
// 				}
// 				else if(nrn->next->isTerminal == 1){
// 					// printSet(lhsntl->followHead);
// 					// printf("%s\n", fntl->next->value);
// 					appendFollowSet(fntl, createSetNode(nrn->next->value));
// 				}
// 				else{
// 					recurseFollowSet(ntlHead, fntl, lhsntl, nrn->next);
// 				}
// 			}
// 			else{
// 				appendFollowSet(fntl, createSetNode(sn->value));
// 			}
// 			sn = sn->next;
// 		}
// 		rl = rl->next;
// 	}
// }

void readFollowSet(char *fileName, nonTerminalList * ntlHead){
	nonTerminalList *ntl = ntlHead;
	FILE *fp = fopen(fileName, "r");
	if(fp == NULL || ntl == NULL)
		return;
	char word[30];
	int i =0;
	setNode *sn = NULL;
	i = fscanf(fp,"%s",word);
	while(i != -1){
		//read ===>
		i = fscanf(fp,"%s",word);
		// read first token
		i = fscanf(fp,"%s",word);
		ntl->followHead = createSetNode(word);
		sn = ntl->followHead;
		// read next all
		i = fscanf(fp,"%s",word);
		while(word[0] != '#'){
			sn->next = createSetNode(word);
			sn = sn->next;
			i = fscanf(fp,"%s",word);
		}
		ntl = ntl->next;
		i = fscanf(fp,"%s",word);
	}
	fclose(fp);
}

void printFollowSet(nonTerminalList * ntlHead){
	nonTerminalList * ntl = ntlHead;
	setNode *sn = NULL; 
	while(ntl != NULL){
		printf("%s --> ", ntl->value);
		printSet(ntl->followHead);
		printf("\n");
		ntl = ntl->next;
	}
}

void printSet( setNode * sn){
	setNode * tmp = sn;
	while(sn != NULL){
		printf("%s ",sn->value);
		sn = sn->next;
	}
}


int getTokenIndex(char * tokenValue){
	if (strcmp("MAIN", tokenValue) == 0)
		return 0;
	else if (strcmp("END", tokenValue) == 0)
		return 1;
	else if (strcmp("SQO", tokenValue) == 0)
		return 2;
	else if (strcmp("SQC", tokenValue) == 0)
		return 3;
	else if (strcmp("OP", tokenValue) == 0)
		return 4;
	else if (strcmp("CL", tokenValue) == 0)
		return 5;
	else if (strcmp("SEMICOLON", tokenValue) == 0)
		return 6;
	else if (strcmp("COMMA", tokenValue) == 0)
		return 7;
	else if (strcmp("FUNCTION", tokenValue) == 0)
		return 8;
	else if (strcmp("FUNID", tokenValue) == 0)
		return 9;
	else if (strcmp("ID", tokenValue) == 0)
		return 10;
	else if (strcmp("NUM", tokenValue) == 0)
		return 11;
	else if (strcmp("RNUM", tokenValue) == 0)
		return 12;
	else if (strcmp("STR", tokenValue) == 0)
		return 13;
	else if (strcmp("INT", tokenValue) == 0)
		return 14;
	else if (strcmp("REAL", tokenValue) == 0)
		return 15;
	else if (strcmp("STRING", tokenValue) == 0)
		return 16;
	else if (strcmp("MATRIX", tokenValue) == 0)
		return 17;
	else if (strcmp("ASSIGNOP", tokenValue) == 0)
		return 18;
	else if (strcmp("IF", tokenValue) == 0)
		return 19;
	else if (strcmp("ELSE", tokenValue) == 0)
		return 20;
	else if (strcmp("ENDIF", tokenValue) == 0)
		return 21;
	else if (strcmp("READ", tokenValue) == 0)
		return 22;
	else if (strcmp("PRINT", tokenValue) == 0)
		return 23;
	else if (strcmp("PLUS", tokenValue) == 0)
		return 24;
	else if (strcmp("MINUS", tokenValue) == 0)
		return 25;
	else if (strcmp("DIV", tokenValue) == 0)
		return 26;
	else if (strcmp("MUL", tokenValue) == 0)
		return 27;
	else if (strcmp("SIZE", tokenValue) == 0)
		return 28;
	else if (strcmp("AND", tokenValue) == 0)
		return 29;
	else if (strcmp("OR", tokenValue) == 0)
		return 30;
	else if (strcmp("NOT", tokenValue) == 0)
		return 31;
	else if (strcmp("LT", tokenValue) == 0)
		return 32;
	else if (strcmp("LE", tokenValue) == 0)
		return 33;
	else if (strcmp("EQ", tokenValue) == 0)
		return 34;
	else if (strcmp("GT", tokenValue) == 0)
		return 35;
	else if (strcmp("GE", tokenValue) == 0)
		return 36;
	else if (strcmp("NE", tokenValue) == 0)
		return 37;
	else if (strcmp("EOF", tokenValue) == 0)
		return 38;
	else if (strcmp("EPSILON", tokenValue) == 0)
		return 39;
	return -1;
}

int getNTLIndex(char * ntlValue){
	if( strcmp(ntlValue,"mainFunction") == 0) 
		return 0;
	else if( strcmp(ntlValue,"stmtsAndFunctionDefs") == 0) 
		return 1;
	else if( strcmp(ntlValue,"snfdFactors") == 0) 
		return 2;
	else if( strcmp(ntlValue,"stmtOrFunctionDef") == 0) 
		return 3;
	else if( strcmp(ntlValue,"stmt") == 0) 
		return 4;
	else if( strcmp(ntlValue,"functionDef") == 0) 
		return 5;
	else if( strcmp(ntlValue,"parameter_list") == 0) 
		return 6;
	else if( strcmp(ntlValue,"type") == 0) 
		return 7;
	else if( strcmp(ntlValue,"remainingList") == 0) 
		return 8;
	else if( strcmp(ntlValue,"declarationStmt") == 0) 
		return 9;
	else if( strcmp(ntlValue,"var_list") == 0) 
		return 10;
	else if( strcmp(ntlValue,"more_ids") == 0) 
		return 11;
	else if( strcmp(ntlValue,"assignmentStmt_type1") == 0) 
		return 12;
	else if( strcmp(ntlValue,"assignmentStmt_type2") == 0) 
		return 13;
	else if( strcmp(ntlValue,"leftHandSide_singleVar") == 0) 
		return 14;
	else if( strcmp(ntlValue,"leftHandSide_listVar") == 0) 
		return 15;
	else if( strcmp(ntlValue,"rightHandSide_type1") == 0) 
		return 16;
	else if( strcmp(ntlValue,"rightHandSide_type2") == 0) 
		return 17;
	else if( strcmp(ntlValue,"sizeExpression") == 0) 
		return 18;
	else if( strcmp(ntlValue,"ifStmt") == 0) 
		return 19;
	else if( strcmp(ntlValue,"elseStmts") == 0) 
		return 20;
	else if( strcmp(ntlValue,"otherStmts") == 0) 
		return 21;
	else if( strcmp(ntlValue,"ioStmt") == 0) 
		return 22;
	else if( strcmp(ntlValue,"funCallStmt") == 0) 
		return 23;
	else if( strcmp(ntlValue,"inputParameterList") == 0) 
		return 24;
	else if( strcmp(ntlValue,"listVar") == 0) 
		return 25;
	else if( strcmp(ntlValue,"arithmeticExpression") == 0) 
		return 26;
	else if( strcmp(ntlValue,"arithmeticFactor") == 0) 
		return 27;
	else if( strcmp(ntlValue,"arithmeticTerm") == 0) 
		return 28;
	else if( strcmp(ntlValue,"arithmeticTermFactor") == 0) 
		return 29;
	else if( strcmp(ntlValue,"factor") == 0) 
		return 30;
	else if( strcmp(ntlValue,"operator_lowPrecedence") == 0) 
		return 31;
	else if( strcmp(ntlValue,"operator_highPrecedence") == 0) 
		return 32;
	else if( strcmp(ntlValue,"booleanExpression") == 0) 
		return 33;
	else if( strcmp(ntlValue,"constrainedVars") == 0) 
		return 34;
	else if( strcmp(ntlValue,"var") == 0) 
		return 35;
	else if( strcmp(ntlValue,"matrix") == 0) 
		return 36;
	else if( strcmp(ntlValue,"rows") == 0) 
		return 37;
	else if( strcmp(ntlValue,"rowsFactor") == 0) 
		return 38;
	else if( strcmp(ntlValue,"row") == 0) 
		return 39;
	else if( strcmp(ntlValue,"remainingColElements") == 0) 
		return 40;
	else if( strcmp(ntlValue,"matrixElement") == 0) 
		return 41;
	else if( strcmp(ntlValue,"logicalOp") == 0) 
		return 42;
	else if( strcmp(ntlValue,"relationalOp") == 0) 
		return 43;
	return -1;
}

void initParseTable(){
	int i,j;
	for(i = 0; i< LENGTH_NTL; i++){
		for(j = 0; j< LENGTH_TOKENS; j++){
			pt[i][j].ruleHead = NULL;
		}
	}
}


void generateParseTable(nonTerminalList * ntlHead){
	initParseTable();
	nonTerminalList *ntl = ntlHead;
	ruleList * rl = NULL;
	setNode * sn = NULL;
	setNode * en = NULL;
	int i,j,k;
	while(ntl != NULL){
		i = getNTLIndex(ntl->value);
		rl = ntl->ruleListHead;
		while(rl != NULL) {
			sn = rl->firstHead;
			while(sn != NULL){
				j = getTokenIndex(sn->value);
				if(j == LENGTH_TOKENS){
					en = ntl->followHead;
					while(en!= NULL){
						k = getTokenIndex(en->value);
						pt[i][k].ruleHead = rl->ruleHead;
						en = en->next;
					}
				}
				else{
					pt[i][j].ruleHead = rl->ruleHead;
				}
				sn = sn->next;
			}
			rl = rl->next;
		}
		ntl = ntl->next;
	}
}

void printParseTable(){
	int i,j;
	for(i = 0; i<LENGTH_NTL; i++){
		for(j = 0; j<LENGTH_TOKENS; j++){
			printf("%d--%p ",j,pt[i][j].ruleHead);
		}
		printf("\n\n");
	}
}

parseStack * createStackNode(char * value){
	parseStack * psn = (parseStack *) malloc (sizeof(parseStack));
	if(psn == NULL)
		return NULL;
	strcpy(psn->value, value);
	psn->next = NULL;
	return psn;
}

parseStack * psPush(parseStack * psh, char * value){
	if(strcmp(value,"EPSILON") == 0)
		return psh;
	if(psh == NULL)
		psh = createStackNode(value);
	else{
		parseStack *tmp = createStackNode(value);
		tmp->next = psh;
		psh = tmp;
	}
	return psh;
}

parseStack * psPop(parseStack * psh){
	if(psh == NULL)
		return NULL;
	else{
		parseStack *tmp = psh;
		psh = psh->next;
		// printf("Poping: %s\n", tmp->value);
		free(tmp);
		return psh;
	}
}

parseStack * initStack(parseStack *psHead){
	psHead = psPush(psHead, "EOF");
	psHead = psPush(psHead, "mainFunction");
	return psHead;
}

parseTree* createPTNode(char *value, parseTree * parent){
	parseTree *ptn = (parseTree *) malloc (sizeof(parseTree));
	if(ptn == NULL)
		return NULL;
	strcpy(ptn->value, value);
	ptn->ti = NULL;
	ptn->child = NULL;
	ptn->parent = parent;
	ptn->sibling = NULL;
	return ptn;
}

parseTree *initParseTree(parseTree * ptHead){
	ptHead = createPTNode("ROOT", NULL);
	ptHead->child = createPTNode("mainFunction", NULL);
	ptHead->child->parent = ptHead;
	ptHead = ptHead->child;
	return ptHead;
}


void generateParseTree(parseTree * ptHead, nonTerminalList * ntlHead, char * inFileName){
	psHead = initStack(psHead);
	// ptHead = initParseTree(ptHead);
	FILE *infp = fopen(inFileName, "r");

	tokenInfo *ti = NULL;
	ti = getNextToken(infp);
	while(strcmp(ti->token,"COMMENT") == 0 || strcmp(ti->token,"Lexical error")){
		if((ti->token)[0] == 'L'){
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
		ti = getNextToken(infp);
	}
	// printf("%s\n",ti->value );
	int i,j;
	parseTree *cpt = ptHead;
	parseTree *ptn = NULL;
	int errorFlag = 0;

	nonTerminalList * ntl = NULL;
	ruleList * rl = NULL;
	setNode * sn = NULL; 
	setNode * en = NULL;

	while( strcmp(psHead->value,"EOF")  != 0){
		i = getNTLIndex(psHead->value);
		j = getTokenIndex(ti->token);
		// printf("yo %s:%d %s:%d\n",psHead->value,i,ti->token,j);
		if(strcmp(psHead->value,ti->token) == 0){
			cpt->ti = ti;
			// printf("---CPT: %s\n",cpt->ti->token);
			// if(strcmp(cpt->value,ti->token) == 0){
			// 	printf("0 MATCHED: %s\n", ti->token);
			// }
			// else{
			// 	printf("1 MATCHED: %s\n", ti->token);
			// }
			while(cpt != NULL){
				if(cpt->sibling != NULL){
					cpt = cpt->sibling;
					break;
				}
				cpt = cpt->parent;
			}

			while(cpt != NULL && cpt->child != NULL){
				cpt = cpt->child;
			}
			// printf("yo----\n");
			psHead = psPop(psHead);
			ti = getNextToken(infp);
			while(strcmp(ti->token,"COMMENT") == 0 || strcmp(ti->token,"Lexical error") == 0){
				if((ti->token)[0] == 'L'){
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
				ti = getNextToken(infp);
			};
			if(strcmp(ti->token,"EOF") == 0){
				return;
			}
		}
		else if(getTokenIndex(psHead->value) != -1){
			printf("ERROR\n");
			errorFlag = 1;
			break;
		}
		else if(i != -1 && j != -1 && pt[i][j].ruleHead == NULL){
			printf("line no.:%llu Syntax error: The token %s for lexeme %s does not match at line %llu. The expected token here is", ti->lineno, ti->token, ti->value);
			ntl = getNonTerminal(ntlHead, psHead->value);
			rl = ntl->ruleListHead;
			while(rl != NULL) {
				sn = rl->firstHead;
				while(sn != NULL){
					if(strcmp(sn->value,"EPSILON") == 0){
						en = ntl->followHead;
						while(en!= NULL){
							printf(" %s",en->value);
							en = en->next;
						}
					}
					else{
						printf(" %s",en->value);
					}
					sn = sn->next;
				}
				rl = rl->next;
			}
			printf("\n");

			ti = getNextToken(infp);
			while(1){
				while(strcmp(ti->token,"COMMENT") == 0 || strcmp(ti->token,"Lexical error") == 0){
					if((ti->token)[0] == 'L'){
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
					ti = getNextToken(infp);
				};
				if(strcmp(ti->token,"EOF") == 0){
					return;
				}

				en = ntl->followHead;
				while(en!=NULL){
					if(strcmp(en->value, ti->token) == 0){
						//fix cpt
						psHead = psPop(psHead);
						while( cpt->sibling == NULL ){
							if(cpt->parent != NULL){
								cpt = cpt->parent;
							}
							else{
								return;
							}
						}
						cpt = cpt->sibling;
						break;
					}
					en = en->next;
				}
				ti = getNextToken(infp);
			}

			errorFlag = 1;
			break;
		}
		else if(i != -1 && j != -1 && pt[i][j].ruleHead != NULL){
			// printf("get rule:%s %s\n",psHead->value, ti->token);
			ruleNode *rn = pt[i][j].ruleHead;
			if(strcmp(rn->value,"EPSILON") == 0){
				// printf("EPison rule\n");
				cpt->child = createPTNode(rn->value,cpt);
				while(cpt != NULL){
					if(cpt->sibling != NULL){
						cpt = cpt->sibling;
						break;
					}
					cpt = cpt->parent;
				}
				while(cpt->child != NULL){
					cpt = cpt->child;
				}
				// printf("%s----edn epi\n",cpt->value);
			}
			else {
				while(rn != NULL){
					// printf("%s-",rn->value);
					if(cpt->child == NULL){
						cpt->child = createPTNode(rn->value,cpt);
						// cpt = cpt->child;
						ptn = cpt->child;
					}
					else{
						ptn->sibling = createPTNode(rn->value,cpt);
						ptn = ptn->sibling;
					}
					dummyStack = psPush(dummyStack, rn->value);
					rn = rn->next;
				}
				cpt = cpt->child;
			}
			psHead = psPop(psHead);
			while(dummyStack != NULL){
				psHead = psPush(psHead, dummyStack->value);
				dummyStack = psPop(dummyStack);
			}
			// printStack(psHead);
		}
	};
	if(errorFlag == 0){
		printf("Input source code is syntactically correct...........\n");
	}
	fclose(infp);
}

void printStack(parseStack * psh){
	while(psh != NULL){
		printf("%s-", psh->value);
		psh = psh->next;
	}
	printf("\n");
}

void printParseTree(FILE *fp, parseTree * pt){
	if(pt != NULL){
		char isLeaf[4] = "yes";
		if(pt->child != NULL){
			strcpy(isLeaf,"no");
			printParseTree(fp,pt->child);
		}

		//printDetails
		if(pt->ti != NULL){
			if(strcmp(pt->ti->token,"RNUM") == 0 || strcmp(pt->ti->token,"NUM") == 0){
				fprintf(fp,"%s %d %s %s %s %s %s\n", pt->ti->value, pt->ti->lineno, pt->ti->token, pt->ti->value, pt->parent->value, isLeaf, pt->value);
			}
			else{
				fprintf(fp,"%s %d %s %s %s %s %s\n", pt->ti->value, pt->ti->lineno, pt->ti->token, "----", pt->parent->value, isLeaf, pt->value);
			}
		}
		else{
			if(strcmp(pt->value, "EPSILON")==0){
				fprintf(fp,"%s %s %s %s %s %s %s\n", "----","----", pt->value, "----", pt->parent->value, isLeaf, pt->value );
			}
			else{
				fprintf(fp,"%s %s %s %s %s %s %s\n", "----", "----", "----","----", pt->parent->value, isLeaf, pt->value);
			}
		}

		if(pt->child != NULL){
			parseTree *tmp = pt->child->sibling; 
			while(tmp != NULL){
				printParseTree(fp,tmp);
				tmp = tmp->sibling;
			}
		}
	}
};
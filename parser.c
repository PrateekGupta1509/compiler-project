#include "parser.h"

int main(){
	// printf("yo\n");
	nonTerminalList * ntl = readGrammar("grammer.txt");
	// printGrammar(ntl);
	computeFirstSet(ntl);
	printFirstSet(ntl);
	return 0;
	// ask main ke end ke baad semicolon
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

nonTerminalList * createNTNode(char * nonTerminal){
	nonTerminalList * ntl = (nonTerminalList *) malloc (sizeof(nonTerminalList));
	if(ntl == NULL)
		return NULL;
	strcpy(ntl->value, stripNonTerminal(nonTerminal));
	// ntl->value = stripNonTerminal(nonTerminal) ;
	ntl->ruleListHead = NULL; 
	// ntl->next = NULL; 
	return ntl;
}

ruleList* createRule(ruleNode * rn){
	ruleList *rlh = (ruleList *) malloc (sizeof(ruleList));
	if(rlh == NULL)
		return NULL;
	rlh->ruleHead = rn;
	rlh->firstHead = NULL;
	rlh->followHead = NULL;
	rlh->next = NULL;
	return rlh; 
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

char * stripNonTerminal(char *value){
	++value;
	value[strlen(value)-1] = '\0';
	return value;
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

setNode * createSetNode(char *value){
	setNode *fis = (setNode *) malloc (sizeof(setNode));
	// printf("--%s\n", value);
	strcpy(fis->value, value);
	fis->next = NULL; 
	return fis;
}

ruleList * getNonTerminal(nonTerminalList *ntlHead, char * value){
	nonTerminalList * ntl = ntlHead;
	while(ntl != NULL){
		if( strcmp(ntl->value, value) == 0){
			return ntl->ruleListHead;
		}
		ntl = ntl->next;
	}
	return NULL;
}

setNode * recurseFirstSet(nonTerminalList* ntlHead, ruleNode * ruleHead){
	if(ruleHead->isTerminal == 1)
		return createSetNode(ruleHead->value);
	else{
		ruleList * rl = getNonTerminal(ntlHead, ruleHead->value);
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

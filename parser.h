#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct setNode {
	char value[25];
	struct setNode *next;
}setNode;

typedef struct ruleNode{
	int isTerminal;
	char value[25];
	struct ruleNode *next;
}ruleNode;

typedef struct ruleList{
	ruleNode * ruleHead;
	setNode * firstHead;
	setNode * followHead;
	struct ruleList * next;
}ruleList;

typedef struct nonTerminalList{
	ruleList * ruleListHead;
	char value[25];
	struct nonTerminalList *next;
}nonTerminalList;

nonTerminalList* readGrammar (char * fileName);
nonTerminalList * createNTNode(char * nonTerminal);
ruleList* createRule(ruleNode * rn);
ruleNode * createRuleNode(char * value);
char * stripNonTerminal(char *value);
void printGrammar(nonTerminalList *ntlHead);

void computeFirstSet(nonTerminalList *ntlHead);
setNode * createSetNode(char *value);
ruleList * getNonTerminal(nonTerminalList *ntlHead, char * value);
setNode * recurseFirstSet(nonTerminalList* ntlHead, ruleNode * ruleHead);
void printFirstSet(nonTerminalList * ntlHead);

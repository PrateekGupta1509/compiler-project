// PRATEEK GUPTA
// 2014B4A70767P

#ifndef PARSERDEF
#define PARSERDEF
#include "lexer.h"

#define LENGTH_NTL 43
#define LENGTH_TOKENS 39

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
	struct ruleList * next;
}ruleList;

typedef struct nonTerminalList{
	ruleList * ruleListHead;
	setNode * followHead;
	char value[25];
	struct nonTerminalList *next;
}nonTerminalList;

typedef struct parseTable{
	ruleNode * ruleHead;
}parseTable;


typedef struct parseStack{
	char value[30];
	struct parseStack * next;
}parseStack;

typedef struct parseTree{
	char value[25];
	tokenInfo * ti;
	struct parseTree* parent;
	struct parseTree* sibling;
	struct parseTree* child;
}parseTree;

#endif

// PRATEEK GUPTA
// 2014B4A70767P

#include "lexer.h"
#include "parserDef.h"

nonTerminalList* readGrammar (char * fileName);
nonTerminalList * createNTNode(char * nonTerminal);
ruleList* createRule(ruleNode * rn);
ruleNode * createRuleNode(char * value);
char * stripNonTerminal(char *value);
void printGrammar(nonTerminalList *ntlHead);

nonTerminalList * getNonTerminal(nonTerminalList *ntlHead, char * value);

void computeFirstSet(nonTerminalList *ntlHead);
setNode * createSetNode(char *value);
setNode * recurseFirstSet(nonTerminalList* ntlHead, ruleNode * ruleHead);
void printFirstSet(nonTerminalList * ntlHead);


void readFollowSet(char *fileName, nonTerminalList * ntlHead);

// void computeFollowSet(nonTerminalList * ntlHead);
// void appendFollowSet(nonTerminalList * ntl, setNode * followSet);
// void recurseFollowSet(nonTerminalList *ntlHead, nonTerminalList *fntl, nonTerminalList * lhsntl, ruleNode * nrn);

void printFollowSet(nonTerminalList * ntlHead);
void printSet( setNode * sn);


int getTokenIndex(char * tokenValue);
int getNTLIndex(char * ntlValue);

void initParseTable();
void generateParseTable(nonTerminalList * ntlHead);
void printParseTable();

parseStack * createStackNode(char * value);
parseStack * psPush(parseStack * psh ,char * value);
parseStack * psPop(parseStack * psh);
parseStack * initStack(parseStack *psHead);
void printStack(parseStack * psh);

parseTree * initParseTree();
parseTree* createPTNode(char *value, parseTree * parent);


void generateParseTree(parseTree * ptHead, char * inFileName);
void printParseTree(parseTree * pt);

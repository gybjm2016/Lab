#include <stdio.h>
#include "lex.yy.c"
#include"semantics.h"
//#include "tree.h"
extern FILE* yyin;
extern struct node* root;
extern void printtree(struct node *p,int height);
int main(int argc,char** argv){
	if(argc<=1)
		return 1;
	FILE* f=fopen(argv[1],"r");
	if(!f){
		perror(argv[1]);
		return 1;
	}
	yyrestart(f);
	yyparse();
	if(root!=NULL){
		printtree(root,0);
		semantic_analysis(root);
		pronotdef();
	}
	return 0;
}

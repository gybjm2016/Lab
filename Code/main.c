#include <stdio.h>
#include "lex.yy.c"
#include"semantics.h"
#include"translate.h"
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
		printf("fenxijieshu\n");
		code* temp=translate(root);
		printf("fanyijieshu\n");
		printcode(temp,"hhhh.ir");
		printf("zhongjiandaimajieshu\n");
		mipscode(temp,"result.s");
	}
	return 0;
}

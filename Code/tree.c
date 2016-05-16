#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include "tree.h"
//#include "syntax.tab.h"
/*
struct node{
	char type[30];
	int linenum;
	int childnum;
	struct node *child[10];
};*/
int iserror=0;
typedef struct node nodei;
void addnode(struct node* father,struct node *child){
	father->child[father->childnum]=child;
	father->childnum++;
}
struct node* setnode(char *type,char *text,int linum){
	struct node *temp=(struct node *)malloc(sizeof(node));
  strcpy(temp->type,type);
	strcpy(temp->text,text);
  temp->linenum=linum;
	temp->childnum=0;
  return temp;
}
void printtree(struct node *p,int height){
	if(p==NULL||iserror==1)
		return;
	if(strcmp(p->type,"")!=0)
		for(int i=0;i<height;i++)
			printf(" ");
	if(strcmp(p->type,"TYPE")==0)
		printf("%s: %s\n",p->type,p->text);
	else if(strcmp(p->type,"INT")==0)
		printf("%s: %d\n",p->type,myatoi(p->text));
	else if(strcmp(p->type,"ID")==0)
		printf("%s: %s\n",p->type,p->text);
	else if(strcmp(p->type,"FLOAT")==0)
		printf("%s: %s\n",p->type,p->text);
	else if(strcmp(p->type,"")!=0)
		printf("%s (%d)\n",p->type,p->linenum);
	for(int i=0;i<p->childnum;i++)
		printtree(p->child[i],height+1);
}
int myatoi(char* p){
	if(strlen(p)==1)
		return atoi(p);
	else if(p[0]=='0'){
		if(p[1]=='x'||p[1]=='X')
			return (int)strtoul(p+2,0,16);
		else
			return (int)strtoul(p+1,0,8);
	}
	else
		return atoi(p);
}

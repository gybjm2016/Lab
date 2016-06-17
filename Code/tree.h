#ifndef TREE_H_
#define TREE_H_
struct node{
	char type[30];
	char text[30];
	int linenum;
	int childnum;
	struct node *child[10];
};
extern int iserror;
typedef struct node node;
void addnode(struct node* father,struct node* child);
struct node* setnode(char *type,char *text,int linum);
void printtree(struct node *p,int height);
int myatoi(char* p);
#endif

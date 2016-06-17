#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"tree.h"
typedef struct Type_* Type;
typedef struct FieldList_* FieldList;
struct Type_{
	enum { BASIC,ARRAY,STRUCTURE,FUNC } kind;
	union{
		char* basic;
		struct { 
			Type elem;
			int size;
		}array;
	FieldList structure;
	FieldList function;
	} u;
};
struct FieldList_{
	char* name;
	Type type;
	FieldList tail;
};
struct charatable{
	char* name;
	Type charatype;
	int  ispara;
	struct charatable* next;
};
struct dectable{
	int isdef;
	int lnum;
	Type funcdeclare;
	struct dectable* next;
};
struct charatable *head;
struct charatable* search(char* nam);

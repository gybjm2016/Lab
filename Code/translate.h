#include <stdio.h>
#include"tree.h"
typedef struct Operand_* Operand;
struct Operand_{
	enum { VARIABLE, CONSTANT, ADDRESS,TEMPVA,LABEL} kind;
	union {
		char* var_name;
		int value;
		int address;
		int no;
		int labno;
	} u;
	struct Operand_ *next;
};
struct InterCode
{
	enum {ASSIGN, ADD, MIN, MUL, DIVID,LAB,RET,GOTO,IFGOTO,READ,WRITE,ARG,CALL,FUNCTION,DECSP,PARAM,ADDRTO,GETADD,TOADD} kind;
	union {
		struct { Operand right,left;} assign;//call dec addrto getadd toadd
		struct { Operand result,op1,op2 } calcu;
		struct { Operand op1} lab;
		struct { Operand op1} ret;//read write arg function param 
		struct { Operand op1} got;
		struct { Operand op1,op2,op3,op4} ifgoto;
	} u;
};
struct code{
	struct InterCode word;
	struct code* next;
	struct code* pre;
};
typedef struct code code;

code* translate_FunDec(struct node * root);
code* translate_Cond(struct node* h,Operand label_true,Operand label_false);
code* translate_Paramdec(struct node *h);
code* translate_Varlist(struct node *h);
code* translate_Vardec(struct node *h);
code* translate_Deflist(struct node *h);
code* translate_Args(struct node *h,Operand arg_list);
code* translate_Compst(struct node *h);
code* translate(struct node* h);
code* translate_Extdeflist(struct node *h);
code* translate_Extdef(struct node *h);
code* translate_Fundec(struct node *h);

%{	
#include <stdio.h>
//#include "lex.yy.c"
#include "tree.h"
#include "lexicial.h"
#define YYSTYPE struct node*
struct node *root=NULL;
int testno = 1;
%}
%token INT FLOAT TYPE ID LC RC
%right ASSIGNOP
%left OR 
%left AND 
%left RELOP 
%left PLUS MINUS 
%left STAR DIV 
%right NOT
%left LP RP LB RB DOT
%nonassoc LOWER_THAN_ELSE
%nonassoc STRUCT RETURN IF ELSE WHILE COMMA SEMI

%%
Program 	: ExtDefList			{$$ = setnode("Program","",@$.first_line); addnode($$,$1);root=$$;};
ExtDefList 	: ExtDef ExtDefList		{$$ = setnode("ExtDefList","",@$.first_line);addnode($$,$1);addnode($$,$2);}
		|				{$$ = setnode("","",@$.first_line);};
ExtDef 		: Specifier ExtDecList SEMI	{$$ = setnode("ExtDef","",@$.first_line);addnode($$,$1);addnode($$,$2);addnode($$,$3);}
		| Specifier SEMI		{$$ = setnode("ExtDef","",@$.first_line);addnode($$,$1);addnode($$,$2);}
		| Specifier FunDec SEMI		{$$ = setnode("ExtDef","",@$.first_line);addnode($$,$1);addnode($$,$2);addnode($$,$3);}
		| Specifier FunDec CompSt	{$$ = setnode("ExtDef","",@$.first_line);addnode($$,$1);addnode($$,$2);addnode($$,$3);}
		| Specifier ExtDecList error	{iserror=1;printf("Error type B at line %d :Missing ;\n",@$.first_line);}
		| Specifier error		{iserror=1;printf("Error type B at line %d :Missing ;\n",@$.first_line);};
ExtDecList	: VarDec			{$$ = setnode("ExtDecList","",@$.first_line);addnode($$,$1);}
		| VarDec COMMA ExtDecList	{$$ = setnode("ExtDecList","",@$.first_line);addnode($$,$1);addnode($$,$2);addnode($$,$3);}
		| VarDec error ExtDecList	{iserror=1;printf("Error type B at line %d :cccccccccMissing ,\n",@$.first_line);};
Specifier	: TYPE				{$$ = setnode("Specifier","",@$.first_line);addnode($$,$1);}
		| StructSpecifier		{$$ = setnode("Specifier","",@$.first_line);addnode($$,$1);};
StructSpecifier	: STRUCT OptTag LC DefList RC	{$$ = setnode("StructSpecifier","",@$.first_line);addnode($$,$1);addnode($$,$2);addnode($$,$3);addnode($$,$4);addnode($$,$5);}
		| STRUCT Tag			{$$ = setnode("StructSpecifier","",@$.first_line);addnode($$,$1);addnode($$,$2);}
		| STRUCT OptTag LC DefList error{iserror=1;printf("Error type B at line %d :Missing }\n",@$.first_line);};
OptTag		: ID				{$$ = setnode("OptTag","",@$.first_line);addnode($$,$1);}
		|				{$$ = setnode("","",@$.first_line);};
Tag		: ID				{$$ = setnode("Tag","",@$.first_line);addnode($$,$1);};
VarDec		: ID				{$$ = setnode("VarDec","",@$.first_line);addnode($$,$1);}
		| VarDec LB INT RB		{$$ = setnode("VarDec","",@$.first_line);addnode($$,$1);addnode($$,$2);addnode($$,$3);addnode($$,$4);}
		| VarDec LB INT error		{iserror=1;printf("Error type B at line %d :Missing ]\n",@$.first_line);};
FunDec		: ID LP VarList RP		{$$ = setnode("FunDec","",@$.first_line);addnode($$,$1);addnode($$,$2);addnode($$,$3);addnode($$,$4);}
		| ID LP RP			{$$ = setnode("FunDec","",@$.first_line);addnode($$,$1);addnode($$,$2);addnode($$,$3);}
		| ID LP VarList error		{iserror=1;printf("Error type B at line %d :Missing )\n",$1->linenum);}
		| ID LP error			{iserror=1;printf("Error type B at line %d :Missing )\n",$2->linenum);};
VarList		: ParamDec COMMA VarList	{$$ = setnode("VarList","",@$.first_line);addnode($$,$1);addnode($$,$2);addnode($$,$3);}
		| ParamDec			{$$ = setnode("VarList","",@$.first_line);addnode($$,$1);}
		| ParamDec error VarList	{iserror=1;printf("Error type B at line %d :bbbbbbbbMissing ,\n",@$.first_line);};
ParamDec	: Specifier VarDec		{$$ = setnode("ParamDec","",@$.first_line);addnode($$,$1);addnode($$,$2);};
CompSt		: LC DefList StmtList RC	{$$ = setnode("CompSt","",@$.first_line);addnode($$,$1);addnode($$,$2);addnode($$,$3);addnode($$,$4);}
		| LC DefList StmtList error	{iserror=1;printf("Error type B at line %d :Missing }\n",@$.first_line);};
StmtList	: Stmt StmtList			{$$ = setnode("StmtList","",@$.first_line);addnode($$,$1);addnode($$,$2);}
		|				{$$ = setnode("","",@$.first_line);};
Stmt		: Exp SEMI			{$$ = setnode("Stmt","",@$.first_line);addnode($$,$1);addnode($$,$2);}
		| CompSt			{$$ = setnode("Stmt","",@$.first_line);addnode($$,$1);}
		| RETURN Exp SEMI		{$$ = setnode("Stmt","",@$.first_line);addnode($$,$1);addnode($$,$2);addnode($$,$3);}
		| IF LP Exp RP Stmt %prec LOWER_THAN_ELSE		{$$ = setnode("Stmt","",@$.first_line);addnode($$,$1);addnode($$,$2);addnode($$,$3);addnode($$,$4);addnode($$,$5);}
		| IF LP Exp  RP Stmt ELSE Stmt	{$$ = setnode("Stmt","",@$.first_line);addnode($$,$1);addnode($$,$2);addnode($$,$3);addnode($$,$4);addnode($$,$5);addnode($$,$6);addnode($$,$7); }
		| WHILE LP Exp  RP Stmt		{$$ = setnode("Stmt","",@$.first_line);addnode($$,$1);addnode($$,$2);addnode($$,$3);addnode($$,$4);addnode($$,$5);}
		| IF LP Exp error Stmt		{iserror=1;printf("Error type B at line %d :Missing )\n",$1->linenum);}
		| IF LP Exp error Stmt ELSE Stmt{iserror=1;printf("Error type B at line %d :Missing )\n",$1->linenum);}
		| WHILE LP Exp error Stmt	{iserror=1;printf("Error type B at line %d :Missing )\n",$1->linenum);}
		| Exp error			{iserror=1;printf("Error type B at line %d :Missing ;\n",@$.first_line);}
		| RETURN Exp error              {iserror=1;printf("Error type B at line %d :Missing ;\n",@$.first_line);};
DefList		: Def DefList			{$$ = setnode("DefList","",@$.first_line);addnode($$,$1);addnode($$,$2);}
		|				{$$ = setnode("","",@$.first_line);};
Def 		: Specifier DecList  SEMI	{$$ = setnode("Def","",@$.first_line);addnode($$,$1);addnode($$,$2);addnode($$,$3);}
		| Specifier DecList error	{iserror=1;printf("Error type B at line %d :Missing ;\n",@$.first_line);};
DecList		: Dec				{$$ = setnode("DecList","",@$.first_line);addnode($$,$1);}
		| Dec COMMA DecList		{$$ = setnode("DecList","",@$.first_line);addnode($$,$1);addnode($$,$2);addnode($$,$3);}
		| Dec error DecList		{iserror=1;printf("Error type B at line %d :aaaaaMissing ,\n",@$.first_line);};
Dec 		: VarDec			{$$ = setnode("Dec","",@$.first_line);addnode($$,$1);}
  		| VarDec ASSIGNOP Exp		{$$ = setnode("Dec","",@$.first_line);addnode($$,$1);addnode($$,$2);addnode($$,$3);};
Exp		: Exp ASSIGNOP Exp		{$$ = setnode("Exp","",@$.first_line);addnode($$,$1);addnode($$,$2);addnode($$,$3);}
		| Exp AND Exp			{$$ = setnode("Exp","",@$.first_line);addnode($$,$1);addnode($$,$2);addnode($$,$3);}
		| Exp OR Exp			{$$ = setnode("Exp","",@$.first_line);addnode($$,$1);addnode($$,$2);addnode($$,$3);}
		| Exp RELOP Exp			{$$ = setnode("Exp","",@$.first_line);addnode($$,$1);addnode($$,$2);addnode($$,$3);}
		| Exp PLUS Exp			{$$ = setnode("Exp","",@$.first_line);addnode($$,$1);addnode($$,$2);addnode($$,$3);}
		| Exp MINUS Exp			{$$ = setnode("Exp","",@$.first_line);addnode($$,$1);addnode($$,$2);addnode($$,$3);printf("- testno = %d\n",testno);testno++;}
		| Exp STAR Exp			{$$ = setnode("Exp","",@$.first_line);addnode($$,$1);addnode($$,$2);addnode($$,$3);}
		| Exp DIV Exp			{$$ = setnode("Exp","",@$.first_line);addnode($$,$1);addnode($$,$2);addnode($$,$3);}
		| LP Exp RP			{$$ = setnode("Exp","",@$.first_line);addnode($$,$1);addnode($$,$2);addnode($$,$3);}
		| MINUS Exp			{$$ = setnode("Exp","",@$.first_line);addnode($$,$1);addnode($$,$2);}
		| NOT Exp			{$$ = setnode("Exp","",@$.first_line);addnode($$,$1);addnode($$,$2);}
		| ID LP Args RP			{$$ = setnode("Exp","",@$.first_line);addnode($$,$1);addnode($$,$2);addnode($$,$3);addnode($$,$4);}
		| ID LP RP			{$$ = setnode("Exp","",@$.first_line);addnode($$,$1);addnode($$,$2);addnode($$,$3);}
		| LP Exp error                  {iserror=1;printf("Error type B at line %d :Missing )\n",$1->linenum);}  
		| ID LP Args error 		{iserror=1;printf("Error type B at line %d :Missing )\n",$2->linenum);}
		| ID LP error 			{iserror=1;printf("Error type B at line %d :Missing )\n",$2->linenum);}
		| Exp LB Exp RB			{$$ = setnode("Exp","",@$.first_line);addnode($$,$1);addnode($$,$2);addnode($$,$3);addnode($$,$4);printf("array testno = %d\n",testno);testno++;}
		| Exp DOT ID			{$$ = setnode("Exp","",@$.first_line);addnode($$,$1);addnode($$,$2);addnode($$,$3);}
		| ID				{$$ = setnode("Exp","",@$.first_line);addnode($$,$1);printf("id testno = %d\n",testno);testno++;}
		| INT				{$$ = setnode("Exp","",@$.first_line);addnode($$,$1);printf("int testno = %d\n",testno);testno++;}
		| FLOAT				{$$ = setnode("Exp","",@$.first_line);addnode($$,$1);}
		| Exp LB Exp error		{iserror=1;printf("Error type B at line %d :Missing ]\n",$2->linenum);};
Args		: Exp COMMA Args		{$$ = setnode("Args","",@$.first_line);addnode($$,$1);addnode($$,$2);addnode($$,$3);}
		| Exp				{$$ = setnode("Args","",@$.first_line);addnode($$,$1);};

%%

/*
#include "lex.yy.c"
int main(){
	yyparse();
}*/

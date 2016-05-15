%{	
#include <stdio.h>
//#include "lex.yy.c"
#include "tree.h"
#include "lexicial.h"
#define YYSTYPE struct node*
struct node *root=NULL;
%}
%token INT FLOAT TYPE ID SEMI COMMA
%right ASSIGNOP NOT
%left PLUS MINUS STAR DIV LP RP AND OR LB RB LC RC DOT RELOP
%{/*%type <node>Program ExtDefList ExtDef ExtDecList
%type <node>Specifier StructSpecifier OptTag Tag
%type <node>VarDec FunDec VarList ParamDec
%type <node>CompSt StmtList Stmt
%type <node>DefList Def DecList Dec
%type <node>Exp Args*/
%}
%nonassoc LOWER_THAN_ELSE
%nonassoc STRUCT RETURN IF ELSE WHILE

%%
Program 	: ExtDefList			{$$ = setnode("Program","",@$.first_line); addnode($$,$1);root=$$;};
ExtDefList 	: ExtDef ExtDefList		{$$ = setnode("ExtDefList","",@$.first_line);addnode($$,$1);addnode($$,$2);}
		|				{$$ = setnode("","",@$.first_line);};
ExtDef 		: Specifier ExtDecList SEMI	{$$ = setnode("ExtDef","",@$.first_line);addnode($$,$1);addnode($$,$2);addnode($$,$3);}
		| Specifier SEMI		{$$ = setnode("ExtDef","",@$.first_line);addnode($$,$1);addnode($$,$2);}
		| Specifier FunDec SEMI		{$$ = setnode("ExtDef","",@$.first_line);addnode($$,$1);addnode($$,$2);addnode($$,$3);}
		| Specifier FunDec CompSt	{$$ = setnode("ExtDef","",@$.first_line);addnode($$,$1);addnode($$,$2);addnode($$,$3);}
		| Specifier ExtDecList error	{iserror=1;printf("Error type B at line %d :Missing ;\n",yylineno);}
		| Specifier error		{iserror=1;printf("Error type B at line %d :Missing ;\n",yylineno);};
ExtDecList	: VarDec			{$$ = setnode("ExtDecList","",@$.first_line);addnode($$,$1);}
		| VarDec COMMA ExtDecList	{$$ = setnode("ExtDecList","",@$.first_line);addnode($$,$1);addnode($$,$2);addnode($$,$3);}
		| VarDec error ExtDecList	{iserror=1;printf("Error type B at line %d :cccccccccMissing ,\n",yylineno);};
Specifier	: TYPE				{$$ = setnode("Specifier","",@$.first_line);addnode($$,$1);}
		| StructSpecifier		{$$ = setnode("Specifier","",@$.first_line);addnode($$,$1);};
StructSpecifier	: STRUCT OptTag LC DefList RC	{$$ = setnode("StructSpecifier","",@$.first_line);addnode($$,$1);addnode($$,$2);addnode($$,$3);addnode($$,$4);addnode($$,$5);}
		| STRUCT Tag			{$$ = setnode("StructSpecifier","",@$.first_line);addnode($$,$1);addnode($$,$2);}
		| STRUCT OptTag LC DefList error{iserror=1;printf("Error type B at line %d :Missing }\n",yylineno);};
OptTag		: ID				{$$ = setnode("OptTag","",@$.first_line);addnode($$,$1);}
		|				{$$ = setnode("","",@$.first_line);};
Tag		: ID				{$$ = setnode("Tag","",@$.first_line);addnode($$,$1);};
VarDec		: ID				{$$ = setnode("VarDec","",@$.first_line);addnode($$,$1);}
		| VarDec LB INT RB		{$$ = setnode("VarDec","",@$.first_line);addnode($$,$1);addnode($$,$2);addnode($$,$3);addnode($$,$4);}
		| VarDec LB INT error		{iserror=1;printf("Error type B at line %d :Missing ]\n",yylineno);};
FunDec		: ID LP VarList RP		{$$ = setnode("FunDec","",@$.first_line);addnode($$,$1);addnode($$,$2);addnode($$,$3);addnode($$,$4);}
		| ID LP RP			{$$ = setnode("FunDec","",@$.first_line);addnode($$,$1);addnode($$,$2);addnode($$,$3);}
		| ID LP VarList error		{iserror=1;printf("Error type B at line %d :Missing )\n",$1->linenum);}
		| ID LP error			{iserror=1;printf("Error type B at line %d :Missing )\n",$2->linenum);};
VarList		: ParamDec COMMA VarList	{$$ = setnode("VarList","",@$.first_line);addnode($$,$1);addnode($$,$2);addnode($$,$3);}
		| ParamDec			{$$ = setnode("VarList","",@$.first_line);addnode($$,$1);}
		| ParamDec error VarList	{iserror=1;printf("Error type B at line %d :bbbbbbbbMissing ,\n",yylineno);};
ParamDec	: Specifier VarDec		{$$ = setnode("ParamDec","",@$.first_line);addnode($$,$1);addnode($$,$2);};
CompSt		: LC DefList StmtList RC	{$$ = setnode("CompSt","",@$.first_line);addnode($$,$1);addnode($$,$2);addnode($$,$3);addnode($$,$4);}
		| LC DefList StmtList error	{iserror=1;printf("Error type B at line %d :Missing }\n",yylineno);};
StmtList	: Stmt StmtList			{$$ = setnode("StmtList","",@$.first_line);addnode($$,$1);addnode($$,$2);}
		|				{$$ = setnode("","",@$.first_line);};
Stmt		: Exp SEMI			{$$ = setnode("Stmt","",@$.first_line);addnode($$,$1);addnode($$,$2);}
		| CompSt			{$$ = setnode("Stmt","",@$.first_line);addnode($$,$1);}
		| RETURN Exp SEMI		{$$ = setnode("Stmt","",@$.first_line);addnode($$,$1);addnode($$,$2);addnode($$,$3);}
		| IF LP Exp RP Stmt %prec LOWER_THAN_ELSE		{$$ = setnode("Stmt","",@$.first_line);addnode($$,$1);addnode($$,$2);addnode($$,$3);addnode($$,$4);addnode($$,$5);}
		| IF LP Exp  RP Stmt ELSE Stmt	{$$ = setnode("Stmt","",@$.first_line);addnode($$,$1);addnode($$,$2);addnode($$,$3);addnode($$,$4);addnode($$,$5);addnode($$,$6);addnode($$,$7);}
		| WHILE LP Exp  RP Stmt		{$$ = setnode("Stmt","",@$.first_line);addnode($$,$1);addnode($$,$2);addnode($$,$3);addnode($$,$4);addnode($$,$5);}
		| IF LP Exp error Stmt		{iserror=1;printf("Error type B at line %d :Missing )\n",$1->linenum);}
		| IF LP Exp error Stmt ELSE Stmt{iserror=1;printf("Error type B at line %d :Missing )\n",$1->linenum);}
		| WHILE LP Exp error Stmt	{iserror=1;printf("Error type B at line %d :Missing )\n",$1->linenum);}
		| Exp error			{iserror=1;printf("Error type B at line %d :Missing ;\n",yylineno);}
		| RETURN Exp error              {iserror=1;printf("Error type B at line %d :Missing ;\n",yylineno);};
DefList		: Def DefList			{$$ = setnode("DefList","",@$.first_line);addnode($$,$1);addnode($$,$2);}
		|				{$$ = setnode("","",@$.first_line);};
Def 		: Specifier DecList  SEMI	{$$ = setnode("Def","",@$.first_line);addnode($$,$1);addnode($$,$2);addnode($$,$3);}
		| Specifier DecList error	{iserror=1;printf("Error type B at line %d :Missing ;\n",yylineno);};
DecList		: Dec				{$$ = setnode("DecList","",@$.first_line);addnode($$,$1);}
		| Dec COMMA DecList		{$$ = setnode("DecList","",@$.first_line);addnode($$,$1);addnode($$,$2);addnode($$,$3);}
		| Dec error DecList		{iserror=1;printf("Error type B at line %d :aaaaaMissing ,\n",yylineno);};
Dec 		: VarDec			{$$ = setnode("Dec","",@$.first_line);addnode($$,$1);}
		| VarDec ASSIGNOP Exp		{$$ = setnode("Dec","",@$.first_line);addnode($$,$1);addnode($$,$2);addnode($$,$3);};
Exp		: Exp ASSIGNOP Exp		{$$ = setnode("Exp","",@$.first_line);addnode($$,$1);addnode($$,$2);addnode($$,$3);}
		| Exp AND Exp			{$$ = setnode("Exp","",@$.first_line);addnode($$,$1);addnode($$,$2);addnode($$,$3);}
		| Exp OR Exp			{$$ = setnode("Exp","",@$.first_line);addnode($$,$1);addnode($$,$2);addnode($$,$3);}
		| Exp RELOP Exp			{$$ = setnode("Exp","",@$.first_line);addnode($$,$1);addnode($$,$2);addnode($$,$3);}
		| Exp PLUS Exp			{$$ = setnode("Exp","",@$.first_line);addnode($$,$1);addnode($$,$2);addnode($$,$3);}
		| Exp MINUS Exp			{$$ = setnode("Exp","",@$.first_line);addnode($$,$1);addnode($$,$2);addnode($$,$3);}
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
		| Exp LB Exp RB			{$$ = setnode("Exp","",@$.first_line);addnode($$,$1);addnode($$,$2);addnode($$,$3);addnode($$,$4);}
		| Exp DOT ID			{$$ = setnode("Exp","",@$.first_line);addnode($$,$1);addnode($$,$2);addnode($$,$3);}
		| ID				{$$ = setnode("Exp","",@$.first_line);addnode($$,$1);}
		| INT				{$$ = setnode("Exp","",@$.first_line);addnode($$,$1);}
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

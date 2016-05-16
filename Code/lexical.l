%{
#include <stdio.h>
#include "syntax.tab.h"
#include "tree.h"
#include "lexicial.h"
	int yycolumn = 1;
	 #define YY_USER_ACTION \
	     yylloc.first_line = yylloc.last_line = yylineno; \
			 yylloc.first_column = yycolumn; \
			 yylloc.last_column = yycolumn + yyleng - 1; \
			 yycolumn += yyleng;  

%}
delim [ \t]
ws {delim}+
digit [0-9]
letter [_a-zA-Z]
float (0|[1-9][0-9]*)[.]{digit}+ 
int 0|[1-9][0-9]*
int8 [0][1-7][0-7]*
int16 [0][Xx][0-9a-fA-F][0-9a-fA-F]*
id {letter}({letter}|{digit})*
relop >|<|>=|<=|==|!=
%%
{delim} {}
[\n] { yylineno++;}
[\r] {}
int|float { yylval=(struct node *)setnode("TYPE",yytext,yylineno); return TYPE;}
{relop} { yylval=(struct node *)setnode("RELOP",yytext,yylineno);return RELOP;}
return { yylval=(struct node *)setnode("RETURN",yytext,yylineno);return RETURN;}
if { yylval=(struct node *)setnode("IF",yytext,yylineno);return IF;}
else { yylval=(struct node *)setnode("ELSE",yytext,yylineno);return ELSE;}
while { yylval=(struct node *)setnode("WHILE",yytext,yylineno);return WHILE;}
struct { yylval=(struct node *)setnode("STRUCT",yytext,yylineno);return STRUCT;}
";" { yylval=(struct node *)setnode("SEMI",yytext,yylineno);return SEMI;}
{id} { yylval=(struct node *)setnode("ID",yytext,yylineno);return ID;}
{int} { yylval=(struct node *)setnode("INT",yytext,yylineno);return INT;}
{int8} { yylval=(struct node *)setnode("INT",yytext,yylineno);return INT;}
{int16} { yylval=(struct node *)setnode("INT",yytext,yylineno);return INT;}
{float} { yylval=(struct node *)setnode("FLOAT",yytext,yylineno);return FLOAT;}
"," { yylval=(struct node *)setnode("COMMA",yytext,yylineno);return COMMA;}
"=" { yylval=(struct node *)setnode("ASSIGNOP",yytext,yylineno);return ASSIGNOP;}
"+" { yylval=(struct node *)setnode("PLUS",yytext,yylineno);return PLUS;}
"-" { yylval=(struct node *)setnode("MINUS",yytext,yylineno);return MINUS;}
"*" { yylval=(struct node *)setnode("STAR",yytext,yylineno);return STAR;}
"/" { yylval=(struct node *)setnode("DIV",yytext,yylineno);return DIV;}
"&&" { yylval=(struct node *)setnode("AND",yytext,yylineno);return AND;}
"||" { yylval=(struct node *)setnode("OR",yytext,yylineno);return OR;}
"." { yylval=(struct node *)setnode("DOT",yytext,yylineno);return DOT;}
"!" { yylval=(struct node *)setnode("NOT",yytext,yylineno);return NOT;}
"(" { yylval=(struct node *)setnode("LP",yytext,yylineno);return LP;}
")" { yylval=(struct node *)setnode("RP",yytext,yylineno);return RP;}
"[" { yylval=(struct node *)setnode("LB",yytext,yylineno);return LB;} 
"]" { yylval=(struct node *)setnode("RB",yytext,yylineno);return RB;}
"{" { yylval=(struct node *)setnode("LC",yytext,yylineno);return LC;}
"}" { yylval=(struct node *)setnode("RC",yytext,yylineno);return RC;}
. { printf("Error type A at Line %d:Myterious characters \'%s\'\n",yylineno,yytext);}
%%

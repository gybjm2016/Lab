#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include"translate.h"
#include <assert.h>
char* getop(Operand h){
//	assert(h!=NULL);
//	printf("h->kind = %d\n",h->kind);
	switch(h->kind){
		case VARIABLE:
//			printf("here \n");
			return h->u.var_name;
			break;
		case CONSTANT:{
			//char temp[1000];
			//itoa(h->u.value,temp,10);
//			printf("constant\n");

			char *str = (char*)malloc(16);
			str[0]='#';
			sprintf(&(str[1]), "%d", h->u.value);
	//		printf("cons %s \n",str);
			return str;
		}
		case ADDRESS:
//			printf("not finshed\n");
			break;
		case TEMPVA:{
			char *str = (char*)malloc(16);
			sprintf(&(str[1]), "%d", h->u.no);
			str[0]='t';
//			printf("%s\n",str);
			return str;
		}
		case LABEL:{
			char str[100];
			sprintf(&(str[0]), "%d", h->u.labno);
			char ggg[100]="label";
			strcat(ggg,str);
			return ggg;
			break;
		}
		default:
//			printf("nevr here\n");
//			return "kongde";
			break;
	}
}
void printcode(code* h,char *filename){
//	printf("print to file\n");
	FILE *fp=fopen(filename,"w");
	if(fp==NULL){
		printf("fail to open file\n");
		return;
	}
	code* p=h;
//	int i = 0;
	while(p!=NULL){
//		printf("%d\n",p->word.kind);
//		i++;
		switch(p->word.kind){
			case ASSIGN:
//				printf("assign\n");
				fprintf(fp,"%s := %s\n",getop(p->word.u.assign.left),getop(p->word.u.assign.right));
				break;
			case ADD:
				fprintf(fp,"%s := %s + %s\n",getop(p->word.u.calcu.result),getop(p->word.u.calcu.op1),getop(p->word.u.calcu.op2));
				break;
			case MIN:
				fprintf(fp,"%s := %s - %s\n",getop(p->word.u.calcu.result),getop(p->word.u.calcu.op1),getop(p->word.u.calcu.op2));  
				break;
			case MUL:
				fprintf(fp,"%s := %s * %s\n",getop(p->word.u.calcu.result),getop(p->word.u.calcu.op1),getop(p->word.u.calcu.op2));
				break;
			case DIVID:
				fprintf(fp,"%s := %s / %s\n",getop(p->word.u.calcu.result),getop(p->word.u.calcu.op1),getop(p->word.u.calcu.op2)); 
				break;
			case LAB:
				fprintf(fp,"LABEL %s :\n",getop(p->word.u.lab.op1));
				break;
			case RET:
				fprintf(fp,"RETURN %s\n",getop(p->word.u.ret.op1));
				break;
			case GOTO:
				fprintf(fp,"GOTO %s\n",getop(p->word.u.got.op1));
				break;
			case IFGOTO:
				fprintf(fp,"IF %s %s %s GOTO %s\n",getop(p->word.u.ifgoto.op1),getop(p->word.u.ifgoto.op2),getop(p->word.u.ifgoto.op3),getop(p->word.u.ifgoto.op4));
				break;
			case READ:
				fprintf(fp,"READ %s\n",getop(p->word.u.ret.op1));
				break;
			case WRITE:
				fprintf(fp,"WRITE %s\n",getop(p->word.u.ret.op1));
				break;
			case ARG:
				fprintf(fp,"ARG %s\n",getop(p->word.u.ret.op1));
				break;
			case CALL:
				fprintf(fp,"%s := CALL %s\n",getop(p->word.u.assign.left),getop(p->word.u.assign.right));
				break;
			case FUNCTION:
				fprintf(fp,"FUNCTION %s :\n",getop(p->word.u.ret.op1));
				break;
			case DECSP:
				fprintf(fp,"DEC %s %d\n",getop(p->word.u.assign.left),p->word.u.assign.right->u.value);
				break;
			case PARAM:
				fprintf(fp,"PARAM %s\n",getop(p->word.u.ret.op1));
				break;
			case ADDRTO:
				fprintf(fp,"%s := *%s\n",getop(p->word.u.assign.left),getop(p->word.u.assign.right));
				break;
			case GETADD:
				fprintf(fp,"%s := &%s\n",getop(p->word.u.assign.left),getop(p->word.u.assign.right));
				break;
			case TOADD:
				fprintf(fp,"*%s := %s\n",getop(p->word.u.assign.left),getop(p->word.u.assign.right));
				break;
		}
//		printf("hhhhhhhhhhh\n");
		p=p->next;
	}
	fclose(fp);
	return ;
}

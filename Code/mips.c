#include<stdio.h>
#include"translate.h"
struct OffsetNode{
	Operand name;
	int offset;
};
typedef struct OffsetNode OffsetNode;
#define MaxOffset 1000
OffsetNode offsetArray[MaxOffset];
int lastOffsetIndex = 0;
int stackoffset=0;
int parnum=0;
int setoffset(Operand operand, int offset){
	offsetArray[lastOffsetIndex].name = operand;
	offsetArray[lastOffsetIndex].offset = offset;
	lastOffsetIndex++;
}
int testEqual(Operand op1,Operand op2){
	if(op1->kind!=op2->kind)
		return 0;
	else{
		if(op1->kind==VARIABLE){
			if(strcmp(op1->u.var_name,op2->u.var_name)==0)
				return 1;
			else
				return 0;
		}
		else if(op1->kind==CONSTANT){
			if(op1->u.value==op2->u.value)
				return 1;
			else
				return 0;
		}
		else if(op1->kind==ADDRESS){
			if(op1->u.address==op2->u.address)
				return 1;
			else
				return 0;
		}
		else if(op1->kind==TEMPVA){
			if(op1->u.no==op2->u.no)
				return 1;
			else
				return 0;
		}
		else if(op1->kind==LABEL){
			if(op1->u.labno==op2->u.labno)
				return 1;
			else
				return 0;
		}
	}
}

int getoffset(Operand operand){
	int i =0;
	while(i<lastOffsetIndex){
		if(testEqual(operand,offsetArray[i].name)==1)
			return offsetArray[i].offset;
		i++;
	}
	return -1;
}


char* reg(FILE* fp,Operand h,int flag){
	if(h->kind==CONSTANT){
		fprintf(fp,"li $t%d, %d\n",flag,h->u.value);
		if(flag==1)
			return "$t1";
		else if(flag==2)
			return "$t2";
		else 
			return "$t3";
	}
	if(getoffset(h)!=-1){
		if(flag==1){
			fprintf(fp,"lw $t1, %d($sp)\n",getoffset(h));
			return "$t1";
		}
		if(flag==2){
			fprintf(fp,"lw $t2, %d($sp)\n",getoffset(h));
			return "$t2";
		}
		if(flag==3){
			fprintf(fp,"lw $t3, %d($sp)\n",getoffset(h));
			return "$t3";
		}
	}
	else{
		setoffset(h,stackoffset);
		stackoffset=stackoffset+4;
		if(flag==1)
			return "$t1";
		else if(flag==2)
			return "$t2";
		else
			return "$t3";
	}

}
void save(FILE* fp,Operand h,int flag){
	int offset = getoffset(h);
	fprintf(fp, "sw $t%d, %d($sp)\n",flag,offset);
}
void mipscode(code* h,char* filename){
	FILE *fp=fopen(filename,"w");
	if(fp==NULL){
		printf("fail to open file\n");
		return;
	}
	fprintf(fp,".data\n");
	fprintf(fp,"_prompt: .asciiz \"Enter an integer:\"\n");
	fprintf(fp,"_ret: .asciiz \"\\n\"\n");
	fprintf(fp,".globl main\n");
	fprintf(fp,".text\n");
	fprintf(fp,"read:\n");
	fprintf(fp,"li $v0, 4\n");
	fprintf(fp,"la $a0, _prompt\n");
	fprintf(fp,"syscall\n");
	fprintf(fp,"li $v0 5\n");
	fprintf(fp,"syscall\n");
	fprintf(fp,"jr $ra\n");
	fprintf(fp,"\n");
	fprintf(fp,"write:\n");
	fprintf(fp,"li $v0, 1\n");
	fprintf(fp,"syscall\n");
	fprintf(fp,"li $v0, 4\n");
	fprintf(fp,"la $a0, _ret\n");
	fprintf(fp,"syscall\n");
	fprintf(fp,"move $v0, $0\n");
	fprintf(fp,"jr $ra\n");
	fprintf(fp,"\n");
	code* p=h;
	code *tempcode;
	while(p!=NULL){
		switch(p->word.kind){
			case LAB:
				fprintf(fp,"%s:\n",getop(p->word.u.lab.op1));
				break;
			case ASSIGN:
				if(p->word.u.assign.right->kind==CONSTANT){
					fprintf(fp,"li %s, %d\n",reg(fp,p->word.u.assign.left,1),p->word.u.assign.right->u.value);
					save(fp,p->word.u.assign.left,1);
				}
				else{
					fprintf(fp,"move %s, %s\n",reg(fp,p->word.u.assign.left,1),reg(fp,p->word.u.assign.right,2));
					save(fp,p->word.u.assign.left,1);
				}
				break;
			case ADD:
				fprintf(fp,"add %s, %s, %s\n",reg(fp,p->word.u.calcu.result,1),reg(fp,p->word.u.calcu.op1,2),reg(fp,p->word.u.calcu.op2,3));
				save(fp,p->word.u.calcu.result,1);
				break;
			case MIN:
				fprintf(fp,"sub %s, %s, %s\n",reg(fp,p->word.u.calcu.result,1),reg(fp,p->word.u.calcu.op1,2),reg(fp,p->word.u.calcu.op2,3));
				save(fp,p->word.u.calcu.result,1);
				break;
			case MUL:
				fprintf(fp,"mul %s, %s, %s\n",reg(fp,p->word.u.calcu.result,1),reg(fp,p->word.u.calcu.op1,2),reg(fp,p->word.u.calcu.op2,3));
				save(fp,p->word.u.calcu.result,1);
				break;
			case DIVID:
				fprintf(fp,"div %s, %s\n",reg(fp,p->word.u.calcu.op1,2),reg(fp,p->word.u.calcu.op2,3));
				fprintf(fp,"mflo %s\n",reg(fp,p->word.u.calcu.result,1));
				save(fp,p->word.u.calcu.result,1);
				break;
			case ADDRTO:
				fprintf(fp,"lw %s, 0(%s)\n",reg(fp,p->word.u.assign.left,1),reg(fp,p->word.u.assign.right,2));
				save(fp,p->word.u.assign.left,1);
				break;
			case TOADD:
				fprintf(fp,"sw %s,0(%s)\n",reg(fp,p->word.u.assign.right,1),reg(fp,p->word.u.assign.left,2));
				break;
			case GOTO:
				fprintf(fp,"j %s\n",getop(p->word.u.got.op1));
				break;
			case CALL:
			case READ:
			case WRITE:
				parnum=0;
				if(p->word.kind==WRITE)
					fprintf(fp,"move $a0, %s\n",reg(fp,p->word.u.ret.op1,1));
				fprintf(fp,"addi $sp, $sp, %d\n",stackoffset+4);
				fprintf(fp,"sw $ra, -4($sp)\n");
				if(p->word.kind==CALL)
					fprintf(fp,"jal %s\n",getop(p->word.u.assign.right));
				else if(p->word.kind==READ)
					fprintf(fp,"jal read\n");
				else{
					fprintf(fp,"jal write\n");
				}
				fprintf(fp,"lw $ra, -4($sp)\n");
				fprintf(fp,"addi $sp, $sp, -%d\n",stackoffset+4);
				if(p->word.kind==CALL)
					fprintf(fp,"move %s,$v0\n",reg(fp,p->word.u.assign.left,1));
				else
					fprintf(fp,"move %s,$v0\n",reg(fp,p->word.u.ret.op1,1));
				if(p->word.kind==CALL)
					save(fp,p->word.u.assign.left,1);
				else
					save(fp,p->word.u.ret.op1,1);
				break;
			case RET:
				fprintf(fp,"move $v0, %s\n",reg(fp,p->word.u.ret.op1,1));
				fprintf(fp,"jr $ra\n");
				break;
			case IFGOTO:
				printf("%s\n",getop(p->word.u.ifgoto.op4));
				if(strcmp(getop(p->word.u.ifgoto.op2),"==")==0)
					fprintf(fp,"beq %s, %s, %s\n",reg(fp,p->word.u.ifgoto.op1,1),reg(fp,p->word.u.ifgoto.op3,2),getop(p->word.u.ifgoto.op4));
				else if(strcmp(getop(p->word.u.ifgoto.op2),"!=")==0)
					fprintf(fp,"bne %s, %s, %s\n",reg(fp,p->word.u.ifgoto.op1,1),reg(fp,p->word.u.ifgoto.op3,2),getop(p->word.u.ifgoto.op4));
				else if(strcmp(getop(p->word.u.ifgoto.op2),">")==0) 
					fprintf(fp,"bgt %s, %s, %s\n",reg(fp,p->word.u.ifgoto.op1,1),reg(fp,p->word.u.ifgoto.op3,2),getop(p->word.u.ifgoto.op4));
				else if(strcmp(getop(p->word.u.ifgoto.op2),"<")==0) 
					fprintf(fp,"blt %s, %s, %s\n",reg(fp,p->word.u.ifgoto.op1,1),reg(fp,p->word.u.ifgoto.op3,2),getop(p->word.u.ifgoto.op4));
				else if(strcmp(getop(p->word.u.ifgoto.op2),">=")==0) 
					fprintf(fp,"bge %s, %s, %s\n",reg(fp,p->word.u.ifgoto.op1,1),reg(fp,p->word.u.ifgoto.op3,2),getop(p->word.u.ifgoto.op4));
				else
					fprintf(fp,"ble %s, %s, %s\n",reg(fp,p->word.u.ifgoto.op1,1),reg(fp,p->word.u.ifgoto.op3,2),getop(p->word.u.ifgoto.op4));
				break;
			case FUNCTION:
				fprintf(fp,"%s:\n",getop(p->word.u.ret.op1));
				stackoffset=0;
				break;
			case ARG:
				if(parnum<=3){
					fprintf(fp,"move $a%d, %s\n",parnum,reg(fp,p->word.u.ret.op1,1));
					parnum++;
				}
				else{
					fprintf(fp,"sw %s, %d($sp)",reg(fp,p->word.u.ret.op1,1),stackoffset+4+(parnum-4)*4); 
					parnum++;
				}
				break;
			case PARAM:
				tempcode = p;
				parnum=0;
				while(tempcode->word.kind == PARAM){
					parnum++;
					tempcode=tempcode->next;
				}
				tempcode = p;
				if(parnum<4)
					stackoffset = 0;
				else
					stackoffset = parnum*4; 
				for(int i=0;i<parnum;i++){
					setoffset(p->word.u.ret.op1,stackoffset);
					stackoffset+=4;
				}

				for(int i=0;i<4 && tempcode->word.kind == PARAM;i++){
					fprintf(fp, "sw $a%d, %d($sp)\n",i,getoffset(tempcode->word.u.ret.op1));
				tempcode = tempcode->next;
				}
				if(parnum>4){
					for(int i=4;i<parnum;++i){
						fprintf(fp, "lw $t1, %d($sp)\n", (i-4)*4);
						fprintf(fp, "sw $t1, %d($sp)\n", getoffset(tempcode->word.u.ret.op1));
						tempcode = tempcode->next;
					}
				}
				for(;p->next->word.kind == PARAM;p=p->next);
				parnum=0;
				break;
				/*for(int i=4;i<parnum;i++){
					if(i<4)
						fprintf(fp,"sw $a%d, %d($sp)\n",i,(i+parnum-4)*4);
					else{
						fprintf(fp,"lw $t1, %d($sp)\n",(i-4)*4);
						fprintf(fp,"sw $t1, %d($sp)\n",(i+parnum-4)*4);
					}
				}
				else 
					for(int i=0;i<parnum;i++)
						fprintf(fp,"sw $a%d, %d($sp)\n",i,i*4);
				for(;p->next->word.kind==PARAM;)
					p=p->next;
					*/
			default:
				printf("not finished\n");
		}
		p=p->next;
	}
	fclose(fp);
	return;
}

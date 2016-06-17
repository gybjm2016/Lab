#include "translate.h"
#include "semantics.h"
int count=0;
int labelnum=0;
Operand arghead;
int large(Type h){
	if(h->kind==BASIC)
		return 4;
	else if(h->kind==ARRAY)
		return 4*(h->u.array.size);
	else 
		return 0;
}
int getmovenum(struct charatable* h,char* nam){
	if(nam==NULL){
		FieldList p=h->charatype->u.structure;
		int all=0;
		for(;p!=NULL;p=p->tail)
			all=all+large(p->type);
		return all;
	}
	FieldList p=h->charatype->u.structure;
	int result=0;
	for(;p!=NULL;p=p->tail){
		if(strcmp(p->name,nam)!=0)
			result=result+large(p->type);
		else
			break;
	}
	int all=0;
	p=h->charatype->u.structure;
	for(;p!=NULL;p=p->tail)
		all=all+large(p->type);
	return all-4-result;
}
Operand new_temp(){
	Operand temp=(Operand)malloc(sizeof(struct Operand_));
	temp->kind=TEMPVA;
	temp->u.no=count;
	count++;
	return temp;
}
Operand new_label(){
	Operand temp=(Operand)malloc(sizeof(struct Operand_));
	temp->kind=LABEL;
	temp->u.labno=labelnum;
	labelnum++;
	return temp;
}
Operand new_Operand(){
	Operand temp=(Operand)malloc(sizeof(struct Operand_));
	return temp;
}
struct code* merge(struct code* m,struct code* n){
	if(m==NULL && n==NULL)
		return NULL;
	if(m==NULL)
		return n;
	if(n==NULL)
		return m;
	struct code* p=m;
	for(;p->next!=NULL;)
		p=p->next;
	p->next=n;
	n->pre=p;
	return m;
}
Operand mergeOperand(Operand m,Operand n){
	if(m==NULL && n==NULL)
		return NULL;
	if(m==NULL)
		return n;
	if(n==NULL)
		return m;
	Operand p=m;
	for(;p->next!=NULL;)
		p=p->next;
	p->next=n;
	return m;
}
code* translate(struct node* h){
	if(h==NULL)
		return NULL;
	struct node* child;
	if(strcmp(h->child[0]->type,"ExtDefList")==0){
		h=h->child[0];
		code* temp1=translate_Extdeflist(h);
		return temp1;
	}
}
code* translate_Extdeflist(struct node *h){
//	printf("etdeflist\n");
	code* temp1=NULL;
	while(h->childnum!=0){
		code* temp2=translate_Extdef(h->child[0]);
		temp1=merge(temp1,temp2);
		h=h->child[1];
	}
	return temp1;
}
code* translate_Extdef(struct node *h){
//	printf("extdef%d\n",h->childnum);
	if(h->childnum==3&&strcmp(h->child[1]->type,"FunDec")==0){
		code* temp1=translate_Fundec(h->child[1]);
		code* temp2=translate_Compst(h->child[2]);
		return merge(temp1,temp2);
	}
	else
		return NULL;
}
struct code* translate_Exp(struct node* h,Operand place){
	if(h->childnum==1&&strcmp(h->child[0]->type,"INT")==0){
		int num=myatoi(h->child[0]->text);
		struct code* temp=(struct code*)malloc(sizeof(struct code));
		temp->word.kind=ASSIGN;
		temp->word.u.assign.right=new_Operand();
		temp->word.u.assign.right->kind=CONSTANT;
		temp->word.u.assign.right->u.value=num;
		temp->word.u.assign.left=place;
		return temp;
	}
	else if(h->childnum==1&&strcmp(h->child[0]->type,"ID")==0){
		struct code* temp=(struct code*)malloc(sizeof(struct code));
		if(search(h->child[0]->text)->charatype->kind==BASIC) 
			temp->word.kind=ASSIGN;
		else
			temp->word.kind=GETADD;
		temp->word.u.assign.left=place;
		temp->word.u.assign.right=new_Operand();
		temp->word.u.assign.right->kind=VARIABLE;
		temp->word.u.assign.right->u.var_name=h->child[0]->text;
		return temp;
	}
	else if(h->childnum==3&&(strcmp(h->child[1]->type,"PLUS")==0||strcmp(h->child[1]->type,"MINUS")==0||strcmp(h->child[1]->type,"STAR")==0||strcmp(h->child[1]->type,"DIV")==0)){
		Operand t1=new_temp();
		Operand t2=new_temp();
		struct code* temp1=translate_Exp(h->child[0],t1);
		struct code* temp2=translate_Exp(h->child[2],t2);
		struct code* temp3=(struct code*)malloc(sizeof(struct code));
		if(strcmp(h->child[1]->type,"PLUS")==0)
			temp3->word.kind=ADD;
		else if(strcmp(h->child[1]->type,"MINUS")==0)
			temp3->word.kind=MIN;
		else if(strcmp(h->child[1]->type,"STAR")==0)
			temp3->word.kind=MUL;
		else
			temp3->word.kind=DIVID;
		temp3->word.u.calcu.result=place;
		temp3->word.u.calcu.op1=t1;
		temp3->word.u.calcu.op2=t2;
		return merge(merge(temp1,temp2),temp3);
	}
	else if(h->childnum==3&&strcmp(h->child[1]->type,"ASSIGNOP")==0){
		if(h->child[0]->childnum==1&&strcmp(h->child[0]->child[0]->type,"ID")==0){
//			printf("id\n");
//			printf("%d",h->child[2]->childnum);   
			Operand t1=new_temp();
//			printf("zhubei\n");
//			printf("%d",h->child[2]->childnum);
			code* temp1=translate_Exp(h->child[2],t1);
			code* temp2=(struct code*)malloc(sizeof(struct code));
			code* temp3=(struct code*)malloc(sizeof(struct code));
			temp2->word.kind=ASSIGN;
//			printf("uuuuu\n");
			temp2->word.u.assign.left=new_Operand();
			temp2->word.u.assign.left->kind=VARIABLE;
			temp2->word.u.assign.left->u.var_name=h->child[0]->child[0]->text;
			temp2->word.u.assign.right=t1;
			temp3->word.kind=ASSIGN;
			temp3->word.u.assign.left=place;
			temp3->word.u.assign.right=new_Operand();
			temp3->word.u.assign.right->kind=VARIABLE;
			temp3->word.u.assign.right->u.var_name=h->child[0]->child[0]->text;
			if(place!=NULL){
				temp2->next=temp3;
				temp3->pre=temp2;
				return merge(temp1,temp2);
			}
			else
				return merge(temp1,temp2);
		}
		else if(h->child[0]->childnum==4&&strcmp(h->child[0]->child[1]->type,"LB")==0){
//			printf("shuzu\n");
			Operand tt=new_temp();
			code* tempt=translate_Exp(h->child[2],tt);
			h=h->child[0];
			Operand t1=new_temp();
			code* temp1=translate_Exp(h->child[2],t1);
			Operand t2=new_temp();
			code* temp2=(code*)malloc(sizeof(struct code));
			temp2->word.kind=MUL;
			temp2->word.u.calcu.result=t2;
			temp2->word.u.calcu.op1=t1;
			temp2->word.u.calcu.op2=new_Operand();
			temp2->word.u.calcu.op2->kind=CONSTANT;
			temp2->word.u.calcu.op2->u.value=4;
			code* temp3=(code*)malloc(sizeof(struct code));
			temp3->word.kind=GETADD;
			Operand t3=new_temp();
			temp3->word.u.assign.left=t3;
			temp3->word.u.assign.right=new_Operand();
			temp3->word.u.assign.right->kind=VARIABLE;
			temp3->word.u.assign.right->u.var_name=h->child[0]->child[0]->text;
			code* temp4=(code*)malloc(sizeof(struct code));
			Operand t4=new_temp();
			temp4->word.kind=ADD;
			temp4->word.u.calcu.result=t4;
			temp4->word.u.calcu.op1=t2;
			temp4->word.u.calcu.op2=t3;
			code* temp5=(code*)malloc(sizeof(struct code));
			temp5->word.kind=TOADD;
			temp5->word.u.assign.left=t4;
			temp5->word.u.assign.right=tt;
			return merge(merge(merge(merge(merge(tempt,temp1),temp2),temp3),temp4),temp5);
		}
		else if(h->child[0]->childnum==3&&strcmp(h->child[0]->child[1]->type,"DOT")==0){
			Operand tt=new_temp();
			code* tempt=translate_Exp(h->child[2],tt);
			h=h->child[0];
			struct charactable* hhh=search(h->child[0]->child[0]->text);
			int movenum=getmovenum(hhh,h->child[2]->text);
//			printf("getadd\n");
			code* temp1=(code*)malloc(sizeof(struct code));
			Operand t1=new_temp();
			if(search(h->child[0]->child[0]->text)->ispara==0){
				temp1->word.kind=GETADD;
				temp1->word.u.assign.left=t1;
				temp1->word.u.assign.right=new_Operand();
				temp1->word.u.assign.right->kind=VARIABLE;
				temp1->word.u.assign.right->u.var_name=h->child[0]->child[0]->text;
			}
			else{
				temp1->word.kind=ASSIGN;
				temp1->word.u.assign.left=t1;
				temp1->word.u.assign.right=new_Operand();
				temp1->word.u.assign.right->kind=VARIABLE;
				temp1->word.u.assign.right->u.var_name=h->child[0]->child[0]->text;
			}
			Operand t2=new_temp();
			code* temp2=(code*)malloc(sizeof(struct code));
			temp2->word.kind=ASSIGN;
			temp2->word.u.assign.left=t2;
			temp2->word.u.assign.right=new_Operand();
			temp2->word.u.assign.right->kind=CONSTANT;
			temp2->word.u.assign.right->u.value=movenum;
			Operand t3=new_temp();
			code* temp3=(code*)malloc(sizeof(struct code));
			temp3->word.kind=ADD;
			temp3->word.u.calcu.result=t3;
			temp3->word.u.calcu.op1=t1;
			temp3->word.u.calcu.op2=t2;
			code* temp4=(code*)malloc(sizeof(struct code));
			temp4->word.kind=TOADD;
			temp4->word.u.assign.left=t3;
			temp4->word.u.assign.right=tt;
			return merge(merge(merge(merge(tempt,temp1),temp2),temp3),temp4);
		}
	}
	else if(h->childnum==2&&strcmp(h->child[0]->type,"MINUS")==0){
		Operand t1=new_temp();
		code* temp1=translate_Exp(h->child[1],t1);
		code* temp2=(code*)malloc(sizeof(struct code));
		temp2->word.kind=MIN;
		temp2->word.u.calcu.result=place;
		temp2->word.u.calcu.op1=new_Operand();
		temp2->word.u.calcu.op1->kind=CONSTANT;
		temp2->word.u.calcu.op1->u.value=0;
		temp2->word.u.calcu.op2=t1;
		return merge(temp1,temp2);
	}
	else if((h->childnum==2&&strcmp(h->child[0]->type,"NOT")==0)||(h->childnum==3&&(strcmp(h->child[1]->type,"RELOP")==0||strcmp(h->child[1]->type,"AND")==0||strcmp(h->child[1]->type,"OR")==0))){
		Operand label1=new_label();
		Operand label2=new_label();
		code* temp1=(code*)malloc(sizeof(struct code));
		temp1->word.kind=ASSIGN;
		temp1->word.u.assign.left=place;
		temp1->word.u.assign.right=new_Operand();
		temp1->word.u.assign.right->kind=CONSTANT;
		temp1->word.u.assign.right->u.value=0;
		code* temp2=translate_Cond(h,label1,label2);
		code* temp3=(code*)malloc(sizeof(struct code));
		temp3->word.kind=LAB;
		temp3->word.u.lab.op1=label1;
		code* temp4=(code*)malloc(sizeof(struct code));
		temp4->word.kind=ASSIGN;
		temp4->word.u.assign.left=place;
		temp4->word.u.assign.right=new_Operand();
		temp4->word.u.assign.right->kind=CONSTANT;
		temp4->word.u.assign.right->u.value=1;
		code* temp5=(code*)malloc(sizeof(struct code));
		temp5->word.kind=LAB;
		temp5->word.u.lab.op1=label2;
		return merge(merge(merge(merge(temp1,temp2),temp3),temp4),temp5);
	}
	else if(h->childnum==3&&strcmp(h->child[0]->type,"ID")==0){
		if(strcmp(h->child[0]->text,"read")==0){
			code* temp1=(code*)malloc(sizeof(struct code));
			temp1->word.kind=READ;
			temp1->word.u.ret.op1=place;
			return temp1;
		}
		else{
			code* temp1=(code*)malloc(sizeof(struct code));
			temp1->word.kind=CALL;
			temp1->word.u.assign.left=place;
			temp1->word.u.assign.right=new_Operand();
			temp1->word.u.assign.right->kind=VARIABLE;
			temp1->word.u.assign.right->u.var_name=h->child[0]->text;
			return temp1;
		}
	}
	else if(h->childnum==4&&strcmp(h->child[0]->type,"ID")==0){
//		printf("hanshu\n");
		Operand arg_list=NULL;
		arghead=NULL;
		code* temp1=translate_Args(h->child[2],arg_list);
		arg_list=arghead;
		if(strcmp(h->child[0]->text,"write")==0){
			code* temp2=(code*)malloc(sizeof(struct code));
			temp2->word.kind=WRITE;
			temp2->word.u.ret.op1=arg_list;
			return merge(temp1,temp2);
		}
		else{
//			printf("canshuwanbi\n");
			code* temp2=NULL;
			for(Operand p=arg_list;p!=NULL;p=p->next){
				code* temp3=(code*)malloc(sizeof(struct code));
				temp3->word.kind=ARG;
				temp3->word.u.ret.op1=p;
				temp2=merge(temp2,temp3);
//				printf("meiyige\n");
			}
			code* temp4=(code*)malloc(sizeof(struct code));
			temp4->word.kind=CALL;
			if(place==NULL)
				temp4->word.u.assign.left=new_temp();
			else
				temp4->word.u.assign.left=place;
			temp4->word.u.assign.right=new_Operand();
			temp4->word.u.assign.right->kind=VARIABLE;
			temp4->word.u.assign.right->u.var_name=h->child[0]->text;
//			printf("jieshule\n");
			return merge(merge(temp1,temp2),temp4);
//			printf("meifanuhui\n");
		}
	}
	else if(strcmp(h->child[0]->type,"LP")==0){
		return translate_Exp(h->child[1],place);
	}
	else if(h->childnum==4&&strcmp(h->child[0]->type,"Exp")==0){
		Operand t1=new_temp();
		code* temp1=translate_Exp(h->child[2],t1);
		Operand t2=new_temp();
		code* temp2=(code*)malloc(sizeof(struct code));
		temp2->word.kind=MUL;
		temp2->word.u.calcu.result=t2;
		temp2->word.u.calcu.op1=t1;
		temp2->word.u.calcu.op2=new_Operand();
		temp2->word.u.calcu.op2->kind=CONSTANT;
		temp2->word.u.calcu.op2->u.value=4;
		code* temp3=(code*)malloc(sizeof(struct code));
		temp3->word.kind=GETADD;
		Operand t3=new_temp();
		temp3->word.u.assign.left=t3;
		temp3->word.u.assign.right=new_Operand();
		temp3->word.u.assign.right->kind=VARIABLE;
		temp3->word.u.assign.right->u.var_name=h->child[0]->child[0]->text;
		code* temp4=(code*)malloc(sizeof(struct code));
		Operand t4=new_temp();
		temp4->word.kind=ADD;
		temp4->word.u.calcu.result=t4;
		temp4->word.u.calcu.op1=t2;
		temp4->word.u.calcu.op2=t3;
		code* temp5=(code*)malloc(sizeof(struct code));
		Operand t5=new_temp();
		temp5->word.kind=ADDRTO;
		temp5->word.u.assign.left=t5;
		temp5->word.u.assign.right=t4;
		code* temp6=(code*)malloc(sizeof(struct code));
		temp6->word.kind=ASSIGN;
		temp6->word.u.assign.left=place;
		temp6->word.u.assign.right=t5;
		return merge(merge(merge(merge(merge(temp1,temp2),temp3),temp4),temp5),temp6);
	}
	else if(h->childnum==3&&strcmp(h->child[1]->type,"DOT")==0){
//		printf("DOT\n");
		struct charactable* hhh=search(h->child[0]->child[0]->text);
		int movenum=getmovenum(hhh,h->child[2]->text);
//		printf("getadd\n");
		code* temp1=(code*)malloc(sizeof(struct code));
		Operand t1=new_temp();
		if(search(h->child[0]->child[0]->text)->ispara==0){
			temp1->word.kind=GETADD;
			temp1->word.u.assign.left=t1;
			temp1->word.u.assign.right=new_Operand();
			temp1->word.u.assign.right->kind=VARIABLE;
			temp1->word.u.assign.right->u.var_name=h->child[0]->child[0]->text;
		}
		else{
			temp1->word.kind=ASSIGN;
			temp1->word.u.assign.left=t1;
			temp1->word.u.assign.right=new_Operand();
			temp1->word.u.assign.right->kind=VARIABLE;
			temp1->word.u.assign.right->u.var_name=h->child[0]->child[0]->text;
		}
		Operand t2=new_temp();
		code* temp2=(code*)malloc(sizeof(struct code));
		temp2->word.kind=ASSIGN;
		temp2->word.u.assign.left=t2;
		temp2->word.u.assign.right=new_Operand();
		temp2->word.u.assign.right->kind=CONSTANT;
		temp2->word.u.assign.right->u.value=movenum;
		Operand t3=new_temp();
		code* temp3=(code*)malloc(sizeof(struct code));
		temp3->word.kind=ADD;
		temp3->word.u.calcu.result=t3;
		temp3->word.u.calcu.op1=t1;
		temp3->word.u.calcu.op2=t2;
		code* temp4=(code*)malloc(sizeof(struct code));
		temp4->word.kind=ADDRTO;
		temp4->word.u.assign.left=place;
		temp4->word.u.assign.right=t3;
		return merge(merge(merge(temp1,temp2),temp3),temp4);
	}
	else
		return NULL;
}
code* translate_Stmt(struct node* h){
//	printf("stmt\n");
	if(h->childnum==2){
		return translate_Exp(h->child[0],NULL);
	}
	else if(h->childnum==1){
		return translate_Compst(h->child[0]);
	}
	else if(h->childnum==3){
		Operand t1=new_temp();
		code* temp1=translate_Exp(h->child[1],t1);
		code* temp2=(code*)malloc(sizeof(struct code));
		temp2->word.kind=RET;
		temp2->word.u.ret.op1=t1;
		return merge(temp1,temp2);
	}
	else if(h->childnum==5&&strcmp(h->child[0]->type,"IF")==0){
//		printf("meiyou\n");
		Operand label1=new_label();
		Operand label2=new_label();
		code* temp1=translate_Cond(h->child[2],label1,label2);
//		printf("hhhh\n");
//		printf("%d",h->child[4]->childnum);
		code* temp2=translate_Stmt(h->child[4]);
		code* temp3=(code*)malloc(sizeof(struct code));
		temp3->word.kind=LAB;
		temp3->word.u.lab.op1=label1;
		code* temp4=(code*)malloc(sizeof(struct code));
		temp4->word.kind=LAB;
		temp4->word.u.lab.op1=label2;
		return merge(merge(merge(temp1,temp3),temp2),temp4);
	}
	else if(h->childnum==7){
//		printf("jaiyou\n");
		Operand label1=new_label();
		Operand label2=new_label();
		Operand label3=new_label();
		code* temp1=translate_Cond(h->child[2],label1,label2);
		code* temp2=translate_Stmt(h->child[4]);
		code* temp3=translate_Stmt(h->child[6]);
		code* temp4=(code*)malloc(sizeof(struct code));
		temp4->word.kind=LAB;
		temp4->word.u.lab.op1=label1;
		code* temp5=(code*)malloc(sizeof(struct code));
		temp5->word.kind=GOTO;
		temp5->word.u.got.op1=label3;
		code* temp6=(code*)malloc(sizeof(struct code));
		temp6->word.kind=LAB;
		temp6->word.u.lab.op1=label2;
		code* temp7=(code*)malloc(sizeof(struct code));
		temp7->word.kind=LAB;
		temp7->word.u.lab.op1=label3;
		return merge(merge(merge(merge(merge(merge(temp1,temp4),temp2),temp5),temp6),temp3),temp7);
	}
	else{
//		printf("her\n");
		Operand label1=new_label();
		Operand label2=new_label();
		Operand label3=new_label();
		code* temp1=translate_Cond(h->child[2],label2,label3);
		code* temp2=translate_Stmt(h->child[4]);
		code* temp3=(code*)malloc(sizeof(struct code));
		temp3->word.kind=LAB;
		temp3->word.u.lab.op1=label1;
		code* temp4=(code*)malloc(sizeof(struct code));
		temp4->word.kind=LAB;
		temp4->word.u.lab.op1=label2;
		code* temp5=(code*)malloc(sizeof(struct code));
		temp5->word.kind=GOTO;
		temp5->word.u.got.op1=label1;
		code* temp6=(code*)malloc(sizeof(struct code));
		temp6->word.kind=LAB;
		temp6->word.u.lab.op1=label3;
		return merge(merge(merge(merge(merge(temp3,temp1),temp4),temp2),temp5),temp6);
	}
}
code* translate_Stmtlist(struct node *h){
	code* result=NULL;
	while(h->childnum!=0){
		code* temp1=translate_Stmt(h->child[0]);
		result=merge(result,temp1);
		h=h->child[1];
	}
	return result;
}
code* translate_Compst(struct node *h){
//	printf("compst\n");
	code* temp1=translate_Deflist(h->child[1]);
	code* temp2=translate_Stmtlist(h->child[2]);
	return merge(temp1,temp2);
}
code* translate_Cond(struct node* h,Operand label_true,Operand label_false){
	if(strcmp(h->child[1]->type,"RELOP")==0){
		Operand t1=new_temp();
		Operand t2=new_temp();
		code* temp1=translate_Exp(h->child[0],t1);
		code* temp2=translate_Exp(h->child[2],t2);
		code* temp3=(code*)malloc(sizeof(struct code));
		Operand hhh=(Operand)malloc(sizeof(struct Operand_));
		hhh->kind=VARIABLE;
		hhh->u.var_name=h->child[1]->text;
		temp3->word.kind=IFGOTO;
		temp3->word.u.ifgoto.op1=t1;
		temp3->word.u.ifgoto.op2=hhh;
		temp3->word.u.ifgoto.op3=t2;
		temp3->word.u.ifgoto.op4=label_true;
		code* temp4=(code*)malloc(sizeof(struct code));
		temp4->word.kind=GOTO;
		temp4->word.u.got.op1=label_false;
		return merge(merge(merge(temp1,temp2),temp3),temp4);
	}
	else if(strcmp(h->child[0]->type,"NOT")==0){
		return translate_Cond(h->child[1],label_false,label_true);
	}
	else if(strcmp(h->child[1]->type,"AND")==0){
		Operand label1=new_label();
		code* temp1=translate_Cond(h->child[0],label1,label_false);
		code* temp2=translate_Cond(h->child[2],label_true,label_false);
		code* temp3=(code*)malloc(sizeof(struct code));
		temp3->word.kind=LAB;
		temp3->word.u.lab.op1=label1;
		return merge(merge(temp1,temp3),temp2);
	}
	else if(strcmp(h->child[1]->type,"OR")==0){
		Operand label1=new_label();
		code* temp1=translate_Cond(h->child[0],label_true,label1);
		code* temp2=translate_Cond(h->child[2],label_true,label_false);
		code* temp3=(code*)malloc(sizeof(struct code));
		temp3->word.kind=LAB;
		temp3->word.u.lab.op1=label1;
		return merge(merge(temp1,temp3),temp2);
	}
	else{
		Operand t1=new_temp();
		code* temp1=translate_Exp(h,t1);
		code* temp2=(code*)malloc(sizeof(struct code));
		Operand hhh=(Operand)malloc(sizeof(struct Operand_));
		hhh->kind=VARIABLE;
		hhh->u.var_name="!=";
		Operand yyy=(Operand)malloc(sizeof(struct Operand_));
		yyy->kind=CONSTANT;
		yyy->u.value=0;
		temp2->word.kind=IFGOTO;
		temp2->word.u.ifgoto.op1=t1;
		temp2->word.u.ifgoto.op2=hhh;
		temp2->word.u.ifgoto.op3=yyy;
		temp2->word.u.ifgoto.op4=label_true;
		code* temp3=(code*)malloc(sizeof(struct code));
		temp3->word.kind=GOTO;
		temp3->word.u.got.op1=label_false;
		return merge(merge(temp1,temp2),temp3);
	}
}
code* translate_Args(struct node *h,Operand arg_list){
//	printf("args\n");
	if(h->childnum==1){
		Operand t1=new_temp();
		code* temp1=translate_Exp(h->child[0],t1);
		Operand hhh=mergeOperand(t1,arg_list);
		arghead=hhh;
		return temp1;
	}
	else{
		Operand t1=new_temp();
		code* temp1=translate_Exp(h->child[0],t1);
		Operand hhh=mergeOperand(t1,arg_list);
		arghead=hhh;
		code* temp2=translate_Args(h->child[2],hhh);
		return merge(temp1,temp2);
	}
}
code* translate_Dec(struct node *h){
	if(h->childnum==1){
		return translate_Vardec(h->child[0]);
	}
	else{
		Operand t1=new_temp();
		code* temp1=translate_Exp(h->child[2],t1);
		code* temp2=(code*)malloc(sizeof(struct code));
		temp2->word.kind=ASSIGN;
		temp2->word.u.assign.right=t1;
		temp2->word.u.assign.left=new_Operand();
		temp2->word.u.assign.left->kind=VARIABLE;
		temp2->word.u.assign.left->u.var_name=h->child[0]->child[0]->text;
		return merge(temp1,temp2);
	}
}
code* translate_Declist(struct node *h){
	code* temp1=NULL;
	while(h->childnum==3){
		code* temp2=translate_Dec(h->child[0]);
		temp1=merge(temp1,temp2);
		h=h->child[2];
	}
	code* temp2=translate_Dec(h->child[0]);
	temp1=merge(temp1,temp2);
	return temp1;
}
code* translate_Def(struct node *h){
	code* temp1=translate_Declist(h->child[1]);
	return temp1;
}
code* translate_Deflist(struct node *h){
	code* temp1=NULL;
	while(h->childnum!=0){
		code* temp2=translate_Def(h->child[0]);
		temp1=merge(temp1,temp2);
		h=h->child[1];
	}
	return temp1;
}
code* translate_Vardec(struct node *h){
//	printf("vardec\n");
	if(h->childnum==1){
		struct charatable* temp=search(h->child[0]->text);
		if(temp->charatype->kind==STRUCTURE){
//			printf("struct\n");
			int size=getmovenum(temp,NULL);
//			printf("getnum\n");
			code* temp1=(code*)malloc(sizeof(struct code));
			temp1->word.kind=DECSP;
			temp1->word.u.assign.left=new_Operand();
			temp1->word.u.assign.left->kind=VARIABLE;
			temp1->word.u.assign.left->u.var_name=h->child[0]->text;
			temp1->word.u.assign.right=new_Operand();
			temp1->word.u.assign.right->kind=CONSTANT;
			temp1->word.u.assign.right->u.value=size;
			return temp1;
		}
		else
			return NULL;
	}
	else{
		code* temp1=(code*)malloc(sizeof(struct code));
		temp1->word.kind=DECSP;
		temp1->word.u.assign.left=new_Operand();
		temp1->word.u.assign.left->kind=VARIABLE;
		temp1->word.u.assign.left->u.var_name=h->child[0]->child[0]->text;
		temp1->word.u.assign.right=new_Operand();
		temp1->word.u.assign.right->kind=CONSTANT;
		int ggg=myatoi(h->child[2]->text);
		temp1->word.u.assign.right->u.value=4*ggg;
		return temp1;
	}
}
code* translate_Fundec(struct node *h){
//	printf("fun\n");
	code* temp1=(code*)malloc(sizeof(struct code));
	temp1->word.kind=FUNCTION;
	temp1->word.u.ret.op1=new_Operand();
	temp1->word.u.ret.op1->kind=VARIABLE;
	temp1->word.u.ret.op1->u.var_name=h->child[0]->text;
	if(h->childnum==3)
		return temp1;
	else{
		code* temp2=translate_Varlist(h->child[2]);
		return merge(temp1,temp2);
	}
}
code* translate_Varlist(struct node *h){
//	printf("varlist\n");
	code* temp1=NULL;
	while(h->childnum==3){
		code* temp2=translate_Paramdec(h->child[0]);
		temp1=merge(temp1,temp2);
		h=h->child[2];
	}
	code* temp2=translate_Paramdec(h->child[0]);
	return merge(temp1,temp2);
}
code* translate_Paramdec(struct node *h){
//	printf("paramdec\n");
	Operand t1=new_temp();
	code* temp1=(code*)malloc(sizeof(struct code));
	temp1->word.kind=PARAM;
	temp1->word.u.ret.op1=new_Operand();
	if(h->child[1]->childnum==1){
		temp1->word.u.ret.op1->kind=VARIABLE;
		temp1->word.u.ret.op1->u.var_name=h->child[1]->child[0]->text;
		search(h->child[1]->child[0]->text)->ispara=1;
	}
	else{
		temp1->word.u.ret.op1->kind=VARIABLE;
		temp1->word.u.ret.op1->u.var_name=h->child[1]->child[0]->child[0]->text;
	}
	return temp1;
}

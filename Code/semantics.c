#include"semantics.h"
extern  Type typedistin(struct node* h);
extern struct charatable* processVardec(Type lasttype,struct node* h,int flag);
extern FieldList prodeflist(struct node* ,int i);
extern void proExtdeflist(struct node* h);
extern FieldList proArgs(struct node* h);
extern Type proExp(struct node* h);
extern void proCompst(Type restype,struct node* h);
struct charatable *head = NULL;
struct dectable *headdec=NULL;
void addtodec(int k,Type chara){
	struct dectable *temp=(struct dectable*)malloc(sizeof(struct dectable));
	temp->isdef=0;
	temp->lnum=k;
	temp->funcdeclare=chara;
	if(headdec==NULL)
		headdec=temp;
	else{
		temp->next=headdec;
		headdec=temp;
	}
}
void add(char* nam,Type chara){
	struct charatable *temp=(struct charatable*)malloc(sizeof(struct charatable));
	temp->name=nam;
	temp->charatype=chara;
	if(head==NULL)
		head=temp;
	else{
		temp->next=head;
		head=temp;
	}
}
struct dectable* searchindec(char* nam){
	for(struct dectable* p=headdec;p!=NULL;p=p->next){
		if(strcmp(p->funcdeclare->u.function->name,nam)==0)
			return p;
	}
	return NULL;
}
struct charatable* search(char* nam){
	for(struct charatable *p=head;p!=NULL;p=p->next){
		if(strcmp(p->name,nam)==0)
			return p;
		}
	return NULL;
}
int searchlist(FieldList h,char* nam){
	FieldList p=h;
	for(;p!=NULL;p=p->tail){
		if(strcmp(p->name,nam)==0)
			return 1;
	}
	return 0;
}
void semantic_analysis(struct node* h){
	if(h==NULL)
		return;
	if(strcmp(h->type,"ExtDefList")==0){
		proExtdeflist(h);
		return;
	}
	else{
	int i=0;
	for(;i<h->childnum;i++)
		semantic_analysis(h->child[i]);
	}
}
void pronotdef(){
	struct dectable* p=headdec;
	for(;p!=NULL;p=p->next){
		if(p->isdef==0)
			printf("Error type 18 at line %d: Udefined function %s\n",p->lnum,p->funcdeclare->u.function->name);
	}
}
struct charatable*  processVardec(Type lasttype,struct node* h,int flag){
	Type result=lasttype;
	while(strcmp(h->child[0]->type,"ID")!=0){
		Type temp=(Type)malloc(sizeof(struct Type_));
		temp->kind=ARRAY;
		temp->u.array.elem=result;
		temp->u.array.size=atoi(h->child[2]->text);
		result=temp;
		h=h->child[0];
	}
	if(flag==0){
		if(search(h->child[0]->text)!=NULL)
			printf("Error type 3 at lint %d: redifined variable %s\n",h->child[0]->linenum,h->child[0]->text);
		else{
			add(h->child[0]->text,result);
		}
	}
	struct charatable* temp2=(struct charatable*)malloc(sizeof(struct charatable));
	temp2->charatype=result;
	temp2->name=h->child[0]->text;
	return temp2;
}
Type typedistin(struct node* h){
	if(strcmp(h->child[0]->type,"TYPE")==0){
		Type temp=(Type)malloc(sizeof(struct Type_));
		temp->kind = BASIC;
		temp->u.basic =h->child[0]->text;
		return temp;
	}
	else{
		if(strcmp(h->child[0]->child[1]->type,"Tag")==0){
			if(search(h->child[0]->child[1]->child[0]->text)==NULL)
				 printf("Error type 17 at line %d: Undefined structure %s.\n",h->child[0]->child[1]->child[0]->linenum,h->child[0]->child[1]->child[0]->text);
			else{
				if(search(h->child[0]->child[1]->child[0]->text)->charatype->kind!=STRUCTURE)
					printf("Error type 16 at line %d:  Duplicated name %s. \n",h->child[0]->child[0]->linenum,h->child[0]->child[1]->child[0]->text);
				else{
					struct charatable* temp=search(h->child[0]->child[1]->child[0]->text);
					return temp->charatype;
				}
			}

		}
		else{
			Type addstruct=(Type)malloc(sizeof(struct Type_));
			addstruct->kind=STRUCTURE;
			addstruct->u.structure=prodeflist(h->child[0]->child[3],1);
			if(h->child[0]->child[1]->childnum==1){
				if(search(h->child[0]->child[1]->child[0]->text)==NULL){
					add(h->child[0]->child[1]->child[0]->text,addstruct);
					return addstruct;
				}
				else
					printf("Error type 16 at line %d:  Duplicated name %s. \n",h->child[0]->child[1]->child[0]->linenum,    h->child[0]->child[1]->child[0]->text);
			}
			else{
				return addstruct;
			}
		}
	}
	return NULL;
}
FieldList prodef(struct node* h,int flag) {
//	printf("st def\n");
	Type lasttype=typedistin(h->child[0]);
	h=h->child[1];
	FieldList headtemp=NULL;
	while(h->childnum>1){
		if(h->child[0]->childnum==3&&flag==1){
			printf("Error type 15 at line %d: field can not ASSIGNMENT",h->child[0]->child[1]->linenum);
			continue;
		}
		FieldList temp=(FieldList)malloc(sizeof(struct FieldList_));
		temp->name=processVardec(lasttype,h->child[0]->child[0],flag)->name;
		temp->type=lasttype;
		if(headtemp==NULL)
			headtemp=temp;
		else{
			if(searchlist(headtemp,temp->name)==0){
				temp->tail=headtemp;
				headtemp=temp;
			}
			else{
				if(flag==1)
					printf("Error type 15 at line %d: Redifined field %s\n",h->child[0]->child[0]->child[0]->linenum,temp->name);
			}
		}
		h=h->child[2];
	}
	if(h->child[0]->childnum==3&&flag==1){
		printf("Error type 15 at line %d: field can not ASSIGNMENT\n",h->child[0]->child[1]->linenum);
		return headtemp;
	}
	FieldList temp=(FieldList)malloc(sizeof(struct FieldList_));
	temp->name=processVardec(lasttype,h->child[0]->child[0],flag)->name;
	temp->type=lasttype;
	if(headtemp==NULL)
		headtemp=temp;
	else{
		if(searchlist(headtemp,temp->name)==0){
			temp->tail=headtemp;
			headtemp=temp;
		}
		else
			if(flag==1)
			printf("Error type 15 at line %d: Redifined field %s\n",h->child[0]->child[0]->child[0]->linenum,temp->name);
	}
//	printf("end def\n");
	return headtemp;
}
FieldList prodeflist(struct node* h,int flag){
//	printf("st deflist\n");
	FieldList result=NULL;
	while(h->childnum==2){
		FieldList temp=prodef(h->child[0],flag);
		FieldList q=temp;
		if(result==NULL){
			result=temp;
		}
		else{
			FieldList q=temp;
			for(;q!=NULL;q=q->tail)
				if(searchlist(result,q->name)==1){
					if(flag==1)
					printf("Error type 15 at line %d: Redifined field %s\n",h->child[0]->child[1]->child[0]->child[0]->child[0]->linenum,q->name);
					break;
				}
			if(q==NULL){
				for(q=temp;temp->tail!=NULL;)
					q=q->tail;
				q->tail=result;
				result=temp;
			}
		}
		h=h->child[1];
	}
//	printf("end deflist\n");
	return result;
}
FieldList proVarlist(struct node* h,int flag){
	FieldList result=NULL;
	FieldList last=result;
	while(h->childnum>1){
		FieldList temp1=(FieldList)malloc(sizeof(struct FieldList_));
		Type temp=typedistin(h->child[0]->child[0]);
		temp1->name=processVardec(temp,h->child[0]->child[1],flag)->name;
		temp1->type=temp;
		if(result==NULL){
			result=temp1;
			last=temp1;
		}
		else{
			if(searchlist(result,temp1->name)==1&&flag==1){
				printf("Error type 3 at line %d: redifined variable %s\n",h->child[1]->linenum,temp1->name);
				return result;
			}
			last->tail=temp1;
			last=temp1;
		}
		h=h->child[2];
	}
	FieldList temp1=(FieldList)malloc(sizeof(struct FieldList_));
	Type temp=typedistin(h->child[0]->child[0]);
	temp1->name=processVardec(temp,h->child[0]->child[1],flag)->name;
	temp1->type=temp;
	if(result==NULL){
		result=temp1;
		last=temp1;
	}
	else{
		if(searchlist(result,temp1->name)==1&&flag==1){
			printf("Error type 3 at line %d: redifined variable %s\n",h->child[0]->linenum,temp1->name);
			return result;
		}
		last->tail=temp1;
		last=temp1;
	}
	return result;
}
Type proFundec(Type lasttype,struct node* h,int flag){
	Type temp=(Type)malloc(sizeof(struct Type_));
	FieldList result=(FieldList)malloc(sizeof(struct FieldList_));
	result->name=h->child[0]->text;
	result->type=lasttype;
	if(h->childnum==4)
		if(flag==0)
			result->tail=proVarlist(h->child[2],1);
		else
			result->tail=proVarlist(h->child[2],0);
	temp->kind=FUNC;
	temp->u.function=result;
	if(flag==0){
		if(searchindec(result->name)==NULL){
			addtodec(h->child[0]->linenum,temp);
		}
		else{
			struct dectable* decnode=searchindec(result->name);
			if(equal(decnode->funcdeclare,temp)==0)
				printf("Error type 19 at line %d: Inconsistent declaration of funtion  %s\n",h->child[0]->linenum,result->name);
		}
	}
	else{
		if(search(result->name)==NULL&&searchindec(result->name)==NULL)
			add(result->name,temp);
		else if(search(result->name)==NULL&&searchindec(result->name)!=NULL){
			struct dectable* decnode=searchindec(result->name);
			if(equal(decnode->funcdeclare,temp)==0)
				printf("Error type 19 at line %d: conflict between declaration and funtion  %s\n",h->child[0]->linenum,result->name);
			else{
				add(result->name,temp);
				decnode->isdef=1;
			}
		}
		else
			printf("Error type 4 at line %d: Redifined function %s\n",h->child[0]->linenum,result->name);
	}
	return temp;
}
void proStmt(Type restype,struct node* h){
	if(h->childnum==1)
		proCompst(restype,h->child[0]);
	else if(h->childnum==2)
		proExp(h->child[0]);
	else if(h->childnum==3){
		Type temp1=proExp(h->child[1]);
		if(equal(temp1,restype)==0)
			printf("Error type 8 at line %d: Type mismatched for return\n",h->child[0]->linenum);
	}
	else if(h->childnum==5){
		Type temp1=proExp(h->child[2]);
		if(!(temp1->kind==BASIC&&strcmp(temp1->u.basic,"int")==0))
			printf("Error type 7 at line %d: Type mismatched for operands\n",h->child[0]->linenum);
		proStmt(restype,h->child[4]);
	}
	else{
		Type temp1=proExp(h->child[2]);
		if(!(temp1->kind==BASIC&&strcmp(temp1->u.basic,"int")==0))
			printf("Error type 7 at line %d: Type mismatched for operands\n",h->child[0]->linenum);
		proStmt(restype,h->child[4]);
		proStmt(restype,h->child[6]);
	}
}
void proStmtlist(Type restype,struct node* h){
	while(h!=NULL&&h->childnum==2){
		proStmt(restype,h->child[0]);
		h=h->child[1];
	}
}
void proCompst(Type restype,struct node* h){
	prodeflist(h->child[1],0);
	proStmtlist(restype,h->child[2]);
}
void proExtdef(struct node* h){
	Type lasttype=typedistin(h->child[0]);                           
	if(strcmp(h->child[1]->type,"ExtDecList")==0){
		h=h->child[1];
		while(h->childnum==3){
			processVardec(lasttype,h->child[0],0);
			h=h->child[2];
		}
		processVardec(lasttype,h->child[0],0);
	}
	else if(strcmp(h->child[1]->type,"SEMI")==0){
		//typedistin(h->child[0]);
	}
	else if(strcmp(h->child[2]->type,"SEMI")==0){
		proFundec(lasttype,h->child[1],0);
	}
	else {
		proFundec(lasttype,h->child[1],1);
		proCompst(lasttype,h->child[2]);
	}
}
void proExtdeflist(struct node* h){
	while(h->childnum==2){
		proExtdef(h->child[0]);
		h=h->child[1];
	}
}
int canassig(struct node* h){
	if(h->childnum==1&&strcmp(h->child[0]->type,"ID")==0)
		return 1;
	else if(h->childnum==4&&strcmp(h->child[0],"Exp")==0)
		return 1;
	else if(h->childnum==3&&strcmp(h->child[1],"DOT")==0)
		return 1;
	return 0;
}
int equal(Type a,Type b){
	if(a->kind!=b->kind){
		return 0;
	}
	else{
		if(a->kind==BASIC){
			if(strcmp(a->u.basic,b->u.basic)==0)
				return 1;
			else
				return 0;
		}
		else if(a->kind==ARRAY){
			if(equal(a->u.array.elem,a->u.array.elem)==0)
				return 0;
			else 
				return 1;
		}
		else if(a->kind==STRUCTURE){
			FieldList p=a->u.structure;
			FieldList q=b->u.structure;
			for(;p!=NULL&&q!=NULL;p=p->tail,q=q->tail){
				if(equal(p->type,q->type)==0||strcmp(p->name,q->name)!=0)
					return 0;
			}
			if(p==NULL&&q==NULL)
				return 1;
			else
				return 0;
		}
		else if(a->kind==FUNC){
			FieldList p=a->u.function;
			FieldList q=b->u.function;
			for(;p!=NULL&&q!=NULL;p=p->tail,q=q->tail){
				if(equal(p->type,q->type)==0)
					return 0;
			}
			if(p==NULL&&q==NULL)
				return 1;
			else
				return 0;
		}
	}
}
Type proExp(struct node* h){
	printf("h->num:%d\n",h->childnum);
	if(h->childnum==3&&strcmp(h->child[0]->type,"Exp")==0&&strcmp(h->child[2]->type,"Exp")==0){
		if(strcmp(h->child[1]->type,"ASSIGNOP")!=0){
				printf("operandtype:%s\n",h->child[1]->type);
			Type temp1=proExp(h->child[0]);
			Type temp2=proExp(h->child[2]);
			if(temp1==NULL||temp2==NULL)
				return NULL;
			if(temp1->kind!=BASIC||temp2->kind!=BASIC)
				printf("1Error type 7 at Line %d: Type mismatched for operands\n",h->child[1]->linenum);
			else if(strcmp(temp1->u.basic,temp2->u.basic)!=0)
				printf("2Error type 7 at Line %d: Type mismatched for operands\n",h->child[1]->linenum);
			else if(strcmp(temp1->u.basic,"float")==0&&(strcmp(h->child[1]->type,"AND")==0||strcmp(h->child[1]->type,"OR")==0))
				printf("3Error type 7 at Line %d: Type mismatched for operands\n",h->child[1]->linenum);
			else return temp1;
			return NULL;
		}
		else{
			if(canassig(h->child[0])==0)
				printf("Error type 6 at line %d: The left-hand side of  an assignment must be variable\n",h->child[1]->linenum);
			else{
				Type temp1=proExp(h->child[0]);
				Type temp2=proExp(h->child[2]);
				if(temp1==NULL||temp2==NULL)
					return NULL;
				if(equal(temp1,temp2)==0)
					printf("Error type 5 at line %d: Type mismatched for assignment\n",h->child[1]->linenum);
				else
					return temp1;
			}
		}
	}
	else if(h->childnum==3&&strcmp(h->child[1]->type,"Exp")==0)
		return proExp(h->child[1]);
	else if(h->childnum==2){
		if(strcmp(h->child[0]->type,"MINUS")==0){
			Type temp1=proExp(h->child[1]);
			if(temp1==NULL)
				return NULL;
			if(temp1->kind!=BASIC)
				printf("Error type 7 at Line %d: Type mismatched for operands\n",h->child[0]->linenum);
			
			else
				return temp1;
		}
		else{
			Type temp1=proExp(h->child[1]);
			if(temp1==NULL)
				return NULL;
			if(temp1->kind==BASIC&&strcmp(temp1->u.basic,"int")==0)
				return temp1;
			else
				printf("Error type 7 at Line %d: Type mismatched for operands\n",h->child[0]->linenum);
		}
	}
	else if(h->childnum==4&&strcmp(h->child[0]->type,"ID")==0){
		struct charatable* temp1=search(h->child[0]->text);
		if(temp1==NULL){
			printf("Error type 2 at line  %d: Undefined function %s\n",h->child[0]->linenum,h->child[0]->text);
			return NULL;
		}
		else if(temp1->charatype->kind!=FUNC)
			printf("Error type 11 at line %d: %s is not a function\n",h->child[0]->linenum,h->child[0]->text);
		else{
			FieldList temp2=proArgs(h->child[2]);
			if(temp2==NULL)
				return NULL;
			FieldList p=temp1->charatype->u.function->tail;
			FieldList q=temp2;
			for(;p!=NULL&&q!=NULL;p=p->tail,q=q->tail){
			//	printf("%s   %s\n",p->type->u.basic,q->type->u.basic);
				if(equal(p->type,q->type)==0){
					printf("Error type 9 at line %d: %s is not applicable for arguments\n",h->child[0]->linenum,h->child[0]->text);
					return NULL;
				}
			}
			if(p!=NULL||q!=NULL)
				printf("Error type 9 at line %d: %s is not applicable for arguments\n",h->child[0]->linenum,h->child[0]->text);
			else{
				return temp1->charatype->u.function->type;
			}
		}
	}
	else if(h->childnum==3&&strcmp(h->child[0]->type,"ID")==0){
		struct charatable* temp1=search(h->child[0]->text);
		if(temp1==NULL)
			printf("Error type 2 at line  %d: Undefined function %s\n",h->child[0]->linenum,h->child[0]->text);
		else if(temp1->charatype->kind!=FUNC)
			printf("Error type 11 at line %d: %s is not a function\n",h->child[0]->linenum,h->child[0]->text);
		else if(temp1->charatype->u.function->tail!=NULL)
			printf("Error type 9 at line %d: %s is not applicable for arguments\n",h->child[0]->linenum,h->child[0]->text);
		else
			return temp1->charatype->u.function->type;
	}
	else if(h->childnum==1){
		if(strcmp(h->child[0]->type,"ID")==0){
			struct charatable* temp1=search(h->child[0]->text);
			if(temp1==NULL){
				printf("Error type 1 at line %d: Undefined variable %s\n",h->child[0]->linenum,h->child[0]->text);
				return NULL;
			}
			else{
				if(temp1->charatype->kind!=ARRAY)
					printf("dddd %s\n",temp1->name);
				else
					printf("ddddddddddddddd\n");
				return temp1->charatype;
			}
		}
		else if(strcmp(h->child[0]->type,"INT")==0){
			Type temp1=(Type)malloc(sizeof(struct Type_));
			temp1->kind=BASIC;
			temp1->u.basic="int";
			return temp1;
		}
		else{
			Type temp1=(Type)malloc(sizeof(struct Type_));
			temp1->kind=BASIC;
			temp1->u.basic="float";
			return temp1;
		}
	}
	else if(h->childnum==4&&strcmp(h->child[0]->type,"Exp")==0){
		Type temp1=proExp(h->child[0]);
		if(temp1==NULL)
			return NULL;
		if(temp1->kind!=ARRAY){
			printf("Error type 10 at line %d: not a array",h->child[0]->linenum);
			return NULL;
		}
		else{
			Type temp2=proExp(h->child[2]);
			if(temp2==NULL)
				return NULL;
			if(!(temp2->kind==BASIC&&strcmp(temp2->u.basic,"int")==0)){
				printf("Error type 12 at line %d: not a integer",h->child[0]->linenum);
				return NULL;
			}
			else{
				printf("ArrayType: %d \n",temp1->u.array.elem);
				return temp1->u.array.elem;
			}
		}
	}
	else if(h->childnum==3&&strcmp(h->child[1],"DOT")==0){
		Type temp1=proExp(h->child[0]);
		if(temp1==NULL)
			return NULL;
		if(temp1->kind!=STRUCTURE)
			printf("Error type 13 at line %d: illegal use of .\n",h->child[1]->linenum);
		else{
			FieldList p=temp1->u.structure;
			if(searchlist(p,h->child[2]->text)==0)
				printf("Error type 14 at line %d: Non-existent field %s\n",h->child[1]->linenum,h->child[2]->text);
			else{
				for(;p!=NULL;p=p->tail)
					if(strcmp(p->name,h->child[2]->text)==0)
						break;
				return p->type;
			}
		}
	}
	return NULL;
}
FieldList proArgs(struct node* h){
	FieldList result=NULL;
	FieldList last=result;
	int i=0;
	while(h->childnum==3){
		FieldList temp1=(FieldList)malloc(sizeof(struct FieldList_));
		temp1->type=proExp(h->child[0]);
		i++;
		//printf("%s\n",temp1->type->u.basic);
		if(temp1->type==NULL)
			return NULL;
		if(result==NULL){
			result=temp1;
			last=temp1;
		}
		else{
			last->tail=temp1;
			last=temp1;
		}
		h=h->child[2];
	}
	FieldList temp1=(FieldList)malloc(sizeof(struct FieldList_));
	temp1->type=proExp(h->child[0]);
	if(temp1->type==NULL)
		return NULL;
	if(result==NULL){
		result=temp1;
		last=temp1;
	}
	else{
		last->tail=temp1;
		last=temp1;
	}
	//printf("%s\n",temp1->type->u.basic);
	//printf("%d\n",i);
	return result;
}


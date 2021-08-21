#include"ast.h"
#include<vector>
using namespace std;
#include<string.h>


int sp = 0;
enum type {a0=0, a1, a2, a3, b0, b1, b2, b3, error};
vector<string>val{"a0", "a1", "a2", "a3", "b0", "b1", "b2", "b3"};
ofstream file("lq.code.cpp");
ofstream ast("lq.ast");
static int temp=0;
int x=0;
void space(){
	for(int i=1; i<x; i++)
		cout<<"   ";
}
void AST::generateCode(){
	file<<"#include<iostream>\n"
		<<"using namespace std;\n\n"
		<<"int main{\n";
	quad->printCode();
	file<<"\n\tt"<<temp-1<<".print_quilt();\n}\n";	
	file.close();
}


void AST :: printM(){
	q.printMat();
}
void AST :: printQ(){
	generateCode();
	
}

type next(int p){
	switch(p){
		case 0 : return a1;
				break;
		case 1 : return a2;
				break;
		case 2 : return a3;
				break;
		case 3 : return a0;
				break;

		case 4 : return b1;
				break;
		case 5 : return b2;
				break;
		case 6 : return b3;
				break;
		case 7 : return b0;
				break;
		default: return error ;
				break;
	}
}
A_AST::A_AST(){
	quad = new Quadruple("a", NULL, NULL,temp);
	temp++;
}

B_AST::B_AST(){
	quad = new Quadruple("b", NULL, NULL, temp);
	temp++;
}

SEW_AST::SEW_AST(AST *l, AST *r){
	left = l;
	right = r;
	quad = new Quadruple("sew", l->quad, r->quad, temp);
	temp++;
}

TURN_AST::TURN_AST( AST *c){
	child = c;
	quad = new Quadruple("turn", child->quad, NULL,temp);
	temp++;
}



Quilt A_AST :: evaluate(){
	q.mat.resize(1);
	q.mat[0].push_back(a0);
	return q;
}



Quilt B_AST :: evaluate(){
	q.mat.resize(1);
	q.mat[0].push_back(b0);
	return q;
}

Quilt TURN_AST :: evaluate(){
	child -> evaluate();
	int r = child->q.mat.size();
	int c = child->q.mat[0].size();
	q.mat.resize(c);
	
	for(int i=r-1; i>=0; i--){
		for(int j=0; j<c; j++){
			
			q.mat[j].push_back(next(child->q.mat[i][j]));		
			
		}
	}
return q;
}

Quilt SEW_AST :: evaluate(){
	left->evaluate();
	right->evaluate();
	int lr = left->q.mat.size();
	int lc = left->q.mat[0].size();
	int rr = right->q.mat.size();
	int rc = right->q.mat[0].size();
	q.mat.resize(lr);
	for(int i=0; i<lr; i++){
		for(int j=0; j<lc; j++){
			q.mat[i].push_back(left->q.mat[i][j]);		
		}	
	}
	q.mat.resize(rr);
	for(int i=0; i<rr; i++){
	q.mat[i].resize(rc);
		for(int j=0; j<rc; j++ ){
			q.mat[i].push_back(right->q.mat[i][j]);
		}
	}
	return q;
}

void A_AST::print_ast(){
	x++;
	space();
	cout<<"(";
	cout<<"a\n";
	space();
	cout<<")\n";
	x--;
}

void B_AST::print_ast(){
	x++;
	space();
	cout<<"(";
	cout<<"b\n";
	space(); 
	cout<<")\n";
	x--;
}

void TURN_AST::print_ast(){
	x++;
	space();
	cout<<"(turn"<<endl;
	child->print_ast();
	space();
	cout<<")\n";
	x--;
}
void SEW_AST::print_ast(){
	x++;
	space();
	cout<<"(sew"<<endl;	
	left->print_ast();
	x++;
	space();
	cout<<","<<endl;
	x--;
	right->print_ast();
	space();
	cout<<")\n";
	x--;
}

LET_AST::LET_AST(AST *d, AST *e){
	decl = d;
	expr = e;
	
}

FUN_AST::FUN_AST(string fn, string t, AST *formal, AST *e){
	funName=fn;
	formals=formal;
	exp=e;
	type=t;
}

VAL_AST::VAL_AST(string n, string t, AST *e){
	name=n;
	exp=e;
	type=t;
}

CALL_AST::CALL_AST(string n, string t, AST *call){
	name=n;
	callee=call;
	type=t;
}

DECL_AST::DECL_AST(AST *e1, AST *e2){
	exp1=e1;
	exp2=e2;
}

FORMAL_AST::FORMAL_AST(string n1, AST *form){
	name1=n1;
	formal=form;
}

ACTUAL_AST::ACTUAL_AST(AST *n, AST *a){
	exp=n;
	actual=a;
}


void LET_AST::print_ast(){
	x++;
	space();
	cout<<"(LET\n";
	x++;
	space();
	cout<<"(DECL\n";
	decl->print_ast();
	space();
	cout<<")\n";
	
	space();
	cout<<",\n";
	space();
	cout<<"(IN_EXPR\n";
	
	expr->print_ast();
	space();
	cout<<")\n";
	
	x--;
	space();
	cout<<")\n";
	
	x--;
}

void FUN_AST::print_ast(){
	x++;
	space();
	cout<<"(FUN "<<funName<<endl;
	
	x++;
	space();
	cout<<"(PARAMS\n";
	
	x++;
	space();
	cout<<"( ";
	
	formals->print_ast();
	cout<<" )\n";
	x--;
	
	space();
	cout<<")\n";
	
	space();
	cout<<",\n";
	space();
	cout<<"(EXPR\n";
	
	
	
	exp->print_ast();
	
	
	
	space();
	cout<<")\n";
	x--;
	space();
	cout<<")\n";
	/*x--;
	space();
	cout<<")\n";*/
	x--;
	
}

void VAL_AST::print_ast(){
	x++;
	space();
	
	cout<<"(VAL "<<name<<" =\n";
	//cout<<quad->op<<endl;
	exp->print_ast();
	//cout<<endl;
	
	space();
	cout<<")\n";
	x--;
}

void DECL_AST::print_ast(){
	if(exp2==NULL){
		exp1->print_ast();
	}
	else{
	exp1->print_ast();
	x++;
	space();
	cout<<",\n";
	x--;
	exp2->print_ast();
	}

}

void CALL_AST::print_ast(){
	
	if(callee==NULL)
		{	
			if(type=="val"){
			x++;
			space();
			cout<<"(CALL VAL\n";
			x++;
			space();
			cout<<"( "<<name<<" )\n";
			x--;
			space();
			cout<<")\n";
			}
			else{
			x++;
				space();
				cout<<"(FORMAL PARAMETERS\n";
				x++;
				space();
				cout<<name<<endl;
				x--;
				space();
				cout<<")\n";
				//x--;
			}
		}
		
	else{
		x++;
		space();
		cout<<"( CALL FUN ";
		cout<<name<<"\n";
		x++;
		space();
		cout<<"(ACTUAL PARAMETERS\n";
		callee->print_ast();
		
		space();
		cout<<")\n";
		
		x--;
		space();
		cout<<")\n";
	}

	x--;
}

void FORMAL_AST::print_ast(){
	
	//space();
	if(formal==NULL){
		
		cout<<name1;
	}
	else
	{	
		cout<<name1<<" , ";
		formal->print_ast();
	}
	
}

void ACTUAL_AST:: print_ast(){	
	if(actual==NULL)
	{	exp->print_ast();}
	else{
		exp->print_ast();
		x++;
		space();
		cout<<",\n";
		x--;
		actual->print_ast();
	}
}

Quilt LET_AST::evaluate(){
	cout<<"LET evaluate \n";
	return q;
}

Quilt FUN_AST::evaluate(){
	cout<<"FUN evaluate \n";
	return q;
}

Quilt VAL_AST::evaluate(){
	cout<<"VAL evaluate \n";
	return q;
}

Quilt CALL_AST::evaluate(){
	cout<<"CALL evaluate \n";
	return q;
}

Quilt DECL_AST::evaluate(){
	cout<<"DECL evaluate \n";
	return q;
}

Quilt FORMAL_AST::evaluate(){
	cout<<"FORMAL evaluate \n";
	return q;
}

Quilt ACTUAL_AST::evaluate(){
	cout<<"ACTUAL evaluate\n";
	return q;
}

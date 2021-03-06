%{
	
	#include<iostream>
	#include<string>
	#include "ast.h"	
	using namespace std;
	extern "C" void yyerror(char *s);
	extern int yylex(void);
	
	AST *exp;
	SymEntry *head;
	SymEntry *start;
	void insert(string n, string t, AST *v);
	void find(string node);
%}

%union{
	AST *ast;
	string *name;
}
%token SEW TURN A B L R COM LET IN END FUN VAL EQ
%start Start 
%token <name> NAME
%type <ast> formal
%type <ast> expr
%type <ast> actuals
%type <name> SEW
%type <name> TURN
%type <ast> decl
%type <ast> decls

%%

Start:	expr 	
	{	cout<<"\n---------printing exp ast---------\n";	
		$1->print_ast();	
		cout<<"\n--------------ast printed-----------\n";	}
	/*| Start expr
	{	exp=$2;}*/
     ;
expr: SEW L expr COM expr R
    	{	//cout<<"sew!\n";
    		$$=new SEW_AST($3,$5);}
	| TURN L expr R
		{	//cout<<"turn\n";
			$$=new TURN_AST($3);}
	| A
		{	//cout<<"a\n";
			$$=new A_AST();}
	| B
		{	//cout<<"b\n";
			$$=new B_AST();}
	| LET decls IN expr END
		{//cout<<"********************let block is found------------\n";
		$$=new LET_AST($2, $4);
		}
	| NAME
		{//cout<<"name is found for cal_ast\n";
		string t;
		find(*$1);
		t=start->type;
		//cout<<"---------------------------call_ast start->type        "<<start->type<<endl;
		$$=new CALL_AST(*$1,t, NULL);}
	|NAME L actuals R
		{
		string t;
		find(*$1);
		t=start->type;
		$$=new CALL_AST(*$1,t, $3);}
		//$$ = new CALL_AST($1);
		
	;
actuals: expr
		{//cout<<"expr in actuals\n";}
		$$=new ACTUAL_AST($1, NULL);
		//insert(*$1, start->type, NULL);
		}
	| expr COM actuals
		{//cout<<"expr comma actuals\n";};
		$$=new ACTUAL_AST($1, $3);
		//insert(*$1, start->type, $3);
		};
decls : decl
		{//cout<<"decl is found\n";
		$$=new DECL_AST($1, NULL);}
	|decl decls
		{//cout<<"decl decls is found\n";
		$$=new DECL_AST($1, $2);}
	;
decl :  FUN NAME L formal R EQ expr
		{//cout<<"Fun is found with its formals\n";
		//cout<<"FUN start->type       "<<start->type<<endl;
		$$=new FUN_AST(*$2,"fun", $4, $7);
		insert(*$2, "fun", $7);
		}
	|VAL NAME EQ expr
		{//cout<<"Val is found\n";
		$$=new VAL_AST(*$2,"val", $4);
		insert(*$2, "val", $4);}
	;
formal : NAME{//cout<<"NAME IS FOUND\n";
		string t;
		//t=start->type;
		//cout<<"formal start->type           "<<start->type<<endl;
		//cout<<"name "<<*$1;
		//cout<<"  type  ";
		//cout<<start->type;
		//cout<<endl;
		$$=new FORMAL_AST(*$1, NULL);
		insert(*$1,"formal", NULL);
		//cout<<"name found 2\n";
		}
	|NAME COM formal
		{//cout<<"name with comma and formal is found\n";
		//string t;
		//t=start->type;
		//cout<<"name "<<*$1;
		//cout<<"  type  ";
		//cout<<start->type;
		//cout<<endl;
		insert(*$1,"formal",$3);
		$$=new FORMAL_AST(*$1, $3);
		
		}
		;
%%
void insert(string n, string t, AST *v){
	SymEntry *node = new SymEntry(n,t,v);
	if(head==NULL){
		head=node;
		start=head;
	}
	else
	{
		while(start->next!=NULL){
			start=start->next;
		}
		start->next = node;
		start=start->next;
	}
	//cout<<start->type<<endl;
}

void find(string node){
	//cout<<"find  start->type    "<<start->type<<endl;
	SymEntry *p=head;
	while(p->next!=NULL){
		if(p->name==node){
			//cout<<"find___NODE   "<<node<<endl;
			start=p;
			//cout<<"found the type??????? "<<start->type<<endl;
			break;
		}
	p=p->next;
	}
}
int main(){
	yyparse();
	//cout<<"MAIN start->type         "<<start->type<<endl;
	//exp->print_ast();	
	
	/*exp->evaluate();
	exp->printM();*/
return 0;
}

#include<iostream>

#include<fstream>
#include<vector>
using namespace std;
#include<string>

class Quilt;
class Quadruple;


class Quilt{
	public:
		vector<vector <int> > mat;
		void printMat();
};

class Quadruple{
	public:
	
	string op="";
	Quadruple *op1=NULL;
	Quadruple *op2=NULL;
	int res = 0;
	void printCode();
	
	Quadruple(string _op, Quadruple *_op1, Quadruple *_op2, int _res);
	
};
class AST{
	public:
		Quilt q;
		Quadruple *quad;
		void generateCode();
		//void printCode()=0;
		virtual void print_ast()=0;
		virtual Quilt evaluate()=0;
		void printM();
		void printQ();
};

class LET_AST: public AST{
	protected:
		AST *decl;
		AST *expr;
	public:
		LET_AST(AST *d, AST *e);
		void print_ast();
		Quilt evaluate();
};


class FORMAL_AST: public AST{
	protected:
		string name1;
		AST *formal;
		
	public:
		
		FORMAL_AST(string n1, AST *form);
		void print_ast();
		Quilt evaluate();
};

class ACTUAL_AST: public AST{
	protected:
		AST *exp;
		AST *actual;
	public:
		ACTUAL_AST(AST *n, AST *a);
		void print_ast();
		Quilt evaluate();		
};

class FUN_AST: public AST{
	protected:
		string funName;
		AST *formals;
		AST *exp;
		string type;
	public:
		FUN_AST(string fn, string t, AST *formal, AST *e);
		void print_ast();		
		Quilt evaluate();
};

class VAL_AST: public AST{
	protected:
		string name, type;
		AST *exp;
	public:
		VAL_AST(string n, string t, AST *e);
		void print_ast();
		Quilt evaluate();
};


class CALL_AST: public AST{
	protected:
		string name;
		AST *callee;
		string type;
	public:
		CALL_AST(string n, string t, AST *call);
		void print_ast();
		Quilt evaluate();
};


class DECL_AST: public AST{
	protected:
		AST *exp1, *exp2;;
	public:
		DECL_AST(AST *e1, AST *e2);
		void print_ast();
		Quilt evaluate();
};




class A_AST: public AST{
	public:
		A_AST();
		void print_ast();
		Quilt evaluate();
		void printCode();
};
class B_AST: public AST{
	public:
		B_AST();
		void print_ast();
		Quilt evaluate();
		void printCode();
};
class SEW_AST: public AST{
	protected:
		//string name ;
		AST *left, *right;
	public:
		SEW_AST(AST *l, AST *r);
		void print_ast();
		Quilt evaluate();
		void printCode();
};
class TURN_AST: public AST{
	protected:
		//string name;
		AST *child;
	public:
		void printCode();
		TURN_AST(AST *c);
		void print_ast();
		Quilt evaluate();
};


class SymEntry{
	public:
		string name, type;
		AST *value;
		SymEntry *next;

		SymEntry(){
			next=NULL;
		}
		SymEntry(string n, string t, AST *v){
			name=n;
			type=t;
			value=v;
		}
};

#include "parserInterp.h"
//Working Correct Test Cases 1,2,3,4,5,6,7,8,9,10,11,12,13,15

map<string, bool> defVar;
map<string, Token> SymTable;

map<string, Value> TempsResults; //Container of temporary locations of Value objects for results of expressions, variables values and constants 
queue <Value> * ValQue; //declare a pointer variable to a queue of Value objects

namespace Parser {
	bool pushed_back = false;
	LexItem	pushed_token;

	static LexItem GetNextToken(istream& in, int& line) {
		if( pushed_back ) {
			pushed_back = false;
			return pushed_token;
		}
		return getNextToken(in, line);
	}

	static void PushBackToken(LexItem & t) {
		if( pushed_back ) {
			abort();
		}
		pushed_back = true;
		pushed_token = t;	
	}

}

static int error_count = 0;

int ErrCount()
{
    return error_count;
}

void ParseError(int line, string msg)
{
	++error_count;
	cout << line << ": " << msg << endl;
}

//Prog ::= PROGRAM IDENT ; DeclPart CompoundStmt .
bool Prog(istream& in, int& line)
{
	LexItem t;

	t = Parser::GetNextToken(in, line); //Check for PROGRAM keyword
	if (t != PROGRAM)
	{
		ParseError(line, "Missing PROGRAM Keyword.");
		return false;
	}
	t = Parser::GetNextToken(in, line); //Check for Program Name 
	if (t != IDENT){
		ParseError(line, "Missing Program Name.");
		return false;
	}
	t = Parser::GetNextToken(in, line); //check for the following semicolon
	if (t != SEMICOL){
		ParseError(line, "Missing Semicolon.");
		return false;
	}

	bool declP = DeclPart(in, line); 
	if (!declP)
	{
		ParseError(line, "Incorrect Declaration Section.");
		return false;
	}
	bool CStmt = CompoundStmt(in, line);
	if (!CStmt)
	{
		ParseError(line, "Missing Compound Statement.");
		return false;
	}
	t = Parser::GetNextToken(in, line); //Check for Termination dot
	if(t != DOT){
		ParseError(line, "Missing Final Termination Dot.");
		return false;
	}
	return true; //Parse Success
}//End of Prog

//DeclPart ::= VAR DeclStmt; {	DeclStmt ; }
bool DeclPart(istream& in, int& line)
{
	//VAR
	LexItem t = Parser::GetNextToken(in, line);
	if(t != VAR)
	{
		ParseError(line, "Missing Var Declaration Section.");
		return false;
	}

	//DeclStmt
	bool status = DeclStmt(in, line);
	while (status)
	{
		t = Parser::GetNextToken(in, line);
		if (t == BEGIN)
		{
			Parser::PushBackToken(t);
			return true;
		}
		if (t != SEMICOL)
		{
			ParseError(line, "Missing Semicolon Declaration Section.");
			return false;
		}

		status = DeclStmt(in, line);
	}

	if(!status)
	{
		ParseError(line, "Unrecognizeable Declaration Section.");
		return false;
	}

	return true;
}//End of DeclPart

//DeclStmt ::= IDENT {, IDENT } : Type [:= Expr]
bool DeclStmt(istream& in, int& line)
{
	bool status = false;
	Value retVal;
	LexItem t = Parser::GetNextToken(in, line);
	if (t == BEGIN)
	{
		Parser::PushBackToken(t);
		return true;
	}
	else if (t != IDENT)
	{
		ParseError(line, "Unrecognizeable Variable Declaration Statement.");
		return false;
	}
	else if (defVar[t.GetLexeme()])
	{
		ParseError(line, "Variable Redefinition.");
		ParseError(line, "Incorrect identifiers list in Declaration Statement.");
		return false;
	}
	defVar[t.GetLexeme()] = true;
	queue<string> idents;
	idents.push(t.GetLexeme());
	bool rep = true;
	while (rep)
	{
		t = Parser::GetNextToken(in, line);

		if (t != COMMA)
		{
			Parser::PushBackToken(t);
			rep = false;
			break;
		}
		t = Parser::GetNextToken(in, line);
		if (t != IDENT)
		{
			ParseError(line, "Unrecognizeable Variable Declaration Statement.");
			return false;
		}
		else if (defVar[t.GetLexeme()])
		{
			ParseError(line, "Variable Redefinition.");
			ParseError(line, "Incorrect identifiers list in Declaration Statement.");
			return false;
		}
		defVar[t.GetLexeme()] = true;
		idents.push(t.GetLexeme());
	}
	
	t = Parser::GetNextToken(in, line);
	if (t != COLON)
	{
		ParseError(line, "Missing Colon in Declaration Statement.");
		return false;
	}

	t = Parser::GetNextToken(in, line);
	if (t != INTEGER && t != REAL && t != BOOLEAN && t != STRING)
	{
		ParseError(line, "Unrecognized Type in Declaration Statement.");
		return false;
	}
	Token type = t.GetToken();

	t = Parser::GetNextToken(in, line);
	if (t == ASSOP)
	{
		status = Expr(in, line, retVal);
		if (!status)
		{
			ParseError(line, "Unrecognized Expression in Declaration Statement.");
			return false;
		}
		else if (retVal.IsErr())
		{
			ParseError(line, "Invalid Assignment Value in Declaration Statement.");
			return false;
		}
		while(!idents.empty())
		{
			SymTable[idents.front()] = type;
			TempsResults[idents.front()] = retVal;
			idents.pop();
		}
		return true;
	}
	else{
		while(!idents.empty())
		{
			SymTable[idents.front()] = type;
			idents.pop();
		}
		Parser::PushBackToken(t);
		return true;
	}
	return status;
}//End of DeclStmt

//Stmt ::= SimpleStmt | StructuredStmt
bool Stmt(istream& in, int& line)
{
	bool status = false;
	LexItem t = Parser::GetNextToken(in, line);
	if (t == BEGIN || t == IF)
	{ //StructuredStmt
		Parser::PushBackToken(t);
		status = StructuredStmt(in, line);
		if (!status)
		{
			ParseError(line, "Invalid StructuredStmt.");
			return false;
		}
		return status;
	}
	else if (t == IDENT || t == WRITE || t == WRITELN)
	{ //SimpleStmt
		Parser::PushBackToken(t);
		status = SimpleStmt(in, line);
		if (!status)
		{
			ParseError(line, "Invalid SimpleStmt.");
			return false;
		}
		return status;
	}
	return true;
}//End of Stmt

//StructuredStmt ::= IfStmt | CompoundStmt
bool StructuredStmt(istream& in, int& line)
{
	bool status = false;
	LexItem t = Parser::GetNextToken(in, line);
	if (t == IF)
	{//IfStmt
		Parser::PushBackToken(t);
		status = IfStmt(in, line);
		if (!status)
		{
			ParseError(line, "Error IfStmt.");
			return false;
		}
		return status;
	}
	else if (t == BEGIN)
	{//CompoundStmt
		Parser::PushBackToken(t);
		status = CompoundStmt(in, line);
		if (!status)
		{
			ParseError(line, "Error CompoundStmt.");
			return false;
		}
		return status;
	}
	Parser::PushBackToken(t);
	return false;
}//End of StructuredStmt

//CompoundStmt ::= BEGIN Stmt {; Stmt } END
bool CompoundStmt(istream& in, int& line)
{
	LexItem t = Parser::GetNextToken(in, line);
	if (t != BEGIN)
	{
		ParseError(line, "Unrecognized Compound Statement.");
		return false;
	}

	bool status = Stmt(in, line);
	if (!status)
	{
		ParseError(line, "Incorrect statement in Compound Statement.");
		return false;
	}

	bool rep = true;
	while(rep)
	{
		t = Parser::GetNextToken(in, line);
		
		if (t != SEMICOL)
		{
			Parser::PushBackToken(t);
			rep = false;
			break;
		}

		status = Stmt(in, line);
		if (!status)
		{
			ParseError(line, "Incorrect statement in Compound Statement.");
			return false;
		}
		
	}

	t = Parser::GetNextToken(in, line);
	if (t != END)
	{
		ParseError(line, "Missing END keyword in Compound Statement.");
		return false;
	}
	
	return true;
}//End of CompoundStmt

//SimpleStmt ::= AssignStmt | WriteLnStmt | WriteStmt
bool SimpleStmt(istream& in, int& line)
{
	bool status = false;
	LexItem t = Parser::GetNextToken(in , line);

	if (t == IDENT)
	{//AssignStmt
		Parser::PushBackToken(t);
		status = AssignStmt(in, line);
		if (!status)
		{
			ParseError(line, "Error AssignStmt.");
			return false;
		}
		return status;
	}

	else if (t == WRITE)
	{
		//Parser::PushBackToken(t);
		status = WriteStmt(in, line);
		if (!status)
		{
			ParseError(line, "Error WriteStmt.");
			return false;
		}
		return status;
	}

	else if (t == WRITELN)
	{
		//Parser::PushBackToken(t);
		status = WriteLnStmt(in, line);
		if (!status)
		{
			ParseError(line, "Error WriteLnStmt.");
			return false;
		}
		return status;
	}
	Parser::PushBackToken(t);
	return status;
}//End of SimpleStmt

//WriteStmt ::= WRITE (ExprList)
bool WriteStmt(istream& in, int& line)
{
	LexItem t;
	//cout << "in WriteStmt" << endl;
	ValQue = new queue<Value>;
	
	t = Parser::GetNextToken(in, line);
	if( t != LPAREN ) {
		
		ParseError(line, "Missing Left Parenthesis");
		return false;
	}
	
	bool ex = ExprList(in, line);
	
	if( !ex ) {
		ParseError(line, "Missing expression list for WriteLn statement");
		return false;
	}
	
	t = Parser::GetNextToken(in, line);
	if(t != RPAREN ) {
		
		ParseError(line, "Missing Right Parenthesis");
		return false;
	}
	
	//Evaluate: print out the list of expressions' values
	while (!(*ValQue).empty())
	{
		Value nextVal = (*ValQue).front();
		cout << nextVal;
		ValQue->pop();
	}
	return ex;
}//End of Write Stmt

//IfStmt ::= IF Expr THEN Stmt [ ELSE Stmt ]
bool IfStmt(istream& in, int& line)
{
	Value retVal;
	LexItem t = Parser::GetNextToken(in, line);
	if (t != IF)
	{
		ParseError(line, "Missing IF keyword.");
		return false;
	}

	bool status = Expr(in, line, retVal);
	if (!status)
	{
		ParseError(line, "Invalid Expr in IfStmt.");
		return false;
	}
	else if(!retVal.IsBool())
	{
		ParseError(line, "Invalid Condition in IfStmt.");
		return false;
	}

	t = Parser::GetNextToken(in, line);
	if (t != THEN)
	{
		ParseError(line, "Missing THEN keyword.");
		return false;
	}

	//implement skipping
	if (retVal.GetBool())
	{
		status = Stmt(in, line);
		if (!status)
		{
			ParseError(line, "Invalid Stmt in IfStmt.");
			return false;
		}
	}
	else
	{
		t = Parser::GetNextToken(in, line);
		if (t == BEGIN)
		{
			while (t != END)
			{
				t = Parser::GetNextToken(in, line);
			}
		}
		else
		{
			while (t != SEMICOL)
			{
				t = Parser::GetNextToken(in, line);
			}
		}
	}
	

	t = Parser::GetNextToken(in, line);
	if (t == ELSE && !retVal.GetBool())
	{
		status = Stmt(in, line);
		if (!status)
		{
			ParseError(line, "Invalid Stmt in IfStmt Else.");
			return false;
		}
		return true;
	}
	else if (t == ELSE && retVal.GetBool())
	{
		t = Parser::GetNextToken(in, line);
		if (t == BEGIN)
		{
			while (t != END)
			{
				t = Parser::GetNextToken(in, line);
			}
		}
		else
		{
			while (t != SEMICOL)
			{
				t = Parser::GetNextToken(in, line);
			}
		}
	}
	else
	{
		Parser::PushBackToken(t);
		return true;
	}
	return status;
}

//AssignStmt ::= Var := Expr
bool AssignStmt(istream& in, int& line)
{
	Value retVal;
	LexItem idtok;
	bool status = Var(in, line, idtok);
	if (!status)
	{
		ParseError(line, "Invalid Variable in Assignment.");
		return false;
	}

	LexItem t = Parser::GetNextToken(in, line);
	if (t != ASSOP)
	{
		ParseError(line, "Missing Assignment operator.");
		return false;
	}

	status = Expr(in, line, retVal);
	if (!status)
	{
		ParseError(line, "Invalid Expression in Assignment.");
		return false;
	}
	else if (((SymTable[idtok.GetLexeme()] == BOOLEAN) && !retVal.IsBool()) ||
			 ((SymTable[idtok.GetLexeme()] == STRING) && !retVal.IsString()))//asignment type match
	{
		ParseError(line, "Illegal Mixed Assignment.");
		return false;
	}
	else if((SymTable[idtok.GetLexeme()] == INTEGER) && retVal.IsReal())
	{
		TempsResults[idtok.GetLexeme()] = Value((int)retVal.GetReal());
	}
	else if((SymTable[idtok.GetLexeme()] == REAL) && retVal.IsInt())
	{
		TempsResults[idtok.GetLexeme()] = Value((double)retVal.GetInt());
	}
	else
	{
		TempsResults[idtok.GetLexeme()] = retVal;
	}
	return status;
}

//Var ::= IDENT
bool Var(istream& in, int& line, LexItem & idtok)
{
	idtok = Parser::GetNextToken(in, line);
	if (idtok == IDENT)
	{
		if (!(defVar[idtok.GetLexeme()]))
		{
			ParseError(line, "Undeclared Variable.");
			return false;
		}
		return true;
	}
	else
	{
		ParseError(line, "Error in Var.");
		return false;
	}
	return false;
}

//Expr ::= LogOrExpr ::= LogAndExpr { OR LogAndExpr }
bool Expr(istream& in, int& line, Value & retVal)
{
	Value val1;
	bool status = LogANDExpr(in, line, retVal);
	if (!status)
	{
		ParseError(line, "Unrecognized OR Expression.");
		return false;
	}

	LexItem t = Parser::GetNextToken(in, line);
	while (t == OR)
	{
		status = LogANDExpr(in, line, val1);
		if (!status)
		{
			ParseError(line, "Unrecognized OR Expression.");
			return false;
		}
		
		retVal = retVal || val1;

		if (retVal.IsErr())
		{
			ParseError(line, "Illegal OR Operation.");
			return false;
		}

		t = Parser::GetNextToken(in, line);
	}
	Parser::PushBackToken(t);
	return true;
}//End of Expr

//LogAndExpr ::= RelExpr {AND RelExpr }
bool LogANDExpr(istream& in, int& line, Value & retVal)
{
	Value val1;
	bool status = RelExpr(in, line, retVal);
	if (!status)
	{
		ParseError(line, "Unrecognized AND Expression.");
		return false;
	}

	LexItem t = Parser::GetNextToken(in, line);
	while (t == AND)
	{
		status = RelExpr(in, line, val1);
		if (!status)
		{
			ParseError(line, "Unrecognized AND Expression.");
			return false;
		}
		
		retVal = retVal && val1;

		if (retVal.IsErr())
		{
			ParseError(line, "Illegal AND Operation.");
			return false;
		}

		t = Parser::GetNextToken(in, line);
	}
	Parser::PushBackToken(t);
	return true;
}//End of LogAndExpr

//RelExpr ::= SimpleExpr [ ( = | < | > ) SimpleExpr ]
bool RelExpr(istream& in, int& line, Value & retVal)
{
	Value val1;
	bool status = SimpleExpr(in, line, retVal);
	if (!status)
	{
		ParseError(line, "Error RelExpr.");
		return false;
	}

	LexItem t = Parser::GetNextToken(in, line);
	if (t == EQ || t == LTHAN || t == GTHAN)
	{
		status = SimpleExpr(in, line, val1);
		if (!status)
		{
			ParseError(line, "Incomplete Relation Expression.");
			return false;
		}
		else if (t == EQ)
		{
			retVal = retVal == val1;
		}
		else if (t == LTHAN)
		{
			retVal = retVal < val1;
		}
		else if (t == GTHAN)
		{
			retVal = retVal > val1;
		}
		if(retVal.IsErr()){
			ParseError(line, "Illegal operation in RelExpr.");
			return false;
		}
		return true;
	}
	else
	{
		Parser::PushBackToken(t);
	}

	if(retVal.IsErr()){
		ParseError(line, "Illegal operation in RelExpr.");
		return false;
	}
	return true;
}// End of RelExpr

//SimpleExpr :: Term { ( + | - ) Term }
bool SimpleExpr(istream& in, int& line, Value & retVal)
{
	Value val1;
	bool status = Term(in, line, retVal);
	if (!status)
	{
		ParseError(line, "Unrecognized Term.");
		return false;
	}

	LexItem t = Parser::GetNextToken(in, line);
	while (t == PLUS || t == MINUS)
	{
		bool status = Term(in, line, val1);
		if (!status)
		{
			ParseError(line, "Unrecognized Signed Factor.");
			return false;
		}
		if (t == PLUS) {
			retVal = retVal + val1;
		}
		else if (t == MINUS) {
			retVal = retVal - val1;
		}
		if(retVal.IsErr()){
			ParseError(line, "Illegal operation in SimpleExpr.");
			return false;
		}
		t = Parser::GetNextToken(in, line);
	}
	if (retVal.IsErr())
	{
		ParseError(line, "Illegal operation in SimpleExpr.");
		return false;
	}

	Parser::PushBackToken(t);
	return true;
}//End of SimpleExpr

//Term ::= SFactor { ( * | / | DIV | MOD ) SFactor }
bool Term(istream& in, int& line, Value & retVal)
{
	Value val1;
	bool status = SFactor(in, line, retVal);
	if (!status)
	{
		ParseError(line, "Unrecognized Signed Factor.");
		return false;
	}

	LexItem t = Parser::GetNextToken(in, line);
	while (t == MULT || t == DIV || t == IDIV || t == MOD)
	{
		bool status = SFactor(in, line, val1);
		if (!status)
		{
			ParseError(line, "Unrecognized Signed Factor.");
			return false;
		}
		if (t == MULT) {
			retVal = retVal * val1;
		}
		else if (t == DIV) {
			retVal = retVal / val1;
		}
		else if (t == IDIV) {
			retVal = retVal.div(val1);
		}
		else if (t == MOD)
		{
			retVal = retVal % val1;
		}
		if(retVal.IsErr()){
			ParseError(line, "Illegal operation in Term.");
			return false;
		}
		t = Parser::GetNextToken(in, line);

	}
	if (retVal.IsErr())
	{
		ParseError(line, "Illegal operation in Term.");
		return false;
	}
	Parser::PushBackToken(t);
	return true;

}//End of Term

//SFactor ::= [( - | + | NOT )] Factor
bool SFactor(istream& in, int& line, Value & retVal)
{
	bool status;
	LexItem t = Parser::GetNextToken(in, line);
	int sign = 1;
	if (t == MINUS)
	{
		sign = -1;
	}
	else if (t == NOT)
	{
		sign = 2;
	}
	else if (t != PLUS)
	{
		sign = 0;
		Parser::PushBackToken(t);
	}

	status = Factor(in, line, retVal, sign);
	if (!status)
	{
		ParseError(line, "Unrecognized Factor.");
		return false;
	}
	return true;
}//End of SFactor

//Factor ::= IDENT | ICONST | RCONST | SCONST | BCONST | (Expr)
bool Factor(istream& in, int& line, Value & retVal, int sign)
{
	bool status;
	LexItem t = Parser::GetNextToken(in, line);
	if (t == LPAREN)
	{
		status = Expr(in, line, retVal);
		if (!status)
		{
			ParseError(line, "Invalid Expression.");
			return false;
		}

		t = Parser::GetNextToken(in, line);
		if (t != RPAREN)
		{
			ParseError(line, "Missing Right Parenthesis.");
			return false;
		}
		return true;
	}
	else if (t != IDENT && t != ICONST && t != RCONST && t != SCONST && t != BCONST)
	{
		ParseError(line, "Invalid Factor.");
		return false;
	}
	else if (t == IDENT)
	{
		if (!defVar[t.GetLexeme()])
		{
			ParseError(line, "Undeclared Variable in Factor.");
			return false;
		}
		else if (TempsResults.find(t.GetLexeme()) == TempsResults.end())
		{
			ParseError(line, "Unassigned Variable in Factor.");
			return false;
		}
		else if (sign != 0)
		{
			if (sign == 1)
			{
				if (TempsResults[t.GetLexeme()].IsBool() || TempsResults[t.GetLexeme()].IsString())
				{
					ParseError(line, "Invalid Variable Sign in Factor.");
					return false;
				}
				else if (TempsResults[t.GetLexeme()].IsReal())
				{
					retVal = Value(fabs(TempsResults[t.GetLexeme()].GetReal()));
					return true;
				}
				else
				{
					retVal = Value(abs(TempsResults[t.GetLexeme()].GetInt()));
					return true;
				}
			}
			else if (sign == -1)
			{
				if (TempsResults[t.GetLexeme()].IsBool() || TempsResults[t.GetLexeme()].IsString())
				{
					ParseError(line, "Invalid Variable Sign in Factor.");
					return false;
				}
				else if (TempsResults[t.GetLexeme()].IsReal())
				{
					retVal = Value(-(TempsResults[t.GetLexeme()].GetReal()));
					return true;
				}
				else
				{
					retVal = Value(-(TempsResults[t.GetLexeme()].GetInt()));
					return true;
				}
			}
			else if (sign == 2)
			{
				if (TempsResults[t.GetLexeme()].IsInt() || TempsResults[t.GetLexeme()].IsString() || TempsResults[t.GetLexeme()].IsReal())
				{
					ParseError(line, "Invalid Variable Sign in Factor.");
					return false;
				}
				else if (TempsResults[t.GetLexeme()].IsBool())
				{
					retVal = Value(!(TempsResults[t.GetLexeme()].GetBool()));
					return true;
				}
			}
		}
		retVal = Value(TempsResults[t.GetLexeme()]);
		return true;
	}
	else if (t == ICONST)
	{
		if (sign == 1)
		{
			retVal = Value(fabs(stod(t.GetLexeme())));
			return true;
		}
		else if (sign == -1)
		{
			retVal = Value(-(stod(t.GetLexeme())));
			return true;
		}
		else if (sign == 2)
		{
			ParseError(line, "Invalid Variable Sign in Factor.");
			return false;
		}
		retVal = Value(stoi(t.GetLexeme()));
		return true;
	}
	else if (t == RCONST)
	{
		if (sign == 1)
		{
			retVal = Value(fabs(stod(t.GetLexeme())));
			return true;
		}
		else if (sign == -1)
		{
			retVal = Value(-(stod(t.GetLexeme())));
			return true;
		}
		else if (sign == 2)
		{
			ParseError(line, "Invalid Variable Sign in Factor.");
			return false;
		}
		retVal = Value(stod(t.GetLexeme()));
		return true;
	}
	else if (t == BCONST || t == TRUE || t == FALSE)
	{
		if (sign == 1 || sign == -1)
		{
			ParseError(line, "Unassigned Variable in Factor.");
			return false;
		}
		if (t.GetLexeme() == "true" || t == TRUE)
		{
			if (sign == 2)
			{
				retVal = Value(false);
				return true;
			}
			retVal = Value(true);
			return true;
		}
		else
		{
			if (sign == 2)
			{
				retVal = Value(true);
				return true;
			}
			retVal = Value(false);
			return true;
		}
	}
	else if (t == SCONST)
	{
		if (sign != 0)
		{
			ParseError(line, "Incorrect Sign Variable Type in Factor.");
			return false;
		}
		retVal = Value(t.GetLexeme());
		return true;
	}
	else
	{
		retVal = Value();
		return true;
	}

}//End of Factor

//WriteLnStmt ::= writeln (ExprList) 
bool WriteLnStmt(istream& in, int& line) {
	LexItem t;
	//cout << "in WriteStmt" << endl;
	ValQue = new queue<Value>;
	
	t = Parser::GetNextToken(in, line);
	if( t != LPAREN ) {
		
		ParseError(line, "Missing Left Parenthesis");
		return false;
	}
	
	bool ex = ExprList(in, line);
	
	if( !ex ) {
		ParseError(line, "Missing expression list for WriteLn statement");
		return false;
	}
	
	t = Parser::GetNextToken(in, line);
	if(t != RPAREN ) {
		
		ParseError(line, "Missing Right Parenthesis");
		return false;
	}
	
	//Evaluate: print out the list of expressions' values
	while (!(*ValQue).empty())
	{
		Value nextVal = (*ValQue).front();
		cout << nextVal;
		ValQue->pop();
	}
	cout << endl;
	return ex;
}//End of WriteLnStmt

//ExprList:= Expr {,Expr}
bool ExprList(istream& in, int& line) {
	bool status = false;
	Value retVal;
	//cout << "in ExprList and before calling Expr" << endl;
	status = Expr(in, line, retVal);
	if(!status){
		ParseError(line, "Missing Expression");
		return false;
	}
	ValQue->push(retVal);
	LexItem tok = Parser::GetNextToken(in, line);
	
	if (tok == COMMA) {
		//cout << "before calling ExprList" << endl;
		status = ExprList(in, line);
		//cout << "after calling ExprList" << endl;
	}
	else if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	else{
		Parser::PushBackToken(tok);
		return true;
	}
	return status;
}//ExprList


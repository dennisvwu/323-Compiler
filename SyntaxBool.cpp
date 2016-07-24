
//-------------------------------------------------------------------
//	Name:		Dennis Wu
//	Project:	Syntax Analyzer for RAT16S
//	Due:		May 6, 2016
//	Course:		CPSC 323 Compilers
//
//	This program generates a tokens.txt from the Lexer function which
//  reads from the user created code file (code.txt). The Syntax 
//  function then prases the tokens/lexeme from the tokens.txt file
//  and generates a symbol table and assembly code into a file 
//  called assembly.txt
// 
//-------------------------------------------------------------------
// Current limitations:
// 
//-------------------------------------------------------------------

#include<iostream>
#include<fstream>
#include<string>

#include "Lexer.h"

using namespace std;

string token = "";
string lexeme = "";

int maxToken;
int pointer = -1;

bool debugmode = true;

ofstream outputFile("syntax.txt");

void NextToken(string tokenArray[], string lexemeArray[]);

void WriteSyntax(string tokenArray[], string lexemeArray[]);
void ErrorQuit(string errorMsg);

bool rat16s(string tokenArray[], string lexemeArray[], int pointer);

bool OptFunctionDefinitions(string tokenArray[], string lexemeArray[], int pointer);
bool FunctionDefinitions(string tokenArray[], string lexemeArray[], int pointer);
bool Function(string tokenArray[], string lexemeArray[], int pointer);

bool Identifier();

bool OptParameterList(string tokenArray[], string lexemeArray[], int pointer);
bool ParameterList(string tokenArray[], string lexemeArray[], int pointer);
bool Parameter(string tokenArray[], string lexemeArray[], int pointer);

bool Qualifier(string tokenArray[], string lexemeArray[], int pointer);
bool Body(string tokenArray[], string lexemeArray[], int pointer);

bool OptDeclarationList(string tokenArray[], string lexemeArray[], int pointer);
bool DeclarationList(string tokenArray[], string lexemeArray[], int pointer);
bool Declaration(string tokenArray[], string lexemeArray[], int pointer);

bool IDs(string tokenArray[], string lexemeArray[], int pointer);

bool StatementList(string tokenArray[], string lexemeArray[], int pointer);
bool Statement(string tokenArray[], string lexemeArray[], int pointer);

bool Compound(string tokenArray[], string lexemeArray[], int pointer);
bool Assign(string tokenArray[], string lexemeArray[], int pointer);
bool If(string tokenArray[], string lexemeArray[], int pointer);
bool Return(string tokenArray[], string lexemeArray[], int pointer);
bool Write(string tokenArray[], string lexemeArray[], int pointer);
bool Read(string tokenArray[], string lexemeArray[], int pointer);
bool While(string tokenArray[], string lexemeArray[], int pointer);

bool Condition(string tokenArray[], string lexemeArray[], int pointer);
bool Relop(string tokenArray[], string lexemeArray[], int pointer);

bool Expression(string tokenArray[], string lexemeArray[], int pointer);
bool ExpressionPrime(string tokenArray[], string lexemeArray[], int pointer);

bool Term(string tokenArray[], string lexemeArray[], int pointer);
bool TermPrime(string tokenArray[], string lexemeArray[], int pointer);

bool Factor(string tokenArray[], string lexemeArray[], int pointer);
bool Primary(string tokenArray[], string lexemeArray[], int pointer);

bool Empty(string tokenArray[], string lexemeArray[], int pointer);

int main()
{
	// Start Lexer
	Lexer();

	maxToken = GetTokenCount();//-1;

	// Debug Mode
	string debug;

	cout << "Print Production Rules? Y/N: ";
	cin >> debug;

	if (debug == "N" | debug == "n")
		debugmode = false;
	// Debug Mode

	//test
	if (debugmode){ cout << "\n[debug] Token Count: " << maxToken-1 << endl; }
	//test

	string* tokenArray = new string[maxToken];	
	string* lexemeArray = new string[maxToken];

	// Open Token.txt file
	ifstream inputFile;
	inputFile.open ("tokens.txt");

	// REMOVE
	cout << "\n============== Array Table ================" << endl;
	// REMOVE

	// Save Tokens & Lexemes into an array
	for (int i = 0; i < maxToken; i++)
	{
		inputFile >> token >> lexeme;

		tokenArray[i] = token;
		lexemeArray[i] = lexeme;

		// REMOVE
		cout << "Token[" << i << "]: " << token << "      Lexeme[" << i << "]: " << lexeme << endl;
		// REMOVE
	}
	cout << "\nNo more tokens\n";

	inputFile.close();

	// Start Syntax Analyizer	
	if (rat16s(tokenArray, lexemeArray, pointer))
	{ 
		cout << "\nSyntax Analyizer Sucessful" << endl; 
		cout << "Syntax Analysis Complete - See output file 'syntax.txt' \n" << endl;

		if (debugmode){ 
			outputFile << "\n=======================================================================\n";
			outputFile << "Syntax Analyizer Sucessful" << endl; 
			outputFile << "=======================================================================\n"; } 
	}
	else 
	{
		cout << "Syntax Analyizer Error" << endl;
		if (debugmode){ outputFile << "Error with the syntax, expected <Rat16S>" << endl; } 
	}

	// Close files
	outputFile.close();

	// Delete arrays
	delete[] tokenArray;
	delete[] lexemeArray;

	// Terminate program
	system("pause");
	return 0;
}

void NextToken(string tokenArray[], string lexemeArray[])
{
	if ((pointer+1) < maxToken)
	{	
		// Go to next data pair
		pointer = pointer + 1;

		token = tokenArray[pointer]; 
		lexeme = lexemeArray[pointer];
	}
}


bool Identifier()
{
	if (token == "identifier")
		return true;
	else
		return false;
}

void WriteSyntax(string tokenArray[], string lexemeArray[])
{ 
	
	// REMOVE
	if (debugmode){ 
		outputFile << "\n=======================================================================\n";
		outputFile << "#" << pointer << " Token: " << token << "          Lexeme: " << lexeme << endl; 
		outputFile << "=======================================================================\n"; } 
	// REMOVE

}

void ErrorQuit(string errorMsg)
{
	cout << "Error with parsing, see output file." << endl;

	outputFile << "Error with the syntax, " << errorMsg << endl << "Now quitting...\n"; 
	system("pause"); 
	exit(1);
}

bool rat16s(string tokenArray[], string lexemeArray[], int pointer)
{
	// R1.
	// <Rat16S> ::= <Opt Function Defintions> 
	//				$$
	//				<Opt Declaration List>
	//				$$
	//				<Statement List>
	//				$$		
	
	bool valid = true;
	
	// Get first token
	NextToken(tokenArray, lexemeArray);

	if (debugmode){ outputFile << "<Rat16S> ::= <Opt Function Definitions> $$ <Opt Declaration List> $$ <Statement List> $$ \n"; }

	// <Opt Function Definition>
//	OptFunctionDefinitions(tokenArray, lexemeArray, pointer);

	// $$
	if (lexeme == "$$")
	{	
		if (debugmode){ WriteSyntax(tokenArray, lexemeArray); }
		NextToken(tokenArray, lexemeArray);

		// <Opt Declaration List>
		if (!(OptDeclarationList(tokenArray, lexemeArray, pointer)))
		{ ErrorQuit("expected <Opt Declaration List>"); }

		// $$
		if (lexeme == "$$")
		{
			NextToken(tokenArray, lexemeArray);

			if (!(StatementList(tokenArray, lexemeArray, pointer)))
			{ valid = true; }
		}
		else 
		{ ErrorQuit("expected $$"); }

		valid = true;
		NextToken(tokenArray, lexemeArray);
		if (debugmode){ WriteSyntax(tokenArray, lexemeArray); }

		if (lexeme != "$$")
		{ ErrorQuit("expected $$"); }
	}
	else
	{ ErrorQuit("expected $$"); }

	return valid;
}

bool OptFunctionDefinitions(string tokenArray[], string lexemeArray[], int pointer)
{
	// REMOVE!
	outputFile << endl << "Inside OptFunctionDefinitions" << endl;
	// REMOVE!!
	
	// R2.
	// <Opt Function Definitions> ::= <Function Definitions> | <emtpy>

	bool valid = true;

	if (lexeme == "function") 		
	{
		if (debugmode) { outputFile << "\t<Opt Function Definitions> ::= <Function Definitions>\n"; } // WriteSyntax(tokenArray, lexemeArray);

		if (FunctionDefinitions(tokenArray, lexemeArray, pointer))
		{  }
	}
	else
	{	
		WriteSyntax(tokenArray, lexemeArray); 		

		if (debugmode) { outputFile << "\t<Opt Function Definitions> ::= <Empty>\n"; }

		Empty(tokenArray, lexemeArray, pointer); 
	}

	return valid;
}

bool FunctionDefinitions(string tokenArray[], string lexemeArray[], int pointer)
{
	// REMOVE!
	outputFile << endl << "Inside FunctionDefinitions" << endl;
	// REMOVE!!

	// R3.
	// <Function Definitions> ::= <Function> | <Function><Function Definitions>

//	NextToken(tokenArray, lexemeArray);

	bool valid = true;

	if (Function(tokenArray, lexemeArray, pointer))
	{
		if(FunctionDefinitions(tokenArray, lexemeArray, pointer))
		{
			if (debugmode) { outputFile << "\t<Function Definitions> ::= <Function><Function Definitions>\n"; }
		}
		else
		{
			if (debugmode) { outputFile << "\t<Function Definitions> ::= <Function>\n"; }
		}
	}

	return valid;
}

bool Function(string tokenArray[], string lexemeArray[], int pointer)
{

	// REMOVE!
	outputFile << endl << "Inside Function" << endl;
	// REMOVE!!

	// R4.
	// <Function> ::= function <Identifier> ( <Opt Parameter List> ) <Opt Declaration List> <Body>

	bool valid = false;

	if (debugmode) { outputFile << "\t<Function> ::= function <Identifier> ( <Opt Parameter List> ) <Opt Declaration List> <Body>\n"; }
		
	if (lexeme == "function")
	{	
		NextToken(tokenArray, lexemeArray);

		if (token == "identifier")
		{	
			NextToken(tokenArray, lexemeArray);
			
			if (lexeme == "(")
			{
				if (OptParameterList(tokenArray, lexemeArray, pointer))
				{
					if (lexeme == ")")
					{
						if (OptDeclarationList(tokenArray, lexemeArray, pointer))
						{
							if (Body(tokenArray, lexemeArray, pointer))
							{ valid = true; }
							else
							{ ErrorQuit("expected <Body>"); }
						}
						else
						{ ErrorQuit("expected <Opt Declaration List>"); }
					}
				}
				else 
				{ ErrorQuit("expected <Opt Parameter List>"); }
			}
			else				
			{ ErrorQuit("expected '('"); }
		}				
		else	
		{ ErrorQuit("expected Identifier"); }
	}

	return valid;
}

bool OptParameterList(string tokenArray[], string lexemeArray[], int pointer)
{
	
	// REMOVE!
	outputFile << endl << "Inside OptParameterList" << endl;
	// REMOVE!!	
	
	// R5.
	// <Opt Parameter List> ::= <Parameter List> | <Empty>

	bool valid = false;

	NextToken(tokenArray, lexemeArray);

	if(ParameterList(tokenArray, lexemeArray, pointer))
	{
		if(debugmode) {	outputFile << "\t<Opt Parameter List> ::= <Parameter List>\n"; }

		valid = true;
	}
	else
	{
		if(debugmode) {	outputFile << "\t<Opt Parameter List> ::= <Empty>\n"; }

		Empty(tokenArray, lexemeArray, pointer);
		valid = true;
	}

	return valid;
}

bool ParameterList(string tokenArray[], string lexemeArray[], int pointer)
{

	// REMOVE!
	outputFile << endl << "Inside ParameterList" << endl;
	// REMOVE!!	

	// R6. 
	// <Parameter List> ::= <Parameter> | <Parameter> , <Parameter List>

	bool valid = false;

	if(Parameter(tokenArray, lexemeArray, pointer))
	{
		NextToken(tokenArray, lexemeArray);

		if(lexeme == ",")
		{
			if(debugmode) {	outputFile << "\t<Parameter List> ::= <Parameter> , <Parameter List>\n"; }
			
			if(ParameterList(tokenArray, lexemeArray, pointer))
				valid = true;
			else 
				valid = false;
		}
		else 
		{
			if(debugmode) {	outputFile << "\t<Parameter List> ::= <Parameter>\n"; }
			
			valid = true;
		}
	}

	return valid;
}

bool Parameter(string tokenArray[], string lexemeArray[], int pointer)
{
	// REMOVE!
	outputFile << endl << "Inside Parameter" << endl;
	// REMOVE!!	

	// R7. 
	// <Parameter> ::= <IDs> : <Qualifier>

	bool valid = false;

	if(debugmode) { WriteSyntax(tokenArray, lexemeArray); 
	outputFile << "\t<Parameter> ::= <IDS> : <Qualifier>\n"; }
	
	if(IDs(tokenArray, lexemeArray, pointer))
	{
		if(lexeme == ":")
		{
			NextToken(tokenArray, lexemeArray);

			if(Qualifier(tokenArray, lexemeArray, pointer))
			{ valid = true; }
		}
		else
		{ ErrorQuit("expected ':'"); }
	}

	return valid;
}

bool Qualifier(string tokenArray[], string lexemeArray[], int pointer)
{
	// REMOVE!
	outputFile << endl << "Inside Qualifier" << endl;
	// REMOVE!!	

	// R8.
	// <Qualifier> ::= integer | boolean | real

	bool valid = true;

//	if (debugmode){ WriteSyntax(tokenArray, lexemeArray);
//	outputFile << "\t<Declaration> ::= <Qualifier> <IDs>\n"; }

	if (lexeme == "integer")
	{ 
		if (debugmode){ WriteSyntax(tokenArray, lexemeArray);
		outputFile << "\t<Declaration> ::= <Qualifier> <IDs>\n"; 
		outputFile << "\t<Qualifier> ::= integer\n"; }
	}
	else if (lexeme == "real")
	{ 
		if (debugmode){ WriteSyntax(tokenArray, lexemeArray);
		outputFile << "\t<Declaration> ::= <Qualifier> <IDs>\n"; 
		outputFile << "\t<Qualifier> ::= real\n"; }
	}
	else if (lexeme == "boolean")
	{
		if (debugmode){ WriteSyntax(tokenArray, lexemeArray);
		outputFile << "\t<Declaration> ::= <Qualifier> <IDs>\n"; 
		outputFile << "\t<Qualifier> ::= boolean\n"; }
	}
	else
	{ valid = false; }

	return valid;
}

bool Body(string tokenArray[], string lexemeArray[], int pointer)
{
	// REMOVE!
	outputFile << endl << "Inside Body" << endl;
	// REMOVE!!	

	// R9.
	// <Body> ::= { <Statement List> }

	NextToken(tokenArray, lexemeArray);	
	
	bool valid = false;

	if (debugmode) { WriteSyntax(tokenArray, lexemeArray);
		outputFile << endl << "\t<Body> ::= { <Statement List> }\n"; }

	if (lexeme == "{")
	{
		if(StatementList(tokenArray, lexemeArray, pointer))
		{
			if(lexeme == "}")
			{ valid = true; }
			else
			{ ErrorQuit("expected '}'"); }
		}
	}
	else
	{ ErrorQuit("expected '{'");}

	return valid;
}

bool OptDeclarationList(string tokenArray[], string lexemeArray[], int pointer)
{	
	// REMOVE!
	outputFile << endl << "Inside OptDeclarationList" << endl;
	// REMOVE!!	

	// R10.
	// <Opt Declaration List> ::= <Declaration List> | <Empty>	

	if (DeclarationList(tokenArray, lexemeArray, pointer))
	{ 
		if (debugmode) { WriteSyntax(tokenArray, lexemeArray);
		outputFile << "\t<Opt Declaration List> ::= <Declaration List>\n"; }
	}
	else
	{ 
		if (debugmode) { WriteSyntax(tokenArray, lexemeArray);
		outputFile << "\t<Opt Declaration List> ::= <Empty>\n"; }

		Empty(tokenArray, lexemeArray, pointer);
	}

	return true;
}

bool DeclarationList(string tokenArray[], string lexemeArray[], int pointer)
{
	// REMOVE!
	outputFile << endl << "Inside DeclarationList" << endl;
	// REMOVE!!		
	
	// R11.
	// <Declaration List> ::= <Declaration> ; | <Declaration> ; <Declaration List>
	
	bool valid = false;
	
	if (Declaration(tokenArray, lexemeArray, pointer))
	{
		//Dennis
		//NextToken(tokenArray, lexemeArray);

		if (lexeme == ";")
		{
			if (debugmode) { WriteSyntax(tokenArray, lexemeArray);}

			NextToken(tokenArray, lexemeArray);
			valid = true;

			if (DeclarationList(tokenArray, lexemeArray, pointer))
			{ 
				if(debugmode) { WriteSyntax(tokenArray, lexemeArray);
				outputFile << "\t<Declaration List> ::= <Declaration> ; <Declaration List>\n"; }
			}
			else
			{ 
				if(debugmode) { WriteSyntax(tokenArray, lexemeArray);
				outputFile << "\t<Declaration List> ::= <Declaration>\n"; }
			}
		}
		else
		{
			valid = false;
			ErrorQuit("expected ';'");
		}
	}

	return valid;
}

bool Declaration(string tokenArray[], string lexemeArray[], int pointer)
{
	// REMOVE!
	outputFile << endl << "Inside Declaration" << endl;
	// REMOVE!!		
	
	// R12.
	// <Declaration> ::= <Qualifier> <IDs>

	bool valid = false;

	if (Qualifier(tokenArray, lexemeArray, pointer))
	{
		//Dennis
		NextToken(tokenArray, lexemeArray);

		if (IDs(tokenArray, lexemeArray, pointer))
		{ valid = true; }
	}
	else
	{ valid = false; }

	return valid;
}

bool IDs(string tokenArray[], string lexemeArray[], int pointer)
{
	// REMOVE!
	outputFile << endl << "Inside IDs" << endl;
	// REMOVE!!			
	
	// R13.
	// <IDs> ::= <Identifier> | <Identifier> , <IDs>	
	
	bool valid = false;

	//Dennis
	//NextToken(tokenArray, lexemeArray);
	if (debugmode){ WriteSyntax(tokenArray, lexemeArray);}

	if (Identifier() || lexeme == ",")
	{
		//Dennis
		NextToken(tokenArray, lexemeArray);

		if (lexeme == ",")
		{
			valid = true;

			if (debugmode){ WriteSyntax(tokenArray, lexemeArray);
				outputFile << "\t<IDs> ::= <Identifier>, <IDs>\n"; }

			NextToken(tokenArray, lexemeArray);
			if (debugmode){ WriteSyntax(tokenArray, lexemeArray); }

			if (IDs(tokenArray, lexemeArray, pointer))
			{ valid = true; }
			else 
			{ valid = false; }
		}
		else
		{
			if(debugmode) { outputFile << "\t<IDs> ::= <Identifier>\n"; }
			valid = true;
		}
	}

	return valid;
}

bool StatementList(string tokenArray[], string lexemeArray[], int pointer)
{	
	// REMOVE!
	outputFile << endl << "Inside StatementList" << endl;
	// REMOVE!!		
	
	// R14.
	// <Statement List> ::= <Statement> | <Statement><Statement List>	

	bool valid = false;

	if (Statement(tokenArray, lexemeArray, pointer))
	{
		if (debugmode){ WriteSyntax(tokenArray, lexemeArray); }

		if (StatementList(tokenArray, lexemeArray, pointer))
		{
			//* outputFile << "\t<Statement List> ::= <Statement> <Statement List>\n";
			valid = true;
		}
		else
		{
			//* outputFile << "\t<Statement List> ::= <Statement>\n";
			valid = true;
		}
	}

	return valid;
}

bool Statement(string tokenArray[], string lexemeArray[], int pointer)
{
	// REMOVE!
	outputFile << endl << "Inside Statement" << endl;
	// REMOVE!!	

	// R15.
	// <Statement> ::= <Compound> | <Assign> | <If> | <Return> |
	//					<Write> | <Read> | <While>	
	
	bool valid = false;

	if (lexeme == "{")
	{
		if (debugmode) { WriteSyntax(tokenArray, lexemeArray);
			outputFile << "\t<Statement> ::= <Compound>\n"; }

		valid = Compound(tokenArray, lexemeArray, pointer);
	}
	else if (token == "identifier")
	{
		valid = true;

		if (debugmode) { outputFile << "\t<Statement> ::= <Assign>\n"; }

		if (Assign(tokenArray, lexemeArray, pointer))
		{ }
	}
	else if (lexeme == "if")
	{
		valid = true;

		if (debugmode) { WriteSyntax(tokenArray, lexemeArray);
			outputFile << "\t<Statement> ::= <If>\n";}

		If(tokenArray, lexemeArray, pointer);
	}
	else if (lexeme == "return")
	{
		valid = true;

		if (debugmode) { WriteSyntax(tokenArray, lexemeArray);
			outputFile << "\t<Statement> ::= <Return> \n";}

		Return(tokenArray, lexemeArray, pointer);
	}

	else if (lexeme == "printf")
	{
		valid = true;

		if (debugmode) { WriteSyntax(tokenArray, lexemeArray);
			outputFile << "\t<Statement> ::= <Write> \n";}

		Write(tokenArray, lexemeArray, pointer);
	}

	else if (lexeme == "scanf")
	{
		valid = true;

		if (debugmode) { WriteSyntax(tokenArray, lexemeArray);
			outputFile << "\t<Statement> ::= <Read>\n"; }

		Read(tokenArray, lexemeArray, pointer);
	}

	else if (lexeme == "while")
	{
		valid = true;

		if (debugmode) { WriteSyntax(tokenArray, lexemeArray);
			outputFile << "\t<Statement> ::= <While>\n"; }

		While(tokenArray, lexemeArray, pointer);
	}

	return valid;
}

bool Compound(string tokenArray[], string lexemeArray[], int pointer)
{
	// REMOVE!
	outputFile << endl << "Inside Compound" << endl;
	// REMOVE!!		
	
	// R16.
	// <Compound> ::= { <Statement List> }	
	
	bool valid = false;

	if (debugmode) { WriteSyntax(tokenArray, lexemeArray);
		outputFile << "\t<Compound> ::= { <Statement List> }\n"; }

	if (lexeme == "{")
	{
		NextToken(tokenArray, lexemeArray);

		if (StatementList(tokenArray, lexemeArray, pointer))
		{
			if (lexeme == "}")
			{
				valid = true;
				NextToken(tokenArray, lexemeArray);
			}
			else
			{	ErrorQuit("expected '}'"); }
		}
	}

	return valid;
}

bool Assign(string tokenArray[], string lexemeArray[], int pointer)
{
	// REMOVE!
	outputFile << endl << "Inside Assign" << endl;
	// REMOVE!!	

	// R17.
	// <Assign> ::= <Identifier> := <Expression> ;	
	
	bool valid = false;
	
	if (debugmode) { WriteSyntax(tokenArray, lexemeArray);
		outputFile << "\t<Assign> ::= <Identifier> := <Expression> ;\n"; }

	if (Identifier())
	{
		NextToken(tokenArray, lexemeArray);

		if (lexeme == ":=")
		{
			if (debugmode) { WriteSyntax(tokenArray, lexemeArray); }

			NextToken(tokenArray, lexemeArray);

			if (Expression(tokenArray, lexemeArray, pointer))
			{
				if (!(lexeme == ";"))
				{
					NextToken(tokenArray, lexemeArray);

					if (!(lexeme == ";"))
					{ ErrorQuit("expected ';'"); }
				}
				else
				{ valid = true; }

				NextToken(tokenArray, lexemeArray);
			}
		}
		else
		{ ErrorQuit("expected ':='"); }
	}

	return valid;
}

bool If(string tokenArray[], string lexemeArray[], int pointer)
{
	// REMOVE!
	outputFile << endl << "Inside 'IF'" << endl;
	// REMOVE!!		
	
	// R18.
	// <If> ::= if ( <Condition> ) <Statement> endif |
	//			if ( <Condition> ) <Statement> else <Statement> endif	
	
	bool valid = false;

	if (debugmode) { WriteSyntax(tokenArray, lexemeArray);
		outputFile << "\t<If> ::= if ( <Condition> ) <Statement>"; }

	if (lexeme == "if")
	{
		NextToken(tokenArray, lexemeArray);

		if (lexeme == "(")
		{
			NextToken(tokenArray, lexemeArray);
			Condition(tokenArray, lexemeArray, pointer);
	
			if (lexeme == ")")
			{
				NextToken(tokenArray, lexemeArray);
				Statement(tokenArray, lexemeArray, pointer);

				if (lexeme == "endif")
				{
					NextToken(tokenArray, lexemeArray);
					if(debugmode) { outputFile << " endif\n"; }
					valid = true;
				}
				else if (lexeme == "else")
				{
					if(debugmode) { outputFile <<" else <Statement> endif\n"; }			
					NextToken(tokenArray, lexemeArray);

					if (Statement(tokenArray, lexemeArray, pointer))
					{
						NextToken(tokenArray, lexemeArray);

						if(lexeme == "endif")
						{ valid = true; }
					}
				}
				else				
				{ ErrorQuit("expected 'endif' or 'else'"); };				
			}
		}
		else
		{ ErrorQuit("expected ')'"); }
	}
	else
	{ ErrorQuit("expected '('"); };
	
	return valid;
}

bool Return(string tokenArray[], string lexemeArray[], int pointer)
{
	// REMOVE!
	outputFile << endl << "Inside Return" << endl;
	// REMOVE!!		

	// R19.
	// <Return> ::= return ; | return <Expression> ;	
	
	bool valid = false;

	NextToken(tokenArray, lexemeArray);

	if (lexeme == ";")
	{
		if(debugmode) { WriteSyntax(tokenArray, lexemeArray);
		outputFile <<"\t<Return> ::= return;\n"; }		
		valid = true;
	}
	else if (Expression(tokenArray, lexemeArray, pointer))
	{ 
		if(debugmode) { WriteSyntax(tokenArray, lexemeArray);
		outputFile <<"\t<Return> ::= return <Expression>;\n"; }
	}
	else
	{ ErrorQuit("expected ';'"); };
	
	return valid;
}

bool Write(string tokenArray[], string lexemeArray[], int pointer)
{
	// REMOVE!
	outputFile << endl << "Inside Write" << endl;
	// REMOVE!!		

	// R20.
	// <Write> ::= printf ( <Expression> ) ;	
	
	bool valid = false;

	if (debugmode) { WriteSyntax(tokenArray, lexemeArray);
		outputFile <<"\t<Write> :: = printf ( <Expression> );\n"; }

	if (lexeme == "printf")
	{
		NextToken(tokenArray, lexemeArray);
        
		if (lexeme =="(")
		{
			NextToken(tokenArray, lexemeArray);

			if (Expression(tokenArray, lexemeArray, pointer))
			{
				NextToken(tokenArray, lexemeArray);

				if (lexeme == ")")
				{
					NextToken(tokenArray, lexemeArray);

					if (lexeme == ";")
					{ valid = true; }
				}          
			}
		}
		else
		{ ErrorQuit("expected '('"); };
	}
	else
	{ ErrorQuit("expected 'printf'"); };
    
	return valid;
}

bool Read(string tokenArray[], string lexemeArray[], int pointer)
{
	// REMOVE!
	outputFile << endl << "Inside Read" << endl;
	// REMOVE!!		

	// R21.
	// <Read> ::= scanf ( <IDs> ) ;	
	
	bool valid = false;

	if (debugmode) { WriteSyntax(tokenArray, lexemeArray);
		outputFile << "\t<Read> ::= scanf ( <IDs> );\n";}
	
	if (lexeme =="scanf")
	{
		NextToken(tokenArray, lexemeArray);

		if (lexeme == "(")
		{
			if (debugmode) { WriteSyntax(tokenArray, lexemeArray); }
			//Dennis
			NextToken(tokenArray, lexemeArray);

			if (IDs(tokenArray, lexemeArray, pointer))
			{
				if (lexeme == ")")
				{
					if (debugmode) { WriteSyntax(tokenArray, lexemeArray); }
					NextToken(tokenArray, lexemeArray);

					if (lexeme ==";")
					{
						valid = true;

						if (debugmode) { WriteSyntax(tokenArray, lexemeArray); }

						NextToken(tokenArray, lexemeArray);
					}
					else
					{ ErrorQuit("expected ';'"); };
				}
				else
				{ ErrorQuit("expected ')'"); };
			}
			else
			{ ErrorQuit("expected '<IDs>'"); };
		}
		else
		{ ErrorQuit("expected '('"); };
	}
	else
	{ ErrorQuit("expected 'scanf'"); };
    
	return valid;
}

bool While(string tokenArray[], string lexemeArray[], int pointer)
{
	// REMOVE!
	outputFile << endl << "Inside While" << endl;
	// REMOVE!!			
	
	// R22.
	// <While> ::= while ( <Condition> ) <Statement>	
	
	bool valid = false;

	if (lexeme == "while")
	{
		NextToken(tokenArray, lexemeArray);

		if (lexeme == "(")
		{
			NextToken(tokenArray, lexemeArray);

			if (Condition(tokenArray, lexemeArray, pointer))
			{
				if (lexeme == ")")
				{
					NextToken(tokenArray, lexemeArray);
					
					if(Statement(tokenArray, lexemeArray, pointer))
					{
						if (debugmode) { WriteSyntax(tokenArray, lexemeArray);
							outputFile << "\t<While> ::= while ( <Condition> ) <Statement>\n"; }                   
						valid =  true;
					}
					else
					{ ErrorQuit("expected <Expression>"); };
				}
				else
				{ ErrorQuit("expected ')'"); };
			}
			else
			{ ErrorQuit("expected <Expression>"); };
		}
		else
		{ ErrorQuit("expected '('"); };
	}
	else
	{ ErrorQuit("expected 'while'"); };

	return valid;
}

bool Condition(string tokenArray[], string lexemeArray[], int pointer)
{
	// REMOVE!
	outputFile << endl << "Inside Condition" << endl;
	// REMOVE!!			
	
	// R23.
	// <Condition> ::= <Expression> <Relop> <Expression>	
	
	bool valid = false;
      
	if(Expression(tokenArray, lexemeArray, pointer))
	{
		if(Relop(tokenArray, lexemeArray, pointer))
		{
			NextToken(tokenArray, lexemeArray);

			if(Expression(tokenArray, lexemeArray, pointer))
			{ valid = true; 
			
			// Assignment 3 goes here
					
			}
			else
			{ ErrorQuit("expected <Expression>"); };
		}
		else
		{ ErrorQuit("expected <Relop>"); };
	}
	else
	{ ErrorQuit("expected <Expression>"); };
    
	return valid;
}

bool Relop(string tokenArray[], string lexemeArray[], int pointer)
{
	// REMOVE!
	outputFile << endl << "Inside Relop" << endl;
	// REMOVE!!			
	
	// R24.
	// <Relop> ::= = | /= | > | < | => | <=

	bool valid = true;
    
	if (lexeme == "=")
	{ 
		if (debugmode) { WriteSyntax(tokenArray, lexemeArray);
			outputFile << "\t<Relop> ::= '=\' \n"; }	
	}
	else if (lexeme == "/=") 
	{ 
		if (debugmode) { WriteSyntax(tokenArray, lexemeArray);
			outputFile << "\t<Relop> ::= '/=' \n"; }
	}
	else if (lexeme == ">") 
	{ 
		if (debugmode) { WriteSyntax(tokenArray, lexemeArray);
			outputFile << "\t<Relop> ::= '>' \n"; }
	}
	else if (lexeme == "<")
	{ 
		if (debugmode) { WriteSyntax(tokenArray, lexemeArray);		
			outputFile << "\t<Relop> ::= '<' \n"; }
	}
	else if (lexeme == "=>")
	{ 
		if (debugmode) { WriteSyntax(tokenArray, lexemeArray);
			outputFile << "\t<Relop> ::= '=>' \n"; }
	}
	else if (lexeme == "<=")
	{ 
		if (debugmode) { WriteSyntax(tokenArray, lexemeArray);
			outputFile << "\t<Relop> ::= '<=' \n"; }
	}	
	else 
	{
		ErrorQuit("expected 'Relop Operator'");
		valid = false;
	}
     
	return valid;
}

bool Expression(string tokenArray[], string lexemeArray[], int pointer)
{
	// REMOVE!
	outputFile << endl << "Inside Expression" << endl;
	// REMOVE!!		

	// R25.
	// <Expression> ::= <Term> <Expression Prime>

	bool valid = false;

	if (debugmode) { WriteSyntax(tokenArray, lexemeArray);
		outputFile << "\t<Expression> ::= <Term> <Expression Prime>\n"; }

	if (Term(tokenArray, lexemeArray, pointer))
	{
		if (ExpressionPrime(tokenArray, lexemeArray, pointer))
		{ valid = true;	}
	}
	else
	{ ErrorQuit("expected <Term>"); }

	return valid;
}

bool ExpressionPrime(string tokenArray[], string lexemeArray[], int pointer)
{
	// REMOVE!
	outputFile << endl << "Inside Expression Prime" << endl;
	// REMOVE!!		

	// R26.
	// <Expression Prime> ::= + <Term> <Expression Prime> | - <Term> <Expression Prime> | <Empty>

	bool valid = false;

	if (lexeme == "+"|| lexeme == "-")
	{		
		if (lexeme == "+") 
		{ if(debugmode) { outputFile << "\t<ExpressionPrime> ::= + <term> <ExpressionPrime> \n"; } }
		else
		{ if(debugmode) { outputFile << "\t<ExpressionPrime> ::= - <term> <ExpressionPrime> \n"; } }

		NextToken(tokenArray, lexemeArray);

		if (Term(tokenArray, lexemeArray, pointer))
		{		
			if (ExpressionPrime(tokenArray, lexemeArray, pointer))
			{ valid = true;	}           
		}
		else
		{ ErrorQuit("expected <Expression>"); }
	}
	else if (Term(tokenArray, lexemeArray, pointer))
	{ valid = true; }
	else if (Empty(tokenArray, lexemeArray, pointer))
	{ 
		// if (debugmode) { outputFile << "\t<ExpressionPrime> ::= <Empty> \n"; }		// WriteSyntax(tokenArray, lexemeArray)
		valid = true;
	}
		
    return valid;
}

bool Term(string tokenArray[], string lexemeArray[], int pointer)
{
	// REMOVE!
	outputFile << endl << "Inside Term" << endl;
	// REMOVE!!		

	// R27.
	// <Term> ::= <Factor> <Term Prime> | <Factor>

	bool valid = false;


	if (Factor(tokenArray, lexemeArray, pointer))
	{	
		if (TermPrime(tokenArray, lexemeArray, pointer))
		{ 
			if (debugmode) { outputFile << "\t<Term> ::= <Factor> <Term Prime> \n"; }
			valid = true;		
		}
	}
	else
	{ 
		if (debugmode) { WriteSyntax(tokenArray, lexemeArray);			
			outputFile << "\t<Term> ::= <Factor> \n"; }
	}

    return valid;
}

bool TermPrime(string tokenArray[], string lexemeArray[], int pointer)
{
	// REMOVE!
	outputFile << endl << "Inside Term Prime" << endl;
	// REMOVE!!		

	// R28.
	// <Term Prime> ::= * <Factor> <Term Prime> | / <Factor> <Term Prime> | <Factor> | <Empty>

	bool valid = false;

	if(lexeme == "*"|| lexeme == "/")
	{
		if (debugmode) { outputFile << "\t<TermPrime> ::= * | / <Factor> <TermPrime> \n"; }	

		NextToken(tokenArray, lexemeArray);

		if (Factor(tokenArray, lexemeArray, pointer))
		{
			if (TermPrime(tokenArray, lexemeArray, pointer))
			{ valid = true; }           
		}
		else
		{ ErrorQuit("expected 'Factor'"); }
	}
	else if (Factor(tokenArray, lexemeArray, pointer))
	{
		if (debugmode) { outputFile << "\t<TermPrime> ::= <Factor> \n"; }
		valid = true;
	}
	else if (Empty(tokenArray, lexemeArray, pointer))
	{
		if (debugmode) { outputFile << "\t<TermPrime> ::= <Empty> \n"; }
		valid = true;
	}

	return valid;
}

bool Factor(string tokenArray[], string lexemeArray[], int pointer)
{
	// REMOVE!
	outputFile << endl << "Inside Factor" << endl;
	// REMOVE!!			
	
	// R29. 
	// <Factor> ::= - <Primary> | <Primary>
	
	bool valid = false;

    if(lexeme == "-")
    {
		NextToken(tokenArray, lexemeArray);

        if (Primary(tokenArray, lexemeArray, pointer))
        {
			if (debugmode) { WriteSyntax(tokenArray, lexemeArray);            
				outputFile << "\t<Factor> ::= - <Primary> \n"; }        

            valid = true;
        }
    }
    else 
	{
        if (Primary(tokenArray, lexemeArray, pointer))
		{ 
			// if (debugmode) { outputFile << "\t<Factor> ::= <Primary> \n"; } // WriteSyntax(tokenArray, lexemeArray);
			valid = true; 
		}
    }

    return valid;
}

bool Primary(string tokenArray[], string lexemeArray[], int pointer)
{
	// REMOVE!
	outputFile << endl << "Inside Primary" << endl;
	// REMOVE!!			
	
	// R30.
	// <Primary> ::= <Identifier> | <Integer> | <Identifier> ( <IDs> ) | ( <Expression> ) | <Real> | true | false
	
	bool valid = false;

/*	if (token == "real")
	{
		valid = true;

		if (debugmode) { WriteSyntax(tokenArray, lexemeArray);
			outputFile << "\t<Primary> ::= <Real>\n";
	}
    else */
		
	if (token == "identifier")
	{
        valid = true;   
		NextToken(tokenArray, lexemeArray);

        if (lexeme == "(")
		{
			NextToken(tokenArray, lexemeArray);

            if (IDs(tokenArray, lexemeArray, pointer))
			{
                if (lexeme == ")")
				{  
					if (debugmode) { outputFile << "\t<Primary> ::= <Identifier> ( <IDs> )\n"; } 
				}
                else
                { ErrorQuit("expected ')'"); }
            }
            else
            { ErrorQuit("expected '<IDs>'"); }
        }

		if (debugmode) { outputFile << "<Factor> ::= <Identifier>\n"; }	
    }
	else if (lexeme == "(")		
	{
        if (Expression(tokenArray, lexemeArray, pointer))
		{
			NextToken(tokenArray, lexemeArray);

            if (lexeme == ")")
			{
                valid = true;
				if (debugmode) { WriteSyntax(tokenArray, lexemeArray);      		
					outputFile << "\t<Primary> ::= ( <Expression> )\n"; }
            }
            else
            { ErrorQuit("expected ')'"); }
        }
    }
	else if (token == "integer")
	{
        valid = true;
		NextToken(tokenArray, lexemeArray);
        
		if (debugmode) { WriteSyntax(tokenArray, lexemeArray);      		
			outputFile << "\t<Primary> ::= Integer\n"; }
    }
	else if (lexeme == "true")
	{
		valid = true;
		NextToken(tokenArray, lexemeArray);
		
		if (debugmode) { WriteSyntax(tokenArray, lexemeArray);      		
			outputFile << "\t<Primary> ::= true\n"; }
    }
	else if(lexeme == "false")
	{
		valid = true;
		NextToken(tokenArray, lexemeArray);
		
		if (debugmode) { WriteSyntax(tokenArray, lexemeArray);      		
			outputFile << "\t<Primary> ::= false\n"; }    
	}

    return valid;
}

bool Empty(string tokenArray[], string lexemeArray[], int pointer)
{	
	// R31.
	// <Empty> ::= empty
		
	if (debugmode) { outputFile << "\t<Empty> ::= Epsilon\n"; }
    
	return true;
}

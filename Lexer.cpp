//-------------------------------------------------------------------
//	Name:		Dennis Wu
//	Project:	Lexer Analyzer for RAT16S
//	Due:		March 8, 2016
//	Course:		CPSC 323 Compilers
//
//	This program accepts a test.txt file containing the RAT16S 
//	language. It will output tokens and the "value" of the token
//	in another text file called output.txt.
//
//-------------------------------------------------------------------
// Current limitations:
//		cannot handle identifier with numbers (ex: abc12)
//		must end txt file with a new line! [hit enter]
//-------------------------------------------------------------------

#include<iostream>
#include<fstream>
#include<string>

#include "Lexer.h"

using namespace std;

	int tokenCount = 0;

	int identifer[5][2] = {  
	 {1, 4} ,  
	 {2, 3} ,  /*  accepting state  */
	 {2, 3} ,  /*  accepting state  */
	 {2, 3} ,
	 {4, 4}
	}; 

	int integer[5][2] = {  
	 {1, 2} ,  
	 {1, 2} ,  /*  accepting state  */
	 {3, 4} , 
	 {3, 4} ,  /*  accepting state  */ 
	 {4, 4}
	}; 

	// Checks for keywords
	bool isKeyword(string word)
	{
		return ( word == "function" || word == "integer" || word == "boolean" || word == "real" || 
			word == "if" || word == "endif" || word == "else" || word == "return" || 
			word == "printf" || word == "scanf" || word == "true" || word == "false" || word == "while" );
	}

	// Checks for seperators
	bool isSeperator(char letter)
	{
		return ( letter == '(' || letter == ')' || letter == ',' || letter == ':' || letter == '{' ||
			letter == '}' || letter == ';' || letter == '$' || letter == ' ' || letter == '\n' );
	}

	// Checks for operators
	bool isOperator(char letter)
	{
		return ( letter == '=' || letter == '/' || letter == '>' || letter == '<' ||
			letter == '+' || letter == '-' || letter == '*' );
	}

	int GetTokenCount()
	{ return tokenCount; }

void Lexer()
{
	char character;
	string lexeme = "";

	int identifierPointer = 0;
	int integerPointer = 0;

	bool isReal = false;

	bool ignore_equal = false;
	bool ignore_mode = false;

	// Open test file
	ifstream inputFile;
	inputFile.open ("code.txt");

	// Creat output file
	ofstream outputFile;
	outputFile.open ("tokens.txt");

	// Header
	//outputFile << "token               lexeme \n\n";

	while (!inputFile.eof()) 
	{	
		// Read char
		inputFile.get(character);

		// Check for comments, if so; ignore until end of comment
		if (character == '[')
		{
			inputFile.get(character); // check next character

			if (character == '*')
			{ ignore_mode = true; }
			else
			{ inputFile.unget(); }
		}

		if ( (character == '*') && (ignore_mode) )
		{
			inputFile.get(character); // check next character

			if (character == ']')
			{ ignore_mode = false; }
			else
			{ inputFile.unget(); }
		}

		// Begin after comments are ignored
		if (ignore_mode == false)
		{
			// Check for letter
			if ( (character > 64 && character < 91) || (character > 96 && character < 123) )
			{
				// Update table
				identifierPointer = identifer[identifierPointer][0];

				// Update local string
				lexeme += tolower(character);
			}

			// Check for underscore
			if (character == '_')
			{ 
				identifierPointer = identifer[identifierPointer][1];
				lexeme += character;
			}

			// Check for integer
			if (character > 47 && character < 58) 
			{
				integerPointer = integer[integerPointer][0];
				lexeme += character;
			}

			// Check for decimal
			if (character == '.')
			{
				isReal = true;
				integerPointer = integer[integerPointer][1];
				lexeme += character;
			}

			// Check for serperator after identifier or number
			if ( (isSeperator(character)) || (isOperator(character)) )
			{ 
				if (identifierPointer != 0)
				{ 
					if (identifierPointer == 1 || identifierPointer == 2)
					{ 
						// Check for keywords
						if ( isKeyword(lexeme) )
						{ outputFile << "keyword "; }
						else
						{ outputFile << "identifier "; }

						// Update Token Counter for Syntax.cpp
						tokenCount = tokenCount + 1;
					}
					else 
					{ 
						outputFile << "unknown "; 
						// Update Token Counter for Syntax.cpp
						tokenCount = tokenCount + 1;								
					}

					// Output to file
					outputFile << lexeme << endl;

					// Reset everything
					lexeme = "";
					identifierPointer = 0;
				}

				if (integerPointer != 0)
				{ 
					if (integerPointer == 1 || integerPointer == 3)					
					{
							if (isReal)
							{ 
								outputFile << "real "; 
								// Update Token Counter for Syntax.cpp
								tokenCount = tokenCount + 1;
							}
							else 
							{ 
								outputFile << "integer "; 
								// Update Token Counter for Syntax.cpp
								tokenCount = tokenCount + 1;
							}
					}
					else
					{ 
						outputFile << "unknown "; 
						// Update Token Counter for Syntax.cpp
						tokenCount = tokenCount + 1;					
					}	

					// Output to file
					outputFile << lexeme << endl; 

					// Reset everything
					lexeme = "";
					isReal = false;

					integerPointer = 0;
				}

				if ( (isSeperator(character)) && (character != ' ') && (character != '\n') ) 
				{ 
					// Check for specific double characters
					if (character == ':' || character == '$')				
					{
						lexeme += character;

						// Check next character
						inputFile.get(character);

						if (character == '=') // Checks for ":="
						{
							lexeme += character;
							outputFile << "operator " << lexeme << endl;

							// Update Token Counter for Syntax.cpp
							tokenCount = tokenCount + 1;

							lexeme = "";

							// Raise a flag
							ignore_equal = true;
						}
						else if (character == '$') // Handles "$$"
						{
							lexeme += character;
							outputFile << "seperator " << lexeme << endl;

							// Update Token Counter for Syntax.cpp
							tokenCount = tokenCount + 1;
					
							// Reset local string
							lexeme = "";
						}
						else
						{ 
							inputFile.unget();  // Return back a character

							outputFile << "seperator " << lexeme << endl; 

							// Update Token Counter for Syntax.cpp
							tokenCount = tokenCount + 1;

							// Reset local string
							lexeme = "";
						};
					}
					else
					{
						lexeme += character;
						outputFile << "seperator " << lexeme << endl; 

						// Update Token Counter for Syntax.cpp
						tokenCount = tokenCount + 1;

						// Reset local string
						lexeme = "";
					}
				}

				if (isOperator(character))
				{
					if (!ignore_equal)
					{
						if (character == '/')
						{
							lexeme += character;

							// Check next character
							inputFile.get(character);

							if (character == '=') // Handles "/="
							{
								lexeme += character;
								outputFile << "operator " << lexeme << endl; 

								// Update Token Counter for Syntax.cpp
								tokenCount = tokenCount + 1;
					
								// Reset local string
								lexeme = "";
							}
							else
							{ 
								inputFile.unget();  // Return back a character

								outputFile << "operator " << lexeme << endl; 

								// Update Token Counter for Syntax.cpp
								tokenCount = tokenCount + 1;

								// Reset local string
								lexeme = "";
							}
						}
						else if (character == '=')
						{
							lexeme += character;

							// Check next character
							inputFile.get(character);

							if (character == '>') // Handles "=>"
							{
								lexeme += character;
								outputFile << "operator " << lexeme << endl; 

								// Update Token Counter for Syntax.cpp
								tokenCount = tokenCount + 1;
					
								// Reset local string
								lexeme = "";
							}
							else
							{ 
								inputFile.unget();  // Return back a character

								outputFile << "operator " << lexeme << endl; 

								// Update Token Counter for Syntax.cpp
								tokenCount = tokenCount + 1;

								// Reset local string
								lexeme = "";
							}
						}
						else if (character == '<')
						{
							lexeme += character;

							// Check next character
							inputFile.get(character);

							if (character == '=') // Handles "<="
							{
								lexeme += character;
								outputFile << "operator " << lexeme << endl; 

								// Update Token Counter for Syntax.cpp
								tokenCount = tokenCount + 1;

								lexeme = "";
							}
							else
							{ 
								inputFile.unget();  // Return back a character

								outputFile << "operator " << lexeme << endl; 

								// Update Token Counter for Syntax.cpp
								tokenCount = tokenCount + 1;

								lexeme = "";
							}
						}
						else
						{
							lexeme += character;
							outputFile << "operator " << lexeme << endl; 

							// Update Token Counter for Syntax.cpp
							tokenCount = tokenCount + 1;

							lexeme = "";
						}
					}
					else
					{ ignore_equal = false; } // Reset the flag
				}
			}
		}
	} 

	// close files
	inputFile.close();
	outputFile.close();
}
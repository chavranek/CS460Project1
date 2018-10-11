#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstring>
#include "SyntacticalAnalyzer.h"

using namespace std;

SyntacticalAnalyzer::SyntacticalAnalyzer (char * filename)
{
	lex = new LexicalAnalyzer (filename);
	token_type t;
	while ((t = lex->GetToken()) != EOF_T)
	{

        cout << '\t' << setw(12) << left << lex->GetLexeme();
        cout << setw(12) << left << lex->GetTokenName(t) << endl;

        int i;

		// write its name to the .p1 file
		// write the corresponding lexeme to the .p1 file
	}
    cout << setw(12) << left << lex->GetTokenName(t) << endl;
}

SyntacticalAnalyzer::~SyntacticalAnalyzer ()
{
	delete lex;
}

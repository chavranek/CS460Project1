#ifndef LEX_H
#define LEX_H

#include <iostream>
#include <fstream>

using namespace std;

// >=100 is a backup state
// >=200 is not a backup state
enum token_type {NONE = -1, EOF_T, 
        LISTOP_T =100,PREDICATE_T,KEYWORD_T,IDENT_T,PLUS_T,MIN_T,NUMLIT_T,GT_T,LT_T,
        STRLIT_T = 200,DIV_T,MULT_T,MODULO_T,ROUND_T,EQUALTO_T,GTE_T,LTE_T,LPAREN_T,RPAREN_T,SQUOTE_T,PREDICATE,ERROR_T,
        NUM_TOKENS};

class LexicalAnalyzer 
{
    public:
	LexicalAnalyzer (char * filename);
	~LexicalAnalyzer ();
	token_type GetToken ();
	string GetTokenName (token_type t) const;
	string GetLexeme () const;
	void ReportError (const string & msg);
    private:
	ifstream input;
	ofstream listingFile;
	ofstream tokenFile;
	ofstream debugFile;
	token_type token;
	string line;
	int linenum;
	int pos;
	string lexeme;
	int errors;
};
	
#endif

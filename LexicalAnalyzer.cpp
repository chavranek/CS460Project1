#include <iomanip>
#include <cstdlib>
#include "LexicalAnalyzer.h"

using namespace std;



int DFA[][21] = {
{5,9,6,2,1,1,1,1,ERROR_T,EQUALTO_T,11,12,DIV_T,MULT_T,LPAREN_T,RPAREN_T,SQUOTE_T,8,0,1,10},
{KEYWORD_T,KEYWORD_T,1,1,1,1,1,1,PREDICATE,KEYWORD_T,KEYWORD_T,KEYWORD_T,KEYWORD_T,KEYWORD_T,KEYWORD_T,KEYWORD_T,KEYWORD_T,KEYWORD_T,KEYWORD_T,1,KEYWORD_T},
{IDENT_T,IDENT_T,1,1,3,3,1,1,IDENT_T,IDENT_T,IDENT_T,IDENT_T,IDENT_T,IDENT_T,IDENT_T,IDENT_T,IDENT_T,IDENT_T,IDENT_T,1,IDENT_T},
{IDENT_T,IDENT_T,1,1,1,3,4,1,IDENT_T,IDENT_T,IDENT_T,IDENT_T,IDENT_T,IDENT_T,IDENT_T,IDENT_T,IDENT_T,IDENT_T,IDENT_T,1,IDENT_T},
{LISTOP_T,LISTOP_T,1,1,1,1,1,1,LISTOP_T,LISTOP_T,LISTOP_T,LISTOP_T,LISTOP_T,LISTOP_T,LISTOP_T,LISTOP_T,LISTOP_T,LISTOP_T,LISTOP_T,1,LISTOP_T},
{PLUS_T,PLUS_T,6,PLUS_T,PLUS_T,PLUS_T,PLUS_T,PLUS_T,PLUS_T,PLUS_T,PLUS_T,PLUS_T,PLUS_T,PLUS_T,PLUS_T,PLUS_T,PLUS_T,8,PLUS_T,PLUS_T,PLUS_T},
{NUMLIT_T,NUMLIT_T,6,NUMLIT_T,NUMLIT_T,NUMLIT_T,NUMLIT_T,NUMLIT_T,NUMLIT_T,NUMLIT_T,NUMLIT_T,NUMLIT_T,NUMLIT_T,NUMLIT_T,NUMLIT_T,NUMLIT_T,NUMLIT_T,7,NUMLIT_T,NUMLIT_T,NUMLIT_T},
{NUMLIT_T,NUMLIT_T,7,NUMLIT_T,NUMLIT_T,NUMLIT_T,NUMLIT_T,NUMLIT_T,NUMLIT_T,NUMLIT_T,NUMLIT_T,NUMLIT_T,NUMLIT_T,NUMLIT_T,NUMLIT_T,NUMLIT_T,NUMLIT_T,NUMLIT_T,NUMLIT_T,NUMLIT_T,NUMLIT_T},
{ERROR_T,ERROR_T,7,ERROR_T,ERROR_T,ERROR_T,ERROR_T,ERROR_T,ERROR_T,ERROR_T,ERROR_T,ERROR_T,ERROR_T,ERROR_T,ERROR_T,ERROR_T,ERROR_T,ERROR_T,ERROR_T,ERROR_T,ERROR_T},
{MIN_T,MIN_T,6,MIN_T,MIN_T,MIN_T,MIN_T,MIN_T,MIN_T,MIN_T,MIN_T,MIN_T,MIN_T,MIN_T,MIN_T,MIN_T,MIN_T,8,MIN_T,MIN_T,MIN_T},
{10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,STRLIT_T},
{GT_T,GT_T,GT_T,GT_T,GT_T,GT_T,GT_T,GT_T,GT_T,GTE_T,GT_T,GT_T,GT_T,GT_T,GT_T,GT_T,GT_T,GT_T,GT_T,GT_T,GT_T},
{LT_T,LT_T,LT_T,LT_T,LT_T,LT_T,LT_T,LT_T,LT_T,LTE_T,LT_T,LT_T,LT_T,LT_T,LT_T,LT_T,LT_T,LT_T,LT_T,LT_T,LT_T}
};

static string token_names[] = {	"EOF_T" }; 

LexicalAnalyzer::LexicalAnalyzer (char * filename)
{
	// This function will initialize the lexical analyzer class
    

    input = ifstream(filename);
    line = "";
    linenum = 1;
    pos = 0;
    lexeme ="";
    errors = 0;

}

LexicalAnalyzer::~LexicalAnalyzer ()
{
	// This function will complete the execution of the lexical analyzer class
}

token_type LexicalAnalyzer::GetToken ()
{


    // need to start reading the next line of input?
    if(pos == line.length() -1){
        pos = 0;
        getline(input,line);
        linenum++;
    }

	// reading character by character from line
	
	string tmp_lexeme;
	while(pos < line.length()){

		tmp_lexeme += line[pos];
	}


	// done reading lexeme
	lexeme = tmp_lexeme;
	

    cout << line << endl;
	// This function will find the next lexeme int the input file and return
	// the token_type value associated with that lexeme
	return token;
}

string LexicalAnalyzer::GetTokenName (token_type t) const
{
	// The GetTokenName function returns a string containing the name of the
	// token passed to it. 
	return "";
}

string LexicalAnalyzer::GetLexeme () const
{
	// This function will return the lexeme found by the most recent call to 
	// the get_token function
	return "";
}

void LexicalAnalyzer::ReportError (const string & msg)
{
	// This function will be called to write an error message to a file
}
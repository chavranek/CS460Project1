#ifndef LEX_H
#define LEX_H

#include <iostream>
#include <fstream>
#include <map>

using namespace std;

// >=100 is a backup state
// >=200 is not a backup state
// >=300 is a keyword
// >=400 is a predicate
enum token_type {NONE = -1, EOF_T,
        LISTOP_T =100,PREDICATE_T,KEYWORD_T,IDENT_T,PLUS_T,MIN_T,NUMLIT_T,GT_T,LT_T,
        STRLIT_T = 200,DIV_T,MULT_T,MODULO_T,ROUND_T,EQUALTO_T,GTE_T,LTE_T,LPAREN_T,RPAREN_T,SQUOTE_T,PREDICATE,ERROR_T,
        CONS_T=300,IF_T,COND_T,ELSE_T,DISPLAY_T,NEWLINE_T,AND_T,OR_T,NOT_T,DEFINE_T,
        NUMBERP_T=400,LISTP_T,ZEROP_T,NULLP_T,STRINGP_T,
        NUM_TOKENS};


static const int DFA[][21] = {
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

class LexicalAnalyzer
{
    public:
	LexicalAnalyzer (char * filename);
	~LexicalAnalyzer ();
	token_type GetToken ();
	string GetTokenName (token_type t);
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
  string errorMsg = "";
  map<token_type,string> token_names;
  map<string,token_type> keywords;
  map<string,token_type> predicates;
  void initTokenToName();
  void initKeywordToName();
  void initPredicateToName();
  string trimExtension(string s);
  int getcol(char c);
};

#endif

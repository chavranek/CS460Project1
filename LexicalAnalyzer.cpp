#include <iomanip>
#include <cstdlib>
#include "LexicalAnalyzer.h"

using namespace std;

string errorMsg = "";

inline std::string trim(std::string& str)
{
    str.erase(0, str.find_first_not_of(' '));       //prefixing spaces
    str.erase(str.find_last_not_of(' ')+1);         //surfixing spaces
    return str;
}

map<token_type,string> token_names;
map<string,token_type> keywords;
map<string,token_type> predicates;
LexicalAnalyzer::LexicalAnalyzer (char * filename)
{
	// This function will initialize the lexical analyzer class


    input = ifstream(filename);
    getline(input,line);
	line = trim(line);
    linenum = 1;
    pos = 0;
    lexeme ="";
    errors = 0;
    token = NONE;
    initTokenToName();
    initKeywordToName();
    initPredicateToName();
}

LexicalAnalyzer::~LexicalAnalyzer ()
{
	// This function will complete the execution of the lexical analyzer class
}


token_type LexicalAnalyzer::GetToken ()
{
    // need to start reading the next line of input?
    if(pos == line.length()){
      // write the entire line to the .lst file
      cout << "   " << linenum << ": " << line << endl;
      if (errorMsg != "")
	{
	  cout << errorMsg << endl;
	}
      //cout << linenum << ": " << line << endl;
      pos = 0;
      errorMsg = "";
      // if a line can't be grabbed then the end of the file has been reached
	if(!getline(input,line))
	  {
	    return EOF_T;
	  }
       	line = trim(line);
        linenum++;
    }
    //cout << "pos: " << pos << endl;

    // read a lexeme?
    int state = 0;
    string tmp_lexeme;
    token = NONE;
	while(pos < line.length()){
	    char c = line[pos];
	    int col = getcol(c);
	    if (col == 21)
	    {
	        //Report Error
	        if(errorMsg != "")
	        {
		        errorMsg += "\n";
	        }
	        errorMsg += "Error at " + to_string(linenum) + "," + to_string(pos + 1) + ": Invalid character found: " + c;
	        pos++;
	        errors++;
	        token = ERROR_T;
	        return token;
	    }

	    state =  DFA[state][getcol(c)];
	    if (state == ERROR_T)
	    {
	        if(errorMsg != "")
	        {
                errorMsg += "\n";
            }
            errorMsg += "Error at " + to_string(linenum) + "," + to_string(pos + 1) + ": Invalid character found: " + c;
	        //pos++;
	        errors++;
	        //token = ERROR_T;
	        //return token;
	    }
	// only add whitespace if state == 9 (dbl quote)
        if (state ==10 || c!= ' ')
	        tmp_lexeme += c;

        pos++;
        // we hit a non-backup accepting state
        if (state >= 200){
	        token = (token_type)state;
	        break;
        }
        // we hit a backup accepting state
        else if (state >=100){
	        token = (token_type)state;
            pos--;
            break;
        }
	}


    // this fixes if the lexeme is the only/last character on line
    if (token == NONE){
        token = (token_type)DFA[state][14];
    }

    // check if its actually a keyword or just an IDENT_T
    if(token == KEYWORD_T){
        // we found it in the keywords hash table
        if(keywords.find(tmp_lexeme) != keywords.end()){
            token = keywords[tmp_lexeme];
        }
        else // we didnt find it
            token = IDENT_T;
    }
    // check if its actually a predicate or an IDEN_T + ?
    else if (token == PREDICATE){
        if(predicates.find(tmp_lexeme) != predicates.end()){
            token = predicates[tmp_lexeme];
        }
        else{
            pos--; // backup on the ?
            tmp_lexeme.pop_back();
            token = IDENT_T;
        }
    }

	// no more?
	// if statement causes us to stop even at blank lines in middle of file
	/*if (tmp_lexeme == ""){
		return EOF_T;
		}*/

	// done reading lexeme
	//cout  << tmp_lexeme <<  "    " << this->GetTokenName(token) << endl;
	lexeme = tmp_lexeme;


	// This function will find the next lexeme int the input file and return
	// the token_type value associated with that lexeme
	return token;
	//return DIV_T;
}

string LexicalAnalyzer::GetTokenName (token_type t)
{
	// The GetTokenName function returns a string containing the name of the
	// token passed to it.
  return token_names[t];
  //return "";
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

void LexicalAnalyzer::initTokenToName() {
    token_names[LISTOP_T] = "LISTOP_T";
    token_names[PREDICATE_T] = "PREDICATE_T";
    token_names[KEYWORD_T] = "KEYWORD_T";
    token_names[IDENT_T] = "IDENT_T";
    token_names[PLUS_T] = "PLUS_T";
    token_names[MIN_T] = "MIN_T";
    token_names[NUMLIT_T] = "NUMLIT_T";
    token_names[GT_T] = "GT_T";
    token_names[LT_T] = "LT_T";
    token_names[STRLIT_T] = "STRLIT_T";
    token_names[DIV_T] = "DIV_T";
    token_names[MULT_T] = "MULT_T";
    token_names[MODULO_T] = "MODULO_T";
    token_names[ROUND_T] = "ROUND_T";
    token_names[EQUALTO_T] = "EQUALTO_T";
    token_names[GTE_T] = "GTE_T";
    token_names[LTE_T] = "LTE_T";
    token_names[LPAREN_T] = "LPAREN_T";
    token_names[RPAREN_T] = "RPAREN_T";
    token_names[SQUOTE_T] = "SQUOTE_T";
    token_names[PREDICATE] = "PREDICATE";
    token_names[ERROR_T] = "ERROR_T";
    token_names[CONS_T] = "CONS_T";
    token_names[IF_T] = "IF_T";
    token_names[COND_T] = "COND_T";
    token_names[ELSE_T] = "ELSE_T";
    token_names[DISPLAY_T] = "DISPLAY_T";
    token_names[NEWLINE_T] = "NEWLINE_T";
    token_names[AND_T] = "AND_T";
    token_names[OR_T] = "OR_T";
    token_names[NOT_T] = "NOT_T";
    token_names[DEFINE_T] = "DEFINE_T";
    token_names[NUMBERP_T] = "NUMBERP_T";
    token_names[LISTP_T] = "LISTP_T";
    token_names[ZEROP_T] = "ZEROP_T";
    token_names[NULLP_T] = "NULLP_T";
    token_names[STRINGP_T] = "STRINGP_T";
}

void LexicalAnalyzer::initKeywordToName() {
    keywords["cons"] = CONS_T;
    keywords["if"] = IF_T;
    keywords["cond"] = COND_T;
    keywords["else"] = ELSE_T;
    keywords["display"] = DISPLAY_T;
    keywords["newline"] = NEWLINE_T;
    keywords["and"] = AND_T;
    keywords["or"] = OR_T;
    keywords["not"] = NOT_T;
    keywords["define"] = DEFINE_T;
}

void LexicalAnalyzer::initPredicateToName() {
    predicates["number?"] = NUMBERP_T;
    predicates["list?"] = LISTP_T;
    predicates["zero?"] = ZEROP_T;
    predicates["null?"] = NULLP_T;
    predicates["string?"] = STRINGP_T;
}

int LexicalAnalyzer::getcol( char c){

    if (c == '+'){
        return 0;
    }
    else if (c == '-'){
        return 1;
    }
    else if (isdigit(c)){
        return 2;
    }
    else if (c == 'c'){
        return 3;
    }
    else if (c == 'a'){
        return 4;
    }
    else if (c == 'd'){
        return 5;
    }
    else if (c == 'r'){
        return 6;
    }
    else if (c == '_'){
        return 7;
    }
    else if (c == '?'){
        return 8;
    }
    else if (c == '='){
        return 9;
    }
    else if (c == '>'){
        return 10;
    }
    else if (c == '<'){
        return 11;
    }
    else if (c == '/'){
        return 12;
    }
    else if (c == '*'){
        return 13;
    }
    else if (c == '('){
        return 14;
    }
    else if (c == ')'){
        return 15;
    }
    else if (c == '\''){
        return 16;
    }
    else if (c == '.'){
        return 17;
    }
    else if (c == ' '){
        return 18;
    }
    else if (isalpha(c)){
        return 19;
    }
    else if (c == '"'){
        return 20;
    }
    return 21;
}

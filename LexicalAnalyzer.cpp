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
string errorMsg = "";

inline std::string trim(std::string& str)
{
    str.erase(0, str.find_first_not_of(' '));       //prefixing spaces
    str.erase(str.find_last_not_of(' ')+1);         //surfixing spaces
    return str;
}

int getcol( char c){

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
	    errorMsg += "Error at " + to_string(pos) + "," + to_string(linenum) + ": Invalid character found: " + c;
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
            errorMsg += "Error at " + to_string(pos) + "," + to_string(linenum) + ": Invalid character found: " + c;	    
	    pos++;
	    errors++;
	    token = ERROR_T;
	    return token;
	  }
	// only add whitespace if state == 9 (dbl quote)
        if (state ==10 || c!= ' ')
	  tmp_lexeme += c;
        pos++;
        // we hit a non-backup accepting state
        if (state >= 200){
            break;
        }
        // we hit a backup accepting state
        else if (state >=100){
            pos--;
            break;
        }
	}


	// no more?
	// if statement causes us to stop even at blank lines in middle of file
	/*if (tmp_lexeme == ""){
		return EOF_T;
		}*/

	// done reading lexeme
	//cout  << tmp_lexeme <<endl;
	lexeme = tmp_lexeme;
	

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

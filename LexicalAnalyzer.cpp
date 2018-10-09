#include <iomanip>
#include <cstdlib>
#include "LexicalAnalyzer.h"

using namespace std;

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
    string filenameBase = trimExtension(filename);
    listingFile = ofstream(filenameBase + ".lst");
    listingFile << "Input file: " << filename << endl;
    tokenFile = ofstream(filenameBase + ".p1");
    debugFile = ofstream(filenameBase + ".dbg");
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
  listingFile.close();
  tokenFile.close();
  debugFile.close();
  input.close();
}


// This function will find the next lexeme int the input file and return
// the token_type value associated with that lexeme
token_type LexicalAnalyzer::GetToken ()
{


    // need to start reading the next line of input?
    if(pos == line.length()){
      // write the entire line to the .lst file
      listingFile << "   " << linenum << ": " << line << endl;
      if (errorMsg != "")
	{
	  this->ReportError(errorMsg);
	}
      
      pos = 0;
      errorMsg = "";
      // if a line can't be grabbed then the end of the file has been reached
	if(!getline(input,line))
	  {
	    listingFile << errors;
	    if(errors > 1) {
	      listingFile << " errors";
	    } else {
	      listingFile << " error";
	    }
	    listingFile << " found in input file\n";
        token = EOF_T;
        tokenFile << "\t" << this->GetTokenName(token) << endl;
	    return EOF_T;
	  }
       	line = trim(line);
        linenum++;
    }
    

    // read a lexeme?
    int state = 0;
    string tmp_lexeme;
    token = NONE;
    while(pos < line.length())
      {
	char c = line[pos];
	int col = getcol(c);
	// only add whitespace if state == 9 (dbl quote)
	if (state ==10 || c!= ' ')
	{
	  tmp_lexeme += c;
	}
	
	int prevState = state;
	state =  DFA[state][col];
	
	if (state == ERROR_T || (tmp_lexeme == "." && pos == line.length() - 1))
	  {
	    //if(tmp_lexeme != "+." && tmp_lexeme != "-.")
	    //{
		if(errorMsg != "")
		  {
		    errorMsg += "\n";
		  }
		// handles a situation with a bad .
		if (prevState == 8 && !isdigit(c))
		  {
		    if (c != ' ')
		      {
			if (tmp_lexeme[0] == '+' || tmp_lexeme[0] == '-')
			  {
			    errorMsg += "Error at " + to_string(linenum) + "," + to_string(pos) + ": Invalid character found: " + tmp_lexeme[0] + tmp_lexeme[1];
			  }
			else
			  errorMsg += "Error at " + to_string(linenum) + "," + to_string(pos) + ": Invalid character found: " + tmp_lexeme[0];
		      }
		    else
		      errorMsg += "Error at " + to_string(linenum) + "," + to_string(pos) + ": Invalid character found: " + tmp_lexeme;
		    
		  }
		else
		  errorMsg += "Error at " + to_string(linenum) + "," + to_string(pos + 1) + ": Invalid character found: " + c;
		
		errors++;
		
		if (tmp_lexeme == "")
		  {
		    tmp_lexeme += c;
		  }
		//}
		  
	  }
	
	pos++;
	// we have hit a . that doesn't have a number after it.
	if (prevState == 8 && !isdigit(c) && c != ' ')
	  {
	    if(tmp_lexeme.length() > 1)
	      tmp_lexeme.pop_back();
	    token = (token_type)state;
	    pos--;
	    break;
	  }
	/*else if (tmp_lexeme == "+." && state == ERROR_T)
	  {
	    if(tmp_lexeme[0] == '+')
	      {
		state = PLUS_T;
		tmp_lexeme.pop_back();
		token = (token_type)state;
		pos--;
		pos--;
		break;
	      }
	  }
	else if (tmp_lexeme == "-." && state == ERROR_T)
	  {
	    if(tmp_lexeme[0] == '-')
	      {
		state = MIN_T;
		tmp_lexeme.pop_back();
		token = (token_type)state;
		pos--;
		pos--;
		break;
	      }
	  }*/
	// we hit a non-backup accepting state
	else if (state >= 200){
	  token = (token_type)state;
	  break;
	}
	// we hit a backup accepting state
	else if (state >=100){
	  token = (token_type)state;
	  if(c != ' '){
	    tmp_lexeme.pop_back();
	  }
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
	  {
	    token = IDENT_T;
	  }
    }
    // check if its actually a predicate or an IDEN_T + ?
    else if (token == PREDICATE){
        if(predicates.find(tmp_lexeme) != predicates.end()){
            token = predicates[tmp_lexeme];
        }
	else
	  {
	    pos--;
	    tmp_lexeme.pop_back();
	    if (keywords.find(tmp_lexeme) != keywords.end())
	      {
		//pos--;
		//tmp_lexeme.pop_back();
		token = keywords[tmp_lexeme];
	      }
	    else
	      {
		//pos--; // backup on the ?
		//tmp_lexeme.pop_back();
		token = IDENT_T;
	      }
	  }
        /*else{
            pos--; // backup on the ?
            tmp_lexeme.pop_back();
            token = IDENT_T;
        }*/
    }
    else if(state == 10)
      {
	errors++;
	errorMsg += "Error at " + to_string(linenum) + "," + to_string(pos) + ": Invalid character found: " + tmp_lexeme;
	token = (token_type)ERROR_T;
      }

    if(token == IDENT_T && tmp_lexeme.back() == '.')
        tmp_lexeme.pop_back();

    if(!tmp_lexeme.empty())
      tokenFile << "\t" << left << setw(16) <<  this->GetTokenName(token) << tmp_lexeme << endl;

	lexeme = tmp_lexeme;


	return token;
}

string LexicalAnalyzer::GetTokenName (token_type t)
{
	// The GetTokenName function returns a string containing the name of the
	// token passed to it.
  return token_names[t];
}

string LexicalAnalyzer::GetLexeme () const
{
	// This function will return the lexeme found by the most recent call to
	// the get_token function
    return lexeme;
}

void LexicalAnalyzer::ReportError (const string & msg)
{
	// This function will be called to write an error message to a file
  listingFile << msg << endl;
}

string LexicalAnalyzer::trimExtension(string s) {
    string tmp(s);
    int extensionLoc = tmp.find(".");
    tmp = tmp.erase(extensionLoc);
    return tmp;
}







void LexicalAnalyzer::initTokenToName() {
    token_names[LISTOP_T] = "LISTOP_T";
    token_names[KEYWORD_T] = "KEYWORD_T";
    token_names[IDENT_T] = "IDENT_T";
    token_names[PLUS_T] = "PLUS_T";
    token_names[MINUS_T] = "MINUS_T";
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
    token_names[EOF_T] = "EOF_T";
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
    keywords["modulo"] = MODULO_T;
    keywords["round"] = ROUND_T;
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

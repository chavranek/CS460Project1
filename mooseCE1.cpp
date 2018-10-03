// CS 460 - Class Exercise 1
// File:
// Authors: Christian Havranek, Eduardo Roman
//
/*
	Description
*/

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>

using namespace std;

void breakup (const string & code, ostream & output);
int get_col(char  position);

int main (int argc, char * argv [])
{
	if (argc < 2)
	{	
		cerr << "Usage: " << argv[0] << " <filename>\n";
		exit (1);
	}
	ifstream infile (argv[1]);
	if (infile.fail())
	{
		cerr << "File: " << argv[1] << " not found.\n";
		exit (2);
	}
	string line;
	int lineno = 1;
	while (getline (infile, line))
	{
	  cout << lineno++ << ". " << line << endl;
	  breakup (line, cout);
	}
	infile.close();
	return 0;
}

void breakup (const string & line, ostream & output)
{
  
  
  //vector<string> line;

  // this is the state table for our lexical analyzer. values less than or equal to 20 are valid states for an operator, number
  // or word. 21 represents a back up, whiile 22 represents a good state.
  vector<vector<int>> table = {{1, 1, 2, 5, 16, 6, 7, 8, 9, 10, 11, 12, 14, 13, 20, 22, 22, 22, 22, 22, 22, 22, 22, 17, 22},
			       {1, 1, 1, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21},
			       {21, 21, 2, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 3, 21, 21, 21, 21, 21},
			       {21, 21, 4, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21},
			       {21, 21, 4, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21},
			       {21, 21, 21, 22, 22, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21},
			       {21, 21, 21, 21, 22, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21},
			       {21, 21, 21, 21, 22, 21, 22, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21},
			       {21, 21, 21, 21, 22, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21},
			       {21, 21, 21, 21, 22, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21},
			       {21, 21, 21, 21, 22, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21},
			       {21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 22, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21},
			       {21, 21, 21, 21, 22, 21, 21, 21, 21, 21, 21, 22, 22, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21},
			       {21, 21, 21, 21, 22, 21, 21, 21, 21, 21, 21, 21, 21, 22, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21},
			       {21, 21, 21, 21, 22, 21, 21, 21, 21, 21, 21, 21, 15, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21},
			       {21, 21, 21, 21, 22, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21},
			       {21, 21, 21, 21, 22, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21},
			       {21, 21, 21, 21, 18, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 19, 21},
			       {21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 22, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21},
			       {21, 21, 21, 21, 22, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21},
			       {21, 21, 21, 21, 22, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21}};

  
  //stringstream stream(code);
  //string line;
  int pos = 0;

  while(pos < line.size())
    {
      int state = 0;
      int col = 0;
      string lexeme = "";
      while(state < 21)
	{
	  if (line[pos] != ' ')
	    {
	      lexeme += line[pos];
	      col = get_col(line[pos]);
	      state = table[state][col];
	    }
	  else if (line[pos] == ' ')
	    {
	      state = 22;
	    }
	  pos++;
	}
      if (state == 21)
	{
	  pos--;
	  lexeme.pop_back();
	  cout << lexeme << endl;
	}
      
      else if(state == 22)
	{
	  if (lexeme != "")
	    {
	      cout << lexeme << endl;
	    }
	}
    }
}

int get_col(char  position)
{
      if (isalpha(position))
	{return 0;
	}
      else if (position == '_')
	{return 1;
	}
      else if (isdigit(position))
	{ return 2;
	}
      else if (position == '+')
	{return 3;
	}
      else if (position == '=')
	{return 4;
	}
      else if (position == '%')
	{return 5;
	}
      else if (position == '&')
	{return 6;
	}
      else if (position == '*')
	{return 7;
	}
      else if (position == '/')
	{return 8;
	}
      else if (position == '!')
	{return 9;
	}
      else if (position == ':')
	{return 10;
	}
      else if (position == '-')
	{return 11;
	}
      else if (position == '>')
	{return 12;
	}
      else if (position == '|')
	{return 13;
	}
      else if (position == '^')
	{return 14;
	}
      else if (position == ']')
	{return 15;
	}
      else if (position == '[')
	{return 16;
	}
      else if (position == ')')
	{return 17;
	}
      else if (position == '(')
	{return 18;
	}
      else if (position == '.')
	{return 19;
	}
      else if (position == '?')
	{return 20;
	}
      else if (position == ',')
	{return 21;
	}
      else if (position == '~')
	{return 22;
	}
      else if (position == '<')
	{return 23;
	}
      else if (position == ';')
	{return 24;
	}
    }

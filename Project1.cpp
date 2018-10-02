#include <cstdlib>
#include <iostream>
#include <iomanip>
#include "SetLimits.h"
#include "SyntacticalAnalyzer.h"

int main (int argc, char * argv[])
{
	if (argc < 2)
	{
		printf ("format: proj1 <filename>\n");
		exit (1);
	}
	SetLimits ();
	//SyntacticalAnalyzer parser (argv[1]);
    LexicalAnalyzer lex(argv[1]);


    lex.GetToken();


    cout << NUM_TOKENS << endl;
 

	return 0;
}

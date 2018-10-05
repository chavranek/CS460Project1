P1.out : Project1.o SetLimits.o LexicalAnalyzer.o SyntacticalAnalyzer.o
	g++ -g -o P1.out Project1.o SetLimits.o LexicalAnalyzer.o SyntacticalAnalyzer.o

Project1.o : Project1.cpp SetLimits.h SyntacticalAnalyzer.h
	g++ -g -c Project1.cpp

SetLimits.o : SetLimits.cpp SetLimits.h
	g++ -g -c SetLimits.cpp

LexicalAnalyzer.o : LexicalAnalyzer.cpp LexicalAnalyzer.h
	g++ -g -c -std=c++11 LexicalAnalyzer.cpp

SyntacticalAnalyzer.o : SyntacticalAnalyzer.cpp SyntacticalAnalyzer.h LexicalAnalyzer.h
	g++ -g -c SyntacticalAnalyzer.cpp


.PHONY: clean
clean:
	@rm -f *.o P1.out *.gch *.lst *.p1 *.dbg

run: P1.out
	./P1.out P1-0.ss

submit : Project1.cpp LexicalAnalyzer.h LexicalAnalyzer.cpp SyntacticalAnalyzer.h SyntacticalAnalyzer.cpp makefile README.txt
	rm -rf TeamhP1
	mkdir TeamhP1
	cp Project1.cpp TeamhP1
	cp LexicalAnalyzer.h TeamhP1
	cp LexicalAnalyzer.cpp TeamhP1
	cp SyntacticalAnalyzer.h TeamhP1
	cp SyntacticalAnalyzer.cpp TeamhP1
	cp makefile TeamhP1
	cp README.txt TeamhP1
	tar cfvz TeamhP1.tgz TeamhP1
	cp TeamhP1.tgz ~tiawatts/cs460drop

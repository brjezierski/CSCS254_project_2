# Trivial makefile for the calculator scanner/parser.
# Depends on default (built-in) rules for C compilation.

# Note that rule for goal (parse) must be the first one in this file.


parse: parse.o scan.o
	g++ -std=c++11 -Wall parse.o scan.o -o parse
	#g++ -o parse parse.o scan.o

clean:
	rm -f parse.o parse

parse.o: scanpp.h parse.cpp 
	#g++ -c parse.cpp
	
	g++ -c -std=c++11 parse.cpp

scan.o: scanpp.h scan.cpp 
	#g++ -c scan.cpp
	g++ -c -std=c++11 scan.cpp



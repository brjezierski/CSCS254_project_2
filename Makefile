# Makefile for the calculator scanner/parser.
# Depends on default (built-in) rules for C++ compilation.


run: parse
	./parse < test.txt

parse: parse.o scan.o
	g++ -std=c++11 -Wall parse.o scan.o -o parse 


clean:
	rm -f parse.o parse

parse.o: scanpp.h parse.cpp 

	g++ -c -std=c++11 parse.cpp

scan.o: scanpp.h scan.cpp 

	g++ -c -std=c++11 scan.cpp




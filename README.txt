
Bartlomiej Jezierski
Mazen Meziad
mmeziad

The code can be ran by writing in the correct folder the following:
$make

In order to change the test file, go to the Makefile and change the piped file number

Note:
'$' can not be allowed in the language, because we have used it to determine the end of file. The reason why we did that is because, if you use cin.get(), instead of getchar(), then when the end of the standard input comes, the value of c is not EOF, which means that we need to define another symbol, which will mark the end of the file. 

The syntax_error messages refer to the characters not being in the follow set of the token, which means that there are cases in which the program will return an error, if it is a scanning error or matching error, but that is outside of the scope of this project.


Makefile
runs parse file and pipes a test file into it.
We use the c++11 version, as approved by the professor on piazza.



parse.cpp

Used try and catch, as well as throw to get errors in grammar. The error gets printed with its corresponding enum number originally (can be found in header file). The checker finds out whether the input_token is in the follow set of the grammar by iterating through the hard-coded follow set. Every time the follow set finds an error it pushes it into a list of errors.

After reaching the end of the program and printing the tree, if there are errors, the list calls converter(), which returns the corresponding token name, instead of just the enum value. 



test2.txt
contains the correct calculator grammar

test3.txt
The simplest correct program possible in the grammar

test4.txt
It has a '+' instead of n after 'while' line 3

test5.txt
It has a '+' instead of n after 'while' line 3
There is an incorrect ')' on line 15
There is an incorrect '(' on line 22


Bonus:
1) Exception-based error recovery in all non-terminals
2) Printing errors and error name at the end by creating a list of error causing tokens and then converting them to the correct output, instead of just the enum number, which is the default.

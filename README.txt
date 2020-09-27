Mazen Meziad
Bartlomiej Jezierski
Project 2

To execute the program use the 'make' command.
Our parser takes a text file with the program as an input file (default from test.txt). A dollar sign is required in the end of the file to mark the end of the program. In order to use a different input file, test.txt needs to be substituted with a different file name in Makefile. Other test files available are:
* Test2.txt - tests if-else functionality


When syntax errors occur, they are printed during the parsing as well as in the end of the output.
Both production rules and the syntax tree are printed to standard output and the syntax tree is only printed when no errors occur.


Extra Credit:
* The syntax tree is also printed in the tabulated form to better depict the structure
* If-else extension to if statement is added and so the following grammar rules are changed
	S	→ 	id := E  |  read id  |  write E  |  if C SL CT  |  while C SL end
	CT	→ 	end | else SL end

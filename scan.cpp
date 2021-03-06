
#include <string>
#include <locale>
#include <iostream>
#include <fstream>
#include "scanpp.h"


char token_image[MAX_TOKEN_LEN];

/*
    The problem is that reading from a file doesn't save, which line we were last at,
    because we call scan from different places instead of just one

    We also don't want to pass that information every time we scan.

    How can we scan from a file, line by line while knowing which line we were at before?
    
    Note:
    There doesn't seem to be a built in method for this exactly

    Suggestions: 
    1) parse file into arrays or maps and then save what column you were at in the array
            This will help with printing, which line the error occured on
    2) Make scanner get as input, which line it was last on
            Line by line might be fine, but this will cause problems for each char in a line
    3)Make an in between function that gets called when you wanna scan smth and keep 
    track of counter there




*/



token scan() {
      static char c = ' ';
    //                              TRY READING FROM INSIDE THIS FUNCTION


    // std::string line;
    //std::ifstream file("test2");
    // file.open("test2");


    // if(file){
    //     file.getc();
    //     std::cout<<c<<"\n";
    // }
    // getline(inFile,line);
    // std::cout<<line<<"\n";

    // std::string line;
    // if(inFile.is_open()){
    //     while (getline(inFile,line)){
    //         std::cout << line << "\n";
    //     }
    //     inFile.close();
    // }else std::cout << "Problem opening file\n"; 

  
        /* next available char; extra (int) width accommodates EOF */
    int i = 0;              /* index into token_image */
    /* skip white space */
    while (isspace(c)) {

        std::cin.get(c);
        //std::cout<<c<<"\n";
    	//c = getchar();
    }



    if (c == EOF)
        return t_eof;

    if (isalpha(c)) {
        do {
            token_image[i++] = c;
            if (i >= MAX_TOKEN_LEN) {
            	std::cout << "max token length exceeded\n";
                exit(1);
            }
            std::cin.get(c);
            //c = getchar();
        } while (isalpha(c) || isdigit(c) || c == '_');
        token_image[i] = '\0';
        if (!strcmp(token_image, "read")) return t_read;
        else if (!strcmp(token_image, "write")) return t_write;
        else if (!strcmp(token_image, "if")) return t_if;
        else if (!strcmp(token_image, "while")) return t_while;
        else if (!strcmp(token_image, "end")) return t_end;
        else return t_id;
    }
    else if (isdigit(c)) {
        do {
            token_image[i++] = c;
            std::cin.get(c);
            //c = getchar();
        } while (isdigit(c));
        token_image[i] = '\0';
        return t_literal;
    } else switch (c) {
        case ':':
            std::cin.get(c);
            if ((c) != '=') {
            //if ((c = getchar()) != '=') {
                
            	std:: cout<< stderr<< "error\n";
                exit(1);
            } else {
                std::cin.get(c);
                //c = getchar();
                return t_gets;
            }
            break;
        case '(': std::cin.get(c); return t_lparen;
        case ')': std::cin.get(c); return t_rparen;
        case '+': std::cin.get(c); return t_add;
        case '-': std::cin.get(c); return t_sub;
        case '*': std::cin.get(c); return t_mul;
        case '/': std::cin.get(c); return t_div;
        case '=': std::cin.get(c); return t_eq;
        // case '(': c = getchar(); return t_lparen;
        // case ')': c = getchar(); return t_rparen;
        // case '+': c = getchar(); return t_add;
        // case '-': c = getchar(); return t_sub;
        // case '*': c = getchar(); return t_mul;
        // case '/': c = getchar(); return t_div;
        // case '=': c = getchar(); return t_eq;
        case '<':
            std::cin.get(c);
            if ((c) == '>') {
                std::cin.get(c);
            //if ((c = getchar()) == '>') {
            //    c = getchar();
                return t_neq;
            }
            
            // else if ((c = getchar()) == '=') {
            //     c = getchar();
            
            else if ((c) == '=') {
                std::cin.get(c);

                return t_lte;
            }else {
                // TODO : should return an error?
                //std::cin.get(c);
            	return t_lt;
            }
            break;
        case '>':
            std::cin.get(c);
            if ((c) != '=') {
                // TODO : should return an error?
                std::cin.get(c);
            	return t_gt;
            } else {
                //std::cin.get(c);
                return t_gte;
            }
            break;
        default:
        	std:: cout<<"error in scan.cpp\n";
            exit(1);
    }
}

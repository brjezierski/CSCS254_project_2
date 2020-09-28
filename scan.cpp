
#include <string>
#include <locale>
#include <iostream>
#include <fstream>
#include "scanpp.h"


char token_image[MAX_TOKEN_LEN];


/*
    Scans the character input and returns corresponding tokens using C++ Standard I/O
*/
token scan() {
    static char c = ' ';
        /* next available char; extra (int) width accommodates EOF */
    int i = 0;              /* index into token_image */
    /* skip white space */
    while (isspace(c)) {

        std::cin.get(c);
    }


    if (c == '$') {
        return t_eof;
    }

    if (isalpha(c)) {
        do {
            token_image[i++] = c;
            if (i >= MAX_TOKEN_LEN) {
            	std::cout << "max token length exceeded\n";
                exit(1);
            }
            std::cin.get(c);
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
        } while (isdigit(c));
        token_image[i] = '\0';
        return t_literal;
    } else switch (c) {
        case ':':
            std::cin.get(c);
            if ((c) != '=') {
            	std:: cout<< stderr<< "error in input of gets in scanner\n";
                exit(1);
            } else {
                std::cin.get(c);
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

        case '<':
            std::cin.get(c);
            if ((c) == '>') {
                std::cin.get(c);
                return t_neq;
            }
            

            
            else if ((c) == '=') {
                std::cin.get(c);

                return t_lte;
            }else {

            	return t_lt;
            }
            break;
        case '>':
            std::cin.get(c);
            if ((c) != '=') {
                std::cin.get(c);
            	return t_gt;
            } else {
                return t_gte;
            }
            break;
        default:
        	std:: cout<<"error in scan.cpp\n";
            exit(1);
    }
}

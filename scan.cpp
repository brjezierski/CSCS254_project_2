//#include <cstdio>
//#include <cstdlib>
#include <cstring>
#include <cctype>
#include <iostream>
#include "scanpp.h"

char token_image[MAX_TOKEN_LEN];

token scan() {
    static int c = ' ';
        /* next available char; extra (int) width accommodates EOF */
    int i = 0;              /* index into token_image */
    /* skip white space */
    while (isspace(c)) {
    	c = getchar();
    }

    if (c == EOF)
        return t_eof;

    if (isalpha(c)) {
        do {
            token_image[i++] = c;
            if (i >= MAX_TOKEN_LEN) {
            	std::cout << "max token length exceeded\n";
                //printf("max token length exceeded\n");
                exit(1);
            }
            c = getchar();
        } while (isalpha(c) || isdigit(c) || c == '_');
        token_image[i] = '\0';
        if (!strcmp(token_image, "read")) return t_read;
        else if (!strcmp(token_image, "write")) return t_write;
        else return t_id;
    }
    else if (isdigit(c)) {
        do {
            token_image[i++] = c;
            c = getchar();
        } while (isdigit(c));
        token_image[i] = '\0';
        return t_literal;
    } else switch (c) {
        case ':':
            if ((c = getchar()) != '=') {
            	std:: cout<< stderr<< "error\n";
                //fprintf(stderr, "error\n");
                exit(1);
            } else {
                c = getchar();
                return t_gets;
            }
            break;
        case '(': c = getchar(); return t_lparen;
        case ')': c = getchar(); return t_rparen;
        case '+': c = getchar(); return t_add;
        case '-': c = getchar(); return t_sub;
        case '*': c = getchar(); return t_mul;
        case '/': c = getchar(); return t_div;
        default:
        	std:: cout<<"error\n";
           // printf("error\n");
            exit(1);
    }
}

// int main () {
//     input_token = scan();
//     //program ();
//     return 0;
// }
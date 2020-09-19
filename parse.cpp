/* Complete recursive descent parser for the calculator language.
    Builds on figure 2.16.  Prints a trace of productions predicted and
    tokens matched.  Does no error recovery: prints "syntax error" and
    dies on invalid input.
    Michael L. Scott, 2008-2020.
*/

#include <cstdio>
#include <cstdlib>

#include <iostream>
#include "scanpp.h"
#include <string>

using namespace std;

const char* names[] = {"read", "write", "id", "literal", "gets",
                       "add", "sub", "mul", "div", "lparen", "rparen", "eof",
                       "if", "end", "eq", "neq", "lt", "gt", "lte", "gte"};

static token input_token;

void error () {
    cout << "syntax error\n";
    exit (1);
}

void match (token expected) {
    if (input_token == expected) {
        cout << "matched " << names[input_token];
        if (input_token == t_id || input_token == t_literal)
            cout << ": " << token_image;
        cout << "\n";
        input_token = scan();
    }
    else {
        cout << "match error\n";
        error ();
    }
}

void program ();
void stmt_list ();
void stmt ();
void expr ();
void term ();
void term_tail ();
void factor ();
void factor_tail ();
void add_op ();
void mul_op ();
void cond ();
void rel_op ();

void program () {
    switch (input_token) {
        case t_id:
        case t_read:
        case t_write:
        case t_eof:
        case t_if:
        case t_while:
            cout << "predict program --> stmt_list eof\n";
            stmt_list ();
            match (t_eof);
            break;
        default:
            cout << "program error\n";
            error ();
    }

}

void stmt_list () {
    switch (input_token) {
        case t_id:
        case t_read:
        case t_write:
        case t_if:
        case t_while:
            cout << "predict stmt_list --> stmt stmt_list\n";
            stmt ();
            stmt_list ();
            break;
        case t_eof:
        case t_end:
            cout << "predict stmt_list --> epsilon\n";
            break;          /* epsilon production */
        default:
            cout << "stmt_list error\n";
            error ();
    }
}

void stmt () {
    switch (input_token) {
        case t_id:
            cout << "predict stmt --> id gets expr\n";
            match (t_id);
            match (t_gets);
            expr ();
            break;
        case t_read:
            cout << "predict stmt --> read id\n";
            match (t_read);
            match (t_id);
            break;
        case t_write:
            cout << "predict stmt --> write expr\n";
            match (t_write);
            expr ();
            break;
        case t_if:
            cout << "predict stmt --> if cond stmt_list end\n";
            match (t_if);
            cond ();
            stmt_list ();
            match (t_end);
            break;
        case t_while:
            cout << "predict stmt --> while cond stmt_list end\n";
            match (t_while);
            cond ();
            stmt_list ();
            match (t_end);
            break;
        default:
            cout << "stmt error\n";
            error ();
    }
}

void expr () {
    switch (input_token) {
        case t_id:
        case t_literal:
        case t_lparen:
            cout << "predict expr --> term term_tail\n";
            term ();
            term_tail ();
            break;
        default:
            cout << "syntax error\n";
            error ();
    }
}

void term () {
    switch (input_token) {
        case t_id:
        case t_literal:
        case t_lparen:
            cout << "predict term --> factor factor_tail\n";
            factor ();
            factor_tail ();
            break;
        default:
            cout << "expr error\n";
            error ();
    }
}

void term_tail () {
    switch (input_token) {
        case t_add:
        case t_sub:
            cout << "predict term_tail --> add_op term term_tail\n";
            add_op ();
            term ();
            term_tail ();
            break;
        case t_rparen:
        case t_id:
        case t_read:
        case t_write:
        case t_if:
        case t_while:
        case t_eof:
        case t_end:
        case t_eq:
        case t_neq:
        case t_lt:
        case t_gt:
        case t_lte:
        case t_gte:
            cout << "predict term_tail --> epsilon\n";
            break;          /* epsilon production */
        default:
            cout << "term_tail error\n";
            error ();
    }
}

void factor () {
    switch (input_token) {
        case t_literal:
            cout << "predict factor --> literal\n";
            match (t_literal);
            break;
        case t_id :
            cout << "predict factor --> id\n";
            match (t_id);
            break;
        case t_lparen:
            cout << "predict factor --> lparen expr rparen\n";
            match (t_lparen);
            expr ();
            match (t_rparen);
            break;
        default:
            cout << "factor error\n";
            error ();
    }
}

void factor_tail () {
    switch (input_token) {
        case t_mul:
        case t_div:
            cout << "predict factor_tail --> mul_op factor factor_tail\n";
            mul_op ();
            factor ();
            factor_tail ();
            break;
        case t_add:
        case t_sub:
        case t_rparen:
        case t_id:
        case t_read:
        case t_write:
        case t_if:
        case t_while:
        case t_eof:
        case t_end:
        case t_eq:
        case t_neq:
        case t_lt:
        case t_gt:
        case t_lte:
        case t_gte:
            cout << "predict factor_tail --> epsilon\n";
            break;          /* epsilon production */
        default:
            cout << "factor_tail error\n";
            error ();
    }
}

void add_op () {
    switch (input_token) {
        case t_add:
            cout << "predict add_op --> add\n";
            match (t_add);
            break;
        case t_sub:
            cout << "predict add_op --> sub\n";
            match (t_sub);
            break;
        default:
            cout << "add_op error\n";
            error ();
    }
}

void mul_op () {
    switch (input_token) {
        case t_mul:
            cout << "predict mul_op --> mul\n";
            match (t_mul);
            break;
        case t_div:
            cout << "predict mul_op --> div\n";
            match (t_div);
            break;
        default:
            cout << "mul_op error\n";
            error ();
    }
}

void cond () {
    switch (input_token) {
        case t_id:
        case t_literal:
        case t_lparen:
            cout << "predict cond --> expr rel_op expr\n";
            expr ();
            rel_op ();
            expr ();
            break;
        default:
            cout << "cond error\n";
            error ();
    }
}

void rel_op () {
    switch (input_token) {
        case t_eq:
            cout << "predict rel_op --> eq\n";
            match (t_eq);
            break;
        case t_neq:
            cout << "predict rel_op --> neq\n";
            match (t_neq);
            break;
        case t_lt:
            cout << "predict rel_op --> lt\n";
            match (t_lt);
            break;
        case t_gt:
            cout << "predict rel_op --> gt\n";
            match (t_gt);
            break;
        case t_lte:
            cout << "predict rel_op --> lte\n";
            match (t_lte);
            break;
        case t_gte:
            cout << "predict rel_op --> gte\n";
            match (t_gte);
            break;
        default:
            cout << "rel_op error\n";
            error ();
    }
}

int main () {
    input_token = scan();
    program();
    return 0;
}

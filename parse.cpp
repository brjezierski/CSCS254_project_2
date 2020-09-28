/* Complete recursive descent parser for the calculator language.
    Builds on figure 2.16.  Prints a trace of productions predicted and
    tokens matched.  Does no error recovery: prints "syntax error" and
    dies on invalid input.
    Michael L. Scott, 2008-2020.
*/

#include <set>
#include <string>
#include <stdexcept>
#include <iostream>
#include <algorithm>
#include <stdexcept>
#include <fstream>
#include <list>
#include <iterator>


#include "scanpp.h"
#include <string>
#include <vector> 

using namespace std;


/*
    Hard-coded FOLLOW sets for the different non-terminals
*/
set<token> condSet={ t_id, t_read, t_write, t_if, t_while, t_end, t_eof};
set<token> exprSet={t_id, t_read, t_write, t_if, t_while, t_end, t_eof, t_eq,t_neq,t_lt,t_gt,t_lte,t_gte,t_rparen};
set<token> stmtSet={ t_id, t_read, t_write, t_if, t_while,t_eof,t_end};

set<token> stmtlistSet={t_eof, t_end};
set<token> termSet={t_add, t_sub,t_id, t_read,t_write,t_if,t_while,t_eof,t_eq,t_neq,t_lt,t_gt,t_lte,t_gte,t_end,t_rparen};
set<token> factorSet={t_mul,t_div,t_add,t_sub,t_id,t_read,t_write,t_if, t_while,t_eof,t_eq,t_neq,t_lt,t_gt,t_lte,t_gte,t_end,t_rparen};
set<token> termtailSet={t_id,t_read,t_write,t_if,t_while,t_eof,t_eq,t_neq,t_lt,t_gt,t_lte,t_gte,t_end,t_rparen};
set<token> factortailSet={t_add,t_sub, t_id,t_read,t_write,t_if,t_while,t_eof,t_eq,t_neq,t_lt,t_gt,t_lte,t_gte,t_end,t_rparen};
set<token> roSet={t_lparen,t_id,t_literal};
set<token> aoSet={t_lparen,t_id,t_literal};
set<token> moSet={t_lparen,t_id,t_literal};

//Stores errors 
static list <token> errorList;

Node* rootNode;

   
struct Node 
{
    string token; 
    vector<Node *> child; 
}; 
   
Node *newNode(string token) 
{ 
    Node *temp = new Node; 
    temp->token = token; 
    return temp; 
} 

const char* names[] = {"read", "write", "id", "literal", "gets",
                       "add", "sub", "mul", "div", "lparen", "rparen", "eof",
                       "if", "end", "eq", "neq", "lt", "gt", "lte", "gte"};


static token input_token;

/* Print nodes in preorder*/
void printPreorder(Node* tree) 
{ 
    if (tree == NULL) {
        return; 
    }
    cout << tree->token << " "; 
    if (tree->child.size() == 0) {
        cout << "()";
    }
    for (int i=0; i<tree->child.size(); i++) {
        if (i == 0) {
            cout << "(";
        }
        printPreorder(tree->child[i]); 
        if (i == tree->child.size()-1) {
            cout << ")";
        }   
    }
}  

void printTree(Node* tree) {
    cout << "(";
    printPreorder(tree);
    cout << ")";
}

/* Print nodes in preorder */
void printPreorderIdent(Node* tree, int identation) 
{
    if (tree == NULL) {
        return; 
    }
    cout << "\n";
    for (int i=0; i<identation; i++) {
        cout << "\t";
    }
    cout << tree->token << " "; 
    if (tree->child.size() == 0) {
        cout << "()";
    }
    for (int i=0; i<tree->child.size(); i++) {
        if (i == 0) {
            cout << "(";
        }
        printPreorderIdent(tree->child[i], identation+1); 
        if (i == tree->child.size()-1) {
            cout << ")";
        }   
    }
}

void error () {
    cout << "syntax error\n";
    exit (1);
}


void match (token expected);
void program (Node* tree);
void stmt_list (Node* tree);
void stmt (Node* tree);
void expr (Node* tree);
void term (Node* tree);
void term_tail (Node* tree);
void factor (Node* tree);
void factor_tail (Node* tree);
void add_op (Node* tree);
void mul_op (Node* tree);
void cond (Node* tree);
void rel_op (Node* tree);

void match (token expected) {
    if (input_token == expected) {
        cout << "matched " << names[input_token];
        if (input_token == t_id || input_token == t_literal)
            cout << ": " << token_image;
        cout << "\n";
        input_token = scan();
    }
    else {
        cout << "error in MATCH\n";
        error ();
    }
}

void program (Node* tree) {
    tree->token="P";
    rootNode = tree;
    try{
        switch (input_token) {
        case t_id:
        case t_read:
        case t_write:
        case t_eof:
        case t_if:
        case t_while:
            cout << "predict program --> stmt_list eof\n";
            (tree->child).push_back(newNode("SL"));
            stmt_list (tree->child[(tree->child).size()-1]);
            (tree->child).push_back(newNode("$$"));
            match (t_eof);
            break;
        default:
            // cout << "program error\n";
            // error ();
            throw std::runtime_error("exception thrown in program\n");
        }
    }catch(std:: exception const& except){
        cout << "program error\n";

        while(!stmtSet.count(input_token)){

            cout<<"Syntax error in program: "<<input_token<<"\n\n";
            errorList.push_back(input_token);
            input_token=scan();
        }

    }
    
    printTree(rootNode);
}

void stmt_list (Node* tree) {
    try{
        switch (input_token) {
        case t_id:
        case t_read:
        case t_write:
        case t_if:
        case t_while:
            cout << "predict stmt_list --> stmt stmt_list\n";
            (tree->child).push_back(newNode("S"));
            stmt (tree->child[(tree->child).size()-1]);
            (tree->child).push_back(newNode("SL"));
            stmt_list (tree->child[(tree->child).size()-1]);
            break;
        case t_eof:
        case t_end:
            cout << "predict stmt_list --> epsilon\n";
            (tree->child).push_back(newNode("epsilon"));
            break;          /* epsilon production */
        default:
            // cout << "stmt_list error\n";
            // error ();
            throw std::runtime_error("exception thrown in statement list\n");
        }
    }catch(std:: exception const& except){
        cout << "stmt list error\n";

        while(!stmtSet.count(input_token)){

            cout<<"Syntax error in statement list: "<<input_token<<"\n\n";
            errorList.push_back(input_token);
            input_token=scan();
        }

    }
    
}

void stmt (Node* tree) {
    try{
        switch (input_token) {
        case t_id:
            cout << "predict stmt --> id gets expr\n";
            (tree->child).push_back(newNode(token_image));
            match (t_id);
            (tree->child).push_back(newNode(":="));
            match (t_gets);
            (tree->child).push_back(newNode("E"));
            expr (tree->child[(tree->child).size()-1]);
            break;
        case t_read:
            cout << "predict stmt --> read id\n";
            (tree->child).push_back(newNode("read"));
            match (t_read);
            (tree->child).push_back(newNode(token_image));
            match (t_id);
            break;
        case t_write:
            cout << "predict stmt --> write expr\n";
            (tree->child).push_back(newNode("write"));
            match (t_write);
            (tree->child).push_back(newNode("E"));
            expr (tree->child[(tree->child).size()-1]);
            break;
        case t_if:
            cout << "predict stmt --> if cond stmt_list end\n";
            (tree->child).push_back(newNode("if"));
            match (t_if);
            (tree->child).push_back(newNode("C"));
            cond (tree->child[(tree->child).size()-1]);
            (tree->child).push_back(newNode("SL"));
            stmt_list (tree->child[(tree->child).size()-1]);
            (tree->child).push_back(newNode("end"));
            match (t_end);
            break;
        case t_while:
            cout << "predict stmt --> while cond stmt_list end\n";
            (tree->child).push_back(newNode("while"));
            match (t_while);
            (tree->child).push_back(newNode("C"));
            cond (tree->child[(tree->child).size()-1]);
            (tree->child).push_back(newNode("SL"));
            stmt_list (tree->child[(tree->child).size()-1]);
            (tree->child).push_back(newNode("end"));
            match (t_end);
            break;
        default:
            //throw 'e';
            //cout << "stmt error\n";
            //error ();
            throw std::runtime_error("exception thrown in stmt\n");

        }
    }catch(std:: exception const& except){
        cout << "stmt error\n";

        while(!stmtSet.count(input_token)){

            cout<<"Syntax error in statement: "<<input_token<<"\n\n";
            errorList.push_back(input_token);
            input_token=scan();
        }

    }
    
}

void expr (Node* tree) {              //add handler
    try{
        switch (input_token) {
        case t_id:
        case t_literal:
        case t_lparen:
            cout << "predict expr --> term term_tail\n";
            (tree->child).push_back(newNode("T"));
            term (tree->child[(tree->child).size()-1]);
            (tree->child).push_back(newNode("TT"));
            term_tail (tree->child[(tree->child).size()-1]);
            break;
        default:
            //throw 'e';
            //cout << "syntax error\n";
            //error ();
            throw std::runtime_error("exception thrown in expr\n");

        }
    }catch(std:: exception const& except){
        cout << "expr error\n";

        while(!exprSet.count(input_token)){

            cout<<"Syntax error in expression: "<<input_token<<"\n\n";
            errorList.push_back(input_token);
            input_token=scan();
        }
    }
    
}

void term (Node* tree) {
    try{
        switch (input_token) {
        case t_id:
        case t_literal:
        case t_lparen:
            cout << "predict term --> factor factor_tail\n";
            (tree->child).push_back(newNode("F"));
            factor (tree->child[(tree->child).size()-1]);
            (tree->child).push_back(newNode("FT"));
            factor_tail (tree->child[(tree->child).size()-1]);
            break;
        default:
            // cout << "expr error\n";
            // error ();
            throw std::runtime_error("exception thrown in term\n");
        }
    }catch(std:: exception const& except){

        while(!condSet.count(input_token)){
            cout<<"Syntax error in term non-terminal: "<<input_token<<"\n\n";
            //cout<<"IN COND while loop AIIII\n"<<token[input_token]<<"\n\n";
            errorList.push_back(input_token);
            //converter(input_token);
            input_token=scan();
        }
    }
    
}

void term_tail (Node* tree) {
    try{
        switch (input_token) {
        case t_add:
        case t_sub:
            cout << "predict term_tail --> add_op term term_tail\n";
            (tree->child).push_back(newNode("ao"));
            add_op (tree->child[(tree->child).size()-1]);
            (tree->child).push_back(newNode("T"));
            term (tree->child[(tree->child).size()-1]);
            (tree->child).push_back(newNode("TT"));
            term_tail (tree->child[(tree->child).size()-1]);
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
            (tree->child).push_back(newNode("epsilon"));
            break;          /* epsilon production */
        default:
            // cout << "term_tail error\n";
            // error ();
            throw std::runtime_error("exception thrown in term tail\n");
        }
    }catch(std:: exception const& except){

        while(!condSet.count(input_token)){
            cout<<"Syntax error in term tail non-terminal: "<<input_token<<"\n\n";
            //cout<<"IN COND while loop AIIII\n"<<token[input_token]<<"\n\n";
            errorList.push_back(input_token);
            //converter(input_token);
            input_token=scan();
        }
    }
    
}

void factor (Node* tree) {
    try{
        switch (input_token) {
        case t_literal:
            cout << "predict factor --> literal\n";
            (tree->child).push_back(newNode(token_image));
            match (t_literal);
            break;
        case t_id :
            cout << "predict factor --> id\n";
            (tree->child).push_back(newNode(token_image));
            match (t_id);
            break;
        case t_lparen:
            cout << "predict factor --> lparen expr rparen\n";
            (tree->child).push_back(newNode("("));
            match (t_lparen);
            (tree->child).push_back(newNode("T"));
            expr (tree->child[(tree->child).size()-1]);
            (tree->child).push_back(newNode(")"));
            match (t_rparen);
            break;
        default:
            // cout << "factor error\n";
            // error ();
        throw std::runtime_error("exception thrown in factor\n");
        }
    }catch(std:: exception const& except){

        while(!condSet.count(input_token)){
            cout<<"Syntax error in factor non-terminal: "<<input_token<<"\n\n";
            //cout<<"IN COND while loop AIIII\n"<<token[input_token]<<"\n\n";
            errorList.push_back(input_token);
            //converter(input_token);
            input_token=scan();
        }
    }
    

}

void factor_tail (Node* tree) {
    try{
        switch (input_token) {
        case t_mul:
        case t_div:
            cout << "predict factor_tail --> mul_op factor factor_tail\n";
            (tree->child).push_back(newNode("mo"));
            mul_op (tree->child[(tree->child).size()-1]);
            (tree->child).push_back(newNode("F"));
            factor (tree->child[(tree->child).size()-1]);
            (tree->child).push_back(newNode("FT"));
            factor_tail (tree->child[(tree->child).size()-1]);
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
            (tree->child).push_back(newNode("epsilon"));
            break;          /* epsilon production */
        default:
            throw std::runtime_error("exception thrown in factor_tail\n");
            // cout << "factor_tail error\n";
            // error ();
        }
    }catch(std:: exception const& except){

        while(!condSet.count(input_token)){
            cout<<"Syntax error in factor_tail non-terminal: "<<input_token<<"\n\n";
            //cout<<"IN COND while loop AIIII\n"<<token[input_token]<<"\n\n";
            errorList.push_back(input_token);
            //converter(input_token);
            input_token=scan();
        }
    }
    
}

void add_op (Node* tree) {
    try{
        switch (input_token) {
        case t_add:
            cout << "predict add_op --> add\n";
            (tree->child).push_back(newNode("+"));
            match (t_add);
            break;
        case t_sub:
            cout << "predict add_op --> sub\n";
            (tree->child).push_back(newNode("-"));
            match (t_sub);
            break;
        default:
            throw std::runtime_error("exception thrown in add_op\n");
            // cout << "add_op error\n";
            // error ();
        }
    }catch(std:: exception const& except){

        while(!condSet.count(input_token)){
            cout<<"Syntax error in addition op non-terminal: "<<input_token<<"\n\n";
            //cout<<"IN COND while loop AIIII\n"<<token[input_token]<<"\n\n";
            errorList.push_back(input_token);
            //converter(input_token);
            input_token=scan();
        }
    }
    
}

void mul_op (Node* tree) {
    try{
        switch (input_token) {
        case t_mul:
            cout << "predict mul_op --> mul\n";
            (tree->child).push_back(newNode("*"));
            match (t_mul);
            break;
        case t_div:
            cout << "predict mul_op --> div\n";
            (tree->child).push_back(newNode("/"));
            match (t_div);
            break;
        default:
            //cout << "mul_op error\n";
            //error ();
            throw std::runtime_error("exception thrown in mul_op\n");
        }
    }catch(std:: exception const& except){
        //cout << "cond error\n\n"<<input_token<<"\n\n";

        while(!condSet.count(input_token)){
            cout<<"Syntax error in multiplication op non-terminal: "<<input_token<<"\n\n";
            //cout<<"IN COND while loop AIIII\n"<<token[input_token]<<"\n\n";
            errorList.push_back(input_token);
            //converter(input_token);
            input_token=scan();
        }
    }
    
}


   /*
             procedure stmt
        try
            ...             -- code to parse a statement
        except when syntax_error =>
            while input_token not in (FOLLOW(stmt) U {$$})
                get_next_token()
                
        */

void cond (Node* tree) {
    //cout<<"\nIN COND at all\n\n";
    try{
        switch (input_token) {
        case t_id:
        case t_literal:
        case t_lparen:
            cout << "predict cond --> expr rel_op expr\n";
            (tree->child).push_back(newNode("E"));
            expr (tree->child[(tree->child).size()-1]);
            (tree->child).push_back(newNode("ro"));
            rel_op (tree->child[(tree->child).size()-1]);
            (tree->child).push_back(newNode("E"));
            expr (tree->child[(tree->child).size()-1]);
            break;
        default:
            //cout << "cond error\n";           //commented out two lines
            //error ();
            //throw 'e';
            //cout << "cond error\n";           //commented out two lines
            //error ();
            throw std::runtime_error("exception thrown in cond\n");

        }
    }catch(std:: exception const& except){
        cout << "cond error\n\n"<<input_token<<"\n\n";

        while(!condSet.count(input_token)){
            cout<<"Syntax error in condition non-terminal: "<<input_token<<"\n\n";
            //cout<<"IN COND while loop AIIII\n"<<token[input_token]<<"\n\n";
            errorList.push_back(input_token);
            //converter(input_token);
            input_token=scan();
        }

    }      
}

void rel_op (Node* tree) {
    switch (input_token) {
        case t_eq:
            cout << "predict rel_op --> eq\n";
            (tree->child).push_back(newNode("="));
            match (t_eq);
            break;
        case t_neq:
            cout << "predict rel_op --> neq\n";
            (tree->child).push_back(newNode("<>"));
            match (t_neq);
            break;
        case t_lt:
            cout << "predict rel_op --> lt\n";
            (tree->child).push_back(newNode("<"));
            match (t_lt);
            break;
        case t_gt:
            cout << "predict rel_op --> gt\n";
            (tree->child).push_back(newNode(">"));
            match (t_gt);
            break;
        case t_lte:
            cout << "predict rel_op --> lte\n";
            (tree->child).push_back(newNode("<="));
            match (t_lte);
            break;
        case t_gte:
            cout << "predict rel_op --> gte\n";
            (tree->child).push_back(newNode(">="));
            match (t_gte);
            break;
        default:
            cout << "rel_op error\n";
            error ();
    }
}
/*
    Takes the input tokens and prints the corresponding token information with a syntax error message
*/
void converter(token input_token){

    switch(input_token){
        case 0:
            std::cout<<"Syntax_error with read token\n";
            break;
        case 1:
            std::cout<<"Syntax_error with write token\n";
            break;
        case 2:
            std::cout<<"Syntax_error with id token\n";
            break;
        case 3:
            std::cout<<"Syntax_error with literal token\n";
            break;
        case 4:
            std::cout<<"Syntax_error with gets token\n";
            break;
        case 5:
            std::cout<<"Syntax_error with + token\n";
            break;
        case 6:
            std::cout<<"Syntax_error with - token\n";
            break;
        case 7:
            std::cout<<"Syntax_error with * token\n";
            break;
        case 8:
            std::cout<<"Syntax_error with / token\n";
            break;
        case 9:
            std::cout<<"Syntax_error with ( token\n";
            break;
        case 10:
            std::cout<<"Syntax_error with ) token\n";
            break;
        case 11:
            std::cout<<"Syntax_error with eof token\n";
            break;
        case 12:
            std::cout<<"Syntax_error with if token\n";
            break;
        case 13:
            std::cout<<"Syntax_error with end token\n";
            break;
        case 14:
            std::cout<<"Syntax_error with while token\n";
            break;
        case 15:
            std::cout<<"Syntax_error with = token\n";
            break;
        case 16:
            std::cout<<"Syntax_error with <> token\n";
            break;
        case 17:
            std::cout<<"Syntax_error with < token\n";
            break;
        case 18:
            std::cout<<"Syntax_error with > token\n";
            break;
        case 19:
            std::cout<<"Syntax_error with <= token\n";
            break;
        case 20:
            std::cout<<"Syntax_error with >= token\n";
            break;
        default:
            std:cout<<"default";
            break;
    }
    
}

int main () {
    input_token = scan();
    const char* root = "";
    Node* tree = newNode(root);
    program(tree);

    /*
        If there are any errors in the list, it prints them out with a message
        at the end of the program.
    */
    std::cout<<"\n";
    if(!errorList.empty()){

        std::cout<<"Check output above for information about where the error happened\n\n";
        list <token> :: iterator it;
        for(it=errorList.begin(); it!=errorList.end();++it){
            converter(*it);
        }
    }
    return 0;
}









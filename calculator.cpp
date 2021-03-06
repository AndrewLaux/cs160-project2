
#include "calculator.hpp"
#include <string>
#include <cmath>
#include <stdexcept>
#include <cstdio>
#include <iostream>
#include <cctype>
#include <string>

// Scanner implementation

// You may have to modify this constructor, although it might not be neccessary.
Scanner::Scanner() : line(1), 
                     value(0),
                     tok_pos(0) {
    long c = 0;  
                    
    do {
        c = getchar();
        input.push_back(char(c));
    }while(c != EOF);
}

// You need to fill this method with the appropriate code for it to work as described in the project description.
Token Scanner::nextToken() {

    return Scanner::machine(Init, tok_pos);
}

// Recursive state machine for computing next token.
Token Scanner::machine(State state, long pos) {
    char current_char = input.at(pos);
    this->last_pos = pos;

    switch(state) {

        case State::Init:
            if(current_char == ' ') return machine(state, pos + 1);
            else if(current_char == '(') return Token::T_OPENPAREN;
            else if(current_char == ')') return Token::T_CLOSEPAREN;
            else if(current_char == ';') return Token::T_SEMICOLON;
            else if(current_char == '+') return Token::T_PLUS;
            else if(current_char == '*') return Token::T_MULTIPLY;
            else if(current_char == '/') return Token::T_DIVIDE;
            else if(current_char == '-') return Token::T_MINUS;
            else if(current_char == char(EOF)) return Token::T_EOF;
            else if(current_char == 'm') return machine(State::Mod, pos + 1);
            else if(current_char == '\n') return Token::T_NEWLN;
            else if(std::isdigit(static_cast<unsigned char>(current_char))) {
                std:: string s;
                while(std::isdigit(static_cast<unsigned char>(current_char))) {
                    this->last_pos = pos;
                    s.push_back(current_char);
                    long num = std::stol(s);
                    this->value = num;
                    pos = pos + 1;
                    current_char = input.at(pos);
                }
                return Token::T_NUMBER;
            }
            else scanError(this->line, current_char);
            break;
        
        case State::Mod:
            if(current_char == 'o') {
                pos = pos + 1;
                char current_char = input.at(pos);
                this->last_pos = pos;
                if(current_char == 'd') return Token::T_MODULO;
            }
            else scanError(this->line, current_char);
            break;

        default:
            
            break;
    }
    return T_EOF;
}

// You need to fill this method with the appropriate code for it to work as described in the project description.
void Scanner::eatToken(Token toConsume) {
    if(toConsume == nextToken()) {
        this->tok_pos = this->last_pos + 1;
        if(toConsume == Token::T_NEWLN) this->line++;
    }
    
    return;
}

long Scanner::lineNumber() {
    return this->line;
}

long Scanner::getNumberValue() {
    return this->value;
}

// Parser implementation

// You may need to modify this constructor and make it do stuff, although it might not be neccessary.
Parser::Parser(bool eval) : evaluate(eval) {
    this->lookahead = this->scanner.nextToken();
    output = "";
}

void Parser::parse() {
    start();
    match(Token::T_EOF);
    if(evaluate)std::cout << output;
}

void Parser::start() {
    ExpL();
}

void Parser::ExpL(){
    long result = 0;
    Exp(result);
    ExpL_(result, false);
}

void Parser::ExpL_(long &res, bool recall){
    switch (lookahead)
    {
        case Token::T_SEMICOLON:
            match(Token::T_SEMICOLON);
            if(evaluate) {
                if(res > 0x7FFFFFFF) outOfBoundsError(scanner.lineNumber(), res);
                output.append(std::to_string(res));
            }
            recall = true;
            ExpL_(res, recall);
            break;

        case Token::T_EOF:
            if (evaluate && !recall) {
                if(res > 0x7FFFFFFF) outOfBoundsError(scanner.lineNumber(), res);
                output.append(std::to_string(res));
            }
            break;
    
        default:
            output += "\\n";
            ExpL();
            break;
    }
}

void Parser::Exp(long &res){
    Term(res);
    Exp_(res);
}

void Parser::Exp_(long &res){
    long rhs;
    switch (lookahead)
    {
        case Token::T_PLUS:
            match(Token::T_PLUS);
            Term(rhs);
            long long x;
            x = static_cast<long long>(res) + rhs;
            if(x > 0x7FFFFFF) outOfBoundsError(scanner.lineNumber(), x);
            res = res + rhs;
            Exp_(res);
            break;

        case Token::T_MINUS:
            match(Token::T_MINUS);
            Term(rhs);
            res = res - rhs;
            Exp_(res);
            break;
    
        default: //Epsilon
            break;
    }
}

void Parser::Term(long &res){
    Num(res);
    Term_(res);
}

void Parser::Term_(long &res){
    long rhs;
    switch (lookahead)
    {
        case Token::T_MULTIPLY:
            match(Token::T_MULTIPLY);
            Num(rhs);
            long x;
            x = static_cast<long>(res) * rhs;
            if(x > 0x7FFFFFF) outOfBoundsError(scanner.lineNumber(), x);
            res = res * rhs; 
            Term_(res);
            break;
    
        case Token::T_DIVIDE:
            match(Token::T_DIVIDE);
            Num(rhs);
            if(rhs == 0) divideByZeroError(scanner.lineNumber(), res);
            res = res / rhs;
            Term_(res);
            break;

        case Token::T_MODULO:
            match(Token::T_MODULO);
            Num(rhs);
            res = res % rhs;
            Term_(res);
            break;

        default:    //Epsilon
            break;
    }
}

void Parser::Num(long &res) {

    if(lookahead == Token::T_NUMBER){
        res = scanner.getNumberValue();
        match(Token::T_NUMBER);
    } 
    else if(lookahead == Token::T_OPENPAREN) {
        match(Token::T_OPENPAREN);
        Exp(res);
        match(Token::T_CLOSEPAREN);
    }
    else parseError(scanner.lineNumber(), lookahead);
    
}


void Parser::match(Token token){
    if (lookahead == token) {
        scanner.eatToken(token);
        if(token != Token::T_EOF) lookahead = scanner.nextToken();
        while (lookahead == Token::T_NEWLN) {
            scanner.eatToken(T_NEWLN);
            lookahead = scanner.nextToken();
        }
    }
    else mismatchError(scanner.lineNumber(), token, lookahead);
}



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
Scanner::Scanner() : line(0), 
                     value(0),
                     tok_pos(0) {
    int c = 0;  
                    
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
Token Scanner::machine(State state, int pos) {
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
                    int num = std::stoi(s);
                    this->value = num;
                    pos = pos + 1;
                    current_char = input.at(pos);
                }
                return Token::T_NUMBER;
            }
            break;
        
        case State::Mod:
            if(current_char == 'o') {
                pos = pos + 1;
                char current_char = input.at(pos);
                this->last_pos = pos;
                if(current_char == 'd') return Token::T_MODULO;
            }
            break;

        default:
            std::string s; 
            s.push_back(current_char);
            int state_num = static_cast<int>(state);
            std::string msg_str = "Unhandled char '" + s + "' in state: " + std::to_string(state_num);
            const char *msg = msg_str.c_str();
            throw std::runtime_error(msg);
    }
    return T_EOF;
}

// You need to fill this method with the appropriate code for it to work as described in the project description.
void Scanner::eatToken(Token toConsume) {
    this->tok_pos = this->last_pos + 1;
    return;
}

int Scanner::lineNumber() {
    return this->line;
}

int Scanner::getNumberValue() {
    return this->value;
}

// Parser implementation

// You may need to modify this constructor and make it do stuff, although it might not be neccessary.
Parser::Parser(bool eval) : evaluate(eval) {
    // WRITEME
}

void Parser::parse() {
    start();
}

void Parser::start() {
    // I am a placeholder. Implement a recursive descent parser starting from me. Add other methods for different recursive steps.
    // Depending on how you designed your grammar, how many levels of operator precedence you need, etc., you might end up with more
    // or less steps in the process.
    //
    // WRITEME
}

// You will need to add more methods for this to work. Don't forget to also define them in calculator.hpp!
// WRITEME


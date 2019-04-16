#pragma once

#include "helpers.hpp"

#include <climits>
#include <vector>
#include <string>

// SCANNER DEFINITION
enum State {
    Init,
    Mod,
    Number, 
};

// You can freely add member fields and functions to this class.
class Scanner {
    int line;
    int value;
    int tok_pos;
    int last_pos;
    std::vector<char>input;

public:
    // You really need to implement these four methods for the scanner to work.
    Token nextToken();
    Token machine(State state, int pos);
    void eatToken(Token toConsume);
    int lineNumber();
    int getNumberValue();
    
    Scanner();
};

// PARSER DEFINITION

// You can freely add member fields and functions to this class.
class Parser {
    Scanner scanner;
    
    bool evaluate;
    Token lookahead;

    void start();
    void ExpL();
    void ExpL_();
    void Exp();
    void Exp_();
    void Term();
    void Term_();
    void Num();

    void match(Token token);

    // You will need to define the recursive descent functions you're going to use here.
    // WRITEME
public:
    void parse();
    
    Parser(bool);
};

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
    long line;
    long value;
    long tok_pos;
    long last_pos;
    std::vector<char>input;

public:
    // You really need to implement these four methods for the scanner to work.
    Token nextToken();
    Token machine(State state, long pos);
    void eatToken(Token toConsume);
    long lineNumber();
    long getNumberValue();
    
    Scanner();
};

// PARSER DEFINITION

// You can freely add member fields and functions to this class.
class Parser {
    Scanner scanner;
    
    bool evaluate;
    Token lookahead;
    std::string output;

    void start();
    void ExpL();
    void ExpL_(long &res, bool recall);
    void Exp(long &res);
    void Exp_(long &res);
    void Term(long &res);
    void Term_(long &res);
    void Num(long &res);

    void match(Token token);

    // You will need to define the recursive descent functions you're going to use here.
    // WRITEME
public:
    void parse();
    
    Parser(bool);
};

#pragma once
#include <iostream>
#include <vector>
#include "Token.h"

class Parser {
    private:
        std::string inputStream;
    public: 
        std::vector<Token> tokenStream; 
        Token currentToken;

        Parser();
        void readStream(const std::string &instream);

        //parser rules
        void regex();
        void subexpr();
        void binaryop();
        void unaryop();
        void leaf();

        void match(Token tok); //match token at index i
};
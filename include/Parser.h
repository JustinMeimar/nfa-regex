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
        Token lookAhead;

        Parser();
        void readStream(const std::string &instream);

        //parser rules
        void regex();
        void paren();
        void union_(); //avoid name conflict.
        void concat();
        void star();
        void leaf();

        void match(int x); //match token of type x
        void consume();
};
#pragma once
#include <iostream>
#include <vector>
#include <memory>

#include "Token.h"

class Parser {
    private:
        std::string inputStream;
    public: 
        std::vector<std::shared_ptr<Token>> tokenStream; 
        std::shared_ptr<Token> nextToken;
        int streamIndex = 0;

        Parser();
        ~Parser();

        void readStream(const std::string &instream);

        //parser rules
        void regex();
        void union_(); //avoid name conflict.
        void concat();
        void star();
        void paren(); 
        void leaf();

        void match(Type type);  //match token of type x
        void expect(Type type); //expect a token type at current stream index
        void advance();         //increment stream index
};
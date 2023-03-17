#pragma once
#include <iostream>
#include <vector>
#include <memory>

#include "Token.h"
#include "Node.h"

#define DEBUG 0 //debug flag

class Parser {
    private:
        std::string inputStream;
    public: 
        std::vector<std::shared_ptr<Token>> tokenStream; 
        std::shared_ptr<Token> nextToken;
        std::shared_ptr<Node> ctx = nullptr; // default start (no nodes in the parse tree yet)
        std::shared_ptr<Node> root; 
        int streamIndex = 0;

        Parser();
        ~Parser();

        //facilitate parsing & tree building
        void readStream(const std::string &instream);
        void enterRule(std::shared_ptr<Node> rule_node);

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

        // hacky helper method for making input strings have implicit concatenation between adjacent characters. 
        void insertImplicitConcatTokens();
};
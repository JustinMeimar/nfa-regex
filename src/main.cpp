#pragma once
#include <iostream>
#include <memory>
#include <vector>

#include "Node.h"
#include "Parser.h"
#include "NFA.h"
#include "Visitor.h"

int main(int argc, char** argv) {

    auto regexParser = std::make_shared<Parser>();   
    std::string regex;
    std::string string;

    if (argc == 1) {
        printf("no arguments supplied...\n");
        return 0;
    } else if (argc >= 2) {
        regex = argv[1]; //regex specification
    } if (argc == 3) {
        string = argv[2]; //string to run on RE 
    } 

    regexParser->readStream(regex);
    regexParser->insertImplicitConcatTokens();
    regexParser->regex();

    std::shared_ptr<Node> root = regexParser->root; 

    std::shared_ptr<Visitor> visitor = std::make_shared<Visitor>(); 
    visitor->visit(root);

    std::shared_ptr<NFA> nfa = visitor->nfa;
    nfa->execute(nfa->startState, string, 0);

    if (nfa->accept) {
        std::cout << "Accept String :D" << std::endl;
    } else {
        std::cout << "Reject String :(" << std::endl;
    }

    return 0;
}
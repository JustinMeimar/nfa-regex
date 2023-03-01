#pragma once
#include <iostream>
#include <memory>
#include <vector>

#include "Node.h"
#include "Parser.h"
#include "NFA.h"

int main(int argc, char** argv) {

    auto regexParser = std::make_shared<Parser>();   

    if (argc <= 1) {
        printf("no arguments supplied...\n");
        return 0;
    }

    std::string input = argv[1];
    regexParser->readStream(input);
    regexParser->regex();

    std::shared_ptr<Node> root = regexParser->root; 

    //temporary walk in main to validate parse tree is correct:
    std::cout << root->children.size() << std::endl;

    std::shared_ptr<Node> temp = root;
    
    std::shared_ptr<NFA> nfa = std::make_shared<NFA>();
    nfa->constructFromDFS(root); 

    return 0;
}
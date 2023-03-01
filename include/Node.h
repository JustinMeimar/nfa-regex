#pragma once
#include <memory>
#include <vector>
#include <iostream>

#include "Token.h"
#include "Rules.h"

class Node {
    public:
        std::vector<std::shared_ptr<Node>> children; 
        std::shared_ptr<Node> parent; 
        std::shared_ptr<Token> token; //for terminal nodes only, otherwise nullptr
        ParserRule rule; //production this node was generated in

        Node(); //for root node
        Node(ParserRule rule); //for non-terminal rule nodes
        Node(std::shared_ptr<Token> token); //for terminal token nodes

        //methods 
        void addChild(std::shared_ptr<Node> child);
};
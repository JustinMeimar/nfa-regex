#include "Node.h"

Node::Node() { } 

Node::Node(ParserRule rule) { 
    this->rule = rule; //this notation to make clear 
}

Node::Node(std::shared_ptr<Token> token) { 
    this->token = token; 
}

void Node::addChild(std::shared_ptr<Node> child) {
    children.push_back(child);
}

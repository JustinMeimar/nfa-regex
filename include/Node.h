#pragma once
#include <vector>
#include "Token.h"

class Node {
    public:
        std::vector<Node> children; 
        Token token;
        
        Node() {}
};
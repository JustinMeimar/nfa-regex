#pragma once
#include <iostream>
#include <vector>
#include <stack>
#include <memory>
#include <set>

#include "Node.h"

class NFA {
    public:
        NFA() {}

        // Construct the NFA from a depth first traversal of the Parse Tree 
        void constructFromDFS(std::shared_ptr<Node> node);
};

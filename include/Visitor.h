#pragma once
#include <iostream>
#include <vector>
#include <stack>
#include <memory>
#include <set>

#include "Token.h"
#include "Node.h"
#include "NFA.h"

#define DEBUG 0

class Visitor {
    public:
        std::stack<std::shared_ptr<NFA>> expr_stack;
        std::shared_ptr<NFA> nfa; //final NFA we return
        Visitor() {}

        // Parse Tree Walkers
        void visit(std::shared_ptr<Node> node);
        void visitChildren(std::shared_ptr<Node> node); 

        // Walkers for rules
        void visitRoot(std::shared_ptr<Node> node);
        void visitRegex(std::shared_ptr<Node> node);
        void visitUnion(std::shared_ptr<Node> node);
        void visitConcat(std::shared_ptr<Node> node);
        void visitStar(std::shared_ptr<Node> node);
        void visitComp(std::shared_ptr<Node> node);
        void visitParen(std::shared_ptr<Node> node);
        void visitLeaf(std::shared_ptr<Node> node);

        //Helpers
        void stackHelper(ParserRule rule);
};
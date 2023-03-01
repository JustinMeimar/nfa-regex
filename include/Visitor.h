#pragma once
#include <iostream>
#include <vector>
#include <stack>
#include <memory>
#include <set>

#include "Token.h"
#include "Node.h"
#include "NFA.h"

class Visitor {
    public:
        std::stack<std::shared_ptr<NFA>> expr_stack;
        Visitor() {}

        void visit(std::shared_ptr<Node> node);
        void visitChildren(std::shared_ptr<Node> node); 

        //For rules
        void visitRegex(std::shared_ptr<Node> node);
        void visitUnion(std::shared_ptr<Node> node);
        void visitConcat(std::shared_ptr<Node> node);
        void visitStar(std::shared_ptr<Node> node);
        void visitParen(std::shared_ptr<Node> node);
        void visitLeaf(std::shared_ptr<Node> node);

        //Helper
        void stackHelper(ParserRule rule);
};
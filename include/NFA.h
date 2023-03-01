#pragma once
#include <iostream>
#include <vector>
#include <stack>
#include <memory>
#include <set>

#include "Token.h"
#include "Node.h"
#include "Rules.h"

class NFA {
    public:
        NFA(ParserRule rule, std::shared_ptr<Token> token);
        NFA(ParserRule rule, std::shared_ptr<NFA> lhs, std::shared_ptr<NFA> rhs);     
};

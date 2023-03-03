#pragma once
#include <iostream>
#include <vector>
#include <stack>
#include <memory>
#include <set>
#include <tuple>

#include "Token.h"
#include "Node.h"
#include "Rules.h"
#include "State.h"

class NFA {
    public:
        NFA(ParserRule rule, std::shared_ptr<Token> token);
        NFA(ParserRule rule, std::shared_ptr<NFA> lhs, std::shared_ptr<NFA> rhs);     

        // NFA N = (Q, E, s, delta, F);
        std::set<std::shared_ptr<State>> states; // Q
        std::set<std::shared_ptr<State>> acceptStates; // F
        std::shared_ptr<State> startState; // s
        std::map<std::tuple<std::shared_ptr<State>, const char>, std::shared_ptr<State>> transition_table; //delta

        //construct NFA:
        void addTransition(std::shared_ptr<State> q1, std::shared_ptr<State> q2, const char sigma); //add a transitino from (q1, sigma)  > q2

        //execution:
        void execute(const std::string &string);

};

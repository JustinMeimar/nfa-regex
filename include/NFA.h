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
        std::map<std::tuple<State, const char>, State> transition; //delta

        //construct NFA:
        void addTransition(State q1, const char sigma, State q2); //add a transitino from (q1, sigma)  > q2

        //execution:
        void execute(const std::string &inStream);

};

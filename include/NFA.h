#pragma once
#include <iostream>
#include <vector>
#include <stack>
#include <memory>
#include <set>
#include <tuple>
#include <algorithm>
#include <queue>

#include "Token.h"
#include "Node.h"
#include "Rules.h"
#include "State.h"

#ifndef EPSILON_H
#define EPSILON_H
    #define EPSILON '\xCE' //Unicode character for epsilon 
#endif
#define DEBUG_NFA 0

typedef std::tuple<std::shared_ptr<State>, const char, std::shared_ptr<State>> TransitionTuple;
typedef std::set<TransitionTuple> TransitionTable;
typedef std::set<std::shared_ptr<State>> StateSet;
typedef std::tuple<std::shared_ptr<State>, unsigned int> ExecutionConfig;

class NFA {
    public:
        NFA(ParserRule rule, std::shared_ptr<Token> token);
        NFA(ParserRule rule, std::shared_ptr<NFA> lhs, std::shared_ptr<NFA> rhs);     
        NFA(ParserRule rule, std::shared_ptr<NFA> nfa); //for unary operator star

        // NFA N = (Q, E, s, delta, F);
        StateSet states; // Q
        StateSet acceptStates; // F
        std::shared_ptr<State> startState; // s
        TransitionTable transition_table; 
        bool accept; // flipped to true if a path 

        //construct NFA:
        void addTransition(std::shared_ptr<State> q1, std::shared_ptr<State> q2, const char sigma); //add a transitino from (q1, sigma)  > q2
        void constructFromUnion(std::shared_ptr<NFA> lhs, std::shared_ptr<NFA> rhs);
        void constructFromConcat(std::shared_ptr<NFA> lhs, std::shared_ptr<NFA> rhs);
        void constructFromStar(std::shared_ptr<NFA> lhs);

        //execution:
        void execute(std::shared_ptr<State> start_state, const std::string &string, unsigned int input_pointer);

        //helper 
        void computeComplement();
        void printTransitionTable(TransitionTable transition_table);

    private: 
        void copyTransitions(TransitionTable *src, TransitionTable *dest);
        StateSet unionStates(StateSet lhs, StateSet rhs);
        std::set<TransitionTuple> computeAvailableTransitions(std::shared_ptr<State> current_state, char c);
};
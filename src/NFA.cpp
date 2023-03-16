#include "NFA.h"

// Atomic NFA constructor
NFA::NFA(ParserRule rule, std::shared_ptr<Token> token) : accept(false) {
    std::shared_ptr<State> startState = std::make_shared<State>(REJECT);
    std::shared_ptr<State> acceptState = std::make_shared<State>(ACCEPT);
    
    this->startState = startState;
    this->acceptStates.insert(startState);  

    addTransition(startState, acceptState, token->character);
}

// New NFA as a composition of two smaller NFA and a parse rule to join them.
NFA::NFA(ParserRule rule, std::shared_ptr<NFA> lhs, std::shared_ptr<NFA> rhs) : accept(false) {

    switch(rule) {
        case RULE_UNION:
            constructFromUnion(lhs, rhs); 
            break;
        case RULE_CONCAT:
            constructFromConcat(lhs, rhs);
            break; 
        case RULE_STAR:
            constructFromStar(lhs);
    }
}     

void NFA::constructFromUnion(std::shared_ptr<NFA> lhs, std::shared_ptr<NFA> rhs) {
    // Create new start state  
    std::shared_ptr<State> new_start_state = std::make_shared<State>(REJECT);
    this->startState = new_start_state;
    this->transition_table = lhs->transition_table;
    
    #if DEBUG_NFA
        std::cout << "== Construct from union == " << std::endl;
    #endif

    for ( TransitionTable::iterator it = rhs->transition_table.begin(); it != rhs->transition_table.end(); it++) { // for body

        TransitionTuple transition_tuple = *it;
        std::shared_ptr<State> current_state = std::get<0>(transition_tuple);
        std::shared_ptr<State> next_state = std::get<2>(transition_tuple);
        const char sigma = std::get<1>(transition_tuple);
        this->transition_table.insert(*it);
    }

    addTransition(startState, lhs->startState, EPSILON);
    addTransition(startState, rhs->startState, EPSILON);
}

void NFA::constructFromConcat(std::shared_ptr<NFA> lhs, std::shared_ptr<NFA> rhs) {}

void NFA::constructFromStar(std::shared_ptr<NFA> lhs) {}

void NFA::addTransition(std::shared_ptr<State> q1, std::shared_ptr<State> q2, const char sigma) {
    
    TransitionTuple transition_tuple = {q1, sigma, q2};  
    
    #if DEBUG_NFA
    #endif

    transition_table.insert(transition_tuple);
}

TransitionTable NFA::computeAvailableTransitions(std::shared_ptr<State> current_state, char c) {
    
    TransitionTable available_transitions = {}; 
    
    for (TransitionTable::iterator it = transition_table.begin(); it != transition_table.end(); it++) { // for body

        TransitionTuple transition_tuple = *it;
        std::shared_ptr<State> q1 = std::get<0>(transition_tuple);
        std::shared_ptr<State> q2 = std::get<2>(transition_tuple);
        const char sigma = std::get<1>(transition_tuple);

        if (current_state == q1 && (c == sigma || sigma == EPSILON)) {
            available_transitions.insert( *it ); 
        }  
    }

    return available_transitions;
}

/* Compute a breadth first execution path on the NFA. At a given state all available transitions
are collected, then each transition executes on a separate thread. 
*/
void NFA::execute(std::shared_ptr<State> start_state, const std::string &string, unsigned int input_pointer) {

    std::queue<ExecutionConfig> bfsQueue;
    bfsQueue.push({start_state, input_pointer});
    
    while (bfsQueue.size() != 0) {

        // load some data 
        ExecutionConfig current_config = bfsQueue.front();
        bfsQueue.pop();
        std::shared_ptr<State> cur_state = std::get<0>(current_config);
        unsigned int cur_input_ptr = std::get<1>(current_config);
        char c = string.at(cur_input_ptr);

        //check if we are in accept configuration 
        if (cur_state->type == ACCEPT && cur_input_ptr == string.size()) {
            accept = true; 
            break;
        }

        // push available transitions to the queue
        TransitionTable available_transitions = computeAvailableTransitions(cur_state, c); 
        std::cout << " available transitions" << std::endl;
        printTransitionTable(available_transitions); 
        for (auto transition : available_transitions) {
            std::shared_ptr<State> q2 = std::get<2>(transition);
            char sigma = std::get<1>(transition);

            unsigned int new_input_ptr = cur_input_ptr;
            if (sigma != EPSILON ) {
                new_input_ptr++;
            }

            bfsQueue.push({q2, new_input_ptr});
        }
    }
    return; 
}

void NFA::computeComplement() {

    std::shared_ptr<State> new_start_state = std::make_shared<State>(REJECT);
    this->startState = new_start_state;

    // flip reject states to accept
    for (auto state : this->states) {
        if (state->type == REJECT) {
            state->type = ACCEPT;
        }
    }

    // flip accept states to reject
    for (auto state : this->acceptStates) {
        state->type = REJECT;
        addTransition(startState, state, EPSILON);
    }

    // invert transition rule
    for (auto transition_tuple : this->transition_table) {
        std::cout << "q1: " << std::get<0>(transition_tuple); 
        std::cout << " sigma: " << std::get<1>(transition_tuple);
        std::cout << " --> ";
        std::cout << " q2: " << std::get<2>(transition_tuple) << std::endl;

        std::shared_ptr<State> q1 = std::get<0>(transition_tuple);
        std::shared_ptr<State> q2 = std::get<2>(transition_tuple);
        
        std::get<2>(transition_tuple) = q1;
        std::get<0>(transition_tuple) = q2; 
    }
}

void NFA::printTransitionTable(TransitionTable transition_table) {

    for ( TransitionTable::iterator it = transition_table.begin(); it != transition_table.end(); it++) {
        // Get key and value
        TransitionTuple transition_tuple = *it;
        std::cout << "==Transition Function: " << "(" << std::get<0>(*it) << ", " << std::get<1>(*it) << ") => " << std::get<2>(*it) << std::endl;
    }
    return;
}
#include "NFA.h"

// Atomic NFA constructor
NFA::NFA(ParserRule rule, std::shared_ptr<Token> token) : accept(false) {

    #if DEBUG_NFA
        std::cout << "Construct from leaf: " << token->character << std::endl;
    #endif

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

void NFA::copyTransitions(TransitionTable *src, TransitionTable *dest) {
    for ( TransitionTable::iterator it = src->begin(); it != src->end(); it++) { 
        dest->insert(*it);
    }
}

StateSet NFA::unionStates(StateSet lhs, StateSet rhs) {
    
    StateSet join_states(lhs);

    for (StateSet::iterator it = rhs.begin(); it != rhs.end(); it++) {
        join_states.insert(*it);
    }

    return join_states;
}

void NFA::constructFromUnion(std::shared_ptr<NFA> lhs, std::shared_ptr<NFA> rhs) {
    // Create new start state  
    std::shared_ptr<State> new_start_state = std::make_shared<State>(REJECT);
    this->startState = new_start_state;
    this->transition_table = lhs->transition_table;
    this->acceptStates = unionStates(lhs->acceptStates, rhs->acceptStates);

    #if DEBUG_NFA
        std::cout << "== Construct from union == " << std::endl;
    #endif

    copyTransitions(&rhs->transition_table, &transition_table); 

    addTransition(startState, lhs->startState, EPSILON);
    addTransition(startState, rhs->startState, EPSILON);
}

void NFA::constructFromConcat(std::shared_ptr<NFA> lhs, std::shared_ptr<NFA> rhs) {

    this->transition_table = lhs->transition_table;
    this->startState = lhs->startState;
    this->acceptStates = rhs->acceptStates;

    copyTransitions(&rhs->transition_table, &transition_table);

    std::shared_ptr<State> rhs_start = rhs->startState;
    for (auto state : lhs->acceptStates) {
        std::cout << "ADD LINK " << state << " -> " << rhs_start;
        state->type = REJECT;
        addTransition(state, rhs_start, EPSILON);        
    }
}

void NFA::constructFromStar(std::shared_ptr<NFA> lhs) {}

void NFA::addTransition(std::shared_ptr<State> q1, std::shared_ptr<State> q2, const char sigma) {
    
    TransitionTuple transition_tuple = {q1, sigma, q2};  
    
    #if DEBUG_NFA
    #endif

    transition_table.insert(transition_tuple);
}

std::set<TransitionTuple> NFA::computeAvailableTransitions(std::shared_ptr<State> current_state, char c) {
    
    std::set<TransitionTuple> available_transitions; 
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

void NFA::execute(std::shared_ptr<State> current_state, const std::string &string, unsigned int input_pointer) {

    // Check if we are in an accept state and have consumed all tokens.
    if (input_pointer == string.size() && current_state->type == ACCEPT) {
        // we are at the end of the input string
        accept = true;
        return; 
    }
    
    char c = string.at(input_pointer);
    std::set<TransitionTuple> available_transitions = computeAvailableTransitions(current_state, c);  

    #if DEBUG_NFA
        std::cout << "==== EXECUTE ====" << std::endl; std::cout << "Current State: " << current_state << std::endl;
        std::cout << "String: " << string << std::endl; std::cout << "input pointer: " << input_pointer << std::endl; 
        std::cout << "number of available transitions: " << available_transitions.size() << std::endl;
    #endif

    //crash the  
    if (available_transitions.size() == 0) {
        return;
    }

    // execute in parallel for each available transition 
    for (auto transition : available_transitions) {
        int next_input_pointer = input_pointer;
        std::shared_ptr<State> q2 = std::get<2>(transition);
        
        // only increment the input pointer for non-epsilon symbol consumptions.
        if (std::get<1>(transition) != EPSILON) {
            next_input_pointer++;
        }

        // recursively execute all possible NFA derivations.
        execute(q2, string, next_input_pointer); 
    }    
    return; 
}

void NFA::printTransitionTable() {
    for ( TransitionTable::iterator it = transition_table.begin(); it != transition_table.end(); it++) {
        // Get key and value
        TransitionTuple transition_tuple = *it;
        std::cout << "==Transition Function: " << "(" << std::get<0>(*it) << ", " << std::get<1>(*it) << ") => " << std::get<2>(*it) << std::endl;
    }
    return;
}
#include "NFA.h"

// Atomic NFA constructor
NFA::NFA(ParserRule rule, std::shared_ptr<Token> token) : accept(false) {

    #if DEBUG_NFA
        std::cout << "Construct from leaf: " << token->character << std::endl;
    #endif

    std::shared_ptr<State> startState = std::make_shared<State>(REJECT);
    std::shared_ptr<State> acceptState = std::make_shared<State>(ACCEPT);
    
    this->startState = startState;
    this->states = StateSet({startState, acceptState}); 
    this->acceptStates.insert(acceptState);  

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
    }
}     

// New NFA composed from kleen star operator
NFA::NFA(ParserRule rule, std::shared_ptr<NFA> nfa) : accept(false) {
    constructFromStar(nfa);
}    

void NFA::addTransition(std::shared_ptr<State> q1, std::shared_ptr<State> q2, const char sigma) {
    
    TransitionTuple transition_tuple = {q1, sigma, q2};  
    
    #if DEBUG_NFA
    #endif

    transition_table.insert(transition_tuple);
}

void NFA::copyTransitions(TransitionTable *src, TransitionTable *dest) {
    for (TransitionTable::iterator it = src->begin(); it != src->end(); it++) { 
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
    #if DEBUG_NFA
        std::cout << "== Construct from union == " << std::endl;
    #endif
    
    // Create new start state  
    std::shared_ptr<State> new_start_state = std::make_shared<State>(REJECT);
    this->startState = new_start_state;
    this->states = unionStates(lhs->states, rhs->states);
    this->states.insert(new_start_state);
    this->acceptStates = unionStates(lhs->acceptStates, rhs->acceptStates);

    copyTransitions(&lhs->transition_table, &transition_table); 
    copyTransitions(&rhs->transition_table, &transition_table); 

    addTransition(startState, lhs->startState, EPSILON);
    addTransition(startState, rhs->startState, EPSILON);
}

void NFA::constructFromConcat(std::shared_ptr<NFA> lhs, std::shared_ptr<NFA> rhs) {
     
    #if DEBUG_NFA
        std::cout << "== Construct from Concat == " << std::endl;
    #endif   
    
    // Create and assign new states
    std::shared_ptr<State> newStartState = std::make_shared<State>(REJECT);
    std::shared_ptr<State> newAcceptState = std::make_shared<State>(ACCEPT);    
    this->startState = newStartState;
    this->states = unionStates(lhs->states, rhs->states);
    this->states.insert(newStartState);
    this->states.insert(newAcceptState);
    this->acceptStates.insert({newAcceptState});   

    // Add transition from the new Start state to the LHS Start state
    addTransition(newStartState, lhs->startState, EPSILON);

    // Copy all the intermediate transitions for LHS and RHS NFA 
    copyTransitions(&lhs->transition_table, &this->transition_table);
    copyTransitions(&rhs->transition_table, &this->transition_table);

    // Set each LHS accept state to reject, but add an epsilon transition to the RHS start state
    std::shared_ptr<State> rhsStartState = rhs->startState;   
    for (auto acceptState : lhs->acceptStates) {
        acceptState->type = REJECT;
        addTransition(acceptState, rhsStartState, EPSILON);
    }
    
    // Set each RHS accept state to reject, but add an epsilon transition to the final AcceptState
    for (auto acceptState : rhs->acceptStates) {
        acceptState->type = REJECT;
        addTransition(acceptState, newAcceptState, EPSILON);
    }
}

void NFA::constructFromStar(std::shared_ptr<NFA> nfa) {
    #if DEBUG_NFA
        std::cout << "== Construct from Star" << std::endl;
    #endif 

    std::shared_ptr<State> newStartState = std::make_shared<State>(REJECT);
    std::shared_ptr<State> newAcceptState = std::make_shared<State>(ACCEPT);
    this->startState = newStartState;
    this->states = nfa->states;
    this->states.insert(newStartState);
    this->states.insert(newAcceptState);
    this->acceptStates = {newAcceptState};

    copyTransitions(&nfa->transition_table, &this->transition_table);

    addTransition(newStartState, nfa->startState, EPSILON);
    addTransition(newStartState, newAcceptState, EPSILON);

    for (auto acceptState : nfa->acceptStates) {
        acceptState->type = REJECT; 
        addTransition(acceptState, newAcceptState, EPSILON);
        addTransition(acceptState, nfa->startState, EPSILON);
    }

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
are collected, then each transition executes and reports back what it found on the frontier. If 
an accept state exists some finite distance from the root, it will be found and the program will halt.  
*/
void NFA::execute(std::shared_ptr<State> start_state, const std::string &string) {

    #if DEBUG_NFA
        printTransitionTable(this->transition_table); 
    #endif

    std::queue<ExecutionConfig> bfsQueue;
    bfsQueue.push({start_state, 0});
    
    while (bfsQueue.size() != 0) {

        // load some data 
        ExecutionConfig current_config = bfsQueue.front();
        bfsQueue.pop();
        std::shared_ptr<State> cur_state = std::get<0>(current_config);
        unsigned int cur_input_ptr = std::get<1>(current_config);
        
        //check if we are in accept configuration 
        char c;
        if (cur_input_ptr == string.size()) {
            accept = false; 
            if (cur_state->type == ACCEPT) {
                accept = true;
                break;    
            }
            c = EPSILON;
        } else {
            c = string.at(cur_input_ptr);
        }

        // push available transitions to the queue
        TransitionTable available_transitions = computeAvailableTransitions(cur_state, c); 

        #if DEBUG_NFA
            std::cout << "current state: " << cur_state << std::endl;
            std::cout << "current queue size: " << bfsQueue.size() + 1 << std::endl;
            std::cout << "current input ptr: " <<  cur_input_ptr << std::endl;
            std::cout << " === start: available transitions: char: "<< c << std::endl;
            printTransitionTable(available_transitions); 
            std::cout << " === end" << std::endl;
        #endif  

        for (auto transition : available_transitions) {
            std::shared_ptr<State> q2 = std::get<2>(transition);
            char sigma = std::get<1>(transition);

            if (sigma != EPSILON ) { 
                bfsQueue.push({q2, cur_input_ptr + 1});
                continue; 
            }       
            bfsQueue.push({q2, cur_input_ptr});
        }
    }
    return; 
}

/*
setStates(data.states);
setEdges(data.edges);
setStartState(data.startState);
setAcceptStates(data.acceptStates);
*/ 
json NFA::serializeToJSON(const std::string& string) { 
    json nfa; 

    // STATES 
    std::vector<uint32_t> state_ids; 
    for (auto state : states) {
        state_ids.push_back(state->getId());
    }
    nfa["states"] = state_ids;

    // ACCEPT STATES 
    std::vector<uint32_t> accept_state_ids; 
    for (auto state : acceptStates) {
         accept_state_ids.push_back(state->getId());
    }
    nfa["acceptStates"] = accept_state_ids;

    // START STATE
    nfa["startState"] = this->startState->getId();

    // EDGES
    std::vector<json> edges; 
    for ( TransitionTable::iterator it = transition_table.begin(); it != transition_table.end(); it++) {
        // Get key and value
        json edge;
        TransitionTuple transition_tuple = *it;
        edge["qi"] = (uint32_t) std::get<0>(*it)->getId();
        edge["sigma"] = (char) std::get<1>(*it);
        edge["qj"] = (uint32_t) std::get<2>(*it)->getId();

        edges.push_back(edge);
    }
    nfa["edges"] = edges;
    //add string and accept fields
    execute(startState, string);
    nfa["string"] = string.c_str();
    if (accept) {
       nfa["accept"] = "true";
    } else {
       nfa["accept"] = "false";
    }

    return nfa;
}

void NFA::printTransitionTable(TransitionTable transition_table) {

    for ( TransitionTable::iterator it = transition_table.begin(); it != transition_table.end(); it++) {
        // Get key and value
        TransitionTuple transition_tuple = *it;
        std::cout << "\t==Transition Function: " << "(" << std::get<0>(*it) << ", " << std::get<1>(*it) << ") => " << std::get<2>(*it) << std::endl;
    }
    return;
}

void NFA::printStates() {
    for (auto state : states) {
        std::cout << "q" << state->getId() << std::endl;
    }
}
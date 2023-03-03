#include "NFA.h"

// Atomic NFA constructor
NFA::NFA(ParserRule rule, std::shared_ptr<Token> token) {
    std::shared_ptr<State> startState = std::make_shared<State>(REJECT);
    std::shared_ptr<State> acceptState = std::make_shared<State>(ACCEPT);
    
    this->startState = startState;
    this->acceptStates.insert(startState);  

    addTransition(startState, acceptState, token->character);
}

// New NFA as a composition of two smaller NFA and a parse rule to join them.
NFA::NFA(ParserRule rule, std::shared_ptr<NFA> lhs, std::shared_ptr<NFA> rhs) {

    std::cout << "MAKE NFA" << rule << std::endl;
    switch(rule) {
        case RULE_CONCAT:
            constructFromUnion(lhs, rhs); 
            break;
    }

}     

void NFA::constructFromUnion(std::shared_ptr<NFA> lhs, std::shared_ptr<NFA> rhs) {
    // Create new start state 
    std::shared_ptr<State> start_state = std::make_shared<State>(REJECT);

    this->transition_table = lhs->transition_table;
    for (
        std::map<std::tuple<std::shared_ptr<State>, const char>, std::shared_ptr<State>>::iterator it = rhs->transition_table.begin();
        it != rhs->transition_table.end();
        it++
    )  {

        std::tuple<std::shared_ptr<State>, const char> key = it->first;
        std::shared_ptr<State> q1 = std::get<0>(key);
        std::cout << q1 << std::endl;
    }
}

void NFA::constructFromConcat(std::shared_ptr<NFA> lhs, std::shared_ptr<NFA> rhs) {

}

void NFA::constructFromStar(std::shared_ptr<NFA> lhs) {

}

void NFA::addTransition(std::shared_ptr<State> q1, std::shared_ptr<State> q2, const char sigma) {

    // (state, symbol) -> state
    std::tuple<std::shared_ptr<State>, const char> transition_tuple = {q1, sigma};  
    this->transition_table.insert({transition_tuple, q2});
}

void NFA::execute(const std::string &string) {
    
    unsigned int input_pointer = 0;
    std::shared_ptr<State> current_state = startState;

    std::cout << transition_table.size();
    const char c = string.at(0);
    
    for (const char c: string) { 
        std::shared_ptr<State> next_state = transition_table[{current_state, c}];
        if (next_state != nullptr) {
            // There is a valid transition
            current_state = next_state;
            if (c != EPSILON) {
                input_pointer++;
            }
        } else {
            break;
        }
    }

    if (current_state->type == ACCEPT && input_pointer == string.size()) {
        std::cout << "String Accepts!" << std::endl;
    } else {
        std::cout << "String Size: " << string.size() << "Current Input:" << input_pointer << std::endl; 
        std::cout << "String Rejects!" << std::endl;
    }

    return;
    
}

#include <iostream>
#include <memory>
#include <vector>
#ifdef COMPILE_WEB 
#include <emscripten/bind.h> 
#endif
#include "Node.h"
#include "Parser.h"
#include "NFA.h"
#include "Visitor.h"

NFA *createNFA(const std::string &regex);

int main(int argc, char** argv) {

    auto regexParser = std::make_shared<Parser>();   
    std::string regex;
    std::string string;

    if (argc == 1) {
        printf("no arguments supplied...\n");
        return 0;
    } else if (argc >= 2) {
        regex = argv[1]; //regex specification
    } if (argc == 3) {
        string = argv[2]; //string to run on RE 
    }

    NFA *nfa = createNFA(regex);
    nfa->execute(nfa->startState, string, 0);
    
    if (nfa->accept) {
        std::cout << "Accept String :D" << std::endl;
    } else {
        std::cout << "Reject String :(" << std::endl;
    }   
    
    return 0;
}


// NFA FUNCTIONALITY
NFA *createNFA(const std::string &regex) {

    auto regexParser = std::make_shared<Parser>();   
    regexParser->readStream(regex);
    regexParser->insertImplicitConcatTokens();
    regexParser->regex();

    std::shared_ptr<Node> root = regexParser->root; 
    std::shared_ptr<Visitor> visitor = std::make_shared<Visitor>(); 
    visitor->visit(root);

    return visitor->nfa.get();
}

void executeNFA(NFA *nfa, const std::string &string) {
    nfa->execute(nfa->startState, string, 0);
}

// WRAPPERS
std::vector<uint32_t> getStatesWrapper(NFA *nfa) {
    std::vector<uint32_t> states;
    for (auto state : nfa->states) {
        states.push_back( (uint32_t) state.get());
    }
    return states;
}

std::vector<uint32_t> getAcceptStatesWrapper(NFA *nfa) {
    std::vector<uint32_t> acceptStates;
    for (auto state : nfa->acceptStates) {
        acceptStates.push_back( (uint32_t) state.get());
    }
    return acceptStates;
}

bool getDidAcceptWrapper(NFA *nfa) {
    return nfa->accept;
}

uint32_t getStartStateWrapper(NFA *nfa) {
    return (uint32_t) nfa->startState.get();
}

// std::vector<std::tuple<int, int, char>> getTransitionTableWrapper(NFA *nfa) { 

//     std::vector<std::tuple<uint32_t, uint32_t, char>> transition_table;

//     for (auto it = nfa->transition_table.begin(); it != nfa->transition_table.end(); it++) {
//         std::tuple<uint32_t, uint32_t, char> transition_tup;
//         std::set transition_tup
        
//     }
// }

EMSCRIPTEN_BINDINGS(my_module) {  
    emscripten::class_<NFA>("NFA");

    emscripten::function("createNFA", &createNFA, emscripten::allow_raw_pointers()); 
    emscripten::function("executeNFA", &executeNFA, emscripten::allow_raw_pointers()); 
    emscripten::function("getDidAccept", &getDidAcceptWrapper, emscripten::allow_raw_pointers()); 
      
    // emscripten::function("createNFALeafFactory", &createNFALeafFactory); 
    // emscripten::function("createNFABinaryOpFactory", &createNFABinaryOpFactory); 
    // emscripten::function("createNFAUnaryOpFactory", &createNFAUnaryOpFactory); 
 
    // emscripten::class_<Point>("Point")
    // .constructor<>()
    // .property("x", &Point::x)
    // .property("y", &Point::
    // emscripten::enum_<StateType>("StateType")
    //     .value("ACCEPT", StateType::ACCEPT)
    //     .value("REJECT", StateType::REJECT);

    // emscripten::class_<State>("State")
    //     .smart_ptr_constructor("State", &std::make_shared<State, StateType>)
    //     .property("type", &State::type);
}

#ifdef COMPILE_WEB
#endif
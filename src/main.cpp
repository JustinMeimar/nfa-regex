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

std::shared_ptr<NFA> createNFA(const std::string &regex);

int main(int argc, char** argv) {

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

    std::shared_ptr<NFA> nfa = createNFA(regex);
    nfa->execute(nfa->startState, string);
    nfa->printStates();
    if (nfa->accept) {
        std::cout << "Accept String :D" << std::endl;
    } else {
        std::cout << "Reject String :(" << std::endl;
    }   
    
    return 0;
}

std::shared_ptr<NFA> createNFA(const std::string &regex) {

    auto regexParser = std::make_shared<Parser>();   
    regexParser->readStream(regex);
    regexParser->insertImplicitConcatTokens();
    regexParser->regex();

    std::shared_ptr<Node> root = regexParser->root; 
    std::shared_ptr<Visitor> visitor = std::make_shared<Visitor>(); 
    visitor->visit(root);

    return visitor->nfa;
}

void executeNFA(std::shared_ptr<NFA> nfa, const std::string &string) {
    nfa->execute(nfa->startState, string);
}

std::vector<uint32_t> getStatesWrapper(std::shared_ptr<NFA> nfa) {
    std::vector<uint32_t> states;
    for (auto state : nfa->states) {
        states.push_back(state->getId());
    }
    return states;
}

std::vector<uint32_t> getAcceptStatesWrapper(std::shared_ptr<NFA> nfa) {
    std::vector<uint32_t> acceptStates;
    for (auto state : nfa->acceptStates) {
        acceptStates.push_back(state->getId());
    }
    return acceptStates;
}

bool getDidAcceptWrapper(std::shared_ptr<NFA> nfa) {
    return nfa->accept;
}

uint32_t getStartStateWrapper(std::shared_ptr<NFA> nfa) {
    return nfa->startState->getId();
}

class TupleWrapper {
    public:
        TupleWrapper(uint32_t qi, char sigma, uint32_t qj) : qi(qi), sigma(sigma), qj(qj) {}

        uint32_t get_qi() const { return qi; }
        void set_qi(uint32_t value) { qi = value; }

        char get_sigma() const { return sigma; }
        void set_sigma(char value) { sigma = value; }

        uint32_t get_qj() const { return qj; }
        void set_qj(uint32_t value) { qj = value; }

    private:
        uint32_t qi;
        char sigma;
        uint32_t qj;
};

std::vector<TupleWrapper> getTransitionTableWrapper(std::shared_ptr<NFA> nfa) { 

    std::vector<TupleWrapper> transition_table;

    for ( TransitionTable::iterator it = nfa->transition_table.begin(); it != nfa->transition_table.end(); it++) {
        TransitionTuple transition_tuple = *it; 

        uint32_t qi = std::get<0>(*it)->getId();
        char sigma = std::get<1>(*it);
        uint32_t qj = std::get<2>(*it)->getId();
        
        TupleWrapper tup_wrapper(qi, sigma, qj);
        transition_table.push_back(tup_wrapper);
    }
    return transition_table;
}

#ifdef COMPILE_WEB
EMSCRIPTEN_BINDINGS(my_module) {  
    emscripten::class_<NFA>("NFA")
        .smart_ptr<std::shared_ptr<NFA>>("shared_ptr<NFA>");

    emscripten::class_<TupleWrapper>("TupleWrapper")
        .constructor<uint32_t, char, uint32_t>()
        .property("qi", &TupleWrapper::get_qi, &TupleWrapper::set_qi)
        .property("sigma", &TupleWrapper::get_sigma, &TupleWrapper::set_sigma)
        .property("qj", &TupleWrapper::get_qj, &TupleWrapper::set_qj);
    
    emscripten::function("createNFA", &createNFA);
    emscripten::function("executeNFA", &executeNFA);
    emscripten::function("getDidAccept", &getDidAcceptWrapper);
    emscripten::function("getStates", &getStatesWrapper);
    emscripten::function("getAcceptStates", &getAcceptStatesWrapper);
    emscripten::function("getTransitionTable", &getTransitionTableWrapper);

    emscripten::register_vector<uint32_t>("vector_uin32_t");
    emscripten::register_vector<TupleWrapper>("vector_tuple_uint_char_unint");
}
#endif
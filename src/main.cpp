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

struct Point {
    int x;
    int y;
};

std::shared_ptr<NFA> run_regex(std::string &regex, std::string &string);

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
    run_regex(regex, string);

    return 0;
}

std::shared_ptr<NFA> run_regex(std::string &regex, std::string &string) {

    auto regexParser = std::make_shared<Parser>();   

    regexParser->readStream(regex);
    regexParser->insertImplicitConcatTokens();
    regexParser->regex();

    std::shared_ptr<Node> root = regexParser->root; 
    std::shared_ptr<Visitor> visitor = std::make_shared<Visitor>(); 
    visitor->visit(root);

    std::shared_ptr<NFA> nfa = visitor->nfa;
    
    nfa->execute(nfa->startState, string, 0);
     if (nfa->accept) {
        std::cout << "Accept String :D" << std::endl;
    } else {
        std::cout << "Reject String :(" << std::endl;
    }   
    return nfa;
}

#ifdef COMPILE_WEB
EMSCRIPTEN_BINDINGS(my_module) {
    emscripten::class_<Point>("Point")
    .constructor<>()
    .property("x", &Point::x)
    .property("y", &Point::y);

    emscripten::enum_<StateType>("StateType")
        .value("ACCEPT", StateType::ACCEPT)
        .value("REJECT", StateType::REJECT);

    emscripten::class_<State>("State")
        .smart_ptr_constructor("State", &std::make_shared<State, StateType>)
        .property("type", &State::type);

    /*
    //Define some custom types
    emscripten::type<StateSet>();
    emscripten::type<TransitionTuple>();
    emscripten::type<TransitionTable>();

    //Define the NFA class
    emscripten::class_<NFA>("Nfa") 
        .constructor<>() 
        .function() // interface to get states
        .function() // interface to get transitions
        .function() // interface to execute the NFA and mutate the 
        .function() // interface to get the accept state of the machine

    */
    // emscripten::class_<Parser>("Parser")
    // .constructor<>
    // .function()  // sta
    // .property() // static member 

}
#endif
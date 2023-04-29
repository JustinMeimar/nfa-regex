#include <iostream>
#include <memory>
#include <vector>
#include "Node.h"
#include "Parser.h"
#include "NFA.h"
#include "Visitor.h"
#include "Bindings.h"
#ifdef COMPILE_WASM 
    #include <emscripten/bind.h> 
#endif



void process_args(std::vector<std::string> args, bool& emitJsonToPipe, bool& emitJsonToStdout) {
    for (size_t i = 0; i< args.size(); i++) {
        if (args[i] == "--emit-json-pipe" || args[i] == "-e") {
            emitJsonToPipe = true; 
        }
        if (args[i] == "--emit-json-stdout") {
            emitJsonToStdout = true;
        }
    }
}

int main(int argc, char** argv) {

    std::string regex;
    std::string string;
    bool emitJsonToPipe = false;
    bool emitJsonToStdout = false;

    std::vector<std::string> args(argv +1, argv+argc);

    regex = argv[1]; //regex specification
    string = argv[2]; //string to run on RE  
    process_args(args, emitJsonToPipe, emitJsonToStdout);    
    
    auto regexParser = std::make_shared<Parser>();   
    regexParser->readStream(regex);
    regexParser->insertImplicitConcatTokens();
    regexParser->regex();

    std::shared_ptr<Node> root = regexParser->root; 
    std::shared_ptr<Visitor> visitor = std::make_shared<Visitor>(); 
    visitor->visit(root);
    std::shared_ptr<NFA> nfa = visitor->nfa;

    std::string str(string);
    if (emitJsonToPipe) {
        writeNFAToNamedPipe(nfa, str);
    }
    if (emitJsonToStdout) {
        writeNFAToStdout(nfa, str);
    }

    nfa->execute(nfa->startState, string); 
    if (nfa->accept) {
        return 1;
    } else { 
        return 0;
    }     
}

/*WASM bindings compiled with -COMPILE_WASM=1*/
#ifdef COMPILE_WASM
EMSCRIPTEN_BINDINGS(my_module) {  
    emscripten::class_<NFA>("NFA")
        .smart_ptr<std::shared_ptr<NFA>>("shared_ptr<NFA>");

    emscripten::function("createNFA", &createNFA);
    emscripten::function("executeNFA", &executeNFA);
    emscripten::function("getDidAccept", &getDidAcceptWrapper);
}
#endif
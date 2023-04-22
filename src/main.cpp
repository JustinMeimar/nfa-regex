#include <iostream>
#include <memory>
#include <vector>
#include "Node.h"
#include "Parser.h"
#include "NFA.h"
#include "Visitor.h"
#include "Bindings.h"
#ifdef COMPILE_WEB 
    #include <emscripten/bind.h> 
#endif

int main(int argc, char** argv) {

    std::string regex;
    std::string string;
    bool emitJson = false;
    std::vector<std::string> args(argv +1, argv+argc);

    for (size_t i = 0; i< args.size(); i++) {
        if (args[i] == "--emit-json" || args[i] == "-e") {
            emitJson = true; 
        }
    }

    if (argc < 3) {
        std::cout << "insufficient args" << std::endl;
        exit(0);
    } else {
        regex = argv[1]; //regex specification
        string = argv[2]; //string to run on RE 
    } 
 
    auto regexParser = std::make_shared<Parser>();   
    regexParser->readStream(regex);
    regexParser->insertImplicitConcatTokens();
    regexParser->regex();

    std::shared_ptr<Node> root = regexParser->root; 
    std::shared_ptr<Visitor> visitor = std::make_shared<Visitor>(); 
    visitor->visit(root);
    std::shared_ptr<NFA> nfa = visitor->nfa;

    if (emitJson) {
        writeNFAToNamedPipe(nfa);
    }

    nfa->execute(nfa->startState, string); 
    if (nfa->accept) {
        return 1;
    } else { 
        return 0;
    }     
}

/*WASM bindings compiled with -DCOMPILE_WEB=1*/
#ifdef COMPILE_WEB
EMSCRIPTEN_BINDINGS(my_module) {  
    emscripten::class_<NFA>("NFA")
        .smart_ptr<std::shared_ptr<NFA>>("shared_ptr<NFA>");

    emscripten::function("createNFA", &createNFA);
    emscripten::function("executeNFA", &executeNFA);
    emscripten::function("getDidAccept", &getDidAcceptWrapper);
}
#endif
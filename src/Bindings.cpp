#include "Bindings.h"

/* IPC bindings */
void writeNFAToNamedPipe(std::shared_ptr<NFA> nfa, const std::string& string, const char* pipe) {
    
    std::string serializedJson = nfa->serializeToJSON(string).dump();
    const char *serializedJsonStr = serializedJson.c_str(); 
    // const char *pipeName = "/tmp/nfa_pipe";
    // const char *pipe = pipe; 

    if (access(pipe, F_OK) == 0) {
        if (unlink(pipe) == -1) {
            perror("unlink");
        }
    }

    if (mkfifo(pipe, 0666) == -1) {
        perror("mkfifo");
    }

    int pipeFD = open(pipe, O_WRONLY);
    write(pipeFD, serializedJsonStr, strlen(serializedJsonStr)); 
    close(pipeFD);
}

void writeNFAToStdout(std::shared_ptr<NFA> nfa, const std::string& string) {
    std::string serialzedJson = nfa->serializeToJSON(string).dump();
    std::cout << serialzedJson;
}

/* WASM Bindings*/
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

bool getDidAcceptWrapper(std::shared_ptr<NFA> nfa) {
    return nfa->accept;
}
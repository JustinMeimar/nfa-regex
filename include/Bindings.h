#pragma once
#include "NFA.h"
#include "Parser.h"
#include "Visitor.h"
#include <fstream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

// IPC
void handleCommandArgs();
void writeNFAToNamedPipe(std::shared_ptr<NFA> nfa, const std::string& string, const char* pipe = "/tmp/nfa_pipe");
void writeNFAToStdout(std::shared_ptr<NFA> nfa, const std::string& string);

// WASM Bindings
std::shared_ptr<NFA> createNFA(const std::string &regex);
void executeNFA(std::shared_ptr<NFA> nfa, const std::string &string);
bool getDidAcceptWrapper(std::shared_ptr<NFA> nfa);
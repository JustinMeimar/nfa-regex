#pragma once
#include "NFA.h"
#include "Parser.h"
#include "Visitor.h"
#include <fstream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

void writeNFAToNamedPipe(std::shared_ptr<NFA> nfa);
std::shared_ptr<NFA> createNFA(const std::string &regex);
void executeNFA(std::shared_ptr<NFA> nfa, const std::string &string);
bool getDidAcceptWrapper(std::shared_ptr<NFA> nfa);
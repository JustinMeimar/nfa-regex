#include "NFA.h"

// Atomic NFA constructor
NFA::NFA(ParserRule rule, std::shared_ptr<Token> token) {

}

// New NFA as a composition of two smaller NFA and a parse rule to join them.
NFA::NFA(ParserRule rule, std::shared_ptr<NFA> lhs, std::shared_ptr<NFA> rhs) {

}     


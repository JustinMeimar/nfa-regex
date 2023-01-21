#include "Parser.h"

Parser::Parser() {}

void Parser::readStream(const std::string &instream) {
    std::cout << instream << std::endl;
}

void Parser::regex() {
    while (currentToken.character != (char) NULL) {
        subexpr();
    }
}

void Parser::subexpr() {
    if (currentToken.isLeaf) {
        match(currentToken);
    }
    subexpr();
 
    //match binary / unary op
    match(currenttoken);

    subexpr();
}

void Parser::binaryop() {
    match(currentToken);
}

void Parser::unaryop() {
    match(currentToken);
}

void Parser::leaf() {
    match(currentToken);
}

void Parser::match(Token tok) {
    // create node from input buffer at index i
}

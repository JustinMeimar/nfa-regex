#include "Parser.h"
#define DEBUG 1

Parser::Parser() {}

Parser::~Parser() {}

void Parser::readStream(const std::string &instream) {
    
    for (int i = 0; i < instream.length(); i++) {
        char c = instream[i];
        if (c == '\"' || c == ' ') {
            continue;
        } 

        int charPosition = tokenStream.size();
        tokenStream.push_back(std::make_shared<Token>(c, charPosition));
        
        #if DEBUG
        std::cout << tokenStream[charPosition]->type << " | " 
            << tokenStream[charPosition]->character << std::endl;
        #endif DEBUG 
    }

    tokenStream.push_back(std::make_shared<Token>(END, tokenStream.size()));
    nextToken = tokenStream[0]; //assign next token to first in stream
}

void Parser::match(Type type) {

    std::cout << "received match of type: " << type << std::endl;
    if (nextToken->type = type) {
        advance();
    }    
    // // create node from input buffer at index i
    // if (currentToken.type == x) {
    //     consume();
    // } else {
    //     // Throw Exception!
    //     return;
    // }
}

void Parser::expect(Type type) {
    //expects a certain token to be at the streamIndex
}

void Parser::advance() {
    if (streamIndex == tokenStream.size()) {
        return;
    }
    streamIndex++;
}

void Parser::regex() {

    // while (currentToken.type != Type.END) {
    //     union_();           
    // }
    union_();
}

void Parser::union_() {

    concat();
    // // union : concat
    // concat();
    // // union : concat (UNION concat)* 
    // while (currentToken.type == Type.UNION ) {
    //     match(Type.UNION);
    //     concat();
    // } 
}

void Parser::concat() {

    star();
    // // concat : star
    // star(); 
    // // concat : star
    // while (currentToken.type == Type.CONCAT) {
    //     match(Type.CONCAT);
    //     star();
    // } 
}

void Parser::star() {

    paren();
    // paren();
    // leaf();
    // while (currentToken.type == Type.STAR) {
    //     match(Type.STAR);
    // }
}

void Parser::paren() {
    if (nextToken->type == LPAREN) {
        match(LPAREN);
        leaf();
        expect(RPAREN); 
    } else if (nextToken->type == LETTER){
        match(LETTER);
    }
    return;
}

void Parser::leaf() {

    match(LETTER);

    //eventually add support for terminals other than letter...

    // int leafType = currentToken.type;
    // switch(leafType) {
    //     case Type.LETTER: match(Type.LETER); break;
    //     case Type.EPSILON: match(Type.EPSILON); break;
    //     case Type.EMPTY_SET: match(Type.EMPTY_SET); break;
    // }
}
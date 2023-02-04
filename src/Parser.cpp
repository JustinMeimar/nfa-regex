#include "Parser.h"
#define DEBUG 1

Parser::Parser() { ctx = root; }
Parser::~Parser() {}

void Parser::enterRule(std::shared_ptr<Node> rule_node) {
    //link parent node to     
    ctx->addChild(rule_node);
    rule_node->parent = ctx;
    ctx = rule_node;
}

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

    if (nextToken->type == type) {
        std::cout << "matched " << type << std::endl;
        advance(); 
    } else {
        printf("unexpected type recieved: %d, expected: %d", type, nextToken->type); 
        exit(1);
    }
}

void Parser::expect(Type type) {
    //expects a certain token to be at the streamIndex
}

void Parser::advance() {
    if (streamIndex >= tokenStream.size()) {
        return; 
    }
    streamIndex++;
    nextToken = tokenStream[streamIndex];
}

void Parser::regex() {
    #if DEBUG 
        printf("===call regex\n");
    #endif 

    std::shared_ptr<Node> regex_root = std::make_shared<Node>();
    enterRule(regex_root);
    union_();

    std::cout << "string succesfully parsed!\n" << std::endl;
}

void Parser::union_() {
    #if DEBUG   
        printf("===call union\n");
    #endif

    //link parent node to     
    std::shared_ptr<Node> union_node = std::make_shared<Node>(RULE_UNION); 
    enterRule(union_node);

    concat();
    while (nextToken->type == UNION) {
        union_node->addChild(std::make_shared<Node>(nextToken));
        match(UNION);
        ctx = union_node;
        concat();
    }  
}

void Parser::concat() {
    #if DEBUG   
        printf("===call concat\n");
    #endif

    //link child to parent and parent to child, then make ctx child.
    std::shared_ptr<Node> concat_node = std::make_shared<Node>();
    enterRule(concat_node);

    star();
    while (nextToken->type == CONCAT) {
        concat_node->addChild(std::make_shared<Node>(nextToken));
        match(CONCAT);
        ctx = concat_node;
        star();
    }
}

void Parser::star() {
    #if DEBUG
        printf("===call star\n");
    #endif 
    
    //link child to parent and parent to child, then make ctx child.
    std::shared_ptr<Node> star_node = std::make_shared<Node>();
    enterRule(star_node);

    paren();
    while (nextToken->type == STAR){
        star_node->addChild(std::make_shared<Node>(nextToken));
        match(STAR);
        ctx = star_node;
    } 
}

void Parser::paren() {
    #if DEBUG
        printf("===call paren\n");
    #endif
    
    std::shared_ptr<Node> paren_node = std::make_shared<Node>();
    enterRule(paren_node);

    if (nextToken->type == LPAREN ) {
        while (nextToken->type == LPAREN) {
            paren_node->addChild(std::make_shared<Node>(nextToken));
            match(LPAREN);
            ctx = paren_node; 
            union_(); 
            paren_node->addChild(std::make_shared<Node>(nextToken));
            match(RPAREN); 
        }
    } else { 
        leaf();
    } 
}

void Parser::leaf() { 
    #if DEBUG
        printf("===call leaf\n");
    #endif
    
    std::shared_ptr<Node> leaf_node = std::make_shared<Node>(nextToken);
    enterRule(leaf_node);

    match(LETTER);
}
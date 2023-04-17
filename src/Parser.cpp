#include "Parser.h"

Parser::Parser() { root = std::make_shared<Node>(RULE_ROOT); ctx = root; }
Parser::~Parser() {}

/** Tie the parse tree context from the current node to the 
 * newly created node.
 */
void Parser::enterRule(std::shared_ptr<Node> rule_node) {
    //link parent node to new rule node   
    ctx->addChild(rule_node);
    rule_node->parent = ctx;
    ctx = rule_node;
}

/* Creates token stream from the regex specfication passed in from
* the command line.
*/
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

/* Aserts that the following token is of the type we expect given the context.
* For example we expect a symbol after a 'U' or '&'
*/
void Parser::match(Type type) {

    if (nextToken->type == type) {
        advance(); 
    } else {
        printf("unexpected type recieved: %d, expected: %d", type, nextToken->type); 
        exit(1);
    }
}

/* Advance the pointer into the stream
 * @return void when the entire stream has been consumed.
 */
void Parser::advance() {
    if (streamIndex >= tokenStream.size()) {
        return; 
    }
    streamIndex++;
    nextToken = tokenStream[streamIndex];
}

/* Top level parser rule. Expects the special end token 
 */
void Parser::regex() {
    #if DEBUG 
        printf("===call regex\n");
    #endif 

    std::shared_ptr<Node> regex_root = std::make_shared<Node>(RULE_REGEX);
    enterRule(regex_root);
    
    while(this->nextToken->type != Type::END) {
        ctx = regex_root; 
        union_();
    }
}

/* Union parser rule. Lowest precedence thus called first. Expects
 * the 'U' symbol between two symbols as it is a binary operator. 
 */
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
        union_();
    }  
}

/** Concat parser rule. Second lowest precedence thus called below Union. Expects
 * the '&' symbol between two symbols as it is a binary operator. 
 */
void Parser::concat() {
    #if DEBUG   
        printf("===call concat\n");
    #endif

    //link child to parent and parent to child, then make ctx child.
    std::shared_ptr<Node> concat_node = std::make_shared<Node>(RULE_CONCAT);
    enterRule(concat_node);

    star();
    while (nextToken->type == CONCAT) {
        concat_node->addChild(std::make_shared<Node>(nextToken));
        match(CONCAT);
        ctx = concat_node;
        concat();
    }
}

/** Star parser rule. Highest precedence. Expects the '*' symbol, 
 * right associative on a symbol as it is a unary operator. 
 */
void Parser::star() {
    #if DEBUG
        printf("===call star\n");
    #endif 
    
    //link child to parent and parent to child, then make ctx child.
    std::shared_ptr<Node> star_node = std::make_shared<Node>(RULE_STAR);
    enterRule(star_node);

    complement();
    while (nextToken->type == STAR){
        star_node->addChild(std::make_shared<Node>(nextToken));
        match(STAR);
        ctx = star_node;
    } 
}

void Parser::complement() {
    std::shared_ptr<Node> comp_node = std::make_shared<Node>(RULE_COMP);
    enterRule(comp_node); 
    paren();
}

/** Matches a left parentheses and expects a right parentheses.
 * Calls UNION inside so that Union operator may be higher precedence when
 * wrapped in parentheses.
 */
void Parser::paren() {
    #if DEBUG
        printf("===call paren\n");
    #endif
    
    std::shared_ptr<Node> paren_node = std::make_shared<Node>(RULE_PAREN);
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

/** Matches a left parentheses and expects a right parentheses.
 * Calls UNION inside so that Union operator may be higher precedence when
 * wrapped in parentheses.
 */
void Parser::leaf() { 
    #if DEBUG
        printf("===call leaf\n");
        std::cout << "token we are assigning into the node: " << nextToken << std::endl; 
    #endif

    std::shared_ptr<Node> leaf_node = std::make_shared<Node>(nextToken);
    enterRule(leaf_node);

    match(LETTER);
}
 

/** Inserts implicit concatenation tokens in the following cases
 * 1) letter letter: "ab" -> "a&b".
 * 2) rparen letter: "(a)(b)" -> "(a)&(b)".
 * 3) rparen lapren: "(a)b" -> "(a)&b".
 * 4) letter lparen: "a(b)" -> "a&(b)".
 */   
void Parser::insertImplicitConcatTokens() {

    char concat = '&';
    for (int i = 0; i < tokenStream.size()-1; i++) {
 
        std::shared_ptr<Token> t1 = tokenStream[i]; 
        std::shared_ptr<Token> t2 = tokenStream[i+1]; 
 
        if (t1->type == LETTER && t2->type == LETTER  || t1->type == RPAREN && t2->type == LETTER 
        || t1->type == RPAREN && t2->type == LPAREN  || t1->type == LETTER && t2->type == LPAREN) {
            std::shared_ptr<Token> implicitConcat = std::make_shared<Token>(concat, 0);
            tokenStream.insert(tokenStream.begin() + i + 1, implicitConcat);
        }
    }
}
#include "Visitor.h"

/**
 * Visit the given node.
 * @param node The node to visit.
 * @return void
 */
void Visitor::visit(std::shared_ptr<Node> node) {

    switch(node->rule) {
        case RULE_ROOT:
            visitRoot(node);
            break;
        case RULE_REGEX:
            visitRegex(node); 
            break;
        case RULE_UNION:
            visitUnion(node);
            break; 
        case RULE_CONCAT:
            visitConcat(node);
            break;
        case RULE_STAR:
            visitStar(node);
            break;
        case RULE_PAREN:
            visitParen(node);
            break;
        case RULE_LEAF:
            visitLeaf(node);
            break; 
        default: 
            break;
    }

    return;
}

/**
 * Helper function to process stack operations for binary operators.
 * @param rule The parser rule associated with the binary operator.
 * @return void
 */
void Visitor::stackHelper(ParserRule rule) {
 
    std::shared_ptr<NFA> rhsNFA = expr_stack.top(); // Pop RHS NFA pointer 
    expr_stack.pop();
     
    std::shared_ptr<NFA> lhsNFA = expr_stack.top(); // Pop LHS NFA Pointer
    expr_stack.pop();

    std::shared_ptr<NFA> resultNFA = std::make_shared<NFA>(rule, lhsNFA, rhsNFA);
    expr_stack.push(resultNFA);

    return;
}

/**
 * Visit all children of the given node.
 * @param node The parent node whose children to visit.
 * @return void
 */
void Visitor::visitChildren(std::shared_ptr<Node> node) {
    
    for (auto node : node->children) {
        visit(node);
    }
    return;
}

/**
 * Visit the root node of the parse tree.
 * @param node The root node of the parse tree.
 * @return void
 */
void Visitor::visitRoot(std::shared_ptr<Node> node) {
 
    // Visit the entire tree 
    visitChildren(node);

    #if DEBUG
        std::cout << expr_stack.size() << std::endl;
    #endif

    // Then save the final NFA 
    this->nfa = expr_stack.top(); 
    expr_stack.pop(); 
    
    return;
}

/**
 * Visit a regex node.
 * @param node The regex node to visit.
 * @return void
 */
void Visitor::visitRegex(std::shared_ptr<Node> node) {
    #if DEBUG
        std::cout << "Visit Regex" << std::endl;
    #endif 
    visitChildren(node);   
    return;
}

/**
 * Visit a union node.
 * @param node The union node to visit.
 * @return void
 */
void Visitor::visitUnion(std::shared_ptr<Node> node) {
    #if DEBUG
        std::cout << "Visit Union" << std::endl;
    #endif 
    if (node->children.size() == 3) {
        // Visit LHS and RHS child nodes       
        visit(node->children[0]); 
        visit(node->children[2]);
        stackHelper(RULE_UNION);  

        return;
    } 
    
    visitChildren(node);
}

/**
 * Visit a concatenation node.
 * @param node The concatenation node to visit.
 * @return void
 */
void Visitor::visitConcat(std::shared_ptr<Node> node) {
    #if DEBUG
        std::cout << "Visit Concat" << std::endl;
    #endif 
    if (node->children.size() == 3) {
        // Visit LHS and RHS child nodes       
        visit(node->children[0]); 
        visit(node->children[2]);
        stackHelper(RULE_CONCAT);  

        return;
    } 
    
    visitChildren(node);   
}

/**
 * Visit a star node.
 * @param node The star node to visit.
 * @return void
 */
void Visitor::visitStar(std::shared_ptr<Node> node) {
    #if DEBUG
        std::cout << "Visit Star" << std::endl;
    #endif 
    
    if (node->children.size() == 2) {
        visit(node->children[0]); 
        
        std::shared_ptr<NFA> starNFA = expr_stack.top(); // Pop RHS NFA pointer 
        expr_stack.pop();      
        std::shared_ptr<NFA> resultNFA = std::make_shared<NFA>(RULE_STAR, starNFA);
        expr_stack.push(resultNFA);
        
        return;
    }  

    visitChildren(node);
}

/**
 * Visit a paren node.
 * @param node The paren node to visit.
 * @return void
 */
void Visitor::visitParen(std::shared_ptr<Node> node) {
   #if DEBUG
        std::cout << "Visit Paren" << std::endl;
    #endif 

    if (node->children.size() == 3) {
        visit(node->children[1]);
        return; 
    }
    visitChildren(node);
    return;   
}

/**
 * Visit a leaf node.
 * @param node The leaf node to visit.
 * @return void
 */
void Visitor::visitLeaf(std::shared_ptr<Node> node) {

    #if DEBUG
        std::cout << "Visit Leaf: " << node->token->character << std::endl;
    #endif 
    std::shared_ptr<NFA> atomicNFA = std::make_shared<NFA>(RULE_LEAF, node->token);
    expr_stack.push(atomicNFA);

    return;
}
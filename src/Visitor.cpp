#include "Visitor.h"

void Visitor::visit(std::shared_ptr<Node> node) {

    switch(node->rule) {
        
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

void Visitor::stackHelper(ParserRule rule) {
 
    std::shared_ptr<NFA> rhsNFA = expr_stack.top(); // Pop RHS NFA pointer 
    expr_stack.pop();
     
    std::shared_ptr<NFA> lhsNFA = expr_stack.top(); // Pop LHS NFA Pointer
    expr_stack.pop();

    std::shared_ptr<NFA> resultNFA = std::make_shared<NFA>(rule, lhsNFA, rhsNFA);
    expr_stack.push(resultNFA);

    return;
}

void Visitor::visitChildren(std::shared_ptr<Node> node) {
    
    for (auto node : node->children) {
        visit(node);
    }
    return;
}

void Visitor::visitRegex(std::shared_ptr<Node> node) {

    visitChildren(node);   
    return;
}

void Visitor::visitUnion(std::shared_ptr<Node> node) {

    if (node->children.size() == 3) {
        // Visit LHS and RHS child nodes       
        visit(node->children[0]); 
        visit(node->children[2]);
        stackHelper(RULE_UNION);  

        return;
    } 
    
    visitChildren(node);
}

void Visitor::visitConcat(std::shared_ptr<Node> node) {
    
    if (node->children.size() == 3) {
        // Visit LHS and RHS child nodes       
        visit(node->children[0]); 
        visit(node->children[2]);
        stackHelper(RULE_CONCAT);  

        return;
    } 
    
    visitChildren(node);   
}

void Visitor::visitStar(std::shared_ptr<Node> node) {
    
    //TODO: think about star more  
    if (node->children.size() == 2) {
        // Visit LHS and RHS child nodes       
        visit(node->children[0]); 
        visit(node->children[2]);
        stackHelper(RULE_STAR);  

        return;
    } 
    
    visitChildren(node);
}

void Visitor::visitParen(std::shared_ptr<Node> node) {
   
    visitChildren(node);
    return;   
}

void Visitor::visitLeaf(std::shared_ptr<Node> node) {
 
    std::shared_ptr<NFA> atomicNFA = std::make_shared<NFA>(RULE_LEAF, node->token);
    expr_stack.push(atomicNFA);

    return;
}
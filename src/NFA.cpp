#include "NFA.h"

void NFA::constructFromDFS(std::shared_ptr<Node> node) {

    // std::vector<Node> visited;
    std::set<std::shared_ptr<Node>> visited;
    std::stack<std::shared_ptr<Node>> node_stack;
    
    //implement iterative depth first search
    node_stack.push(node);   

    while (node_stack.size() != 0) { 
        std::shared_ptr<Node> top_node = node_stack.top();

        if (top_node->rule != NULL) { 
            std::cout << "rule node: " << top_node->rule << std::endl; 
        }
         else if  (top_node->token != NULL) {
            std::cout << "terminal character: " << top_node->token->character << std::endl; 
        }

        // std::cout << "terminal character: " << (top_node->token == NULL) << std::endl; 
        // std::cout << "terminal character: " << top_node->rule << std::endl;

        node_stack.pop();

        for (auto child : top_node->children)  {
            if (visited.find(child) == visited.end()) {
                visited.insert(child);
                node_stack.push(child);
            } 
        }
    }  

    return;
}
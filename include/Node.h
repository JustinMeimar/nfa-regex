#include <vector>
#include "Token.h"

enum Operator {
    STAR,      // R = (R1)*
    CONCAT,    // R = R1R2
    UNION      // R = R1 | R2 
};

enum Base {
    LETTER,     // R = a
    EPSILON,    // R = e
    EMPTY_SET   // R = 0/
};

class Node {
    private: 
        enum Type {
            Operator, 
            Base
        };

    public:
        std::vector<Node> children; 
        Token token;
        
        Node() {}
};
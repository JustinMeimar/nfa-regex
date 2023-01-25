#include "Parser.h"
#include "Token.h"

namespace Parser {
    
    Parser() {}

    void readStream(const std::string &instream) {
        std::cout << instream << std::endl;
    }

    void regex() {
        while (currentToken.type != Type.END) {
            union_();           
        }
    }
    
    void union_() {
        // union : concat
        concat();
        // union : concat (UNION concat)* 
        while (currentToken.type == Type.UNION ) {
            match(Type.UNION);
            concat();
        } 
    }
    
    void concat() {
        // concat : star
        star(); 
        // concat : star
        while (currentToken.type == Type.CONCAT) {
            match(Type.CONCAT);
            star();
        } 
    }
 
    void star() {
        leaf();
        while (currentToken.type == Type.STAR) {
            match(Type.STAR);
        }
    }

    void leaf() {
        int leafType = currentToken.type;
        switch(leafType) {
            case Type.LETTER: match(Type.LETER); break;
            case Type.EPSILON: match(Type.EPSILON); break;
            case Type.EMPTY_SET: match(Type.EMPTY_SET); break;
        }
    }

    void match(int x) {
        // create node from input buffer at index i
        if (currentToken.type == x) {
            consume();
        } else {
            // Throw Exception!
            return;
        }
    }
};
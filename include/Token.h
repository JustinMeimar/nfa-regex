enum Type {
    STAR, 
    CONCAT, 
    UNION, 
    LPAREN,
    RPAREN,
    LETTER, 
    EPSILON,
    EMPTY_SET,
    END
};

class Token {
    public:
        int charPosition;   //char position in line
        char character;     //each token is only one character in this language
        Type type;            
        Token() {}
};
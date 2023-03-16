#pragma once

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
        Token(); 
        Token(const Type END, int charPosition);
        Token(char c, int charPosition);
        Type recognizeType(const char c);
};
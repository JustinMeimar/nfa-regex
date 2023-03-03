#include "Token.h"

Token::Token() {}

Token::Token(const Type END, int charPosition) {
    this->character = 'e';
    this->type = END;
    this->charPosition = charPosition; 
}

Token::Token(char c, int charPosition) {
    this->character = c;
    this->type = recognizeType(c); 
    this->charPosition = charPosition;
} 

Type Token::recognizeType(char c) {

    Type type; 

    if (c == 'U')       {type = UNION;}
    else if (c == '&')  {type = CONCAT;}
    else if (c == '*')  {type = STAR;}
    else if (c == '\e') {type = EPSILON;}
    else if (c == '\0') {type = EMPTY_SET;}
    else if (c == '(')  {type = LPAREN;} 
    else if (c == ')')  {type = RPAREN;}
    else                {type = LETTER;}

    return type;
}
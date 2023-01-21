grammar: 

regex: subexpr*; 

subexpr: 
    | '(' subexpr ')'
    | subexpr STAR
    | subexpr CONCAT subexpr
    | subexpr UNION subexpr
    | leaf
    ;

leaf: (LETTER | EPSILON | EMPTY_SET);

UNION = 'U';
CONCAT = '&';
STAR = '*';

LETTER = [a-zA-Z];
EPSILON = '\e';
EMPTY_SET = '\0'
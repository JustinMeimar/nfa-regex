grammar: 

regex: union; 
union : concat (UNION concat)*; 
concat : star (CONCAT star)*;
star : paren (STAR)*;
paren : leaf | '('union ')';

leaf: (LETTER | EPSILON | EMPTY_SET);

UNION = 'U';
CONCAT = '&';
STAR = '*';

LETTER = [a-zA-Z];
EPSILON = '\e';
EMPTY_SET = '\0'

// example input
0. "a U b & c" 
 
0. "a & b U c"

1. (a U b)* 
should produce:

    *
    |
    U
   / \
  a   b

2. a U b*
should produce:

    U
   / \
  a   *
      |
      b
grammar: 

regex: union*; 

union : concat (UNION concat)*; 
concat : star (CONCAT star)*;
star : leaf (STAR)*;
<!-- paren : '(' star ')'; -->

leaf: (LETTER | EPSILON | EMPTY_SET);

UNION = 'U';
CONCAT = '&';
STAR = '*';

LETTER = [a-zA-Z];
EPSILON = '\e';
EMPTY_SET = '\0'

// example input

(a U b)* 
should produce:

    *
    |
    U
   / \
  a   b

a U b*
should produce:

    U
   / \
  a   *
      |
      b
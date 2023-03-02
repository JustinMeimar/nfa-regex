/*
grammar: 

regex: union; 
union : concat (UNION concat)*; 
concat : star (CONCAT star)*;
star : paren (STAR)*;
paren : leaf | '('union ')';
leaf: (LETTER | EPSILON | EMPTY_SET);
*/

#ifndef RULES_H
#define RULES_H 

enum ParserRule {
    RULE_REGEX, 
    RULE_UNION, 
    RULE_CONCAT, 
    RULE_STAR,
    RULE_PAREN, 
    RULE_LEAF
};

#endif
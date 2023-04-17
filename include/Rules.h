#ifndef RULES_H
#define RULES_H 

enum ParserRule {
    RULE_REGEX, 
    RULE_UNION, 
    RULE_CONCAT, 
    RULE_STAR,
    RULE_COMP,
    RULE_PAREN, 
    RULE_LEAF,
    RULE_ROOT
};

#endif
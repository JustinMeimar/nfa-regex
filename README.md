# NFA-Regex

### Goal
All regular expressions can be converted to a non-deterministic finite automata (NFA). The
goal is of this project is to take in some regular expression and generate a NFA to run
strings on. Any string over the base alphabet is either in the language specified by
the regular expression or it is not.  

NFA's can have an exponential running time due its parallel executions, which is makes inn order to search for accepting strings along all alternative pathways. The goal is to make that parallelism fast using GPU.

### Grammar
```
regex: union;
union: concat (UNION concat)*;
concat: star (CONCAT star)*;
star: paren (STAR)*;
paren: leaf | '('union ')';
leaf: (LETTER | EPSILON | EMPTY_SET);

UNION = 'U';
CONCAT = '&';
STAR = '*';
LETTER = [a-zA-Z];
EPSILON = '\e';
EMPTY_SET = '\0';
```
### Operator Precedence
```
0. parentheses '(' ')'
1. kleen star '*'
2. concatenation '&'
3. union 'U'
```

### State of Progress

- Grammar and tokenization
- Recursive Descent parser to recognize valid regular expressions
- Respect operator precedence. 
- Parse Tree generation 
- Generate NFA by walking the Parse Tree
- execute strings on NFA
- Union closure
- Concatenation closure
- Automated testcases

### Todo
- Compile to WASM for webiste? Or use binary method like before
- Set up command line interface to be user friendly, intuitive and with more options 

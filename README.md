# NFA-Regex

### Goal
All regular expressions can be converted to a non-deterministic finite automata (NFA). The
goal is of this project is to take in some regular expression and generate a NFA to run
strings on. Any string over the base alphabet is either in the language specified by
the regular expression or it is not. 

### How it works
We would like to take two strings, a specification for a regular
expression and an input string. First we construct a NFA from the expression by from the follwing steps:
1. Tokenize the regular expression
2. Parse the token stream with the precedence perserving grammar below.
3. Walk the tree.
  Generate atomic NFA's from the leaf nodes and push onto an expression stack.
  Any time we exit an operator node (`union`, `concat` or `star`), pop existing NFAs from the expression stack and construct a new
    NFA from its closure property.
4. Pop the last NFA off the expression stack.
5. Run the breadth-first-search `execute` method on the NFA object to make all avaialble transitions from the input string.
  `accept` if we have consumed the entire input string and the current state is accept. Otherwise `reject`

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
### Build
``git clone git@github.com:JustinMeimar/nfa-regex.git`` \
``cd nfa-regex``\
`mkdir build && cd build && cmake ..`\
`make`\
`../bin/regex <regular-expression> <input string>`

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

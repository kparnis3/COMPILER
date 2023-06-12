# Compiler Theory and Practice assignment 2022

## Description

In this assignment you are to develop a lexer, parser, semantic analyser and interpreter for the
programming language - TinyLang. The assignment is composed of three major components split
in a number of tasks. These are: i) design and implementation of a FSA-based table-driven lexer
(scanner) and hand-crafted top-down LL(k) parser, ii) implementation of visitor classes to perform
XML generation, semantic analysis and execution (evaluation) of the abstract syntax tree (AST)
produced by the parser and iii) a report detailing how you designed and implemented the different
tasks.

TinyLang is an expression-based strongly-typed programming language. TinyLang distinguishes
between expressions and statements. The language has C-style comments, that is, //. . . for line
comments and /*. . . */ for block comments. The language is case-sensitive and each function is
expected to return a value. TinyLang has 4 primitive types: ‘bool’, ‘char’, ‘int’ and ‘float’. For
‘char’ type, the range of characters supported starts with space (\x20) and ends with ˜ (\x7E).
Binary operators, such as ‘+’, require that the operands have matching types; the language does
not perform any implicit/automatic typecast.

A TinyLang program is simply a sequence of 0 or more statements. A function declaration is
itself a statement. It is declared using the ‘fn’ keyword and its arguments are type annotated.
All functions return a value and the return type must be specified after an arrow ->. Note that
functions do not necessarily have to be declared before they are used since semantic analysis and
evaluation are carried out on the AST created following a correct parse of the entire program input.
An important aspect of a programming language are the different ways to modify flow control.
TinyLang supports both branching (if-else) and looping (while, for) but no jumps to labels. Neither break nor continue are supported to exit loops

## The TinyLang programming language

The following rules describe the syntax of TinyLang in EBNF. Each rule has three parts: a left
hand side (LHS), a right-hand side (RHS) and the ‘::=’ symbol separating these two sides. The
LHS names the EBNF rule whereas the RHS provides a description of this name. Note that the
RHS uses four control forms namely sequence, choice, option and repetition. In a sequence order
is important and items appear left-to-right. The stroke symbol ( . . . | . . .) is used to denote choice
between alternatives. One item is chosen from this list; order is not important. Optional items are
enclosed in square brackets ([ . . . ]) indicating that the item can either be included or discarded.
Repeatable items are enclosed in curly brackets ({ . . . }); the items within can be repeated zero or
more times. For example, a Block consists of zero or more Statement enclosed in curly brackets.
- 〈Letter〉 ::= [A-Za-z]
- 〈Printable〉 ::= [\x20-\x7E]
- 〈Type〉 ::= ‘float’ | ‘int’ | ‘bool’ | ‘char’
- 〈BooleanLiteral〉 ::= ‘true’ | ‘false’
- 〈IntegerLiteral〉 ::= 〈Digit〉 { 〈Digit〉 }
- 〈FloatLiteral〉 ::= 〈Digit〉 { 〈Digit〉 } ‘.’ 〈Digit〉 { 〈Digit〉 }
- 〈CharLiteral〉 ::= ‘‘’ 〈Printable〉 ‘’’
- 〈Literal〉 ::= 〈BooleanLiteral〉
  - | 〈IntegerLiteral〉
  - | 〈FloatLiteral〉
  - | 〈CharLiteral〉
- 〈Identifier〉 ::= ( ‘ ’ | 〈Letter 〉 ) { ‘ ’ | 〈Letter 〉 | 〈Digit〉 }
- 〈MultiplicativeOp〉 ::= ‘*’ | ‘/’ | ‘and’
- 〈AdditiveOp〉 ::= ‘+’ | ‘-’ | ‘or’
- 〈RelationalOp〉 ::= ‘<’ | ‘>’ | ‘==’ | ‘!=’ | ‘<=’ | ‘>=’
- 〈ActualParams〉 ::= 〈Expression〉 { ‘,’ 〈Expression〉 }
- 〈FunctionCall〉 ::= 〈Identifier 〉 ‘(’ [ 〈ActualParams〉 ] ‘)’
- 〈SubExpression〉 ::= ‘(’ 〈Expression〉 ‘)’
- 〈Unary〉 ::= ( ‘-’ | ‘not’ ) 〈Expression〉
- 〈Factor〉 ::= 〈Literal〉
  - | 〈Identifier 〉
  - | 〈FunctionCall〉
  - | 〈SubExpression〉
  - | 〈Unary〉
- 〈Term〉 ::= 〈Factor 〉 { 〈MultiplicativeOp〉 〈Factor 〉 }
- 〈SimpleExpr〉 ::= 〈Term〉 { 〈AdditiveOp〉 〈Term〉 }
- 〈Expression〉 ::= 〈SimpleExpr 〉 { 〈RelationalOp〉 〈SimpleExpr 〉 }
- 〈Assignment〉 ::= 〈Identifier 〉 ‘=’ 〈Expression〉
- 〈VariableDecl〉 ::= ‘let’ 〈Identifier 〉 ‘:’ 〈Type〉 ‘=’ 〈Expression〉
- 〈PrintStatement〉 ::= ‘print’ 〈Expression〉
- 〈RtrnStatement〉 ::= ‘return’ 〈Expression〉
- 〈IfStatement〉 ::= ‘if’ ‘(’ 〈Expression〉 ‘)’ 〈Block〉 [ ‘else’ 〈Block〉 ]
- 〈ForStatement〉 ::= ‘for’ ‘(’ [ 〈VariableDecl〉 ] ’;’ 〈Expression〉 ’;’ [ 〈Assignment〉 ] ‘)’ 〈Block〉
- 〈WhileStatement〉 ::= ‘while’ ‘(’ 〈Expression〉 ‘)’ 〈Block〉
- 〈FormalParam〉 ::= 〈Identifier 〉 ‘:’ 〈Type〉
- 〈FormalParams〉 ::= 〈FormalParam〉 { ‘,’ 〈FormalParam〉 }
- 〈FunctionDecl〉 ::= ‘fn’ 〈Identifier 〉 ‘(’ [ 〈FormalParams〉 ] ‘)’ ‘->’ 〈Type〉 〈Block〉
- 〈Statement〉 ::= 〈VariableDecl〉 ‘;’
  - | 〈Assignment〉 ‘;’
  - | 〈PrintStatement〉 ‘;’
  - | 〈IfStatement〉
  - | 〈ForStatement〉
  - | 〈WhileStatement〉
  - | 〈RtrnStatement〉 ‘;’
  - | 〈FunctionDecl〉
  - | 〈Block〉
- 〈Block〉 ::= ‘{’ { 〈Statement〉 } ‘}’
- 〈Program〉 ::= { 〈Statement〉 }

## Constructing the Transition Table

When deciding what the DFA of the program will look like two important factors had to be
taken:
1.  What the actual accepted characters are.
2.  What tokens are to be considered.

For ease of reading enum classes were created for these two factors. To better understand
which tokens/characters these would be a DFA was drawn which is derived from the
specifications of the EBNF of what the final DFA should look like and thus what all the
characters and tokens are. The following is the DFA considered, the DFA has been spilt into
parts due to its scale but, every state emanates from the same starting node S0.

![1](https://github.com/kparnis3/COMPILER/assets/81303628/17984dc1-47c7-4d01-80f8-ed144d76eeb3)
![2](https://github.com/kparnis3/COMPILER/assets/81303628/1064ec56-6fdf-431e-891e-c47a1c9039e5)
![3](https://github.com/kparnis3/COMPILER/assets/81303628/7e859038-b11a-4cdb-a3ee-13a14cbd1d1d)

## The Abstract Syntax Tree

The (AST) was coded as a tree, being stored as a vector of shared
pointers which point to a particular node’s child. A class hierarchy was used to represent each
AST which allows for storage for operators, types, identifier names etc. as well as allowing
the visitor design patter to traverse these classes. Shared Pointers are used for ease of
deletion, since no loops will be found within the tree, we can simply delete the starting
ASTNode and the rest of the tree will henceforth be deleted, preventing any memory leaks
from occurring.

![4](https://github.com/kparnis3/COMPILER/assets/81303628/881e5157-b13b-4ef0-afb3-d725b7ce19dc)
![6](https://github.com/kparnis3/COMPILER/assets/81303628/13c0efa6-60cf-4a80-a040-8bfcdaf948b0)






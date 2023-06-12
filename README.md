### Compiler Theory and Practice assignment 2022

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

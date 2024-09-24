// JugaadSyntax.g4 - ANTLR grammar file for Jugaad Syntax

grammar JugaadSyntax;

// Parser rules
program : statement+ ;
statement : printStatement
          | variableDeclaration
          | functionDefinition
          | ifStatement
          | loopStatement
          | exitStatement
          | functionCall
          ;

printStatement : 'bolo' '(' (STRING | variable) ')' ';' ;
variableDeclaration : 'jugaad' variable '=' expression ';' ;
functionDefinition : 'kaamChalau' ID '(' (variable (',' variable)*)? ')' '{' statement* 'khatam' expression ';' '}' ;
ifStatement : 'dekhteHain' '(' expression ')' '{' statement* '}' ('dekhLenge' '{' statement* '}')? ;
loopStatement : ('firse' | 'jabTak') '(' expression ')' '{' statement* '}' ;
exitStatement : 'khatam' variable ';' ;  // Implemented
functionCall : ID '(' (expression (',' expression)*)? ')' ';' ;

// Lexer rules
variable : ID ;
expression : variable
           | STRING
           | NUMBER
           | expression ('*' | '/' | '+' | '-') expression
           ;
ID : [a-zA-Z_][a-zA-Z_0-9]* ;
NUMBER : [0-9]+ ;
STRING : '"' (~["\\] | '\\' .)* '"' ;
WS : [ \t\r\n]+ -> skip ;
COMMENT : '//' ~[\r\n]* -> skip ;

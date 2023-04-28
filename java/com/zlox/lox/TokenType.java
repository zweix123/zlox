package com.zlox.lox;

enum TokenType {
    LEFT_PAREN, // left paren (
    RIGHT_PAREN, // right paren )
    LEFT_BRACE, // left brace {
    RIGHT_BRACE, // right brace }
    COMMA, // comma ,
    DOT, // dot .
    MINUS, // minus -
    PLUS, // plus +
    SEMICOLON, // semicolon ;
    SLASH, // slash /
    STAR, // star *

    BANG, BANG_EQUAL, // bang, bang equal ! !=
    EQUAL, EQUAL_EQUAL, // equal, equal equal = ==
    GREATER, GREATER_EQUAL, // greater, gteater equal > >=
    LESS, LESS_EQUAL, // less, less equal < <=

    IDENTIFIER, // identifier
    STRING, // strings
    NUMBER, // number

    AND, // and
    CLASS, // class
    ELSE, // else
    FALSE, // false
    FUN, // fun
    FOR, // for
    IF, // if
    NIL, // nil
    OR, // or
    PRINT, // print
    RETURN, // return
    SUPER, // super
    THIS, // this
    TRUE, // true,
    VAR, // var
    WHILE, // while

    EOF
}
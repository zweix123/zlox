package com.zlox.lox;

enum TokenType {
    // Single-character tokens
    LEFT_PAREN, // left paren
    RIGHT_PAREN, // right paren
    LEFT_BRACE, // left brace
    RIGHT_BRACE, // right brace
    COMMA, // comma
    DOT, // dot
    MINUX, // minux
    PLUS, // plus
    SEMICOLON, // semicolon
    SLASH, // slash
    STAR, // star

    // One ro two character tokens
    BANG, BANG_EQUAL, // bang, bang equal
    EQUAL, EQUAL_EQUAL, // equal, equal equal
    GREATER, GREATER_EQUAL, // greater, gteater equal
    LESS, LESS_EQUAL, // less, less equal

    // Literals
    IDENTIFIER, // identifier
    STRING, // strings
    NUMBER, // number

    // Keywords
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

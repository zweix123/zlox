package com.zlox.lox;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import static com.zlox.lox.TokenType.*;

class Scanner {
    private final String source;
    private final List<Token> tokens = new ArrayList<>();
    private int start = 0; // 指向被扫描的词素中的第一个字符
    private int current = 0; // 指向当前正在处理的字符的下一个位置
    private int line = 1; // current所在的源文件行数
    private static final Map<String, TokenType> keywords;

    static {
        keywords = new HashMap<>();
        keywords.put("and", AND);
        keywords.put("class", CLASS);
        keywords.put("else", ELSE);
        keywords.put("false", FALSE);
        keywords.put("for", FOR);
        keywords.put("fun", FUN);
        keywords.put("if", IF);
        keywords.put("nil", NIL);
        keywords.put("or", OR);
        keywords.put("print", PRINT);
        keywords.put("return", RETURN);
        keywords.put("super", SUPER);
        keywords.put("this", THIS);
        keywords.put("true", TRUE);
        keywords.put("var", VAR);
        keywords.put("while", WHILE);
    }

    Scanner(String source) {
        this.source = source;
    }

    List<Token> scanTokens() {
        while (!isAtEnd()) {
            start = current; // 双指针, 一个token一个token的跳
            scanToken();
        }

        tokens.add(new Token(EOF, "", null, line));
        return tokens;
    }

    private void scanToken() {
        char c = advance();
        switch (c) {
            // 普通的单字符
            // @formatter:off
            case '(': addToken(LEFT_PAREN); break;
            case ')': addToken(RIGHT_PAREN); break;
            case '{': addToken(LEFT_BRACE); break;
            case '}': addToken(RIGHT_BRACE); break;
            case ',': addToken(COMMA); break;
            case '.': addToken(DOT); break;
            case '-': addToken(MINUS); break;
            case '+': addToken(PLUS); break;
            case ';': addToken(SEMICOLON); break;
            case '*': addToken(STAR); break;
            // @formatter:on

            // 有些单字符和双字符有相同前缀
            case '!': // ! and !=
                addToken(match('=') ? BANG_EQUAL : BANG);
                break;
            case '=': // = and ==
                addToken(match('=') ? EQUAL_EQUAL : EQUAL);
                break;
            case '<': // < and <=
                addToken(match('=') ? LESS_EQUAL : LESS);
                break;
            case '>': // > and >=
                addToken(match('=') ? GREATER_EQUAL : GREATER);
                break;
            case '/': // /, 除号 and //, 注释
                if (match('/')) { // 之后是注释，吞掉当前行所有
                    while (peek() != '\n' && !isAtEnd())
                        // 注意`\n`没有在这里被处理
                        advance();
                } else {
                    addToken(SLASH);
                }
                break;
            // Ignore whitespace, 注意换行有副作用
            case ' ':
            case '\r':
            case '\t':
            case '\n':
                if (c == '\n')
                    line++;
                break;

            case '"':
                string();
                break;

            default: // 数字和标识符的前缀都是多元的, 放在default
                if (isDigit(c)) {
                    number();
                } else if (isAlpha(c)) {
                    // 这里同时处理和保留字和标识符, 因为它们可能有共同的前缀
                    identifier();
                } else {
                    Lox.error(line, "Unexpected character.");
                }
                break;
        }
    }

    private void identifier() {
        while (isAlphaNumeric(peek()))
            advance();

        String text = source.substring(start, current);
        TokenType type = keywords.get(text);
        if (type == null)
            type = IDENTIFIER;
        addToken(type);
    }

    private void number() {
        while (isDigit(peek()))
            advance();

        if (peek() == '.' && isDigit(peekNext())) {
            advance();
            while (isDigit(peek()))
                advance();
        }

        addToken(NUMBER, Double.parseDouble(source.substring(start, current)));
    }

    private void string() {
        while (peek() != '"' && !isAtEnd()) {
            if (peek() == '\n') // 我们的语言的字符串可以跨行
                line++; // 艰苦朴素不能忘
            advance();
        }
        if (isAtEnd()) {
            Lox.error(line, "Unterminated string.");
            return;
        }

        advance(); // 右边的'"'

        String value = source.substring(start + 1, current - 1);
        addToken(STRING, value);
    }

    // lookahead, 有副作用
    private boolean match(char expected) {
        if (isAtEnd())
            return false;
        if (source.charAt(current) != expected)
            return false;
        current++;
        return true;
    }

    // lookahead, 无副作用
    private char peek() {
        if (isAtEnd())
            return '\0';
        return source.charAt(current);
    }

    // lookahead, 无副作用
    private char peekNext() {
        if (current + 1 >= source.length())
            return '\0';
        return source.charAt(current + 1);
    }

    private boolean isAlpha(char c) {
        return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
    }

    private boolean isAlphaNumeric(char c) {
        return isAlpha(c) || isDigit(c);
    }

    private boolean isDigit(char c) {
        return c >= '0' && c <= '9';
    }

    private boolean isAtEnd() {
        return current >= source.length();
    }

    private char advance() {
        current++; // current是超尾
        return source.charAt(current - 1);
    }

    private void addToken(TokenType type) {
        addToken(type, null);
    }

    private void addToken(TokenType type, Object literal) {
        String text = source.substring(start, current);
        tokens.add(new Token(type, text, literal, line));
    }
}
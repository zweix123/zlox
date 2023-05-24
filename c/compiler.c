#include <stdio.h>
#include <stdlib.h>

#include "common.h"
#include "compiler.h"
#include "scanner.h"

#ifdef DEBUG_PRINT_CODE
#include "debug.h"
#endif

typedef struct {
    Token previous; // 当前token
    Token current;  // 下一个token
    bool hadError;
    bool panicMode;
} Parser;

Parser parser;

typedef enum {
    // 这里是优先级从低到高的排列，而C的enum会将枚举隐式的转换成整数，即优先级实现了数值的比较
    PREC_NONE,
    PREC_ASSIGNMENT, // =
    PREC_OR,         // or
    PREC_AND,        // and
    PREC_EQUALITY,   // == !=
    PREC_COMPARISON, // < > <= >=
    PREC_TERM,       // + -
    PREC_FACTOR,     // * /
    PREC_UNARY,      // ! -
    PREC_CALL,       // . ()
    PREC_PRIMARY
} Precedence;

typedef void (*ParseFn)();

typedef struct {
    ParseFn prefix;
    ParseFn infix;
    Precedence precedence; // 优先级
} ParseRule;

Chunk *compilingChunk;

static Chunk *currentChunk() {
    return compilingChunk;
}

static void errorAt(Token *token, const char *message) {
    if (parser.panicMode) return;
    parser.panicMode = true;

    fprintf(stderr, "[line %d] Error", token->line);

    if (token->type == TOKEN_EOF) {
        fprintf(stderr, " at end");
    } else if (token->type == TOKEN_ERROR) {
        // nothing, 占位
    } else {
        fprintf(stderr, " at '%.*s'", token->length, token->start);
    }

    fprintf(stderr, ": %s\n", message);
    parser.hadError = true;
}

static void error(const char *message) { // errorAtPrevious
    errorAt(&parser.previous, message);
}

static void errorAtCurrent(const char *message) {
    errorAt(&parser.current, message);
}

static void advance() {
    parser.previous = parser.current;
    for (;;) {
        // 这里会不断的寻找正确的Token，错误的Token不会被保存
        parser.current = scanToken();
        if (parser.current.type != TOKEN_ERROR) break;
        errorAtCurrent(parser.current.start);
    }
}

static void consume(TokenType type, const char *message) {
    if (parser.current.type == type) {
        advance();
        return;
    }
    errorAtCurrent(message);
}

// 以上是前端
// ========
// 以下是后端

static void emitByte(uint8_t byte) {
    writeChunk(currentChunk(), byte, parser.previous.line);
    // 还记得么? previous是当前的
}

static void emitBytes(uint8_t byte1, uint8_t byte2) {
    emitByte(byte1);
    emitByte(byte2);
}

static void emitReturn() {
    emitByte(OP_RETURN);
}

static uint8_t makeConstant(Value value) {
    int constant = addConstant(currentChunk(), value);
    if (constant > UINT8_MAX) {
        error("Too many constants in one chunk.");
        return 0;
    }

    return (uint8_t)constant;
}

static void emitConstant(Value value) {
    emitBytes(OP_CONSTANT, makeConstant(value));
}

static void endCompiler() {
    emitReturn();
#ifdef DEBUG_PRINT_CODE
    if (!parser.hadError) { disassembleChunk(currentChunk(), "code"); }
#endif
}

static ParseRule *getRule(TokenType type);
static void parsePrecedence(Precedence precedence);
static void expression();

static void number() {
    double value = strtod(parser.previous.start, NULL);
    emitConstant(NUMBER_VAL(value));
}

static void string() {
    emitConstant(OBJ_VAL(copyString(parser.previous.start + 1, parser.previous.length - 2)));
    // 在这个函数里的调用流程比较复杂
    // 1. 创建一个新字符数组拷贝字符串(字符串脱离原本的C编译器的管理)
    // 2. 创建一个大小从满足的堆内存块
    //    1. 强制转换成Obj, 设置type
    //    2. 强制转换会ObjString, 设置其他属性(上面的字符串放入)
    // 返回最后的ObjString
}

static void grouping() {
    expression();
    consume(TOKEN_RIGHT_PAREN, "Expect ')' after expression.");
}

static void unary() {
    TokenType operatorType = parser.previous.type;

    parsePrecedence(PREC_UNARY);

    switch (operatorType) {
        case TOKEN_BANG: emitByte(OP_NOT); break;
        case TOKEN_MINUS: emitByte(OP_NEGATE); break;
        default: return;
    }
}

static void binary() {
    TokenType operatorType = parser.previous.type;
    ParseRule *rule = getRule(operatorType);
    parsePrecedence((Precedence)(rule->precedence + 1));

    switch (operatorType) {
        case TOKEN_BANG_EQUAL: emitBytes(OP_EQUAL, OP_NOT); break;
        case TOKEN_EQUAL_EQUAL: emitByte(OP_EQUAL); break;
        case TOKEN_GREATER: emitByte(OP_GREATER); break;
        case TOKEN_GREATER_EQUAL: emitBytes(OP_LESS, OP_NOT); break;
        case TOKEN_LESS: emitByte(OP_LESS); break;
        case TOKEN_LESS_EQUAL: emitBytes(OP_GREATER, OP_NOT); break;
        case TOKEN_PLUS: emitByte(OP_ADD); break;
        case TOKEN_MINUS: emitByte(OP_SUBTRACT); break;
        case TOKEN_STAR: emitByte(OP_MULTIPLY); break;
        case TOKEN_SLASH: emitByte(OP_DIVIDE); break;
        default: return; // Unreachable.
    }
}

static void literal() {
    switch (parser.previous.type) {
        case TOKEN_TRUE: emitByte(OP_TRUE); break;
        case TOKEN_FALSE: emitByte(OP_FALSE); break;
        case TOKEN_NIL: emitByte(OP_NIL); break;
        default: return;
    }
}

ParseRule rules[] = {
    [TOKEN_LEFT_PAREN] = {grouping, NULL, PREC_NONE},        // left_paren
    [TOKEN_RIGHT_PAREN] = {NULL, NULL, PREC_NONE},           // right_paren
    [TOKEN_LEFT_BRACE] = {NULL, NULL, PREC_NONE},            // left_brace
    [TOKEN_RIGHT_BRACE] = {NULL, NULL, PREC_NONE},           // right_brace
    [TOKEN_COMMA] = {NULL, NULL, PREC_NONE},                 // comma
    [TOKEN_DOT] = {NULL, NULL, PREC_NONE},                   // dot
    [TOKEN_MINUS] = {unary, binary, PREC_TERM},              // minus
    [TOKEN_PLUS] = {NULL, binary, PREC_TERM},                // plus
    [TOKEN_SEMICOLON] = {NULL, NULL, PREC_NONE},             // semicolon
    [TOKEN_SLASH] = {NULL, binary, PREC_FACTOR},             // slash
    [TOKEN_STAR] = {NULL, binary, PREC_FACTOR},              // star
    [TOKEN_BANG] = {unary, NULL, PREC_NONE},                 // bang !
    [TOKEN_BANG_EQUAL] = {NULL, binary, PREC_EQUALITY},      // bang_equal
    [TOKEN_EQUAL] = {NULL, NULL, PREC_NONE},                 // equal
    [TOKEN_EQUAL_EQUAL] = {NULL, binary, PREC_EQUALITY},     // equal_equal
    [TOKEN_GREATER] = {NULL, binary, PREC_COMPARISON},       // greater
    [TOKEN_GREATER_EQUAL] = {NULL, binary, PREC_COMPARISON}, // greater_equal
    [TOKEN_LESS] = {NULL, binary, PREC_COMPARISON},          // less
    [TOKEN_LESS_EQUAL] = {NULL, binary, PREC_COMPARISON},    // less_equal
    [TOKEN_IDENTIFIER] = {NULL, NULL, PREC_NONE},            // identifier
    [TOKEN_STRING] = {string, NULL, PREC_NONE},              // string
    [TOKEN_NUMBER] = {number, NULL, PREC_NONE},              // number
    [TOKEN_AND] = {NULL, NULL, PREC_NONE},                   // and
    [TOKEN_CLASS] = {NULL, NULL, PREC_NONE},                 // class
    [TOKEN_ELSE] = {NULL, NULL, PREC_NONE},                  // else
    [TOKEN_FALSE] = {literal, NULL, PREC_NONE},              // false
    [TOKEN_FOR] = {NULL, NULL, PREC_NONE},                   // for
    [TOKEN_FUN] = {NULL, NULL, PREC_NONE},                   // fun
    [TOKEN_IF] = {NULL, NULL, PREC_NONE},                    // if
    [TOKEN_NIL] = {literal, NULL, PREC_NONE},                // nil
    [TOKEN_OR] = {NULL, NULL, PREC_NONE},                    // or
    [TOKEN_PRINT] = {NULL, NULL, PREC_NONE},                 // print
    [TOKEN_RETURN] = {NULL, NULL, PREC_NONE},                // return
    [TOKEN_SUPER] = {NULL, NULL, PREC_NONE},                 // super
    [TOKEN_THIS] = {NULL, NULL, PREC_NONE},                  // this
    [TOKEN_TRUE] = {literal, NULL, PREC_NONE},               // true
    [TOKEN_VAR] = {NULL, NULL, PREC_NONE},                   // var
    [TOKEN_WHILE] = {NULL, NULL, PREC_NONE},                 // while
    [TOKEN_ERROR] = {NULL, NULL, PREC_NONE},                 // error
    [TOKEN_EOF] = {NULL, NULL, PREC_NONE}};                  // eof

static ParseRule *getRule(TokenType type) {
    return &rules[type];
}

static void parsePrecedence(Precedence precedence) {
    // 该函数实现从当前Token开始，去parse比precedence更高优先级的表达式
    advance();
    ParseFn prefixRule = getRule(parser.previous.type)->prefix;
    if (prefixRule == NULL) {
        error("Expect expression.");
        return;
    }

    prefixRule();

    while (precedence <= getRule(parser.current.type)->precedence) {
        advance();
        ParseFn infixRule = getRule(parser.previous.type)->infix;
        infixRule();
    }
}

static void expression() {
    parsePrecedence(PREC_ASSIGNMENT);
}

bool compile(const char *source, Chunk *chunk) {
    initScanner(source);
    compilingChunk = chunk;

    parser.hadError = false;
    parser.panicMode = false;

    advance();
    expression();

    consume(TOKEN_EOF, "Expect end of expression.");
    endCompiler();
    return !parser.hadError;
}
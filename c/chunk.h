#ifndef clox_chunk_h
#define clox_chunk_h

#include "common.h"
#include "value.h"

typedef enum {
    OP_CONSTANT, // op_constant
    OP_NIL,      // nil: 压入nil
    OP_TRUE,     // true: 压入true
    OP_FALSE,    // false: 压入false
    OP_EQUAL,    // equal
    OP_GREATER,  // greater
    OP_LESS,     // less
    OP_ADD,      // op_add +
    OP_SUBTRACT, // op_subtract -
    OP_MULTIPLY, // op_multiply *
    OP_DIVIDE,   // op_divide /
    OP_NOT,      // not !
    OP_NEGATE,   // op_negate -
    OP_RETURN,   // op_return
} OpCode;        // operation code

typedef struct {
    int count;
    int capacity;
    uint8_t *code;
    int *lines;
    ValueArray constants; // 常量池
} Chunk;

void initChunk(Chunk *chunk);
void freeChunk(Chunk *chunk);
void writeChunk(Chunk *chunk, uint8_t byte, int line);
int addConstant(Chunk *chunk, Value value);

#endif
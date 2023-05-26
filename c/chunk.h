#ifndef clox_chunk_h
#define clox_chunk_h

#include "common.h"
#include "value.h"

// 虚拟机字节码
typedef enum {
    OP_CONSTANT, // constant: 从常量池取出常量压入栈, args: 常量在常量池的索引
    OP_NIL,      // nil: 向栈中压入nil
    OP_TRUE,     // true: 向栈中压入true
    OP_FALSE,    // false: 向栈中压入false
    OP_POP,      //
    OP_GET_GLOBAL,    //
    OP_DEFINE_GLOBAL, //
    OP_SET_GLOBAL,    //
    OP_EQUAL, // equal: 取出栈顶两个元素进行比较, 并将结构压入栈中
    OP_GREATER,  // greater >:
    OP_LESS,     // less <:
    OP_ADD,      // add +:
    OP_SUBTRACT, // subtract 中缀-:
    OP_MULTIPLY, // multiply *:
    OP_DIVIDE,   // divide /:
    OP_NOT,      // not !:
    OP_NEGATE,   // negate 前缀-
    OP_PRINT,    //
    OP_RETURN,   // return
} OpCode;        // operation code

// 并没有<=、>=、!=
//  因为 !(a < b) -> (a >= b)
//      !(a > b) -> (a <= b)
//      !(a == b) -> (a != b)
//  有一个例外, 就是数字Nan, 在C中它和任何数字比较都是false

// 字节码存储
typedef struct {
    int count;
    int capacity;
    uint8_t *code;
    // 以上三个实现一个vector存储字节码
    int *lines;
    // 这个数组维护上面字节码数组对应索引的字节码在源代码中的行号
    ValueArray constants; // 常量池, 也是一个vector
} Chunk;

void initChunk(Chunk *chunk);
void freeChunk(Chunk *chunk);
void writeChunk(Chunk *chunk, uint8_t byte, int line);
int addConstant(Chunk *chunk, Value value);

#endif
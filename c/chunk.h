#ifndef clox_chunk_h
#define clox_chunk_h

#include "common.h"
#include "value.h"

// 虚拟机字节码
typedef enum {
    OP_CONSTANT, // op arg: 从常量池中取出索引为arg的常量放入栈中
    OP_NIL,      // op: 将NIL放入栈中
    OP_TRUE,     // 类似上
    OP_FALSE,    // 类似上
    OP_POP,      // 弹出栈顶
    // (全局)变量名存在常量池中,
    // 下面三个命令的arg即为其索引,
    // 其值存储在global哈希表中
    OP_DEFINE_GLOBAL, // op arg, 取出栈顶元素作为变量名的value放入global
    OP_GET_GLOBAL, // op arg, 取出变量名的value放入栈中
    OP_SET_GLOBAL, // op arg, 将栈顶元素作为变量名在global中的value
    // 而局部变量直接存在栈中, 下面的arg是栈的地址
    OP_GET_LOCAL, // op arg, 取出栈arg位置的值入栈
    OP_SET_LOCAL, // op_arg, 直接修改arg位置的值
    OP_EQUAL, // equal: 取出栈顶两个元素进行比较, 并将结构压入栈中
    OP_GREATER,       // greater >:
    OP_LESS,          // less <:
    OP_ADD,           // add +:
    OP_SUBTRACT,      // subtract 中缀-:
    OP_MULTIPLY,      // multiply *:
    OP_DIVIDE,        // divide /:
    OP_NOT,           // not !:
    OP_NEGATE,        // negate 前缀-
    OP_PRINT,         // op, "取出"栈顶元素并打印
    OP_JUMP,          //
    OP_JUMP_IF_FALSE, //
    OP_LOOP,          //
    OP_RETURN,        // return
} OpCode;             // operation code

// 并没有<=、>=、!=
//  因为 !(a < b) -> (a >= b)
//      !(a > b) -> (a <= b)
//      !(a == b) -> (a != b)
//  有一个例外, 就是数字Nan, 在C中它和任何数字比较都是false

// 字节码存储
typedef struct {
    int count;
    int capacity;
    uint8_t* code;
    // 以上三个实现一个vector存储字节码
    int* lines;
    // 这个数组维护上面字节码数组对应索引的字节码在源代码中的行号
    ValueArray constants; // 常量池, 也是一个vector
} Chunk;

void initChunk(Chunk* chunk);
void freeChunk(Chunk* chunk);
void writeChunk(Chunk* chunk, uint8_t byte, int line);
int addConstant(Chunk* chunk, Value value);

#endif
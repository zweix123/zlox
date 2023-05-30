#ifndef clox_vm_h
#define clox_vm_h

#include "chunk.h"
#include "value.h"
#include "table.h"
#define STACK_MAX 256

typedef struct {
    Chunk* chunk;           // 字节码
    uint8_t* ip;            // IP寄存器模拟
    Value stack[STACK_MAX]; // 运行时栈
    Value* stackTop;        // 栈顶指针
    Table globals;          // 全局变量
    Table strings;          // 字符串驻留
    Obj* objects;           // 不定内存(链表)
} VM;

typedef enum {
    INTERPRET_OK,            // ok
    INTERPRET_COMPILE_ERROR, // compile
    INTERPRET_RUNTIME_ERROR  // runtime
} InterpretResult;

extern VM vm;

void initVM();
void freeVM();
InterpretResult interpret(const char* source);

void push(Value value);
Value pop();

#endif
#ifndef clox_vm_h
#define clox_vm_h

#include "chunk.h"
#include "value.h"
#include "table.h"
#define STACK_MAX 256

typedef struct {
    Chunk *chunk;
    uint8_t *ip; // 指向即将执行的指令指针
    Value stack[STACK_MAX];
    Value *stackTop;
    Table strings; // 用于string interning字符串驻留
    Obj *objects;
} VM;

typedef enum {
    INTERPRET_OK,
    INTERPRET_COMPILE_ERROR, // compile
    INTERPRET_RUNTIME_ERROR  // runtime
} InterpretResult;

extern VM vm;

void initVM();
void freeVM();
InterpretResult interpret(const char *source);
void push(Value value);
Value pop();

#endif
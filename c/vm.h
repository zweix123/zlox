#ifndef clox_vm_h
#define clox_vm_h

#include "chunk.h"
#include "value.h"
#include "table.h"
#define STACK_MAX 256

typedef struct {
    Chunk *chunk; // 字节码和常量池
    uint8_t *ip;  // 模拟IP寄存器, 指向即将执行的指令指针
    Value stack[STACK_MAX]; // 运行时栈
    Value *stackTop;        // 栈指针
    Table globals;          // 全局变量变量名
    Table strings;          // 用于string interning
    Obj *objects;           // 相当于堆内存
} VM;

typedef enum {
    INTERPRET_OK,            // ok
    INTERPRET_COMPILE_ERROR, // compile
    INTERPRET_RUNTIME_ERROR  // runtime
} InterpretResult;

extern VM vm; // extern关键字用于声明一个变量或函数是在其他文件中定义的

void initVM();
void freeVM();
InterpretResult interpret(const char *source);

void push(Value value);
Value pop();

#endif
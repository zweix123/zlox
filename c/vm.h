#ifndef clox_vm_h
#define clox_vm_h

#include "object.h"
#include "chunk.h"
#include "value.h"
#include "table.h"

#define FRAMES_MAX 64
#define STACK_MAX  (FRAMES_MAX * UINT8_COUNT)

typedef struct {
    ObjFunction* function;
    uint8_t* ip;  // 返回地址
    Value* slots; // 虚拟机的栈中该函数可以使用的第一个slot槽
                  // 这里的指针的语义不是数组, 而是指针, 全局只有一个常量池,
                  // 在调用栈中接力
} CallFrame;

typedef struct {
    CallFrame
        frames[FRAMES_MAX]; // 调用栈, 每层栈都有独立的字节码、常量池和调用栈
    int frameCount;
    Value stack[STACK_MAX]; // 运行时栈
    Value* stackTop;        // 栈顶指针
    Table globals;          // 全局变量
    Table strings;          // 字符串驻留
    Obj* objects;           // 不定内存(链表)
} VM;

typedef enum {
    INTERPRET_OK,            // ok
    INTERPRET_COMPILE_ERROR, // compile_error
    INTERPRET_RUNTIME_ERROR  // runtime_error
} InterpretResult;

extern VM vm;

void initVM();
void freeVM();
InterpretResult interpret(const char* source);

void push(Value value);
Value pop();

#endif

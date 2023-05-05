#ifndef clox_value_h
#define clox_value_h

#include "common.h"

// tagged union
typedef enum {
    VAL_BOOL,
    VAL_NIL,
    VAL_NUMBER,
} ValueType;

typedef struct {
    ValueType type; // 4 byte

    union {         // 取max type byte
        bool boolean;
        double number;
    } as;
} Value;

#define IS_BOOL(value)   ((value).type == VAL_BOOL)
#define IS_NIL(value)    ((value).type == VAL_NIL)
#define IS_NUMBER(value) ((value).type == VAL_NUMBER)
#define AS_BOOL(value)   ((value).as.boolean)
#define AS_NUMBER(value) ((value).as.number)
// 上面并没有类型检测
// 下面要求提供正常的C值
#define BOOL_VAL(value)   ((Value){VAL_BOOL, {.boolean = value}})
#define NIL_VAL           ((Value){VAL_NIL, {.number = 0}})
#define NUMBER_VAL(value) ((Value){VAL_NUMBER, {.number = value}})

typedef struct {
    int capacity;
    int count;
    Value *values;
} ValueArray;

bool valuesEqual(Value a, Value b);

void initValueArray(ValueArray *array);               // 初始化
void writeValueArray(ValueArray *array, Value value); // 添加元素
void freeValueArray(ValueArray *array);               // 释放内存

void printValue(Value value);                         // 打印Value类型值

#endif
#ifndef clox_value_h
#define clox_value_h

#include "common.h"

typedef double Value;

typedef struct {
    int capacity;
    int count;
    Value *values;
} ValueArray;

void initValueArray(ValueArray *array);               // 初始化
void writeValueArray(ValueArray *array, Value value); // 添加元素
void freeValueArray(ValueArray *array);               // 释放内存

void printValue(Value value);                         // 打印Value类型值

#endif
#include <stdio.h>
#include <string.h>

#include "memory.h"
#include "object.h"
#include "table.h"
#include "value.h"
#include "vm.h"

#define ALLOCATE_OBJ(type, objectType) \
    (type*)allocateObject(sizeof(type), objectType)

static Obj* allocateObject(size_t size, ObjType type) {
    Obj* object = (Obj*)reallocate(NULL, 0, size);
    object->type = type;
    vm.objects = object;
    return object;
}

static ObjString* allocateString(char* chars, int length, uint32_t hash) {
    ObjString* string = ALLOCATE_OBJ(ObjString, OBJ_STRING);
    string->length = length;
    string->chars = chars;
    string->hash = hash;
    tableSet(&vm.strings, string, NIL_VAL);
    return string;
}

static uint32_t hashString(const char* key, int length) { // FNV-1a
    uint32_t hash = 2166136261u;
    for (int i = 0; i < length; i++) {
        hash ^= (uint8_t)key[i];
        hash *= 16777619;
    }
    return hash;
}

ObjString* takeString(char* chars, int length) {
    // 虚拟机内部使用
    // 将C下的字符串转换成lox的ObjString
    // 该字符串可以直接用(C语义下的)
    uint32_t hash = hashString(chars, length);
    ObjString* interned = tableFindString(&vm.strings, chars, length, hash);
    if (interned != NULL) {
        FREE_ARRAY(char, chars, length + 1);
        return interned;
    }
    return allocateString(chars, length, hash);
}

ObjString* copyString(const char* chars, int length) {
    // 编译器使用的
    // 将C接受的字符串放在虚拟机中
    // 需要深拷贝, 因为不知道虚拟机在使用时这个字符串在C的状态
    uint32_t hash = hashString(chars, length);
    ObjString* interned = tableFindString(&vm.strings, chars, length, hash);
    if (interned != NULL) return interned;
    // 如果这个字符串已经被虚拟机驻留过, 说明已经被接管过, 不用再拷贝独立的了
    char* heapChars = ALLOCATE(char, length + 1);
    memcpy(heapChars, chars, length);
    heapChars[length] = '\0';
    return allocateString(heapChars, length, hash);
}

void printObject(Value value) {
    switch (OBJ_TYPE(value)) {
        case OBJ_STRING: printf("%s", AS_CSTRING(value)); break;
        default: break;
    }
}

void showObjString(ObjString* osp) {
    if (!osp) printf("The ObjString pointer is NULL");
    else
        printf("OBJ_STRING(%s, length=%d)", osp->chars, osp->length);
}
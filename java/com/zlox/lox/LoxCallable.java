package com.zlox.lox;

import java.util.List;

interface LoxCallable {
    int arity(); // 参数个数

    Object call(Interpreter interpreter, List<Object> arguments); // 调用
}
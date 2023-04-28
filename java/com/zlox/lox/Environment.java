package com.zlox.lox;

import java.util.HashMap;
import java.util.Map;

public class Environment {
    private final Map<String, Object> values = new HashMap<>();
    // 这里的key使用string而不是token，因为同一个名称在多个token中

    Object get(Token name) {
        if (values.containsKey(name.lexeme)) {
            return values.get(name.lexeme);
        }
        throw new RuntimeError(name, "Undefined variable '" + name.lexeme + "'.");
        // 为什么将未定义操作放在运行时？因为如果在静态检查的错误，则前面的函数调用后面的函数则不行
    }

    void assign(Token name, Object value) {
        if (values.containsKey(name.lexeme)) {
            values.put(name.lexeme, value);
            return;
        }

        throw new RuntimeError(name,
                "Undefined variable '" + name.lexeme + "'.");
    }

    void define(String name, Object value) {
        values.put(name, value); // 这里没有检测name是否存在, 即var可以重复定义统一名称
    }
}

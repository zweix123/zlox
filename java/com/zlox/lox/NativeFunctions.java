package com.zlox.lox;

import java.util.List;

public class NativeFunctions {
    public static class Clock implements LoxCallable {
        @Override
        public int arity() {
            return 0;
        }

        @Override
        public Object call(Interpreter interpreter, List<Object> arguments) {
            return (double) System.currentTimeMillis() / 1000.0;
        }

        @Override
        public String toString() {
            return "<native fn clock>";
        }
    }

    public static class Exit implements LoxCallable {
        @Override
        public int arity() {
            return 0;
        }

        @Override
        public Object call(Interpreter interpreter, List<Object> arguments) {
            System.exit(0);
            return null;
        }

        @Override
        public String toString() {
            return "<native fn exit>";
        }
    }
}
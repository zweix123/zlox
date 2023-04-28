package com.zlox.lox;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.nio.charset.Charset;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.List;

public class Lox {
    static boolean hadError = false; // scanner遇到错误
    private static final Interpreter interpreter = new Interpreter();
    static boolean hadRuntimeError = false; // parser遇到错误

    public static void main(String[] args) {
        try {
            if (args.length > 1) {
                System.out.println("Usage: jlox [script]");
                System.exit(64);
            } else if (args.length == 1) {
                runFile(args[0]);
            } else {
                runPrompt();
            }
        } catch (IOException e) {
            System.out.println("发生异常: " + e.getMessage());
        }
    }

    private static void runFile(String path) throws IOException {
        byte[] bytes = Files.readAllBytes(Paths.get(path));
        run(new String(bytes, Charset.defaultCharset()));

        if (hadError)
            System.exit(65);
        if (hadRuntimeError)
            System.exit(70);
    }

    private static void runPrompt() throws IOException {
        InputStreamReader input = new InputStreamReader(System.in);
        BufferedReader reader = new BufferedReader(input);

        String prompt = "> ";
        for (;;) {
            System.out.print(prompt);
            String line = reader.readLine();
            if (line == null)
                break;
            run(line);
            hadError = false;
        }
    }

    private static void run(String source) {
        Scanner scanner = new Scanner(source);
        List<Token> tokens = scanner.scanTokens();

        // 测试词法分析器
        // for (Token token : tokens) {
        // System.out.println(token);
        // }

        Parser parser = new Parser(tokens);
        List<Stmt> statements = parser.parse();

        // // Stop if there was a syntax error.
        if (hadError)
            return;

        interpreter.interpret(statements);
    }

    private static void report(int line, String where, String message) {
        System.err.println("[line " + line + "] Error" + where + ": " + message);
        hadError = true;
    }

    static void error(int line, String message) { // 词法分析错误
        report(line, "", message);
    }

    static void error(Token token, String message) { // 句法分析错误
        if (token.type == TokenType.EOF) {
            report(token.line, " at end", message);
        } else {
            report(token.line, " at '" + token.lexeme + "'", message);
        }
    }

    static void runtimeError(RuntimeError error) { //
        System.err.println(error.getMessage() + "\n[line " + error.token.line + "]");
        hadRuntimeError = true;
    }
}
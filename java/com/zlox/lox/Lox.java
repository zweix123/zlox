package com.zlox.lox;

import java.io.IOException;
import java.nio.charset.Charset;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.List;

import org.jline.reader.LineReader;
import org.jline.reader.LineReaderBuilder;
import org.jline.terminal.Terminal;
import org.jline.terminal.TerminalBuilder;

public class Lox {
    private static final Interpreter interpreter = new Interpreter();
    static boolean hadStaticError = false;
    static boolean hadRuntimeError = false;

    public static void main(String[] args) {
        try {
            if (args.length > 1) {
                System.out.println("Usage: 直接运行进入命令行模式或者提供一个文件路径作为参数解释代码");
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

        if (hadStaticError)
            System.exit(65);
        if (hadRuntimeError)
            System.exit(70);
    }

    private static void runPrompt() throws IOException {
        Terminal terminal = TerminalBuilder.terminal();
        LineReader reader = LineReaderBuilder.builder().terminal(terminal).build();

        String prompt = "> ";
        for (;;) {
            String line = reader.readLine(prompt);
            if (line == null)
                break;
            run(line);
            hadStaticError = false;
        }
    }

    private static void run(String source) {
        Scanner scanner = new Scanner(source);
        List<Token> tokens = scanner.scanTokens(); // token列表

        Parser parser = new Parser(tokens);
        List<Stmt> statements = parser.parse(); // 抽象语法树的根节点列表

        // // Stop if there was a syntax error.
        if (hadStaticError)
            return;

        Resolver resolver = new Resolver(interpreter);
        resolver.resolve(statements);

        // Stop if there was a resolution error.
        if (hadStaticError)
            return;

        interpreter.interpret(statements);
    }

    private static void report(int line, String where, String message) {
        System.err.println("[line " + line + "] Error" + where + ": " + message);
        hadStaticError = true;
    }

    static void error(int line, String message) { // StaticError
        report(line, "", message);
    }

    static void error(Token token, String message) {
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
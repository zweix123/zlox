package com.zlox.tool;

import java.io.IOException;
import java.io.PrintWriter;
import java.util.Arrays;
import java.util.List;
import java.nio.file.Paths;

public class GenerateAst {
    public static void main(String[] args) throws IOException {
        if (args.length != 1) {
            System.err.println("Usage: generate_ast <output directory>");
            System.exit(64);
        }
        String outputDir = args[0];

        defineAst(outputDir, "Expr", Arrays.asList(
                "Binary   : Expr left, Token operator, Expr right",
                "Grouping : Expr expression",
                "Literal  : Object value",
                "Unary    : Token operator, Expr right"));

        defineAst(outputDir, "Stmt", Arrays.asList(
                "Expression : Expr expression",
                "Print      : Expr expression"));
    }

    private static void defineAst(String outputDir, String baseName, List<String> types) throws IOException {
        String path = Paths.get(outputDir).resolve(baseName + ".java").toString();
        PrintWriter writer = null;
        try {
            writer = new PrintWriter(path, "UTF-8");

            writer.println("package com.zlox.lox;");
            writer.println();
            // writer.println("import java.util.List;");
            // writer.println();
            writer.println("abstract class" + " " + baseName + " " + "{");
            defineVisitor(writer, baseName, types);
            writer.println();

            for (String type : types) {
                String className = type.split(":")[0].trim();
                String fields = type.split(":")[1].trim();
                defineType(writer, baseName, className, fields);
                writer.println();
            }

            writer.println("\t" + "abstract <R> R accept(Visitor<R> visitor);");

            writer.println("}");
        } catch (IOException e) {
            throw e;
        } finally {
            if (writer != null) {
                writer.close();
            }
        }
    }

    private static void defineVisitor(PrintWriter writer, String baseName, List<String> types) {
        writer.println("\t" + "interface Visitor<R>" + " " + "{");

        for (String type : types) {
            String typeName = type.split(":")[0].trim();
            writer.println("\t\t" + "R" + " " + "visit" + typeName + baseName +
                    "(" + typeName + " " + baseName.toLowerCase() + ");");
            writer.println();
        }

        writer.println("\t" + "}");
    }

    private static void defineType(PrintWriter writer, String baseName, String className, String fieldList) {
        writer.println("\t" + "static class" + " " + className + " " + "extends" + " " + baseName + " " + "{");

        writer.println("\t\t" + className + "(" + fieldList + ")" + " " + "{");

        String[] fields = fieldList.split(", ");
        for (String field : fields) {
            String name = field.split(" ")[1];
            writer.println("\t\t\t" + "this." + name + " " + "=" + " " + name + ";");
        }

        writer.println("\t\t" + "}");

        writer.println();
        writer.println("\t\t" + "@Override");
        writer.println("\t\t" + "<R> R accept(Visitor<R> visitor) {");
        writer.println("\t\t\t" + "return visitor.visit" + className + baseName + "(this);");
        writer.println("\t\t" + "}");

        writer.println();
        for (String field : fields) {
            writer.println("\t\t" + "final" + " " + field + ";");
        }

        writer.println("\t" + "}");
    }
}

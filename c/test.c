#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "scanner.h"
#include "compiler.h"
#include "linenoise.h"

void test() {
    char *line;
    while ((line = linenoise("> ")) != NULL) {
        initScanner(line);

        for (Token token = scanToken(); token.type != TOKEN_EOF;
             token = scanToken()) {
            showToken(token);
        }

        linenoiseFree(line);
    }
}

void test_scanner(const char *s) {
    if (s == NULL) {
        char line[1024];
        for (;;) {
            printf("> ");
            if (!fgets(line, sizeof(line), stdin)) {
                printf("\n");
                break;
            }
            initScanner(line);
            for (Token token = scanToken(); token.type != TOKEN_EOF;
                 token = scanToken()) {
                showToken(token);
            }
        }
    } else {
        initScanner(s);
        for (Token token = scanToken(); token.type != TOKEN_EOF;
             token = scanToken()) {
            showToken(token);
        }
    }
}

void test_compiler(const char *s) {
    if (s == NULL) {
        char line[1024];
        for (;;) {
            printf("> ");
            if (!fgets(line, sizeof(line), stdin)) {
                printf("\n");
                break;
            }
            Chunk chunk;
            initChunk(&chunk);

            bool ok = compile(line, &chunk);
            printf("%d\n", ok);

            freeChunk(&chunk);
        }
    } else {
        Chunk chunk;
        initChunk(&chunk);
        bool ok = compile(s, &chunk);
        printf("%d\n", ok);
        freeChunk(&chunk);
    }
}

int main(int argc, const char *argv[]) {
    const char *s = "!(5 - 4 > 3 * 2 == ! nil)";
    test_scanner(s);
    test_compiler(s);
    return 0;
}
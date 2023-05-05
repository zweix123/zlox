#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "scanner.h"
#include "linenoise.h"

void test_scanner() {
    char* line;
    while ((line = linenoise("> ")) != NULL) {
        initScanner(line);

        for (Token token = scanToken(); token.type != TOKEN_EOF;
             token = scanToken()) {
            showToken(token);
        }

        linenoiseFree(line);
    }
}

int main(int argc, const char *argv[]) {
    test_scanner();
    return 0;
}
#include <stdio.h>

#include "compiler.h"
#include "lexer.h"

void compile(char* source){
    initLexer(source);


    for(;;) {
        Token token = scanToken();
        printToken(token);

        if (token.type == TOKEN_EOF || token.type == TOKEN_ERROR) break;
    }
}

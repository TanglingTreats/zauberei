#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "lexer.h"

Lexer lexer;

Lexer initLexer(const char *source) {
    lexer.line = 1;
    lexer.column = 1;
    lexer.start = source;
    lexer.current = source;

    return lexer;
}

Token makeToken(TokenType type) {
    Token token;
    token.type = type;
    token.start = lexer.start;
    token.length = lexer.current - lexer.start;
    token.column = lexer.column;

    return token;
}

Token makeErrorToken() {
    Token token;
    token.type = TOKEN_ERROR;
    token.start = lexer.start;
    token.length = 0;
    token.column = lexer.column;

    return token;
}

static bool isTheEnd() {
    return *lexer.current == '\0';
}

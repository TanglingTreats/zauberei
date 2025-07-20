#include "lexer.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

Lexer lexer;

const char *const ttype_string[] = {
    // Single-character tokens
    "TOKEN_LEFT_PAREN", "TOKEN_RIGHT_PAREN", "TOKEN_LEFT_BRACE",
    "TOKEN_RIGHT_BRACE", "TOKEN_COMMA",
    "TOKEN_DOT", "TOKEN_PLUS", "TOKEN_MINUS", "TOKEN_SEMICOLON", "TOKEN_SLASH",
    "TOKEN_STAR", "TOKEN_HASH",
    // One or two character tokens
    "TOKEN_BANG", "TOKEN_BANG_EQUAL", "TOKEN_EQUAL", "TOKEN_EQUAL_EQUAL",
    "TOKEN_GREATER", "TOKEN_GREATER_EQUAL", "TOKEN_LESS", "TOKEN_LESSER_EQUAL",
    // Literals
    "TOKEN_IDENTIFIER", "TOKEN_STRING", "TOKEN_NUMBER",
    // Keywords
    "TOKEN_INT", "TOKEN_CHAR", "TOKEN_BYTE", "TOKEN_IF", "TOKEN_ELSE",
    "TOKEN_ELIF", "TOKEN_FUNC", "TOKEN_TRUE", "TOKEN_FALSE", "TOKEN_WHILE",
    "TOKEN_AND", "TOKEN_OR",
    // Misc
    "TOKEN_ERROR", "TOKEN_EOF"
};

static bool isTheEnd() { return *lexer.current == '\0'; }

static char advance() {
  lexer.current++;
  lexer.column++;
  return lexer.current[-1];
}

static void newLine() {
  lexer.line++;
  lexer.column = 1;
}

static void skipBlockCmmt() {
  advance();
  advance();
}

static bool match(char expected) {
  if (isTheEnd()) return false;
  if (*lexer.current != expected) return false;
  lexer.current++;
  return true;
}

static char peek() { return *lexer.current; }

static char peekNext() {
  if (isTheEnd())
    return '\0';
  return lexer.current[1];
}

static Token makeToken(TokenType type) {
  Token token;
  token.type = type;
  token.start = lexer.start;
  token.length = (int)(lexer.current - lexer.start);
  token.line = lexer.line;
  token.column = lexer.column - token.length - 1;

  return token;
}

static Token makeErrorToken(const char *message) {
  Token token;
  token.type = TOKEN_ERROR;
  token.start = message;
  token.length = strlen(message);
  token.line = lexer.line;
  token.column = lexer.column;

  return token;
}

static void skipWhitespace() {
  for(;;) {
    char c = peek();

    switch (c) {
    case ' ':
    case '\r':
    case '\t':
      advance();
      break;

    case '\n':
      newLine();
      advance();
      break;

    case '/':
      if (peekNext() == '/') {
        // Continue until end-of-line
        while(peek() != '\n' && !isTheEnd()) {
          advance();
        }

        newLine();

        if (peekNext() == '/') {
          advance();
        }

      } else if (peekNext() == '*') {
        skipBlockCmmt();
        while((peek() != '*' && peekNext() != '/') && !isTheEnd()) {
          advance();
          if(peek() == '\n') {
            newLine();
          }
        }
        skipBlockCmmt();


        // When there are subsequent comments
        if (peek() == '\n') {
          newLine();
          advance();
        }

      } else {
        return;
      }
      break;
    default:
      return;
    }
  }
}

static TokenType checkKeyword(int start, int length, const char* rest, TokenType type) {

  if (lexer.current - lexer.start == start + length &&
      memcmp(lexer.start + start, rest, length) == 0) {
    return type;
  }

  return TOKEN_IDENTIFIER;
}

static bool isAlpha(char c) {
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

static bool isDigit(char c) {
  return c >= '0' && c <= '9';
}

static Token string() {
  while (peek() != '"' && !isTheEnd()) {
    if(peek() =='\n') newLine();
    advance();
  }

  if (isTheEnd()) return makeErrorToken("Unterminated string.");

  // Closing quote
  advance();

  return makeToken(TOKEN_STRING);
}

static Token number() {
  while (isDigit(peek())) {
    advance();
  }

  if (peek() == '.' && isDigit(peekNext())) {
    // Consume the period
    advance();

    while (isDigit(peek())) advance();
  }

  return makeToken(TOKEN_NUMBER);
}

static TokenType identifierType() {
  switch (lexer.start[0]) {
    case 'a': return checkKeyword(1, 2, "nd", TOKEN_AND);
    case 'b': return checkKeyword(1, 3, "yte", TOKEN_BYTE);
    case 'c': return checkKeyword(1, 3, "har", TOKEN_CHAR);
    case 'f': return checkKeyword(1, 1, "n", TOKEN_FUNC);
    case 'i': return checkKeyword(1, 2, "nt", TOKEN_INT);
  }

  return TOKEN_IDENTIFIER;
}

static Token identifier() {
  while (isAlpha(peek()) || isDigit(peek())) advance();
  return makeToken(identifierType());
}

void initLexer(const char *source) {
  lexer.line = 1;
  lexer.column = 1;
  lexer.start = source;
  lexer.current = source;
}

Token scanToken() {

  skipWhitespace();
  lexer.start = lexer.current;

  if (isTheEnd()) {
    return makeToken(TOKEN_EOF);
  }

  // Gets previous char. Lexer points 1 char ahead
  char c = advance();
  if (isAlpha(c)) return identifier();
  if (isDigit(c)) return number();

  switch (c) {
  // One-char tokens
  case '(':
    return makeToken(TOKEN_LEFT_PAREN);
  case ')':
    return makeToken(TOKEN_LEFT_PAREN);
  case '{':
    return makeToken(TOKEN_LEFT_BRACE);
  case '}':
    return makeToken(TOKEN_RIGHT_BRACE);
  case ',':
    return makeToken(TOKEN_COMMA);
  case '.':
    return makeToken(TOKEN_DOT);
  // One or more char tokens
  case '*':
    return makeToken(TOKEN_STAR);
  case '/':
    return makeToken(TOKEN_SLASH);
  case '!':
    return makeToken(match('=') ? TOKEN_BANG_EQUAL : TOKEN_BANG);
  case '=':
    return makeToken(match('=') ? TOKEN_EQUAL_EQUAL : TOKEN_EQUAL);
  case '<':
    return makeToken(match('=') ? TOKEN_LESSER_EQUAL : TOKEN_LESS);
  case '>':
    return makeToken(match('=') ? TOKEN_GREATER_EQUAL : TOKEN_GREATER);
    // Variable char tokens
  case '"':
    return string();
  }

  return makeErrorToken("Unexpected character");
}

void printToken(Token token) {
    int line = -1;

    if (token.line != line) {
      printf("%4d:%d - ", token.line, token.column);
      line = token.line;
    } else {
      printf("    | ");
    }

    printf("%s '%.*s'\n", ttype_string[token.type], token.length, token.start);
}

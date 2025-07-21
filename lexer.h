#ifndef LEXER_H_
#define LEXER_H_

typedef enum {
  // Single-character tokens
  TOKEN_LEFT_PAREN,
  TOKEN_RIGHT_PAREN,
  TOKEN_LEFT_BRACE,
  TOKEN_RIGHT_BRACE,
  TOKEN_COMMA,
  TOKEN_DOT,
  TOKEN_PLUS,
  TOKEN_MINUS,
  TOKEN_SEMICOLON,
  TOKEN_SLASH,
  TOKEN_STAR,
  TOKEN_HASH,
  // One or two character tokens
  TOKEN_BANG,
  TOKEN_BANG_EQUAL,
  TOKEN_EQUAL,
  TOKEN_EQUAL_EQUAL,
  TOKEN_GREATER,
  TOKEN_GREATER_EQUAL,
  TOKEN_LESS,
  TOKEN_LESSER_EQUAL,
  // Literals
  TOKEN_IDENTIFIER,
  TOKEN_STRING,
  TOKEN_NUMBER,
  // Keywords
  TOKEN_INT,
  TOKEN_CHAR,
  TOKEN_BYTE,
  TOKEN_IF,
  TOKEN_ELSE,
  TOKEN_ELIF,
  TOKEN_FUNC,
  TOKEN_TRUE,
  TOKEN_FALSE,
  TOKEN_WHILE,
  TOKEN_AND,
  TOKEN_OR,
  // Misc
  TOKEN_ERROR,
  TOKEN_EOF

} TokenType;

typedef struct {
  TokenType type;
  const char *start;
  int length;
  int line;
  int column;
} Token;

/*
** Looks through source code and constructs tokens
*/
typedef struct {
  const char *start;
  const char *current;
  int buflen;
  int line;
  int column;
} Lexer;

void initLexer(const char *source);
Token scanToken();

void parse();
// For debugging
void printToken(Token token);

#endif // LEXER_H_

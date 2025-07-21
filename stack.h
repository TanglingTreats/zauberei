#ifndef STACK_H_
#define STACK_H_

#include "lexer.h"

void push(Token);

Token pop();

typedef struct {
    int size;
    int capacity;

    Token items[];
} Stack;


#endif // STACK_H_

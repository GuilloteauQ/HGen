#ifndef _STACK_H_

#define _STACK_H_

#include "stack.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
struct stack {
    bool val; // true if ,{ false if }
    struct stack *next;
};

struct stack *new_stack(bool value) ;

bool top(struct stack *stack) ;

void pop(struct stack **stack) ;

void push_value(struct stack **stack, bool value) ;

void free_stack(struct stack *stack) ;

int length(struct stack *stack) ;

#endif
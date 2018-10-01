#include "stack.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/* [raw]
struct stack {
    bool val; // true if ,{ false if }
    struct stack *next;
};*/


// Returns a new stack with the value
struct stack *new_stack(bool value) {
    struct stack *new = malloc(sizeof(struct stack));

    new->val = value;
    new->next = NULL;

    return new;
}

// Returns the top element of the stack
bool top(struct stack *stack) {
    return stack->val;
}

// Deletes the top element of the stack
void pop(struct stack **stack) {
    assert (*stack != NULL);
    struct stack *new = (*stack)->next;

    free(*stack);
    *stack = new;
}

// Adds a new value to the stack only of it the same value as the top of the stack
void push_value(struct stack **stack, bool value) {
    if (*stack == NULL) {
        // if the stack is empty
        *stack = new_stack(value);
    } else {
        if (top(*stack) == value) {
            struct stack *new = new_stack(value);
            new->next = *stack;
            *stack = new;
        } else {
            // if we are pushing a { and if the last element is } then we pop the top
            // because they canceled each other out
            pop(stack);
        }
    }
}


// Frees the stack
void free_stack(struct stack *stack) {
    if (stack != NULL) {
        free_stack(stack->next);
        free(stack);
    }
}

// Returns the length of the stack
int length(struct stack *stack) {
    if (stack != NULL) {
        return 1 + length(stack->next);
    } else {
        return 0;
    }
}


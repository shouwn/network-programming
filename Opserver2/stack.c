#include "stack.h"

void push(Stack* stack, char oprator) {
	stack->arr[stack->size++] = oprator;
}

Stack* stack_init() {
	Stack* stack = (Stack*)malloc(sizeof(Stack));
	stack->size = 0;
	return stack;
}

char pop(Stack* stack) {
	return stack->arr[--stack->size];
}
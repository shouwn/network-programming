#include "stack.h"

void push(Stack* stack, int operand) {
	stack->arr[stack->size++] = operand;
}

Stack* stack_init() {
	Stack* stack = (Stack*)malloc(sizeof(Stack));
	stack->size = 0;
	return stack;
}

int pop(Stack* stack) {
	return stack->arr[--stack->size];
}

int peak(Stack* stack) {
	return stack->arr[stack->size - 1];
}

//if stack is empty return 1 not empty return 0
int isEmpty(Stack* stack) {
	return stack->size == 0;
}

void printStack(Stack* stack) {
	int i = 0;

	printf("\n stack : ");

	for (i = stack->size - 1; i >= 0; i--) {
		printf("%c ", stack->arr[i]);
	}

	printf("\n");
}
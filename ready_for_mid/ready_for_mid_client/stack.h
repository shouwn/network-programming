#pragma once

#include <stdlib.h>

typedef struct stack {
	char arr[10];
	int size;
} Stack;

Stack* stack_init();
void push(Stack* stack, char operator);
char pop(Stack* stack);
char peak(Stack* stack);
int isEmpty(Stack* stack);
void printStack(Stack* stack);
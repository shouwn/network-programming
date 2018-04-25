#pragma once

#include <stdlib.h>

typedef struct stack {
	int arr[10];
	int size;
} Stack;

Stack* stack_init();
void push(Stack* stack, char operator);
int pop(Stack* stack);
int peak(Stack* stack);
int isEmpty(Stack* stack);
void printStack(Stack* stack);
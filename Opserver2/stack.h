#pragma once

#include <stdlib.h>

typedef struct stack {
	void* arr[10];
	int size;
} Stack;

Stack* stack_init();
void push(Stack* stack, char oprator);
char pop(Stack* stack);
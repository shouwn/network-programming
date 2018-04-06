// HelloClientWin.cpp : Defines the entry point for the console application.
//
#include <stdio.h>
#include <winsock2.h>
#include <stdlib.h>
#include <string.h>
#include "stack.h"

#define BUF_SIZE 1024
#define RLT_SIZE 4
#define OPSZ 4

void ErrorHandling(char* message);
int scanInfix2Postfix(char* buffer);

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET hSocket;
	SOCKADDR_IN servAddr;

	char opMsg[BUF_SIZE];
	int result, opndCnt, i, inputLen;

	if (argc != 3) {
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error!");

	hSocket = socket(PF_INET, SOCK_STREAM, 0);
	if (hSocket == INVALID_SOCKET)
		ErrorHandling("socket() error");

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(argv[1]);
	servAddr.sin_port = htons(atoi(argv[2]));

	if (connect(hSocket, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
		ErrorHandling("connect() error!");
	else
		puts("Connected........\n");

	inputLen = scanInfix2Postfix(opMsg);

	send(hSocket, opMsg, inputLen, 0);

	recv(hSocket, (char*)&result, RLT_SIZE, 0);

	printf("Operation result: %d \n", result);

	closesocket(hSocket);
	WSACleanup();
	return 0;
}

void ErrorHandling(char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

int scanInfix2Postfix(char* buffer) {
	int opndCnt, i, len;
	char operator;

	Stack* stack = stack_init();

	fputs("Operand count: ", stdout);
	scanf("%d", &opndCnt);
	buffer[0] = opndCnt;

	len = 1;

	for (i = 0; i < opndCnt - 1; i++) {

		printf("Operand %d: ", i + 1);

		buffer[len++] = 4;
		scanf_s("%d", (int*)&buffer[len]);
		len += sizeof(int);


		fgetc(stdin);
		fputs("Operator: ", stdout);
		scanf_s("%c", &operator);

		if (isEmpty(stack))
			push(stack, operator);
		else {
			switch (operator) {
				case '+': case '-': {
					while (!isEmpty(stack)) {
						buffer[len++] = 1;
						buffer[len++] = pop(stack);
					}

					push(stack, operator);
					break;
				}
				case '*': {
					while (!isEmpty(stack) && peak(stack) == '*'){
						printStack(stack);
						buffer[len++] = 1;
						buffer[len++] = pop(stack);
					}

					push(stack, operator);
					break;
				}
			}
		}

	}
	buffer[len++] = 4;
	scanf_s("%d", (int*)&buffer[len]);
	len += sizeof(int);

	while (!isEmpty(stack)) {
		buffer[len++] = 1;
		buffer[len++] = pop(stack);
	}

	free(stack);

	return len;
}

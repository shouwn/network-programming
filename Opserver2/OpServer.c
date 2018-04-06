// HelloServerWin.cpp : Defines the entry point for the console application.
//

#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <string.h>
#include "stack.h"

#define BUF_SIZE 1024
#define OPSZ 4

void ErrorHandling(char * message);
int calculate(int opnum, int opnds[], char oprator);
char* infix2Postfix(char* buffer);
int calculate2(char* opMsg);
int cal(int opnd1, int opnd2, char op);

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET hServSock, hClntSock;
	SOCKADDR_IN servAddr, clntAddr;

	char message[30] = "Connection Complete...!!!";

	int szClntAddr;

	char opInfo[BUF_SIZE];

	int result, opndCnt, i;
	int strLen, recvCnt, recvLen;

	if (argc != 2) {
		printf("Uasge : %s <port>\n", argv[0]);
		exit(1);
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error!");

	hServSock = socket(PF_INET, SOCK_STREAM, 0);
	if (hServSock == INVALID_SOCKET)
		ErrorHandling("socket() error");

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(atoi(argv[1]));

	if (bind(hServSock, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
		ErrorHandling("bind() error");

	if (listen(hServSock, 5) == SOCKET_ERROR)
		ErrorHandling("listen() error");

	fputs("Server Lisening\n", stdout);

	szClntAddr = sizeof(clntAddr);

	for (i = 0; i < 5; i++) {
		opndCnt = 0;
		hClntSock = accept(hServSock, (SOCKADDR*)&clntAddr, &szClntAddr);
		printf("%s %d\n", message, i + 1);

		recv(hClntSock, &opInfo, 1, 0);
		opndCnt = opInfo[0];

		recvLen = 1;
		while ((opndCnt * (OPSZ + 1) - 1) > recvLen)
		{
			recvCnt = recv(hClntSock, (char*)&opInfo[recvLen], BUF_SIZE - 1, 0);
			recvLen += recvCnt;
		}

		result = calculate2(opInfo);

		printf("\n\nresult : %d \n", result);

		send(hClntSock, (char*)&result, sizeof(result), 0);

		closesocket(hClntSock);
	}

	closesocket(hServSock);
	WSACleanup();
	return 0;
}

int calculate(int opnum, int opnds[], char op) {
	int result = opnds[0], i;
	switch (op) {
	case '+':
		for (i = 1; i < opnum; i++) result += opnds[i];
		break;
	case '-':
		for (i = 1; i < opnum; i++) result -= opnds[i];
		break;
	case '*':
		for (i = 1; i < opnum; i++) result *= opnds[i];
		break;
	}

	return result;
}

int calculate2(char * opMsg) {

	int len = 0, i;
	int opCnt = opMsg[0];
	int max = opCnt * 2 - 1;
	int type, opnd1, opnd2;
	int result;
	Stack* stack = stack_init();

	len = 1;

	for (i = 0; i < max; i++) {

		type = opMsg[len++];

		if (type == OPSZ) {
			printf("%d",opMsg[len]);
			len += OPSZ;
		}
		else {
			printf("%c", opMsg[len++]);
		}
	}

	printf("\n\n전체 시작 max : %d\n", max);

	len = 1;

	for (i = 0; i < max; i++) {

		printf("시작 len : %d, ", len);

		type = opMsg[len++];

		printf("type : %d\n", type);

		if (type == OPSZ) {
			printf("\t숫자 스택 추가 시작\n");
			push(stack, *(int*)&(opMsg[len]));
			len += OPSZ;
			printf("\t숫자 스택 추가 종료\n\n");
		}
		else {
			printf("\t계산 시작\n");
			opnd2 = pop(stack);
			opnd1 = pop(stack);
			push(stack, cal(opnd1, opnd2, opMsg[len++]));
			printf("\t계산 종료\n");
		}
		printf("종료\n");
	}
	printf("전체 종료");

	result = pop(stack);
	free(stack);

	return result;
}

int cal(int opnd1, int opnd2, char op) {

	switch (op) {
	case '+': return opnd1 + opnd2;
	case '-': return opnd1 - opnd2;
	case '*': return opnd1 * opnd2;
	}
}

void ErrorHandling(char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}


char* infix2Postfix(char* buffer) {
	int opndCnt, i, len;
	char operator;
	char result[1024];

	Stack* stack = stack_init();

	opndCnt = buffer[0];
	result[0] = opndCnt;

	len = 1;

	for (i = 0; i < opndCnt; i++) {

		*((int*)&(result[len])) = *((int*)&(buffer[len]));
		len += sizeof(int);
		
		operator = buffer[len];

		if (isEmpty(stack))
			push(stack, operator);
		else {
			switch (operator) {
				case '+': case '-': {
					while (!isEmpty(stack))
						result[len++] = pop(stack);
					break;
				}
				case '*': {
					if (peak(stack) == '*')
						while (!isEmpty(stack))
							result[len++] = pop(stack);
					else
						push(stack, operator);

					break;
				}
			}
		}
	}

	free(stack);
	return result;
}

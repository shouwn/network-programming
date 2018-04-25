#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include "stack.h"

void ErrorHandling(char * message);
void recvMessage(SOCKET sock, char* buf, int strLen);
int postCalculate(char * opMsg);
int cal(int opnd1, int opnd2, char op);

int main(int argc, char* argv[])
{

	WSADATA wsaData;
	SOCKET servSock, clntSock, sendLen;
	SOCKADDR_IN servAddr, clntAddr;
	int i;
	int szClntAddr;
	int messageLen;
	char readBuf[1024];
	
	FILE* fp;
	char fileName[100];
	int fSize;
	//fp = fopen(fileName, "rt"); //read mode
	//fSize = fread(read_buf, 1, 1024, fp); //read by 1byte for 1024byte
	//fclose(fp); //close file

	int result;
	//write here


	char success[] = "success";
	char failed[] = "failed";

	//Listening
	if (argc != 2) {
		printf("Uasge : %s <port>\n", argv[0]);
		exit(1);
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error!");

	servSock = socket(PF_INET, SOCK_STREAM, 0);
	if (servSock == INVALID_SOCKET)
		ErrorHandling("socket() error");

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(atoi(argv[1]));

	if (bind(servSock, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
		ErrorHandling("bind() error");

	if (listen(servSock, 5) == SOCKET_ERROR)
		ErrorHandling("listen() error");
	else
		fputs("Server: listening\n", stdout);
	//Listening


	//accept
	szClntAddr = sizeof(clntAddr);
	clntSock = accept(servSock, (SOCKADDR*)&clntAddr, &szClntAddr);

	if (clntSock == INVALID_SOCKET)
		ErrorHandling("accept() error");
	else
		fputs("Server: accept\n", stdout);
	//accept

	recvMessage(clntSock, (char*)&messageLen, sizeof(int));
	recvMessage(clntSock, readBuf, messageLen);

	result = postCalculate(readBuf);
	send(clntSock, (char*)&result, sizeof(result), 0);
	printf("client request : %s \n result : %d\n", readBuf, result);

	closesocket(clntSock);
	closesocket(servSock);
	WSACleanup();
	return 0;
}

void recvMessage(SOCKET sock, char* buf, int strLen) {
	int recvLen = 0;
	int recvCnt;

	memset(buf, 0, sizeof(buf));
	while (strLen > recvLen) {
		recvCnt = recv(sock, &buf[recvLen], strLen, 0);

		if (strLen == -1)
			ErrorHandling("read() error!");

		recvLen += recvCnt;
	}
}


int postCalculate(char * opMsg) {

	int len = 0, i;
	int max = ((int) opMsg[0]) * 2 - 1;
	int opnd1, opnd2;
	int result;
	Stack* stack = stack_init();

	len = 1;

	for (i = 0; i < max; i++) {

		if (opMsg[len++] == sizeof(int)) {
			push(stack, *(int*)&(opMsg[len]));
			len += sizeof(int);
		}
		else {
			opnd2 = pop(stack);
			opnd1 = pop(stack);
			push(stack, cal(opnd1, opnd2, opMsg[len++]));
		}
	}

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
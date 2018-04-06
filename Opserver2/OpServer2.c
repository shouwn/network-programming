// HelloServerWin.cpp : Defines the entry point for the console application.
//

#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <string.h>

#define BUF_SIZE 1024
#define OPSZ 4

void ErrorHandling(char * message);
int calculate(int opnum, int opnds[], char oprator);

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

		recv(hClntSock, &opndCnt, 1, 0);
		printf("%s %d\n", message, i + 1);

		recvLen = 0;
		while ((opndCnt * OPSZ + 1) > recvLen)
		{
			recvCnt = recv(hClntSock, (char*)&opInfo[recvLen], BUF_SIZE - 1, 0);
			recvLen += recvCnt;
		}

		result = calculate(opndCnt, (int*)opInfo, opInfo[recvLen - 1]);

		printf("result : %d \n", result);

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

void ErrorHandling(char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
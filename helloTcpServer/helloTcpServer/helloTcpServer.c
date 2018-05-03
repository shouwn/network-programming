// HelloServerWin.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>

void ErrorHandling(char * message);

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET hServSock, hClntSock, sendLen;
	SOCKADDR_IN servAddr, clntAddr;

	int szClntAddr;
	int strLen, i, recvLen, len;
	char buffer[1024];
	char* message[] = { "Hello Clinet!", "I'm server", "Nice to meet you" };

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

	szClntAddr = sizeof(clntAddr);
	hClntSock = accept(hServSock, (SOCKADDR*)&clntAddr, &szClntAddr);

	if (hClntSock == INVALID_SOCKET)
		ErrorHandling("accept() error");

	for (i = 0; i < 3; i++) {
		strLen = strlen(message[i]) + 1;
		send(hClntSock, (char*)&strLen, sizeof(int), 0);
		send(hClntSock, message[i], strLen, 0);

		recv(hClntSock, (char*)&strLen, sizeof(int), 0);
		recv(hClntSock, buffer, strLen, 0);

		puts(buffer);
	}

	closesocket(hClntSock);
	closesocket(hServSock);
	WSACleanup();
	return 0;
}

void ErrorHandling(char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}


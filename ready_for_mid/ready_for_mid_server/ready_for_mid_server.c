#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>

void ErrorHandling(char * message);
void recvMessage(SOCKET sock, char* buf, int strLen);

int main(int argc, char* argv[])
{
	char fileName[100];
	int fSize;
	FILE* fp;

	WSADATA wsaData;
	SOCKET servSock, clntSock, sendLen;
	SOCKADDR_IN servAddr, clntAddr;

	int szClntAddr;
	int strLen, i, recvLen, len;
	char readBuf[1024];
	char* message;
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

	recvMessage(clntSock, readBuf, sizeof("Hello"));
	send(clntSock, "Hello", sizeof("Hello"), 0);

	puts(readBuf);

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

void ErrorHandling(char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
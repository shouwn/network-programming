#include "stdafx.h"
#include <stdio.h>
#include <winsock2.h>
#include <stdlib.h>

void ErrorHandling(char* message);
void recvMessage(SOCKET sock, char* buf, int len);

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET sock;
	SOCKADDR_IN servAddr;

	char readBuf[1024];
	char fileName[100];
	int strLen, i, recvLen, len, sendLen;
	char message[30];

	// connection
	if (argc != 3) {
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error!");

	sock = socket(PF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
		ErrorHandling("socket() error");

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(argv[1]);
	servAddr.sin_port = htons(atoi(argv[2]));

	if (connect(sock, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
		ErrorHandling("connect() error!");
	else
		fputs("client: connected\n", stdout);
	// connection

	send(sock, "Hello", sizeof("Hello"), 0);
	recvMessage(sock, readBuf, sizeof("Hello"));

	puts(readBuf);

	closesocket(sock);
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
// HelloClientWin.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#include <winsock2.h>
#include <stdlib.h>

void ErrorHandling(char* message);

int main(int argc, char* argv[])
{
	FILE* fp;
	int fSize;

	WSADATA wsaData;
	SOCKET hSocket;
	SOCKADDR_IN servAddr;

	char read_buf[1024];
	char fileName[100];
	int strLen, i, recvLen, len, sendLen;
	char message[30];

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

	scanf_s("%s", fileName, sizeof(fileName));

	strLen = strlen(fileName) + 1;
	send(hSocket, (char*)&strLen, sizeof(int), 0);
	send(hSocket, fileName, strLen, 0);

	recv(hSocket, (char*)&strLen, sizeof(int), 0);
	memset(read_buf, 0, sizeof(char) * 1024);

	if (strLen >= 0) {
		recv(hSocket, read_buf, strLen, 0);
		fp = fopen("receive.txt", "wt");
		fwrite(read_buf, 1, 1024, fp);
		fclose(fp);
	}
	memset(message, 0, sizeof(char) * 30);
	recv(hSocket, (char*)&strLen, sizeof(int), 0);
	recv(hSocket, message, strLen, 0);

	puts(message);
	puts(read_buf);

	closesocket(hSocket);
	WSACleanup();
	return 0;
}

void ErrorHandling(char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}


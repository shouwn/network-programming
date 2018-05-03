// HelloClientWin.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#include <winsock2.h>
#include <stdlib.h>

#define BUF_SIZE 100

void ErrorHandling(char* message);

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET sock;
	SOCKADDR_IN servAddr;

	char message[BUF_SIZE];
	int strLen;

	if (argc != 3) {
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error!");

	sock = socket(PF_INET, SOCK_DGRAM, 0);

	if (sock == INVALID_SOCKET)
		ErrorHandling("socket() error");

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(argv[1]);
	servAddr.sin_port = htons(atoi(argv[2]));

	connect(sock, (SOCKADDR*)&servAddr, sizeof(servAddr));

	fputs("Insert message : ", stdout);
	fgets(message, sizeof(message), stdin);

	send(sock, message, strlen(message), 0);

	memset(message, 0, sizeof(message));

	strLen = recv(sock, message, sizeof(message), 0);
	printf("Message from server: %s", message);

	closesocket(sock);
	WSACleanup();
	return 0;
}

void ErrorHandling(char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}


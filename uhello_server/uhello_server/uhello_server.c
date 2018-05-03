// HelloServerWin.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>

#define BUF_SIZE 100

void ErrorHandling(char * message);

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET servSock;
	SOCKADDR_IN servAddr, clntAddr;

	int i;
	int clntAdrSz;
	int strLen;
	char message[BUF_SIZE] = "Connection Complete...!!!";

	if (argc != 2) {
		printf("Uasge : %s <port>\n", argv[0]);
		exit(1);
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error!");

	servSock = socket(PF_INET, SOCK_DGRAM, 0);
	if (servSock == INVALID_SOCKET)
		ErrorHandling("socket() error");

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(atoi(argv[1]));

	if (bind(servSock, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
		ErrorHandling("bind() error");

	clntAdrSz = sizeof(clntAddr);
	memset(message, 0, sizeof(message));

	strLen = recvfrom(servSock, message, BUF_SIZE, 0,
		(SOCKADDR*)&clntAddr, &clntAdrSz);

	printf("Message from server: %s", message);

	fputs("\nInsert message : ", stdout);

	memset(message, 0, sizeof(message));
	fgets(message, sizeof(message), stdin);

	sendto(servSock, message, strLen, 0,
		(SOCKADDR*)&clntAddr, sizeof(clntAddr));

	closesocket(servSock);
	WSACleanup();
	return 0;
}

void ErrorHandling(char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}


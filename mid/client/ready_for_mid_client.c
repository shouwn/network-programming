#include "stdafx.h"
#include <stdio.h>
#include <winsock2.h>
#include <stdlib.h>

void ErrorHandling(char* message);
void recvMessage(SOCKET sock, char* buf);

int main(int argc, char* argv[])
{
	const int COLLECT = 200;
	const int FAIL = -200;

	int i;

	WSADATA wsaData;
	SOCKET sock;
	SOCKADDR_IN servAddr;

	char readBuf[100];
	int messageLen;

	int result;
	int count;
	int input;

	int recvLen;
	int recvCnt;
	int strLen;

	//wrtie here

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
	// connection

	printf("숫자를 맞추시오.\n\n");
	
	do {
		recvLen = 0;

		printf("숫자를 입력하시오: ");
		scanf("%d", &input);

		send(sock, (char*)&input, sizeof(int), 0);

		recv(sock, (char*)&result, sizeof(int), 0);

		if (result == COLLECT) {
			recv(sock, (char*)&count, sizeof(int), 0);
			recv(sock, (char*)&strLen, sizeof(int), 0);

			memset(readBuf, 0, sizeof(readBuf));
			while (strLen > recvLen) {
				recvCnt = recv(sock, &readBuf[recvLen], strLen, 0);

				if (strLen == -1)
					ErrorHandling("read() error!");

				recvLen += recvCnt;
			}

			printf("%s\n", readBuf);
		}
		else {

			recv(sock, (char*)&strLen, sizeof(int), 0);
			memset(readBuf, 0, sizeof(readBuf));

			while (strLen > recvLen) {
				recvCnt = recv(sock, &readBuf[recvLen], strLen, 0);

				if (strLen == -1)
					ErrorHandling("read() error!");

				recvLen += recvCnt;
			}

			printf("%s\n", readBuf);
		}
	} while (result != COLLECT);

	printf("%d번 만에 맞추었습니다. 클라이언트 종료합니다.\n", count);

	closesocket(sock);
	WSACleanup();
	return 0;
}

void ErrorHandling(char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
#include "stdafx.h"
#include <stdio.h>
#include <winsock2.h>
#include <stdlib.h>
#include "stack.h"

void ErrorHandling(char* message);
void recvMessage(SOCKET sock, char* buf, int len);
void printOp(char* buffer);
int scanOp(char* buffer);

int main(int argc, char* argv[])
{
	int i;

	WSADATA wsaData;
	SOCKET sock;
	SOCKADDR_IN servAddr;

	char readBuf[1024];
	int messageLen;

	FILE* fp;
	char fileName[100];
	int fSize;
	//fp = fopen(fileName, "wt"); //read mode
	//fSize = fread(read_buf, 1, 1024, fp); //read by 1byte for 1024byte
	//fclose(fp); //close file

	int result;
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
	else
		fputs("client: connected\n", stdout);
	// connection

	messageLen = scanOp(readBuf);
	send(sock, readBuf, messageLen, 0);

	recvMessage(sock, (char*)&result, sizeof(int));

	printOp(readBuf);
	printf(" = %d\n", result);

	puts("\nclient end\n");

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

int scanOp(char* buffer) {
	int opndCnt, len = 0, i;

	memset(buffer, 0, sizeof(buffer));

	printf("Operand count: ");
	scanf("%d", &opndCnt);
	buffer[len++] = opndCnt;

	fgetc(stdin);

	for (i = 0; i < opndCnt - 1; i++) {
		printf("Operand %d: ", i + 1);
		scanf("%d", (int*)&buffer[len]);
		len += 4;

		fgetc(stdin);

		printf("Operator: ");
		scanf("%c", &buffer[len++]);
	}
	fgetc(stdin);

	printf("Operand %d: ", i + 1);
	scanf("%d", (int*)&buffer[len]);
	len += 4;

	return len;
}

void printOp(char* buffer) {
	int opndCnt;
	int len = 0;
	int i;

	opndCnt = buffer[len++];

	for (i = 0; i < opndCnt - 1; i++) {
		printf("%d ", *(int*)&buffer[len]);
		len += 4;
		printf("%c ", buffer[len++]);
	}

	printf("%d", *(int*)&buffer[len]);
}

void ErrorHandling(char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
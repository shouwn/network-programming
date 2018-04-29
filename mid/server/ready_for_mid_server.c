#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <time.h>

void ErrorHandling(char * message);
void recvMessage(SOCKET sock, char* buf);
void sendMessage(SOCKET sock, char* message);

int main(int argc, char* argv[])
{
	const int COLLECT = 200;
	const int FAIL = -200;

	WSADATA wsaData;
	SOCKET servSock, clntSock, sendLen;
	SOCKADDR_IN servAddr, clntAddr;
	int i;
	int szClntAddr;
	int messageLen;
	char readBuf[10];
	
	int random;
	int recvNum;
	int temp;
	//write here

	char * message;
	char smaller[] = "그것 보다 작다";
	char larger[] = "그것 보다 크다";
	char end[] = "정답. 서버를 종료합니다.";
	int result;

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
		fputs("연결 성공...\n", stdout);
	//accept

	srand((unsigned)time(NULL));
	random = rand() % 101;

	printf("난수 생성: %d\n", random);

	i = 0;
	do {
		i++;
		recv(clntSock, (char*)&recvNum, sizeof(int), 0);
		printf("%d를 입력 받음\n", recvNum);

		if (recvNum == random) {
			send(clntSock, (char*)&COLLECT, sizeof(COLLECT), 0);
			
			send(clntSock, (char*)&i, sizeof(int), 0);
			
			temp = strlen(end);
			send(clntSock, (char*)&temp, sizeof(int), 0);
			send(clntSock, end, temp, 0);
		}
		else {
			send(clntSock, (char*)&FAIL, sizeof(FAIL), 0);
			message = (recvNum < random) ? larger : smaller;
			
			temp = strlen(message);
			send(clntSock, (char*)&temp, sizeof(int), 0);
			send(clntSock, message, temp, 0);
		}

	} while (recvNum != random);

	printf("%s\n", end);

	closesocket(clntSock);
	closesocket(servSock);
	WSACleanup();
	return 0;
}

void recvMessage(SOCKET sock, char* buf) {
	int recvLen = 0;
	int recvCnt;
	int strLen;

	memset(buf, 0, sizeof(buf));

	recv(sock, (char*)&strLen, sizeof(int), 0);

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
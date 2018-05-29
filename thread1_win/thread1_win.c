// thread1_win.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h> /* _beginthreadex, _endthreadex - ���� �߰��� �����带 ���̰� ���� �� ���*/
#include <process.h>
unsigned WINAPI ThreadFunc(void *arg);
//WINAPI�� �����쿡�� �����ϴ� ȣ�� �Ծ�
//���������� ȣ���� �� �̷� ������ �ض�

int main()
{
	HANDLE hThread;//�����쿡���� �ڵ�� ���ҽ��� ����
	unsigned threadID;
	int param = 5;

	//�ڵ� ���� hThread�� �����ؼ� ������
	hThread = (HANDLE)_beginthreadex(NULL, 0, ThreadFunc, (void*)&param, 0, &threadID);
	//                                        ������ ����
	if (hThread == 0) {
		puts("_beginthreadex() error");
		return -1;
	}
	Sleep(3000);
	puts("end of main");
	return 0;
}

unsigned WINAPI ThreadFunc(void *arg) {
	int i;
	int cnt = *((int*)arg);
	for (i = 0; i < cnt; i++) {
		Sleep(1000); puts("running thread");
	}
	return 0;
}
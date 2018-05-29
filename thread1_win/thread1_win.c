// thread1_win.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h> /* _beginthreadex, _endthreadex - 내가 중간에 쓰레드를 죽이고 싶을 때 사용*/
#include <process.h>
unsigned WINAPI ThreadFunc(void *arg);
//WINAPI는 윈도우에서 제공하는 호출 규약
//내부적으로 호출할 때 이런 식으로 해라

int main()
{
	HANDLE hThread;//윈도우에서는 핸들로 리소스를 관리
	unsigned threadID;
	int param = 5;

	//핸드 값을 hThread에 저장해서 관리함
	hThread = (HANDLE)_beginthreadex(NULL, 0, ThreadFunc, (void*)&param, 0, &threadID);
	//                                        쓰레드 메인
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
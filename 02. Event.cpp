// Event.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <windows.h>

int event_num = 1; /*定义全局变量*/
HANDLE hEvent = NULL;/*定义事件句柄*/

DWORD WINAPI ThreadFunction(LPVOID param)
{
    int* id = (int*)param;
    long result = 0;
    while (event_num < 100)
    {
        WaitForSingleObject(hEvent, INFINITE);
        printf("thread %d use semaphore_num: %d\n", *id, event_num);
        ++event_num;
        SetEvent(hEvent);
        Sleep(1000);
    }

    return NULL;
}

int main()
{
    HANDLE hThread[5] = { NULL };
    int thread[5] = { 0 };
    DWORD thread_id[5] = { 0 };
    hEvent = CreateEvent(NULL, FALSE, TRUE, L"event");
    for (int i = 0; i < 5; ++i)
    {
        thread[i] = i + 1;
        hThread[i] = CreateThread(NULL, 0, ThreadFunction, &thread[i], 0, &thread_id[i]);
    }
    Sleep(1000);
    for (int i = 0; i < 5; ++i)
    {
        WaitForSingleObject(hThread[i], INFINITE);
    }

    CloseHandle(hEvent);
    return 0;
}
// Semaphore.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <windows.h>

int semaphore_num = 1; /*定义全局变量*/
HANDLE hSemaphore = NULL;/*定义信号量句柄*/

DWORD WINAPI ThreadFunction(LPVOID param)
{
    int* id = (int*)param;
    long result = 0;
    while (semaphore_num < 100)
    {
        WaitForSingleObject(hSemaphore, INFINITE);
        printf("thread %d use semaphore_num: %d\n", *id, semaphore_num);
        ++semaphore_num;
        ReleaseSemaphore(hSemaphore, 1, &result);
        Sleep(1000);
    }

    return NULL;
}

int main()
{
    HANDLE hThread[5] = { NULL };
    int thread[5] = { 0 };
    DWORD thread_id[5] = { 0 };
    hSemaphore = CreateSemaphore(NULL, 1, 100, L"sema");
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

    CloseHandle(hSemaphore);
    return 0;
}

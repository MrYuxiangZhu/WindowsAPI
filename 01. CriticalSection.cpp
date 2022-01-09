// CriticalSection.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <windows.h>


int counter_value = 0;/*counter value*/
int max_counter = 5;/*counter value max*/
int min_counter = 0;/*counter value min*/
int producer_num = 0;/*生产者进入临界区次数*/
int consumer_num = 0;/*消费者进入临界区次数*/

CRITICAL_SECTION critical_section;/*临界区*/

DWORD WINAPI producer(LPVOID param)
{
    int* id = (int*)param;
    while (true)
    {
        Sleep(rand() % 1000);
        srand(counter_value);
        EnterCriticalSection(&critical_section);
        int add_count = rand() % 6 + 1;
        /*判断是否超过最大值*/
        if (counter_value < max_counter)
        {
            counter_value += add_count;
            if (counter_value > max_counter)
            {
                add_count -= counter_value - max_counter;
                counter_value = max_counter;
            }
            printf("Producer%d : produced %d items\n", *id, add_count);
        }
        else
        {
            printf("Producer%d : counter value is full, cancel producing...\n", *id);
        }

        printf("items num is %d\n", counter_value);
        LeaveCriticalSection(&critical_section);
        //生产者进入临界区，次数增加
        producer_num++;
    }
}

DWORD WINAPI consumer(LPVOID param)
{
    int* id = (int*)param;

    while (true) 
    {
        //sleep for a random period of time
        Sleep(rand() % 1000);
        EnterCriticalSection(&critical_section);
        // generate 
        srand(counter_value);
        int decrease_count = rand() % 6 + 1;
        //判断是否超过最小值
        if (counter_value > min_counter) 
        {
            counter_value -= decrease_count;
            if (counter_value <= min_counter)
            {
                decrease_count -= min_counter - counter_value;
                counter_value = min_counter;
            }
            printf("Consumer%d : consumed %d items\n", *id, decrease_count);
        }
        else
        {
            printf("Consumer%d : counter value is less than mixinum, cancel consuming...\n", *id);
        }

        printf("items num is %d\n", counter_value);
        LeaveCriticalSection(&critical_section);
        //消费者进入临界区，次数增加
        consumer_num++;
    }
    return 0;
}
int main()
{
    srand(counter_value);
    int thread_producer = 5;
    int thread_consumer = 5;
    int pvalue[5] = { 0 };/*生产者*/
    int cvalue[5] = { 0 };/*消费者*/
    DWORD thread_pid[5], thread_cid[5];
    HANDLE hthread_p[5], hthread_c[5];/*生产者和消费者线程*/
    InitializeCriticalSection(&critical_section);
    //FILE* fp;
    //freopen_s(&fp, "CriticalSection_output.txt", "w", stdout);
    /*create producer thread*/
    for (int i = 0; i < thread_producer; ++i)
    {
        pvalue[i] = i + 1;
        hthread_p[i] = CreateThread(NULL, 0, producer, &pvalue[i], 0, &thread_pid[i]);
    }

    /*create consumer thread*/
    for (int i = 0; i < thread_consumer; ++i)
    {
        cvalue[i] = i + 1;
        hthread_c[i] = CreateThread(NULL, 0, consumer, &cvalue[i], 0, &thread_cid[i]);
    }

    Sleep(1000);
    for (int i = 0; i < thread_producer; ++i)
    {
        WaitForSingleObject(hthread_p[i], INFINITE);
    }

    for (int i = 0; i < thread_producer; ++i)
    {
        WaitForSingleObject(hthread_c[i], INFINITE);
    }
    DeleteCriticalSection(&critical_section);
    //fclose(stdout);
    return 0;
}

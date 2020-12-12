#include <windows.h>
#include <iostream>
#include <iomanip>
#include <cmath>
#pragma comment(lib, "winmm.lib")
using namespace std;


DWORD WINAPI MyThreadFunction(LPVOID lpParam);


unsigned long N = 100000000;
long double step = 1.0 / N;
unsigned long ch = 10 * 830805;
long double sum =0.0;
unsigned long iterplus = 0;
int cntThreads;
HANDLE* THREADS;
DWORD* IDTHREAD;
HANDLE MUTEX;

int main()
{
    system("chcp 1251");
    cout << "Введите кол-во потоков: ";
    cin >> cntThreads;
    iterplus = cntThreads * ch;
    MUTEX = CreateMutex(NULL, FALSE, NULL);
    THREADS = new HANDLE[cntThreads];
    IDTHREAD = new DWORD[cntThreads];

    for (int b = 0; b < cntThreads; b++)
    {
        THREADS[b] = CreateThread(NULL, 0, MyThreadFunction, (LPVOID)(b), CREATE_SUSPENDED, IDTHREAD + b);
    }
     
    DWORD t1 = timeGetTime();
    for (int b = 0; b < cntThreads; b++)
    {
        ResumeThread(THREADS[b]);
    }
    WaitForMultipleObjects(cntThreads, THREADS,TRUE, INFINITE);
    sum = sum * step;
    DWORD t2 = timeGetTime();
    for (int b = 0; b < cntThreads; ++b)
    {
        CloseHandle(THREADS[b]);
    }
    cout << "Время: " << t2 - t1 << endl;
    cout << "Результат: " << fixed << setprecision(10) <<sum<<endl;


    delete[] THREADS;
    THREADS = nullptr;
   
    return 0;
}


DWORD WINAPI MyThreadFunction(LPVOID lpParam)
{
  
    long double pi=0,x;
    unsigned long iter = (unsigned long)lpParam;
    unsigned long  stop=0;
    iter = iter * ch;
    while (iter < N)
    {
        
        iter + ch > N ? stop=N : stop=iter+ch;
        for (unsigned long j = iter; j < stop; j++)
        {
            x = (j + 0.5) * step;
            pi = pi + (4.0 / (1.0 + x*x));
        }
        iter = iterplus;
        WaitForSingleObject(MUTEX, INFINITE);
        {
           iterplus = iterplus + ch;
           sum =sum + pi;
        }
        ReleaseMutex(MUTEX);
        pi = 0;
    } 
    return TRUE;

}
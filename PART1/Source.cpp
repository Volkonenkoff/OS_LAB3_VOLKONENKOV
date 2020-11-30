#include <windows.h>
#include <iostream>
#include <iomanip>
#include <cmath>
#pragma comment(lib, "winmm.lib")
using namespace std;


DWORD WINAPI MyThreadFunction(LPVOID lpParam);


const long N = 100000000;
const long BLOCK_SIZE = 10*830805;
DWORD tlsIndex;
int cntThreads;
CRITICAL_SECTION CriticalSection;

typedef struct NumberOfPi {
    int i, number, j;
    double pi;
}Pi, * LPi;

int main()
{
    system("chcp 1251");
    double res=0;
    
    cout << "Введите кол-во потоков: ";
    cin >> cntThreads;

    HANDLE* THREADS = new HANDLE[cntThreads];
    DWORD* IDTHREAD = new DWORD[cntThreads];
    LPi* PiData = new LPi[cntThreads];
    tlsIndex = TlsAlloc();
    InitializeCriticalSection(&CriticalSection);
    for (int i = 0; i < cntThreads; ++i)
    {
        PiData[i] = (LPi)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(Pi));
        PiData[i]->i = i * BLOCK_SIZE;
        PiData[i]->number = i;
        PiData[i]->j = cntThreads;

        THREADS[i] = CreateThread(NULL, 0, MyThreadFunction, PiData[i], CREATE_SUSPENDED, IDTHREAD + i);

    }
     
    DWORD t1 = timeGetTime();
    for (int i = 0; i < cntThreads; ++i)
        ResumeThread(THREADS[i]);
    WaitForMultipleObjects(cntThreads, THREADS, TRUE, INFINITE);
    DWORD t2 = timeGetTime();
    for (int i = 0; i < cntThreads; ++i)
    {
        res = res + PiData[i]->pi;
        CloseHandle(THREADS[i]);
        HeapFree(GetProcessHeap(), 0, PiData[i]);
    }
    cout << "Время: " << t2 - t1 << endl;

    cout << "Результат: " << fixed << setprecision(10) <<res <<endl;

    DeleteCriticalSection(&CriticalSection);
   
        
    delete[] THREADS;
    THREADS = nullptr;
    TlsFree(tlsIndex);
    return 0;
}


DWORD WINAPI MyThreadFunction(LPVOID lpParam)
{
    LPi PiData;
    PiData = (LPi)lpParam;
   
  
    double pi = 0,x;

    TlsSetValue(tlsIndex, (LPVOID)&pi);
    
    while(PiData->i<N)
    {

        for (long j = PiData->i; j < PiData->i + BLOCK_SIZE && j < N; ++j)
        {
            x = (j + 0.5) * (1.0 / N);
            pi += ((4.0 / (1.0 + x * x)) * (1.0 / N));
        }
        TlsSetValue(tlsIndex, (LPVOID)&pi);
        EnterCriticalSection(&CriticalSection);
        {
            PiData->pi = pi;
            PiData->i = PiData->j * BLOCK_SIZE  + PiData->i;
           
        }
        LeaveCriticalSection(&CriticalSection);
    }
    return 0;

}
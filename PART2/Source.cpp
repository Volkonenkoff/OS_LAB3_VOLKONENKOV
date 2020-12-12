#include <iostream>
#include <iomanip>
#include <cmath>
#include <omp.h>
#include <Windows.h>

#pragma comment(lib, "winmm.lib")
using namespace std;

int main()
{
    system("chcp 1251");
    unsigned long N =  100000000;
    long double step = 1.0 / N;
    unsigned long ch = 10 * 830805;
    long double sum = 0.0;
    long double x;

#pragma omp parallel
    {
#pragma omp master
        {
            int cntThreads;
            cout << "Введите кол-во потоков: ";
            cin >> cntThreads;
            omp_set_num_threads(cntThreads);
            cout << "Количество потоков " << cntThreads << std::endl;
        }
    }
    
    
    DWORD t1 = timeGetTime();
#pragma omp parallel for schedule(dynamic,ch) private(x) reduction (+:sum)
    {
        for (unsigned long i = 0; i < N; i++)
        {
            x = (i + .5) * step;
            sum = sum + 4.0 / (1. + x * x);
        }
    }
    sum = sum * step;
    DWORD t2 = timeGetTime();
    cout << "Время: " << t2 - t1 << endl;
    cout << "Результат: "<<fixed<<setprecision(10) << sum <<  endl;
 
   
    return 0;
}

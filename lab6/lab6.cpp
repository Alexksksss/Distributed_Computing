//6.	Реализовать перемножение матриц.
// Сравнить время при различных значениях schedule и числе потоков(for)
#include <omp.h>
#include <iostream>
#include <chrono>
using namespace std;


int main()
{

    int n = 1000;
    int** a = new int* [n];
    int** b = new int* [n];
    int** result = new int* [n];
    for (int i = 0; i < n; i++) {
        a[i] = new int[n];
        b[i] = new int[n];
        result[i] = new int[n];
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            a[i][j] = 1;
            b[i][j] = 2;
        }
    }
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            result[i][j] = 0;

    auto start_time = chrono::high_resolution_clock::now();


#pragma omp parallel for num_threads(20)  schedule (dynamic)
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) {
            
            for (int k = 0; k < n; k++) {
                result[i][j] += a[i][k] * b[k][j];
                //cout << omp_get_num_threads();
            }
        }
 
    auto end_time = chrono::high_resolution_clock::now();


    auto duration = chrono::duration_cast<chrono::milliseconds>(end_time - start_time);

    cout << "Time taken: " << duration.count() << " milliseconds" << endl;



    for (int i = 0; i < n; i++) {
        delete a[i];
        delete b[i];
        delete result[i];
    }
    delete[]a;
    delete[]b;
    delete[]result;
    return 0;
}
//Вызов функции без omp - 9095
//static - 2709
//dynamic - 2662
//guided - 2626
//static 5 - 2758
//dynamic 5 -2633
//guided 5 - 2544
//static 50 - 2681
//dynamic 50 - 2802
//guided 50 - 2910


//20 потоков
//static - 2595
//dynamic - 2595
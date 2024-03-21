#include <iostream>
#include <thread>
#include <time.h>

using namespace std;

void arrayRandomizer(float* arr, int start, int end) {
    unsigned seed = hash<thread::id>{}(this_thread::get_id());//шаблон класса hash, создает хеш-функцию
    srand(seed);
    for (int i = start; i < end; i++) {
        arr[i] = rand() % 100 / 10.0;
    }
    cout << "Thread id:" << this_thread::get_id() << endl;

}

int main()
{


    int n, k, start = 0;
    cout << "Enter n:";
    cin >> n;
    cout << "ENter k:";
    cin >> k;

    float* arr = new float[n];

    thread* threads = new thread[k];

    int bin_size = n / k;

    for (int i = 0; i < k - 1; i++) {
        int start = i * bin_size;
        threads[i] = thread(arrayRandomizer, arr, start, start + bin_size);
    }

    threads[k - 1] = thread(arrayRandomizer, arr, (k - 1) * bin_size, n);

    for (int i = 0; i < k; i++)
        threads[i].join();

    for (int i = 0; i < n; i++)
        cout << arr[i] << endl;

    delete[] arr;
    delete[]threads;
    

    return 0;
}


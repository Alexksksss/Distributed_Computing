#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

using namespace std;

mutex mtx;
condition_variable cv;
int numberArrived = 0;

void barrier(int id, int k) {
    unique_lock <mutex> lock(mtx);
    cout << "Thread number " << id << " is working before barrier" << endl;

    numberArrived++;
    if (numberArrived < k) {
        cv.wait(lock);
        cout << "Thread number " << id << " is waiting barrier" << endl;
    }
    else {
        numberArrived = 0;
        cv.notify_all();
    }
}

int main()
{
    int k;
    cout << "Enter k:";
    cin >> k;

    thread* threads = new thread[k];

    for (int i = 0; i < k; i++)
        threads[i] = thread(barrier, i, k);

    for (int i = 0; i < k; i++)
        threads[i].join();

    cout << "Threads are working after barrier" << endl;

    delete[]threads;
    return 0;
}
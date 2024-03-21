#include <iostream>
#include <thread>
#include <queue>
#include <mutex>
#include <shared_mutex>

using namespace std;

class Q {
private:
    queue<int> q;
    shared_mutex mtxFront;
    shared_mutex mtxBack;
    shared_mutex mtxBig;


public:
    Q() {}
    ~Q() {}
    void add(int item) {
        lock_guard<shared_mutex> lock(mtxBack);
        unique_lock<shared_mutex> lockBig(mtxBig, defer_lock);
        if (q.size() == 1) 
            lockBig.lock();
        q.push(item);
    }
    void del() {
        lock_guard<shared_mutex> lock(mtxFront);
        unique_lock<shared_mutex> lockBig(mtxBig, defer_lock);
        if (q.size() == 1) 
            lockBig.lock();
        if (!q.empty())
            q.pop();
        else
            cout << "Queue is empty!" << endl;
    }
    int findMax() {
        mtxFront.lock_shared();
        mtxBack.lock_shared();

        if (!q.empty()) {
            queue<int> tempQueue = q;
            int maxElement = tempQueue.front();
            tempQueue.pop();
            while (!tempQueue.empty()) {
                maxElement = max(maxElement, tempQueue.front());
                tempQueue.pop();
            }
            mtxFront.unlock_shared();
            mtxBack.unlock_shared();
            return maxElement;
        }
        else {
            cout << "Queue is empty!" << endl;
            mtxFront.unlock_shared();
            mtxBack.unlock_shared();
            return INT_MAX;
        }
    }
    int findMin() {
        mtxFront.lock_shared();
        mtxBack.lock_shared();

        if (!q.empty()) {
            queue<int> tempQueue = q;
            int minElement = tempQueue.front();
            tempQueue.pop();
            while (!tempQueue.empty()) {
                minElement = min(minElement, tempQueue.front());
                tempQueue.pop();
            }
            mtxFront.unlock_shared();
            mtxBack.unlock_shared();
            return minElement;
        }
        else {
            cout << "Queue is empty!" << endl;
            mtxFront.unlock_shared();
            mtxBack.unlock_shared();
            return INT_MIN;
        }
    }
    void display() {
        mtxFront.lock_shared();
        mtxBack.lock_shared();

        if (!q.empty()) {
            cout << "Queue contents: ";
            queue<int> tempQueue = q;
            while (!tempQueue.empty()) {
                cout << tempQueue.front() << " ";
                tempQueue.pop();
            }
            cout << endl;
        }
        else {
            cout << "Queue is empty!" << endl;
        }
        mtxFront.unlock_shared();
        mtxBack.unlock_shared();
    }


};
shared_mutex mtxPrint;
unique_lock<shared_mutex> lockPrint(mtxPrint, defer_lock);
int main()
{
    Q myQueue;



    myQueue.add(1);
    //myQueue.add(2);
    //myQueue.add(3);

    cout << "Starting with:";

    lockPrint.lock();
    thread t0(&Q::display, &myQueue);
    lockPrint.unlock();
    thread t2(&Q::add, &myQueue, 4);
    thread t1(&Q::del, &myQueue);
    

    lockPrint.lock();
    thread t3(&Q::display, &myQueue);
    lockPrint.unlock();
    thread t4([&myQueue]() {
        int result = myQueue.findMax();
        lockPrint.lock();
        cout << "Max element: " << result << endl;
        lockPrint.unlock();
        });
    //thread t4(&Q::findMax, &myQueue);
    thread t5([&myQueue]() {
        int result = myQueue.findMin();
        lockPrint.lock();
        cout << "Min element: " << result << endl;
        lockPrint.unlock();
        });
    t0.join();
    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();

    return 0;
}

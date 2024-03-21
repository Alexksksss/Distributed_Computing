#include <iostream>
#include <thread>

using namespace std;

void counter(int n) {
    for (int i = n; i >= 0; i--) {
        cout << i << endl;
        this_thread::sleep_for(chrono::seconds(1));
    }
}

int main()
{
    int n;
    cout << "Enter n: ";
    cin >> n;
    thread t(counter, n);
    t.join();
    return 0;
}

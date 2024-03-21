
#include <thread>
#include <mutex>
#include <omp.h>
#include <vector>
#include <iostream>

using namespace std;
//5.	Реализовать скалярное произведение векторов (Reduction)
int main() {
    int size = 1000;

    vector<int> vector1(size, 1);
    vector<int> vector2(size, 2);

    int result = 0;

    #pragma omp parallel for reduction(+:result)
        for (int i = 0; i < size; i++) {
            result += vector1[i] * vector2[i];
            cout << omp_get_num_threads();
        }

    cout << "Scalar product: " << result << endl;

    return 0;
}
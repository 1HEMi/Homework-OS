#include <iostream>  
#include <thread>  
#include <vector>  

using namespace std;  

void incrementCounter(int& total, int iterations) {  
    for (int i = 0; i < iterations; ++i) {  
        total += 1;  
    }  
}  

void runNonSynchronizedTest() {  
    int total = 0, iterations = 10000, threadCount = 6;  

    vector<thread> threadPool;  
    threadPool.reserve(threadCount);  
    for (size_t i = 0; i < threadCount; ++i) {  
        threadPool.emplace_back(incrementCounter, ref(total), iterations);  
    }  
    for (size_t i = 0; i < threadCount; ++i) {  
        threadPool[i].join();  
    }  
    cout << total << endl;  
}  

int main() {  
    runNonSynchronizedTest();  
    return 0;  
}  

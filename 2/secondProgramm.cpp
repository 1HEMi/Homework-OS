#include <iostream>  
#include <thread>  
#include <mutex>  

using namespace std;  

mutex mtx;  

void incrementCounter(int& total, int iterations) {  
    for (int i = 0; i < iterations; ++i) {  
        lock_guard<mutex> lock(mtx);  
        total += 2;  
    }  
}  

void runNonSynchronizedTest() {  
    int total = 0, iterations = 10000;  
    thread thread1(incrementCounter, ref(total), iterations);  
    thread thread2(incrementCounter, ref(total), iterations);  
    thread thread3(incrementCounter, ref(total), iterations);  
    thread thread4(incrementCounter, ref(total), iterations);  
    thread thread5(incrementCounter, ref(total), iterations);  
    thread thread6(incrementCounter, ref(total), iterations);  
    
    thread1.join();  
    thread2.join();  
    thread3.join();  
    thread4.join();  
    thread5.join();  
    thread6.join();  
    
    cout << total << endl;  
}  

int main() {  
    runNonSynchronizedTest();  
    return 0;  
}

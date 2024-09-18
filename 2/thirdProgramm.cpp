#include <iostream>  
#include <thread>  
#include <atomic>  

using namespace std;  

void incrementAtomicSum(atomic<int>& total, int iterations) {  
    for (int i = 0; i < iterations; ++i) {  
        total += 2;  
    }  
}  

void runAtomicTest() {  
    atomic<int> total = 0;  
    int iterations = 10000;  
    thread thread1(incrementAtomicSum, ref(total), iterations);  
    thread thread2(incrementAtomicSum, ref(total), iterations);  
    thread thread3(incrementAtomicSum, ref(total), iterations);  
    thread thread4(incrementAtomicSum, ref(total), iterations);  
    thread thread5(incrementAtomicSum, ref(total), iterations);  
    thread thread6(incrementAtomicSum, ref(total), iterations);  
    
    thread1.join();  
    thread2.join();  
    thread3.join();  
    thread4.join();  
    thread5.join();  
    thread6.join();  
    
    cout << total << endl;  
}  

int main() {  
    runAtomicTest();  
    return 0;  
}  

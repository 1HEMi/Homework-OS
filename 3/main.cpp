#include <iostream>  
#include <fstream>  
#include <thread>  
#include <semaphore.h>  
#include <fcntl.h>  
#include <chrono>  

using namespace std;  

bool hasBeenRead = false;  
sem_t* namedSemaphore;  

void writerThreadFunction() {  
    string message;  
    ofstream outputFile;  
    while (true) {  
        outputFile.open("text.txt", ios::app);  
        if (outputFile.is_open()) {  
            getline(cin, message);  
            outputFile << message << '\n';  
            outputFile.close();  
            this_thread::sleep_for(chrono::milliseconds(100));  
            hasBeenRead = true;  
            sem_post(namedSemaphore);  
            sem_post(namedSemaphore);  
        }  
    }  
}  

void readerThreadFunction() {  
    string line;  
    ifstream inputFile;  
    inputFile.open("text.txt");  
    while (true) {  
        sem_wait(namedSemaphore);  
        if (inputFile.is_open()) {  
            getline(inputFile, line);  
            if (!hasBeenRead) {  
                cout << "Также я: " + line << endl;  
            }  
            this_thread::sleep_for(chrono::milliseconds(100));  
            hasBeenRead = false;  
        }  
    }  
}  

int main() {  
    const char* semaphoreName = "/my_named_semaphore";  
    namedSemaphore = sem_open(semaphoreName, O_CREAT, 0644, 0);  

    thread writer(writerThreadFunction);  
    thread reader(readerThreadFunction);  

    writer.join();  
    reader.join();  

    sem_destroy(namedSemaphore);  
    return 0;  
}

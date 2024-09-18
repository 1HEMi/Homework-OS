#include <iostream>  
#include <sys/mman.h>  
#include <fstream>  
#include <thread>  
#include <semaphore.h>  
#include <fcntl.h>  
#include <chrono>  
#include <cstring>  
#include <sys/stat.h>  
#include <algorithm>  
#include <unistd.h>  

using namespace std;  

const int BUFFER_SIZE = 1024;  

bool hasBeenRead = false;  
sem_t* namedSemaphore;  

void writerThread(void* sharedMemory) {  
    string message;  
    while (true) {  
        getline(cin, message);  
        memset(sharedMemory, 0, BUFFER_SIZE); // Очистка буфера перед записью  
        memcpy(static_cast<char *>(sharedMemory), message.c_str(), message.size());  
        this_thread::sleep_for(chrono::milliseconds(100));  
        hasBeenRead = true;  
        sem_post(namedSemaphore);  
        sem_post(namedSemaphore);  
    }  
}  

void readerThread(void* sharedMemory) {  
    while (true) {  
        sem_wait(namedSemaphore);  
        char* text = static_cast<char*>(sharedMemory);  
        if (!hasBeenRead) {  
            cout << "Также я: " + string{text} << endl;  
        }  
        this_thread::sleep_for(chrono::milliseconds(100));  
        hasBeenRead = false;  
    }  
}  

int main() {  
    const char *semaphoreName = "/my_named_semaphore";  
    namedSemaphore = sem_open(semaphoreName, O_CREAT, 0644, 0);  

    int shmFd = shm_open("shared_memory", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);  
    ftruncate(shmFd, BUFFER_SIZE); // Задаем размер разделяемой памяти  

    // Получаем указатель на начало отображенного участка памяти размером BUFFER_SIZE  
    void *sharedMemoryAddress = mmap(NULL, BUFFER_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shmFd, 0);  

    thread writer(writerThread, sharedMemoryAddress);  
    thread reader(readerThread, sharedMemoryAddress);  

    writer.join();  
    reader.join();  

    sem_destroy(namedSemaphore);  
    return 0;  
}

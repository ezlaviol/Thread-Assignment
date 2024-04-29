#include <iostream>
#include <thread>
#include <semaphore.h>
using namespace std;

sem_t sem;

void runMeFirst() {
     cout << "Run me first" << endl;
}

void runMeSecond() {
     cout << "I run second" << endl;
}

void thread_func(int id) {
    cout << "Thread " << id << " is waiting..." << endl;
    sem_wait(&sem); // blocking call
    cout << "Thread " << id << " has acquired the semaphore";
    cout << endl;
    if (id == 0) runMeFirst();
    if (id == 1) runMeSecond();
    this_thread::sleep_for(chrono::seconds(2));
    cout << "Thread " << id << " is releasing the semaphore";
    cout << endl;
    sem_post(&sem);
}

int main() {
    const int num_threads = 2;
    // initialize the semaphore with the number of threads
    sem_init(&sem, 0, num_threads); // 0: thread, 1: process
    thread threads[num_threads];
    for (int i = 0; i < num_threads; ++i) {
        threads[i] = thread(thread_func, i);
    }
    for (int i = 0; i < num_threads; ++i) {
        threads[i].join();
    }
    sem_destroy(&sem); // destroy the semaphore
    return 0;
}

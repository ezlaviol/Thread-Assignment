#include <iostream>
#include <thread>
using namespace std;
bool stop = true;
void thread1() { cout << "I am thread 1\n"; stop=false; }
void thread2() { while (stop); cout << "I am thread2\n"; }

int main() {
    thread t1, t2;
    t1 = thread(thread1);
    t2 = thread(thread2);
    t1.join();
    t2.join();
    return 0;
}

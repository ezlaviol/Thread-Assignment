#include <iostream>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <semaphore.h>
#include <cstdlib>
using namespace std;

mutex mtx;
condition_variable cv;
sem_t sem;

int counter = 0;
string webpages[10] = {"Google.com", "Bing.com", "Yahoo.com", 
                    "CoolMathGames.com", "Twitter.com", "Facebook.com", 
                    "Instagram.com", "GoDuckGo.com",
                    "YouTube.com", "Twitch.com"};
                    
struct requestStructure {
       int request_id;
       string ip_address;
       string page_requested;
};

queue<requestStructure> msg_queue;

void listen(int requests) {
     cout << "Reciever thread started" << endl;
     requestStructure myRequest;
     
     for (int i = 0; i < requests; i++) {
         this_thread::sleep_for(chrono::seconds((rand() % (2)) + 1));
         
         counter++;
         myRequest.request_id = counter;
         myRequest.ip_address = "";
         myRequest.page_requested = rand() % sizeof(webpages);
         
         lock_guard<mutex>lock(mtx);
         msg_queue.push(myRequest);
         cout << "Received request " << myRequest.request_id << endl;
         cv.notify_one();
     }
}

void do_request() {
     cout << "Searcher thread started" << endl;
     while (true) {
           unique_lock<mutex>lock(mtx);
           cv.wait(lock, []{return !msg_queue.empty();});
           requestStructure searched = msg_queue.front();
           msg_queue.pop();
           cout << "Thread " << this_thread::get_id() << " completed request" << endl;
           cout << searched.request_id << " requesting webpage " << searched.page_requested << endl;
           lock.unlock();
     }
}

int main() {
    const int num_threads = 3;
    thread requestReciever(listen, num_threads);
    // initialize the semaphore with the number of threads
    sem_init(&sem /*thread*/, 0/*process*/, num_threads);
    
    thread pageSearchers[counter];
    for (int i = 0; i < num_threads; ++i) {
        pageSearchers[i] = thread(do_request);
    }
    for (int i = 0; i < num_threads; ++i) {
        pageSearchers[i].join();
    }
    sem_destroy(&sem); // destroy the semaphore
    return 0;
}

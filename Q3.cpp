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
const int webpagesLength = 10;
string webpages[webpagesLength] = {"Google.com", "Bing.com", "Yahoo.com", 
                    "CoolMathGames.com", "Twitter.com", "Facebook.com", 
                    "Instagram.com", "GoDuckGo.com",
                    "YouTube.com", "Twitch.com"};
                    
struct requestStructure {
       int request_id;
       string ip_address;
       string page_requested;
};
bool listenExit = false;
queue<requestStructure> msg_queue;

void listen(int requests) {
     
     requestStructure myRequest;
     
     for (int i = 0; i < requests; i++) {
         this_thread::sleep_for(chrono::seconds((rand() % (2)) + 1));
         
         counter++;
         myRequest.request_id = counter;
         myRequest.ip_address = "";
         myRequest.page_requested = webpages[rand() % webpagesLength];
         
         unique_lock<mutex> lock(mtx);
         msg_queue.push(myRequest);
         cout << "Received request " << myRequest.request_id << endl;
         lock.unlock();
         
         cv.notify_one();
     }
     listenExit = true;
     cv.notify_all();
}

void do_request(int id) {
     while (true) {
           unique_lock<mutex>lock(mtx);
           while (msg_queue.empty()) {
           		
           		if (listenExit && msg_queue.empty()) {
           			cout << "Thread " << id << " Exits\n";
           			return;
           		}
				cv.wait(lock);  
		   }
           
           requestStructure searched = msg_queue.front();
           msg_queue.pop();
           cout << "Thread " << id << " completed request" << endl;
           cout << searched.request_id << " requesting webpage " << searched.page_requested << endl;
           lock.unlock();
     }
}

int main() {
	
    const int num_requests = 3;
    thread requestReciever(listen, num_requests);
    // initialize the semaphore with the number of threads
    //sem_init(&sem /*thread*/, 0/*process*/, num_threads);
    
    int num_threads = 3;
	thread pageSearchers[num_threads];
    for (int i = 0; i < num_threads; ++i) {
        pageSearchers[i] = thread(do_request, i);
    }
    
    requestReciever.join();
    for (int i = 0; i < num_threads; ++i) {
        pageSearchers[i].join();
    }
    //sem_destroy(&sem); // destroy the semaphore
    return 0;
}

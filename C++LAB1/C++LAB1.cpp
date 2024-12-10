#include <iostream>
#include <mutex>
#include <thread>
#include <chrono>
#include <condition_variable>

std::mutex mtx;
std::condition_variable condition_variable;
bool ready = false;

void provide(int index) {
    std::unique_lock<std::mutex> lock(mtx);
    if (ready) return;

    ready = true;
    condition_variable.notify_one();
    std::cout << "notify " << index << "\n";
    std::this_thread::sleep_for(std::chrono::seconds(2));
}

void consume() {
    std::unique_lock<std::mutex> lock(mtx);
    condition_variable.wait(lock, [] { return ready; });
    ready = false;
}

int main() {
    for (int i = 0; i < 5; ++i) {
        std::thread t1(provide, i);
        std::thread t2(consume);
        t1.join();
        t2.join();
        std::cout << "end of iter " << i << "\n\n";
    }
    return 0;
}
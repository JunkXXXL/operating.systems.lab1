#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <list>
#include <chrono>
#include <ctime>

using namespace std;

mutex m;
condition_variable cv;

list<int> x;

bool ready2 = false;

void Provide(int num)
{
    unique_lock<mutex> ul(m);
    if (ready2)
    {
        ul.unlock();
        return;
    }
    ready2 = true;
    cout << num << endl;
    x.push_back(num);
    this_thread::sleep_for(chrono::seconds(5));
    cout << "provided " << this_thread::get_id() << endl;
    cv.notify_one();
    ul.unlock();
}

void Consume()
{
    unique_lock<mutex> ul(m);
    cv.wait(ul, [] { return ready2; });
    ready2 = false;
    cout << x.back() << endl;
    this_thread::sleep_for(chrono::seconds(2));
    cout << "consumed " << this_thread::get_id() << endl << endl;
    ul.unlock();
}

int main2()
{
    srand(time(NULL));
    for (int i = 0; i < 5; i++)
    {
        thread provider(Provide, rand() & 100);
        thread consumer(Consume);
        provider.join();
        consumer.join();
    }
    return 0;
}
#include <iostream>
#include <thread>
#include <mutex>

int a = 0;
std::mutex lock;

void func()
{
    for (int i = 0; i < 1000; ++i)
    {
        lock.lock();
        a++;
        lock.unlock();
    }
}

int main()
{
    std::thread thread1(func);
    std::thread thread2(func);

    thread1.join();
    thread2.join();

    std::cout << a << std::endl;
}
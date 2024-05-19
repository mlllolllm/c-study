#include <iostream>
#include <thread>
#include <mutex>
#include <atomic>
#include <chrono>

std::atomic<int> a = 0;

void func()
{
    for (int i = 0; i < 1000000; ++i)
    {
        a++;
    }
}

int main()
{
    // auto time_start = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    
    std::thread thread1(func);
    std::thread thread2(func);

    

    thread1.join();
    thread2.join();

    std::cout << a << std::endl;
}
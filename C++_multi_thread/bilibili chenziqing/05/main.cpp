#include <iostream>
#include <thread>
#include <mutex>

int shared_data = 0;
int a = 0;

std::mutex mtx;
void func()
{
    
    for (int i = 0; i < 1000000; ++i)
    {
        std::lock_guard<std::mutex> mutex_guard(mtx);
        a++;
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
#include <iostream>
#include <queue>
#include <mutex>
#include <thread>
#include <string>
#include <chrono>
#include <condition_variable>

std::queue<int> g_queue;
std::condition_variable g_cv;
std::mutex g_mtx;
void Producer()
{
    for (int i = 0; i < 100; ++i)
    {
        std::unique_lock<std::mutex> lock(g_mtx);
        g_queue.push(i);
        // 消费者来取任务
        g_cv.notify_one();
        std::cout << "Produce:" << i << std::endl;
    }
    std::this_thread::sleep_for(std::chrono::microseconds(100));
}

void Consumer()
{
    while (1)
    {
        std::unique_lock<std::mutex> lock(g_mtx);
        // bool is_empty=g_queue.empty();
        g_cv.wait(lock, []()
                  { return !g_queue.empty(); });
        int value = g_queue.front();
        g_queue.pop();

        std::cout << "Consume:" << value << std::endl;
    }
}

int main()
{
    std::thread t1(Producer);
    std::thread t2(Consumer);
    t1.join();
    t2.join();

    // std::thread t3(Producer);
    // t3.join();
}
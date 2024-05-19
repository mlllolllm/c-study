// 先检讨，对于c++11及之后的新特性还有很多不理解，对STL使用方法过于依赖gpt
// 自我评价为：有较强的分析问题与分解问题的能力，但是基础仍然薄弱，需要在使用方法上下功夫

#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <queue>
#include <chrono>
#include <functional>
#include <condition_variable>
#include <atomic>

class Threadpool
{
public:
    static Threadpool &getThreadpool(int threadNums = 4)
    {
        std::call_once(m_onceFlag, &Threadpool::init, threadNums);
        return *m_threadpool;
    }

    template <typename F, typename... Args>
    // 函数模板中右值引用是万能引用
    void enqueue(F &&f, Args &&...args)
    {
        // bind,forward完美转发
        std::function<void()> task(std::bind(std::forward<F>(f), std::forward<Args>(args)...));
        {
            std::unique_lock<std::mutex> lock(m_mtx);
            m_tasks.emplace(std::move(task));
        }

        m_cv.notify_one();
    }

private:
    Threadpool(const Threadpool &) = delete;
    Threadpool &operator=(const Threadpool &) = delete;

    Threadpool(int threadNum) : m_stop(false)
    {
        for (int i = 0; i < threadNum; ++i)
        {
            m_threads.emplace_back([this]
                                   {
                                  while (1)
                                  {
                                    std::unique_lock<std::mutex> lock(m_mtx);
                                    m_cv.wait(lock, [this]
                                                { return m_stop || !m_tasks.empty(); });
                                    if (m_stop && m_tasks.empty())
                                          return;

                                    std::function<void()> task(std::move(m_tasks.front()));
                                    m_tasks.pop();
                                    lock.unlock();
                                    task();
                                    
                                  } });
        }
    }

public:
    ~Threadpool()
    {
        {
            std::unique_lock<std::mutex> lock(m_mtx);
            m_stop = true;
        }

        m_cv.notify_all();
        for (auto &t : m_threads)
        {
            if (t.joinable())
                t.join();
        }
    }

private:
    static std::unique_ptr<Threadpool> m_threadpool;
    static std::once_flag m_onceFlag;

    static void init(int threadNums)
    {
        m_threadpool.reset(new Threadpool(threadNums));
    }

    std::vector<std::thread> m_threads;
    std::queue<std::function<void()>> m_tasks;
    std::mutex m_mtx;
    std::condition_variable m_cv;
    bool m_stop;
};

std::unique_ptr<Threadpool> Threadpool::m_threadpool;
std::once_flag Threadpool::m_onceFlag;

std::atomic<int> a = 0;

void plusA()
{
    for (int i = 0; i < 100000; ++i)
    {
        ++a;
    }
}

int main()
{
    // Threadpool pool(4);

    for (int i = 0; i < 10; ++i)
    {
        Threadpool::getThreadpool().enqueue(plusA);
    }
    std::this_thread::sleep_for(std::chrono::seconds(3));
    std::cout << a << std::endl;
}

// #include <iostream>
// #include <thread>
// #include <mutex>
// #include <condition_variable>
// #include <functional>
// #include <queue>

// class ThreadPool
// {
// public:
//     ThreadPool(int numThreads) : stop(false)
//     {
//         for (int i = 0; i < numThreads; ++i)
//         {
//             threads.emplace_back([this]
//                                  {
//                 while (true) {
//                     std::unique_lock<std::mutex> lock(mutex);
//                     condition.wait(lock, [this] { return stop || !tasks.empty(); });
//                     if (stop && tasks.empty()) {
//                         return;
//                     }
//                     std::function<void()> task(std::move(tasks.front()));
//                     tasks.pop();
//                     lock.unlock();
//                     task();
//                 } });
//         }
//     }

//     ~ThreadPool()
//     {
//         {
//             std::unique_lock<std::mutex> lock(mutex);
//             stop = true;
//         }
//         condition.notify_all();
//         for (std::thread &thread : threads)
//         {
//             thread.join();
//         }
//     }

//     template <typename F, typename... Args>
//     void enqueue(F &&f, Args &&...args)
//     {
//         std::function<void()> task(std::bind(std::forward<F>(f), std::forward<Args>(args)...));
//         {
//             std::unique_lock<std::mutex> lock(mutex);
//             tasks.emplace(std::move(task));
//         }
//         condition.notify_one();
//     }

// private:
//     std::vector<std::thread> threads;
//     std::queue<std::function<void()>> tasks;
//     std::mutex mutex;
//     std::condition_variable condition;
//     bool stop;
// };

// int main()
// {
//     ThreadPool pool(4);
//     for (int i = 0; i < 8; ++i)
//     {
//         pool.enqueue([i]
//                      {
//             std::cout << "Task " << i << " is running in thread " << std::this_thread::get_id() << std::endl;
//             std::this_thread::sleep_for(std::chrono::seconds(1));
//             std::cout << "Task " << i << " is done" << std::endl; });
//     }
//     return 0;
// }
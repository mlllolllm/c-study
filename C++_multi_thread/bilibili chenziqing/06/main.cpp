#include <iostream>
// #include <thread>
#include <mutex>

class Singleton
{
public:
    static Singleton &getInstance()
    {
        std::call_once(m_onceFlag, &Singleton::init);
        return *m_instance;
    }
    void setData(int data)
    {
        m_data = data;
    }
    int getData() const
    {
        return m_data;
    }

private:
    Singleton() {}
    Singleton(const Singleton &) = delete;
    Singleton &operator=(const Singleton &) = delete;
    static void init()
    {
        m_instance.reset(new Singleton);
    }
    static std::unique_ptr<Singleton> m_instance;
    static std::once_flag m_onceFlag;
    int m_data = 0;
};
std::unique_ptr<Singleton> Singleton::m_instance;
std::once_flag Singleton::m_onceFlag;
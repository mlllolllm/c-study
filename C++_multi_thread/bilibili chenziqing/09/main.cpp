#include <iostream>
#include <future>
using namespace std;

void func(std::promise<int> &f)
{
    f.set_value(1000);
}

int main()
{
    std::promise<int> f;
    auto future_result = f.get_future();
    std::thread t1(func, ref(f));
    t1.join();
    cout << future_result.get() << endl;
}
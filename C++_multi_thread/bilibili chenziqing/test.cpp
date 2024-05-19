#include <iostream>
#include <functional>

void otherFunction(int &x);
void otherFunction(int &&x);
template <typename T>
void process(T &&arg)
{
    otherFunction(arg);
}

void otherFunction(int &x)
{
    std::cout << "Lvalue reference: " << x << std::endl;
}

void otherFunction(int &&x)
{
    std::cout << "Rvalue reference: " << x << std::endl;
}

int return_1()
{
    std::cout << "return 1" << std::endl;
    return 1;
}

int outputNum(int x, int y)
{
    std::cout << x << std::endl;
    std::cout << y << std::endl;
    return 1;
}

int main()
{
    int a = 42;

    process(a);   // 传递左值
    process(100); // 传递右值

    std::function<int(int)> task = std::bind(outputNum, 100, std::placeholders::_1);
    int res = task(200);
    // std::cout<<res<<std::endl;

    return 0;
}
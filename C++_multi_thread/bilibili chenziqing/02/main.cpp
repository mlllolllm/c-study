// #include <iostream>
// #include <thread>
// #include <string>

// std::thread t;

// void foo(int &x){
//     x+=1;
// }

// void test(){
//     int a=1;
//     t=std::thread(foo,std::ref(a));
//     t.join();
//     std::cout<<a<<std::endl;
// }

// int main(){
//     // int a=1;
//     // //这里查了很久没查清楚
//     // //反正传引用记得用ref
//     // std::thread t(foo,std::ref(a));
//     // t.join();

//     //std::cout<<a<<std::endl;

//     test();
//     //t.join();
//     return 0;
// }

#include <iostream>
#include <thread>
#include <memory>

class MyClass {
public:
    void func(int a,int b) {
        std::cout << "Thread " << std::this_thread::get_id() 
        << " started" << std::endl;
        // do some work
        std::cout << "Thread " << std::this_thread::get_id() 
        << " finished" << std::endl;
        std::cout<<a<<std::endl;
        std::cout<<b<<std::endl;
    }
};

int main() {
    std::shared_ptr<MyClass> obj = std::make_shared<MyClass>();
    std::thread t(&MyClass::func, obj,1,2);
    t.join();
    return 0;
}
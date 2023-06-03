//
// Created by admin on 2021/11/4.
//

#include <iostream>
#include <memory>

using namespace std;

unique_ptr<string> demo(const char * s)
{
    unique_ptr<string> temp (new string (s));
    return temp;
}

int main() {
    {
        unique_ptr<std::string> p1 (new string ("Hello"));
//        std::make_unique<string>("Hello");
        //传递所有权
        // 错误
//        unique_ptr<string> p2 = p1;
        // 正确
        unique_ptr<string> p2 = std::move(p1);
        if(p1 == nullptr)
            std::cout<<"p1 is empty"<<std::endl;
        //所有权从 p1 传递给了 p3
        if(p2 != nullptr)
            std::cout<<"p2 is not empty"<<std::endl;
    }
    {
        unique_ptr<string> p3;
        p3 = demo("Hello");
        if(p3 != nullptr)
            std::cout<<"p3 is not empty"<<std::endl;
    }

    return 0;
}
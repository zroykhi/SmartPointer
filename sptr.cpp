//
// Created by admin on 2021/10/24.
//

#include <iostream>
#include <memory>
#include <string>

struct Sample
{
    Sample() {
        std::cout << "Sample\n";
    }
    ~Sample() {
        std::cout << "~Sample\n";
    }
};

void deleter(Sample * x)
{
    std::cout << "Custom Deleter\n";
    delete[] x;
}

class Deleter
{
public:
    void operator() (Sample * x) {
        std::cout<<"DELETER FUNCTION CALLED\n";
        delete[] x;
    }
};

struct Son;
struct Father {
    std::shared_ptr<Son> son_;
};
struct Son {
    std::shared_ptr<Father> father_;
//    std::weak_ptr<Father> father_;
};

int main()
{
    {
        // 使用默认的 deleter，未能正确析构，将报错
//        std::shared_ptr<Sample> p1(new Sample[3]);
    }

    {
        // 使用自定义函数作为 deleter
        std::shared_ptr<Sample> p1(new Sample[3], deleter);
    }

    {
        // Lambda表达式作为删除器
        std::shared_ptr<Sample> p1(new Sample[3], [](Sample * x){
            std::cout<<"DELETER FUNCTION CALLED\n";
            delete[] x;
        });
    }

    {
        // 函数对象作为删除器
        std::shared_ptr<Sample> p1(new Sample[3], Deleter());
    }

    {
        int *num = new int(23);
        std::shared_ptr<int> p1(num);

//        std::shared_ptr<int> p1 = std::make_shared<int>(23);
        std::weak_ptr<int> wp1 = p1;

        std::shared_ptr<int> p2(p1);  // 正确使用方法
//        std::shared_ptr<int> p3(num); // 不推荐

        std::cout << "p1 Reference = " << p1.use_count() << std::endl; // 输出 2
        std::cout << "p2 Reference = " << p2.use_count() << std::endl; // 输出 2
//        std::cout << "p3 Reference = " << p3.use_count() << std::endl; // 输出 1
        // 假如使用原始指针num创建了p1，又同样方法创建了p3，当p1超出作用域时会调用delete释放num内存，此时num成了悬空指针，当p3超出作用域再次delete的时候就可能会出错。

        p1.reset();
        std::cout << "p1 Reference = " << p1.use_count() << std::endl; // 输出 0（放弃管理之前的对象，当前管理空指针）
        std::cout << "p2 Reference = " << p2.use_count() << std::endl;
        p1.reset(new int(45));
        std::cout << "p1 Reference = " << p1.use_count() << std::endl; // 输出 1
    }


    // 循环引用
    {
        auto father = std::make_shared<Father>();
        auto son = std::make_shared<Son>();
        father->son_ = son;
        son->father_ = father;
    }

    return 0;
}
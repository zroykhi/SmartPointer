//
// Created by admin on 2021/10/24.
//
#include <iostream>
#include <memory>

using namespace std;

//strong reference
class B;
class A
{
public:
    shared_ptr<class B> m_spB;
};


class B
{
public:
    shared_ptr<class A> m_spA;
};

//weak reference
class WeakB;
class WeakA
{
public:
    weak_ptr<class WeakB> m_wpB;
};


class WeakB
{
public:
    weak_ptr<class WeakA> m_wpA;
};


void test_loop_ref()
{
    weak_ptr<class A> wp1;

    {
        auto pA = make_shared<class A>();
        auto pB = make_shared<class B>();

        pA->m_spB = pB;
        pB->m_spA = pA;

        wp1 = pA;
    }//内存泄漏

    cout << "wp1 reference number: " << wp1.use_count() << "\n";

    weak_ptr<class WeakA> wp2;
    {
        auto pA = make_shared<class WeakA>();
        auto pB = make_shared<class WeakB>();

        pA->m_wpB = pB;
        pB->m_wpA = pA;

        wp2 = pA;
    }//无内存泄漏

    cout << "wp2 reference number: " << wp2.use_count() << "\n";
}

void test_valid(weak_ptr<int> &wp)
{
    if(shared_ptr<int> smptr2 = wp.lock())
    {
        cout << "the shared_ptr is valid\n";
    } else {
        cout << "the shared_ptr is not valid\n";
    }

    //检查被引用的对象是否已删除 false 仍存在  true 释放
    if(!wp.expired()) {
        //it is getting valid shared_ptr obj now;
        shared_ptr<int> smptr1 = wp.lock();
        cout << "   get obj value: " << *smptr1 << endl;
    }
}

int main() {
    auto p5 = std::make_shared<int>(123);
    //default consstructor
    std::weak_ptr<int> wp1;

    //copy constructor
    std::weak_ptr<int> wp2(p5);

    //assign constructor
    std::weak_ptr<int> wp3 = wp2;
    test_valid(wp2);

    //释放被管理对象的所有权, 调用后 *this 不管理对象
    wp2.reset();

    test_valid(wp2);

    //std::weak_ptr 用来避免 std::shared_ptr 的循环引用
    test_loop_ref();
    return 0;
}


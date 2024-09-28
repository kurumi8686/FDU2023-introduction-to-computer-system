#pragma once
#include <ucontext.h>
#include <cassert>
#include <iostream>

namespace coro {
    class coroutine;
    using func_t = void (*)(void*);

    coroutine* create(func_t func, void* args);
    void release(coroutine* co);
    int resume(coroutine* co, int param = 0);
    int yield(int ret = 0);

    const size_t STACK_SIZE = 1024 * 1024;  //协程栈的大小
    enum CoroStatus { CORO_READY = 1, CORO_RUNNING, CORO_END }; //记录协程当前的状态

    struct coroutine {
        bool started = false;  //是否已经开始执行
        bool end = false;  //是否已经结束执行

        func_t coro_func = nullptr;  //协程函数
        void* args = nullptr;  //函数的参数

        int paraData = 0;

        ucontext_t ctx = { 0 };
        char coroStack[STACK_SIZE];
        int coroStatus;

        coroutine(func_t func, void* args) : coro_func(func), args(args) {}
        ~coroutine() {}
    };

    class coroutine_env {  
    private:
        coroutine* coroList[2]; 
        size_t coroNum = 0; 

    public:
        ucontext_t main_coro;  //用于记录main函数的上下文
        int paraData = 0;  //resume的第二个参数 //yield的返回值

        coroutine_env() {}

        coroutine* get_coro(int idx) {
            // TODO: implement your code here
            assert(idx >= 0 && idx <= 1);
            return coroList[idx];
        }

        void push(coroutine* co) {
            // std::cout << "push: " << coroNum << std::endl;
            assert(coroNum < 2);
            coroList[coroNum] = co;
            coroNum++;
        }

        coroutine* pop() {
            // std::cout << "start pop: " << coroNum-1 << std::endl;
            assert(coroNum >= 1);
            coroutine* co = coroList[coroNum - 1];
            coroNum--;
            return co;
            // TODO: implement your code here
        }
    };

}  // namespace coro
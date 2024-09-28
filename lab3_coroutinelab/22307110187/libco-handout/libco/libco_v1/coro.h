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

    const size_t STACK_SIZE = 1024 * 1024;  //Э��ջ�Ĵ�С
    enum CoroStatus { CORO_READY = 1, CORO_RUNNING, CORO_END }; //��¼Э�̵�ǰ��״̬

    struct coroutine {
        bool started = false;  //�Ƿ��Ѿ���ʼִ��
        bool end = false;  //�Ƿ��Ѿ�����ִ��

        func_t coro_func = nullptr;  //Э�̺���
        void* args = nullptr;  //�����Ĳ���

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
        ucontext_t main_coro;  //���ڼ�¼main������������
        int paraData = 0;  //resume�ĵڶ������� //yield�ķ���ֵ

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
#pragma once
#include <cassert>
#include <cstdlib>
#include "coro_ctx.h"

namespace coro {

    struct coroutine;
    struct coroutine_attr;

    coroutine* create(func_t coro_func, void* arg, const coroutine_attr* attr = nullptr);
    void release(coroutine* co);
    int resume(coroutine* co, int param = 0);
    int yield(int ret = 0);

    struct stack_mem {
        int stack_size = 0;           // 栈的大小
        char* stack_buffer = nullptr; // 栈的缓冲区
        char* stack_bp = nullptr;     // 栈的基指针
        coroutine* owner = nullptr;   // 拥有该栈的协程

        stack_mem(size_t size) : stack_size(size) {
            stack_buffer = new char[size];
            stack_bp = stack_buffer + size;
        }
        ~stack_mem() {
            delete[] stack_buffer;
        }
    };

    struct share_stack {
        unsigned long alloc_idx = 0; //下次使用的栈的索引
        int count = 0; //总共有多少个栈可以使用
        int stack_size = 0; //每个栈的大小
        stack_mem** stack_array = nullptr; //栈数组（每个元素是个stack_mem类型的栈）
        share_stack(int count, size_t stack_size)
            : count(count), stack_size(stack_size) {
            stack_array = new stack_mem * [count];
            for (int i = 0; i < count; i++)
            {
                stack_array[i] = new stack_mem(stack_size);
            }
        }
        ~share_stack() {
            for (int i = 0; i < count; i++) delete stack_array[i];
            delete[] stack_array;
        }
        stack_mem* get_stackmem() {
            if (alloc_idx >= count) alloc_idx = 0;
            return stack_array[alloc_idx++];
        }
    };
    struct coroutine_attr {
        int stack_size = 128 * 1024;
        share_stack* sstack = nullptr;
    };

    struct coroutine {
        bool started = false;
        bool end = false;

        func_t coro_func = nullptr;
        void* arg = nullptr;

        int data;
        context ctx = { 0 };

        char* stack_sp = nullptr;
        int save_size = 0; //只将其使用到的栈空间进行保存
        char* save_buffer = nullptr;

        stack_mem* coro_stack_mem = nullptr;
        int stack_size = 0;

        coroutine(func_t func, void* arg)
            : coro_func(func), arg(arg) {
        }
        ~coroutine() {}
    };

    class coroutine_env {
    public:
        coroutine* call_stack[128];
        int call_stack_size = 0;
        coroutine* pending_co = nullptr;
        coroutine* occupy_co = nullptr;
        coroutine_env() {
            // init main coroutine
            coroutine* main_coro = create(nullptr, nullptr, nullptr);
            push(main_coro);
        }
        coroutine* get_coro(int idx) {
            return call_stack[idx];
        }

        void pop() {
            call_stack_size--;
        }
        void push(coroutine* co) {
            call_stack[call_stack_size++] = co;
        }
    };
}  // namespace coro

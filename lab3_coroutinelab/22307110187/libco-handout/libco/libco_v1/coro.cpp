#include "coro.h"

namespace coro {

    static coroutine_env g_coro_env;

    coroutine* create(func_t func, void* args) {
        coroutine* new_coro = new coroutine(func, args);
        new_coro->ctx.uc_stack.ss_sp = new_coro->coroStack;
        new_coro->ctx.uc_stack.ss_size = STACK_SIZE;
        //new_coro->ctx.uc_link = &g_coro_env.main_coro;
        new_coro->coroStatus = CORO_READY;
        return new_coro;
    }

    void release(coroutine* co) {
        if (co) delete co;
        return;
    }

    static void func_wrap(coroutine* co) {
        if (co->coro_func) {
            co->coro_func(co->args);
        }
        co->end = true;
        co->coroStatus = CORO_END;
        yield(-1);
    }

    int resume(coroutine* co, int param) {
        g_coro_env.push(co);  //要恢复开始运行，压入list
        co->paraData = param;  //yield时返回给调用者的参数

        switch (co->coroStatus) {
        case CORO_READY:
            getcontext(&co->ctx);
            co->ctx.uc_link = &g_coro_env.main_coro;  //ctx关联主协程
            co->coroStatus = CORO_RUNNING;
            makecontext(&co->ctx, (void (*)(void)) func_wrap, 1, co);  //初始化
            swapcontext(&g_coro_env.main_coro, &co->ctx);  //保存当前上下文到main中，然后激活ctx上下文
            break;
        case CORO_RUNNING:
            swapcontext(&g_coro_env.main_coro, &co->ctx);
            break;
        }

        return co->paraData;
    }

    int yield(int ret) {
        coroutine* co = g_coro_env.pop();
        co->paraData = ret;
        if (ret != -1) {
            swapcontext(&co->ctx, &g_coro_env.main_coro);  //保存当前上下文在ctx中，激活主协程的运行
            return co->paraData;
        }
        else return -1; 
       
    }

}  // namespace coro
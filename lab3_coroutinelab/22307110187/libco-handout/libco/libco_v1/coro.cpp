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
        g_coro_env.push(co);  //Ҫ�ָ���ʼ���У�ѹ��list
        co->paraData = param;  //yieldʱ���ظ������ߵĲ���

        switch (co->coroStatus) {
        case CORO_READY:
            getcontext(&co->ctx);
            co->ctx.uc_link = &g_coro_env.main_coro;  //ctx������Э��
            co->coroStatus = CORO_RUNNING;
            makecontext(&co->ctx, (void (*)(void)) func_wrap, 1, co);  //��ʼ��
            swapcontext(&g_coro_env.main_coro, &co->ctx);  //���浱ǰ�����ĵ�main�У�Ȼ�󼤻�ctx������
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
            swapcontext(&co->ctx, &g_coro_env.main_coro);  //���浱ǰ��������ctx�У�������Э�̵�����
            return co->paraData;
        }
        else return -1; 
       
    }

}  // namespace coro
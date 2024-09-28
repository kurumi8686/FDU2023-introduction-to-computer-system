#include "coroutine.h"
#include <cstring>

namespace coro {

    static coroutine_env g_coro_env;

    extern "C" {
        extern void coro_ctx_swap(context*, context*) asm("coro_ctx_swap");
    };

    coroutine* create(func_t coro_func, void* arg, const coroutine_attr* attr) {
        coroutine_attr at;
        coroutine* co = new coroutine(coro_func, arg);

        if (attr) memcpy(&at, attr, sizeof(at));
        if (at.stack_size <= 0) at.stack_size = 8 * 1024;
        else if (at.stack_size > 8 * 1024 * 1024) at.stack_size = 8 * 1024 * 1024;
        at.stack_size &= ~0xFFF;
        at.stack_size += 0x1000;//2^12  4k
 
        stack_mem* cur_stack_mem = nullptr;

        if (at.sstack) cur_stack_mem = at.sstack->get_stackmem();
        else cur_stack_mem = new coro::stack_mem(at.stack_size);

        co->coro_stack_mem = cur_stack_mem;
        co->stack_size = co->coro_stack_mem->stack_size;
        co->ctx.ss_size = co->stack_size;
        co->ctx.ss_sp = co->coro_stack_mem->stack_buffer;

        return co;
    }

    void release(coroutine* co) {
        if (co) delete co;
    }

    static void func_wrap(coroutine* co) {
        if (co->coro_func) {
            co->coro_func(co->arg);
        }
        co->end = true;
        yield(-1);
    }

    void save_stack(coroutine* co) {
        stack_mem* stack_mem = co->coro_stack_mem;
        int len = stack_mem->stack_bp - co->stack_sp;  //
        if (co->save_buffer)
        {
            delete[] co->save_buffer;
            co->save_buffer = nullptr;
        }
        co->save_buffer = new char[len];
        co->save_size = len;
        memcpy(co->save_buffer, co->stack_sp, len);
    }

    void swap(coroutine* curr, coroutine* pending) {
        char c;
        curr->stack_sp = &c;
        g_coro_env.pending_co = pending;
        //get last occupy co on the same stack mem
        coroutine* occupy_co = pending->coro_stack_mem->owner;
        //set pending co to occupy thest stack mem;
        pending->coro_stack_mem->owner = pending;
        g_coro_env.occupy_co = occupy_co;
        if (occupy_co && occupy_co != pending) save_stack(occupy_co);

        coro_ctx_swap(&(curr->ctx), &(pending->ctx));

        coroutine* update_occupy_co = g_coro_env.occupy_co;
        coroutine* update_pending_co = g_coro_env.pending_co;

        if (update_occupy_co && update_pending_co && update_occupy_co != update_pending_co)
        {
            if (update_pending_co->save_buffer && update_pending_co->save_size)
            {
                memcpy(update_pending_co->stack_sp, update_pending_co->save_buffer, update_pending_co->save_size);
            }
        }
    }

    int resume(coroutine* co, int param) {
        co->data = param;
        coroutine* curr = g_coro_env.get_coro(g_coro_env.call_stack_size - 1);
        if (!co->started)
        {
            ctx_make(&co->ctx, (func_t)func_wrap, co);
            co->started = 1;
        }
        g_coro_env.push(co);
        swap(curr, co);
        return co->data;
    }

    int yield(int ret) {
        coroutine* curr = g_coro_env.get_coro(g_coro_env.call_stack_size - 1);
        coroutine* last = g_coro_env.get_coro(g_coro_env.call_stack_size - 2);

        g_coro_env.pop();

        curr->data = ret;
        swap(curr, last);
        return curr->data;
    }
}  // namespace coro

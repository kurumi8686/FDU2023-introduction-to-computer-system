#include "coro_ctx.h"
#include <cstdint>
#include <cstring>

namespace coro {

void ctx_make(context* ctx, func_t coro_func, const void* arg) {
        char* sp = (char*)ctx->ss_sp + ctx->ss_size - sizeof(void*);
        sp = (char*)((unsigned long)sp & -16L);

        memset(ctx->regs, 0, sizeof(ctx->regs)); //initialize

        void** ret_addr = (void**)sp;
        *ret_addr = (void*)coro_func;

        ctx->regs[13] = sp;
        ctx->regs[9] = (void*)coro_func;
        ctx->regs[7] = (void*)arg;
    }

}  // namespace coro


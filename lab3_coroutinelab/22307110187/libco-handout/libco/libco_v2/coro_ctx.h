#pragma once
#include <stdlib.h>

namespace coro {

    using func_t = void (*)(void*);

    struct context {
        // 64 bit
        // low 
        //    | regs[0]: r15  |
        //    | regs[1]: r14  |
        //    | regs[2]: r13  |
        //    | regs[3]: r12  |
        //    | regs[4]: r9   |
        //    | regs[5]: r8   |
        //    | regs[6]: rbp  |
        //    | regs[7]: rdi  |
        //    | regs[8]: rsi  |  // not used
        //    | regs[9]: ret  |  // ret func address
        //    | regs[10]: rdx |
        //    | regs[11]: rcx |
        //    | regs[12]: rbx |
        //    | regs[13]: rsp |
        // high
        void* regs[15];

        size_t ss_size;
        void* ss_sp;
    };

    void ctx_make(context* ctx, func_t coro_func, const void* arg);

}  // namespace coro

#include <cassert>
#include <iostream>

#define CO_BEGIN  case 0:

#define CO_END  return 0;

#define CO_YIELD(...)  line = __LINE__; return __VA_ARGS__; case __LINE__:

#define CO_RETURN(...)  return __VA_ARGS__;

struct coroutine_base {
    int line = 0;   //记录上一次推出时的位置
};

class fib : public coroutine_base {
private:
    int a = 0;
    int b = 1;

public:
    int operator()()
    {
        switch (line) {
            CO_BEGIN
                while (true)
                {
                    CO_YIELD(a)
                    int temp = a;
                    a = b;
                    b = temp + b;  //fibonacci
                }
            CO_RETURN(-1)   //其实不需要，到不了这里
        }
        CO_END
    }
};

int main() {
    int ans[] = { 0, 1, 1, 2, 3, 5, 8, 13, 21, 34 };
    fib foo;
    for (int i = 0; i < 10; i++)
        assert(foo() == ans[i]);
    std::cout << "libco_v3 test passed!" << std::endl;
}

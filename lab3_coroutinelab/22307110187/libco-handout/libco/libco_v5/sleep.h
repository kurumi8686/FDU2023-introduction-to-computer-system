#include <coroutine>
#include <functional>
#include <queue>
#include <thread>

namespace coro {

    static std::queue<std::function<bool()>> task_queue;

    struct sleep {
        sleep(int n_ms) : delay{ n_ms } {}
        std::chrono::milliseconds delay;

        constexpr bool await_ready() const noexcept { return false; }
        void await_suspend(std::coroutine_handle<> h) const noexcept {
            task_queue.pop();
            std::this_thread::sleep_for(delay);
            task_queue.push([h]() -> bool {
                h.resume();
                return h.done();
                });
        }
        void await_resume() const noexcept {}
    };

    struct Task {
        struct promise_type {
            Task get_return_object() {
                return Task{ std::coroutine_handle<promise_type>::from_promise(*this) };
            }
            std::suspend_always initial_suspend() { return {}; }
            std::suspend_always final_suspend() noexcept { return {}; }
            void return_void() {}
            void unhandled_exception() { std::terminate(); }
            std::suspend_always yield_value(std::function<bool()> func) {
                task_queue.pop();
                if(func) task_queue.push(func);
                return {};
            }
        };
        std::coroutine_handle<promise_type> coro;

        Task(std::coroutine_handle<promise_type> h) : coro(h) {
            task_queue.push([this]() -> bool {
                coro.resume();
                return coro.done();
                });
        }
        ~Task() {
            if (coro) coro.destroy();
        }
    };

    void wait_task_queue_empty() {
        while (!task_queue.empty())
        {
            auto task = task_queue.front();
            if (task()) task_queue.pop();
        }
    }

}  // namespace coro


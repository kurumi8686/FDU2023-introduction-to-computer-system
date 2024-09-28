#pragma once
#include <coroutine>
#include <iterator>
#include <utility>

namespace coro {
    template <typename Ref, typename Value = std::remove_cvref_t<Ref>>
    class generator {
    public:
        generator() noexcept = default;
        generator(const generator&) = delete;
        generator(generator&& completion) noexcept
            : coro_(std::exchange(completion.coro_, {})) {}

        ~generator() noexcept {
            if (coro_) coro_.destroy();
        }

        struct promise_type {
            using Handle = std::coroutine_handle<promise_type>; //协程句柄
            promise_type* node = nullptr;
            promise_type* pre = nullptr;
            Value value;

            promise_type() : node(this) {}  //构造函数

            std::suspend_always initial_suspend() const noexcept
            {
                return {};
            }
            std::suspend_always final_suspend() const noexcept
            {
                return {};
            }
            generator get_return_object()
            {
                return generator{ Handle::from_promise(*this) };
            }
            void unhandled_exception() { std::terminate(); }
            void return_void() {}
            
            std::suspend_always yield_value(const Value& val) noexcept {
                node->value = val;
                return {};
            }
            
            struct end_awaiter {
                constexpr bool await_ready() noexcept { return false; }
                Handle await_suspend(Handle h) noexcept {
                    auto prev = h.promise().pre;
                    if (prev) return Handle::from_promise(*prev);
                    else return std::noop_coroutine();
                }
                void await_resume() noexcept {}
            };

            struct yield_awaiter {
                generator gen;
                constexpr bool await_ready() noexcept { return false; }
                Handle await_suspend(Handle h) noexcept {
                    auto& cur = h.promise();
                    auto& nested = gen.coro_.promise();
                    
                    nested.node = cur.node;
                    nested.pre = cur.pre;
                    cur.node->node = std::addressof(nested);
                    
                    return gen.coro_;
                }
                void await_resume() noexcept {}
            };

            yield_awaiter yield_value(generator&& g) noexcept {
                return yield_awaiter{ std::move(g) };
            }

            void resume() {
                Handle::from_promise(*node).resume();
            }
        };

        struct sentinel {};

        class iterator {
        public:
            using iterator_category = std::input_iterator_tag;
            using difference_type = std::ptrdiff_t;
            using value_type = Value;
            using reference = Ref;
            using pointer = std::add_pointer_t<Ref>;

            using Handle = std::coroutine_handle<promise_type>; //协程句柄

            iterator() noexcept = default;
            iterator(const iterator&) = delete;
            iterator(iterator&& o) {
                std::swap(coro_, o.coro_);
            }
            iterator& operator=(iterator&& o) {
                std::swap(coro_, o.coro_);
                return *this;
            }

            ~iterator() noexcept {}

            friend bool operator==(const iterator& o, sentinel) 
                noexcept {
                return !o.coro_ || o.coro_.done();
            }
            friend bool operator!=(const iterator& o, sentinel)
                noexcept {
                return o.coro_ && !o.coro_.done();
            }
            iterator& operator++() {
                coro_.promise().resume();
                return *this;
            }
            iterator operator++(int) {
                coro_.promise().resume();
                return *this;
            }
            reference operator*() const noexcept {
                return coro_.promise().value;
            }
            pointer operator->() const noexcept {
                return std::addressof(operator*());
            }

        private:
            friend generator;
            explicit iterator(Handle coro) noexcept : coro_(coro) {}
            Handle coro_ = nullptr;
        };

        iterator begin() noexcept {
            if (coro_) coro_.resume();
            return iterator{ coro_ };
        }
        sentinel end() noexcept {
            return {};
        }

    private:
        explicit generator(std::coroutine_handle<promise_type> h) noexcept : coro_(h) {}
        std::coroutine_handle<promise_type> coro_ = nullptr;
    };

}  // namespace coro

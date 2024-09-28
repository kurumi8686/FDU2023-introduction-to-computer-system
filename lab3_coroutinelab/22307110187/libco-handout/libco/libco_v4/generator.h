#pragma once
#include <coroutine>
#include <iterator>
#include <utility>

namespace coro {

template <typename Ref, typename Value = std::remove_cvref_t<Ref>>
class generator {
public:
    // TODO: implement promise_type
    struct promise_type{
        Value value;
        // 构造成功后直接挂起
        std::suspend_always initial_suspend() const
        {
            return {};
        }
        // 协程要结束时也直接挂起
        std::suspend_always final_suspend() const noexcept
        {
            return {};
        }
        generator get_return_object()
        {
            return generator{ std::coroutine_handle<promise_type>::from_promise(*this) };
        }
        // 出现未经处理的异常时执行
        void unhandled_exception() { std::terminate(); }
        // co_return时执行
        void return_void() {}
        // 功能好像全被yield干完了，用不到这个
        //void return_value(const Value& val) noexcept {value = val; return;}  
        // co_yield时执行
        std::suspend_always yield_value(const Value& val) noexcept
        {
            value = val;
            return {};
        }
    };

    generator() noexcept = default;
    generator(const generator&) = delete;
    ~generator() noexcept {
        if (coro_)  coro_.destroy();
    }

    struct sentinel {};  //代表结尾

    class iterator {
    public:
        using iterator_category = std::input_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = Value;
        using reference = Ref;
        using pointer = std::add_pointer_t<Ref>;

        iterator() noexcept = default;
        iterator(const iterator&) = delete;

        iterator(iterator&& o) {
            std::swap(coro_, o.coro_);
            o.coro_ = {};
        }

        iterator& operator=(iterator&& o) {
            std::swap(coro_, o.coro_);
            o.coro_ = {};
            return *this;
        }

        ~iterator() noexcept {} 

        // TODO: implement operator== and operator!=
        //sentinel——终止条件   判断o这个迭代器是否终止
        friend bool operator==(const iterator& o, sentinel)
            noexcept {
            return !o.coro_ || o.coro_.done();
        }
        friend bool operator!=(const iterator& o, sentinel) 
            noexcept {
            return o.coro_ && !o.coro_.done();
        }

        // TODO: implement operator++ and operator++(int)
        // 迭代器到下一个，也就是上一次yield后挂起的协程恢复继续运行
        iterator& operator++() {
            coro_.resume();
            return *this;
        }
        //++it和it++的区别，功能上相同
        iterator& operator++(int) { 
            coro_.resume();
            return *this;
        }

        // TODO: implement operator* and operator->
        // 对迭代器解引用（即取出其中的值）
        reference operator*() const noexcept {
            return coro_.promise().value;
        }

        pointer operator->() const noexcept {
            return std::addressof(operator*());
        }


    private:
        friend generator;
        // TODO: implement iterator constructor
        // hint: maybe you need to a promise handle
        explicit iterator(std::coroutine_handle<promise_type> coro) noexcept : coro_(coro) {}
        std::coroutine_handle<promise_type> coro_ = nullptr;
        // TODO: add member variables you need
    };

    // TODO: implement begin() and end() member functions
    iterator begin() noexcept {
        if (coro_) coro_.resume();  //激活
        return iterator{ coro_ };
    }

    sentinel end() noexcept {
        return {};
    }


private:
    // TODO: implement generator constructor
    explicit generator(std::coroutine_handle<promise_type> handle) noexcept : coro_(handle) {}
    std::coroutine_handle<promise_type> coro_ = nullptr;
    // TODO: add member variables you need
};

}  // namespace coro

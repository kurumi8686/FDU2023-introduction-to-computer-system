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
        // ����ɹ���ֱ�ӹ���
        std::suspend_always initial_suspend() const
        {
            return {};
        }
        // Э��Ҫ����ʱҲֱ�ӹ���
        std::suspend_always final_suspend() const noexcept
        {
            return {};
        }
        generator get_return_object()
        {
            return generator{ std::coroutine_handle<promise_type>::from_promise(*this) };
        }
        // ����δ��������쳣ʱִ��
        void unhandled_exception() { std::terminate(); }
        // co_returnʱִ��
        void return_void() {}
        // ���ܺ���ȫ��yield�����ˣ��ò������
        //void return_value(const Value& val) noexcept {value = val; return;}  
        // co_yieldʱִ��
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

    struct sentinel {};  //�����β

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
        //sentinel������ֹ����   �ж�o����������Ƿ���ֹ
        friend bool operator==(const iterator& o, sentinel)
            noexcept {
            return !o.coro_ || o.coro_.done();
        }
        friend bool operator!=(const iterator& o, sentinel) 
            noexcept {
            return o.coro_ && !o.coro_.done();
        }

        // TODO: implement operator++ and operator++(int)
        // ����������һ����Ҳ������һ��yield������Э�ָ̻���������
        iterator& operator++() {
            coro_.resume();
            return *this;
        }
        //++it��it++�����𣬹�������ͬ
        iterator& operator++(int) { 
            coro_.resume();
            return *this;
        }

        // TODO: implement operator* and operator->
        // �Ե����������ã���ȡ�����е�ֵ��
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
        if (coro_) coro_.resume();  //����
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

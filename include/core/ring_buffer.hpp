#ifndef lib_ring_buffer_h
#define lib_ring_buffer_h

// standard headers
#include <atomic>
#include <cstddef>
#include <new>
#include <bit>
#include <array>
#include <expected>
#include <string_view>

namespace core {
    
    template <typename T, std::size_t _capacity>
    requires (std::has_single_bit(_capacity))
    class ring_buffer {

    private:
#if defined (__cpp_lib_hardware_interference_size) && __cpp_lib_hardware_interference_size >= 64
        static constexpr std::size_t _cache_line_ = std::hardware_constructive_interference_size;
#else
        static constexpr std::size_t _cache_line_ = 64;
#endif
        static constexpr std::size_t _mask_ = _capacity - 1;

        struct Node {
            std::atomic<std::size_t> sequence;
            alignas(alignof(T)) std::byte storage[sizeof(T)];
        };
        alignas(_cache_line_) std::atomic<std::size_t> m_head;
        alignas(_cache_line_) std::atomic<std::size_t> m_tail;
        alignas(_cache_line_) std::array<Node, _capacity> m_buffer;
    public:

        ring_buffer() noexcept;
        ~ring_buffer() noexcept;
        ring_buffer(const ring_buffer&) = delete;
        ring_buffer& operator=(ring_buffer&&) = delete;

        std::expected<T, std::string_view> pop() noexcept;
        void push(T&& x) noexcept;
    };

}

#endif

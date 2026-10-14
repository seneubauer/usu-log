#ifndef lib_core_ring_buffer_h
#define lib_core_ring_buffer_h

// internal headers
#include <util/types.hpp>
#include <core/record.hpp>

// standard headers
#include <atomic>
#include <cstddef>
#include <new>
#include <bit>
#include <concepts>
#include <string_view>
#include <utility>
#include <type_traits>
#include <array>

namespace core {
    
    template <std::size_t Capacity>
    requires (std::has_single_bit(Capacity))
    class ring_buffer {
    private:

        static constexpr std::size_t m_mask = Capacity - 1;
        #if defined (__cpp_lib_hardware_interference_size) && __cpp_lib_hardware_interference_size >= 64
        static constexpr std::size_t m_cache_line = std::hardware_constructive_interference_size;
        #else
        static constexpr std::size_t m_cache_line = 64;
        #endif

        struct buffer_node {
            std::atomic<std::size_t> sequence{0};
            core::record entry{};
        };
        alignas(m_cache_line) std::atomic<std::size_t> m_head;
        alignas(m_cache_line) std::atomic<std::size_t> m_tail;
        alignas(m_cache_line) std::array<buffer_node, Capacity> m_buffer;

    public:

        static_assert(std::has_single_bit(Capacity));
        static_assert(std::is_trivially_copyable_v<core::record>);
        static_assert(std::atomic<std::size_t>::is_always_lock_free, "ring_buffer does not guarantee lock-free size_t atomics");

        ring_buffer() noexcept {
            for (std::size_t i = 0; i < Capacity; ++i) {
                m_buffer[i].sequence.store(i, std::memory_order_relaxed);
            }
        }
        ~ring_buffer() { /* remove all instances */ }
        ring_buffer(const ring_buffer&) = delete;
        ring_buffer& operator=(const ring_buffer&) = delete;
        ring_buffer(ring_buffer&&) = delete;
        ring_buffer& operator=(ring_buffer&&) = delete;

        util::types::status_code pop(core::record& rec) {
            
        }

        template <util::types::log_level Level, std::same_as<util::types::key_value_pair>... Args>
        bool push(std::string_view host, std::string_view application, Args... args) {
            std::size_t position = m_head.load(std::memory_order_relaxed);
            while (true) {
                if (head >= Capacity)
                    return util::types::status_code::buffer_limit_reached;

                if (m_head.compare_exchange_weak(head, head + 1, std::memory_order_acquire, std::memory_order_relaxed))
                    break;
            }
            m_buffer[head].state.store(atomic_state::under_construction, std::memory_order_relaxed);
            ::new (static_cast<void*>(m_buffer[head].storage)) core::record(host, application, std::forward<Args>(args)...);
            m_buffer[head].state.store(atomic_state::ready, std::memory_order_release);
            return util::types::status_code::success;
        }

    };

}

#endif

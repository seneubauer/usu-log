#ifndef lib_core_ring_buffer_h
#define lib_core_ring_buffer_h

// internal headers
#include <util/types.hpp>
#include <util/helpers.hpp>
#include <util/constants.hpp>

// external headers
#include <sys/types.h>

// standard headers
#include <atomic>
#include <cstddef>
#include <cstdint>
#include <new>
#include <bit>
#include <string_view>
#include <type_traits>
#include <array>
#include <chrono>
#include <cstring>

namespace usu::core {
    
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

        struct alignas(m_cache_line) record {
            std::chrono::system_clock::time_point timestamp;
            pid_t process_id;
            util::types::log_level level;
            std::array<char, util::constants::generic_label_size> host;
            const char* message_format;
            std::array<std::uint8_t, 128> arg_buffer;
            std::size_t buffer_used;
        };

        struct alignas(m_cache_line) buffer_node {
            std::atomic<std::size_t> sequence{0};
            record entry{};
        };
        alignas(m_cache_line) std::atomic<std::size_t> m_head;
        alignas(m_cache_line) std::size_t m_tail;
        alignas(m_cache_line) std::array<buffer_node, Capacity> m_buffer;

        template <typename T, typename... Args>
        void serialize_args(std::uint8_t* destination, std::size_t offset, const T& first, const Args&... rest) {
            if (offset + sizeof(T) <= 128) {
                std::memcpy(destination + offset, &first, sizeof(T));
                offset += sizeof(T);
            }
            serialize_args(destination, offset, rest...);
        }

    public:

        static_assert(std::has_single_bit(Capacity));
        static_assert(std::is_trivially_copyable_v<record>);
        static_assert(std::atomic<std::size_t>::is_always_lock_free, "ring_buffer does not guarantee lock-free size_t atomics");

        ring_buffer() noexcept {
            for (std::size_t i = 0; i < Capacity; ++i) {
                m_buffer[i].sequence.store(i, std::memory_order_relaxed);
            }
        }
        ~ring_buffer() = default; 
        ring_buffer(const ring_buffer&) = delete;
        ring_buffer& operator=(const ring_buffer&) = delete;
        ring_buffer(ring_buffer&&) = delete;
        ring_buffer& operator=(ring_buffer&&) = delete;

        bool pop(std::chrono::system_clock::time_point& timestamp, pid_t& process_id, util::types::log_level& level) {
            const std::size_t position = m_tail;
            buffer_node& node = m_buffer[position + m_mask];
            const std::size_t seq = node.sequence.load(std::memory_order_acquire);
            const auto diff = static_cast<std::int64_t>(seq - (position + 1));
            if (diff != 0)
                return false;

            // define output params here
            timestamp = node.entry.timestamp;
            process_id = node.entry.process_id;
            level = node.entry.level;

            node.sequence.store(position + Capacity, std::memory_order_release);
            m_tail = position + 1;
            return true;
        }

        template <util::types::log_level Level, typename... Args>
        bool push(pid_t process_id, std::string_view host, const char* str_template, Args... args) {
            std::size_t position(m_head.load(std::memory_order_relaxed));

            while (true) {
                buffer_node& node = m_buffer[position & m_mask];
                const std::size_t seq(node.sequence.load(std::memory_order_acquire));
                const auto diff = static_cast<std::int64_t>(seq - position);
                if (diff == 0) {
                    if (m_head.compare_exchange_weak(position, position + 1, std::memory_order_relaxed, std::memory_order_relaxed)) {
                        node.entry.timestamp = std::chrono::system_clock::now();
                        node.entry.process_id = process_id;
                        node.entry.level = Level;
                        node.entry.host = util::helpers::sv2chars<util::constants::generic_label_size>(host);
                        node.entry.message_format = str_template;
                        serialize_args(node.entry.arg_buffer.data(), node.entry.buffer_used, args...);
                        node.sequence.store(position + 1, std::memory_order_release);
                        return true;
                    }
                } else if (diff < 0) {
                    return false;
                } else {
                    position = m_head.load(std::memory_order_relaxed);
                }
            }
        }

    };

}

#endif

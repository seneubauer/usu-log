#ifndef lib_ring_buffer_h
#define lib_ring_buffer_h

// standard headers
#include <atomic>
#include <cstddef>
#include <new>
#include <type_traits>
#include <bit>

namespace core {
    
    template <typename T, std::size_t _capacity>
    requires (std::__has_single_bit(_capacity))
    class ring_buffer {

    private:
        struct _node_ {
            std::atomic<T> turn{};
            typename std::aligned_storage

        };

    };

}

#endif

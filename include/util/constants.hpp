#ifndef lib_util_constants_h
#define lib_util_constants_h

#include <cstddef>

namespace usu::util::constants {

    static constexpr std::size_t str_buffer_size{256};
    static constexpr std::size_t arg_buffer_size{256};
    static constexpr std::size_t key_size{64};
    static constexpr std::size_t value_size{256};
    static constexpr std::size_t key_value_pair_limit{16};
    static constexpr std::size_t generic_label_size{128};

}

#endif

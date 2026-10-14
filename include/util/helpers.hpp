#ifndef lib_util_helpers_h
#define lib_util_helpers_h

// standard headers
#include <array>
#include <cstddef>
#include <string_view>

namespace util::helpers {

    template <std::size_t N>
    static constexpr std::array<char, N> sv2chars(std::string_view str) {
        std::array<char, N> arr{};
        str.copy(arr.data(), N - 1);
        return arr;
    }

}

#endif

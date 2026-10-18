#ifndef lib_util_types_h
#define lib_util_types_h

// internal headers
#include <util/constants.hpp>
#include <util/helpers.hpp>

// standard headers
#include <cstdint>

namespace usu::util::types {

    enum class status_code : std::uint8_t {
        success,
        bad_alloc,
        invalid_index_reference,
        buffer_limit_reached,
        undefined_object_reference
    };

    enum class log_level : std::uint8_t {
        debug,
        info,
        warn,
        error
    };

}

#endif

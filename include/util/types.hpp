#ifndef lib_util_types_h
#define lib_util_types_h

// internal headers
#include <util/constants.hpp>
#include <util/helpers.hpp>

// standard headers
#include <cstdint>
#include <string_view>
#include <array>

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

    struct key_value_pair {
    private:
        std::array<char, util::constants::key_size> m_key = {'\0'};
        std::array<char, util::constants::value_size> m_value = {'\0'};
        bool m_defined{false};
    public:
        key_value_pair() {}
        key_value_pair(
            std::string_view key,
            std::string_view value
        ) :
            m_key(util::helpers::sv2chars<util::constants::key_size>(key)),
            m_value(util::helpers::sv2chars<util::constants::value_size>(value)),
            m_defined(true) {}
        ~key_value_pair() {}
        void init(std::string_view key, std::string_view value) {
            m_key = util::helpers::sv2chars<util::constants::key_size>(key);
            m_value = util::helpers::sv2chars<util::constants::value_size>(value);
            m_defined = true;
        }
        void forget() {
            m_key.fill('\0');
            m_value.fill('\0');
            m_defined = false;
        }
        std::string_view key() const { return std::string_view(m_key.data()); }
        std::string_view value() const { return std::string_view(m_value.data()); }
        bool defined() const { return m_defined; }
    };

}

#endif

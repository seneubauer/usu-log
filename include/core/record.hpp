#ifndef lib_core_record_h
#define lib_core_record_h

// internal headers
#include <util/constants.hpp>
#include <util/helpers.hpp>
#include <util/types.hpp>

// standard headers
#include <string_view>
#include <chrono>
#include <concepts>
#include <array>
#include <cstddef>
#include <utility>

namespace core {

    class record {
    private:

        std::chrono::system_clock::time_point m_timestamp;
        std::array<char, util::constants::generic_label_size> m_host;
        std::array<char, util::constants::generic_label_size> m_application;
        util::types::log_level m_level;
        std::array<char, util::constants::value_size> m_message = {'\0'};
        std::array<util::types::key_value_pair, util::constants::key_value_pair_limit> m_pairs = {};
        std::size_t m_pair_count;
        util::types::status_code m_status;

    public:

        record() {}

        template <util::types::log_level Level, std::same_as<util::types::key_value_pair>... Args>
        record(
            std::string_view host,
            std::string_view application,
            Args&&... args
        ) :
            m_level(Level),
            m_timestamp(std::chrono::system_clock::now()),
            m_host(util::helpers::sv2chars<util::constants::generic_label_size>(host)),
            m_application(util::helpers::sv2chars<util::constants::generic_label_size>(application)),
            m_message(""),
            m_pairs{ static_cast<util::types::key_value_pair>(std::forward<Args>(args))... },
            m_pair_count(sizeof...(args)) {}

        template<util::types::log_level Level>
        record(
            std::string_view host,
            std::string_view application,
            std::string_view message
        ) :
            m_level(Level),
            m_timestamp(std::chrono::system_clock::now()),
            m_host(util::helpers::sv2chars<util::constants::generic_label_size>(host)),
            m_application(util::helpers::sv2chars<util::constants::generic_label_size>(application)),
            m_message(util::helpers::sv2chars<util::constants::value_size>(message)) {}

        template <util::types::log_level Level, std::same_as<util::types::key_value_pair>... Args>
        void init(std::string_view host, std::string_view application, Args&&... args) {
            m_level = Level;
            m_timestamp = std::chrono::system_clock::now();
            m_host = util::helpers::sv2chars<util::constants::generic_label_size>(host);
            m_application = util::helpers::sv2chars<util::constants::generic_label_size>(application);
            m_message.fill('\0');
            m_pairs = { static_cast<util::types::key_value_pair>(std::forward<Args>(args))... };
            m_pair_count = sizeof...(args);
        }
        
        template <util::types::log_level Level>
        void init(std::string_view host, std::string_view application, std::string_view message) {
            m_level = Level;
            m_timestamp = std::chrono::system_clock::now();
            m_host = util::helpers::sv2chars<util::constants::generic_label_size>(host);
        }

        util::types::key_value_pair* add_kvp(std::string_view key, std::string_view value);
        std::chrono::system_clock::time_point timestamp() const { return m_timestamp; }
        std::string_view host() const { return std::string_view(m_host.data()); }
        std::string_view application() const { return std::string_view(m_application.data()); }
        std::string_view message() const { return std::string_view(m_message.data()); }
        util::types::key_value_pair* kvp(std::size_t index);
        util::types::status_code status() const { return m_status; }

    };

}

#endif

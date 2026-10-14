// internal headers
#include <core/record.hpp>
#include <util/types.hpp>
#include <util/constants.hpp>

// standard headers
#include <string_view>
#include <cstddef>

namespace core {

    util::types::key_value_pair* record::add_kvp(std::string_view key, std::string_view value) {

        if (m_pair_count >= util::constants::key_value_pair_limit) {
            m_status = util::types::status_code::buffer_limit_reached;
            return nullptr;
        }

        m_pairs[m_pair_count].init(key, value);
        ++m_pair_count;

        return &m_pairs[m_pair_count - 1];
    }

    util::types::key_value_pair* record::kvp(std::size_t index) {

        if (index >= m_pairs.size()) {
            m_status = util::types::status_code::invalid_index_reference;
            return nullptr;
        }

        if (!m_pairs[index].defined()) {
            m_status = util::types::status_code::undefined_object_reference;
            return nullptr;
        }

        return &m_pairs[index];
    }

}

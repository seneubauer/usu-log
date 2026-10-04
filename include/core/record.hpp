#ifndef core_record_header_h
#define core_record_header_h

// standard headers
#include <string>
#include <chrono>

namespace core {

    class record {
    private:
        
        std::chrono::system_clock::time_point m_timestamp;
        std::string m_host_name;
        std::string m_application_name;

    };

}

#endif

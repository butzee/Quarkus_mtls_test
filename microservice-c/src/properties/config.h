#pragma once
#include <string>

namespace MicroserviceConfig {
    extern std::string brokers;
    extern std::string topic_str_incoming;
    extern std::string topic_str_outgoing;
    extern int32_t partition;
    extern std::string key_password;
    extern std::string ca_location;
    extern std::string certificate_location;
    extern std::string key_location;

    void configureEnvironment(const std::string& env);
}
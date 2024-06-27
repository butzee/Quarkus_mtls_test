#include "config.h"
#include <string>
#include <rdkafkacpp.h>

namespace MicroserviceConfig {
    std::string brokers = "localhost:9092";
    std::string topic_str_incoming = "topic-seal-data";
    std::string topic_str_outgoing = "topic-encrypted-premium";
    int32_t partition = RdKafka::Topic::PARTITION_UA;
    std::string key_password = "changeit";
    std::string ca_location = "/etc/certs/ca.crt";
    std::string certificate_location = "/etc/certs/microservice-c.pem";
    std::string key_location = "/etc/certs/microservice-c.key";

    void configureEnvironment(const std::string& env) {
        if (env == "kubernetes") {
            brokers = "kafka-broker.default.svc.cluster.local:9092";
            ca_location = "/etc/certs/ca.crt";
            certificate_location = "/etc/certs/microservice-c.pem";
            key_location = "/etc/certs/microservice-c.key";
        } else if (env == "docker") {
            brokers = "127.0.0.1:9092";
            ca_location = "/usr/app/certs/ca.crt";
            certificate_location = "/usr/app/certs/microservice-c.pem";
            key_location = "/usr/app/certs/microservice-c.key";
        } else if (env == "local") {
            brokers = "localhost:9092";
            ca_location = "../microservice-needed-files/certs/ca.crt";
            certificate_location = "../microservice-needed-files/certs/microservice-c.pem";
            key_location = "../microservice-needed-files/certs/microservice-c.key";
        }
    }
}

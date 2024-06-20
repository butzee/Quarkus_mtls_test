#pragma once
#include <string>

const std::string brokers = "kafka.default.svc.cluster.local:9092";
const std::string topic_str_incoming = "prices-in";
const std::string topic_str_outgoing = "prices-out";

const int32_t partition = RdKafka::Topic::PARTITION_UA;
// SSL configuration parameters
const std::string ca_location = "/usr/app/certs/ca.crt";
const std::string certificate_location = "/usr/app/certs/microservice-c.pem";
const std::string key_location = "/usr/app/certs/microservice-c.key";
const std::string key_password = "changeit";
//const std::string ca_location = "../../certs/ca.crt";
//const std::string certificate_location = "../../certs/microservice-c.pem";
//const std::string key_location = "../../certs/microservice-c.key";


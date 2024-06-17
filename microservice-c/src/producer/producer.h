#pragma once
#include <librdkafka/rdkafkacpp.h>
#include "config.h"

class Producer {
public:
    Producer();
    ~Producer();
    void sendMessage(const std::string &message);
private:
    RdKafka::Producer *producer;
    RdKafka::Conf *conf;
};

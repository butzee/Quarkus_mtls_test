#pragma once
#include <librdkafka/rdkafkacpp.h>
#include "config.h"
#include "producer.h"
#include "SealService.h"

class Consumer {
public:
    Consumer();
    ~Consumer();
    void consumeMessage(Producer& producer);

private:
    RdKafka::Consumer *consumer;
    RdKafka::Conf *conf;
    RdKafka::Topic *topic;
    int partition;
    int64_t start_offset;
    SealService sealService;
};

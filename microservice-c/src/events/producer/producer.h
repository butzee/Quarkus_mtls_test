#pragma once
#include <rdkafkacpp.h>
#include <config.h>
#include <ResultData.h>

class Producer {
public:
    Producer();
    ~Producer();
    void sendMessage(const ResultData &resultData);
private:
    RdKafka::Producer *producer;
    RdKafka::Conf *conf;
};

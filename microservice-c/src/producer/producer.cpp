#include "producer.h"
#include <iostream>

Producer::Producer() {
    std::string errstr;
    conf = RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL);

    conf->set("metadata.broker.list", brokers, errstr);
    conf->set("security.protocol", "ssl", errstr);
    conf->set("ssl.ca.location", ca_location, errstr);
    conf->set("ssl.certificate.location", certificate_location, errstr);
    conf->set("ssl.key.location", key_location, errstr);
    conf->set("ssl.key.password", key_password, errstr);

    producer = RdKafka::Producer::create(conf, errstr);
    if (!producer) {
        std::cerr << "Failed to create producer: " << errstr << std::endl;
        exit(1);
    }
}

Producer::~Producer() {
    while (producer->outq_len() > 0) {
        producer->poll(1000);
    }
    delete producer;
    delete conf;
}

void Producer::sendMessage(const std::string &message) {
    RdKafka::ErrorCode resp = producer->produce(
        topic_str_outgoing, partition,
        RdKafka::Producer::RK_MSG_COPY,
        const_cast<char *>(message.c_str()), message.size(),
        nullptr, 0, 0, nullptr, nullptr);

    if (resp != RdKafka::ERR_NO_ERROR) {
        std::cerr << "% Produce failed: " << RdKafka::err2str(resp) << std::endl;
    } else {
        std::cerr << "% Produced message (" << message.size() << " bytes)" << std::endl;
    }

    producer->poll(0);
}

#include <consumer.h>
#include <iostream>
#include <SealData.h>
#include <ResultData.h>
#include <json.hpp>

Consumer::Consumer() {
    std::string errstr;
    conf = RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL);
    partition = 0;
    conf->set("metadata.broker.list", MicroserviceConfig::brokers, errstr);
    conf->set("security.protocol", "ssl", errstr);
    conf->set("ssl.ca.location", MicroserviceConfig::ca_location, errstr);
    conf->set("ssl.certificate.location", MicroserviceConfig::certificate_location, errstr);
    conf->set("ssl.key.location", MicroserviceConfig::key_location, errstr);
    conf->set("ssl.key.password", MicroserviceConfig::key_password, errstr);
    conf->set("group.id", "exampleGroup", errstr);
    conf->set("auto.offset.reset", "earliest", errstr);
    conf->set("fetch.message.max.bytes", "67108864", errstr);

    consumer = RdKafka::Consumer::create(conf, errstr);
    if (!consumer) {
        std::cerr << "Failed to create consumer: " << errstr << std::endl;
        exit(1);
    }

    topic = RdKafka::Topic::create(consumer, MicroserviceConfig::topic_str_incoming, nullptr, errstr);
    if (!topic) {
        std::cerr << "Failed to create topic: " << errstr << std::endl;
        exit(1);
    }

    RdKafka::ErrorCode resp = consumer->start(topic, partition, start_offset);
    if (resp != RdKafka::ERR_NO_ERROR) {
        std::cerr << "Failed to start consumer: " << RdKafka::err2str(resp) << std::endl;
        exit(1);
    }
}

Consumer::~Consumer() {
    consumer->stop(topic, partition);
    consumer->poll(1000);
    delete topic;
    delete consumer;
    delete conf;
}


void Consumer::consumeMessage(Producer& producer) {
    while (true) {
        RdKafka::Message *msg = consumer->consume(topic, partition, 1000);
        if (msg->err() == RdKafka::ERR_NO_ERROR) {
            try {
                std::cout << "Received message! "<< std::endl;
                std::string payload = std::string(static_cast<const char *>(msg->payload()), msg->len());
                SealData data = SealData::fromJson(payload);  // Deserialize JSON to SealData object
                ResultData resultData = sealService.consumeSealData(data);
                producer.sendMessage(resultData);
                std::cout << "Received and send encrypted result message" << std::endl;
            } catch (const std::exception& e) {
                std::cerr << "Error processing message: " << e.what() << std::endl;
            }
        } else if (msg->err() != RdKafka::ERR__TIMED_OUT) {
            std::cerr << "Consume error: " << msg->errstr() << std::endl;
            break;
        }
        delete msg;
    }
}

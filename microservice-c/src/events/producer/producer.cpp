#include <producer.h>
#include <iostream>
#include <ResultData.h>
#include <config.h>

using json = nlohmann::json;

Producer::Producer() {
    std::string errstr;
    conf = RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL);
    std::cout << "brokers: " << MicroserviceConfig::brokers << std::endl;

    conf->set("metadata.broker.list", MicroserviceConfig::brokers, errstr);
    conf->set("security.protocol", "ssl", errstr);
    conf->set("ssl.ca.location", MicroserviceConfig::ca_location, errstr);
    conf->set("ssl.certificate.location", MicroserviceConfig::certificate_location, errstr);
    conf->set("ssl.key.location", MicroserviceConfig::key_location, errstr);
    conf->set("ssl.key.password", MicroserviceConfig::key_password, errstr);

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

void Producer::sendMessage(const ResultData &resultData) {
    json j;
    j["EncryptedResult"] = resultData.encryptedPremium;
    j["UniqueId"] = resultData.uniqueId;

    std::string message = j.dump();

    RdKafka::ErrorCode resp = producer->produce(
        MicroserviceConfig::topic_str_outgoing, MicroserviceConfig::partition,
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

#include "SealData.h"

using json = nlohmann::json;

SealData SealData::fromJson(const std::string& jsonString) {
    auto j = json::parse(jsonString);
    SealData data;
    data.parameters = j.at("Parameters").get<std::string>();
    data.relinKeys = j.at("RelinKeys").get<std::string>();
    data.galoisKeys = j.at("GaloisKeys").get<std::string>();
    data.encryptedInput = j.at("Encryptedinput").get<std::string>();
    data.encryptedPricing = j.at("Encryptedpricing").get<std::string>();
    data.uniqueId = j.at("UniqueId").get<std::string>();
    return data;
}

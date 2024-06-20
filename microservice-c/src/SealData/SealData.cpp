#include "SealData.h"

using json = nlohmann::json;

SealData SealData::fromJson(const std::string& jsonString) {
    auto j = json::parse(jsonString);
    SealData data;
    data.parameters = j.at("parameters").get<std::string>();
    data.relinKeys = j.at("relinKeys").get<std::string>();
    data.galoisKeys = j.at("galoisKeys").get<std::string>();
    data.encryptedInput = j.at("encryptedInput").get<std::string>();
    data.encryptedPricing = j.at("encryptedPricing").get<std::string>();
    return data;
}

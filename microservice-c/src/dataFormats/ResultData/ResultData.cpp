#include <ResultData.h>

using json = nlohmann::json;

ResultData ResultData::fromJson(const std::string& jsonString) {
    auto j = json::parse(jsonString);
    ResultData data;
    data.encryptedPremium = j.at("EncryptedPremium").get<std::string>();
    data.uniqueId = j.at("UniqueId").get<std::string>();
    return data;
}

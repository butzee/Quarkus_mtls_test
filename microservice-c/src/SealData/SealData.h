#ifndef SEALDATA_H
#define SEALDATA_H

#include <string>
#include <json.hpp>

class SealData {
public:
    std::string parameters;
    std::string relinKeys;
    std::string galoisKeys;
    std::string encryptedInput;
    std::string encryptedPricing;

    static SealData fromJson(const std::string& jsonString);
};

#endif // SEALDATA_H

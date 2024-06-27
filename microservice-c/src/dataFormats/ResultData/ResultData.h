#ifndef RESULTDATA_H
#define RESULTDATA_H

#include <string>
#include <json.hpp>

class ResultData {
public:
    std::string encryptedPremium;
    std::string uniqueId;

    static ResultData fromJson(const std::string& jsonString);
};

#endif // SEALDATA_H

#ifndef SEAL_SERVICE_H
#define SEAL_SERVICE_H

#include <seal/seal.h>
#include <string>
#include <vector>
#include <SealData.h>
#include <ResultData.h>

#include "../nlohmann/json.hpp"
using json = nlohmann::json;

class SealService {
public:
    SealService();
    ResultData consumeSealData(const SealData& data);

private:
    seal::SEALContext setupContext();
    double calculate_premium(const std::vector<double>& input_data, const std::vector<double>& pricing_factors, seal::SEALContext& context);
    void save_to_json(const json& j, const std::string& filename);
};

#endif // SEAL_SERVICE_H

#ifndef SEAL_SERVICE_H
#define SEAL_SERVICE_H

#include <seal/seal.h>
#include <string>
#include <vector>
#include <SealData.h>

class SealService {
public:
    SealService();
    std::string transformMessage(const std::string& msg);
    std::string transformMessage(const std::vector<double>& input_data);
    std::string consumeSealData(const SealData& data);

private:
    seal::SEALContext setupContext();
    double calculate_premium(const std::vector<double>& input_data, const std::vector<double>& pricing_factors, seal::SEALContext& context);
};

#endif // SEAL_SERVICE_H

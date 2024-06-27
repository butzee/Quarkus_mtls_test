#ifndef SEAL_SERVICE_H
#define SEAL_SERVICE_H

#include <seal/seal.h>
#include <string>
#include <vector>
#include <json.hpp>
using namespace std;
using json = nlohmann::json;

class SealService {
public:
    SealService();
    string createKeysAndContext(const vector<double> numbers);
    string decrypt(const map<string, string> encryptedData);

private:
    seal::SEALContext setupContext();
    string save_to_json(const json& j, const string& filename);
    void save_parameters(const seal::EncryptionParameters& parms, json& j);
    void save_unique_id(const string& unique_id);
    void save_relin_keys(const seal::RelinKeys& relin_keys, json& j);
    void save_galois_keys(const seal::GaloisKeys& gal_keys, json& j);
    void save_secret_key(const seal::SecretKey& secret_key, json& j);
    void save_encryption(const seal::Ciphertext& encrypted_data, const string& filename, json& j);
    void save_to_file(const string& filename, const string& data);
};

#endif // SEAL_SERVICE_H

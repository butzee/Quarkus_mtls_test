#ifndef SEAL_SERVICE_H
#define SEAL_SERVICE_H

#include <seal/seal.h>
#include <string>
#include <vector>

class SealService {
public:
    SealService();
    void createKeysAndContext();

private:
    seal::SEALContext setupContext();
    void save_parameters(const seal::EncryptionParameters& parms);
    void save_relin_keys(const seal::RelinKeys& relin_keys);
    void save_galois_keys(const seal::GaloisKeys& gal_keys);
    void save_secret_key(const seal::SecretKey& secret_key);
    void save_encryption(const seal::Ciphertext& encrypted_data, const std::string& filename);
    void save_to_file(const std::string& filename, const std::string& data);
};

#endif // SEAL_SERVICE_H

#include <seal/seal.h>
#include "../nlohmann/json.hpp"
#include "../base64/base64.hpp"
#include "SealService.h"

#include <algorithm>
#include <chrono>
#include <cstddef>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <memory>
#include <mutex>
#include <numeric>
#include <random>
#include <sstream>
#include <string>
#include <thread>
#include <vector>
#include <fstream>

using namespace seal;
using namespace std;

SealService::SealService() {
    // Initialization code here, if any
}

void SealService::save_to_file(const string& filename, const string& data) {
    ofstream file(filename, ios::binary);
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file " << filename << endl;
        return;
    }
    file << data;
}

void SealService::save_parameters(const EncryptionParameters& parms) {
    cout << ">>>> Seal Service: Saving Parameters to File called 'parms.DAT'...\n";
    stringstream parms_stream;
    parms.save(parms_stream);
    save_to_file("parms.DAT", parms_stream.str());

    cout << ">>>> Seal Service: Converting Parameters to Base64...\n";
    string base64_data = base64::to_base64(parms_stream.str());

    cout << ">>>> Seal Service: Saving Base64 Parameters to File called 'base64.txt'...\n";
    ofstream base64_file("base64.txt", ios::app);
    if (!base64_file.is_open()) {
        std::cerr << "Error: Unable to open file base64.txt\n";
        return;
    }
    base64_file << "Parameters:\n"<< base64_data << "\n\n";

    cout << ">>>> Seal Service: Base64 Parameters saved to file 'base64.txt'...\n";
    //cout << ">>>> Seal Service: Base64 Parameters: " << base64_data << "\n\n";
}

void SealService::save_relin_keys(const RelinKeys& relin_keys) {
    cout << ">>>> Seal Service: Saving Relin Key to File called 'relinKey.DAT'...\n";
    stringstream relin_stream;
    relin_keys.save(relin_stream);
    save_to_file("relinKey.DAT", relin_stream.str());

    cout << ">>>> Seal Service: Converting Relin Key to Base64...\n";
    string base64_data_relinKeys = base64::to_base64(relin_stream.str());

    cout << ">>>> Seal Service: Saving Base64 Relin Key to File called 'base64.txt'...\n";
    ofstream base64_file("base64.txt", ios::app);
    if (!base64_file.is_open()) {
        std::cerr << "Error: Unable to open file base64.txt\n";
        return;
    }
    base64_file << "RelinKeys:\n"<< base64_data_relinKeys << "\n\n";

    cout << ">>>> Seal Service: Base64 Relin Key saved to file 'base64.txt'...\n";
    //cout << ">>>> Seal Service: Base64 Relin Key: " << base64_data_relinKeys << "\n\n";
}

void SealService::save_galois_keys(const GaloisKeys& galois_keys) {
    cout << ">>>> Seal Service: Saving Relin Key to File called 'galois.DAT'...\n";
    stringstream galois_stream;
    galois_keys.save(galois_stream);
    save_to_file("galois.DAT", galois_stream.str());

    cout << ">>>> Seal Service: Converting Relin Key to Base64...\n";
    string base64_data_galoisKeys = base64::to_base64(galois_stream.str());

    cout << ">>>> Seal Service: Saving Base64 Relin Key to File called 'base64.txt'...\n";
    ofstream base64_file("base64.txt", ios::app);
    if (!base64_file.is_open()) {
        std::cerr << "Error: Unable to open file base64.txt\n";
        return;
    }
    base64_file << "Galois Keys:\n"<< base64_data_galoisKeys << "\n\n";

    cout << ">>>> Seal Service: Base64 Galois Key saved to file 'base64.txt'...\n";
    //cout << ">>>> Seal Service: Base64 Galois Key: " << base64_data_galoisKeys << "\n\n";
}

void SealService::save_secret_key(const SecretKey& secret_key) {
    cout << ">>>> Seal Service: Saving Secret Key to File called 'secretKey.DAT'...\n";
    stringstream secret_key_stream;
    secret_key.save(secret_key_stream);
    save_to_file("secretKey.DAT", secret_key_stream.str());

    cout << ">>>> Seal Service: Converting Secret Key to Base64...\n";
    string base64_data_secretKey = base64::to_base64(secret_key_stream.str());

    cout << ">>>> Seal Service: Saving Base64 Secret Key to File called 'base64.txt'...\n";
    ofstream base64_file("base64.txt", ios::app);
    if (!base64_file.is_open()) {
        std::cerr << "Error: Unable to open file base64.txt\n";
        return;
    }
    base64_file << "Secret Key:\n"<< base64_data_secretKey << "\n\n";

    cout << ">>>> Seal Service: Base64 Secret Key saved to file 'base64.txt'...\n";
    //cout << ">>>> Seal Service: Base64 Secret Key: " << base64_data_secretKey << "\n\n";
}

void SealService::save_encryption(const Ciphertext& encrypted_Data, const string& filename) {
    cout << ">>>> Seal Service: Saving Encrypted " << filename << " Data to File called '" << filename << ".DAT'...\n";
    stringstream encrypted_data_stream;
    encrypted_Data.save(encrypted_data_stream);
    save_to_file(filename + ".DAT", encrypted_data_stream.str());

    cout << ">>>> Seal Service: Converting Encrypted " << filename << " Data to Base64...\n";
    string base64_data_encrypted_input = base64::to_base64(encrypted_data_stream.str());

    cout << ">>>> Seal Service: Saving Encrypted " << filename << " Data to File called 'base64.txt'...\n";
    ofstream base64_file("base64.txt", ios::app);
    if (!base64_file.is_open()) {
        std::cerr << "Error: Unable to open file base64.txt\n";
        return;
    }
    base64_file << "Encrypted "<<filename<<"\n" << base64_data_encrypted_input << "\n\n";

    cout << ">>>> Seal Service: Base64 Encrypted " << filename << " Data saved to file 'base64.txt'...\n";
    //cout << ">>>> Seal Service: Base64 Encrypted " << filename << " Data: " << base64_data_encrypted_input << "\n\n";
}


void SealService::createKeysAndContext() {
    std::cout << ">>>> Seal Service: Creating parameters...\n";
    EncryptionParameters parms(scheme_type::ckks);
    size_t poly_modulus_degree = 8192;
    parms.set_poly_modulus_degree(poly_modulus_degree);
    parms.set_coeff_modulus(CoeffModulus::Create(poly_modulus_degree, { 60, 40, 40, 60 }));
    std::cout << ">>>> Seal Service: Parameters created...\n";

    save_parameters(parms);

    cout << ">>>> Seal Service: Create Context, encoder and keygen...\n";
    SEALContext context(parms);
    CKKSEncoder encoder(context);
    KeyGenerator keygen(context);
    cout << ">>>> Seal Service: Context, encoder and keygen created...\n";


    std::cout << ">>>> Seal Service: Creating Public Key...\n";
    PublicKey public_key;
    keygen.create_public_key(public_key);
    std::cout << ">>>> Seal Service: Public Key created...\n";

    std::cout << ">>>> Seal Service: Creating Relin Key...\n";
    RelinKeys relin_keys;
    keygen.create_relin_keys(relin_keys);
    std::cout << ">>>> Seal Service: Relin Key created...\n";

    save_relin_keys(relin_keys);

    std::cout << ">>>> Seal Service: Creating Galois Key...\n";
    GaloisKeys gal_keys;
    keygen.create_galois_keys(gal_keys);
    std::cout << ">>>> Seal Service: Galois Key created...\n";

    save_galois_keys(gal_keys);
    
    std::cout << ">>>> Seal Service: Create Private Key...\n";
    SecretKey secret_key = keygen.secret_key();
    std::cout << ">>>> Seal Service: Private Key created...\n";

    save_secret_key(secret_key);
   
    std::cout << ">>>> Seal Service: Create encryptor, evaluator and decryptor...\n";
    Encryptor encryptor(context, public_key);
    Evaluator evaluator(context);
    Decryptor decryptor(context, secret_key);
    std::cout << ">>>> Seal Service: Encryptor, evaluator and decryptor created...\n";

    std::cout << ">>>> Seal Service: Encode Plaintext...\n";
    Plaintext plain_input, plain_pricing;
    vector<double> input_data = { 1.0, 2.0, 3.0, 4.0 };
    vector<double> pricing_factors = { 0.1, 0.2, 0.3, 0.4 };
    encoder.encode(input_data, pow(2.0, 40), plain_input);
    encoder.encode(pricing_factors, pow(2.0, 40), plain_pricing);

    std::cout << ">>>> Seal Service: Encrypt Plaintext...\n";
    Ciphertext encrypted_input, encrypted_pricing;
    encryptor.encrypt(plain_input, encrypted_input);
    encryptor.encrypt(plain_pricing, encrypted_pricing);

    std::cout << ">>>> Seal Service: Save encrypted Data...\n";
    save_encryption(encrypted_input, "input");
    save_encryption(encrypted_pricing, "pricing");

    std::coud << ">>>> Get all files "


    // Evaluate encrypted data
    Ciphertext multiplied;
    evaluator.multiply(encrypted_input, encrypted_pricing, multiplied);
    evaluator.relinearize_inplace(multiplied, relin_keys);
    
    // Sum up the rotated vectors
    vector<Ciphertext> rotations(4);
    for(int i = 0; i < 4; ++i) {
        evaluator.rotate_vector(multiplied, i, gal_keys, rotations[i]);
    }
    Ciphertext result;
    evaluator.add_many(rotations, result);

    // Decrypt and decode
    Plaintext result_plain;
    decryptor.decrypt(result, result_plain);
    vector<double> decoded_result;
    encoder.decode(result_plain, decoded_result);

    std::cout << "Premium Value: "+ std::to_string(decoded_result[0]);
}
#include <seal/seal.h>
#include "../nlohmann/json.hpp"
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

using std::vector;
using std::cout;
using std::endl;
using std::invalid_argument;
using namespace seal;

SealService::SealService() {
    // Initialization code here, if any
}

double SealService::calculate_premium(const vector<double>& input_data, const vector<double>& pricing_factors, SEALContext& context) {
    // Encoder, encryptor, evaluator, decryptor
    CKKSEncoder encoder(context);
    KeyGenerator keygen(context);
    PublicKey public_key;
    keygen.create_public_key(public_key);
    RelinKeys relin_keys;
    keygen.create_relin_keys(relin_keys);
    GaloisKeys gal_keys;
    keygen.create_galois_keys(gal_keys);
    Encryptor encryptor(context, public_key);
    Evaluator evaluator(context);
    Decryptor decryptor(context, keygen.secret_key());

    // Encode and encrypt data
    Plaintext plain_input, plain_pricing;
    encoder.encode(input_data, pow(2.0, 40), plain_input);
    encoder.encode(pricing_factors, pow(2.0, 40), plain_pricing);

    Ciphertext encrypted_input, encrypted_pricing;
    encryptor.encrypt(plain_input, encrypted_input);
    encryptor.encrypt(plain_pricing, encrypted_pricing);

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

    return decoded_result[0];
}

std::string SealService::transformMessage(const std::string& msg) {
    // Parse the JSON message
    auto json = nlohmann::json::parse(msg);
    vector<double> input_data = json["data"].get<vector<double>>(); // Car age, Kilometers driven, Accidents, Base Factor

    EncryptionParameters parms(scheme_type::ckks);
    size_t poly_modulus_degree = 8192;
    parms.set_poly_modulus_degree(poly_modulus_degree);
    parms.set_coeff_modulus(CoeffModulus::Create(poly_modulus_degree, { 60, 40, 40, 60 }));
    SEALContext context(parms);

    vector<double> pricing_factors = {25, 0.02, 100, 500}; // Per year, per kilometer, per accident, basic premium

    double premium = calculate_premium(input_data, pricing_factors, context);

    // Return the sum as a string
    return std::to_string(premium);
}

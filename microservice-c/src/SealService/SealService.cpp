#include <seal/seal.h>
#include "../nlohmann/json.hpp"
#include "../base64/base64.hpp"
#include "../SealData/SealData.h"
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

std::string SealService::consumeSealData(const SealData& data) {
    std::cout << "Received SealData: ";
    std::string decoded_parametes = base64::from_base64(data.parameters);
    std::stringstream buffer;
    buffer.str(decoded_parametes);

    EncryptionParameters parms(scheme_type::ckks);
    try {
        parms.load(buffer);
    } catch (const std::exception& e) {
        throw invalid_argument("Error loading parameters: " + std::string(e.what()));
    }
    SEALContext context(parms);

    std::string decoded_relin_keys = base64::from_base64(data.relinKeys);
    buffer.str(decoded_relin_keys);
    RelinKeys relin_keys;
    relin_keys.load(context, buffer);

    std::string decoded_galois_keys = base64::from_base64(data.galoisKeys);
    buffer.str(decoded_galois_keys);
    GaloisKeys gal_keys;
    gal_keys.load(context, buffer);

    std::string decoded_encrypted_input = base64::from_base64(data.encryptedInput);
    buffer.str(decoded_encrypted_input);
    Ciphertext encrypted_input;
    encrypted_input.load(context, buffer);
    
    std::string decoded_encrypted_pricing = base64::from_base64(data.encryptedPricing);
    buffer.str(decoded_encrypted_pricing);
    Ciphertext encrypted_pricing;
    encrypted_pricing.load(context, buffer);

    Evaluator evaluator(context);
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

    return "Maybe worked";
}


std::string SealService::transformMessage(const std::vector<double>& input_data) {
    std::stringstream buffer;
    std::cout << "Received data: ";
    EncryptionParameters parms(scheme_type::ckks);
    size_t poly_modulus_degree = 8192;
    parms.set_poly_modulus_degree(poly_modulus_degree);
    parms.set_coeff_modulus(CoeffModulus::Create(poly_modulus_degree, { 60, 40, 40, 60 }));
    SEALContext context(parms);
    
    // Saving parameters
    std::stringstream parms_stream;
    auto size = parms.save(parms_stream);
    vector<seal_byte> byte_buffer(static_cast<size_t>(parms.save_size()));
    parms.save(reinterpret_cast<seal_byte *>(byte_buffer.data()), byte_buffer.size());
    std::ofstream parms_file("parms.DAT");
    parms_file << parms_stream.str();
    parms_file.close();

    // Loading parameters
    std::ifstream parms_file2("parms.DAT");
    if (!parms_file2.is_open()) {
        throw invalid_argument("Cannot open file 'parms.DAT'");
    }
    std::stringstream bufferParms;
    bufferParms << parms_file2.rdbuf();
    std::string base64_data = base64::to_base64(bufferParms.str());
    bufferParms.clear();

    std::string decoded_data = base64::from_base64(base64_data);
    buffer.str(decoded_data);

    EncryptionParameters parms2(scheme_type::ckks);
    try {
        parms2.load(buffer);
    } catch (const std::exception& e) {
        throw invalid_argument("Error loading parameters: " + std::string(e.what()));
    }

    SEALContext context2(parms2);


    vector<double> pricing_factors = {25, 0.02, 100, 500}; // Per year, per kilometer, per accident, basic premium

    double premium = calculate_premium(input_data, pricing_factors, context2);

    // Return the sum as a string
    return std::to_string(premium);
}

std::string SealService::transformMessage(const std::string& msg) {
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

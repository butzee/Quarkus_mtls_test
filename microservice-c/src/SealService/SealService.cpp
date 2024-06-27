#include <seal/seal.h>
#include <json.hpp>
#include <base64.hpp>
#include <SealService.h>

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
#include <thread>

using namespace std;
using namespace seal;
using json = nlohmann::json;


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

void SealService::save_to_json(const json& j, const string& filename) {
    cout << ">>>> Seal Service: Saving JSON to file...\n";
    const char *path="output/";
    string file_path = path + filename;
    // Save the JSON to a file in specific folder
    ofstream file(file_path);
    if (!file.is_open()) {
        cerr << "Error: Unable to open file " << filename << endl;
        return;
    }
    file << setw(4) << j;
}

ResultData SealService::consumeSealData(const SealData& data) {
    cout << "Received SealData: ";
    string decoded_parametes = base64::from_base64(data.parameters);
    stringstream buffer;
    buffer.str(decoded_parametes);

    EncryptionParameters parms(scheme_type::ckks);
    try {
        parms.load(buffer);
    } catch (const exception& e) {
        throw invalid_argument("Error loading parameters: " + string(e.what()));
    }
    SEALContext context(parms);

    string decoded_relin_keys = base64::from_base64(data.relinKeys);
    buffer.str(decoded_relin_keys);
    RelinKeys relin_keys;
    relin_keys.load(context, buffer);

    string decoded_galois_keys = base64::from_base64(data.galoisKeys);
    buffer.str(decoded_galois_keys);
    GaloisKeys gal_keys;
    gal_keys.load(context, buffer);

    string decoded_encrypted_input = base64::from_base64(data.encryptedInput);
    buffer.str(decoded_encrypted_input);
    Ciphertext encrypted_input;
    encrypted_input.load(context, buffer);
    
    string decoded_encrypted_pricing = base64::from_base64(data.encryptedPricing);
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

    stringstream encrypted_result_stream;
    result.save(encrypted_result_stream);

    json j;
    j["EncryptedResult"] = base64::to_base64(encrypted_result_stream.str());
    save_to_json(j, "EncryptedResult.json");

    // Create a ResultData object with the encrypted result and unique id 
    ResultData result_data;
    result_data.encryptedPremium = base64::to_base64(encrypted_result_stream.str());
    result_data.uniqueId = data.uniqueId;
    
    return result_data;
}
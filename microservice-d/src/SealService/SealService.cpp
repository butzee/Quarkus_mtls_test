#include <seal/seal.h>
#include <base64.hpp>
#include <json.hpp>
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
using json = nlohmann::json;

SealService::SealService() {
    // Initialization code here, if any
}

string SealService::decrypt(const map<string, string> encryptedData) {
    cout << ">>>> Seal Service: Reading Seal Data...\n";
    const char *pathSeal = "output/";
    string filename = "seal_data.json";
    string file_path = pathSeal + filename;
    
    ifstream fileSeal(file_path);
    if (!fileSeal.is_open()) {
        cerr << "Error: Unable to open Seal data file " << filename << endl;
        return "Error: Unable to open Seal data file";
    }
    string sealData((istreambuf_iterator<char>(fileSeal)), istreambuf_iterator<char>());

    fileSeal >> sealData;
    fileSeal.close();

    cout << ">>>> Seal Service: Parsing Seal Data...\n";

    json jsonSealDataParsed = json::parse(sealData)["data"];
    json data;
    bool found = false;

    for (const auto& item : jsonSealDataParsed) {
        if (item.contains("UniqueId") && item["UniqueId"] == encryptedData.at("UniqueId")) {
            data = item;
            found = true;
            break;
        }
    }

    if (!found) {
        return "Error: Unique ID not found in the Seal data";
    }

    cout << ">>>> Seal Service: Decrypting data...\n";

    // Load the parameters and keys from the data
    string parms_str = base64::from_base64(data["Parameters"].get<string>());
    string secret_key_str = base64::from_base64(data["SecretKey"].get<string>());
    string encrypted_Input_str = base64::from_base64(data["Encryptedinput"].get<string>());

    string encrypted_result_str = base64::from_base64(encryptedData.at("EncryptedResult"));

    // Load the parameters and keys from the strings
    stringstream parms_stream(parms_str);
    EncryptionParameters parms;
    parms.load(parms_stream);

    SEALContext context(parms);
    CKKSEncoder encoder(context);
    KeyGenerator keygen(context);

    SecretKey secret_key;

    stringstream secret_key_stream(secret_key_str);
    secret_key.load(context, secret_key_stream);

    // Load the encrypted data from the strings
    stringstream encrypted_result_stream(encrypted_result_str);
    Ciphertext encrypted_result;
    encrypted_result.load(context, encrypted_result_stream);

    stringstream encrypted_input_stream(encrypted_Input_str);
    Ciphertext encrypted_input;
    encrypted_input.load(context, encrypted_input_stream);

    // Decrypt the data
    Decryptor decryptor(context, secret_key);
    Plaintext decrypted_result, decrypted_input;
    decryptor.decrypt(encrypted_result, decrypted_result);
    decryptor.decrypt(encrypted_input, decrypted_input);

    // Decode the decrypted data
    vector<double> decrypted_data;
    encoder.decode(decrypted_result, decrypted_data);
    vector<double> decrypted_input_data;
    encoder.decode(decrypted_input, decrypted_input_data);

    cout << ">>>> Seal Service: Data decrypted...\n";
    cout << ">>>> Seal Service: For the unique ID " << encryptedData.at("UniqueId") << " the input data were:" << endl;
    cout << "\tInsurance Level (1-4) - 75€: " << decrypted_input_data[0] << endl;
    cout << "\tVehicle age in years - 50€: " << decrypted_input_data[1] << endl;
    cout << "\tAnnual mileage in km - 0.01€ : " << decrypted_input_data[2] << endl;
    cout << "\tNumber of accidents - 100€: " << decrypted_input_data[3] << endl;
    cout << ">>>> Seal Service: The decrypted data is: " << decrypted_data[0] << endl;
    return "Decryption successful";
}

void SealService::save_to_file(const string& filename, const string& data) {
    cout << ">>>> Seal Service: Saving data to file in Folder test...\n";
    const char *path="output/";
    string file_path = path + filename;
    ofstream file(file_path, ios::binary);
    if (!file.is_open()) {
        cerr << "Error: Unable to open file " << filename << endl;
        return;
    }
    file << data;
}

void SealService::save_unique_id(const string& unique_id) {
    cout << ">>>> Seal Service: Saving unique ID to file...\n";
    const char *path="output/";
    string filename = "unique_id.txt";
    string file_path = path + filename;

    // append unique_id to file
    ofstream file(file_path, ios::app);
    if (!file.is_open()) {
        cerr << "Error: Unable to open file " << filename << endl;
        return;
    }
    file << unique_id << endl;
}

string SealService::save_to_json(const json& new_data, const string& filename) {
    cout << ">>>> Seal Service: Saving JSON to file...\n";
    const char *path = "output/";
    string file_path = path + filename;
    json root;  // This will store the root object that includes the data array

    // Check if the file exists and load its content
    ifstream file_read(file_path);
    if (file_read.is_open()) {
        file_read >> root;
        file_read.close();
    }

    // Check if the 'data' key exists in the root, if not create it
    if (!root.contains("data") || !root["data"].is_array()) {
        root["data"] = json::array();
    }

    // Generate a unique ID for the new data entry
    srand(static_cast<unsigned>(time(nullptr)));
    string unique_id = to_string(rand()) + "_" + to_string(time(nullptr));

    // Create a new JSON object for the entry
    json new_entry = new_data;
    new_entry["UniqueId"] = unique_id;

    // Append the new entry to the data array
    root["data"].push_back(new_entry);

    // Save the updated root object to the file
    ofstream file_write(file_path);
    if (!file_write.is_open()) {
        // Attempt to create the file if it does not exist
        file_write.open(file_path, ios::out);
        if (!file_write.is_open()) {
            cerr << "Error: Unable to open or create file " << filename << endl;
            return "Error: Unable to open or create file";
        }
    }
    file_write << setw(4) << root;  // Write with nice formatting
    file_write.close();

    return unique_id;
}

void SealService::save_parameters(const EncryptionParameters& parms, json& j) {
    cout << ">>>> Seal Service: Saving Parameters...\n";
    stringstream parms_stream;
    parms.save(parms_stream);
    j["Parameters"] = base64::to_base64(parms_stream.str());
}

void SealService::save_relin_keys(const RelinKeys& relin_keys, json& j) {
    cout << ">>>> Seal Service: Saving Relin Key...\n";
    stringstream relin_stream;
    relin_keys.save(relin_stream);
    j["RelinKeys"] = base64::to_base64(relin_stream.str());
}

void SealService::save_galois_keys(const GaloisKeys& galois_keys, json& j) {
    cout << ">>>> Seal Service: Saving Galois Key...\n";
    stringstream galois_stream;
    galois_keys.save(galois_stream);
    j["GaloisKeys"] = base64::to_base64(galois_stream.str());
}

void SealService::save_secret_key(const SecretKey& secret_key, json& j) {
    cout << ">>>> Seal Service: Saving Secret Key...\n";
    stringstream secret_key_stream;
    secret_key.save(secret_key_stream);
    j["SecretKey"] = base64::to_base64(secret_key_stream.str());
}

void SealService::save_encryption(const Ciphertext& encrypted_Data, const string& filename, json& j) {
    cout << ">>>> Seal Service: Saving Encrypted " << filename << " Data...\n";
    stringstream encrypted_data_stream;
    encrypted_Data.save(encrypted_data_stream);
    j["Encrypted" + filename] = base64::to_base64(encrypted_data_stream.str());
}

string SealService::createKeysAndContext(const vector<double> numbers) {
    json j;

    cout << ">>>> Seal Service: Creating parameters...\n";
    EncryptionParameters parms(scheme_type::ckks);
    size_t poly_modulus_degree = 8192;
    parms.set_poly_modulus_degree(poly_modulus_degree);
    parms.set_coeff_modulus(CoeffModulus::Create(poly_modulus_degree, { 60, 40, 40, 60 }));
    cout << ">>>> Seal Service: Parameters created...\n";

    save_parameters(parms, j);

    cout << ">>>> Seal Service: Create Context, encoder and keygen...\n";
    SEALContext context(parms);
    CKKSEncoder encoder(context);
    KeyGenerator keygen(context);
    cout << ">>>> Seal Service: Context, encoder and keygen created...\n";


    cout << ">>>> Seal Service: Creating Public Key...\n";
    PublicKey public_key;
    keygen.create_public_key(public_key);
    cout << ">>>> Seal Service: Public Key created...\n";

    cout << ">>>> Seal Service: Creating Relin Key...\n";
    RelinKeys relin_keys;
    keygen.create_relin_keys(relin_keys);
    cout << ">>>> Seal Service: Relin Key created...\n";

    save_relin_keys(relin_keys, j);

    cout << ">>>> Seal Service: Creating Galois Key...\n";
    GaloisKeys gal_keys;
    keygen.create_galois_keys(gal_keys);
    cout << ">>>> Seal Service: Galois Key created...\n";

    save_galois_keys(gal_keys, j);
    
    cout << ">>>> Seal Service: Create Private Key...\n";
    SecretKey secret_key = keygen.secret_key();
    cout << ">>>> Seal Service: Private Key created...\n";

    save_secret_key(secret_key, j);
   
    cout << ">>>> Seal Service: Create encryptor, evaluator and decryptor...\n";
    Encryptor encryptor(context, public_key);
    Evaluator evaluator(context);
    Decryptor decryptor(context, secret_key);
    cout << ">>>> Seal Service: Encryptor, evaluator and decryptor created...\n";

    cout << ">>>> Seal Service: Encode Plaintext...\n";
    Plaintext plain_input, plain_pricing;
    vector<double> pricing_factors = { 75.0, 50.0, 0.01, 100.0};
    encoder.encode(numbers, pow(2.0, 40), plain_input);
    encoder.encode(pricing_factors, pow(2.0, 40), plain_pricing);

    cout << ">>>> Seal Service: Encrypt Plaintext...\n";
    Ciphertext encrypted_input, encrypted_pricing;
    encryptor.encrypt(plain_input, encrypted_input);
    encryptor.encrypt(plain_pricing, encrypted_pricing);

    cout << ">>>> Seal Service: Save encrypted Data...\n";
    save_encryption(encrypted_input, "input", j);
    save_encryption(encrypted_pricing, "pricing", j);

    cout << ">>>> Get all files ";

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

    //print json to file
    string unique_id = save_to_json(j, "seal_data.json");
    
    save_unique_id(unique_id);
    j["UniqueId"] = unique_id;

    cout << ">>>> Seal Service: Unique Id generated - " << unique_id << endl;
    return unique_id;
}
#include <fstream>
#include <thread>
#include "SealService/SealService.h"
#include "WebRequestService/WebRequestService.h"
#include "nlohmann/json.hpp"

using json = nlohmann::json;

int main() {
    SealService sealService;
    sealService.createKeysAndContext();
    /*
    WebRequestService webRequestService;
    std::string url = "http://127.0.0.1:8080/produce";

    std::ifstream file("base64.txt");
    if (!file.is_open()) {
        std::cerr << "Failed to open file.\n";
        return 1;
    }
    std::string line;
    std::string parameters, relinKeys, galoisKeys, encryptedInput, encryptedPricing;

    while (std::getline(file, line)) {
        line.erase(0, line.find_first_not_of(" \t\r\n"));
        line.erase(line.find_last_not_of(" \t\r\n") + 1);

        if (line.empty() || line[0] == '#') {
            continue;
        }

        if (line == "Parameters:") {
            std::getline(file, parameters);
        } else if (line == "RelinKeys:") {
            std::getline(file, relinKeys);
        } else if (line == "Galois Keys:") {
            std::getline(file, galoisKeys);
        } else if (line == "Encrypted input") {
            std::getline(file, encryptedInput);
        } else if (line == "Encrypted pricing:") {
            std::getline(file, encryptedPricing);
        }
    }

    file.close();

    // Create a JSON object
    json body;
    body["Parameters"] = parameters;
    body["RelinKeys"] = relinKeys;
    body["GaloisKeys"] = galoisKeys;
    body["EncryptedInput"] = encryptedInput;
    body["EncryptedPricing"] = encryptedPricing;

    std::string json_body = body.dump();
    
    std::string response = webRequestService.send_via_http(url, json_body);
        // Process the response
    std::cout << "Response: " << response << std::endl;

    std::cout <<"Shutting down.\n";
    return 0;
    */
}
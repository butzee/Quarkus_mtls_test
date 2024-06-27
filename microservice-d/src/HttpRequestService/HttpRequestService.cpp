#include <HttpRequestService.h>
#include <HTTPRequest.hpp>
#include <SealData.h>
#include <iostream>
#include <string>
#include <fstream>
#include <json.hpp>
using json = nlohmann::json;

using namespace std;

HttpRequestService::HttpRequestService() {
   
}

string HttpRequestService::sendSealData(string uniqueId) {
    cout << "Sending request to http://localhost:8080/endpoint\n";
    try {
        http::Request request{"http://host.docker.internal:8080/endpoint"};

        const char *path="output/";
        string filename = "seal_data.json";
        string file_path = path + filename;
        // Save the JSON to a file in specific folder
        ifstream fileResult(file_path);
        if (!fileResult.is_open()) {
            cerr << "Error: Unable to open Result data file " << filename << endl;
            return "Error: Unable to open Result data file";
        }
        string resultData((istreambuf_iterator<char>(fileResult)), istreambuf_iterator<char>());
        fileResult >> resultData;
        fileResult.close();

        // Parse the JSON data
        auto j = json::parse(resultData);
        cout << "JSON size: " << j.size() << endl;

        json root;
        root["data"] = json::array();

        // Check if "data" field exists and is an array
        if (j.contains("data") && j.at("data").is_array()) {
            auto dataArray = j.at("data");
            cout << "Data array size: " << dataArray.size() << endl;

            // Iterate over each object in the array
            for (const json& item : dataArray) {
                try {
                    SealData data = SealData::fromJson(item.dump());

                    // Check the unique ID
                    cout << "Checking UniqueId: " << data.uniqueId << endl;

                    if (data.uniqueId == uniqueId) {
                        root["data"].push_back(item);
                        break;
                    }
                } catch (const std::exception& e) {
                    cerr << "Error parsing item: " << e.what() << endl;
                }
            }
        } else {
            return "Error: JSON does not contain 'data' array.";
        }
        const string body = root.dump();
        const auto response = request.send("POST", body, {
            {"Content-Type", "application/json"}
        });
        cout << string{response.body.begin(), response.body.end()} << '\n';
        return string{response.body.begin(), response.body.end()};
    } catch (const exception& e) {
        string error = "Request failed, error: " + string{e.what()};
        return error;
    }
}

map<string, string> HttpRequestService::sendRequest(string uniqueId) {
    try {
        // ...getId?uniqueId=someUniqueId
        http::Request request{"http://host.docker.internal:8081/getId?uniqueId=" + uniqueId};
        // send a get request
        const auto response = request.send("GET");
        string responseString = string{response.body.begin(), response.body.end()};
        // First check if the response is a JSON object
        json j = json::parse(responseString);

        cout << "UniqueId: " << j.at("UniqueId") << endl;

        map<string, string> encryptedData;
        encryptedData["UniqueId"] = j.at("UniqueId").get<string>();
        encryptedData["EncryptedResult"] = j.at("EncryptedResult").get<string>();

        return encryptedData;
    } catch (const exception& e) {
        map<string, string> error;
        error["error"] = "Request failed, error: " + string{e.what()};
        return error;
    }
}
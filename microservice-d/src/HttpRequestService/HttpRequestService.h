#ifndef HTTP_REQUEST_SERVICE_H
#define HTTP_REQUEST_SERVICE_H

#include <json.hpp>
#include <string>
using namespace std;
using json = nlohmann::json;

class HttpRequestService {
public:
    HttpRequestService();
    string sendSealData(string uniqueId);
    map<string, string> sendRequest(string uniqueId);
};

#endif // SEAL_SERVICE_H

#ifndef WEB_REQUEST_SERVICE_H
#define WEB_REQUEST_SERVICE_H

#include <curl/curl.h>
#include <iostream>
#include <vector>
#include <string>

class WebRequestService {
public:
    WebRequestService();
    std::string send_via_http(const std::string& url, const std::string& json_body);
    static size_t write_callback(char* ptr, size_t size, size_t nmemb, std::string* data);
};

#endif // WEB_REQUEST_SERVICE_H

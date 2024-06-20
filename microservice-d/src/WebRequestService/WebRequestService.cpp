#include <curl/curl.h>
#include <iostream>
#include <vector>
#include <string>
#include "WebRequestService.h"

WebRequestService::WebRequestService() {
}

// Static member function definition
size_t WebRequestService::write_callback(char* ptr, size_t size, size_t nmemb, std::string* data) {
    if (data == nullptr) {
        return 0;
    }

    // Append received data to the string
    data->append(ptr, size * nmemb);

    // Return the number of bytes processed
    return size * nmemb;
}

std::string WebRequestService::send_via_http(const std::string& url, const std::string& body) {
        CURL* curl;
    CURLcode res;
    std::string response;

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_POST, 1L);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, body.size());
        
        // Set the callback function to process the response
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        // Perform the request
        res = curl_easy_perform(curl);

        // Check for errors
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }

        // Clean up
        curl_easy_cleanup(curl);
    }

    return response;
}
#include <iostream>
#include <thread>
#include <config.h>
#include <producer.h>
#include <consumer.h>
#include <fstream>
#include <chrono>
#include <thread>

bool file_exists(const std::string& name) {
    std::ifstream f(name.c_str());
    return f.good();
}

int main(int argc, char* argv[]) {
    std::string environment = "local";
    if (argc > 1) {
        std::cout << "Environment: " << argv[1] << std::endl;
        environment = argv[1];
    }

    MicroserviceConfig::configureEnvironment(environment);

    Producer producer;
    Consumer consumer;

    consumer.consumeMessage(producer);
    return 0;
}
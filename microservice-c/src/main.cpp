#include <iostream>
#include <thread>
#include "producer/producer.h"
#include "consumer/consumer.h"

int main() {
    //Producer producer;
    //Consumer consumer;

    //consumer.consumeMessage(producer);

    SealService sealService;
    // {"data": [1.0, 2.0, 3.0, 4.0]}
    std::vector<double> input_data = {1.0, 2.0, 3.0, 4.0};
    std::string premium = sealService.transformMessage(input_data);

    std::cout <<"Premium: " + premium + "\n" << "Shutting down.\n";
    return 0;
}
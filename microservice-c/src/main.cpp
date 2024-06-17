#include <iostream>
#include <thread>
#include "producer/producer.h"
#include "consumer/consumer.h"

int main() {
    Producer producer;
    Consumer consumer;

    consumer.consumeMessage(producer);

    std::cout << "Shutting down.\n";
    return 0;
}
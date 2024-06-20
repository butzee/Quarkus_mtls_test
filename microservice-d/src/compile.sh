#!/bin/bash

g++ -std=c++17 -o myapp main.cpp SealService/SealService.cpp WebRequestService/WebRequestService.cpp \
    -I./SealService -I./properties -I./nlohmann -I./base64 -I./WebRequestService \
    -I../CurlLib/include -L../CurlLib/lib -lcurl \
    -I../KafkaLib/include -L../KafkaLib/lib -lrdkafka++ \
    -I../SealLib/include/SEAL-4.1 -L../SealLib/lib -lseal-4.1 -lssl -lcrypto -lm -Wl,-rpath,../KafkaLib/lib

# export LD_LIBRARY_PATH=../KafkaLib/lib:$LD_LIBRARY_PATH Falls lrdkafka++.so.1 nicht gefefunden wird

echo "Compilation Done"
FROM ubuntu:20.04

ENV DEBIAN_FRONTEND noninteractive

# Set the working directory in the container
WORKDIR /usr/app

# Install necessary packages
RUN apt-get update && apt-get upgrade -y && \
    apt-get install -y --no-install-recommends --no-install-suggests \
    git \
    cmake \
    build-essential \
    ca-certificates \
    libssl-dev 

RUN mkdir -p libs/SealLib SEAL src libs/KafkaLib/include libs/KafkaLib/lib certs SingleHeaders/base64 SingleHeaders/nlohmann SingleHeaders/httprequest output

# Clone and build Microsoft SEAL with specified installation prefix
RUN git clone https://github.com/microsoft/SEAL.git ./SEAL && \
    cmake -S SEAL/. -B build -DCMAKE_INSTALL_PREFIX=./libs/SealLib && \
    cmake --build build && \
    cmake --install build && \
    rm -rf SEAL build

RUN apt-get install -y wget && \
    wget -O SingleHeaders/nlohmann/json.hpp https://raw.githubusercontent.com/nlohmann/json/develop/single_include/nlohmann/json.hpp && \
    wget -O SingleHeaders/nlohmann/json_fwd.hpp https://github.com/nlohmann/json/blob/develop/single_include/nlohmann/json_fwd.hpp && \
    wget -O SingleHeaders/base64/base64.hpp https://raw.githubusercontent.com/tobiaslocker/base64/master/include/base64.hpp && \
    wget -O SingleHeaders/httprequest/HTTPRequest.hpp https://raw.githubusercontent.com/elnormous/HTTPRequest/master/include/HTTPRequest.hpp

COPY src/ ./src

RUN g++ -std=c++17 -O2 -Wall -Wextra -o microservice-d \
    src/main.cpp \
    src/SealData/SealData.cpp \
    src/SealService/SealService.cpp \
    src/HttpRequestService/HttpRequestService.cpp \
    -Isrc/SealService -Isrc/HttpRequestService -Isrc/SealData \
    -ISingleHeaders/nlohmann \
    -ISingleHeaders/base64 \
    -ISingleHeaders/httprequest \
    -Ilibs/SealLib/include/SEAL-4.1 \
    -Llibs/SealLib/lib \
    -lseal-4.1 -lssl -lcrypto -lm

CMD ["./microservice-d"]
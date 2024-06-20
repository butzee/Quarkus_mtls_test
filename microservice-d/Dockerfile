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
    ca-certificates

RUN mkdir -p libs/SealLib SEAL src libs/KafkaLib/include libs/KafkaLib/lib

# Clone and build Microsoft SEAL with specified installation prefix
RUN git clone https://github.com/microsoft/SEAL.git ./SEAL && \
    cmake -S SEAL/. -B build -DCMAKE_INSTALL_PREFIX=./libs/SealLib && \
    cmake --build build && \
    cmake --install build && \
    rm -rf SEAL build

# Clone and build librdkafka with specified installation prefix
RUN git clone https://github.com/confluentinc/librdkafka.git ./librdkafka

RUN cd librdkafka && \
    ./configure --install-deps && \
    make && \
    make install && \
    cp -r win32 ./../libs && \
    cp /usr/local/lib/librd* ../libs/KafkaLib/lib/. && \
    cp /usr/local/include/librdkafka/* ../libs/KafkaLib/include/. && \
    cd .. && \
    rm -rf librdkafka

ENV LD_LIBRARY_PATH=/usr/app/libs/KafkaLib/lib:$LD_LIBRARY_PATH

# Copy src files to the working directory
COPY src/ ./src
COPY certs/ ./certs
COPY base64/ ./base64
COPY nlohmann/ ./nlohmann

#-I Directory wo nach header-Dateien gesucht wird
#-L Directory wo nach Bibliotheken gesucht wird

RUN cd src && g++ -std=c++17 -o myapp main.cpp SealService/SealService.cpp WebRequestService/WebRequestService.cpp \
-I./SealService -I./properties -I./nlohmann -I./base64 -I./WebRequestService \
-I../CurlLib/include -L../CurlLib/lib -lcurl \
-I../KafkaLib/include -L../KafkaLib/lib -lrdkafka++ \
-I../SealLib/include/SEAL-4.1 -L../SealLib/lib -lseal-4.1 -lssl -lcrypto -lm -Wl,-rpath,../KafkaLib/lib

# Run the executable
CMD ["./src/microservice"]
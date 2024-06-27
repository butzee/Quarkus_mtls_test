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

RUN mkdir -p build-libs/SealLib SEAL librdkafka src build-libs/KafkaLib/include build-libs/KafkaLib/lib build-libs/ssl openssl

# Clone and build Microsoft SEAL with specified installation prefix
RUN git clone https://github.com/microsoft/SEAL.git ./SEAL && \
    cmake -S SEAL/. -B build -DCMAKE_INSTALL_PREFIX=./build-libs/SealLib && \
    cmake --build build && \
    cmake --install build && \
    rm -rf SEAL build

# Clone and build librdkafka with specified installation prefix
RUN git clone https://github.com/confluentinc/librdkafka.git ./librdkafka &&  \
    cd librdkafka && \
    ./configure --install-deps && \
    make && \
    make install && \
    cp -r win32 ./../build-libs && \
    cp /usr/local/lib/librd* ../build-libs/KafkaLib/lib/. && \
    cp /usr/local/include/librdkafka/* ../build-libs/KafkaLib/include/. && \
    cd .. && \
    rm -rf librdkafka

COPY entrypoint.sh /usr/app/
RUN chmod +x /usr/app/entrypoint.sh

ENTRYPOINT ["/usr/app/entrypoint.sh"]
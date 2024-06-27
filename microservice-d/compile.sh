#!/bin/bash

# Define paths to libraries
PATH_TO_KAFKA_LIB="../microservice-needed-files/libs/KafkaLib"
PATH_TO_SEAL_LIB="../microservice-needed-files/libs/SealLib"
PATH_TO_Further_HEADERS="../microservice-needed-files/SingleHeaders"

# Compiler and linker configuration
COMPILER="g++"
COMPILER_FLAGS="-std=c++17 -O2 -Wall -Wextra"
LINKER_FLAGS="-Wl,-rpath,${PATH_TO_KAFKA_LIB}/lib"

# Include directories
INCLUDE_DIRS=(
    "src/SealService"
    "src/properties"
    "src/HttpRequestService"
    "${PATH_TO_Further_HEADERS}/nlohmann"
    "${PATH_TO_Further_HEADERS}/base64"
    "${PATH_TO_Further_HEADERS}/httprequest"
    "${PATH_TO_SEAL_LIB}/include/SEAL-4.1"
)

# Library directories and libraries
LIBRARY_DIRS=(
    "-L${PATH_TO_SEAL_LIB}/lib"
)
LIBRARIES=(
    "-lseal-4.1"
    "-lssl"
    "-lcrypto"
    "-lm"
)

# Convert array to string
INCLUDES=$(printf " -I%s" "${INCLUDE_DIRS[@]}")
LIB_DIRS=$(printf " %s" "${LIBRARY_DIRS[@]}")
LIBS=$(printf " %s" "${LIBRARIES[@]}")

# Source files
SOURCE_FILES=(
    "src/main.cpp"
    "src/SealService/SealService.cpp"
    "src/HttpRequestService/HttpRequestService.cpp"
)

# Compile command
$COMPILER $COMPILER_FLAGS -o microservice-d ${SOURCE_FILES[@]} $INCLUDES $LIB_DIRS $LIBS $LINKER_FLAGS

echo "Compilation Done"
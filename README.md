## Project/POC: Secure Microservices with Kafka

This project demonstrates a proof-of-concept (POC) microservice architecture using Kafka for secure communication and computation within a Kubernetes cluster. The architecture simulates a cloud environment and secure computing on sensitive data, specifically for calculating the premium value of car insurance.

### Table of Contents
1. [Architecture Overview](#architecture-overview)
2. [Prerequisites](#prerequisites)
3. [Usage](#usage)
4. [PowerShell Functions](#powershell-functions)
5. [Detailed Steps](#detailed-steps)

### Architecture Overview

The architecture consists of four microservices (a, b, c, and d), with the following deployment and technology details:

- **Microservices a, b, and c**: Deployed inside a Kubernetes cluster (public cloud) and accessible via NodePort configuration.
- **Microservice d**: Running on-premises.

#### Technologies Used

- **Microservices a and b**: Built with Quarkus.
- **Microservices c and d**: Built with C++ and the SEAL library.
- **Secure Communication**: Inside the Kubernetes cluster using mTLS.
- **Data Communication**: Via Kafka topics.

### Microservices Details

**Microservice A**:
- **Endpoint**: `/endpoint` - Accepts a JSON array with SealData objects containing the following fields: Parameters, RelinKeys, GaloisKeys, EncryptedInput, EncryptedPricing, and UniqueId.
- **Functionality**: Produces SealData objects to the `topic-seal-data` Kafka topic.

**Microservice B**:
- **Endpoints**:`/getId` - Accepts a uniqueId as a string and searches for an encrypted result value with the corresponding uniqueId, then returns it.
- **Functionality**: Consumes SealData objects from the `topic-encrypted-premium` Kafka topic.

**Microservice C**:
- **Functionality**:
  - Listens to the `topic-seal-data` Kafka topic.
  - Computes values using encrypted data from the SealData objects.
  - Produces results to the `topic-encrypted-premium` Kafka topic.

**Microservice D**:
- **Functionality**:
  - Interactive application for creating Seal context with a unique ID, required SEAL parameters, and values.
  - Takes user input for premium value computation.
  - Sends SealData objects with a unique ID to the `/endpoint` of microservice A.
  - Makes GET requests to the `/getId` endpoint of microservice B with the unique ID to retrieve and decrypt the computed premium value.

### Workflow

1. **Microservice D**:
   - Creates Seal context and SealData objects with unique IDs.
   - Sends SealData to microservice A via `/endpoint`.
   
2. **Microservice A**:
   - Receives SealData and produces it to the `topic-seal-data` Kafka topic.
   
3. **Microservice C**:
   - Listens to `topic-seal-data`, processes encrypted values, and computes the required data.
   - Produces results to the `topic-encrypted-premium` Kafka topic.
   
4. **Microservice B**:
   - Consumes results from `topic-encrypted-premium`.
   - Endpoint `/getId` allows querying the results by unique ID.

### Prerequisites

Ensure you have the following installed and running:
- Docker Desktop and Docker Compose
- kind (Kubernetes in Docker)
- kubectl
- Helm
- Java 11 or higher
- Quarkus 2.15.3
- Maven

### Usage

Execute the following PowerShell script to manage and execute the project:

```powershell
Start-ContainerOrCluster.ps1
```

This script manages the project fully. Start it and beginn with "Get Information about the Project and Prerequisites".
## Project/POC: Secure Microservices with Kafka

This project demonstrates how to develop and run two secure microservices that communicate with each other using Apache Kafka. The communication is secured with mTLS encryption.

## Table of Contents

* [Prerequisites](#prerequisites)
* [Usage](#usage)

## Prerequisites

This project requires the following software to be installed on your system:

* **Java Development Kit (JDK):** Version 17 or higher
* **Quarkus:** Version 2.15.3.Final or higher
* **Maven:** Must be installed in your environment
* **Docker and Docker Compose** Each a current version

These tools are helpful for running the project in a containerized environment. Instructions for installing Docker and Docker Compose can be found on their respective websites.

## Usage

### 1. Generate Certificates (if needed)

**Note:** This step is only necessary if you don't already have the required certificates for secure communication. You need the metioned files inside the `docker-compose.yaml` and each `applications.properties`-File. They need to be places inside a Folder called `certs` at Root-Level of this Project.

1. Navigate to the `CertificateCreation` folder in your terminal.
2. Run the following command to generate the certificates:

```bash
./Certificate.sh
```

This will create certificates used for mTLS encryption.

### 2. Start the Kafka Service

1. Navigate to the `Kafka` folder in your terminal.
2. Run the following command to start the Kafka service:

```bash
docker-compose up -build
```

**Optional:** Add the `-d` flag after the command to run the service in the background without seeing the logs.

**For debugging purposes:**

You can uncomment the optional argument `KAFKA_OPTS: "-Djavax.net.debug=SSL,handshake"` within the `docker-compose.yaml` file. This will enable more verbose logging for troubleshooting SSL connections.

### 3. Start the Microservices

**Open two separate terminal windows.**

1. Navigate to the directory for each microservice (microservice-a and microservice-b).
2. Run the following command in each terminal to start the microservice in development mode:

```bash
quarkus dev "-Djavax.net.debug=SSL,handshake"
```

**Note:** The `-Djavax.net.debug=SSL,handshake` parameter is optional and enables debugging for SSL connections.

### 4. Testing the Connection

1. Open a web browser and navigate to `localhost:8080/produce`.
2. Microservice A should send the message "Sending price: 123" via the channel "generated-price".
3. Microservice B should consume the message from the mentioned channel.

**Verification:**

The message exchange should be visible in the logs of each microservice.

### 5. Stopping Services

* You can stop the microservices by terminating the running processes in their respective terminal windows.
* To stop the Kafka service, run the following command in your terminal:

```bash
docker-compose down -v
```

The `-v` flag removes any stopped containers.

## TODO

- [ ] Full Deployment via Docker-Compose
- [ ] Full Deployment via Helm to Kubernetes
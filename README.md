## Project/POC: Secure Microservices with Kafka

This project demonstrates how to develop and run two secure microservices that communicate with each other using Apache Kafka. The communication is secured with mTLS encryption.

## Usage

Execute the following powershell-command to run the project:

```powershell
Start-ContainerOrCluster.ps1
```

Inside the `Start-ContainerOrCluster.ps1` script you can decide whether you want to run kafka in a Docker container and the Services via Quarkus-Dev or everything inside a Kubernetes cluster.

You can read the script to see how the services are started and how the Kafka-Cluster is created. You can use the information to start everything manually if you want to.

## Services

The project consists of two services:
- microservice-a
- microservice-b

Both services are implemented with Quarkus and communicate with each other via Apache Kafka. The communication is secured with mTLS encryption.
More Information about the services can be found in the respective README.md files in the service directories.

sed -i -e 's/\r$//' 
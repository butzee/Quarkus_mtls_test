kubectl create secret generic certs-microservice-a --from-file=truststore.jks=../certs/truststore.jks --from-file=microservice-a.keystore.jks=../certs/microservice-a.keystore.jks
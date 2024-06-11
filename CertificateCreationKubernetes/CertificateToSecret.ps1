kubectl create secret generic certs-microservice-a --from-file=truststore.jks=../certs/truststore.jks --from-file=microservice-a.keystore.jks=../certs/microservice-a.keystore.jks

kubectl create secret generic certs-microservice-b --from-file=truststore.jks=../certs/truststore.jks --from-file=microservice-b.keystore.jks=../certs/microservice-a.keystore.jks


kubectl create secret generic secret-name-1 --from-file=kafka.truststore.jks=../certs/kafka.truststore.jks --from-file=kafka.keystore.jks=../certs/kafka.keystore.jks
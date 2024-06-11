# GraalVM Native Image: Generating 'microservice-a-1.0.0-SNAPSHOT-runner' (executable)...
./mvnw clean install -Pnative --define quarkus.native.container-build=true

# Create Docker native micro Image
docker build -f src/main/docker/Dockerfile.native-micro -t 127.0.0.1:5000/microservice-b:latest .

# Tagging des Image
docker tag 127.0.0.1:5000/microservice-b:latest2 127.0.0.1:5000/microservice-b:latest

# Pushen des Images in die lokale Registry
docker push 127.0.0.1:5000/microservice-b:latest

# helm install microservice-a .\microservice-a-0.1.0.tgz
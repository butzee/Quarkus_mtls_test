# Start a local registry
docker run -d -p 5000:5000 --name kind-registry --restart always registry:2

# Create a kind cluster based on the configuration file with the local registry
kind create cluster --config kind-config.yaml

# Connect the kind cluster to the local registry
docker network connect "kind" kind-registry
# Delete the kind cluster
kind delete cluster

# Disconnect the Docker registry from the kind network
docker network disconnect "kind" kind-registry

# Stop the Docker registry
docker stop kind-registry

# Remove the Docker registry container
docker rm kind-registry
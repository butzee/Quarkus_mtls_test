$microserviceApath = Join-Path -Path $PSScriptRoot -ChildPath ".."
# change directory
Set-Location -Path $microserviceApath

Write-Host ""
Write-Host " ############################################################## "
Write-Host " Building Microservice A as a Native Image..."
./mvnw clean install -Pnative --define quarkus.native.container-build=true

Write-Host ""
Write-Host " ############################################################## "
Write-Host " Creating Docker Image for Microservice A..."
docker build -f src/main/docker/Dockerfile.native-micro -t 127.0.0.1:5000/microservice-a:latest .
$microserviceBpath = Join-Path -Path $PSScriptRoot -ChildPath ".."
# change directory
Set-Location -Path $microserviceBpath

Write-Host ""
Write-Host " ############################################################## "
Write-Host " Building Microservice B as a Native Image..."
./mvnw clean install -Pnative --define quarkus.native.container-build=true

Write-Host ""
Write-Host " ############################################################## "
Write-Host " Creating Docker Image for Microservice B..."
docker build -f src/main/docker/Dockerfile.native-micro -t 127.0.0.1:5000/microservice-b:latest .
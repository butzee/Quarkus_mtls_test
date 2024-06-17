$microserviceCpath = Join-Path -Path $PSScriptRoot -ChildPath ".."
# change directory
Set-Location -Path $microserviceCpath

Write-Host ""
Write-Host " ############################################################## "
Write-Host " Creating Docker Image for Microservice C..."
docker build -f Dockerfile -t 127.0.0.1:5000/microservice-c:latest .
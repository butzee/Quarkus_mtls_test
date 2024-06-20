$microserviceDpath = Join-Path -Path $PSScriptRoot -ChildPath ".."
# change directory
Set-Location -Path $microserviceDpath

Write-Host ""
Write-Host " ############################################################## "
Write-Host " Creating Docker Image for Microservice D..."
docker build -f Dockerfile -t 127.0.0.1:5000/microservice-d:latest .
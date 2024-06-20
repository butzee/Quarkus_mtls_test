$microserviceDpath = Join-Path -Path $PSScriptRoot -ChildPath ".."
# change directory
Set-Location -Path $microserviceDpath

Write-Host ""
Write-Host " ############################################################## "
Write-Host " Tagging Docker Image for Microservice D.."
docker tag 127.0.0.1:5000/microservice-d:latest 127.0.0.1:5000/microservice-d:latest

Write-Host ""
Write-Host " ############################################################## "
Write-Host " Pushing Microservice D to local Registry.."
docker push 127.0.0.1:5000/microservice-d:latest
$microserviceCpath = Join-Path -Path $PSScriptRoot -ChildPath ".."
# change directory
Set-Location -Path $microserviceCpath

Write-Host ""
Write-Host " ############################################################## "
Write-Host " Tagging Docker Image for Microservice C.."
docker tag 127.0.0.1:5000/microservice-c:latest 127.0.0.1:5000/microservice-c:latest

Write-Host ""
Write-Host " ############################################################## "
Write-Host " Pushing Microservice C to local Registry.."
docker push 127.0.0.1:5000/microservice-c:latest
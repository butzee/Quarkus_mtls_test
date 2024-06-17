$microserviceBpath = Join-Path -Path $PSScriptRoot -ChildPath ".."
# change directory
Set-Location -Path $microserviceBpath

Write-Host ""
Write-Host " ############################################################## "
Write-Host " Tagging Docker Image for Microservice B.."
docker tag 127.0.0.1:5000/microservice-b:latest 127.0.0.1:5000/microservice-b:latest

Write-Host ""
Write-Host " ############################################################## "
Write-Host " Pushing Microservice B to local Registry.."
docker push 127.0.0.1:5000/microservice-b:latest
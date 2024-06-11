$microserviceAhelmPackagepath = Join-Path -Path $PSScriptRoot -ChildPath "../helmPackage"
Set-Location -Path $microserviceAhelmPackagepath

Write-Host ""
Write-Host " ############################################################## "
Write-Host " Uninstalling Microservice A Helm Chart if it exists"
helm uninstall microservice-a

Write-Host ""
Write-Host " ############################################################## "
Write-Host " Packaging Helm Chart for Microservice A..."
helm package ../helmCharts

Write-Host ""
Write-Host " ############################################################## "
Write-Host " Installing Microservice A Helm Chart..."
helm install microservice-a microservice-a-0.1.0.tgz
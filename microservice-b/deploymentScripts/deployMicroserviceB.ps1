$microserviceBhelmPackagepath = Join-Path -Path $PSScriptRoot -ChildPath "../helmPackage"
Set-Location -Path $microserviceBhelmPackagepath

Write-Host ""
Write-Host " ############################################################## "
Write-Host " Uninstalling Microservice B Helm Chart if it exists"
helm uninstall microservice-b

Write-Host ""
Write-Host " ############################################################## "
Write-Host " Packaging Helm Chart for Microservice B..."
helm package ../helmCharts

Write-Host ""
Write-Host " ############################################################## "
Write-Host " Installing Microservice B Helm Chart..."
helm install microservice-b microservice-b-0.1.0.tgz
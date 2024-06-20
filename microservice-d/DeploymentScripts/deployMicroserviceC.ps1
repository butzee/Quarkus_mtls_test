Set-Location -Path $PSScriptRoot

Write-Host ""
Write-Host " ############################################################## "
Write-Host " Uninstalling Microservice D Helm Chart if it exists"
helm uninstall microservice-d

Write-Host ""
Write-Host " ############################################################## "
Write-Host " Packaging Helm Chart for Microservice D..."
helm package ../helmCharts

Write-Host ""
Write-Host " ############################################################## "
Write-Host " Installing Microservice D Helm Chart..."
helm install microservice-d microservice-d-0.1.0.tgz
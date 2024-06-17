Set-Location -Path $PSScriptRoot

Write-Host ""
Write-Host " ############################################################## "
Write-Host " Uninstalling Microservice C Helm Chart if it exists"
helm uninstall microservice-c

Write-Host ""
Write-Host " ############################################################## "
Write-Host " Packaging Helm Chart for Microservice C..."
helm package ../helmCharts

Write-Host ""
Write-Host " ############################################################## "
Write-Host " Installing Microservice C Helm Chart..."
helm install microservice-c microservice-c-0.1.0.tgz
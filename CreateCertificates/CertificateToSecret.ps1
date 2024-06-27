Set-Location -Path $PSScriptRoot
# Base directory for certificates and keys
$OutputDir = "..\microservice-needed-files\certs"

# Check if the output directory exists and if not, create it
if (-not (Test-Path $OutputDir)) {
    New-Item -ItemType Directory -Path $OutputDir
}


# Mapping services to their namespaces
$NamespaceMap = @{
    "kafka" = "default"
    "microservice-a" = "default"
    "microservice-b" = "default"
    "microservice-c" = "default"
}

# Define service specific files - Truststore and Keystore for each service
$ServiceFiles = @{
    "kafka" = @("kafka.truststore.jks", "kafka.keystore.jks")
    "microservice-a" = @("microservice-a.keystore.jks", "microservice-a.truststore.jks")
    "microservice-b" = @("microservice-b.keystore.jks", "microservice-b.truststore.jks")
    "microservice-c" = @("microservice-c.key", "microservice-c.pem", "ca.crt")
}

# Iterate over each service
foreach ($Service in $ServiceFiles.Keys) {
    $Namespace = $NamespaceMap[$Service]
    $SecretName = "certs-$Service"

    # Check and create namespace if not exists
    if (-not (kubectl get namespace $Namespace -o json | Out-String)) {
        kubectl create namespace $Namespace
    }

    # Prepare the files argument for the secret
    $FilesArgument = $ServiceFiles[$Service] | ForEach-Object {
        "--from-file=$_=$OutputDir\$_"
    }

    # Delete existing secret if it exists
    kubectl delete secret $SecretName --namespace $Namespace --ignore-not-found

    # Create new secret with multiple files
    $CreateSecretCommand = "kubectl create secret generic $SecretName " + $FilesArgument -join " " + " --namespace $Namespace"
    Invoke-Expression $CreateSecretCommand
}

Write-Host "Kubernetes secrets creation completed."
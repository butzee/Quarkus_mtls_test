Set-Location -Path $PSScriptRoot
helm install kafka bitnami/kafka -f kubernetesValues.yaml
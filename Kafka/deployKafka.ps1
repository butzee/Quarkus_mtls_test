Set-Location -Path $PSScriptRoot
helm uninstall kafka
helm install kafka bitnami/kafka -f kubernetesValues.yaml
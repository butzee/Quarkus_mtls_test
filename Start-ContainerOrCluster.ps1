# Script to ask the user for input and execute based on the choice

# Display the options and prerequisites
Write-Host "Before you start, make sure you have the following prerequisites:"
Write-Host "- Docker Desktop and Docker Compose installed and running"
Write-Host "- kind installed"
Write-Host "- kubectl installed"
Write-Host "- Helm installed"
Write-Host "- atleast Java 11 installed"
Write-Host "- Quarkus 2.15.3 installed"
Write-Host "- Maven installed"
Write-Host ""
Write-Host " Furthermore, make sure you have the following files in the 'certs' directory:"
Write-Host "- kafka.truststore.jks"
Write-Host "- kafka.keystore.jks"
Write-Host "- microservice-a.keystore.jks"
Write-Host "- microservice-a.truststore.jks"
Write-Host "- microservice-b.keystore.jks"
Write-Host "- microservice-b.truststore.jks"
Write-Host ""
Write-Host "The 'certs' directory should be located in the root directory of the project."
Write-Host "They are required for setting up the Kafka and Microservices in Kubernetes Cluster."
Write-Host ""
Write-Host "Therefore execute the 'CertificateCreation/Certificate.sh' script before you start."
Write-Host "This script creates the certificates and keys for the Kafka and Microservices."
Write-Host ""
Write-Host "After you have the prerequisites, you can select one of the following options:"
Write-Host "1. Kafka in Docker Container und Microservices via Quarkus dev locally"
Write-Host "2. Deploy Kubernetes and local Registry. Add Secrets to Cluster."
Write-Host "3. Build Microservices."
Write-Host "4. Push Microservices to local Registry."
Write-Host "5. Deploy Kafka to Kubernetes Cluster."
Write-Host "6. Deploy Microservices to Kubernetes Cluster."
Write-Host "7. Stop and remove Kubernetes Cluster and local Registry."
# Get user input
$userChoice = Read-Host "Please enter your choices (1 -7) separated by space (e.g., '2 3 4 5 6'). Press 'q' to quit. "

# Check if the user wants to quit
if ($userChoice -eq "q") {
    Write-Host "Exiting the script..."
    exit
}

# Split the user input into an array of choices
$choices = $userChoice -split ' '

# Iterate through each choice and execute the corresponding block of code
foreach ($choice in $choices) {
    switch ($choice) {
        "1" {
            # Action for Option 1
            Write-Host "You selected Option 1. Executing actions ..."
            Set-Location -Path $PSScriptRoot
            # Deploy Kafka in Docker Container
            Write-Host "Deploying Kafka in Docker Container..."
            docker-compose -f Kafka/docker-compose-kafka.yaml up -d

            # Deploy Microservices via Quarkus dev locally
            Write-Host "Now open a new terminal and navigate to the 'microservice-a' directory."
            Write-Host "Execute the following command to start the Quarkus dev mode for 'microservice-a': .\mvnw quarkus:dev"
            Write-Host "Open another terminal and navigate to the 'microservice-b' directory."
            Write-Host "Execute the following command to start the Quarkus dev mode for 'microservice-b': .\mvnw quarkus:dev"
            Write-Host "After starting the Quarkus dev mode for both microservices, you can test the communication between the microservices and Kafka."
            Write-Host ""
            Write-Host "If you want to see the logs from the Kafka container, execute the following command inside a new terminal: docker logs -f kafka"
            Write-Host "Open your web browser and navigate to the following URL: http://localhost:8080/produce/"
            Write-Host "You should see a message 'Hello from Quarkus REST!'"
            Write-Host "Furthermore, you can check the logs in the terminal where the Quarkus dev mode is running. You should see the messages being produced and consumed."
            Write-Host ""
            # Wait till user presses a key to shutdown the Kafka container
            Write-Host "Press any key to shutdown the Kafka container..."
            $x = $host.UI.RawUI.ReadKey("NoEcho,IncludeKeyDown")
            docker-compose -f Kafka/docker-compose-kafka.yaml down -v
        }
        "2" {
            # Action for Option 2
            Write-Host "You selected Option 2. Executing actions..."
            Set-Location -Path $PSScriptRoot
            # Add your specific execution code for Option 2 here
            Write-Host "Creating a local Registry and a Kubernetes Cluster via kind..."
            KubernetesConfig/startRegistryAndKubernetes.ps1
            Set-Location -Path $PSScriptRoot
            Write-Host "Kubernetes Cluster and local Registry created successfully."
            Write-Host ""
            Write-Host "Creating Secrets for Kafka and Microservices in Kubernetes Cluster..."
            CertificateCreation/CertificateToSecret.ps1
            Set-Location -Path $PSScriptRoot
        }
        "3" {
            Set-Location -Path $PSScriptRoot
            Write-Host "Now building all Microservices..."
            Write-Host "Building Microservice A..."
            microservice-a/deploymentScripts/buildImage.ps1
            Set-Location -Path $PSScriptRoot
            Write-Host "Building Microservice B..."
            microservice-b/deploymentScripts/buildImage.ps1
            Set-Location -Path $PSScriptRoot
            Write-Host "Building Microservice C..."
            microservice-c/deploymentScripts/buildImage.ps1
            Set-Location -Path $PSScriptRoot
            Write-Host "Microservices built."
        }
        "4" {
            Set-Location -Path $PSScriptRoot
            Write-Host "Now pushing both Microservices to local registry..."
            Write-Host "Pushing Microservice A..."
            microservice-a/deploymentScripts/pushImage.ps1
            Set-Location -Path $PSScriptRoot
            Write-Host "Pushing Microservice B..."
            microservice-b/deploymentScripts/pushImage.ps1
            Set-Location -Path $PSScriptRoot
            Write-Host "Pushing Microservice C..."
            microservice-c/deploymentScripts/pushImage.ps1
            Set-Location -Path $PSScriptRoot
            Write-Host "Microservices pushed successfully."
        }
        "5" {
            Set-Location -Path $PSScriptRoot
            Write-Host "Deploying Kafka..."
            Kafka/deployKafka.ps1
            Set-Location -Path $PSScriptRoot
            Write-Host "Waiting for Kafka to be ready..."
            Start-Sleep -s 90
            Write-Host "Kafka deployed successfully."
            Set-Location -Path $PSScriptRoot
        }
        "6" {
            Set-Location -Path $PSScriptRoot
            Write-Host "Deploying Microservice A..."
            microservice-a/deploymentScripts/deployMicroserviceA.ps1
            Set-Location -Path $PSScriptRoot
            Write-Host "Deploying Microservice B..."
            microservice-b/deploymentScripts/deployMicroserviceB.ps1
            Set-Location -Path $PSScriptRoot
            Write-Host "Deploying Microservice C..."
            microservice-c/deploymentScripts/deployMicroserviceC.ps1
            Set-Location -Path $PSScriptRoot
        }
        "7" {
            Set-Location -Path $PSScriptRoot
            Write-Host "Stop and remove the local Registry and Kubernetes Cluster..."
            KubernetesConfig/stopRegistryAndKubernetes.ps1
            Set-Location -Path $PSScriptRoot
            Write-Host "Local Registry and Kubernetes Cluster stopped and removed successfully."
        }
    default {
        Write-Host "Invalid choice: $choice. Please run the script again and select a valid option."
    }
}
}
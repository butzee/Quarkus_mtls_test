###########################################
# Prepare the environment for the project #
###########################################

function New-Certificates {
    Write-Host "Creating Certificates..."
    docker build -t create-certificates -f "CreateCertificates/Dockerfile" .\CreateCertificates\.; 
    if ($?) { docker run -v "${pwd}\microservice-needed-files\certs:/usr/app/certs" create-certificates}
}

function New-Libraries {
    Write-Host "Creating Libraries..."
    docker build -t New-libraries -f "CreateLibraries/." .;
    if ($?) { docker run --rm New-libraries }
}

###########################################
#              Microservices              #
###########################################

function New-Quarkus-Microservice {
    param (
        [string]$microservicePath,
        [string]$imageName
    )

    Set-Location -Path $microservicePath

    Write-Host " Building $imageName as a Native Image..."
    ./mvnw clean install -Pnative --define quarkus.native.container-build=true
}
function New-DockerImage {
    param (
        [string]$microservicePath,
        [string]$dockerfilePath,
        [string]$imageName
    )

    Set-Location -Path $microservicePath
    Write-Host "Creating Docker Image for $imageName..."
    docker build -f $dockerfilePath -t 127.0.0.1:5000/${imageName}:latest .
}
function New-ImageSelectedMicroservices {
    param (
        [string[]]$serviceIdentifiers
    )
    foreach ($id in $serviceIdentifiers) {
        $microservicePath = Join-Path -Path $PSScriptRoot -ChildPath $id
        if (@("microservice-a", "microservice-b") -contains $id) {
            New-Quarkus-Microservice -microse1rvicePath $microservicePath -imageName $id
            New-DockerImage -microservicePath $microservicePath -dockerfilePath "src/main/docker/Dockerfile.native-micro" -imageName $id 
        } elseif (@("microservice-c") -contains $id) {
            New-DockerImage -microservicePath $microservicePath -dockerfilePath "Dockerfile_Kubernetes" -imageName $id
        } else {
            Write-Host "The following value is not supported: $id"
        }
    }
}

function Push-SelectedMicroservices {
    param(
        [string[]]$serviceIdentifiers
    )

    foreach ($id in $serviceIdentifiers) {
        if (@("microservice-a", "microservice-b", "microservice-c") -contains $id) {
            $fullImagePath = "127.0.0.1:5000/${id}:latest"
            Write-Host "Pushing $id to local Registry..."
            docker push $fullImagePath
        } else {
            Write-Host "The following value is not supported: $id"
        }
    }
}

function Publish-SelectedMicroservices {
    param(
        [string[]]$serviceIdentifiers 
    )
    foreach ($id in $serviceIdentifiers) {
        if (@("microservice-a", "microservice-b", "microservice-c") -contains $id) {
                $microservicePath = Join-Path -Path $PSScriptRoot -ChildPath $id
                Set-Location -Path $microservicePath
                Write-Host "Uninstalling $id Helm Chart if it exists..."
                helm uninstall $id
                Write-Host "Packaging Helm Chart for $id..."
                helm package helmCharts -d helmPackage
                Write-Host "Installing $id Helm Chart..."
                helm install $id helmPackage/${id}-0.1.0.tgz
        } else {
            Write-Host "The following value is not supported: $id"
        } 
    }
}

function Uninstall-Microservices {
    param(
        [string[]]$serviceIdentifiers
    )
    foreach ($id in $serviceIdentifiers) {
        if (@("microservice-a", "microservice-b", "microservice-c") -contains $id) {
            helm uninstall $id
        } else {
            Write-Host "The following value is not supported: $id"
        }
    }
}

###########################################
#              Infrastructure             #
###########################################

function New-KubernetesCluster {
    Set-Location -Path $PSScriptRoot
    Write-Host "Creating a local Registry and a Kubernetes Cluster via kind..."
    KubernetesConfig/startRegistryAndKubernetes.ps1
}

function Add-SecretsToKubernetesCluster {
    Set-Location -Path $PSScriptRoot
    Write-Host "Creating Secrets for Kafka and Microservices in Kubernetes Cluster..."
    CreateCertificates/CertificateToSecret.ps1
}

function New-KafkaToKubernetesCluster {
    Set-Location -Path $PSScriptRoot
    Write-Host "Deploying Kafka to Kubernetes Cluster..."
    Write-Host "Uninstalling Kafka Helm Chart if it exists..."
    helm uninstall kafka
    Write-Host "Packaging Helm Chart for Kafka..."
    helm package Kafka/helmCharts -d Kafka/helmPackage
    Write-Host "Installing Kafka Helm Chart..."
    helm install kafka Kafka/helmPackage/kafka-0.1.0.tgz
}

function Remove-KubernetesCluster {
    Set-Location -Path $PSScriptRoot
    Write-Host "Deleting Kubernetes Cluster and local Registry..."
    KubernetesConfig/stopRegistryAndKubernetes.ps1
}
function Remove-KafkaFromCluster {
    Set-Location -Path $PSScriptRoot
    Write-Host "Deleting Kafka from Cluster..."
    helm uninstall kafka
}

function DeployKafkaToDocker {
    Write-Host "Deploying Kafka to Docker..."
    compose -f Kafka/docker-compose-kafka.yaml up -d
    Write-Host "Kafka is now running on localhost:9092"
    Write-Host @"
    To stop Kafka, run the following command:
    docker compose -f Kafka/docker-compose.yml down -v

    Furthermore you need to deploy microservice-a and microservice-b to use Kafka.
    Open for each microservice the terminal and run the following command in the root directory of each microservice:
        >> mvn quarkus:dev << 
    For Microservice C, run the following command in the root directory of the microservice:
        >> docker run -p 8080:8080
"@
}
###########################################
#             Helper Functions            #
###########################################

function Show-Menu {
    param (
        [string]$menuTitle,
        [string[]]$options
    )
    Write-Host ""
    Write-Host "$menuTitle"
    for ($i = 0; $i -lt $options.Length; $i++) {
        Write-Host "$($i + 1). $($options[$i])"
    }
}

function Get-UserChoice {
    param (
        [int]$optionCount
    )
    $choice = Read-Host "Enter your choice"
    if ($choice -match '^\d+$' -and [int]$choice -ge 1 -and [int]$choice -le $optionCount) {
        return [int]$choice
    } else {
        Write-Host "Invalid choice, please try again."
        return $null
    }
}

###########################################
#        Prerequisites Functions          #
###########################################

function Show-Project-Explaining {
    Clear-Host
    Write-Host @"
This project demonstrates a proof-of-concept (POC) microservice architecture using Kafka with secure communication and computation within a Kubernetes cluster.
The architecture simulates a cloud environment and secure computing on sensitive data, specifically for calculating the premium value of car insurance.

It consists of four microservices (a, b, c, and d), with the following deployment and technology details:
- Microservices a, b, and c: Deployed inside a Kubernetes cluster (public cloud) and accessible via NodePort configuration.
- Microservice d: Running "on-premises" - locally inside a Docker-Container.

Technologies Used
- Microservices a and b: Built with Quarkus.
- Microservices c and d: Built with C++ and the SEAL library.
- Secure Communication: Inside the Kubernetes cluster using mTLS.
- Data Communication: Via Kafka topics.

Workflow
1. Microservice D:
   - Creates Seal context and SealData objects with unique IDs.
   - Sends SealData to microservice A via `/endpoint`.
   
2. Microservice A:
   - Receives SealData and produces it to the `topic-seal-data` Kafka topic.
   
3. Microservice C:
   - Listens to `topic-seal-data`, processes encrypted values, and computes the required data.
   - Produces results to the `topic-encrypted-premium` Kafka topic.
   
4. Microservice B:
   - Consumes results from `topic-encrypted-premium`.
   - Endpoint `/getId` allows querying the results by unique ID.

You can choose to execute the Project automatically or step-by-step.
Automatic execution is not recommended, as it may lead to errors.

Steps to execute the project via the menus:
1. Manage Prerequisites: Check and install the necessary prerequisites.
2. Manage Infrastructure:
    - Create a Kubernetes cluster and local registry
    - Create certificates
    - Add secrets to the Kubernetes cluster
    - Manage Kafka > Deploy Kafka to Kubernetes Cluster
3. Manage Microservices:
    - Build, push, and deploy microservices (a-c) to the Kubernetes cluster
    - Start Microservice D as on-premise service (Follow the instructions in the terminal)
"@
}

function Invoke-Master {
    New-Certificates
    New-KubernetesCluster
    Add-SecretsToKubernetesCluster
    New-KafkaToKubernetesCluster
    New-ImageSelectedMicroservices -serviceIdentifiers @("microservice-a", "microservice-b", "microservice-c")
    Push-SelectedMicroservices -serviceIdentifiers @("microservice-a", "microservice-b", "microservice-c")
    Publish-SelectedMicroservices -serviceIdentifiers @("microservice-a", "microservice-b", "microservice-c")
    Invoke-Microservice-D
}

function Show-Prerequisites {
    Clear-Host
    Write-Host @"
Before you start, make sure you have the following prerequisites:
- Docker Desktop and Docker Compose installed and running
- kind installed
- kubectl installed
- Helm installed
- at least Java 11 installed
- Quarkus 2.15.3 installed
- Maven installed

Beware that it will take some time to execute everything, as it will build the microservices, create certificates, and deploy the Kubernetes cluster.
"@
}

function Invoke-Infos-Master {
    do {
        Show-Menu -menuTitle "Select one of the following options:" -options @(
            "Show Prerequisites",
            "Show Project Explaining",
            "Execute everything automatically and pray that everything works fine (not recommended)", 
            "Back to Main Menu"
        )
        $choice = Get-UserChoice -optionCount 4
        switch ($choice) {
            1 { Show-Prerequisites }
            2 { Show-Project-Explaining }
            3 { Invoke-Master }
            4 { return }
        }
    } while ($choice -ne 4)
}

###########################################
#        Infrastructure Functions         #
###########################################

function Invoke-Infrastructure {
    do {
        Show-Menu -menuTitle "Infrastructure Management" -options @(
            "Create Kubernetes Cluster and local Registry",
            "Create Certificates",
            "Add Secrets to Kubernetes Cluster",
            "Manage Kafka",
            "Delete Kubernetes Cluster and local Registry",
            "Back to Main Menu"
        )
        $choice = Get-UserChoice -optionCount 6
        switch ($choice) {
            1 { New-KubernetesCluster }
            2 { New-Certificates }
            3 { Add-SecretsToKubernetesCluster }
            4 { Invoke-Kafka }
            5 { Remove-KubernetesCluster }
            6 { return }
        }
    } while ($choice -ne 6)
}

function Invoke-Kafka {
    do {
        Show-Menu -menuTitle "Kafka Management" -options @(
            "Deploy Kafka to Kubernetes Cluster",
            "Delete Kafka from Cluster",
            "Back to Infrastructure Menu"
        )
        $choice = Get-UserChoice -optionCount 3
        switch ($choice) {
            1 { New-KafkaToKubernetesCluster }
            2 { Remove-KafkaFromCluster }
            3 { return }
        }
    } while ($choice -ne 3)
}

###########################################
#        Microservices Functions          #
###########################################

function Invoke-Microservice-D {
    Set-Location -Path $PSScriptRoot
    docker build -t microservice-d -f microservice-d/Dockerfile microservice-d/.
    docker run -it --rm microservice-d
}

function Remove-CreatedResources {
    Set-Location -Path $PSScriptRoot
    Write-Host "Deleting certs..."

    Remove-Item -Path "microservice-needed-files\certs\*" -Recurse -Force

    Write-Host "Deleting created target resources..."
    Remove-Item -Path "microservice-a\target\*" -Recurse -Force
    Remove-Item -Path "microservice-b\target\*" -Recurse -Force

    Write-Host "Deleting Helm Charts..."
    Remove-Item -Path "microservice-a\helmPackage\*" -Recurse -Force
    Remove-Item -Path "microservice-b\helmPackage\*" -Recurse -Force
    Remove-Item -Path "microservice-c\helmPackage\*" -Recurse -Force
    Remove-Item -Path "microservice-d\helmPackage\*" -Recurse -Force

    Write-Host "Deleting Docker Images..."
    # Get all Docker images
    $allImages = docker images

    # Function to extract image IDs based on a given microservice pattern
    function Get-ImageIDs {
        param (
            [string]$pattern
        )
        return $allImages | Where-Object { $_ -match $pattern } | ForEach-Object { $_.Split(" ", [System.StringSplitOptions]::RemoveEmptyEntries)[2] }
    }

    # Get all image IDs for the specified microservices
    $imagesA = Get-ImageIDs "127.0.0.1:5000/microservice-a"
    $imagesB = Get-ImageIDs "127.0.0.1:5000/microservice-b"
    $imagesC = Get-ImageIDs "127.0.0.1:5000/microservice-c"

    # Combine all lists of image IDs
    $allImages = $imagesA + $imagesB + $imagesC + "microservice-d" + "create-certificates"

    foreach ($image in $allImages) {
        docker rmi $image
    }

    # Get all dangling image IDs
    $danglingImages = docker images -f "dangling=true" -q

    # Remove each dangling image
    foreach ($danglingImage in $danglingImages) {
        docker rmi $danglingImage
    }
    Write-Host "Resources cleaned up successfully."
}

function Invoke-Microservices {
    do {
        Show-Menu -menuTitle "Microservices Management" -options @(
            "Build Microservices",
            "Push Microservices to local Registry",
            "Deploy Microservices to Kubernetes Cluster",
            "Start Microservice D for as On-Premise Service"
            "Uninstall Microservices from Kubernetes Cluster",
            "Back to Main Menu"
        )
        $choice = Get-UserChoice -optionCount 6
        switch ($choice) {
            1 {
                $services = Read-Host "Enter microservices to build (separate by space - e.g. 'microservice-a microservice-b')"
                New-ImageSelectedMicroservices -serviceIdentifiers ($services -split ' ')
            }
            2 {
                $services = Read-Host "Enter microservices to tag and push (separate by space - e.g. 'microservice-a microservice-b')"
                Push-SelectedMicroservices -serviceIdentifiers ($services -split ' ')
            }
            3 {
                $services = Read-Host "Enter microservices to deploy (separate by space - e.g. 'microservice-a microservice-b')"
                Publish-SelectedMicroservices -serviceIdentifiers ($services -split ' ')
            }
            4 {
                Invoke-Microservice-D
            }
            5 {
                $services = Read-Host "Enter microservices to uninstall (separate by space - e.g. 'microservice-a microservice-b')"
                Uninstall-Microservices -serviceIdentifiers ($services -split ' ')
            }
            6 { return }
        }
    } while ($choice -ne 6)
}

###########################################
#             Main Menu                   #
###########################################

function Show-MainMenu {
    Show-Menu -menuTitle "Main Menu:" -options @(
        "Get Information about the Project and Prerequisites",
        "Manage Infrastructure",
        "Manage Microservices",
        "Clean up running Resources",
        "Clean up Docker Images and Containers",
        "Exit"
    )
}

function Main {
    while ($true) {
        Show-MainMenu
        $choice = Get-UserChoice -optionCount 6
        switch ($choice) {
            1 { Invoke-Infos-Master }
            2 { Invoke-Infrastructure }
            3 { Invoke-Microservices }
            4 { Remove-KubernetesCluster }
            5 { Remove-CreatedResources }
            6 {
                Write-Host "Exiting the script..."
                exit
            }
        }
    }
}

###########################################
#             Entry Point                 #
###########################################

Main
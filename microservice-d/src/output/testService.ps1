# Read the JSON file content
$jsonFilePath = "seal_data.json"

# Read the JSON file with UTF-8 encoding and convert it to a PowerShell object
try {
    $jsonData = Get-Content -Path $jsonFilePath -Encoding UTF8 | ConvertFrom-Json
} catch {
    Write-Output "Failed to load or parse the JSON file: $_"
    exit
}

# Prompt user to choose sending one entry or all entries
$choice = Read-Host "Press 'A' to send all entries or 'O' to send one entry based on the Unique ID"

# Prepare the data to send
$dataToSend = @{}

# Process choice
switch ($choice.ToUpper()) {
    'A' {
        # Send all entries
        $dataToSend.data = $jsonData.data
    }
    'O' {
        # Display available Unique IDs
        Write-Output "Available Unique IDs in JSON: $($jsonData.data.UniqueId -join ', ')"
        $uniqueId = Read-Host "Enter the Unique ID: "
        $selectedEntry = $jsonData.data | Where-Object { $_.UniqueId -eq $uniqueId }
        if ($selectedEntry) {
            $dataToSend.data = @($selectedEntry)
        } else {
            Write-Output "Unique ID '$uniqueId' not found in JSON data."
            exit 1
        }
    }
    default {
        Write-Output "Invalid choice. Please enter 'A' for all or 'O' for one."
        exit 1
    }
}

# Convert the entire structure to JSON
$jsonToSend = $dataToSend | ConvertTo-Json -Depth 100

# Make the REST API call
$response = Invoke-RestMethod -Uri "http://localhost:8080/endpoint" -Method Post -Body $jsonToSend -ContentType "application/json"
Write-Output $response
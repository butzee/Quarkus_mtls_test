# Define the path to the input file
$filePath = "base64.txt"

# Check if the file exists
if (-Not (Test-Path -Path $filePath)) {
    Write-Error "Failed to open file."
    exit 1
}

# Initialize variables
$parameters = ""
$relinKeys = ""
$galoisKeys = ""
$encryptedInput = ""
$encryptedPricing = ""

# Read the file line by line
$fileContent = Get-Content -Path $filePath

# Iterate over each line
foreach ($line in $fileContent) {
    # Trim leading and trailing whitespace
    $line = $line.Trim()

    # Skip empty lines and comments
    if ($line -eq "" -or $line.StartsWith("#")) {
        continue
    }

    # Check for each key and read the next line
    switch -Wildcard ($line) {
        "Parameters:" { $parameters = $fileContent[$fileContent.IndexOf($line) + 1] }
        "Relin Keys:" { $relinKeys = $fileContent[$fileContent.IndexOf($line) + 1] }
        "Galois Keys:" { $galoisKeys = $fileContent[$fileContent.IndexOf($line) + 1] }
        "Encrypted input:" { $encryptedInput = $fileContent[$fileContent.IndexOf($line) + 1] }
        "Encrypted pricing:" { $encryptedPricing = $fileContent[$fileContent.IndexOf($line) + 1] }
    }
}

# Create a hashtable to represent the JSON object
$jsonBody = @{
    Parameters       = $parameters
    RelinKeys        = $relinKeys
    GaloisKeys       = $galoisKeys
    EncryptedInput   = $encryptedInput
    EncryptedPricing = $encryptedPricing
}

# Convert hashtable to JSON
$jsonString = $jsonBody | ConvertTo-Json

# Define the URL (Correctly formatted)
$url = "http://localhost:8080/produce"

Write-Output "Attempting to send data to $url"

# Calculate the size of the JSON string
$requestBodySize = $jsonString.Length
Write-Output "Request body size: $requestBodySize bytes"
# to MB 
$requestBodySizeMB = $requestBodySize / 1MB
Write-Output "Request body size: $requestBodySizeMB MB"


try {
    # Send the JSON data via HTTP POST
    $response = Invoke-RestMethod -Uri $url -Method Post -Body $jsonString -ContentType "application/json"
    
    Write-Output "Response from server:"
    Write-Output $response
} catch [System.Net.WebException] {
    Write-Error "A WebException occurred. Details: $_"
    if ($_.Exception.Response) {
        $responseStream = $_.Exception.Response.GetResponseStream()
        $reader = New-Object System.IO.StreamReader($responseStream)
        $responseBody = $reader.ReadToEnd()
        Write-Error "Response body: $responseBody"
    }
} catch {
    Write-Error "An unexpected error occurred. Details: $_"
}

Write-Output "Shutting down."
exit 0

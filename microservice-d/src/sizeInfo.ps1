# Define the path to the text file
$filePath = "base64.txt"

# Function to get the size of a string in MB
function Get-StringSizeInMB {
    param (
        [string]$inputString
    )
    
    # Get the byte size of the string
    $byteSize = [System.Text.Encoding]::UTF8.GetByteCount($inputString)
    
    # Convert bytes to MB (1 MB = 1024 * 1024 bytes)
    $mbSize = $byteSize / (1024 * 1024)
    
    return $mbSize
}

# Read the file line by line
$lines = Get-Content -Path $filePath

# Iterate through each line and calculate its size in MB
foreach ($line in $lines) {
    $lineSizeMB = Get-StringSizeInMB -inputString $line
    Write-Output ("Size of line: {0:N6} MB" -f $lineSizeMB)
}
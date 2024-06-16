#!/bin/bash

# Variablen definieren
output_dir="../certs"
ca_key="$output_dir/ca.key"
ca_cert="$output_dir/ca.crt"
ca_subject="/C=DE/ST=Niedersachen/L=Hannover/O=CCITM/OU=IT/CN=CCITM-CA"
days_valid=3650
password="changeit"

# Namen und Konfigurationen für Zertifikate
declare -A services=(
    ["kafka"]="kafka.default.svc.cluster.local,DNS:kafka-controller-0.kafka-controller-headless.default.svc.cluster.local,DNS:kafka-controller-1.kafka-controller-headless.default.svc.cluster.local,DNS:kafka-controller-2.kafka-controller-headless.default.svc.cluster.local,DNS:localhost"
    ["microservice-a"]="microservice-a.default.svc.cluster.local, DNS:localhost"
    ["microservice-b"]="microservice-b.default.svc.cluster.local, DNS:localhost"
    ["microservice-c"]="microservice-c.default.svc.cluster.local, DNS:localhost"
)

# Ausgabeverzeichnis vorbereiten
rm -rf "$output_dir"
mkdir -p "$output_dir"

# Erstellen der CA
echo "Creating CA..."
openssl req -new -x509 -keyout "$ca_key" -out "$ca_cert" -days $days_valid -subj "$ca_subject" -passout pass:$password

# Funktion zum Erstellen von Keystore und Truststore für jeden Dienst
create_keystore_and_truststore() {
    local service=$1
    local service_san="${services[$service]}"
    local keystore_path="$output_dir/$service.keystore.jks"
    local truststore_path="$output_dir/$service.truststore.jks"
    local csr_path="$output_dir/$service.csr"
    local signed_crt_path="$output_dir/$service-signed.crt"
    local pem_path="$output_dir/$service.pem"
    local key_path="$output_dir/$service.key"

    echo "Creating keystore and truststore for $service..."
    echo "subjectAltName=DNS:${service_san}"

    # Generate Keystore
    echo "Generating keystore..."
    keytool -genkeypair -alias "$service" -keyalg RSA -keysize 2048 -validity $days_valid -keystore "$keystore_path" -storepass $password -keypass $password -dname "CN=$service, OU=IT, O=CCITM, L=Hannover, ST=Niedersachsen, C=DE"
    echo "Keystore generated."
    echo "Generating CSR..."
    keytool -certreq -alias "$service" -keystore "$keystore_path" -file "$csr_path" -storepass $password -keypass $password
    echo "CSR generated."
    # Sign CSR with CA
    echo "Signing CSR with CA..."
    openssl x509 -req -CA "$ca_cert" -CAkey "$ca_key" -in "$csr_path" -out "$signed_crt_path" -days $days_valid -CAcreateserial -passin pass:$password -extfile <(echo "subjectAltName=DNS:${service_san}")
    echo "CSR signed."

    # Import Certificate into Keystore
    echo "Importing certificate into keystore..."
    keytool -importcert -alias CA -file "$ca_cert" -keystore "$keystore_path" -storepass $password -noprompt
    echo "CA certificate imported."
    echo "Importing signed certificate into keystore..."
    keytool -importcert -alias "$service" -file "$signed_crt_path" -keystore "$keystore_path" -storepass $password -keypass $password -noprompt
    echo "Signed certificate imported."

    # Create Truststore for the service and import CA certificate
    echo "Creating truststore for $service..."
    keytool -importcert -alias CA -file "$ca_cert" -keystore "$truststore_path" -storepass $password -noprompt
    echo "Truststore created."

    # Generate PEM and .key for microservice-c if applicable
    if [ "$service" == "microservice-c" ]; then
        echo "Exporting certificate to PEM format for $service..."
        openssl pkcs12 -in "$keystore_path" -out "$pem_path" -passin pass:$password -passout pass:$password -nokeys
        echo "PEM file exported."
        echo "Exporting private key for $service..."
        openssl pkcs12 -in "$keystore_path" -out "$key_path" -passin pass:$password -passout pass:$password -nocerts
        echo "Private key exported."
    fi

    # Remove temporary files
    rm "$csr_path" "$signed_crt_path"
}


# Zertifikate und Stores für jeden Dienst erstellen
for service in "${!services[@]}"; do
    create_keystore_and_truststore "$service"
done

echo "Keystore and truststore creation for all services completed."

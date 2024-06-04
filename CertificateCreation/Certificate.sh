#!/bin/bash


# Variablen definieren
OUTPUT_DIR="../certs"
CA_KEY="$OUTPUT_DIR/ca.key"
CA_CERT="$OUTPUT_DIR/ca.crt"
CA_SUBJECT="/C=DE/ST=Niedersachen/L=Hannover/O=CCITM/OU=IT/CN=CCITM-CA"

DAYS_VALID=3650

# Keystore- und Truststore-Variablen
PASSWORD="changeit"

BROKER_NAME="kafka-broker"
MICROSERVICE_A_NAME="microservice-a"
MICROSERVICE_B_NAME="microservice-b"

# Entferne Ausgabeverzeichnis, falls es existiert
rm -rf $OUTPUT_DIR
# Ausgabeverzeichnis erstellen, falls es nicht existiert
mkdir -p $OUTPUT_DIR

# Erstellen der CA. -keyout ist private Schlüssel und -out ist das Zertifikat
echo "Creating CA..."
openssl req -new -x509 -keyout $CA_KEY -out $CA_CERT -days $DAYS_VALID -subj "$CA_SUBJECT" -passout pass:$PASSWORD

# Funktion zum Erstellen von Keystore
create_keystore() {
    NAME=$1
    CN=$2

    echo "Creating keystore for $NAME..."

    # Keystore erstellen
    keytool -genkeypair -alias $NAME -keyalg RSA -keysize 2048 -validity $DAYS_VALID -keystore $OUTPUT_DIR/$NAME.keystore.jks -storepass $PASSWORD -keypass $PASSWORD -dname "CN=$CN, OU=IT, O=CCITM, L=Hannover, ST=Niedersachsen, C=DE"

    # Certificate Signing Request (CSR) erstellen
    keytool -certreq -alias $NAME -keystore $OUTPUT_DIR/$NAME.keystore.jks -file $OUTPUT_DIR/$NAME.csr -storepass $PASSWORD -keypass $PASSWORD

    # CSR mit der CA signieren
    openssl x509 -req -CA $CA_CERT -CAkey $CA_KEY -in $OUTPUT_DIR/$NAME.csr -out $OUTPUT_DIR/$NAME-signed.crt -days $DAYS_VALID -CAcreateserial -passin pass:$PASSWORD

    # CA-Zertifikat und signiertes Zertifikat in Keystore importieren
    keytool -importcert -alias CA -file $CA_CERT -keystore $OUTPUT_DIR/$NAME.keystore.jks -storepass $PASSWORD -noprompt
    keytool -importcert -alias $NAME -file $OUTPUT_DIR/$NAME-signed.crt -keystore $OUTPUT_DIR/$NAME.keystore.jks -storepass $PASSWORD -keypass $PASSWORD -noprompt

    # Temporäre Dateien löschen
    rm $OUTPUT_DIR/$NAME.csr $OUTPUT_DIR/$NAME-signed.crt
}

# CNs für Kafka-Broker und Microservices wenn Microservice in Quarkus:Dev und Kafka in Docker
# Muss mit Advertising-Listener der Docker-Compose übereinstimmen (SSL://localhost:9093)
BROKER_CN="localhost"

MICROSERVICE_A_CN="localhost"
MICROSERVICE_B_CN="localhost"

# Keystore für Kafka-Broker, Microservice-A und Microservice-B erstellen
create_keystore $BROKER_NAME "$BROKER_CN"
create_keystore $MICROSERVICE_A_NAME "$MICROSERVICE_A_CN"
create_keystore $MICROSERVICE_B_NAME "$MICROSERVICE_B_CN"

echo "Keystores creation completed."

# Truststore erstellen
echo "Creating truststore for Kafka-Broker, Microservice-A and Microservice-B..."
keytool -importcert -alias CA -file $CA_CERT -keystore $OUTPUT_DIR/truststore.jks -storepass $PASSWORD -noprompt
echo "Truststore creation completed."

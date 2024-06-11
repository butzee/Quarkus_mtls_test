#!/bin/bash

# Variablen definieren
OUTPUT_DIR="../certs"
CA_CERT="$OUTPUT_DIR/ca.crt"
PASSWORD="changeit"

BROKER_NAME="kafka-broker"
MICROSERVICE_A_NAME="microservice-a"
MICROSERVICE_B_NAME="microservice-b"

# Funktion zur Überprüfung von Keystore und Truststore
verify_keystore_truststore() {
    NAME=$1

    echo "Verifying keystore and truststore for $NAME..."

    # Inhalte des Keystores anzeigen
    echo "Contents of keystore $NAME.keystore.jks:"
    keytool -list -v -keystore $OUTPUT_DIR/$NAME.keystore.jks -storepass $PASSWORD

    # Inhalte des Truststores anzeigen
    echo "Contents of truststore $NAME.truststore.jks:"
    keytool -list -v -keystore $OUTPUT_DIR/$NAME.truststore.jks -storepass $PASSWORD

    # Zertifikatskette im Keystore anzeigen
    echo "Certificate chain in keystore $NAME.keystore.jks for alias $NAME:"
    keytool -list -v -keystore $OUTPUT_DIR/$NAME.keystore.jks -storepass $PASSWORD -alias $NAME

    # Gültigkeit des Zertifikats überprüfen
    echo "Verifying certificate for $NAME:"
    keytool -exportcert -alias $NAME -keystore $OUTPUT_DIR/$NAME.keystore.jks -storepass $PASSWORD -rfc -file $OUTPUT_DIR/$NAME.crt
    openssl verify -CAfile $CA_CERT $OUTPUT_DIR/$NAME.crt

    # Temporäre Zertifikat-Datei löschen
    rm $OUTPUT_DIR/$NAME.crt
}

# Keystore und Truststore für Kafka-Broker, Microservice-A und Microservice-B überprüfen
verify_keystore_truststore $BROKER_NAME
verify_keystore_truststore $MICROSERVICE_A_NAME
verify_keystore_truststore $MICROSERVICE_B_NAME

echo "Verification completed."

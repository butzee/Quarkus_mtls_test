package org.example;

import jakarta.json.bind.annotation.JsonbProperty;
import io.quarkus.runtime.annotations.RegisterForReflection;

@RegisterForReflection
public class SealData {
    @JsonbProperty("Parameters")
    private String parameters;

    @JsonbProperty("RelinKeys")
    private String relinKeys;

    @JsonbProperty("GaloisKeys")
    private String galoisKeys;

    @JsonbProperty("EncryptedInput")
    private String encryptedInput;

    @JsonbProperty("EncryptedPricing")
    private String encryptedPricing;


    public SealData() {

    }
    public SealData(String parameters) {
        this.parameters = parameters;
    }

    public SealData(String parameters, String relinKeys, String galoisKeys, String encryptedInput, String encryptedPricing) {
        this.parameters = parameters;
        this.relinKeys = relinKeys;
        this.galoisKeys = galoisKeys;
        this.encryptedInput = encryptedInput;
        this.encryptedPricing = encryptedPricing;
    }

    public String getParameters() {
        return parameters;
    }

    public void setParameters(String parameters) {
        this.parameters = parameters;
    }

    public String getRelinKeys() {
        return relinKeys;
    }

    public void setRelinKeys(String relinKeys) {
        this.relinKeys = relinKeys;
    }

    public String getGaloisKeys() {
        return galoisKeys;
    }

    public void setGaloisKeys(String galoisKeys) {
        this.galoisKeys = galoisKeys;
    }

    public String getEncryptedInput() {
        return encryptedInput;
    }

    public void setEncryptedInput(String encryptedInput) {
        this.encryptedInput = encryptedInput;
    }

    public String getEncryptedPricing() {
        return encryptedPricing;
    }

    public void setEncryptedPricing(String encryptedPricing) {
        this.encryptedPricing = encryptedPricing;
    }

    public void get (String data) {
        this.parameters = data;
    }

    @Override
    //only first 15 characters of each field are printed
    public String toString() {
        return "SealData{" +
            "parameters='" + parameters.substring(0, Math.min(parameters.length(), 15)) + '\'' +
            ", relinKeys='" + relinKeys.substring(0, Math.min(relinKeys.length(), 15)) + '\'' +
            ", galoisKeys='" + galoisKeys.substring(0, Math.min(galoisKeys.length(), 15)) + '\'' +
            ", encryptedInput='" + encryptedInput.substring(0, Math.min(encryptedInput.length(), 15)) + '\'' +
            ", encryptedPricing='" + encryptedPricing.substring(0, Math.min(encryptedPricing.length(), 15)) + '\'' +
            '}';

        }
}
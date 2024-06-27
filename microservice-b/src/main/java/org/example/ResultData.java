package org.example;

import jakarta.json.bind.annotation.JsonbProperty;
import io.quarkus.runtime.annotations.RegisterForReflection;

@RegisterForReflection
public class ResultData {
    @JsonbProperty("UniqueId")
    private String uniqueId;

    @JsonbProperty("EncryptedResult")
    private String encryptedResult;

    public ResultData() {

    }

    public ResultData(String encryptedResult, String uniqueId) {
        this.encryptedResult = encryptedResult;
        this.uniqueId = uniqueId;
    }

    public void setEncryptedResult(String encryptedResult) {
        this.encryptedResult = encryptedResult;
    }

    public String getEncryptedResult() {
        return encryptedResult;
    }

    public void setUniqueId(String uniqueId) {
        this.uniqueId = uniqueId;
    }

    public String getUniqueId() {
        return uniqueId;
    }

    public String toJson() {
        return "{\"UniqueId\":\"" + uniqueId + "\",\"EncryptedResult\":\"" + encryptedResult + "\"}";
    }

    public String toPrettyJson() {
        return "{\n" +
                "  \"UniqueId\": \"" + uniqueId + "\",\n" +
                "  \"EncryptedResult\": \"" + encryptedResult + "\"\n" +
                "}";
    }
    
    @Override
    //only first 15 characters of each field are printed
    public String toString() {
        return "ResultData{" +
                "uniqueId='" + uniqueId + '\'' +
                ", encryptedResult='" + encryptedResult.substring(0, Math.min(encryptedResult.length(), 15)) + '\''
                + '}';
    }
}
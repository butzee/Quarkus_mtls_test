package org.example;

import jakarta.enterprise.context.ApplicationScoped;

import java.io.File;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import org.eclipse.microprofile.reactive.messaging.Incoming;

import jakarta.json.Json;
import jakarta.json.JsonArray;
import jakarta.json.JsonArrayBuilder;
import jakarta.json.JsonObject;
import jakarta.json.JsonReader;
import jakarta.json.JsonWriter;
import jakarta.json.bind.Jsonb;
import jakarta.json.bind.JsonbBuilder;

@ApplicationScoped
public class EndpointConsumeResultData {

private static final String FILE_PATH = "resultData.json";

    @Incoming("encrypted-premium")
    public void receivePrice(String resultData) {
        Jsonb jsonb = JsonbBuilder.create();
        ResultData data = jsonb.fromJson(resultData, ResultData.class);
        saveToJsonFile(data);
    }

    public void saveToJsonFile(ResultData resultData) {
        File file = new File(FILE_PATH);
        JsonArray jsonArray = null;

        // Check if the file exists
        if (!file.exists()) {
            try {
                // If file does not exist, create a new file and a new JSON array
                file.createNewFile();
                jsonArray = Json.createArrayBuilder().build(); // Create an empty JSON array
            } catch (IOException e) {
                e.printStackTrace();
            }
        } else {
            // If file exists, read the file content into a JSON array
            try (JsonReader reader = Json.createReader(new FileReader(file))) {
                jsonArray = reader.readArray();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }

        // Determine if the UniqueId already exists in the JSON array
        JsonArrayBuilder arrayBuilder = Json.createArrayBuilder();
        boolean found = false;
        for (JsonObject jsonEntry : jsonArray.getValuesAs(JsonObject.class)) {
            if (jsonEntry.getString("UniqueId").equals(resultData.getUniqueId())) {
                // If UniqueId matches, replace the object with updated data
                JsonObject updatedObject = Json.createObjectBuilder()
                    .add("UniqueId", resultData.getUniqueId())
                    .add("EncryptedResult", resultData.getEncryptedResult())
                    .build();
                arrayBuilder.add(updatedObject);
                found = true;
            } else {
                // Otherwise, add the existing object to the new array
                arrayBuilder.add(jsonEntry);
            }
        }

        if (!found) {
            // If no matching UniqueId was found, add the new object to the array
            JsonObject newObject = Json.createObjectBuilder()
                .add("UniqueId", resultData.getUniqueId())
                .add("EncryptedResult", resultData.getEncryptedResult())
                .build();
            arrayBuilder.add(newObject);
        }

        // Write the JSON array back to the file
        try (JsonWriter writer = Json.createWriter(new FileWriter(file))) {
            writer.writeArray(arrayBuilder.build());
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}

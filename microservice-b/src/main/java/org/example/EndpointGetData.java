package org.example;

import java.io.File;
import java.io.FileReader;
import java.io.IOException;

import jakarta.json.Json;
import jakarta.json.JsonArray;
import jakarta.json.JsonObject;
import jakarta.json.JsonReader;
import jakarta.ws.rs.GET;
import jakarta.ws.rs.Path;
import jakarta.ws.rs.Produces;
import jakarta.ws.rs.QueryParam;
import jakarta.ws.rs.core.MediaType;

@Path("/getId")
public class EndpointGetData{

private static final String FILE_PATH = "resultData.json";

    @GET
    @Produces(MediaType.TEXT_PLAIN)
    public String getData(@QueryParam("uniqueId") String uniqueId) {
        File file = new File(FILE_PATH);
        JsonArray jsonArray = null;
        
        if (!file.exists()) {
            return "Unique Id not found.";
        } else {
            // If file exists, read the file content into a JSON array
            try (JsonReader reader = Json.createReader(new FileReader(file))) {
                jsonArray = reader.readArray();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }

        boolean found = false;
        for (JsonObject jsonEntry : jsonArray.getValuesAs(JsonObject.class)) {
            if (jsonEntry.getString("UniqueId").equals(uniqueId)) {
                return jsonEntry.toString();
            } 
        }

        if (!found) {
            return "Unique Id not found.";
        }

        return uniqueId + "not found !";
    }
}
package org.example;

import jakarta.ws.rs.Consumes;
import jakarta.ws.rs.POST;
import jakarta.ws.rs.Path;
import jakarta.ws.rs.Produces;
import jakarta.ws.rs.core.MediaType;
import jakarta.ws.rs.core.Response;

import java.util.ArrayList;
import java.util.List;

import io.micrometer.core.instrument.MeterRegistry;
import jakarta.inject.Inject;
import jakarta.json.JsonObject;
import jakarta.json.bind.Jsonb;
import jakarta.json.bind.JsonbBuilder;
import io.micrometer.core.instrument.Timer;

@Path("/endpoint")
public class EndpointResource {

    @Inject
    MeterRegistry meterRegistry;

    @Inject
    EndpointProducer endpointProducer;

    @POST
    @Produces(MediaType.TEXT_PLAIN) 
    @Consumes(MediaType.APPLICATION_JSON)
    public Response processJsonEntries(JsonObject jsonData) {
        Timer timer = meterRegistry.timer("process.json.entries.timer");

        Jsonb jsonb = JsonbBuilder.create();

        // Measure the time taken to process the entries
        timer.record(() -> {
            // Assume jsonData contains a JSON array under the key "data"
            String jsonArray = jsonData.getJsonArray("data").toString();

            // Deserialize JSON array to List of SealData objects
            List<SealData> sealDataList = jsonb.fromJson(jsonArray, new ArrayList<SealData>(){}.getClass().getGenericSuperclass());

            // Process each SealData object
            for (SealData sealData : sealDataList) {
                System.out.println("SealData: " + sealData.toString());
                endpointProducer.sendSealData(sealData);
            }
        });

        return Response.ok("JSON received and processed successfully").build();
    }
}
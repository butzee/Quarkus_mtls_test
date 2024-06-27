package org.example;

import jakarta.enterprise.context.ApplicationScoped;
import jakarta.inject.Inject;
import org.eclipse.microprofile.reactive.messaging.Channel;
import org.eclipse.microprofile.reactive.messaging.Emitter;

import jakarta.json.bind.Jsonb;
import jakarta.json.bind.JsonbBuilder;
import jakarta.json.bind.JsonbConfig;

@ApplicationScoped
public class EndpointProducer {

    @Inject
    @Channel("seal-data")
    Emitter<String> dataEmitter;
    
    private Jsonb jsonb = JsonbBuilder.create(new JsonbConfig().withFormatting(true));

    public void sendSealData(SealData sealData) {
        if (dataEmitter != null) {
            System.out.println("Seal data contents: " + sealData.toString());
            String jsonSealData = jsonb.toJson(sealData);
            dataEmitter.send(jsonSealData);
        } else {
            System.err.println("dataEmitter is null");
        }
    }
}
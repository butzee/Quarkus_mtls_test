package org.example;

import jakarta.enterprise.context.ApplicationScoped;
import jakarta.inject.Inject;
import org.eclipse.microprofile.reactive.messaging.Channel;
import org.eclipse.microprofile.reactive.messaging.Emitter;
import jakarta.json.bind.Jsonb;
import jakarta.json.bind.JsonbBuilder;
import jakarta.json.bind.JsonbConfig;

@ApplicationScoped
public class PriceProducer {

    @Inject
    @Channel("generated-price")
    Emitter<String> priceEmitter;
    
    private Jsonb jsonb = JsonbBuilder.create(new JsonbConfig().withFormatting(true));

    public void sendPrice(SealData sealData) {
        if (priceEmitter != null) {
            System.out.println("Seal data contents: " + sealData.toString());
            String jsonSealData = jsonb.toJson(sealData);
            priceEmitter.send(jsonSealData);
        } else {
            System.err.println("priceEmitter is null");
        }
    }
}

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

    public void sendPrice(PriceData priceData) {
        System.out.println("Sending Price data 2");
        if (priceEmitter != null) {
            System.out.println("priceEmitter is not null");
            System.out.println("Price data: " + priceData); 
            System.out.println("Price data contents: " + priceData.getData());
            String jsonPrice = jsonb.toJson(priceData);
            System.out.println("Serialized JSON: " + jsonPrice);
            priceEmitter.send(jsonPrice);
        } else {
            System.err.println("priceEmitter is null");
        }
    }
}

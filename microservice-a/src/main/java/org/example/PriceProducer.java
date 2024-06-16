package org.example;

import jakarta.enterprise.context.ApplicationScoped;
import jakarta.inject.Inject;
import org.eclipse.microprofile.reactive.messaging.Channel;
import org.eclipse.microprofile.reactive.messaging.Emitter;
import jakarta.json.bind.Jsonb;
import jakarta.json.bind.JsonbBuilder;

@ApplicationScoped
public class PriceProducer {

    @Inject
    @Channel("generated-price")
    Emitter<String> priceEmitter;
    private Jsonb jsonb = JsonbBuilder.create();

    public void sendPrice(PriceData priceData) {
        if (priceEmitter != null) {
            String jsonPrice = jsonb.toJson(priceData);
            System.out.println("Sending price data: " + jsonPrice);
            priceEmitter.send(jsonPrice);
        } else {
            System.err.println("priceEmitter is null");
        }
    }
}

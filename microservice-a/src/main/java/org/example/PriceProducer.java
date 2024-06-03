package org.example;

import jakarta.enterprise.context.ApplicationScoped;
import jakarta.inject.Inject;
import org.eclipse.microprofile.reactive.messaging.Channel;
import org.eclipse.microprofile.reactive.messaging.Emitter;

@ApplicationScoped
public class PriceProducer {

    @Inject
    @Channel("generated-price")
    Emitter<String> priceEmitter;

    public void sendPrice(String price) {
        if (priceEmitter != null) {
            System.out.println("Sending price: " + price);
            priceEmitter.send(price);
            
        } else {
            System.err.println("priceEmitter is null");
        }
    }
}

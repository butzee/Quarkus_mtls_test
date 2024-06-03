package org.example;

import jakarta.enterprise.context.ApplicationScoped;
import org.eclipse.microprofile.reactive.messaging.Incoming;

@ApplicationScoped
public class PriceConsumer {

    @Incoming("generated-price")
    public void receivePrice(String price) {
        System.out.println("Received price: " + price);
    }
}

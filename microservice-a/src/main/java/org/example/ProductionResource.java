package org.example;

import jakarta.ws.rs.GET;
import jakarta.ws.rs.Path;
import jakarta.ws.rs.Produces;
import jakarta.ws.rs.core.MediaType;
import jakarta.inject.Inject;
import java.util.List;

@Path("/produce")
public class ProductionResource {

    @Inject
    PriceProducer priceProducer;

    @GET
    @Produces(MediaType.TEXT_PLAIN)
    public String hello() {
        PriceData priceData = new PriceData(List.of(2.0, 100.0, 2.0, 1.0));
        priceProducer.sendPrice(priceData);
        return "Hello from Quarkus REST";
    }
}

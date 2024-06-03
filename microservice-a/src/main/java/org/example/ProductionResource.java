package org.example;

import jakarta.ws.rs.GET;
import jakarta.ws.rs.Path;
import jakarta.ws.rs.Produces;
import jakarta.ws.rs.core.MediaType;
import jakarta.inject.Inject;

@Path("/produce")
public class ProductionResource {

    @Inject
    PriceProducer priceProducer;

    @GET
    @Produces(MediaType.TEXT_PLAIN)
    public String hello() {
        priceProducer.sendPrice("123");
        return "Hello from Quarkus REST";
    }
}

package org.example;

import jakarta.ws.rs.Consumes;
import jakarta.ws.rs.GET;
import jakarta.ws.rs.POST;
import jakarta.ws.rs.Path;
import jakarta.ws.rs.Produces;
import jakarta.ws.rs.core.MediaType;
import jakarta.ws.rs.core.Response;
import jakarta.inject.Inject;
import java.util.List;

@Path("/produce")
public class ProductionResource {

    @Inject
    PriceProducer priceProducer;

    @GET
    @Produces(MediaType.TEXT_PLAIN)
    public String hello() {
        System.out.println("Sending price data");
        PriceData priceData = new PriceData(List.of(2.0, 100.0, 2.0, 1.0));
        System.out.println("Price data: " + priceData);
        priceProducer.sendPrice(priceData);
        return "Hello from Quarkus REST";
    }

    // Invoke-WebRequest -Uri http://localhost:8080/produce -Method POST -Body '{"data":[2.0,100.0,2.0,1.0]}' -ContentType "application/json"
    @POST
    @Produces(MediaType.TEXT_PLAIN) 
    @Consumes(MediaType.APPLICATION_JSON)
    public Response processPrices(PriceData prices) {
        System.out.println("Received price data: " + prices);
        String response = "Total price is: " + prices;
        return Response.ok(response).build();
    }


}

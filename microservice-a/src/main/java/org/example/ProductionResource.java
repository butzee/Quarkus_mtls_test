package org.example;

import jakarta.ws.rs.Consumes;
import jakarta.ws.rs.GET;
import jakarta.ws.rs.POST;
import jakarta.ws.rs.Path;
import jakarta.ws.rs.Produces;
import jakarta.ws.rs.core.MediaType;
import jakarta.ws.rs.core.Response;
import jakarta.inject.Inject;

@Path("/produce")
public class ProductionResource {

    @Inject
    PriceProducer priceProducer;

       // Invoke-WebRequest -Uri http://localhost:8080/produce -Method POST -Body '{"data":[2.0,100.0,2.0,1.0]}' -ContentType "application/json"
    @POST
    @Produces(MediaType.TEXT_PLAIN) 
    @Consumes(MediaType.APPLICATION_JSON)
    public Response processPrices(SealData sealData) {
        System.out.println("Received data: " + sealData);
        priceProducer.sendPrice(sealData);
        String response = "Total price is: " + 5;
        return Response.ok(response).build();
    }


}
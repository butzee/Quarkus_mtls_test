package org.example;

import io.micrometer.core.instrument.MeterRegistry;
import io.micrometer.core.instrument.Counter;
import io.micrometer.core.instrument.Timer;
import jakarta.inject.Inject;
import jakarta.ws.rs.GET;
import jakarta.ws.rs.Path;
import jakarta.ws.rs.PathParam;

@Path("/")
public class PrimeNumberChecker {

    private long highestPrimeNumberSoFar = 2;

    @Inject
    MeterRegistry registry;

    private Counter primeCheckCounter;
    private Timer primeCheckTimer;

    public PrimeNumberChecker(MeterRegistry registry) {
        this.registry = registry;
        this.primeCheckCounter = registry.counter("prime.checks.total");
        this.primeCheckTimer = registry.timer("prime.checks.timer");
        registry.gauge("prime.highest", this, PrimeNumberChecker::getHighestPrimeNumberSoFar);
    }

    @GET
    @Path("/{number}")
    public String checkIfPrime(@PathParam("number") long number) {
        primeCheckCounter.increment();

        Timer timer = Timer.builder("process.prime.timer")
            .publishPercentileHistogram(true)
            .register(registry);
2
        return timer.record(() -> {
            try {
                Thread.sleep((long) (Math.random() * 4000) + 1000);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }

            System.out.println("Checking if " + number + " is prime.");
            if (number < 1) {
                return "Only natural numbers can be prime numbers.";
            }
            if (number == 1) {
                return "1 is not prime.";
            }
            if (number == 2) {
                return "2 is prime.";
            }
            if (number % 2 == 0) {
                return number + " is not prime, it is divisible by 2.";
            }
            for (int i = 3; i < Math.floor(Math.sqrt(number)) + 1; i = i + 2) {
                if (number % i == 0) {
                    return number + " is not prime, is divisible by " + i + ".";
                }
            }
            if (number > highestPrimeNumberSoFar) {
                highestPrimeNumberSoFar = number;
            }
            return number + " is prime.";
        });
        
    }

    public long getHighestPrimeNumberSoFar() {
        return highestPrimeNumberSoFar;
    }
}

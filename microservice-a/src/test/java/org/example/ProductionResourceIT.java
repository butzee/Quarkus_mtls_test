package org.example;

import io.quarkus.test.junit.QuarkusIntegrationTest;

@QuarkusIntegrationTest
class ProductionResourceIT extends ProductionResourceTest {
    // Execute the same tests but in packaged mode.
}

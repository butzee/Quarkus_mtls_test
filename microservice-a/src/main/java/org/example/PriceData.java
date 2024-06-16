package org.example;

import java.util.List;

import jakarta.json.bind.annotation.JsonbProperty;

public class PriceData {
    @JsonbProperty("data")
    private List<Double> data;

    public PriceData(List<Double> data) {
        this.data = data;
    }

    public List<Double> getData() {
        return data;
    }

    public void setData(List<Double> data) {
        this.data = data;
    }
}
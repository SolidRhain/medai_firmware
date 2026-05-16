#include <DHT.h>
#include "dht_sensor.h"
#include "config.h"

static DHT dht(DHT_PIN, DHT_TYPE);

void initDHT() {
    dht.begin();
    Serial.print("[DHT] DHT22 initialized on GPIO");
    Serial.println(DHT_PIN);
}

float readTemperature() {
    return dht.readTemperature();
}

float readHumidity() {
    return dht.readHumidity();
}

bool readDHT(float& temperature, float& humidity) {

    temperature = dht.readTemperature();
    humidity    = dht.readHumidity();

    if (isnan(temperature) || isnan(humidity)) {
        Serial.println("[DHT] Read FAILED — check wiring and pull-up resistor");
        return false;
    }

    Serial.print("[DHT] Temperature: ");
    Serial.print(temperature, 1);
    Serial.print(" °C  |  Humidity: ");
    Serial.print(humidity, 1);
    Serial.println(" %");

    return true;
}

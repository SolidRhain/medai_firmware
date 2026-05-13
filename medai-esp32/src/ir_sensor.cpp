#include <Arduino.h>
#include "ir_sensor.h"
#include "config.h"

void initIRSensor() {
    pinMode(IR_SENSOR_PIN, INPUT);
    Serial.print("[IR] Sensor initialized on GPIO");
    Serial.println(IR_SENSOR_PIN);
}

bool isPillPresent() {
    // IR module: LOW = beam broken = pill detected
    return digitalRead(IR_SENSOR_PIN) == IR_PILL_DETECTED;
}

bool waitForPillDrop(unsigned long timeoutMs) {

    Serial.print("[IR] Waiting for pill to drop (timeout: ");
    Serial.print(timeoutMs);
    Serial.println("ms)");

    unsigned long start = millis();

    while (millis() - start < timeoutMs) {

        if (isPillPresent()) {
            Serial.println("[IR] Pill detected!");
            return true;
        }

        delay(10);
    }

    Serial.println("[IR] Timeout — no pill detected");
    return false;
}

bool waitForPillPickup(unsigned long timeoutMs) {

    Serial.print("[IR] Waiting for person to pick up pill (timeout: ");
    Serial.print(timeoutMs / 1000);
    Serial.println("s)");

    unsigned long start = millis();

    while (millis() - start < timeoutMs) {

        if (!isPillPresent()) {
            Serial.println("[IR] Pill picked up");
            return true;
        }

        delay(100);
    }

    Serial.println("[IR] Timeout — pill not picked up");
    return false;
}

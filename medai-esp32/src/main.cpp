#include <Arduino.h>
#include <WiFiClientSecure.h>

#include "config.h"
#include "wifi_manager.h"
#include "time_manager.h"
#include "cert_manager.h"
#include "mqtt_manager.h"
#include "stepper.h"
#include "ir_sensor.h"
#include "inventory.h"

WiFiClientSecure secureClient;

unsigned long lastReconnectAttempt = 0;
unsigned long lastTelemetry        = 0;

// ----------------------------------------

void setup() {

    Serial.begin(115200);
    delay(2000);

    Serial.println();
    Serial.println("========================================");
    Serial.println("   MedAI Cabinet — Firmware v1.0");
    Serial.println("========================================");

    // Hardware
    initSteppers();
    initIRSensor();
    initInventory();

    // Network
    connectWiFi();
    syncTime();

    // Certificates
    bool certsOk = loadCertificates(secureClient);
    if (!certsOk) {
        Serial.println("[Main] HALTED: Certificate loading failed.");
        Serial.println("[Main] Run 'Upload Filesystem Image' in PlatformIO first.");
        while (true) { delay(1000); }
    }

    // MQTT
    setupMQTT(secureClient);
    connectMQTT();

    Serial.println();
    Serial.println("[Main] System Ready — waiting for commands");
    Serial.println("========================================");
}

// ----------------------------------------

void loop() {

    // Keep MQTT alive, reconnect if dropped
    if (!mqttClient.connected()) {

        unsigned long now = millis();

        if (now - lastReconnectAttempt > RECONNECT_INTERVAL_MS) {
            lastReconnectAttempt = now;
            Serial.println("[Main] Reconnecting MQTT...");
            connectMQTT();
        }

    } else {

        mqttLoop();
    }

    // Periodic telemetry (inventory ping)
    if (millis() - lastTelemetry > TELEMETRY_INTERVAL_MS) {
        lastTelemetry = millis();
        publishInventory();
    }

    delay(10);
}

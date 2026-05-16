#ifndef MQTT_MANAGER_H
#define MQTT_MANAGER_H

#include <WiFiClientSecure.h>
#include <PubSubClient.h>

extern PubSubClient mqttClient;

void setupMQTT(WiFiClientSecure& secureClient);
bool connectMQTT();
void mqttLoop();

void publishStatus(const char* status);
void publishAlert(const char* alertType, const char* detail);

// Send inventory counts only (called internally after dispense)
void publishInventory();

// Send full telemetry: inventory + DHT22 temperature & humidity
// Pass NAN for temp/humidity if sensor read failed
void publishTelemetry(float temperature, float humidity);

#endif

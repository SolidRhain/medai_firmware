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
void publishInventory();

#endif

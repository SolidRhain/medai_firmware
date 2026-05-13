#include <SPIFFS.h>
#include <WiFiClientSecure.h>
#include "cert_manager.h"
#include "config.h"

static String rootCA;
static String deviceCert;
static String privateKey;

static String readFile(const char* path) {

    File file = SPIFFS.open(path, "r");

    if (!file) {
        Serial.print("[Cert] FAILED TO OPEN: ");
        Serial.println(path);
        return "";
    }

    String content = file.readString();
    file.close();

    Serial.print("[Cert] Loaded: ");
    Serial.print(path);
    Serial.print(" (");
    Serial.print(content.length());
    Serial.println(" bytes)");

    return content;
}

static void listFiles() {

    Serial.println("[Cert] Files in SPIFFS:");
    File root = SPIFFS.open("/");
    File file = root.openNextFile();

    while (file) {
        Serial.print("       - ");
        Serial.println(file.name());
        file = root.openNextFile();
    }
}

bool loadCertificates(WiFiClientSecure& secureClient) {

    if (!SPIFFS.begin(true)) {
        Serial.println("[Cert] SPIFFS mount FAILED");
        return false;
    }

    Serial.println("[Cert] SPIFFS mounted");
    listFiles();

    rootCA     = readFile(PATH_ROOT_CA);
    deviceCert = readFile(PATH_DEVICE_CRT);
    privateKey = readFile(PATH_PRIVATE_KEY);

    if (rootCA.isEmpty() || deviceCert.isEmpty() || privateKey.isEmpty()) {
        Serial.println("[Cert] ERROR: One or more cert files are empty or missing");
        return false;
    }

    secureClient.setCACert(rootCA.c_str());
    secureClient.setCertificate(deviceCert.c_str());
    secureClient.setPrivateKey(privateKey.c_str());

    Serial.println("[Cert] All certificates loaded OK");
    return true;
}

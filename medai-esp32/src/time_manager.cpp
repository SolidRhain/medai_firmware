#include <time.h>
#include <Arduino.h>
#include "time_manager.h"

void syncTime() {

    configTime(0, 0, "pool.ntp.org");

    Serial.print("[Time] Waiting for NTP sync");

    time_t now = time(nullptr);

    while (now < 100000) {
        delay(500);
        Serial.print(".");
        now = time(nullptr);
    }

    Serial.println();
    Serial.println("[Time] Synced");
}

#include <Arduino.h>
#include "dispenser.h"
#include "stepper.h"
#include "ir_sensor.h"
#include "inventory.h"
#include "config.h"

// ----------------------------------------
// Private helpers
// ----------------------------------------

static void openSlot(int slot) {
    Serial.print("[Dispenser] Opening slot ");
    Serial.println(slot + 1);
    rotateStepper(slot, STEPS_HALF_ROTATION);  // rotate 180 degrees to open
    powerOffStepper(slot);  // cut coil power after moving
}

static void closeSlot(int slot) {
    Serial.print("[Dispenser] Closing slot ");
    Serial.println(slot + 1);
    rotateStepper(slot, STEPS_HALF_ROTATION);  // rotate another 180 degrees to close
    powerOffStepper(slot);
}

// ----------------------------------------
// Public
// ----------------------------------------

DispenseResult dispenseSinglePill(int slot) {

    Serial.println("-----------------------------");
    Serial.print("[Dispenser] Dispensing from slot ");
    Serial.println(slot + 1);

    // 1. Check inventory
    if (!hasInventory(slot, 1)) {
        Serial.println("[Dispenser] Slot is EMPTY");
        return DISPENSE_EMPTY;
    }

    // 2. Try to dispense, retry up to MAX_RETRY times
    for (int retry = 0; retry < MAX_RETRY; retry++) {

        if (retry > 0) {
            Serial.print("[Dispenser] Retry ");
            Serial.print(retry);
            Serial.print(" of ");
            Serial.println(MAX_RETRY - 1);
        }

        // Open the slot (rotate disk to expose one pill)
        openSlot(slot);

        // Wait for pill to fall through and hit IR sensor
        bool pillDetected = waitForPillDrop(PILL_DETECT_TIMEOUT_MS);

        if (!pillDetected) {
            // No pill detected — close and retry
            Serial.println("[Dispenser] No pill detected after opening, closing and retrying");
            closeSlot(slot);
            delay(500);
            continue;
        }

        // Pill successfully dropped — update inventory
        decreaseInventory(slot);

        // Close slot
        closeSlot(slot);

        // Wait for person to pick up the pill
        // bool pickedUp = waitForPillPickup(PILL_PICKUP_TIMEOUT_MS);

        // if (!pickedUp) {
        //     Serial.println("[Dispenser] WARNING: Pill not picked up within timeout");
        //     return DISPENSE_NOT_PICKED_UP;
        // }

        Serial.println("[Dispenser] Dispense complete");
        return DISPENSE_OK;
    }

    // All retries failed — likely jammed
    Serial.println("[Dispenser] JAM DETECTED — all retries failed");
    return DISPENSE_JAM;
}

bool dispensePills(int slot, int quantity) {

    Serial.print("[Dispenser] Dispensing ");
    Serial.print(quantity);
    Serial.print(" pill(s) from slot ");
    Serial.println(slot + 1);

    for (int i = 0; i < quantity; i++) {

        Serial.print("[Dispenser] Pill ");
        Serial.print(i + 1);
        Serial.print(" of ");
        Serial.println(quantity);

        DispenseResult result = dispenseSinglePill(slot);

        if (result != DISPENSE_OK) {
            Serial.print("[Dispenser] Stopped at pill ");
            Serial.print(i + 1);
            Serial.print(" — reason: ");
            Serial.println(dispenseResultToString(result));
            return false;
        }

        // Delay between pills (not after the last one)
        if (i < quantity - 1) {
            delay(PILL_DELAY_MS);
        }
    }

    Serial.println("[Dispenser] All pills dispensed successfully");
    return true;
}

const char* dispenseResultToString(DispenseResult result) {
    switch (result) {
        case DISPENSE_OK:           return "ok";
        case DISPENSE_EMPTY:        return "empty";
        case DISPENSE_JAM:          return "jam";
        // case DISPENSE_NOT_PICKED_UP: return "not_picked_up";
        default:                    return "unknown";
    }
}

#ifndef DISPENSER_H
#define DISPENSER_H

// Result codes for dispense operations
enum DispenseResult {
    DISPENSE_OK,              // pill dispensed and picked up successfully
    DISPENSE_EMPTY,           // slot has no pills
    DISPENSE_JAM,             // motor ran but IR never detected a pill
    DISPENSE_NOT_PICKED_UP,   // pill dropped but person didn't pick it up
};

// Dispense one pill from a given slot.
// Returns a DispenseResult code.
DispenseResult dispenseSinglePill(int slot);

// Dispense multiple pills from a slot, one at a time.
// Stops immediately on any failure.
// Returns true if all pills dispensed successfully.
bool dispensePills(int slot, int quantity);

// Convert result code to readable string (for MQTT/logging)
const char* dispenseResultToString(DispenseResult result);

#endif

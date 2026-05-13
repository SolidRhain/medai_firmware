#include <Arduino.h>
#include "inventory.h"
#include "config.h"

static int inventory[MAX_SLOTS] = {10, 10, 10};

void initInventory() {
    for (int i = 0; i < MAX_SLOTS; i++) {
        inventory[i] = 10;
    }
    Serial.println("[Inventory] Initialized (10 pills per slot)");
}

int getInventory(int slot) {
    if (slot < 0 || slot >= MAX_SLOTS) return 0;
    return inventory[slot];
}

bool hasInventory(int slot, int qty) {
    if (slot < 0 || slot >= MAX_SLOTS) return false;
    return inventory[slot] >= qty;
}

void decreaseInventory(int slot) {
    if (slot < 0 || slot >= MAX_SLOTS) return;
    if (inventory[slot] > 0) {
        inventory[slot]--;
    }
    Serial.print("[Inventory] Slot ");
    Serial.print(slot + 1);
    Serial.print(" → ");
    Serial.print(inventory[slot]);
    Serial.println(" pills remaining");
}

void setInventory(int slot, int count) {
    if (slot < 0 || slot >= MAX_SLOTS) return;
    inventory[slot] = count;
    Serial.print("[Inventory] Slot ");
    Serial.print(slot + 1);
    Serial.print(" set to ");
    Serial.println(count);
}

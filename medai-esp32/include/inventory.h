#ifndef INVENTORY_H
#define INVENTORY_H

// Initialize all slots to default count
void initInventory();

// Returns current pill count for a slot (0-based)
int  getInventory(int slot);

// Returns true if slot has enough pills
bool hasInventory(int slot, int qty);

// Decrease pill count by 1 for a slot
void decreaseInventory(int slot);

// Set pill count directly (e.g. from MQTT command)
void setInventory(int slot, int count);

#endif

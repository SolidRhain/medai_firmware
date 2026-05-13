#ifndef IR_SENSOR_H
#define IR_SENSOR_H

// Initialize IR sensor pin
void initIRSensor();

// Wait until a pill is detected falling through the drop point.
// Returns true if detected within timeout, false if timed out.
bool waitForPillDrop(unsigned long timeoutMs);

// Wait until the person picks up the pill (sensor clears).
// Returns true if cleared within timeout.
bool waitForPillPickup(unsigned long timeoutMs);

// Raw read — true if pill is currently blocking the sensor
bool isPillPresent();

#endif

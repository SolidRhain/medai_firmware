#ifndef DHT_SENSOR_H
#define DHT_SENSOR_H

// Initialize DHT22 sensor
void initDHT();

// Read temperature in Celsius.
// Returns NAN if read fails.
float readTemperature();

// Read relative humidity (%).
// Returns NAN if read fails.
float readHumidity();

// Read both at once into output params.
// Returns true if both readings are valid.
bool readDHT(float& temperature, float& humidity);

#endif

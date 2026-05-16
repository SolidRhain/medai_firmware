#ifndef CONFIG_H
#define CONFIG_H

// ==========================
// WIFI
// ==========================

#define WIFI_SSID       "wifiname"
#define WIFI_PASSWORD   "wifipassword"

// ==========================
// AWS MQTT
// ==========================

#define MQTT_BROKER     "a3mqc6kvosl0e0-ats.iot.ap-southeast-1.amazonaws.com"
#define MQTT_PORT       8883
#define DEVICE_ID       "medai-001"

#define MQTT_COMMAND_TOPIC    "medai/device/" DEVICE_ID "/command"
#define MQTT_STATUS_TOPIC     "medai/device/" DEVICE_ID "/status"
#define MQTT_ALERT_TOPIC      "medai/device/" DEVICE_ID "/alert"
#define MQTT_TELEMETRY_TOPIC  "medai/device/" DEVICE_ID "/telemetry"

// ==========================
// CERTIFICATE PATHS (SPIFFS)
// ==========================

#define PATH_ROOT_CA     "/AmazonRootCA1.pem"
#define PATH_DEVICE_CRT  "/device.pem.crt"
#define PATH_PRIVATE_KEY "/private.pem.key"

// ==========================
// MOTOR PINS — ULN2003
// Slot 0 (Compartment 1)
// Slot 1 (Compartment 2)
// Slot 2 (Compartment 3)
// ==========================

#define MOTOR0_IN1  16
#define MOTOR0_IN2  17
#define MOTOR0_IN3  18
#define MOTOR0_IN4  19

#define MOTOR1_IN1  25
#define MOTOR1_IN2  26
#define MOTOR1_IN3  27
#define MOTOR1_IN4  14

#define MOTOR2_IN1  32
#define MOTOR2_IN2  33
#define MOTOR2_IN3  12
#define MOTOR2_IN4  13

// ==========================
// SHARED IR SENSOR
// (Only 1 sensor, placed at
//  the pill drop-off point)
// ==========================

#define IR_SENSOR_PIN   34      // INPUT ONLY pin

// IR output: LOW = pill detected, HIGH = no pill
#define IR_PILL_DETECTED    LOW
#define IR_NO_PILL          HIGH

// ==========================
// MOTOR STEPS
// 28BYJ-48 full rotation = 2048 steps
// Quarter rotation = 512 steps
// ==========================

#define STEPS_QUARTER_TURN  512     
#define STEPS_HALF_ROTATION 1024    // rotate 180 degrees to open/close
#define STEP_DELAY_MS       2       // ms between steps (speed)

// ==========================
// DISPENSE SETTINGS
// ==========================

#define MAX_SLOTS           3
#define MAX_RETRY           3
#define PILL_DETECT_TIMEOUT_MS  5000    // wait up to 5s for pill to drop
#define PILL_PICKUP_TIMEOUT_MS  30000   // wait up to 30s for person to take pill
#define PILL_DELAY_MS           1000    // delay between dispensing multiple pills

// ==========================
// DHT22 SENSOR
// ==========================

#define DHT_PIN         4       // GPIO4 — DATA pin of DHT22
#define DHT_TYPE        DHT22

// Alert thresholds
#define TEMP_MAX_C      35.0    // alert if temperature exceeds this
#define HUMIDITY_MAX    80.0    // alert if humidity exceeds this

// ==========================
// TIMING
// ==========================

#define TELEMETRY_INTERVAL_MS   60000
#define RECONNECT_INTERVAL_MS   5000

#endif

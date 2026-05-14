# MedAI Cabinet — ESP32 Firmware

## File Structure

```
src/
├── main.cpp            ← setup() and loop() only
├── config.h            ← all pins, credentials, timing constants
│
├── wifi_manager.h/cpp  ← WiFi connection
├── time_manager.h/cpp  ← NTP sync (required for TLS)
├── cert_manager.h/cpp  ← load certs from SPIFFS
│
├── stepper.h/cpp       ← ULN2003 motor control (3 motors)
├── ir_sensor.h/cpp     ← shared IR sensor at drop-off point
├── inventory.h/cpp     ← pill count tracking per slot
├── dispenser.h/cpp     ← dispense logic (open→detect→close→pickup)
│
└── mqtt_manager.h/cpp  ← AWS IoT commands + publish helpers
```

---

## MQTT Commands (send from AWS IoT Console or backend)

### Dispense pills
Topic: `medai/device/medai-001/command`
```json
{
  "command": "dispense",
  "slot": 1,
  "quantity": 2
}
```
- `slot`: 1, 2, or 3
- `quantity`: how many pills to dispense

### Set inventory (after refill)
```json
{
  "command": "set_inventory",
  "slot": 1,
  "count": 10
}
```

### Ping
```json
{ "command": "ping" }
```

---

## MQTT Topics Published by ESP32

| Topic | When |
|-------|------|
| `medai/device/medai-001/status` | After each command ("dispensing", "completed", "failed", "pong") |
| `medai/device/medai-001/alert`  | On errors (jam, empty, invalid command) |
| `medai/device/medai-001/telemetry` | Every 60s + after each dispense (inventory counts) |

---

## Dispense Flow (1 pill)

```
MQTT command received
    │
    ▼
Check inventory > 0?  ──No──► publishAlert("inventory_low") → publishStatus("failed")
    │ Yes
    ▼
Open slot (motor +512 steps)
    │
    ▼
Wait for IR to detect pill (up to 5s)
    │
    ├─ No pill ──► close slot, retry (up to 3x)
    │              After 3 fails ──► publishAlert("jam")
    │ Pill detected
    ▼
decreaseInventory()
    │
    ▼
Close slot (motor -512 steps)
    │
    ▼
Wait for person to pick up pill (up to 30s)
    │
    ├─ Not picked up ──► publishAlert("not_picked_up"), return DISPENSE_NOT_PICKED_UP
    │ Picked up
    ▼
publishStatus("completed")
publishInventory()
```

---

## Setup Steps

1. Edit `src/config.h` — fill in WiFi credentials and MQTT broker
2. Paste your 3 AWS cert files into `data/`
3. PlatformIO: **Upload Filesystem Image** (certs → SPIFFS)
4. PlatformIO: **Upload** (firmware)
5. Open Serial Monitor at 115200

---

## Troubleshooting

| Serial Output | Fix |
|---|---|
| `SPIFFS mount FAILED` | Run Upload Filesystem Image first |
| `FAILED TO OPEN: /device.pem.crt` | Cert file missing from data/ folder |
| `MQTT State: -4 → TIMEOUT` | Wrong certificates or cert file still has placeholder text |
| `MQTT State: 5 → UNAUTHORIZED` | AWS IoT policy not attached to certificate |
| Motor doesn't turn | ULN2003 VCC not connected to 5V (not 3.3V) |
| IR always LOW | Adjust potentiometer on IR module |
| IR never detects pill | Sensor aimed wrong — should face drop-off point |

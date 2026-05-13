#include <Arduino.h>
#include "stepper.h"
#include "config.h"

// ----------------------------------------
// Pin map for each slot's ULN2003 inputs
// ----------------------------------------

static const int motorPins[MAX_SLOTS][4] = {
    { MOTOR0_IN1, MOTOR0_IN2, MOTOR0_IN3, MOTOR0_IN4 },  // Slot 0 (Compartment 1)
    { MOTOR1_IN1, MOTOR1_IN2, MOTOR1_IN3, MOTOR1_IN4 },  // Slot 1 (Compartment 2)
    { MOTOR2_IN1, MOTOR2_IN2, MOTOR2_IN3, MOTOR2_IN4 },  // Slot 2 (Compartment 3)
};

// 28BYJ-48 half-step sequence (8 steps per cycle, smoother)
static const int stepSequence[8][4] = {
    {1, 0, 0, 0},
    {1, 1, 0, 0},
    {0, 1, 0, 0},
    {0, 1, 1, 0},
    {0, 0, 1, 0},
    {0, 0, 1, 1},
    {0, 0, 0, 1},
    {1, 0, 0, 1},
};

static int stepIndex[MAX_SLOTS] = {0, 0, 0};  // tracks position in sequence per motor

// ----------------------------------------
// Private: write one step to a motor
// ----------------------------------------

static void writeStep(int slot, int seqIndex) {

    for (int pin = 0; pin < 4; pin++) {
        digitalWrite(motorPins[slot][pin], stepSequence[seqIndex][pin]);
    }
}

// ----------------------------------------
// Public
// ----------------------------------------

void initSteppers() {

    for (int slot = 0; slot < MAX_SLOTS; slot++) {
        for (int pin = 0; pin < 4; pin++) {
            pinMode(motorPins[slot][pin], OUTPUT);
            digitalWrite(motorPins[slot][pin], LOW);
        }
    }

    Serial.println("[Stepper] All 3 motors initialized");
}

void rotateStepper(int slot, int steps) {

    if (slot < 0 || slot >= MAX_SLOTS) {
        Serial.print("[Stepper] Invalid slot: ");
        Serial.println(slot);
        return;
    }

    Serial.print("[Stepper] Slot ");
    Serial.print(slot + 1);
    Serial.print(" rotating ");
    Serial.print(steps);
    Serial.println(" steps");

    int direction = (steps > 0) ? 1 : -1;
    int absSteps  = abs(steps);

    for (int i = 0; i < absSteps; i++) {

        stepIndex[slot] = (stepIndex[slot] + direction + 8) % 8;
        writeStep(slot, stepIndex[slot]);
        delay(STEP_DELAY_MS);
    }
}

void powerOffStepper(int slot) {

    if (slot < 0 || slot >= MAX_SLOTS) return;

    for (int pin = 0; pin < 4; pin++) {
        digitalWrite(motorPins[slot][pin], LOW);
    }
}

void powerOffAllSteppers() {

    for (int slot = 0; slot < MAX_SLOTS; slot++) {
        powerOffStepper(slot);
    }
}

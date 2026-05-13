#ifndef STEPPER_H
#define STEPPER_H

// Initialize all 3 motor pin modes
void initSteppers();

// Rotate a specific motor by a number of steps
// slot: 0, 1, or 2
// steps: positive = forward, negative = backward
void rotateStepper(int slot, int steps);

// Power off all coils of a motor (saves power, reduces heat)
void powerOffStepper(int slot);

// Power off all 3 motors
void powerOffAllSteppers();

#endif

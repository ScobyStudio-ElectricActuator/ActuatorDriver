#include <Arduino.h>
#include <Actuator.h>

//Feedback pins
#define ChA_extPin 8
#define ChA_retPin 7

//Output pins
#define ChA_enablePin 5
#define ChA_output1Pin 4
#define ChA_output2Pin 3

Actuator motorA;

void setup() {
  motorA.attachPins(ChA_extPin, ChA_retPin, ChA_enablePin, ChA_output1Pin, ChA_output2Pin);
}

void loop() {
  motorA.cyclic();
}
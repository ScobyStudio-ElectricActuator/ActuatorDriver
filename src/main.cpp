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
  if(Serial.available()){
    String serialMsg = Serial.readString();

    if(serialMsg == "set speed 100"){
      motorA.setSpeed(100);
    }
    else if(serialMsg == "get speed"){
      Serial.println(motorA.getSpeed());
    }
    else if(serialMsg == "get state"){
      Serial.println(motorA.getState());
    }
    else if(serialMsg == "set extend timeout 1"){
      motorA.setExtendTimeout(1);
    }
    else if(serialMsg == "set retract timeout 1"){
      motorA.setRetractTimeout(1);
    }
    else if(serialMsg == "extend"){
      motorA.extend();
    }
    else if(serialMsg == "retract"){
      motorA.retract();
    }
    else if(serialMsg == "stop"){
      motorA.stop();
    }
    else if(serialMsg == "relax"){
      motorA.relax();
    }
  }

  motorA.cyclic();
  
}
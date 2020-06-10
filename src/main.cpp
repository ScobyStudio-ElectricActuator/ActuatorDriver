#include <Arduino.h>
#include <Actuator.h>

//Feedback pins
#define ChA_extPin 8
#define ChA_retPin 7

//Output pins
#define ChA_enablePin 5
#define ChA_output1Pin 4
#define ChA_output2Pin 3
#define ChA_extLED 16
#define ChA_retLED 17

Actuator motorA(ChA_extPin, ChA_retPin, ChA_enablePin, ChA_output1Pin, ChA_output2Pin, ChA_extLED, ChA_retLED);

void setup() {
  Serial.begin(9600);
  while(!Serial);
  Serial.println("serial began");
}

void loop() {
  if(Serial.available()){
    String serialMsg = Serial.readString();

    if(serialMsg.substring(0,9) == "set speed"){
      motorA.setSpeed(serialMsg.substring(10).toInt());
    }
    else if(serialMsg == "get speed"){
      Serial.println(motorA.getSpeed());
    }
    else if(serialMsg.substring(0,13) == "set min speed"){
      motorA.setMinSpeed(serialMsg.substring(14).toInt());
    }
    else if(serialMsg == "get state"){
      switch(motorA.getState()){
        case Actuator::extended:
          Serial.println("extended");
          break;
        case Actuator::extending:
          Serial.println("extending");
          break;
        case Actuator::retracted:
          Serial.println("retracted");
          break;
        case Actuator::retracting:
          Serial.println("retracting");
          break;
        case Actuator::stopped:
          Serial.println("stopped");
          break;
        case Actuator::relaxed:
          Serial.println("relaxed");
          break;
        case Actuator::timedout:
          Serial.println("timed out");
          break;
      }
    }
    else if(serialMsg.substring(0,11) == "set timeout"){
      motorA.setTimeout(serialMsg.substring(12).toFloat());
    }
    else if(serialMsg.substring(0,17) == "set normally open"){
      motorA.setNormallyOpenFB(serialMsg.substring(18).toInt());
    }
    else if(serialMsg.substring(0,10) == "set has fb"){
      motorA.setHasFeedback(serialMsg.substring(11).toInt());
    }
    else if(serialMsg.substring(0,11) == "set fb time"){
      motorA.setFeedbackTime(serialMsg.substring(12).toInt());
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
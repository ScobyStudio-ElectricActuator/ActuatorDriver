#include "Actuator.h"
#include "Arduino.h"

Actuator::Actuator(){
    _minSpeed = 10;

    setSpeed(100);
    setState(relaxed);
}


void Actuator::attachPins(int extPin, int retPin, int enablePin, int output1Pin, int output2Pin){
    Serial.begin(9600);
    while(!Serial);
    Serial.println("serial began");

    _extPin = extPin;
    _retPin = retPin;
    _enablePin = enablePin;
    _output1Pin = output1Pin;
    _output2Pin = output2Pin;

    pinMode(_extPin, INPUT_PULLUP);
    pinMode(_retPin, INPUT_PULLUP);
    pinMode(_enablePin, OUTPUT);
    pinMode(_output1Pin, OUTPUT);
    pinMode(_output2Pin, OUTPUT);

    setCommand(motorRelax);
}

void Actuator::setSpeed(int speed){
    if(speed >=1 && speed <=100){
        _speed = map(speed, 1, 100, _minSpeed, 255);
    }
    else if(speed == 0){
        _speed = 0;
    }
}

int Actuator::getSpeed(){
    return map(_speed, _minSpeed, 255, 1, 100);
}

void Actuator::setState(state actuatorState){
    _state = actuatorState;
}

Actuator::state Actuator::getState(){
    return _state;
}

void Actuator::setCommand(motorCmd motorCommand){
    switch(motorCommand){
        case motorExtend:
            digitalWrite(_output1Pin,HIGH);
            digitalWrite(_output2Pin,LOW);
            analogWrite(_enablePin,_speed);
            break;
        case motorRetract:
            digitalWrite(_output1Pin,LOW);
            digitalWrite(_output2Pin,HIGH);
            analogWrite(_enablePin,_speed);
            break;
        case motorStop:
            digitalWrite(_output1Pin,LOW);
            digitalWrite(_output2Pin,LOW);
            analogWrite(_enablePin,255);
            break;
        case motorRelax:
            digitalWrite(_output1Pin,LOW);
            digitalWrite(_output2Pin,LOW);
            analogWrite(_enablePin,0);
            break;
    }
}

Actuator::serialCmd Actuator::decodeSerial(){
    static serialCmd serialResult = serialRelax;
    if(Serial.available()){
        String serialMsg = Serial.readString();
        if(serialMsg.substring(0,9) == "set speed"){
            setSpeed(serialMsg.substring(10).toInt());
        }
        else if(serialMsg == "get speed"){
            Serial.println(getSpeed());
        }
        else if(serialMsg == "extend"){
            serialResult = serialExtend;
        }
        else if(serialMsg == "retract"){
            serialResult = serialRetract;
        }
        else if(serialMsg == "stop"){
            serialResult = serialStop;
        }
        else if(serialMsg == "relax"){
            serialResult = serialRelax;
        }
    }

    return serialResult;
}

void Actuator::cyclic(){
    bool extFB = !digitalRead(_extPin);
    bool retFB = !digitalRead(_retPin);
    serialCmd serialMsg = decodeSerial();

    switch(getState()){
        case extended:
            if(serialMsg == serialStop){
                setCommand(motorStop);
                setState(stopped);
            }
            else if(serialMsg == serialRelax){
                setCommand(motorRelax);
                setState(relaxed);
            }
            else if(serialMsg == serialRetract && !retFB){
                setCommand(motorRetract);
                setState(retracting);
            }
            break;
        case extending:
            if(serialMsg == serialStop){
                setCommand(motorStop);
                setState(stopped);
            }
            else if(serialMsg == serialRelax){
                setCommand(motorRelax);
                setState(relaxed);
            }
            else if(serialMsg == serialRetract && !retFB){
                setCommand(motorRetract);
                setState(retracting);
            }
            else if(serialMsg == serialExtend && !extFB){
                setCommand(motorExtend);
                setState(extending);
            }
            else if(extFB){
                setCommand(motorStop);
                setState(extended);
            }
            break;
        case retracted:
            if(serialMsg == serialStop){
                setCommand(motorStop);
                setState(stopped);
            }
            else if(serialMsg == serialRelax){
                setCommand(motorRelax);
                setState(relaxed);
            }
            else if(serialMsg == serialExtend && !extFB){
                setCommand(motorExtend);
                setState(extending);
            }
            break;
        case retracting:
            if(serialMsg == serialStop){
                setCommand(motorStop);
                setState(stopped);
            }
            else if(serialMsg == serialRelax){
                setCommand(motorRelax);
                setState(relaxed);
            }
            else if(serialMsg == serialExtend && !extFB){
                setCommand(motorExtend);
                setState(extending);
            }
            else if(serialMsg == serialRetract && !retFB){
                setCommand(motorRetract);
                setState(retracting);
            }
            else if(retFB){
                setCommand(motorStop);
                setState(retracted);
            }
            break;
        case stopped:
            if(serialMsg == serialRelax){
                setCommand(motorRelax);
                setState(relaxed);
            }
            else if(serialMsg == serialRetract && !retFB){
                setCommand(motorRetract);
                setState(retracting);
            }
            else if(serialMsg == serialExtend && !extFB){
                setCommand(motorExtend);
                setState(extending);
            }
            break;
        case relaxed:
            if(serialMsg == serialStop){
                setCommand(motorStop);
                setState(stopped);
            }
            else if(serialMsg == serialRetract && !retFB){
                setCommand(motorRetract);
                setState(retracting);
            }
            else if(serialMsg == serialExtend && !extFB){
                setCommand(motorExtend);
                setState(extending);
            }
            break;
    }
}
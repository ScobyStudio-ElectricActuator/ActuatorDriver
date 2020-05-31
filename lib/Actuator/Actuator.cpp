#include "Actuator.h"
#include "Arduino.h"

Actuator::Actuator(int extPin, int retPin, int enablePin, int output1Pin, int output2Pin){
    Serial.begin(9600);
    pinMode(extPin, INPUT_PULLUP);
    pinMode(retPin, INPUT_PULLUP);
    pinMode(enablePin, OUTPUT);
    pinMode(output1Pin, OUTPUT);
    pinMode(output2Pin, OUTPUT);

    _extPin = extPin;
    _retPin = retPin;
    _enablePin = enablePin;
    _output1Pin = output1Pin;
    _output2Pin = output2Pin;
    _minSpeed = 10;

    setSpeed(100);
    setState(relaxed);
}

void Actuator::setSpeed(int speed){
    if(speed >=0 && speed <=100){
        _speed = map(speed, 0, 100, _minSpeed, 255);
    }
}

int Actuator::getSpeed(){
    return map(_speed, _minSpeed, 255, 0, 100);
}

void Actuator::setState(state actuatorState){
    _state = actuatorState;
}

Actuator::state Actuator::getState(){
    return _state;
}

void Actuator::setCommand(motorCmd motorCommand){
    _command = motorCommand;
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

Actuator::motorCmd Actuator::getCommand(){
    return _command;
}

Actuator::serialCmd Actuator::decodeSerial(){
    String serialMsg;

    if(Serial.available()){
        serialMsg = Serial.readString();
        if(serialMsg == "extend"){
            return serialExtend;
        }
        else if(serialMsg == "retract"){
            return serialRetract;
        }
        else if(serialMsg == "stop"){
            return serialStop;
        }
        else if(serialMsg == "relax"){
            return serialRelax;
        }
        else if(serialMsg.substring(0,9) == "set speed"){
            setSpeed(serialMsg.substring(11).toInt());
            return serialNoAction;
        }
        else if(serialMsg.substring(0,9) == "get speed"){
            Serial.println(_speed);
            return serialNoAction;
        }
        else{
            return serialNoAction;
        }
    }
    else{
        return serialNoAction;
    }
}

void Actuator::cyclic(){
    bool extFB = digitalRead(_extPin);
    bool retFB = digitalRead(_retPin);
    serialCmd serialMsg = decodeSerial();

    switch(_state){
        case extended:
            if(serialMsg == serialStop){
                setCommand(motorStop);
                _state = stopped;
            }
            else if(serialMsg == serialRelax){
                setCommand(motorRelax);
                _state = relaxed;
            }
            else if(serialMsg == serialRetract && !retFB){
                setCommand(motorRetract);
                _state = retracting;
            }
            break;
        case extending:
            if(serialMsg == serialStop){
                setCommand(motorStop);
                _state = stopped;
            }
            else if(serialMsg == serialRelax){
                setCommand(motorRelax);
                _state = relaxed;
            }
            else if(serialMsg == serialRetract && !retFB){
                setCommand(motorRetract);
                _state = retracting;
            }
            else if(extFB){
                setCommand(motorStop);
                _state = extended;
            }
            break;
        case retracted:
            if(serialMsg == serialStop){
                setCommand(motorStop);
                _state = stopped;
            }
            else if(serialMsg == serialRelax){
                setCommand(motorRelax);
                _state = relaxed;
            }
            else if(serialMsg == serialExtend){
                setCommand(motorExtend);
                _state = extending;
            }
            break;
        case retracting:
            if(serialMsg == serialStop){
                setCommand(motorStop);
                _state = stopped;
            }
            else if(serialMsg == serialRelax){
                setCommand(motorRelax);
                _state = relaxed;
            }
            else if(serialMsg == serialExtend){
                setCommand(motorExtend);
                _state = extending;
            }
            else if(retFB){
                setCommand(motorStop);
                _state = retracted;
            }
            break;
        case stopped:
            if(serialMsg == serialRelax){
                setCommand(motorRelax);
                _state = relaxed;
            }
            else if(serialMsg == serialRetract && !retFB){
                setCommand(motorRetract);
                _state = retracting;
            }
            else if(serialMsg == serialExtend){
                setCommand(motorExtend);
                _state = extending;
            }
            break;
        case relaxed:
            if(serialMsg == serialStop){
                setCommand(motorStop);
                _state = stopped;
            }
            else if(serialMsg == serialRetract && !retFB){
                setCommand(motorRetract);
                _state = retracting;
            }
            else if(serialMsg == serialExtend){
                setCommand(motorExtend);
                _state = extending;
            }
            break;
    }
}
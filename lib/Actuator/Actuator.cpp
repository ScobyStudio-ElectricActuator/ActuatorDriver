#include "Actuator.h"
#include "Arduino.h"

Actuator::Actuator(){
    _minSpeed = 10;

    setSpeed(100);
    _state = relaxed;
    _prevState = relaxed;

    _extendTimeout = 3000;
    _retractTimeout = 3000;
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

    checkFB();
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

Actuator::state Actuator::getState(){
    return _state;
}

void Actuator::setRetractTimeout(float retractTimeout){
    _retractTimeout = retractTimeout*1000;
}

void Actuator::setExtendTimeout(float extendTimeout){
    _extendTimeout = extendTimeout*1000;
}

void Actuator::extend(){
    if(!_extFB){
        _state = extending;
    }
}

void Actuator::retract(){
    if(!_retFB){
        _state = retracting;
    }
}

void Actuator::stop(){
    _state = stopped;
}

void Actuator::relax(){
    _state = relaxed;
}

void Actuator::checkFB(){
    _extFB = !digitalRead(_extPin);
    _retFB = !digitalRead(_retPin);
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

void Actuator::cyclic(){
    unsigned long extendStartTime;
    unsigned long retractStartTime;

    checkFB();
    switch(_state){
        case extended:
            setCommand(motorStop);
            if(_state != _prevState){
                _prevState = _state;
            }
            break;

        case extending:
            setCommand(motorExtend);
            if(_state != _prevState){
                extendStartTime = millis();
                _prevState = _state;
            }

            if(millis()-extendStartTime >= _extendTimeout){
                _state = extendingTimeout;
            }
            else if(_extFB && !_retFB){
                _state = extended;
            }
            break;

        case extendingTimeout:
            setCommand(motorStop);
            if(_state != _prevState){
                _prevState = _state;
            }
            break;

        case retracted:
            setCommand(motorStop);
            if(_state != _prevState){
                _prevState = _state;
            }
            break;

        case retracting:
            setCommand(motorRetract);
            if(_state != _prevState){
                retractStartTime = millis();
                _prevState = _state;
                Serial.println(retractStartTime);
            }

            if(millis()-retractStartTime >= _retractTimeout){
                _state = retractingTimeout;
            }
            else if(!_extFB && _retFB){
                _state = extended;
            }
            break;

        case retractingTimeout:
            setCommand(motorStop);
            if(_state != _prevState){
                _prevState = _state;
            }
            break;

        case stopped:
            setCommand(motorStop);
            if(_state != _prevState){
                _prevState = _state;
            }
            break;

        case relaxed:
            setCommand(motorRelax);
            if(_state != _prevState){
                _prevState = _state;
            }
            break;
    }
}
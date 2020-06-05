#include "Actuator.h"
#include "Arduino.h"

Actuator::Actuator(int extPin, int retPin, int enablePin, int output1Pin, int output2Pin){
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

    _minSpeed = 10;

    setSpeed(100);

    checkFB();
    if(_extFB){
        _state = extended;
        _prevState = extended;
    }
    else if(_retFB){
        _state = retracted;
        _prevState = retracted;
    }
    else{
        _state = relaxed;
        _prevState = relaxed;
    }
    
    cyclic();

    setTimeout(3);
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
    if(_speed != 0){
        return map(_speed, _minSpeed, 255, 1, 100);
    }
    else{
        return 0;
    }
}

Actuator::state Actuator::getState(){
    return _state;
}

void Actuator::setTimeout(unsigned int timeout){
    if(timeout >= 0){
        _timeout = timeout * 1000;
    }
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

void Actuator::setCommand(){
    switch(_state){
        case extending:
            digitalWrite(_output1Pin,HIGH);
            digitalWrite(_output2Pin,LOW);
            analogWrite(_enablePin,_speed);
            break;
        case retracting:
            digitalWrite(_output1Pin,LOW);
            digitalWrite(_output2Pin,HIGH);
            analogWrite(_enablePin,_speed);
            break;
        case stopped:
        case extended:
        case retracted:
        case timedout:
            digitalWrite(_output1Pin,LOW);
            digitalWrite(_output2Pin,LOW);
            analogWrite(_enablePin,255);
            break;
        case relaxed:
            digitalWrite(_output1Pin,LOW);
            digitalWrite(_output2Pin,LOW);
            analogWrite(_enablePin,0);
            break;
    }
}

void Actuator::cyclic(){
    static unsigned long startTime;

    checkFB();
    switch(_state){
        case extended:
            setCommand();
            if(_state != _prevState){
                _prevState = _state;
            }
            break;

        case extending:
            setCommand();
            if(_state != _prevState){
                startTime = millis();
                _prevState = _state;
            }

            if(millis()-startTime >= _timeout && _timeout != 0){
                _state = timedout;
            }
            
            if(_extFB && !_retFB){
                _state = extended;
            }
            break;

        case retracted:
            setCommand();
            if(_state != _prevState){
                _prevState = _state;
            }
            break;

        case retracting:
            setCommand();
            if(_state != _prevState){
                startTime = millis();
                _prevState = _state;
            }

            if(millis()-startTime >= _timeout && _timeout != 0){
                _state = timedout;
            }
            
            if(!_extFB && _retFB){
                _state = retracted;
            }
            break;

        case timedout:
            setCommand();
            if(_state != _prevState){
                _prevState = _state;
            }
            break;

        case stopped:
            setCommand();
            if(_state != _prevState){
                _prevState = _state;
            }
            break;

        case relaxed:
            setCommand();
            if(_state != _prevState){
                _prevState = _state;
            }
            break;
    }
}
#ifndef Actuator_h
#define Actuator_h

#include "Arduino.h"

class Actuator{
    public:
        Actuator(int extPin, int retPin, int enablePin, int output1Pin, int output2Pin);
        void cyclic();
        
    private:
        enum state{
            extended,
            extending,
            retracted,
            retracting,
            stopped,
            relaxed
        };

        enum motorCmd{
            motorExtend,
            motorRetract,
            motorStop,
            motorRelax
        };

        enum serialCmd{
            serialExtend,
            serialRetract,
            serialStop,
            serialRelax,
            serialNoAction
        };

        int _extPin;
        int _retPin;
        int _enablePin;
        int _output1Pin;
        int _output2Pin;
        int _speed;
        int _maxSpeed;
        int _minSpeed;
        int _offSpeed;

        state _state;
        motorCmd _command;

        state getState();
        void setState(state actuatorState);
        
        int getSpeed();
        void setSpeed(int speed);

        motorCmd getCommand();
        void setCommand(motorCmd motorCommand);

        serialCmd decodeSerial();
};


#endif
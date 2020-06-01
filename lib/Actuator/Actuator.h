////////////////////////////////////
//    Project: Actuator Driver    //
//      Author: Scoby Studio      //
//         Revision: v1.0         //
////////////////////////////////////
/*
Revision history

v1.0
-Initial release
-One channel
-Serial controlled

*/

#ifndef Actuator_h
#define Actuator_h

#include "Arduino.h"

class Actuator{
    public:
        Actuator();
        void attachPins(int extPin, int retPin, int enablePin, int output1Pin, int output2Pin);
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
            serialRelax
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

        void setState(state actuatorState);
        state getState();
        
        int getSpeed();
        void setSpeed(int speed);

        void setCommand(motorCmd motorCommand);

        serialCmd decodeSerial();
};


#endif
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

v2.0
-Two channel capability
-Timeout feature
-Change speed while extending/retracting
*/

#ifndef Actuator_h
#define Actuator_h

#include "Arduino.h"

class Actuator{
    public:
        enum state{
            extended,
            extending,
            retracted,
            retracting,
            stopped,
            relaxed,
            extendingTimeout,
            retractingTimeout,
        };

        Actuator();
        void attachPins(int extPin, int retPin, int enablePin, int output1Pin, int output2Pin);
        void cyclic();

        state getState();
        int getSpeed();
        void setSpeed(int speed);
        void setRetractTimeout(float retractTimeout);
        void setExtendTimeout(float extendTimeout);

        void extend();
        void retract();
        void stop();
        void relax();
        
    private:
        enum motorCmd{
            motorExtend,
            motorRetract,
            motorStop,
            motorRelax
        };

        int _extPin;
        int _retPin;
        int _enablePin;
        int _output1Pin;
        int _output2Pin;
        int _speed;
        int _minSpeed;
        unsigned int _extendTimeout;
        unsigned int _retractTimeout;

        bool _extFB;
        bool _retFB;

        state _state;
        state _prevState;
        motorCmd _command;

        void setCommand(motorCmd motorCommand);
        void checkFB();
};


#endif
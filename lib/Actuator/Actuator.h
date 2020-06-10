////////////////////////////////////
//    Project: Actuator Driver    //
//      Author: Scoby Studio      //
//         Revision: v5.0         //
////////////////////////////////////
/*
Revision history
v5.0
-add ext/ret LED

v4.0
-set min speed
-NO/NC feedback
-has feedback
-feedback time

v3.0
-improve set commands
-removed attachPins method
-added disable timeout
-remove motor enums

v2.0
-Two channel capability
-Timeout feature
-Change speed while extending/retracting


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
        enum state{
            extended,
            extending,
            retracted,
            retracting,
            stopped,
            relaxed,
            timedout,
        };

        Actuator(int extPin, int retPin, int enablePin, int output1Pin, int output2Pin, int extLED, int retLED);
        void cyclic();

        state getState();
        int getSpeed();
        void setSpeed(int speed);
        void setMinSpeed(int minSpeed);
        void setTimeout(unsigned int timeout);
        void setNormallyOpenFB(bool isNormallyOpen);
        void setHasFeedback(bool hasFeedback);
        void setFeedbackTime(unsigned int FBTime);

        void extend();
        void retract();
        void stop();
        void relax();
        
        bool testExtFB;
        bool testRetFB;
    private:
        int _extPin;
        int _retPin;
        int _enablePin;
        int _output1Pin;
        int _output2Pin;
        int _extLED;
        int _retLED;

        int _speed;
        int _minSpeed;
        unsigned long _timeout;
        bool _isNO;
        bool _hasFB;
        unsigned long _FBTime;

        bool _extFB;
        bool _retFB;

        state _state;
        state _prevState;

        void writeOutput();
        void readInput();
};


#endif
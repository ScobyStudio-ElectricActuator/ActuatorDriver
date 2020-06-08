#include <Arduino.h>
#include <unity.h>
#include <Actuator.h>

//Feedback pins
#define ChA_extPin 8
#define ChA_retPin 7

//Output pins
#define ChA_enablePin 5
#define ChA_output1Pin 4
#define ChA_output2Pin 3

Actuator motor(ChA_extPin, ChA_retPin, ChA_enablePin, ChA_output1Pin, ChA_output2Pin);

void testSpeed(){
    for(int j = 10; j < 100; j=j+10){
        for(int i = 0; i<=100; i++){
            motor.setSpeed(i);
            TEST_ASSERT_INT_WITHIN(2,i,motor.getSpeed());
        }
        motor.setMinSpeed(j);
    }

    motor.setSpeed(0);
    motor.extend();
    motor.cyclic();
    TEST_ASSERT_EQUAL(Actuator::extending,motor.getState());
    TEST_ASSERT_EQUAL(HIGH,digitalRead(ChA_output1Pin));
    TEST_ASSERT_EQUAL(LOW,digitalRead(ChA_output2Pin));
    TEST_ASSERT_EQUAL(LOW,digitalRead(ChA_enablePin));

    motor.relax();
    motor.cyclic();
}

void testState(){
    motor.setSpeed(100);
    motor.setTimeout(0);
    motor.testExtFB = false;
    motor.testRetFB = false;

    motor.stop();
    motor.cyclic();
    TEST_ASSERT_EQUAL(Actuator::stopped,motor.getState());
    TEST_ASSERT_EQUAL(LOW,digitalRead(ChA_output1Pin));
    TEST_ASSERT_EQUAL(LOW,digitalRead(ChA_output2Pin));
    TEST_ASSERT_EQUAL(HIGH,digitalRead(ChA_enablePin));

    motor.relax();
    motor.cyclic();
    TEST_ASSERT_EQUAL(Actuator::relaxed,motor.getState());
    TEST_ASSERT_EQUAL(LOW,digitalRead(ChA_output1Pin));
    TEST_ASSERT_EQUAL(LOW,digitalRead(ChA_output2Pin));
    TEST_ASSERT_EQUAL(LOW,digitalRead(ChA_enablePin));

    motor.extend();
    motor.cyclic();
    TEST_ASSERT_EQUAL(Actuator::extending,motor.getState());
    TEST_ASSERT_EQUAL(HIGH,digitalRead(ChA_output1Pin));
    TEST_ASSERT_EQUAL(LOW,digitalRead(ChA_output2Pin));
    TEST_ASSERT_EQUAL(HIGH,digitalRead(ChA_enablePin));

    motor.testExtFB = true;
    motor.cyclic();
    TEST_ASSERT_EQUAL(Actuator::extended,motor.getState());
    TEST_ASSERT_EQUAL(LOW,digitalRead(ChA_output1Pin));
    TEST_ASSERT_EQUAL(LOW,digitalRead(ChA_output2Pin));
    TEST_ASSERT_EQUAL(HIGH,digitalRead(ChA_enablePin));
    motor.testExtFB = false;

    motor.retract();
    motor.cyclic();
    TEST_ASSERT_EQUAL(Actuator::retracting,motor.getState());
    TEST_ASSERT_EQUAL(LOW,digitalRead(ChA_output1Pin));
    TEST_ASSERT_EQUAL(HIGH,digitalRead(ChA_output2Pin));
    TEST_ASSERT_EQUAL(HIGH,digitalRead(ChA_enablePin));

    motor.testRetFB = true;
    motor.cyclic();
    TEST_ASSERT_EQUAL(Actuator::retracted,motor.getState());
    TEST_ASSERT_EQUAL(LOW,digitalRead(ChA_output1Pin));
    TEST_ASSERT_EQUAL(LOW,digitalRead(ChA_output2Pin));
    TEST_ASSERT_EQUAL(HIGH,digitalRead(ChA_enablePin));
    motor.testRetFB = false;

    motor.relax();
    motor.cyclic();
}

void testTimeout(){
    motor.setSpeed(100);
    motor.setTimeout(1);
    motor.setHasFeedback(true);
    motor.testExtFB = false;
    motor.testRetFB = false;

    motor.extend();
    motor.cyclic();
    TEST_ASSERT_EQUAL(Actuator::extending,motor.getState());
    TEST_ASSERT_EQUAL(HIGH,digitalRead(ChA_output1Pin));
    TEST_ASSERT_EQUAL(LOW,digitalRead(ChA_output2Pin));
    TEST_ASSERT_EQUAL(HIGH,digitalRead(ChA_enablePin));
    delay(1500);
    motor.cyclic();
    TEST_ASSERT_EQUAL(Actuator::timedout,motor.getState());
    TEST_ASSERT_EQUAL(LOW,digitalRead(ChA_output1Pin));
    TEST_ASSERT_EQUAL(LOW,digitalRead(ChA_output2Pin));
    TEST_ASSERT_EQUAL(HIGH,digitalRead(ChA_enablePin));

    motor.retract();
    motor.cyclic();
    TEST_ASSERT_EQUAL(Actuator::retracting,motor.getState());
    TEST_ASSERT_EQUAL(LOW,digitalRead(ChA_output1Pin));
    TEST_ASSERT_EQUAL(HIGH,digitalRead(ChA_output2Pin));
    TEST_ASSERT_EQUAL(HIGH,digitalRead(ChA_enablePin));
    delay(1500);
    motor.cyclic();
    TEST_ASSERT_EQUAL(Actuator::timedout,motor.getState());
    TEST_ASSERT_EQUAL(LOW,digitalRead(ChA_output1Pin));
    TEST_ASSERT_EQUAL(LOW,digitalRead(ChA_output2Pin));
    TEST_ASSERT_EQUAL(HIGH,digitalRead(ChA_enablePin));

    motor.relax();
    motor.cyclic();
}

void testNoFeedback(){
    motor.setSpeed(100);
    motor.setHasFeedback(false);
    motor.setFeedbackTime(1);
    motor.testExtFB = false;
    motor.testRetFB = false;

    motor.extend();
    motor.cyclic();
    TEST_ASSERT_EQUAL(Actuator::extending,motor.getState());
    TEST_ASSERT_EQUAL(HIGH,digitalRead(ChA_output1Pin));
    TEST_ASSERT_EQUAL(LOW,digitalRead(ChA_output2Pin));
    TEST_ASSERT_EQUAL(HIGH,digitalRead(ChA_enablePin));
    delay(1500);
    motor.cyclic();
    TEST_ASSERT_EQUAL(Actuator::extended,motor.getState());
    TEST_ASSERT_EQUAL(LOW,digitalRead(ChA_output1Pin));
    TEST_ASSERT_EQUAL(LOW,digitalRead(ChA_output2Pin));
    TEST_ASSERT_EQUAL(HIGH,digitalRead(ChA_enablePin));

    motor.retract();
    motor.cyclic();
    TEST_ASSERT_EQUAL(Actuator::retracting,motor.getState());
    TEST_ASSERT_EQUAL(LOW,digitalRead(ChA_output1Pin));
    TEST_ASSERT_EQUAL(HIGH,digitalRead(ChA_output2Pin));
    TEST_ASSERT_EQUAL(HIGH,digitalRead(ChA_enablePin));
    delay(1500);
    motor.cyclic();
    TEST_ASSERT_EQUAL(Actuator::retracted,motor.getState());
    TEST_ASSERT_EQUAL(LOW,digitalRead(ChA_output1Pin));
    TEST_ASSERT_EQUAL(LOW,digitalRead(ChA_output2Pin));
    TEST_ASSERT_EQUAL(HIGH,digitalRead(ChA_enablePin));

    motor.relax();
    motor.cyclic();
}

void runTests(){
    RUN_TEST(testSpeed);
    RUN_TEST(testState);
    RUN_TEST(testTimeout);
    RUN_TEST(testNoFeedback);


}

void setup(){
    delay(2000);
    UNITY_BEGIN();
    runTests();
    UNITY_END();
}

void loop(){
}
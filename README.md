# Actuator Driver IO
## Intro
This library uses the L293D H-bridge drivers with an Arduino Nano to drive a TT hobby motor. Extend and retract feedback sensors can be added to stop the motor when it is fully extended or retracted.
This is useful in robotic applications where an actuator is only required to be in two positions (extended and retracted). A major advantage of this actuator system is there is no limit to the stroke length. The motor will run until the feedback sensors are met (assuming the timeout is set up correctly).
## Parts list
- Arduino Nano
- L293D quad half-H driver
- 2 switches
- 5 LEDs
- 2 capacitors: 1uF and 0.1uF
- 5 resistors: 220 ohm
## Schematics
![Actuator_IO schematics](https://raw.githubusercontent.com/ScobyStudio-ElectricActuator/ActuatorDriver_IO/master/Actuator_IO.svg)

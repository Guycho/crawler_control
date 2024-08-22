#ifndef CONTROL_H
#define CONTROL_H
#include "config.h"
#include "input.h"
#include "steering_motor.h"
#include "wheel_motor.h"
struct ControlConfig {
    SteeringMotor *steering_motors[Config::num_steering];
    WheelMotor *wheel_motors[Config::num_wheels];
};

class Control {
   public:
    // Constructor
    Control();

    // Destructor
    ~Control();

    run();

   private:
    SteeringMotor m_steering_motors[Config::num_steering];
    WheelMotor m_wheel_motors[Config::num_wheels];
    uint8_t m_steering_mode;
    uint8_t m_speed_mode;
};

#endif  // CONTROL_H
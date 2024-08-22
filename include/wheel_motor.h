#ifndef WHEEL_MOTOR_H
#define WHEEL_MOTOR_H

#include "brushed_motor.h"
#include "config.h"

struct WheelMotorConfig {
    BrushedMotorConfig motor_config;
    float max_speed_pct;
};

class WheelMotor {
   public:
    WheelMotor();   // Constructor
    ~WheelMotor();  // Destructor

    void init(const WheelMotorConfig& config);  // Method to start the motor
    void setSpeedPct(int speed_pct);            // Method to set the motor speed percentage
    void start();
    void stop();

   private:
    BrushedMotor m_motor;  // Variable to store the motor

    int m_speed_pct;        // Variable to store the motor speed percentage
    float m_max_speed_pct;  // Variable to store the motor max speed
};

#endif  // WHEEL_MOTOR_H
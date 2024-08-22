#ifndef BRUSHED_MOTOR_H
#define BRUSHED_MOTOR_H

#include <ESP32Servo.h>

struct WheelMotorConfig {
    uint8_t pin;
    uint16_t min_pulse;
    uint16_t max_pulse;
};

class BrushedMotor {
   public:
    BrushedMotor();   // Constructor
    ~BrushedMotor();  // Destructor

    void init();                 // Method to start the motor
    void stop();                 // Method to stop the motor
    void setSpeed(float speed);  // Method to set the motor speed

   private:
    Servo m_servo_output;  // Variable to store the motor
    float m_speed;         // Variable to store the motor speed
};

#endif  // BRUSHED_MOTOR_H
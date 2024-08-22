#ifndef STEERING_MOTOR_H
#define STEERING_MOTOR_H

#include "P_controller.h"
#include "brushed_motor.h"
#include "rotational_encoder.h"

struct SteeringMotorConfig {
    uint8_t servo_pin;
    uint16_t servo_min_pulse;
    uint16_t servo_max_pulse;
    uint8_t encoder_pin;
    bool encoder_reset;
    float encoder_offset;
    float controller_Kp;
    float controller_max_output;
    float max_angle;
    float max_speed_pct;
};

class SteeringMotor {
   public:
    SteeringMotor();   // Constructor
    ~SteeringMotor();  // Destructor

    void init(const SteeringMotorConfig& config);
    void setAngle();  // Method to set angle
    void stop();      // Method to stop the motor
    void run();       // Method to run the motor

   private:
    BrushedMotor m_motor;         // Variable to store the motor
    RotationalEncoder m_encoder;  // Variable to store the encoder
    PController m_controller;     // Variable to store the controller
};

#endif  // STEERING_MOTOR_H
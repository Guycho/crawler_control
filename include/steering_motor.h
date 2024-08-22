#ifndef STEERING_MOTOR_H
#define STEERING_MOTOR_H

#include "P_controller.h"
#include "brushed_motor.h"
#include "rotational_encoder.h"

struct SteeringMotorConfig {
    BrushedMotorConfig motor_config;
    RotationalEncoderConfig encoder_config;
    PControllerConfig controller_config;
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
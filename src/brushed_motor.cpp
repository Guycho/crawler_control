#include <brushed_motor.h>

BrushedMotor::BrushedMotor() {}   // Constructor
BrushedMotor::~BrushedMotor() {}  // Destructor definition

void BrushedMotor::init(const BrushedMotorConfig &config) {
    m_servo_output.attach(config.pin, config.min_pulse,
      config.max_pulse);  // Attaching the motor to the pin and setting the min and max pulse
}  // Method to start the motor

void BrushedMotor::setMicroSeconds(uint16_t micro_seconds) {
    m_servo_output.writeMicroseconds(micro_seconds);  // Setting the motor speed
}  // Method to set the motor speed

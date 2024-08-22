#include "config.h"
#include "steering_motor.h"
#include "wheel_motor.h"

WheelMotor wheel_motors[config::num_wheels];
SteeringMotor steering_motors[config::num_steering];

void setup() {
    for (uint8_t i = 0; i < config::num_wheels; i++) {
        WheelMotorConfig wheel_motor_config;
        wheel_motor_config.servo_pin = config::esc::wheel::pin[i];
        wheel_motor_config.servo_min_pulse = config::esc::wheel::min_pulse[i];
        wheel_motor_config.servo_max_pulse = config::esc::wheel::max_pulse[i];
        wheel_motor_config.max_speed_pct = 100;
        wheel_motors[i].init(wheel_motor_config);
    }

    for (uint8_t i = 0; i < config::num_steering; i++) {
        SteeringMotorConfig steering_motor_config;
        steering_motor_config.servo_pin = config::esc::steering::pin[i];
        steering_motor_config.servo_min_pulse = config::esc::steering::min_pulse[i];
        steering_motor_config.servo_max_pulse = config::esc::steering::max_pulse[i];
        steering_motor_config.encoder_pin = config::encoder::pin[i];
        steering_motor_config.encoder_reset = false;
        steering_motor_config.encoder_offset = config::encoder::offset[i];
        steering_motor_config.controller_Kp = config::controller::Kp[i];
        steering_motor_config.controller_max_output = config::controller::max_output[i];
        steering_motor_config.max_angle = 90;
        steering_motor_config.max_speed_pct = 100;
        steering_motors[i].init(steering_motor_config);
    }
}
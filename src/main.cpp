#include <Chrono.h>

#include "coilover_adjuster.h"
#include "config.h"
#include "control.h"
#include "input.h"
#include "mav_bridge.h"
#include "steering_motor.h"
#include "wheel_motor.h"

Chrono print_timer;
WheelMotor wheel_motors[Config::num_wheels];
SteeringMotor steering_motors[Config::num_steering];
CoiloverAdjuster coilover_adjusters[Config::num_coilover];
MavBridge mav_bridge;
Control control;
void setup() {
    Serial.begin(9600);
    print_timer.start();
    for (uint8_t i = 0; i < Config::num_wheels; i++) {
        WheelMotorConfig wheel_motor_config;
        wheel_motor_config.motor_config.pin = Config::Esc::Wheel::pin[i];
        wheel_motor_config.motor_config.min_pulse = Config::Esc::Wheel::min_pulse[i];
        wheel_motor_config.motor_config.max_pulse = Config::Esc::Wheel::max_pulse[i];
        wheel_motor_config.max_speed_pct = Config::Wheel::max_speed_pct[i];
        wheel_motors[i].init(wheel_motor_config);
    }

    for (uint8_t i = 0; i < Config::num_steering; i++) {
        SteeringMotorConfig steering_motor_config;
        steering_motor_config.motor_config.pin = Config::Esc::Steering::pin[i];
        steering_motor_config.motor_config.min_pulse = Config::Esc::Steering::min_pulse[i];
        steering_motor_config.motor_config.max_pulse = Config::Esc::Steering::max_pulse[i];
        steering_motor_config.encoder_config.pin = Config::Encoder::pin[i];
        steering_motor_config.encoder_config.reverse = false;
        steering_motor_config.encoder_config.offset = Config::Encoder::offset[i];
        steering_motor_config.controller_config.Kp = Config::Controller::Kp[i];
        steering_motor_config.controller_config.max_output = Config::Controller::max_output[i];
        steering_motor_config.max_angle = Config::Steering::max_angle[i];
        steering_motor_config.max_speed_pct = Config::Steering::max_speed_pct[i];
        steering_motors[i].init(steering_motor_config);
    }
    for (uint8_t i = 0; i < Config::num_coilover; i++) {
        CoiloverAdjusterConfig coilover_adjuster_config;
        coilover_adjuster_config.pin = Config::Esc::Coilover::pin[i];
        coilover_adjuster_config.min_pulse = Config::Esc::Coilover::min_pulse[i];
        coilover_adjuster_config.max_pulse = Config::Esc::Coilover::max_pulse[i];
        coilover_adjuster_config.reverse = Config::Coilover::reverse[i];
        coilover_adjuster_config.controller_config.Kp = Config::Coilover::Controller::Kp[i];
        coilover_adjuster_config.controller_config.max_output =
          Config::Coilover::Controller::max_output[i];
        coilover_adjusters[i].init(coilover_adjuster_config);
    }

    InputControllerConfig input_config;
    input_config.mac = Config::PS4Controller::mac;
    input_config.dead_band = Config::PS4Controller::dead_band;
    init_ps4(input_config);

    MavBridgeConfig mav_config;
    mav_config.serial = Config::MavlinkBridge::serial;
    mav_config.baudrate = Config::MavlinkBridge::baudrate;
    mav_config.system_id = Config::MavlinkBridge::system_id;
    mav_config.component_id = Config::MavlinkBridge::component_id;
    mav_config.message_rate = Config::MavlinkBridge::message_rate;
    mav_config.is_alive_timeout = Config::MavlinkBridge::is_alive_timeout;
    mav_bridge.init(mav_config);

    ControlConfig control_config;
    for (uint8_t i = 0; i < Config::num_wheels; i++) {
        control_config.wheel_motors[i] = &wheel_motors[i];
    }
    for (uint8_t i = 0; i < Config::num_steering; i++) {
        control_config.steering_motors[i] = &steering_motors[i];
    }
    for (uint8_t i = 0; i < Config::num_coilover; i++) {
        control_config.coilover_adjusters[i] = &coilover_adjusters[i];
    }
    control_config.mav_bridge = &mav_bridge;
    control.init(control_config);
}

void loop() {
    control.run();
    if (print_timer.hasPassed(500, true)) {
        // Serial.print("FR steering: ");
        // Serial.print(steering_motors[FR].getDesAngle());
        // Serial.print(" RR steering: ");
        // Serial.print(steering_motors[RR].getDesAngle());
        // Serial.print(" RL steering: ");
        // Serial.print(steering_motors[RL].getDesAngle());
        // Serial.print(" FL steering: ");
        // Serial.print(steering_motors[FL].getDesAngle());
        // Serial.print(" FR speed: ");
        // Serial.print(wheel_motors[FR].getSpeedPct());
        // Serial.print(" RR speed: ");
        // Serial.print(wheel_motors[RR].getSpeedPct());
        // Serial.print(" RL speed: ");   
        // Serial.print(wheel_motors[RL].getSpeedPct());
        // Serial.print(" FL speed: ");
        // Serial.print(wheel_motors[FL].getSpeedPct());
        // Serial.print(" arm: ");
        // Serial.print(control.get_arm_enabled());
        // Serial.print(" steering mode: ");
        // Serial.print(control.get_steering_mode());
        // Serial.print(" steering: ");
        // Serial.print(control.get_steering());
        // Serial.print(" throttle mode: ");
        // Serial.print(control.get_throttle_mode());
        // Serial.print(" throttle: ");
        // Serial.print(control.get_throttle());
        // Serial.print(" coilover mode: ");
        // Serial.println(control.get_coilover_mode());
        // Serial.println();
    }
}
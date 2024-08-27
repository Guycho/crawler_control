#include "control.h"

Control::Control() {}
Control::~Control() {}

void Control::init(const ControlConfig &config) {
    for (int i = 0; i < Config::num_steering; i++) {
        m_steering_motors[i] = *config.steering_motors[i];
    }
    for (int i = 0; i < Config::num_wheels; i++) {
        m_wheel_motors[i] = *config.wheel_motors[i];
    }
    for (int i = 0; i < Config::num_coilover; i++) {
        m_coilover_adjusters[i] = *config.coilover_adjusters[i];
        m_coilover_adjusters[i].reset();
    }
    m_mav_bridge = *config.mav_bridge;
    m_arm_enabled = false;
    float m_throttle = 0;
    float m_steering = 0;
}
void Control::run() {
    if (get_arm_toggle()) {
        m_arm_enabled = !m_arm_enabled;
    }
    if (get_steering_mode_toggle()) {
        m_steering_mode = (m_steering_mode + 1) % NUM_STEERING_MODES;
    }
    if (get_throttle_mode_toggle()) {
        m_throttle_mode = (m_throttle_mode + 1) % NUM_THROTTLE_MODES;
    }
    if (get_coilover_mode_toggle()) {
        m_coilover_mode = (m_coilover_mode + 1) % NUM_COILOVER_MODES;
    }
    m_mav_bridge.run();
    m_inertial_data = m_mav_bridge.get_inertial_data();
    m_steering = get_steering();
    m_throttle = get_throttle();
    steering_state_machine_run(m_arm_enabled, m_steering, m_steering_mode);
    throttle_state_machine_run(m_arm_enabled, m_throttle, m_throttle_mode);
}

void Control::steering_state_machine_run(bool arm_enabled, float steering, uint8_t steering_mode) {
    if (arm_enabled) {
        switch (steering_mode) {
            case FRONT:
                m_steering_motors[FR].setAngle(steering);
                m_steering_motors[FL].setAngle(steering);
                m_steering_motors[RR].setAngle(0);
                m_steering_motors[RL].setAngle(0);
                break;
            case REAR:
                m_steering_motors[FR].setAngle(0);
                m_steering_motors[FL].setAngle(0);
                m_steering_motors[RR].setAngle(-steering);
                m_steering_motors[RL].setAngle(-steering);
                break;
            case ALL:
                m_steering_motors[FR].setAngle(steering);
                m_steering_motors[FL].setAngle(steering);
                m_steering_motors[RR].setAngle(-steering);
                m_steering_motors[RL].setAngle(-steering);
                break;
            case CRAB:
                m_steering_motors[FR].setAngle(steering);
                m_steering_motors[FL].setAngle(steering);
                m_steering_motors[RR].setAngle(steering);
                m_steering_motors[RL].setAngle(steering);
                break;
            case S_PIVOT:
                m_steering_motors[FR].setAngle(-45);
                m_steering_motors[FL].setAngle(45);
                m_steering_motors[RR].setAngle(45);
                m_steering_motors[RL].setAngle(-45);
                break;
        }
    } else {
        m_steering_motors[FR].setAngle(0);
        m_steering_motors[FL].setAngle(0);
        m_steering_motors[RR].setAngle(0);
        m_steering_motors[RL].setAngle(0);
    }
}
void Control::throttle_state_machine_run(bool arm_enabled, float throttle, uint8_t throttle_mode) {
    if (arm_enabled) {
        switch (throttle_mode) {
            case FWD:
                m_wheel_motors[FR].setSpeedPct(throttle);
                m_wheel_motors[FL].setSpeedPct(throttle);
                m_wheel_motors[RR].setSpeedPct(0);
                m_wheel_motors[RL].setSpeedPct(0);
                break;
            case RWD:
                m_wheel_motors[FR].setSpeedPct(0);
                m_wheel_motors[FL].setSpeedPct(0);
                m_wheel_motors[RR].setSpeedPct(throttle);
                m_wheel_motors[RL].setSpeedPct(throttle);
                break;
            case AWD:
                m_wheel_motors[FR].setSpeedPct(throttle);
                m_wheel_motors[FL].setSpeedPct(throttle);
                m_wheel_motors[RR].setSpeedPct(throttle);
                m_wheel_motors[RL].setSpeedPct(throttle);
                break;
            case T_PIVOT:
                m_wheel_motors[FR].setSpeedPct(throttle);
                m_wheel_motors[FL].setSpeedPct(-throttle);
                m_wheel_motors[RR].setSpeedPct(throttle);
                m_wheel_motors[RL].setSpeedPct(-throttle);
                break;
        }
    } else {
        m_wheel_motors[FR].setSpeedPct(0);
        m_wheel_motors[FL].setSpeedPct(0);
        m_wheel_motors[RR].setSpeedPct(0);
        m_wheel_motors[RL].setSpeedPct(0);
    }
}
void Control::coilover_state_machine_run(bool arm_enabled, uint8_t coilover_mode,
  RollPitch roll_pitch, RollPitch des_roll_pitch) {
    if (arm_enabled) {
        switch (coilover_mode) {
            case OFF:
                m_coilover_adjusters[FR].reset();
                m_coilover_adjusters[RR].reset();
                m_coilover_adjusters[RL].reset();
                m_coilover_adjusters[FL].reset();
                break;
            case STABILIZE:
                RollPitch m_roll_pitch = Utils::Calcs::rotateRollPitch45Degrees(roll_pitch);
                RollPitch m_des_roll_pitch = Utils::Calcs::rotateRollPitch45Degrees(des_roll_pitch);

                float m_des_coilover_diff[Config::num_coilover];
                m_des_coilover_diff[FR] = m_roll_pitch.pitch - m_des_roll_pitch.pitch;
                m_des_coilover_diff[RR] = m_roll_pitch.roll - m_des_roll_pitch.roll;
                m_des_coilover_diff[RL] = -m_roll_pitch.pitch + m_des_roll_pitch.pitch;
                m_des_coilover_diff[FL] = -m_roll_pitch.roll + m_des_roll_pitch.roll;
                m_coilover_adjusters[FR].run(m_des_coilover_diff[FR]);
                m_coilover_adjusters[RR].run(m_des_coilover_diff[RR]);
                m_coilover_adjusters[RL].run(m_des_coilover_diff[RL]);
                m_coilover_adjusters[FL].run(m_des_coilover_diff[FL]);
                break;
        }
    } else {
        m_coilover_adjusters[FR].reset();
        m_coilover_adjusters[RR].reset();
        m_coilover_adjusters[RL].reset();
        m_coilover_adjusters[FL].reset();
    }
}
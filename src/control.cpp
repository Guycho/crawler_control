#include "control.h"

Control::Control() {}
Control::~Control() {}

void Control::init(const ControlConfig &config) {
    m_hb_timer.start();
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
    m_throttle = 0;
    m_steering = 0;
    m_pivot_steering_angle = config.pivot_steering_angle;
}

void Control::run() {
    m_mav_bridge.run();
    m_inertial_data = m_mav_bridge.get_inertial_data();
    InputControllerData input_data = get_input_data();
    if (input_data.new_data) {
        if (input_data.arm_toggle) {
            m_arm_enabled = !m_arm_enabled;
        }
        if (input_data.steering_mode_toggle) {
            m_steering_mode = (m_steering_mode + 1) % NUM_STEERING_MODES;
        }
        if (input_data.throttle_mode_toggle) {
            m_throttle_mode = (m_throttle_mode + 1) % NUM_THROTTLE_MODES;
        }
        if (input_data.coilover_mode_toggle) {
            m_coilover_mode = (m_coilover_mode + 1) % NUM_COILOVER_MODES;
        }
        m_steering = input_data.steering;
        m_throttle = input_data.throttle;
        if (input_data.roll_pitch_reset) {
            m_des_roll_pitch = {0, 0};
        }
        if (input_data.ride_height_reset) {
            m_ride_height = 0;
        }
        if (input_data.roll_right) {
            m_des_roll_pitch.roll += 0.5;
        } else if (input_data.roll_left) {
            m_des_roll_pitch.roll -= 0.5;
        }
        if (input_data.pitch_forward) {
            m_des_roll_pitch.pitch += 0.5;
        } else if (input_data.pitch_backward) {
            m_des_roll_pitch.pitch -= 0.5;
        }
        if (input_data.ride_height_up) {
            m_ride_height += 0.5;
        } else if (input_data.ride_height_down) {
            m_ride_height -= 0.5;
        }
        if (m_steering_mode == S_PIVOT) {
            m_throttle_mode = T_PIVOT;
        } else if (m_steering_mode != S_PIVOT && m_throttle_mode == T_PIVOT) {
            m_throttle_mode = AWD;
        }
        m_hb_timer.restart();
    }
    if (m_hb_timer.hasPassed(100, true)) {
        m_arm_enabled = false;
    }
    RollPitch roll_pitch = {m_inertial_data.orientation.x, m_inertial_data.orientation.y};

    steering_state_machine_run(m_arm_enabled, m_steering, m_steering_mode);
    throttle_state_machine_run(m_arm_enabled, m_throttle, m_throttle_mode);
    coilover_state_machine_run(m_arm_enabled, m_coilover_mode, roll_pitch, m_des_roll_pitch,
      m_ride_height);
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
                m_steering_motors[FR].setAngle(-m_pivot_steering_angle);
                m_steering_motors[FL].setAngle(m_pivot_steering_angle);
                m_steering_motors[RR].setAngle(m_pivot_steering_angle);
                m_steering_motors[RL].setAngle(-m_pivot_steering_angle);
                break;
        }
    } else {
        m_steering_motors[FR].stop();
        m_steering_motors[FL].stop();
        m_steering_motors[RR].stop();
        m_steering_motors[RL].stop();
    }
    m_steering_motors[FR].run();
    m_steering_motors[FL].run();
    m_steering_motors[RR].run();
    m_steering_motors[RL].run();
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
  RollPitch roll_pitch, RollPitch des_roll_pitch, float ride_height) {
    if (arm_enabled) {
        switch (coilover_mode) {
            case OFF:
                m_coilover_adjusters[RL].reset();
                m_coilover_adjusters[FL].reset();
                break;
            case STABILIZE:
                RollPitch m_roll_pitch = Utils::Calcs::rotateRollPitch45Degrees(roll_pitch);
                RollPitch m_des_roll_pitch = Utils::Calcs::rotateRollPitch45Degrees(des_roll_pitch);
                m_coilover_adjusters[FR].set_base_pos(ride_height);
                m_coilover_adjusters[RR].set_base_pos(ride_height);
                m_coilover_adjusters[RL].set_base_pos(ride_height);
                m_coilover_adjusters[FL].set_base_pos(ride_height);
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

bool Control::get_arm_enabled() { return m_arm_enabled; }
uint8_t Control::get_steering_mode() { return m_steering_mode; }
uint8_t Control::get_throttle_mode() { return m_throttle_mode; }
uint8_t Control::get_coilover_mode() { return m_coilover_mode; }
FourValues Control::get_steering_values() {
    FourValues values;
    values.fr = m_steering_motors[FR].getDesAngle();
    values.fl = m_steering_motors[FL].getDesAngle();
    values.rr = m_steering_motors[RR].getDesAngle();
    values.rl = m_steering_motors[RL].getDesAngle();
    return values;
}
FourValues Control::get_throttle_values() {
    FourValues values;
    values.fr = m_wheel_motors[FR].getSpeedPct();
    values.fl = m_wheel_motors[FL].getSpeedPct();
    values.rr = m_wheel_motors[RR].getSpeedPct();
    values.rl = m_wheel_motors[RL].getSpeedPct();
    return values;
}
FourValues Control::get_coilover_values() {
    FourValues values;
    values.fr = m_coilover_adjusters[FR].get_pos();
    values.fl = m_coilover_adjusters[FL].get_pos();
    values.rr = m_coilover_adjusters[RR].get_pos();
    values.rl = m_coilover_adjusters[RL].get_pos();
    return values;
}

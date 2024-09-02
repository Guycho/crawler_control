#ifndef CONTROL_H
#define CONTROL_H
#include <Chrono.h>

#include "coilover_adjuster.h"
#include "config.h"
#include "input.h"
#include "mav_bridge.h"
#include "steering_motor.h"
#include "wheel_motor.h"

using RollPitch = Utils::Calcs::RollPitch;

enum SteeringMode { ALL = 0, FRONT = 1, REAR = 2, CRAB = 3, S_PIVOT = 4 };
enum ThrottleMode { AWD = 0, FWD = 1, RWD = 2, T_PIVOT = 3 };
enum CoiloverMode { OFF = 0, STABILIZE = 1 };
enum Positions { FR = 0, RR = 1, RL = 2, FL = 3 };

struct ControlConfig {
    SteeringMotor *steering_motors[Config::num_steering];
    WheelMotor *wheel_motors[Config::num_wheels];
    CoiloverAdjuster *coilover_adjusters[Config::num_coilover];
    MavBridge *mav_bridge;
};

class Control {
   public:
    // Constructor
    Control();

    // Destructor
    ~Control();

    void init(const ControlConfig &config);

    void run();
    bool get_arm_enabled();
    uint8_t get_steering_mode();
    uint8_t get_throttle_mode();
    uint8_t get_coilover_mode();
    float get_steering();
    float get_throttle();

   private:
    Chrono m_hb_timer;

    SteeringMotor m_steering_motors[Config::num_steering];
    WheelMotor m_wheel_motors[Config::num_wheels];
    CoiloverAdjuster m_coilover_adjusters[Config::num_coilover];
    MavBridge m_mav_bridge;

    InertialData m_inertial_data;
    RollPitch m_des_roll_pitch;

    void steering_state_machine_run(bool arm_enabled, float steering, uint8_t steering_mode);
    void throttle_state_machine_run(bool arm_enabled, float throttle, uint8_t throttle_mode);
    void coilover_state_machine_run(bool arm_enabled, uint8_t coilover_mode, RollPitch roll_pitch,
      RollPitch des_roll_pitch, float ride_height);
    static constexpr SteeringMode steering_modes[] = {FRONT, REAR, ALL, CRAB, S_PIVOT};
    static constexpr uint8_t NUM_STEERING_MODES =
      sizeof(steering_modes) / sizeof(steering_modes[0]);
    static constexpr ThrottleMode throttle_modes[] = {FWD, RWD, AWD, T_PIVOT};
    static constexpr uint8_t NUM_THROTTLE_MODES =
      sizeof(throttle_modes) / sizeof(throttle_modes[0]);
    static constexpr CoiloverMode coilover_modes[] = {OFF, STABILIZE};
    static constexpr uint8_t NUM_COILOVER_MODES =
      sizeof(coilover_modes) / sizeof(coilover_modes[0]);
    uint8_t m_steering_mode;
    uint8_t m_throttle_mode;
    uint8_t m_coilover_mode;
    bool m_arm_enabled = false;
    float m_throttle = 0;
    float m_steering = 0;
    float m_ride_height = 0;
};

#endif  // CONTROL_H
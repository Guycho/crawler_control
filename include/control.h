#ifndef CONTROL_H
#define CONTROL_H
#include "config.h"
#include "input.h"
#include "steering_motor.h"
#include "wheel_motor.h"

enum SteeringMode { FRONT = 0, REAR = 1, ALL = 2, CRAB = 3, S_PIVOT = 4 };
enum ThrottleMode { FWD = 0, RWD = 1, AWD = 2, T_PIVOT = 3 };
enum POSITIONS { FR = 0, RR = 1, RL = 2, FL = 3 };

struct ControlConfig {
    SteeringMotor *steering_motors[Config::num_steering];
    WheelMotor *wheel_motors[Config::num_wheels];
};

class Control {
   public:
    // Constructor
    Control();

    // Destructor
    ~Control();

    void init(const ControlConfig &config);

    void run();

   private:
    SteeringMotor m_steering_motors[Config::num_steering];
    WheelMotor m_wheel_motors[Config::num_wheels];

    void steering_state_machine_run(bool arm_enabled, float steering, uint8_t steering_mode);
    void throttle_state_machine_run(bool arm_enabled, float throttle, uint8_t throttle_mode);
    static constexpr SteeringMode steering_modes[] = {FRONT, REAR, ALL, CRAB, S_PIVOT};
    static constexpr uint8_t NUM_STEERING_MODES =
      sizeof(steering_modes) / sizeof(steering_modes[0]);
    static constexpr ThrottleMode throttle_modes[] = {FWD, RWD, AWD, T_PIVOT};
    static constexpr uint8_t NUM_THROTTLE_MODES =
      sizeof(throttle_modes) / sizeof(throttle_modes[0]);
    uint8_t m_steering_mode;
    uint8_t m_throttle_mode;
    bool m_arm_enabled = false;
    float m_throttle;
    float m_steering;
};

#endif  // CONTROL_H
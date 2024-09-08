#include "coilover_adjusters_logic.h"

CoiloverAdjustersLogic::CoiloverAdjustersLogic() {}   // Constructor
CoiloverAdjustersLogic::~CoiloverAdjustersLogic() {}  // Destructor

void CoiloverAdjustersLogic::init(const CoiloverAdjustersLogicConfig& config) {
    for (int i = 0; i < Config::num_wheels; i++) {
        m_coilover_adjusters[i] = *config.CoiloverAdjusters[i];
        m_ground_sensors[i] = *config.GroundSensors[i];
        m_p_controllers[i].init(config.PControllers[i]);
    }
}  // Method to start the motor
void CoiloverAdjustersLogic::run(bool arm_enabled, uint8_t coilover_mode, RollPitch roll_pitch,
  RollPitch des_roll_pitch, float ride_height) {
    if (arm_enabled) {
        m_shock_lengths[FR] = m_ground_sensors[FR].get_distance();
        m_shock_lengths[RR] = m_ground_sensors[RR].get_distance();
        m_shock_lengths[RL] = m_ground_sensors[RL].get_distance();
        m_shock_lengths[FL] = m_ground_sensors[FL].get_distance();
        m_ground_contact[FR] = m_ground_sensors[FR].get_ground_contact();
        m_ground_contact[RR] = m_ground_sensors[RR].get_ground_contact();
        m_ground_contact[RL] = m_ground_sensors[RL].get_ground_contact();
        m_ground_contact[FL] = m_ground_sensors[FL].get_ground_contact();
        RollPitch m_roll_pitch = Utils::Calcs::rotateRollPitch45Degrees(roll_pitch);
        RollPitch m_des_roll_pitch = Utils::Calcs::rotateRollPitch45Degrees(des_roll_pitch);
        switch (coilover_mode) {
            case OFF:
                m_coilover_adjusters[FR].reset();
                m_coilover_adjusters[RR].reset();
                m_coilover_adjusters[RL].reset();
                m_coilover_adjusters[FL].reset();
                break;
            case STABILIZE:
                break;
            case KEEP_CONTACT:
                keep_contact();
                break;
            case RIDE_HEIGHT:
                if (m_ground_contact[FR] && m_ground_contact[RR] && m_ground_contact[RL] &&
                    m_ground_contact[FL]) {
                    for (int i = 0; i < Config::num_wheels; i++) {
                        m_coilover_adjusters[i].set_speed(-1);
                    }
                } else {
                    keep_contact();
                }
                break;
        }
    } else {
        m_coilover_adjusters[FR].reset();
        m_coilover_adjusters[RR].reset();
        m_coilover_adjusters[RL].reset();
        m_coilover_adjusters[FL].reset();
    }
}
uint8_t CoiloverAdjustersLogic::get_num_of_coilover_modes() { return NUM_COILOVER_MODES; }

void CoiloverAdjustersLogic::keep_contact() {
    for (int i = 0; i < Config::num_wheels; i++) {
        if (m_ground_contact[i]) {
            m_coilover_adjusters[i].set_speed(100);
        }
        m_coilover_adjusters[i].run();
    }
}
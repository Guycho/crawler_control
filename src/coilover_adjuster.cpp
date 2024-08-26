#include <coilover_adjuster.h>

CoiloverAdjuster::CoiloverAdjuster() {}   // Constructor
CoiloverAdjuster::~CoiloverAdjuster() {}  // Destructor definition

void CoiloverAdjuster::init(const CoiloverAdjusterConfig &config) {
    m_min_pulse = config.min_pulse;
    m_max_pulse = config.max_pulse;
    m_mid_pulse = (config.min_pulse + config.max_pulse) / 2;
    m_servo_output.attach(config.pin, m_min_pulse,
      m_max_pulse);  // Attaching the motor to the pin and setting the min and max pulse
    m_servo_output.writeMicroseconds(m_mid_pulse);  // Setting the coilover position to 0
}  // Method to start the motor

void CoiloverAdjuster::move_to_pos(float pos) {
    uint16_t micro_seconds = Utils::Calcs::map_float(pos, -100, 100, m_min_pulse, m_max_pulse);
    if (m_reverse) {
        micro_seconds = m_max_pulse - micro_seconds + m_min_pulse;
    }
    m_servo_output.writeMicroseconds(micro_seconds);  // Setting the coilover position
}  // Method to set the coilover position

void CoiloverAdjuster::reset() { move_to_pos(0); }  // Method to reset the coilover position

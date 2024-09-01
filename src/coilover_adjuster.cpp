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
    m_reverse = config.reverse;
    m_controller.init(config.controller_config);  // Initializing the controller
}  // Method to start the coilover adjuster

void CoiloverAdjuster::set_pos(float pos) {
    uint16_t micro_seconds = Utils::Calcs::map_float(pos, -100, 100, m_min_pulse, m_max_pulse);
    if (m_reverse) {
        micro_seconds = m_max_pulse - micro_seconds + m_min_pulse;
    }
    m_servo_output.writeMicroseconds(micro_seconds);  // Setting the coilover position
}  // Method to set the coilover position

void CoiloverAdjuster::reset() { set_pos(0); }  // Method to reset the coilover position

void CoiloverAdjuster::set_base_pos(float pos) {
    m_base_pos = pos;  // Setting the base position of the coilover
}
void CoiloverAdjuster::run(float diff) {
    float output = m_controller.calculateOutput(diff);  // Running the controller
    m_current_pos = m_base_pos + output;
    set_pos(m_current_pos + m_base_pos);  // Setting the coilover position
}  // Method to run the coilover adjuster

float CoiloverAdjuster::get_pos() { return m_current_pos; }  // Method to get the coilover position

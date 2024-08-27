#ifndef COILOVER_ADJUSTER_H
#define COILOVER_ADJUSTER_H

#include <ESP32Servo.h>
#include "utils.h"
#include "P_controller.h"

struct CoiloverAdjusterConfig {
    uint8_t pin;
    uint16_t min_pulse;
    uint16_t max_pulse;
    bool reverse;
    PControllerConfig controller_config;
};

class CoiloverAdjuster {
   public:
    CoiloverAdjuster();  // Constructor
    ~CoiloverAdjuster();  // Destructor

    void init(const CoiloverAdjusterConfig &config);
    void set_pos(float pos);
    void reset();
    void run(float diff);


   private:
    Servo m_servo_output;
    PController m_controller;

    uint16_t m_min_pulse;
    uint16_t m_max_pulse;
    uint16_t m_mid_pulse;
    bool m_reverse;
};

#endif  // COILOVER_ADJUSTER_H
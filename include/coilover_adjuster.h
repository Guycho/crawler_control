#ifndef COILOVER_ADJUSTER_H
#define COILOVER_ADJUSTER_H

#include <ESP32Servo.h>
#include "utils.h"

struct CoiloverAdjusterConfig {
    uint8_t pin;
    uint16_t min_pulse;
    uint16_t max_pulse;
    bool reverse;
};

class CoiloverAdjuster {
   public:
    CoiloverAdjuster();  // Constructor
    ~CoiloverAdjuster();  // Destructor

    void init(const CoiloverAdjusterConfig &config);
    void move_to_pos(float pos);
    void reset();

   private:
    Servo m_servo_output;
    uint16_t m_min_pulse;
    uint16_t m_max_pulse;
    uint16_t m_mid_pulse;
    bool m_reverse;
};

#endif  // COILOVER_ADJUSTER_H
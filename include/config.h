#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

namespace config {
    //general config
    const uint8_t num_wheels = 4;
    const uint8_t num_steering = 4;
    //pwm pins for esc and steering
    namespace esc_pin {
        const uint8_t wheels_pwm[4] = {2, 3, 4, 5};
        const uint8_t steering_pwm[4] = {6, 7, 8, 9};
    }
    namespace encoder_pin {
        const uint8_t steering_encoder[4] = {14, 15, 16, 17};
    }
}
#endif  // CONFIG_H
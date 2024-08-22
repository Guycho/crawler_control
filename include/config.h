#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

namespace config {
    namespace mavlink {
        const uint8_t ;
        const uint8_t system_id = 1;
        const uint8_t component_id = 0;
        const uint8_t steering_channel = 12;
        const uint8_t throttle_channel = 5;
        const uint8_t message_rate = 250;
    }  // namespace mavlink
    namespace PS4_controller {
        const char *mac = "A0:DD:6C:03:9A:EE";
        const float dead_band = 5;
    }  // namespace PS4_controller
    namespace PID {
        const float kp = 5;
        const float ki = 0.01;
        const float kd = 0.01;
        const float max_output = 100;
        const float integral_percentage = 30;
        const float low_pass_alpha = 0.0;
        const float high_pass_alpha = 0.0;
        const bool use_filters = false;
    }  // namespace PID
    namespace control {
        const float gyro_input_max = 2*PI;
        const float steering_input_max = 100;
        const float throttle_input_max = 100;
    }  // namespace control
}
#endif  // CONFIG_H
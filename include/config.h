#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

namespace Config {
const uint8_t num_wheels = 4;
const uint8_t num_steering = 4;

namespace Esc {
namespace Wheel {
const uint8_t pin[4] = {2, 3, 4, 5};
const uint16_t min_pulse[4] = {1000, 1000, 1000, 1000};
const uint16_t max_pulse[4] = {2000, 2000, 2000, 2000};
}  // namespace Wheel
namespace Steering {
const uint8_t pin[4] = {2, 3, 4, 5};
const uint16_t min_pulse[4] = {1000, 1000, 1000, 1000};
const uint16_t max_pulse[4] = {2000, 2000, 2000, 2000};
}  // namespace Steering
}  // namespace Esc
namespace Encoder {
const uint8_t pin[4] = {10, 11, 12, 13};
const bool reverse[4] = {false, false, false, false};
const float offset[4] = {0, 0, 0, 0};
}  // namespace Encoder
namespace Controller {
const float Kp[4] = {0.1, 0.1, 0.1, 0.1};
const float max_output[4] = {1, 1, 1, 1};
}  // namespace controller
namespace Wheel {
const float max_speed_pct[4] = {100, 100, 100, 100};
}  // namespace Wheel
namespace Steering {
const float max_angle[4] = {90, 90, 90, 90};
const float max_speed_pct[4] = {100, 100, 100, 100};
}  // namespace Steering
}  // namespace Config
#endif  // CONFIG_H
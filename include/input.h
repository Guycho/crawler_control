#ifndef INPUT_CONTROLLER_H
#define INPUT_CONTROLLER_H

#include <PS4Controller.h>

#include "esp_bt_device.h"
#include "esp_bt_main.h"
#include "esp_err.h"
#include "esp_gap_bt_api.h"
#include "utils.h"

struct InputControllerConfig {
    const char* mac;
    float dead_band;
};

void init_ps4(const InputControllerConfig config);
float get_throttle();
float get_steering();
bool get_steering_mode_toggle();
bool get_throttle_mode_toggle();
bool get_arm_toggle();


#endif  // INPUT_CONTROLLER_H
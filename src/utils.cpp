#include "utils.h"

namespace Utils {
namespace Calcs {
float map_float(float x, float in_min, float in_max, float out_min, float out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
float constrain_float(float x, float min, float max) {
    if (x < min) {
        return min;
    } else if (x > max) {
        return max;
    } else {
        return x;
    }
}
float calc_dead_band(float x, float max_output, float dead_band) {
    if (abs(x) < dead_band) {
        return 0;
    }
    int sign = x > 0 ? 1 : -1;
    float abs_temp = Utils::Calcs::map_float(abs(x), dead_band, max_output, 0, max_output);
    float scaled_temp = sign * abs_temp;
    return scaled_temp;
}

}  // namespace Calcs
}  // namespace Utils

#ifndef UTILS_H
#define UTILS_H

namespace Utils {
namespace Calcs {
float map_float(float x, float in_min, float in_max, float out_min, float out_max);
float constrain_float(float x, float min, float max);
}  // namespace calculators
}  // namespace utils
#endif  // UTILS_H
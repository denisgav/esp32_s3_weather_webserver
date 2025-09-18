#include <Arduino.h>
#include <stdbool.h>
#include "board_defines.h"

class LDR_wrapper
{
public:
    // Class constructor
    LDR_wrapper();
    // Initialize BME sensor
    bool init();
    // Sample sensor values
    bool sample_sensor_data();
    // Returns true if values were sampled from sensor
    bool get_is_sampled() const;

    // get sampled sensor values
    float get_sampled_ldr() const;
    bool get_is_initialized() const;

private:
    float sampled_ldr;
    bool is_sampled;
    bool is_initialized;
};
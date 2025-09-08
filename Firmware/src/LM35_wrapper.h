#include <Arduino.h>
#include <stdbool.h>
#include "board_defines.h"

class LM35_wrapper{
public:
    // Class constructor
    LM35_wrapper();
    // Initialize BME sensor
    bool init();
    // Sample sensor values
    bool sample_sensor_data();

    // get sampled sensor values
    float get_sampled_temperature() const;
    bool get_is_initialized() const;
private:
    float sampled_temperature;
    bool is_initialized;
};
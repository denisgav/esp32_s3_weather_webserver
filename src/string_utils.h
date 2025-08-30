#pragma once

#include <Arduino.h>

String int_to_string(uint32_t i, int num_of_digits);
String string_extend(String s, int num_of_symbols);
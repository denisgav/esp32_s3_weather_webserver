#pragma once

#include <Arduino.h>

enum string_align_e {
  STRING_ALIGN_LEFT,
  STRING_ALIGN_CENTER,
  STRING_ALIGN_RIGHT
}; 

String string_extend(String s, int num_of_symbols, string_align_e align);
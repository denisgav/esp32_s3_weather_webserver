#include "string_utils.h"

String int_to_string(uint32_t i, int num_of_digits){
    String i_s = String(i);
    while(i_s.length() < num_of_digits){
        i_s = "0" + i_s;
    }
    return i_s;
}

String string_extend(String s, int num_of_symbols){
    String i_s = s;
    while(i_s.length() < num_of_symbols){
        i_s = i_s + " ";
    }
    return i_s;
}
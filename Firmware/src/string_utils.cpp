#include "string_utils.h"

String string_extend(String s, int num_of_symbols, string_align_e align){
    String i_s = s;
    while(i_s.length() < num_of_symbols){
        switch(align)
        {
            STRING_ALIGN_LEFT:
            {
                i_s = i_s + " ";
                break;
            }
            STRING_ALIGN_CENTER:
            {
                if(i_s.length()%2)
                    i_s = i_s + " ";
                else
                    i_s = " " + i_s;
                break;
            }
            STRING_ALIGN_RIGHT:
            {
                i_s = " " + i_s;
                break;
            }
            default:
            {
                i_s = i_s + " ";
                break;
            }
        }
        
    }
    return i_s;
}
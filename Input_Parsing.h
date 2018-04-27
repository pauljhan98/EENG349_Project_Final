/*
  Input_Parsing.h - Library for Parsing graphing calculator input
  Created by Paul Han, 4/27/2018
*/


#ifndef Input_Parsing_h
#define Input_Parsing_h

#include "Arduino.h"

class Input_Parsing
{
  public:
    void Input_Parsing();

    //Start Input Parsing globals
    extern bool newInput = false; //flag for completion of current input; for input_parsing; global for now
    extern float assembled_number; //output of number parsing; global for now    
    extern byte assembled_count = 0; //number of assembled values in stack
    extern char operator_flags[21]; //char array for keeping track of which stack items are operators (includes X variable)
    extern char *operator_flags_ptr;
    extern bool x_operator_flag; //flag when X operator detected, triggers Arithmetic logic graph table
    //end input parsing globals
    
  private:
    byte digit_count = 0; //number of digits in number
    int RawInput; //from input device minus '0' to correlate ACSII values with actual numerical value
    int power; // power of 10 times digit to equal place (i.e. 4x10^2=400; power=2
    float top_number; //digit popped from top of stack
    float power_multiply; //10^2=100;100=power_multiply
    float base = 10.0; //10^2;base=10
    static boolean decimal_flag = false; //raised if decimal point is entered
    static int decimal_place = 0;//when triggered, keeps track of places behind decimal
    static byte negative_possible_flag = 0; //detects negative sign vs minus; 0=not negative;1=possibly negative;2=negative  
    bool graph_flag = false; //resets graph flag for new input
};

#endif

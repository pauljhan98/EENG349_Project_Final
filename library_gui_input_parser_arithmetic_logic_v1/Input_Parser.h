/*
   Input_Parser.h - Library for arduino graphing calculator
   Paul Han 4/27/18
*/
#ifndef Input_Parser_h
#define Input_Parser_h

#include "Arduino.h"
#include <StackArray.h> //for Input_Parsing

#define SS_PIN 8 //slave select for spiRam

extern StackArray <float> stack;
extern byte assembled_count;
extern bool graph_flag; //resets graph flag for new input (external)
extern byte assembled_count; //number of assembled values in stack
extern char operator_flags[21]; //char array for keeping track of which stack items are operators (includes X variable)
extern char *operator_flags_ptr;
extern bool x_operator_flag;
extern bool graph_flag;


class Input_Parser {
  public:
    Input_Parser();
    bool parse_input(char*& gui_input_ptr);

  private:
    byte digit_count = 0; //number of digits in number
    int RawInput; //from input device minus '0' to correlate ACSII values with actual numerical value
    int power; // power of 10 times digit to equal place (i.e. 4x10^2=400; power=2
    float top_number; //digit popped from top of stack
    float power_multiply; //10^2=100;100=power_multiply
    float base = 10.0; //10^2;base=10
    boolean decimal_flag = false; //raised if decimal point is entered
    int decimal_place = 0;//when triggered, keeps track of places behind decimal
    byte negative_possible_flag = 0; //detects negative sign vs minus; 0=not negative;1=possibly negative;2=negative
    float assembled_number; //output of number parsing
    bool newInput = false; //resets new Input to false (return)
};


#endif

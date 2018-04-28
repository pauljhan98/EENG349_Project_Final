/*
   Arithmetic_Logic.h - Library for arduino graphing calculator
   Paul Han 4/27/18
*/


#ifndef Arthmetic_Logic_h
#define Arthmetic_Lgoic_h

#include "Arduino.h"
#include <SPI.h>
#include <StackArray.h> //for Input_Parsing
#include <SpiRam_Extended.h> //for external spi ram storage

extern StackArray <float> stack;
extern SpiRAM SpiRam;
extern float x_start; //starting value of x //all following are temp values
extern float step_size; //step size of graph (value per pixel)
extern long iterations; //number of x iterations
extern byte assembled_count;
extern bool graph_flag; //resets graph flag for new input (external)
extern byte assembled_count; //number of assembled values in stack
extern char operator_flags[21]; //char array for keeping track of which stack items are operators (includes X variable)
extern char *operator_flags_ptr;
extern bool x_operator_flag;
extern bool graph_flag;


class Arithmetic_Logic {
  public:
    Arithmetic_Logic();
    bool calculate(long& output_start_address, long& output_end_address);

  private:
    float *stack_sort_array; //array to hold stack so it can be reversed
    int temp_array_size;
    float a; //number 1 or vairable x
    float b; //number 2 or variable x
    int c; //operator (num1 (operator) num 2)
    float x_value; //current value of x for table generation
    long i; //counter for iterations
    float output_buffer[2]; //output buffer for external sram transmission
    long address_counter; //external sram address counter
};


#endif

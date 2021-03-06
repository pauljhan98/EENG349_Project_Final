/*
   Arithmetic_Logic.cpp - Library for arduino graphing calculator
   Paul Han 4/27/18
*/

#include "Arduino.h"
#include <SPI.h>
#include <StackArray.h> //for Input_Parsing
#include <SpiRam_Extended.h> //for external spi ram storage
#include "Arithmetic_Logic.h"
#include "trig.h"

#define X_CENTER 0
#define Y_CENTER 0
#define TICK_HEIGHT 5

#define WEIGHT .075


//start arithmetic logic private
float *stack_sort_array; //array to hold stack so it can be reversed
int temp_array_size;
float a; //number 1 or vairable x
float b; //number 2 or variable x
int c; //operator (num1 (operator) num 2)
float x_value; //current value of x for table generation
long i = 0; //counter for iterations
float output_buffer[2]; //output buffer for external sram transmission
long address_counter = 0; //external sram address counter
int pop_count = 0; //keeps track of how many items have been popped off the stack
float logic_sine; //for trig function
float logic_cosine;
float logic_tangent;
float *logic_cos_point = &logic_cosine;
float *logic_sine_point = &logic_sine;
float *logic_tan_point = &logic_tangent;
//end arithmetic logic private


//references
long output_start_address; //start address of output in sram
long output_end_address; //end address of output in sram (actual last address is sizeof(output_buffer))
//end references

//start constructor
Arithmetic_Logic::Arithmetic_Logic() {
  //nothing here
}
//end constuctor


bool Arithmetic_Logic::calculate(long& output_start_address, long& output_end_address) {
  i = 0;
  a = 0;
  b = 0;
  c = 0;
  pop_count = 0;
  operator_flags_ptr = &operator_flags[0]; //reset operator flags pointer
  if (graph_flag == true) {    
  }
  //start initial stack reversal
  temp_array_size = stack.count(); //number of items in stack = [i]
  stack_sort_array = calloc(stack.count(), sizeof(float) );  //temp array size of stack
  for ( int i = 0; i < temp_array_size; i++) { //load stack into array
    *(stack_sort_array + i) = stack.pop();
  }
  //end initial stack reversal

  output_start_address = address_counter; //set initial address for output
  x_value = X_CENTER - (160 * WEIGHT); //set x value correct for first iteration
  
  do { //if x operator true run many time to generate table, else run once

    //start iteration counter
    if (x_operator_flag == true) {
      x_value += WEIGHT;
      
      i++;
     
      if (i == iterations) { //number of iterations reached
        x_operator_flag = false; //exit table generation (do..while)
      }

    }
    //end iteration counter

    //start stack reversal
    for ( int i = 0; i < temp_array_size; i++ ) { //push array back onto stack
      stack.push(*(stack_sort_array + i));
    }
    if (x_operator_flag == false) { //need to save array for multiple iterations if generating table
      free(stack_sort_array);      // Un-reserve temp array
    }
    //end stack reversal

    //arithmetic operations
    pop_count++; // only first a pop value is part of total pop count, all later a's are the pushed prior result
    while (!stack.isEmpty()) {
      a = stack.pop(); //number 1
      b = stack.pop(); //number 2
      pop_count++;
      c = (int)stack.pop(); //operator
      pop_count++;


      if ((a == 'x' - '0' || a == 'X' - '0') && (pop_count - 2 == (int)*operator_flags_ptr - '0')) { //changes x to current x value on table
        a = x_value;
        operator_flags_ptr++;
      
      }
     


      if ((b == 'x' - '0' || b == 'X' - '0') && ((pop_count - 1) == (int)*operator_flags_ptr - '0')) { //changes x to current x value on table

        b = x_value;
        operator_flags_ptr++;
       
      }



      switch (c) { //determine which operator; must be int;should be float without decimal

        case 's'-'0': //sine
          if (b == 1) { // if b ==1, then user input is degree and convert to radian
            a = 0.01745329252 * a;
          }
          cordicTrig(a, cos_point, sine_point); //point to sine and cosine

          stack.push(sine);
          break;

        case 'c'-'0': //cosine
          if (b == 1) {
            a = 0.01745329252 * a;
          }
          cordicTrig(a, cos_point, sine_point); //point to sine and cosine

          stack.push(cosine);
          break;

        case 't'-'0': //tangent
          if (b == 1) {
            a = 0.01745329252 * a;
          }
          cordicTan(a, tan_point);
          stack.push(tangent);
          break;

        case '+'-'0':
          stack.push(a + b);
          break;

        case '-'-'0':
          stack.push(a - b);
          break;

        case '*'-'0':
          stack.push(a * b);
          break;

        case '/'-'0':
          stack.push(a / b);
          break;

        case '^'-'0':
          stack.push(pow(a, b));
          break;

        default:
          break;
      }//end case

      if (stack.count() == 1) { //if only one stack item left after operation, that is final value
        break;
      }
   
      operator_flags_ptr++;
 

    } //end while stack.isEmpty


    //start external sram transmission
    output_buffer[0] = x_value; //x coordinate
    output_buffer[1] = stack.pop(); //y coordinate
 

    SpiRam.write_floats(address_counter, output_buffer, 2);
    output_end_address = address_counter; //last address of output on last loop
    address_counter += sizeof(output_buffer);
    //end external sram transmission


    operator_flags_ptr = &operator_flags[0]; //reset operator flags pointer
    pop_count = 0;

  } while (x_operator_flag == true); //keeps generating answers per x value until final iteration


  //reset and housekeeping
  memset(operator_flags, '\0', sizeof operator_flags); //clear operator flag arrray to null
  operator_flags_ptr = &operator_flags[0]; //reset operator flags pointer
  assembled_count = 0;
  //end reset and housekeeping




  return graph_flag;
}//end arthmetic logic

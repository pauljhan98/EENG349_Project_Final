/*
   Arithmetic_Logic.cpp - Library for arduino graphing calculator
   Paul Han 4/27/18
*/

#include "Arduino.h"
#include <SPI.h>
#include <StackArray.h> //for Input_Parsing
#include <SpiRam_Extended.h> //for external spi ram storage
#include "Arithmetic_Logic.h"


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
bool graph_flag = false;
//end arithmetic logic private


//references
long output_start_address; //start address of output in sram
long output_end_address; //end address of output in sram (actual last address is sizeof(output_buffer))
//end references

//start constructor
Arithmetic_Logic::Arithmetic_Logic(){ 
  //nothing here  
}
//end constuctor


bool Arithmetic_Logic::calculate(long& output_start_address, long& output_end_address) {
  //start initial stack reversal
  bool graph_flag = false;
  temp_array_size = stack.count(); //number of items in stack = [i]
  stack_sort_array = calloc(stack.count(), sizeof(float) );  //temp array size of stack
  for ( int i = 0; i < temp_array_size; i++) { //load stack into array
    *(stack_sort_array + i) = stack.pop();
  }
  //end initial stack reversal

  output_start_address = address_counter; //set initial address for output
  x_value = x_start - step_size; //set x value correct for first iteration
  do { //if x operator true run many time to generate table, else run once

    //start iteration counter
    if (x_operator_flag == true) {
      x_value += step_size;
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
    while (!stack.isEmpty()) {
      a = stack.pop(); //number 1
      b = stack.pop(); //number 2
      c = (int)stack.pop(); //operator

      if (b == 'x' - '0' || b == 'X' - '0') { //changes x to current x value on table
        bool graph_flag = true;
        b = x_value;
      }
      if (a == 'x' - '0' || a == 'X' - '0') { //changes x to current x value on table
        bool graph_flag = true;
        a = x_value;
      }

      switch (c) { //determine which operator; must be int;should be float without decimal
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

    } //end while stack.isEmpty

    //start external sram transmission
    Serial.println(stack.pop());

    /*
      output_buffer[0] = x_value; //x coordinate
      output_buffer[1] = stack.pop(); //y coordinate
      SpiRam.write_floats(address_counter,output_buffer,2);
      output_end_address = address_counter; //last address of output on last loop
      address_counter+=sizeof(output_buffer);
      //end external sram transmission

      operator_flags_ptr = &operator_flags[0]; //reset operator flags pointer
    */
  } while (x_operator_flag == true); //keeps generating answers per x value until final iteration


  //reset and housekeeping
  memset(operator_flags, '\0', sizeof operator_flags); //clear operator flag arrray to null
  operator_flags_ptr = &operator_flags[0]; //reset operator flags pointer
  assembled_count = 0;
  //end reset and housekeeping

  
  /*

    Serial.println();
    Serial.print(F("Output Start Address (in sram): "));
    Serial.println(output_start_address);
    Serial.println();
    Serial.print(F("Output End Address (in sram): "));
    Serial.println(output_end_address);
    Serial.println();

    //start transmission from external sram
    for (long i = output_start_address;i <= output_end_address; i += sizeof(output_buffer)){
    SpiRam.read_floats(i,output_buffer,2);
    Serial.println();
    Serial.print(F("The Answer is: "));
    Serial.print(output_buffer[1]);
    if (graph_flag == true){
      Serial.print(F(" for x = "));
      Serial.print(output_buffer[0]);
      Serial.println();
    }

    }//end for output start adddress
  */
  return graph_flag;
}//end arthmetic logic

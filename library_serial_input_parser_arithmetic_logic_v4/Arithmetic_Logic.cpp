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
int pop_count = 0; //keeps track of how many items have been popped off the stack
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
    Serial.println("graph flag is on");
  }
  //start initial stack reversal
  temp_array_size = stack.count(); //number of items in stack = [i]
  stack_sort_array = calloc(stack.count(), sizeof(float) );  //temp array size of stack
  for ( int i = 0; i < temp_array_size; i++) { //load stack into array
    *(stack_sort_array + i) = stack.pop();
  }
  //end initial stack reversal

  output_start_address = address_counter; //set initial address for output
  x_value = x_start - step_size; //set x value correct for first iteration
  Serial.println();
  Serial.print("x_value before do loop: ");
  Serial.print(x_value);

  do { //if x operator true run many time to generate table, else run once

    //start iteration counter
    if (x_operator_flag == true) {
      x_value += step_size;
      Serial.println();
      Serial.print("x_value during do loop: ");
      Serial.print(x_value);
      i++;
      //Serial.println(i);
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

      Serial.println();
      Serial.print("a_value before assignment: ");
      Serial.print((float)a);
      Serial.println();
      Serial.print("b_value before assignment: ");
      Serial.print((float)b);
      Serial.println();
      Serial.print("c_value before assignment: ");
      Serial.print(c);

      Serial.println();
      Serial.print("pop_count: ");
      Serial.print(pop_count);
      Serial.println();
      Serial.print("*operator_flags_ptr: ");
      Serial.print(*operator_flags_ptr);
      Serial.println();
      Serial.print("operator_flags_ptr: ");
      Serial.print(operator_flags_ptr);

      if (((pop_count - 2) == (int)(*operator_flags_ptr - '0'))) {
        Serial.println();
        Serial.println("a value detected as x");
      }
      if ((a == 'x' - '0' || a == 'X' - '0') && (pop_count - 2 == (int)*operator_flags_ptr - '0')) { //changes x to current x value on table
        a = x_value;
        operator_flags_ptr++;
        Serial.println();
        Serial.println("This is x value inside if statement a");
        Serial.println((float)x_value);
        Serial.println();
      }
      Serial.println();
      Serial.print("*operator_flags_ptr: ");
      Serial.print(*operator_flags_ptr);
      Serial.println();

      if (((pop_count - 1) == (int)(*operator_flags_ptr - '0'))) {
        Serial.println();
        Serial.println("b value detected as x");
      }
      if ((b == 'x' - '0' || b == 'X' - '0') && ((pop_count - 1) == (int)*operator_flags_ptr - '0')) { //changes x to current x value on table

        b = x_value;
        operator_flags_ptr++;
        Serial.println();
        Serial.println("This is x value inside if statement b");
        Serial.println((float)x_value);
        Serial.println();
      }


      Serial.println();
      Serial.print("a_value after assignment: ");
      Serial.print((float)a);
      Serial.println();
      Serial.print("b_value after assignment: ");
      Serial.print((float)b);
      Serial.println();
      Serial.print("c_value after assignment: ");
      Serial.print(c);
    
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
      Serial.println();
      Serial.println("stack.peek");
      Serial.println(stack.peek());
      Serial.println();
      operator_flags_ptr++;
      
    } //end while stack.isEmpty


    //start external sram transmission
    output_buffer[0] = x_value; //x coordinate
    output_buffer[1] = stack.pop(); //y coordinate
    Serial.println();
    Serial.print("Y value is: ");
    Serial.print(output_buffer[1]);
    Serial.println();
    Serial.print("x value is: ");
    Serial.print(output_buffer[0]);
    Serial.println();

    /*
      SpiRam.write_floats(address_counter, output_buffer, 2);
      output_end_address = address_counter; //last address of output on last loop
      address_counter += sizeof(output_buffer);
      //end external sram transmission

    */
    operator_flags_ptr = &operator_flags[0]; //reset operator flags pointer
    pop_count = 0;

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
    for (long i = output_start_address; i <= output_end_address; i += sizeof(output_buffer)) {
      SpiRam.read_floats(i, output_buffer, 2);
      Serial.println();
      Serial.print(F("The Answer is: "));
      Serial.print(output_buffer[1]);
      if (graph_flag == true) {
        Serial.print(F(" for x = "));
        Serial.print(output_buffer[0]);
        Serial.println();
      }
      Serial.println();

    }//end for output start adddress
  */
  return graph_flag;
}//end arthmetic logic

//first implementation of external ram storage
#include <SPI.h>
#include <StackArray.h> //for Input_Parsing
#include <SpiRam_Extended.h> //for external spi ram storage



StackArray <float> stack;
SpiRAM SpiRam(SPI_CLOCK_DIV2, SS_PIN, CHIP_23LC1024);






//start arithmetic logic globals

float x_start = 1; //starting value of x //all following are temp values
float step_size = 1; //step size of graph (value per pixel)
long iterations =240; //number of x iterations 
bool graph_flag = false; //flag for graph vs arthmetic
//end arithmetic logic globals

//Custom Functions Begin



void Arithmetic_Logic () {

  float *stack_sort_array; //array to hold stack so it can be reversed
  int temp_array_size; 
  float a; //number 1 or vairable x
  float b; //number 2 or variable x
  int c; //operator (num1 (operator) num 2) 
  float x_value; //current value of x for table generation
  long i = 0; //counter for iterations
  float output_buffer[2]; //output buffer for external sram transmission
  static long address_counter = 0; //external sram address counter
  static long output_start_address; //start address of output in sram
  static long output_end_address; //end address of output in sram (actual last address is sizeof(output_buffer))
  
  //start initial stack reversal
  temp_array_size = stack.count(); //number of items in stack = [i]
  stack_sort_array = calloc(stack.count(), sizeof(float) );  //temp array size of stack
  for ( int i = 0; i < temp_array_size; i++) { //load stack into array
    *(stack_sort_array + i) = stack.pop();
  }
  //end initial stack reversal

  output_start_address = address_counter; //set initial address for output
  x_value = x_start - step_size; //set x value correct for first iteration
  do{ //if x operator true run many time to generate table, else run once

    //start iteration counter
    if (x_operator_flag == true){
      x_value += step_size;
      i++;
      if (i == iterations){ //number of iterations reached
        x_operator_flag = false; //exit table generation (do..while)
      }
            
    }
    //end iteration counter

    //start stack reversal
    for ( int i = 0; i < temp_array_size; i++ ) { //push array back onto stack
      stack.push(*(stack_sort_array + i));
    }
    if (x_operator_flag == false){ //need to save array for multiple iterations if generating table
      free(stack_sort_array);      // Un-reserve temp array    
    }
    //end stack reversal

    //arithmetic operations
    while (!stack.isEmpty()){
      a = stack.pop(); //number 1
      b = stack.pop(); //number 2
      c = (int)stack.pop(); //operator

      if (b == 'x'-'0'||b == 'X'-'0'){ //changes x to current x value on table
        b = x_value;
      }
      if (a == 'x'-'0'||a == 'X'-'0'){ //changes x to current x value on table
        a = x_value;
      }
      
      switch (c) { //determine which operator; must be int;should be float without decimal
        case '+'-'0': 
          stack.push(a+b);
          break;
        
        case '-'-'0': 
          stack.push(a-b);
          break;
        
        case '*'-'0':
          stack.push(a*b);
          break;
  
        case '/'-'0':
          stack.push(a/b);
          break;
  
        case '^'-'0':
          stack.push(pow(a,b));
          break;
          
        default:
          break;
      }//end case
      
      if (stack.count() == 1){ //if only one stack item left after operation, that is final value
        break;
      } 
      
    } //end while stack.isEmpty 

    //start external sram transmission
    
    output_buffer[0] = x_value; //x coordinate
    output_buffer[1] = stack.pop(); //y coordinate
    SpiRam.write_floats(address_counter,output_buffer,2);
    output_end_address = address_counter; //last address of output on last loop
    address_counter+=sizeof(output_buffer);
    //end external sram transmission

    operator_flags_ptr = &operator_flags[0]; //reset operator flags pointer    
  }while (x_operator_flag == true); //keeps generating answers per x value until final iteration

  
  //reset and housekeeping
  memset(operator_flags, '\0', sizeof operator_flags); //clear operator flag arrray to null
  operator_flags_ptr = &operator_flags[0]; //reset operator flags pointer
  assembled_count = 0;
  //end reset and housekeeping
  
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
  
}//end arthmetic logic

//end custom functions


void setup() {
  Serial.begin(500000);
  stack.setPrinter (Serial);
  operator_flags_ptr = &operator_flags[0]; //reset operator flags pointer
  memset(operator_flags, '\0', sizeof operator_flags); //clear operator flag array to null
  operator_flags_ptr = &operator_flags[0]; //reset operator flags pointer
  
  Serial.println(F("Input Parser & Arithmetic Unit v5"));
  Serial.println(F("Paul Han, 2018"));
  Serial.println(F("RPN Input Format; e.g. 2,2,+,= (2+2=) | x,x,*,4,+,= (x*x+4=)x,x"));
  Serial.println(F("Numbers may not exceed 9 digits. Precision is 6-7 decimal places."));
  Serial.println(F("Result history stored in External SRAM."));
  Serial.println(F("For Stack output (destructive) enter '&'."));
  Serial.println();
} // end setupF

void loop() {
  Input_Parsing.Input_Parsing();
  
  if (newInput == true) {
    newInput = false;
    Arithmetic_Logic ();    
  } //if new input end

} //loop end

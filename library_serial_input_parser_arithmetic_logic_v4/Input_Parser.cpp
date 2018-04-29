/*
   Input_Parser.cpp - Library for arduino graphing calculator
   Paul Han 4/27/18
*/


#include "Arduino.h"
#include "StackArray.h" //for Input_Parsing
#include "Input_Parser.h"

//start input parsing private variables
byte digit_count = 0; //number of digits in number
int RawInput; //from input device minus '0' to correlate ACSII values with actual numerical value
int power; // power of 10 times digit to equal place (i.e. 4x10^2=400; power=2
float top_number; //digit popped from top of stack
float power_multiply; //10^2=100;100=power_multiply
float base = 10.0; //10^2;base=10
static boolean decimal_flag = false; //raised if decimal point is entered
static int decimal_place = 0;//when triggered, keeps track of places behind decimal
static byte negative_possible_flag = 0; //detects negative sign vs minus; 0=not negative;1=possibly negative;2=negative
float assembled_number; //output of number parsing
bool newInput = false; //resets new Input to false (return)
//end input parsing private variables

//Start Input Parsing external variables
byte assembled_count = 0; //number of assembled values in stack
char operator_flags[21]; //char array for keeping track of which stack items are operators (includes X variable)
char *operator_flags_ptr;
bool x_operator_flag = false; //flag when X operator detected, triggers Arithmetic logic graph table
bool graph_flag = false; //triggers when graphing
//end input parsing external

//start constructor
Input_Parser::Input_Parser() {


  stack.setPrinter (Serial);
  operator_flags_ptr = &operator_flags[0]; //reset operator flags pointer
  memset(operator_flags, '\0', sizeof operator_flags); //clear operator flag array to null
  operator_flags_ptr = &operator_flags[0]; //reset operator flags pointer
}
//end constructor

//parses input stream into numbers and operators //MAX 9 digits with 7-8 digit accuracy //max 20 operators per whole input
bool Input_Parser::parse_input() { //RPN input style, format e.g. -12,34.56,+,-78.09,- //-,+,*,^,/ operators supported
  
  newInput = false;
  while (Serial.available() > 0 && newInput == false) { //still recieving keypresses for current input
    RawInput = Serial.read() - '0'; //convert from ASCII value to numerical value
    switch (RawInput) {
      case 0: case 1: case 2: case 3: case 4: case 5: case 6: case 7: case 8: case 9:

        //start negative tracker
        if (negative_possible_flag == 1) { //if previous input was possibly a negative sign
          negative_possible_flag = 2; //if number after negative sign, is a negative sign not minus
          stack.pop(); //remove negative from stack
          assembled_count--; //one less operator than actual
          operator_flags_ptr--; //go back to previous position
          *operator_flags_ptr = '\0'; //write over operator record
        }
        //end negative tracker

        //start digit pusher
        stack.push(RawInput); //push digits to stack
        newInput = false; //number input still not completed
        //end digit pusher

        //start decimal tracker
        if (decimal_flag) {
          decimal_place--; //accumulates digit offset value
        }
        //end decimal tracker

        break;

      case '.'-'0':
        decimal_flag = true; //starts decimal place counting
        decimal_place = 0;
        break;

      case ','-'0':

        //start negative tracker
        if (negative_possible_flag == 1) { //comma after minus input confirms it is minus, not negative
          negative_possible_flag = 0;
        }
        //end negative tracker

        //start number assembler
        if (stack.peek() >= 0 && stack.peek() <= 9) { //if last input before "," was a number
          digit_count = stack.count() - assembled_count; //number of digits in stack minus previous values represent number of digits in number
          assembled_number = 0; //ready for new number

          for (int i = 0; i < digit_count; i++) { //until all digits have been accounted for (will have to change)
            power = digit_count - (stack.count() - assembled_count) + decimal_place; //stack is LIFO, so digit count
            top_number = stack.pop();                                               //plus assembled count minus current number of values
            power_multiply = 1;

            //start multipler factor
            if (power >= 0) { //testing if negative or positive power
              for (int i = 0; i < power; i++) //calculates 10^power for positive exponents
                power_multiply = power_multiply * base;
            }
            else {
              for (int i = 0; i > power; i--) //same but for negative exponents
                power_multiply = power_multiply / base;
            }
            //end multipler factor

            assembled_number = assembled_number + (top_number) * (power_multiply); //assembles final number
          } //for digit count end

          //start negative tracker
          if (negative_possible_flag == 2) { //if assembled number is negative
            assembled_number = -(assembled_number);
            negative_possible_flag = 0;
          }
          //end negative tracker

          stack.push(assembled_number);

          //start housekeeping and reset
          assembled_count++; //counter for number of assembled values in stack
          decimal_flag = false; //reset decimal flag
          decimal_place = 0; //reset decimal place
          //end housekeeping and reset

        } //if stack peek end
        break;

      case '='-'0':
        newInput = true; // Number assembly completed
        break;

      case '+'-'0': case '^'-'0': case '*'-'0':  case '/'-'0': //+,*,/,^ operators
        stack.push(RawInput);
        assembled_count++; //more assembled values in stack
        *operator_flags_ptr = (char)(assembled_count + '0'); //keeps track of which numbers in stack are actually operators
        operator_flags_ptr++;
        break;

      case 'x'-'0': case 'X'-'0': //x for generation table from equation (not case sensitive) (X is defacto an operator)
        stack.push(RawInput);
        assembled_count++; //more assembled values in stack
        *operator_flags_ptr = (char)(assembled_count + '0'); //keeps track of which numbers in stack are actually operators
        operator_flags_ptr++;
        x_operator_flag = true; //(triggers arthmetic logic)
        graph_flag = true;
        break;

      case '-'-'0': //has to detect negatives from minus operator
        stack.push(RawInput);
        assembled_count++;
        *operator_flags_ptr = (char)(assembled_count + '0'); //keeps track of which numbers in stack are actually operators
        operator_flags_ptr++;
        negative_possible_flag = 1; //possibly a negative sign instead of minus operator
        break;

      case '&'-'0':
        //data output
        Serial.println(F("The stack contained"));
        while (!stack.isEmpty()) {
          Serial.println(stack.pop());
        }
        Serial.println();
        operator_flags_ptr = &operator_flags[0]; //set pointer back to begining
        Serial.println(F("From first to last, entries detected as operators are:"));
        while (*operator_flags_ptr) {
          Serial.print(*operator_flags_ptr);
          Serial.print(",");
          Serial.println();
          operator_flags_ptr++;
        }
        //end data output

        //reset and housekeeping
        memset(operator_flags, '\0', sizeof operator_flags); //clear operator flag arrray to null
        operator_flags_ptr = &operator_flags[0]; //reset operator flags pointer
        assembled_count = 0;
      //end reset and housekeeping

      default:
        newInput = false; //illegal inputs will be ignored
        break;

    } //switch case end

  }//while Serial.available end
  return newInput; //returns true when stack ready for arthmetic
} //end of Input_Parsing


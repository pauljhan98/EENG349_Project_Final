#include <SPI.h>
#include <SpiRam_Extended.h> //for external spi ram storage
#include <StackArray.h> //for Input_Parsing
#include "Input_Parser.h" //input parser
#include "Arithmetic_Logic.h"

#define SS_PIN 8 //slave select for spiRam

Input_Parser input_parser;
Arithmetic_Logic arithmetic_logic;
StackArray <float> stack;
SpiRAM SpiRam(SPI_CLOCK_DIV2, SS_PIN, CHIP_23LC1024);

float x_start = 1.0; //starting value of x //all following are temp values
float step_size = 1.0; //step size of graph (value per pixel)
long iterations = 24; //number of x iterations
long start_address;
long end_address;



void setup() {
  Serial.begin(115200);
  Serial.println(F("Library Implementation Input Parser & Arithmetic Unit v4"));
  Serial.println(F("Paul Han, 2018"));
  Serial.println(F("RPN Input Format; e.g. 2,2,+,= (2+2=) | x,x,*,4,+,= (x*x+4=)x,x"));
  Serial.println(F("Numbers may not exceed 9 digits. Precision is 6-7 decimal places."));
  Serial.println(F("Result history stored in External SRAM."));
  Serial.println(F("For Stack output (destructive) enter '&'."));
  Serial.println();
  
} // end setup

void loop() {
  if (input_parser.parse_input()) {
    if (arithmetic_logic.calculate(start_address,end_address) == true){
      Serial.println("graph flag on"); 
    }else {
      Serial.println("graph flag off");
    }
    Serial.println();
    Serial.print("passed start address: ");
    Serial.print(start_address);    
    Serial.println();
    Serial.print("passed end address: ");
    Serial.print(end_address);
    graph_flag = false;    
  }  


} //loop end

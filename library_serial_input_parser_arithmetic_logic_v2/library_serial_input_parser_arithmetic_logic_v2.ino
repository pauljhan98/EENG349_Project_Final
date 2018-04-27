#include <SPI.h>
#include <SpiRam_Extended.h> //for external spi ram storage
#include <StackArray.h> //for Input_Parsing
#include "Input_Parser.h" //input parser
#include "Arithmetic_Logic.h"
#include "plotter.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"

#define SS_PIN 8 //slave select for spiRam
// For the Adafruit shield, these are the default.
#define TFT_DC 9 // CS is the chip select line; selects the LCD 
#define TFT_CS 10 // DC (or D/C) is the data communication line; it sends the data to the screen

Input_Parser input_parser;
Arithmetic_Logic arithmetic_logic;
StackArray <float> stack;
SpiRAM SpiRam(SPI_CLOCK_DIV2, SS_PIN, CHIP_23LC1024);

float x_start = 1; //starting value of x //all following are temp values
float step_size = 1; //step size of graph (value per pixel)
long iterations = 240; //number of x iterations
long start_address;
long end_address;


// Use Arduino's hardware SPI (uses pins 11,12,13) and TFT_DC / TFT_CS for DC/CS
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
Adafruit_ILI9341 *tft_point = &tft;



void setup() {
  Serial.begin(9600);
  Serial.println(F("Library Implementation Input Parser & Arithmetic Unit v1"));
  Serial.println(F("Paul Han, 2018"));
  Serial.println(F("RPN Input Format; e.g. 2,2,+,= (2+2=) | x,x,*,4,+,= (x*x+4=)x,x"));
  Serial.println(F("Numbers may not exceed 9 digits. Precision is 6-7 decimal places."));
  Serial.println(F("Result history stored in External SRAM."));
  Serial.println(F("For Stack output (destructive) enter '&'."));
  Serial.println();
  
} // end setup

void loop() {
  if (input_parser.parse_input()) {
    if (arithmetic_logic.calculate(start_address,end_address));{
      Serial.println("graph Flag on"); 
    }
    Serial.println(start_address);
    Serial.println(end_address);    
  }  
  drawAxis(tft_point);
  testDrawGraph(tft_point);

  double cosine = 0;
  double sine = 0;
  
  double *cos_point = &cosine;
  double *sine_point = &sine;

  cordicTrig(-13, cos_point, sine_point);

  Serial.print("Sine value ");
  Serial.println(sine, 5);
  Serial.print("Cosine value ");
  Serial.println(cosine, 5);

  delay(100000);


} //loop end

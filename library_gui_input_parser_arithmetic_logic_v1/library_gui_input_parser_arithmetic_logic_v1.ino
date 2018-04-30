#include <SPI.h>
#include <SpiRam_Extended.h> //for external spi ram storage
#include <StackArray.h> //for Input_Parsing
#include "Input_Parser.h" //input parser
#include "Arithmetic_Logic.h"
#include "Gui.h"
#include <Adafruit_GFX.h>
#include "Adafruit_ILI9341.h"
#include <PS2Keyboard.h>

#define SS_PIN 8 //slave select for spiRam
#define TFT_DC 9
#define TFT_CS 10
#define WHITE 0xFFFF
#define BLACK 0x0000
#define DATAPIN 3
#define IRQPIN  2

//start objects
Input_Parser input_parser;
Arithmetic_Logic arithmetic_logic;
StackArray <float> stack;
SpiRAM SpiRam(SPI_CLOCK_DIV2, SS_PIN, CHIP_23LC1024);
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
Gui gui;
PS2Keyboard keyboard;
//end objects

float x_start = 1.0; //starting value of x //all following are temp values
float step_size = 1.0; //step size of graph (value per pixel)
long iterations = 24; //number of x iterations
long start_address;
long end_address;

char* gui_output_ptr;



void setup() {
  Serial.begin(115200);
  Serial.println(F("Library Implementation GUI Based Input Parser & Arithmetic Unit v1"));
  Serial.println(F("Paul Han, 2018"));
  Serial.println(F("RPN Input Format; e.g. 2,2,+,= (2+2=) | x,x,*,4,+,= (x*x+4=)x,x"));
  Serial.println(F("Numbers may not exceed 9 digits. Precision is 6-7 decimal places."));
  Serial.println(F("Result history stored in External SRAM."));
  Serial.println(F("For Stack output (destructive) enter '&'."));
  Serial.println();
  tft.begin();
  keyboard.begin(DATAPIN, IRQPIN);
  tft.fillScreen(WHITE);
  tft.setRotation(3);
  tft.drawLine(0, 190, 320, 190, BLACK);
  tft.drawLine(24, 8, 24, 190, BLACK);
  tft.drawLine(0, 8, 320, 8, BLACK);
  for (int i = 34; i <= 164; i += 26) {
    tft.drawLine(0, i, 24, i, BLACK);
    delay(40);
  }
} // end setup


void loop() {
  if (gui.update_screen(gui_output_ptr)) { //gui_output_ptr
    if (input_parser.parse_input(gui_output_ptr)) {
      if (arithmetic_logic.calculate(start_address, end_address) == true) {
        Serial.println("graph flag on");
      } else {
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

  }


} //loop end

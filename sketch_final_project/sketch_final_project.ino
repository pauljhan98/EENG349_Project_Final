void setup()










#include "input.h"
#include <math.h>




// For the Adafruit shield, these are the default.
// CS is the chip select line; selects the LCD 
// DC (or D/C) is the data communication line; it sends the data to the screen
#define TFT_DC 9
#define TFT_CS 10




// Use Arduino's hardware SPI (uses pins 11,12,13) and TFT_DC / TFT_CS for DC/CS
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
Adafruit_ILI9341 *tft_point = &tft;




void setup() {

  // Paul's setup


  
  Serial.begin(500000);
  
  
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

  
  

 /*
                                            // Begin Serial Communication
  Serial.begin(9600);                       // And initialize a tft display object
  Serial.println("ILI9341 Test!");  
  tft.begin();

  */
  

}

void loop() {

  // Paul's loop
  Input_Parsing();
  
  if (newInput == true) {
    newInput = false;
    Arithmetic_Logic ();    
  }

    /*
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
    */

}

// Draws a trigomentric function
void drawTrig() {
  
}





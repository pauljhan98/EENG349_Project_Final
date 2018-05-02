#include "plotter.h"
#include "Input_Parser.h"
#include "Arithmetic_Logic.h"
#include "Gui.h"
#include "Input_Parser.h"
#include <math.h>
#include "trig.h"




// For the Adafruit shield, these are the default.
// CS is the chip select line; selects the LCD
// DC (or D/C) is the data communication line; it sends the data to the screen
#define TFT_DC 9
#define TFT_CS 10




// Use Arduino's hardware SPI (uses pins 11,12,13) and TFT_DC / TFT_CS for DC/CS
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
Adafruit_ILI9341 *tft_point = &tft;

// Paul's code dependencies
#define SS_PIN 8 //slave select for spiRam

#define WHITE 0xFFFF
#define BLACK 0x0000
#define DATAPIN 3
#define IRQPIN  2

//start objects
Input_Parser input_parser;
Arithmetic_Logic arithmetic_logic;
StackArray <float> stack;
SpiRAM SpiRam(SPI_CLOCK_DIV2, SS_PIN, CHIP_23LC1024);
Gui gui;
PS2Keyboard keyboard;
//end objects

float x_start = 1.0; //starting value of x //all following are temp values
float step_size = 1.0; //step size of graph (value per pixel)
long iterations = 320; //number of x iterations
long start_address;
long end_address;
char* gui_output_ptr;
float output_buff[2]; //output buffer for external sram transmission
int j; //counter for reset output data function
int output_data_count; //counts from 0-2 and resets, where equation and output are located on screen
int gui_buffer_size; //gets size of text buffer from gui
struct data output_data[3]; //data structure used to store answer data. defined in gui.h
char* dummy_ptr; //pointer for feeding gui text buffer into struct input



// End of Paul's code dependencies

//resets data structure
void reset_output_data(int j) {
  output_data[j].in_use = false;
  output_data[j].data_graph_flag = false;
  output_data[j].table_start_address = 0;
  output_data[j].table_end_address = 0;
  output_data[j].output = 0;
  memset(output_data[j].input, '\0', sizeof output_data[output_data_count].input);
}
//end reset data structure

//records answer data to data structure

void record_answer() {



  //when there are three answers on screen and another is entered
  if (output_data_count > 2) { //once there are three answers on the screen
    output_data[0] = output_data[1]; //shift up three answers up once
    output_data[1] = output_data [2];
    reset_output_data(2);
    output_data_count = 2; //sets it to print on the third
  }
  //end when three answers on screen

  //start record data
  Serial.println();
  Serial.print("output data count: ");
  Serial.print(output_data_count);
  output_data[output_data_count].in_use = true;
  if (graph_flag) { //differences between a graphing equation and simple arthmetic
    output_data[output_data_count].data_graph_flag = true;
    output_data[output_data_count].output = 0;
  } else {
    output_data[output_data_count].data_graph_flag = false;
    SpiRam.read_floats(start_address, output_buff, 2);
    output_data[output_data_count].output = output_buff[1];
  }
  output_data[output_data_count].table_start_address = start_address;
  output_data[output_data_count].table_end_address = end_address;

  //start record input
  Serial.println("below is gui output ptr");
  for (int i = 0; i < gui_buffer_size; i++) { //sets pointer back to start of text_buffer array
    gui_output_ptr--;
    Serial.println(*gui_output_ptr);
  }
  dummy_ptr = &output_data[output_data_count].input[0];
  memset(output_data[output_data_count].input, '\0', sizeof output_data[output_data_count].input); //clears input storage array to null
  for (int i = 0; i <= gui_buffer_size; i++) { //writes input to data input
    *dummy_ptr  =  * gui_output_ptr;
    gui_output_ptr++;
    dummy_ptr++;
  }
  Serial.println();
  Serial.println(output_data[output_data_count].input);
  //end record input

  //end record data



}


  float cosine;
  float sine;
  float tangent;
  float *cos_point = &cosine;
  float *sine_point = &sine;
  float *tan_point = &tangent;
void setup() {


  // Paul's code dependencies
  Serial.begin(9600);
  Serial.println(F("Library Implementation GUI Based Input Parser & Arithmetic Unit v1"));
  Serial.println(F("Paul Han, 2018"));
  Serial.println(F("RPN Input Format; e.g. 2,2,+,= (2+2=) | x,x,*,4,+,= (x*x+4=)x,x"));
  Serial.println(F("Numbers may not exceed 9 digits. Precision is 6-7 decimal places."));
  Serial.println(F("Result history stored in External SRAM."));
  Serial.println(F("For Stack output (destructive) enter '&'."));
  Serial.println();
  tft.begin();
  keyboard.begin(DATAPIN, IRQPIN);
  gui.restore_home();
  output_data_count = 0;

  cordicTrig(2.34, cos_point, sine_point);
  Serial.print("Sine value ");
  Serial.println(sine, 5);
  Serial.print("Cosine value ");
  Serial.println(cosine, 5);
  Serial.println("Expected values: 0 , 1");
  Serial.println("");
}

void loop() {
  // Paul's code dependencies
  if (gui.update_screen(gui_output_ptr, gui_buffer_size)) { //gui_output_ptr
    if (input_parser.parse_input(gui_output_ptr)) {
      if (arithmetic_logic.calculate(start_address, end_address) == true) {        
        record_answer();
        makePlot(tft_point, output_data[output_data_count].table_start_address, output_data[output_data_count].table_end_address, output_buff , SpiRam); //display graph
      } else {        
        record_answer();
      }
      graph_flag = false; //reset graph flag for input parser
      
    } //if input parser end
    gui.restore_home(); //restore screen
    gui.display_answers(); //display updated answers    
    output_data_count++;
  } //if update screen end


  ////////////////////////


  // makePlot(tft_point, first_address, last_address, output_point, SpiRam);


  /*

      drawAxis(tft_point);
      testDrawGraph(tft_point);

      double cosine = 0;
      double sine = 0;

      double *cos_point = &cosine;
      double *sine_point = &sine;

    // Testing the cordic trig function, because it's a bit fucked up
      cordicTrig(0, cos_point, sine_point);
      Serial.print("Sine value ");
      Serial.println(sine, 5);
      Serial.print("Cosine value ");
      Serial.println(cosine, 5);
      Serial.println("Expected values: 0 , 1");
      Serial.println("");

      cordicTrig(.25, cos_point, sine_point);
      Serial.print("Sine value ");
      Serial.println(sine, 5);
      Serial.print("Cosine value ");
      Serial.println(cosine, 5);
      Serial.println("Expected values: .247 , .968");
      Serial.println("");


      cordicTrig(.5, cos_point, sine_point);
      Serial.print("Sine value ");
      Serial.println(sine, 5);
      Serial.print("Cosine value ");
      Serial.println(cosine, 5);
      Serial.println("Expected values: .479 , .87758");
      Serial.println("");


      cordicTrig(1.57, cos_point, sine_point);
      Serial.print("Sine value ");
      Serial.println(sine, 5);
      Serial.print("Cosine value ");
      Serial.println(cosine, 5);
      Serial.println("Expected values: .999 , .00079");
      Serial.println("");


      cordicTrig(3, cos_point, sine_point);
      Serial.print("Sine value ");
      Serial.println(sine, 5);
      Serial.print("Cosine value ");
      Serial.println(cosine, 5);
      Serial.println("Expected values: .14112 , -.989 ");
      Serial.println("");


      cordicTrig(6, cos_point, sine_point);
      Serial.print("Sine value ");
      Serial.println(sine, 5);
      Serial.print("Cosine value ");
      Serial.println(cosine, 5);
      Serial.println("Expected values: -.2794 , .960");
      Serial.println("");


      cordicTrig(9, cos_point, sine_point);
      Serial.print("Sine value ");
      Serial.println(sine, 5);
      Serial.print("Cosine value ");
      Serial.println(cosine, 5);
      Serial.println("Expected values: .4121 , -.911");
      Serial.println("");


      cordicTrig(44, cos_point, sine_point);
      Serial.print("Sine value ");
      Serial.println(sine, 5);
      Serial.print("Cosine value ");
      Serial.println(cosine, 5);
      Serial.println("Expected values: .01777 , .999");
      Serial.println("");


      cordicTrig(122, cos_point, sine_point);
      Serial.print("Sine value ");
      Serial.println(sine, 5);
      Serial.print("Cosine value ");
      Serial.println(cosine, 5);
      Serial.println("Expected values: .4987 , -.866");
      Serial.println("");


      cordicTrig(123, cos_point, sine_point);
      Serial.print("Sine value ");
      Serial.println(sine, 5);
      Serial.print("Cosine value ");
      Serial.println(cosine, 5);
      Serial.println("Expected values: -.4599 , -.887");
      Serial.println("");


      cordicTrig(634, cos_point, sine_point);
      Serial.print("Sine value ");
      Serial.println(sine, 5);
      Serial.print("Cosine value ");
      Serial.println(cosine, 5);
      Serial.println("Expected values: -.5660 , .824");
      Serial.println("");


    // Now, for negative values of omega
      cordicTrig(-.25, cos_point, sine_point);
      Serial.print("Sine value ");
      Serial.println(sine, 5);
      Serial.print("Cosine value ");
      Serial.println(cosine, 5);
      Serial.println("Expected values: -.247 , .968");
      Serial.println("");


      cordicTrig(-.5, cos_point, sine_point);
      Serial.print("Sine value ");
      Serial.println(sine, 5);
      Serial.print("Cosine value ");
      Serial.println(cosine, 5);
      Serial.println("Expected values: -.479 , .87758");
      Serial.println("");


      cordicTrig(-1.57, cos_point, sine_point);
      Serial.print("Sine value ");
      Serial.println(sine, 5);
      Serial.print("Cosine value ");
      Serial.println(cosine, 5);
      Serial.println("Expected values: -.999 , .00079");
      Serial.println("");


      cordicTrig(-3, cos_point, sine_point);
      Serial.print("Sine value ");
      Serial.println(sine, 5);
      Serial.print("Cosine value ");
      Serial.println(cosine, 5);
      Serial.println("Expected values: -.14112 , -.989 ");
      Serial.println("");


      cordicTrig(-6, cos_point, sine_point);
      Serial.print("Sine value ");
      Serial.println(sine, 5);
      Serial.print("Cosine value ");
      Serial.println(cosine, 5);
      Serial.println("Expected values: .2794 , .960");
      Serial.println("");


      cordicTrig(-9, cos_point, sine_point);
      Serial.print("Sine value ");
      Serial.println(sine, 5);
      Serial.print("Cosine value ");
      Serial.println(cosine, 5);
      Serial.println("Expected values: -.4121 , -.911");
      Serial.println("");


      cordicTrig(-44, cos_point, sine_point);
      Serial.print("Sine value ");
      Serial.println(sine, 5);
      Serial.print("Cosine value ");
      Serial.println(cosine, 5);
      Serial.println("Expected values: -.01777 , .999");
      Serial.println("");


      cordicTrig(-122, cos_point, sine_point);
      Serial.print("Sine value ");
      Serial.println(sine, 5);
      Serial.print("Cosine value ");
      Serial.println(cosine, 5);
      Serial.println("Expected values: -.4987 , -.866");
      Serial.println("");


      cordicTrig(-123, cos_point, sine_point);
      Serial.print("Sine value ");
      Serial.println(sine, 5);
      Serial.print("Cosine value ");
      Serial.println(cosine, 5);
      Serial.println("Expected values: .4599 , -.887");
      Serial.println("");


      cordicTrig(-634, cos_point, sine_point);
      Serial.print("Sine value ");
      Serial.println(sine, 5);
      Serial.print("Cosine value ");
      Serial.println(cosine, 5);
      Serial.println("Expected values: .5660 , .824");
      Serial.println("");


      delay(100000);
  */


}

// Draws a trigomentric function
void drawTrig() {

}





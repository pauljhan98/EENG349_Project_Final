/* Gui.cpp - arduino graphing calc gui v1
   Paul Han 4/27/18
*/


#include "Arduino.h"
#include "Gui.h"
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <PS2Keyboard.h>

#define WHITE 0xFFFF
#define BLACK 0x0000
#define INITAL_CHAR_X 5
#define INITAL_CHAR_Y 198
#define CHAR_LINE_HEIGHT 20
#define CHAR_WIDTH 13
#define CHAR_PER_LINE 24 //count start at 0, actual value 25
#define LINE_X 29 // x coordinate of each line
#define LINE_X_ANS 188 // x coordinate for answer
#define LINE_1 14 //y coordinates of each line
#define LINE_2 40

//start gui private variables
static char text_buffer[51];
int  t = 0; //counter for text buffer
char newinput;
bool entry_complete = false; //returned by update_screen when =
int char_x = INITAL_CHAR_X;
int char_y = INITAL_CHAR_Y;
bool first_flag = true; //denotates first run
byte blink = millis() / 250;
//end gui private variables

//start gui references
char* text_output_ptr;
int buffer_size;
//end gui references

//start gui external variables
//end gui external variables


//start constructor
Gui::Gui() {
  t = 0; //reset text buffer pointer
  memset(text_buffer, '\0', sizeof text_buffer); //clear text buffer to null
  char_y = INITAL_CHAR_Y;
  char_x = INITAL_CHAR_X;
  first_flag = true;
}
//end constructor

//start display answers
void Gui::display_answers() { //display answers
  for (int i = 0; i <= 2; i++) {

    
    if (output_data[i].in_use == true) {
      tft.setTextColor(BLACK);
      tft.setTextSize(2);
      tft.setTextWrap(false);
      tft.setCursor(LINE_X, LINE_1 + (i * 51)); //height of two rows of pixels
      tft.print(output_data[i].input);
      tft.setCursor(LINE_X_ANS, LINE_2 + (i * 51));
      
      if (output_data[i].data_graph_flag == true) {
        tft.print("[graph]");
      } else {
        tft.print(output_data[i].output, 6);
      }
    }
  }
}
//end display answers

//start restore home graphic
void Gui::restore_home() { //restore to default home screen

  tft.fillScreen(WHITE);
  tft.setRotation(1);
  tft.drawLine(0, 190, 320, 190, BLACK);
  tft.drawLine(24, 8, 24, 190, BLACK);
  tft.drawLine(0, 8, 320, 8, BLACK);
  for (int i = 34; i <= 164; i += 26) {
    tft.drawLine(0, i, 24, i, BLACK);
  }
}
//end restore home graphic

//start update_screen
bool Gui::update_screen(char*& text_output_ptr, int& buffer_size) { //

  if (entry_complete == true) {
    entry_complete = false;
    memset(text_buffer, '\0', sizeof text_buffer); //clear text buffer to null
    for ( t = 0; t <= 2 * CHAR_PER_LINE; t++) {
      if (t < CHAR_PER_LINE) {
        char_x = INITAL_CHAR_X + (CHAR_WIDTH * t);
        char_y = INITAL_CHAR_Y;
      } else {
        char_x = INITAL_CHAR_X + (CHAR_WIDTH * (t - CHAR_PER_LINE));
        char_y = INITAL_CHAR_Y + CHAR_LINE_HEIGHT;
      }
      tft.drawChar(char_x, char_y, text_buffer[t], BLACK, WHITE, 2);
    }
    t = 0;
    first_flag = true;
  }

  if (first_flag == true) {
    if (t < CHAR_PER_LINE) {
      char_x = INITAL_CHAR_X + (CHAR_WIDTH * t);
      char_y = INITAL_CHAR_Y;
    } else {
      char_x = INITAL_CHAR_X + (CHAR_WIDTH * (t - CHAR_PER_LINE));
      char_y = INITAL_CHAR_Y + CHAR_LINE_HEIGHT;
    }
    tft.drawChar(char_x, char_y, text_buffer[t], BLACK, WHITE, 2);

    tft.drawLine(char_x, char_y + 16, char_x + 12, char_y + 16, BLACK);
    first_flag = false;
  }


  blink = millis() / 250;
  if (blink % 2) {
    tft.drawLine(char_x, char_y + 16, char_x + 12, char_y + 16, BLACK);
  } else {
    tft.drawLine(char_x, char_y + 16, char_x + 12, char_y + 16, WHITE);
  }

  while (keyboard.available()) { //still recieving keypresses for current input
    newinput = keyboard.read(); //convert from ASCII value to numerical value
  

    switch (newinput) {
      case '0': case '1': case '2': case '3': case '4': case '5':
      case '6': case '7': case '8': case '9': case ',':
      case '-': case '+': case '^': case '/': case '*': case 'x': 
      case 's': case 'c': case 't': //trig functions
      case 'X': case '=': case '.':
        text_buffer[t] = newinput;
        
        if (t < CHAR_PER_LINE) {
          char_x = INITAL_CHAR_X + (CHAR_WIDTH * t);
          char_y = INITAL_CHAR_Y;

        } else {
          char_x = INITAL_CHAR_X + (CHAR_WIDTH * (t - CHAR_PER_LINE));
          char_y = INITAL_CHAR_Y + CHAR_LINE_HEIGHT;
        }
        tft.drawLine(char_x, char_y + 16, char_x + 12, char_y + 16, WHITE);
        tft.drawChar(char_x, char_y, text_buffer[t], BLACK, WHITE, 2);
        t++;
        if (t < CHAR_PER_LINE) {
          char_x = INITAL_CHAR_X + (CHAR_WIDTH * t);
          char_y = INITAL_CHAR_Y;

        } else {
          char_x = INITAL_CHAR_X + (CHAR_WIDTH * (t - CHAR_PER_LINE));
          char_y = INITAL_CHAR_Y + CHAR_LINE_HEIGHT;
        }
        tft.drawLine(char_x, char_y + 16, char_x + 12, char_y + 16, BLACK);
        break;

      case PS2_BACKSPACE: //backspace key
        tft.drawLine(char_x, char_y + 16, char_x + 12, char_y + 16, WHITE);
        if (t > 0) {
          t--;
          if (t <= CHAR_PER_LINE) {
            char_x = INITAL_CHAR_X + (CHAR_WIDTH * t);
            char_y = INITAL_CHAR_Y;
          } else {
            char_x = INITAL_CHAR_X + (CHAR_WIDTH * (t - CHAR_PER_LINE));
            char_y = INITAL_CHAR_Y + CHAR_LINE_HEIGHT;
          }
          text_buffer[t] = '\0';
          tft.drawChar(char_x, char_y, text_buffer[t], BLACK, WHITE, 2);
          tft.drawLine(char_x, char_y + 16, char_x + 12, char_y + 16, BLACK);
        }
        break;

      case PS2_RIGHTARROW: //right arrow
        tft.drawLine(char_x, char_y + 16, char_x + 12, char_y + 16, WHITE);
        t++;
        if (t <= CHAR_PER_LINE) {
          char_x = INITAL_CHAR_X + (CHAR_WIDTH * t);
          char_y = INITAL_CHAR_Y;
        } else {
          char_x = INITAL_CHAR_X + (CHAR_WIDTH * (t - CHAR_PER_LINE));
          char_y = INITAL_CHAR_Y + CHAR_LINE_HEIGHT;
        }
        tft.drawLine(char_x, char_y + 16, char_x + 12, char_y + 16, BLACK);
        break;


      case PS2_LEFTARROW: //left arrow
        tft.drawLine(char_x, char_y + 16, char_x + 12, char_y + 16, WHITE);
        t--;
        if (t <= CHAR_PER_LINE) {
          char_x = INITAL_CHAR_X + (CHAR_WIDTH * t);
          char_y = INITAL_CHAR_Y;
        } else {
          char_x = INITAL_CHAR_X + (CHAR_WIDTH * (t - CHAR_PER_LINE));
          char_y = INITAL_CHAR_Y + CHAR_LINE_HEIGHT;
        }
        tft.drawLine(char_x, char_y + 16, char_x + 12, char_y + 16, BLACK);
        break;

      case PS2_ENTER: //enter key
        entry_complete = true;
        text_output_ptr = &text_buffer[0];
        tft.drawLine(char_x, char_y + 16, char_x + 12, char_y + 16, WHITE);
        t = 0;
        while (text_buffer[t] != '\0') {          
          t++;
        }
        buffer_size = t;
        t = 0;
        break;


      default:
        break;
    }
    if (t == 2 * CHAR_PER_LINE) {
      t = 0;
    }
  }




  return entry_complete;
}

//end update_screen

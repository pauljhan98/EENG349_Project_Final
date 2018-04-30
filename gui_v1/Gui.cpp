/* gui.cpp - arduino graphing calc gui v1
   Paul Han 4/27/18
*/


#include "Arduino.h"
#include "Gui.h"
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#define WHITE 0xFFFF
#define BLACK 0x0000
#define INITAL_CHAR_X 5
#define INITAL_CHAR_Y 198
#define CHAR_LINE_HEIGHT 20
#define CHAR_WIDTH 13
#define CHAR_PER_LINE 24 //count start at 0, actual value 25

//start gui private variables
char text_buffer[51];
int  c = 0; //counter for text buffer
char newinput;
bool entry_complete = false; //returned by update_screen when =
int char_x = INITAL_CHAR_X;
int char_y = INITAL_CHAR_Y;
bool first_flag = true; //denotates first run
byte blink = millis() / 250;
//end gui private variables

//start gui references
char* text_output_ptr;
//end gui references

//start gui external variables
//end gui external variables


//start constructor
Gui::Gui() {
  c = 0; //reset text buffer pointer
  memset(text_buffer, '\0', sizeof text_buffer); //clear text buffer to null
  char_y = INITAL_CHAR_Y;
  char_x = INITAL_CHAR_X;
  first_flag = true;
}
//end constructor

//start update_screen
bool Gui::update_screen(char*& text_output_ptr) { //

  if (entry_complete == true) {
    entry_complete = false;
    memset(text_buffer, '\0', sizeof text_buffer); //clear text buffer to null
    for ( c = 0; c <= 2 * CHAR_PER_LINE; c++) {
      if (c < CHAR_PER_LINE) {
        char_x = INITAL_CHAR_X + (CHAR_WIDTH * c);
        char_y = INITAL_CHAR_Y;
      } else {
        char_x = INITAL_CHAR_X + (CHAR_WIDTH * (c - CHAR_PER_LINE));
        char_y = INITAL_CHAR_Y + CHAR_LINE_HEIGHT;
      }
      tft.drawChar(char_x, char_y, text_buffer[c], BLACK, WHITE, 2);
    }
    c = 0;
    first_flag = true;
  }

  if (first_flag == true) {
    if (c < CHAR_PER_LINE) {
      char_x = INITAL_CHAR_X + (CHAR_WIDTH * c);
      char_y = INITAL_CHAR_Y;
    } else {
      char_x = INITAL_CHAR_X + (CHAR_WIDTH * (c - CHAR_PER_LINE));
      char_y = INITAL_CHAR_Y + CHAR_LINE_HEIGHT;
    }
    tft.drawChar(char_x, char_y, text_buffer[c], BLACK, WHITE, 2);

    tft.drawLine(char_x, char_y + 16, char_x + 12, char_y + 16, BLACK);
    first_flag = false;
  }


  blink = millis() / 250;
  if (blink % 2) {
    tft.drawLine(char_x, char_y + 16, char_x + 12, char_y + 16, BLACK);
  } else {
    tft.drawLine(char_x, char_y + 16, char_x + 12, char_y + 16, WHITE);
  }

  while (Serial.available() > 0) { //still recieving keypresses for current input
    newinput = Serial.read(); //convert from ASCII value to numerical value

    switch (newinput) {
      case '0': case '1': case '2': case '3': case '4': case '5':
      case '6': case '7': case '8': case '9': case ',':
      case '-': case '+': case '^': case '/': case '*': case 'x': case 'X':
        text_buffer[c] = newinput;
        Serial.println(text_buffer[c]);
        if (c < CHAR_PER_LINE) {
          char_x = INITAL_CHAR_X + (CHAR_WIDTH * c);
          char_y = INITAL_CHAR_Y;

        } else {
          char_x = INITAL_CHAR_X + (CHAR_WIDTH * (c - CHAR_PER_LINE));
          char_y = INITAL_CHAR_Y + CHAR_LINE_HEIGHT;
        }
        tft.drawLine(char_x, char_y + 16, char_x + 12, char_y + 16, WHITE);
        tft.drawChar(char_x, char_y, text_buffer[c], BLACK, WHITE, 2);
        c++;
        if (c < CHAR_PER_LINE) {
          char_x = INITAL_CHAR_X + (CHAR_WIDTH * c);
          char_y = INITAL_CHAR_Y;

        } else {
          char_x = INITAL_CHAR_X + (CHAR_WIDTH * (c - CHAR_PER_LINE));
          char_y = INITAL_CHAR_Y + CHAR_LINE_HEIGHT;
        }
        tft.drawLine(char_x, char_y + 16, char_x + 12, char_y + 16, BLACK);
        break;

      case '~': //backspace key
        tft.drawLine(char_x, char_y + 16, char_x + 12, char_y + 16, WHITE);
        if (c > 0) {
          c--;
          if (c <= CHAR_PER_LINE) {
            char_x = INITAL_CHAR_X + (CHAR_WIDTH * c);
            char_y = INITAL_CHAR_Y;
          } else {
            char_x = INITAL_CHAR_X + (CHAR_WIDTH * (c - CHAR_PER_LINE));
            char_y = INITAL_CHAR_Y + CHAR_LINE_HEIGHT;
          }
          text_buffer[c] = '\0';
          tft.drawChar(char_x, char_y, text_buffer[c], BLACK, WHITE, 2);
          tft.drawLine(char_x, char_y + 16, char_x + 12, char_y + 16, BLACK);
        }
        break;

      case ' ': //right arrow
        tft.drawLine(char_x, char_y + 16, char_x + 12, char_y + 16, WHITE);
        c++;
        if (c <= CHAR_PER_LINE) {
          char_x = INITAL_CHAR_X + (CHAR_WIDTH * c);
          char_y = INITAL_CHAR_Y;
        } else {
          char_x = INITAL_CHAR_X + (CHAR_WIDTH * (c - CHAR_PER_LINE));
          char_y = INITAL_CHAR_Y + CHAR_LINE_HEIGHT;
        }
        tft.drawLine(char_x, char_y + 16, char_x + 12, char_y + 16, BLACK);
        break;

      case '=': //enter key
        text_buffer[c] = newinput;
        entry_complete = true;
        text_output_ptr = &text_buffer[0];
        tft.drawLine(char_x, char_y + 16, char_x + 12, char_y + 16, WHITE);
        c = 0;
        while (text_buffer[c] != '\0') {
          Serial.println();
          Serial.print("text buffer direct output: ");
          Serial.print(text_buffer[c]);
          c++;
        }
        c = 0;
        break;


      default:
        break;
    }
    if (c == 2 * CHAR_PER_LINE) {
      c = 0;
    }
  }




  return entry_complete;
}

//end update_screen

/* Gui.h - arduino graphing calc gui v1
    Paul Han 4/27/18
*/

#ifndef Gui_h
#define Gui_h

#include "Arduino.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SPITFT.h>
#include <Adafruit_SPITFT_Macros.h>
#include <gfxfont.h>
#include <Adafruit_ILI9341.h>
#include <PS2Keyboard.h>

extern Adafruit_ILI9341 tft;
extern PS2Keyboard keyboard;

class Gui {

  public:
    Gui();
    bool update_screen(char*& text_output_ptr); //char*& text_output_ptr

  private:
    char text_buffer[51];
    int  t; //counter for text buffer
    char newinput;
    bool entry_complete; //returned by update_screen when =
    int char_x;
    int char_y;
    bool first_flag;
};
#endif

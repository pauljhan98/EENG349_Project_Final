/* Gui.h - arduino graphing calc gui v1
    Paul Han 4/27/18
*/

#ifndef Gui_h
#define Gui_h

#include "Arduino.h"
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <PS2Keyboard.h>

struct data { //holds last three answers for user display
  char input[51];
  float output;
  bool data_graph_flag;
  long table_start_address;
  long table_end_address;
  bool in_use; //true if data is meant to be displayed
};

extern Adafruit_ILI9341 tft;
extern PS2Keyboard keyboard;
extern struct data output_data[3];


class Gui {

  public:
    Gui();
    bool update_screen(char*& text_output_ptr, int& buffer_size); //char*& text_output_ptr
    void restore_home(); //restores home graphic
    void display_answers(); //displays calculated answers

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

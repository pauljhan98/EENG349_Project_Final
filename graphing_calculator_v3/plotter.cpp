/*
   plotter.cpp
   written by Andrew K.
   5/1/2018
*/

#include "plotter.h"



#define X_CENTER 0
#define Y_CENTER 0
#define TICK_HEIGHT 5

#define WEIGHT .075   // Pixel weight, i.e decimal representation of the interval of a single pixel

bool exit_flag = false; //flag active when user presses esc key
char exit_input; //buffer for keyboard input

unsigned long testDrawGraph(Adafruit_ILI9341 *tft_point)
{
  tft_point->startWrite();                         // Call this function to allow us to write to the display




  for (int x = 1; x <= 320; x++) {
    tft_point->writePixel(x, x, ILI9341_RED);
    tft_point->writePixel(x, x + 1, ILI9341_RED);
  }

  tft_point->endWrite();                          // Call this function to let the screen know we don't want to write to it anymore
}


unsigned long drawAxis(Adafruit_ILI9341 *tft_point)
{
  tft_point->fillScreen(ILI9341_WHITE);

  tft_point->startWrite();
  tft_point->setRotation(0);

  tft_point->drawFastVLine(120, 0, 320, ILI9341_BLACK);                                              // Adds the horizontal line. Length of line is 320. (goes across whole screen)
  tft_point->drawFastHLine(0, 160, 240, ILI9341_BLACK);                                              // Adds the vertical line. Length of line is 240 (goes across whole screen)

  // Find out where to draw the interval spacing lines and their numbers
  // Weight defines the interval of each pixel. The idea here is to find
  // the pixels that corresponds to the whole numbers in some interval [-11x, 11x] for the horizontal axis
  // and [-9y, 9y] for the vertical component
  int center_x = X_CENTER;   // Defines the x value of the center pixel of the screen
  int center_y = Y_CENTER;   // Defines the y value of the center pixel of the screen

  // Idea: Increment by the weight (interval each pixel represents) until you get to a number close to an integer value. Then draw a line
  // Sometimes, you will need to check which pixel, i.e before or after the integer, represents the closest number to the integer values
  // Choose the pixel closest to the integer value to drawn the line and write the number

  // Increment such that we display 11 x-values, and 9 y-values in each half-axis.
  // From the center of the screen, construct the axis lines
  double current_number;
  double prior_number;

  // Plot along the positive x

  for (int i = 160; i <= 320; i++) {
    current_number = center_x + (WEIGHT * (i - 160));                                   // current_number is the number the current pixel represents

    for (int j = center_x + 1; j <= center_x + 11; j++) {
      if ((current_number >= j) && (current_number - WEIGHT < j)) {
        tft_point->drawFastHLine(120, i, TICK_HEIGHT, ILI9341_BLACK);                   // Line needs to go through the axis. So draw 10 pixels above and below as part of the line
        tft_point->drawFastHLine(120 - TICK_HEIGHT, i, TICK_HEIGHT, ILI9341_BLACK);

        tft_point->setCursor(i - 3, 127);
        tft_point->setTextColor(ILI9341_BLACK);
        tft_point->setRotation(1);              // Rotate so the numbers are nice
        tft_point->setTextSize(0);

        char snum[5];

        itoa(j, snum, 10);
        tft_point->println(snum);
        tft_point->setRotation(0);
      }
    }
  }

  // Plot along the negative x

  for (int i = 160; i >= 0; i--) {
    current_number = center_x + (WEIGHT * (i - 160));                                   // current_number is the number the current pixel represents

    for (int j = center_x - 1; j >= center_x - 11; j--) {
      if ((current_number > j) && (current_number - WEIGHT <= j)) {
        tft_point->drawFastHLine(120, i, TICK_HEIGHT, ILI9341_BLACK);                  // Line needs to go through the axis. So draw 10 pixels above and below as part of the line
        tft_point->drawFastHLine(120 - TICK_HEIGHT, i, TICK_HEIGHT, ILI9341_BLACK);

        tft_point->setCursor(i - 5, 127);
        tft_point->setTextColor(ILI9341_BLACK);
        tft_point->setRotation(1);              // Rotate so the numbers are nice
        tft_point->setTextSize(0);

        char snum[5];

        itoa(j, snum, 10);
        tft_point->println(snum);
        tft_point->setRotation(0);
      }
    }
  }

  // Plot along the postive y

  for (int i = 120; i <= 240; i++) {
    current_number = center_y + (WEIGHT * (i - 120));                               // current_number is the number the current pixel represents

    for (int j = center_y + 1; j <= center_y + 9; j++) {
      if ((current_number >= j) && (current_number - WEIGHT < j)) {
        tft_point->drawFastVLine(i, 160, TICK_HEIGHT, ILI9341_BLACK);              // Line needs to go through the axis. So draw 10 pixels above and below as part of the line
        tft_point->drawFastVLine(i, 160 - TICK_HEIGHT, TICK_HEIGHT, ILI9341_BLACK);

        // Here, we are actually numbering the ticks on the negative y axis,
        // due to the limitations of the rotation function
        // The positive y-axis ticks are therefore numbered in the negative axis function
        tft_point->setCursor(170, i - 3);
        tft_point->setTextColor(ILI9341_BLACK);
        tft_point->setRotation(1);              // Rotate so the numbers are nice
        tft_point->setTextSize(0);

        char snum[5];

        itoa(-1 * j, snum, 10);
        tft_point->println(snum);
        tft_point->setRotation(0);
      }
    }
  }

  // Plot along the negative y

  for (int i = 120; i >= 0; i--) {
    current_number = center_y + (WEIGHT * (i - 120));                               // current_number is the number the current pixel represents

    for (int j = center_y - 1; j >= center_y - 9; j--) {
      if ((current_number > j) && (current_number - WEIGHT <= j)) {
        tft_point->drawFastVLine(i, 160, TICK_HEIGHT, ILI9341_BLACK);               // Line needs to go through the axis. So draw 10 pixels above and below as part of the line
        tft_point->drawFastVLine(i, 160 - TICK_HEIGHT, TICK_HEIGHT, ILI9341_BLACK);

        tft_point->setCursor(170, i - 3);
        tft_point->setTextColor(ILI9341_BLACK);
        tft_point->setRotation(1);                                                 // Rotate so the numbers are nice
        tft_point->setTextSize(0);

        char snum[5];

        itoa(-1 * j, snum, 10);
        tft_point->println(snum);
        tft_point->setRotation(0);
      }
    }
  }

  tft_point->setRotation(3);
  tft_point->endWrite();
}

void makePlot(Adafruit_ILI9341 *tft_point, long int start_address, long int end_address, float *output_point, SpiRAM spiram) {

  drawAxis(tft_point);   // Make the screen blank, and plot the numbered axis

  // Next step: we look into the stack to retrieve the x and y values




  tft_point->startWrite();
  tft_point->setRotation(0);

  int pixel_counter = 0;
  float y = 0;
  for (long i = start_address; i <= end_address; i += 8) {

    spiram.read_floats(i, output_point, 2);
    y = output_point[1];

    // We know y, we want to now find the correct pixel in the y axis to display.

    if (y == 0) {
      y = 120;
    }
    else if (y > 0) {
      y = 120 + (y / WEIGHT);
    }
    else if (y < 0) {
      y = 120 - abs((y / WEIGHT));
    }
    else {
      y = 0;
    }


    tft_point->drawPixel(y, pixel_counter, ILI9341_RED);

    pixel_counter++;
  }

  tft_point->setRotation(3);
  tft_point->endWrite();

  while (exit_flag == false) { //this way leaves options for future user options
    while (keyboard.available()) { //still recieving keypresses for current input
      exit_input = keyboard.read(); //convert from ASCII value to numerical value
      switch (exit_input) {
        case PS2_ESC:
          exit_flag = true;
          break;

        default:
          break;
      }//end switch
    } //end while keyboard
  }//end while exit flag
  exit_flag = false;
}



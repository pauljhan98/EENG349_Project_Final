/*
 * plotter.h
 * written by Andrew K.
 * 5/1/2018
 */

#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#include <SPI.h>
#include <SpiRam_Extended.h>
#include <PS2Keyboard.h>

extern PS2Keyboard keyboard;
unsigned long testDrawGraph(Adafruit_ILI9341 *tft_point);  

unsigned long drawAxis(Adafruit_ILI9341 *tft_point);  

void makePlot(Adafruit_ILI9341 *tft_point, long int start_address, long int end_address, float *output_point, SpiRAM spiram);

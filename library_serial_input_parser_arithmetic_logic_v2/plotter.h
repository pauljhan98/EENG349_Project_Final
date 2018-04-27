#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#include "SPI.h"


unsigned long testDrawGraph(Adafruit_ILI9341 *tft_point);  

unsigned long drawAxis(Adafruit_ILI9341 *tft_point);  

void cordicTrig(double angle, double *cosine, double *sine);

void cordicTan(double angle, double *tangent);

void makePlot(Adafruit_ILI9341 *tft_point);

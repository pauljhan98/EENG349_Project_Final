#include "plotter.h"



#define X_CENTER 0
#define Y_CENTER 0
#define TICK_HEIGHT 5

#define WEIGHT .075   // Pixel weight, i.e decimal representation of the interval of a single pixel

#define TAN_VALUES 34

double tan_values[TAN_VALUES] {         // Pre-calculated arc-tangent values
                .7853981634,
                .463647609,
                .2449786631,
                .1243549945,
                .06241881,
                .03123983343,
                .01562372862,
                .00781234106,
                .003906230132,
                .001953122516,
                .0009765621896,
                .0004882812112,
                .0002441406201,
                .0001220703119,
                .00006103515617,
                .00003051757812,
                .00001525878906,
                .000007629394531,
                .000003814697266,
                .000001907348633,
                .0000009536743164,
                .0000004768371582,
                .0000002384185791,
                .0000001192092896,
                .00000005960464478,
                .00000002980232239,
                .00000001490116119,
                .000000007450580597,
                .000000003725290298,
                .000000001862645149,
                .0000000009313225746,
                .0000000004656612873,
                .0000000002328306437,
                .0000000001164153218
};


unsigned long testDrawGraph(Adafruit_ILI9341 *tft_point)            
{        
  tft_point->startWrite();                         // Call this function to allow us to write to the display
     
  for (int x = 1; x <= 320; x++) {
    tft_point->writePixel(x, x, ILI9341_RED);
    tft_point->writePixel(x, x+1, ILI9341_RED);            
   }
     
  tft_point->endWrite();                          // Call this function to let the screen know we don't want to write to it anymore
}


unsigned long drawAxis(Adafruit_ILI9341 *tft_point)
{
  tft_point->fillScreen(ILI9341_WHITE);

 tft_point->startWrite();

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
    current_number = center_x + (WEIGHT*(i - 160));                                     // current_number is the number the current pixel represents

    for(int j = center_x + 1; j <= center_x + 11; j++) {
      if ((current_number >= j) && (current_number - WEIGHT < j)) {
        tft_point->drawFastHLine(120, i, TICK_HEIGHT, ILI9341_BLACK);                   // Line needs to go through the axis. So draw 10 pixels above and below as part of the line
        tft_point->drawFastHLine(120-TICK_HEIGHT,i, TICK_HEIGHT, ILI9341_BLACK);

        tft_point->setCursor(i-3, 127);
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
    current_number = center_x + (WEIGHT*(i - 160));                                     // current_number is the number the current pixel represents

    for(int j = center_x - 1; j >= center_x - 11; j--) {
      if ((current_number > j) && (current_number - WEIGHT <= j)) {
        tft_point->drawFastHLine(120, i, TICK_HEIGHT, ILI9341_BLACK);                  // Line needs to go through the axis. So draw 10 pixels above and below as part of the line
        tft_point->drawFastHLine(120-TICK_HEIGHT,i, TICK_HEIGHT, ILI9341_BLACK);

        tft_point->setCursor(i-5, 127);
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
    current_number = center_y + (WEIGHT*(i - 120));                                 // current_number is the number the current pixel represents

    for(int j = center_y + 1; j <= center_y + 9; j++) {
      if ((current_number >= j) && (current_number - WEIGHT < j)) {
        tft_point->drawFastVLine(i, 160, TICK_HEIGHT, ILI9341_BLACK);              // Line needs to go through the axis. So draw 10 pixels above and below as part of the line
        tft_point->drawFastVLine(i,160-TICK_HEIGHT, TICK_HEIGHT, ILI9341_BLACK);

        // Here, we are actually numbering the ticks on the negative y axis,
        // due to the limitations of the rotation function
        // The positive y-axis ticks are therefore numbered in the negative axis function
        tft_point->setCursor(170, i-3);
        tft_point->setTextColor(ILI9341_BLACK);
        tft_point->setRotation(1);              // Rotate so the numbers are nice
        tft_point->setTextSize(0);

        char snum[5];

        itoa(-1*j, snum, 10);
        tft_point->println(snum);
        tft_point->setRotation(0);
      }    
    }   
  }

// Plot along the negative y 

  for (int i = 120; i >= 0; i--) {
    current_number = center_y + (WEIGHT*(i - 120));                                 // current_number is the number the current pixel represents

    for(int j = center_y - 1; j >= center_y - 9; j--) {
      if ((current_number > j) && (current_number - WEIGHT <= j)) {
        tft_point->drawFastVLine(i, 160, TICK_HEIGHT, ILI9341_BLACK);               // Line needs to go through the axis. So draw 10 pixels above and below as part of the line
        tft_point->drawFastVLine(i,160-TICK_HEIGHT, TICK_HEIGHT, ILI9341_BLACK);
        
        tft_point->setCursor(170, i-3);
        tft_point->setTextColor(ILI9341_BLACK);
        tft_point->setRotation(1);                                                 // Rotate so the numbers are nice
        tft_point->setTextSize(0);

        char snum[5];

        itoa(-1*j, snum, 10);
        tft_point->println(snum);
        tft_point->setRotation(0);
      }    
    }   
  } 
 
 tft_point->endWrite();
}

// For some given x value, we treat it as an angle (given in radians)
// currently only works for the range pi/2, -pi/2
void cordicTrig(double angle, double *cosine, double *sine) {


  int tan_point = *tan_values;                                                // Clean this up, half of these variables you don't need anymore
  double cosine_cal = 0;
  double sine_cal = 0;

  double x = .6037;
  double y = 0;

  double x_old;
  double y_old;

  int sign = 1;

  double tan_val = 0;

  int invert_sine = 1;
  
  if (angle < 0) {
    angle = abs(angle);
    invert_sine = -1;   
  }
  
  // Shift angle into [-pi, pi]
  if (angle > 3.14) {
  int accumlated_periods = angle/1.57;

  angle = angle - (accumlated_periods*1.57);

  Serial.print("Accumlated periods ");
  Serial.println(accumlated_periods);

  Serial.print("Augmented angle value ");
  Serial.println(angle);
  }

  if (angle < 3.14) {
  int accumlated_periods = angle/1.57;

  angle = angle + (accumlated_periods*1.57);

  Serial.print("Accumlated periods ");
  Serial.println(accumlated_periods);

  Serial.print("Augmented angle value ");
  Serial.println(angle);
  }

  // Now, shift into [-pi/2, pi/2] and account for appropriate sign changes


  int signfactor = 1;

  if (angle < -1.57) {
    angle = angle + 3.14;
    signfactor = -1;
  }
  
  else if (1.57 < angle) {             // I don't think this is needed given the current sin implememtation
    angle = angle - 3.14;
    signfactor = -1;
  }

  

  for (int i = 0; i <= TAN_VALUES; i++) {

    if ((angle - tan_val) > 0) {
      sign = 1;
    }
    else {
      sign = -1;
    }
    
    if (sign < 0) {
     
      x_old = x + (y*pow(2, -i));
          y = y - (x*pow(2, -i));
          x = x_old;
    tan_val = tan_val - tan_values[i];
    
    } 
    else {
      
      x_old = x - (y*pow(2, -i));
          y = y + (x*pow(2, -i));
          x = x_old;
    tan_val = tan_val + tan_values[i];
    
    }
    
    Serial.println(y, 8);    
    Serial.println(x, 8);
  } 

 if (invert_sine == -1) {
  *cosine = x*signfactor;
  *sine = y*signfactor*invert_sine;
 }
  else {
  *cosine = x*signfactor;         // Assign the values of cosine and sine
  *sine = y*signfactor;           // To the appropiate places in memory
  }
}

void cordicTan(double angle, double *tangent) {
 
 double cosine;
 double sine;
 double *cos_point = &cosine;
 double *sin_point = &sine;

 cordicTrig(angle, cos_point, sin_point);
 *tangent = (sine/cosine);
}

void makePlot(Adafruit_ILI9341 *tft_point) {

  drawAxis(tft_point);   // Make the screen blank, and plot the numbered axis

                         // Next step: we look into the stack to retrieve the x and y values
                         // Then plot those points

  
}


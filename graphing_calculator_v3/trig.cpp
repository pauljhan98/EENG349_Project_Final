/*
 * trig.cpp
 * written by Andrew K.
 * 5/1/2018
 */



#include "trig.h"

#define TAN_VALUES 34


double tan_values[TAN_VALUES] {         // Pre-calculated arc-tangent values
               //  1.57079632679,
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


void cordicTrig(float angle, float *cosine, float *sine) {

 int cosfactor = 1;
 int sinefactor = 1;
 float angle2 = angle;

 
 // Move input angle into correct bounds [0, pi]
 while (angle > PI) {
   angle = angle - PI;
 }

 while (angle < 0) {
  angle = angle + PI;
 }


 while (angle2 > PI ) {
  angle2 = angle2 - 2*PI;
 }

 while (angle2 < -PI) {
  angle2 = angle2 + 2*PI;
 }
 

 // Figure out what interval cosine and sine are in
 // So that the signs are appropriately set for the final answer

 if ((angle2 >= -HALF_PI) && (angle2 <= HALF_PI)) {
  cosfactor = 1;
   if ((angle2 < PI) && (angle2 > 0)) {
    sinefactor = 1;
   }
   else if ((angle2 > -PI) && (angle2 < 0)) {
    sinefactor = -1;
   }
 }

 else if (((angle2 >= -PI) && (angle2 <= -HALF_PI)) || ((angle2 >= HALF_PI) && (angle2 <= PI))) {
  cosfactor = -1;
   if ((angle2 < PI) && (angle2 > 0)) {
    sinefactor = 1;
   }
   else if ((angle2 > -PI) && (angle2 < 0)) {
    sinefactor = -1;
   }
 } 





 double xval = 0;
 double yval = 1;

 double xval_prior;
 double yval_prior;

 double accumulated_angle = HALF_PI;
 
 for (int i = 0; i <= TAN_VALUES; i++) {
  if (accumulated_angle >= angle) {
    xval_prior = xval;
    xval = xval + (yval*pow(2, -i));
    yval = yval - (xval_prior*pow(2, -i));

    accumulated_angle = accumulated_angle - tan_values[i];
  }
  else { // If accumulated_angle < target angle
    xval_prior = xval;
    xval = xval - (yval*pow(2, -i));
    yval = yval + (xval_prior*pow(2, -i));

   accumulated_angle = accumulated_angle + tan_values[i];
  } 
 }

 *cosine = abs(xval)*cosfactor*.6037;         // Assign the values of cosine and sine
 *sine = abs(yval)*sinefactor*.6037; 
}

void cordicTan(float angle, float *tangent) {
 
 float cosine;
 float sine;
 float *cos_point = &cosine;
 float *sin_point = &sine;

 cordicTrig(angle, cos_point, sin_point);
 *tangent = (sine/cosine);
}


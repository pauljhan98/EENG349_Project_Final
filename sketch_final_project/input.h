
//first implementation of external ram storage
#include <StackArray.h> //for Input_Parsing
#include <SpiRam_Extended.h> //for external spi ram storage
#include "plotter.h"


#define WEIGHT .075
#define INITIAL_VALUE 0


void Input_Parsing();

void Arithmetic_Logic ();


//Start Input Parsing globals
extern bool newInput = false;      //flag for completion of current input; for input_parsing; global for now
extern float assembled_number;     //output of number parsing; global for now
extern byte assembled_count = 0;   //number of assembled values in stack
extern char operator_flags[21];    //char array for keeping track of which stack items are operators (includes X variable)
extern char *operator_flags_ptr;
//end input parsing globals

//start arithmetic logic globals
extern bool x_operator_flag = false;     //flag when X operator detected, triggers Arithmetic logic graph table
extern float x_start = INITIAL_VALUE - (160*WEIGHT);                //starting value of x, at the least value along the axis
extern float step_size = WEIGHT;              //step size of graph (value per pixel)
extern long iterations = 320;            //number of x iterations; i.e, how many pixels along the x axis
extern bool graph_flag = false;          //flag for graph vs arthmetic

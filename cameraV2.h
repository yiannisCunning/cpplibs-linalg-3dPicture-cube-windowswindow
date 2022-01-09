#pragma once
#include <windows.h>
#include <stdio.h>
#include <math.h>
#include "C:\Users\yiann\Documents\Coding_Projects\cppLibs\cube\cube.h"
#include "C:\Users\yiann\Documents\Coding_Projects\cppLibs\linalg\linalg.h"



class cameraV2{


       float *v;
       float *h1;
       float *h2;

       // constants
       float *yAxis;        // = -2 * h1
       float *xAxis;        // = -2 *h2
       float yFact;         // = height/mag(yAxis)
       float xFact;         // = height/mag(xAxis)
       float mag;

       float *depthScreen;


public:
       BYTE *screen;
       int width;
       int height;

       cameraV2(int height_in, int width_in);
       ~cameraV2();

       /* Sets where the scene is being viewed from                                        */
       void setlens(float *v_in);

       /* Draws the xyz axises                                                             */
       void drawAxis();

       /* clears the screen                                                                */
       void clear_screen();



       /* Gives point on screen for point.                                                 */
       void point_from_vector(float *w, float *ans, float *depth);

       /* Gives the w/x, h/y cords of screen given a vector on the plane of the screen     */
       bool screen_cords(float *w, float *depth, float *cords);

       /* Gives the position on the screen from a vector/point                             */
       bool cord_from_vector(float *w, float *cords);

       /*                                                                                  */
       void draw_cube(cube c);


       /* Returns true iff cords is withing range of 0-width, 0-height;                    */
       static bool in_range(float *cords, int height, int width);




       /* Paints a line from cord_i to cord_f with clor clr and size size*/
       void paint_line(float *cord_i, float *cord_f, BYTE *clr, int size);

       /* Paints a dot at a cord                                                           */
       void paint_cord(float *cord, int size, BYTE *clr);

       void setColor(BYTE *clr, int clrNum);

       void paint_square(float *cordA, float *cordB, float *cordC, float *cordD, BYTE *clr, int size);

       void outline_sqaure(float *cordA, float *cordB, float *cordC, float *cordD, BYTE *clr, int size);
};


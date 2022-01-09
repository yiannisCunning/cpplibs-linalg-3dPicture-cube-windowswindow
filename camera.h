#pragma once
#include <windows.h>
#include <stdio.h>
#include <math.h>
#include "C:\Users\yiann\Documents\Coding_Projects\cppLibs\cube\cube.h"
#include "C:\Users\yiann\Documents\Coding_Projects\cppLibs\linalg\linalg.h"
#include <process.h>





/*camera in the scene*/
/* Holds image in byte array of h by w.*/


class camera{


       float *v;     // Direction of veiwing
       float *h1;    // cross of v and h2, scaled to a factor
       float *h2;    // cross of v and k hat, scaled to a factor
       float *q;     // scaler multiple of v
       float *depthScreen;

       float b;      // the scalar used to make q, st q*b = v
       float m;      // magnitude of q
       float *vOther;// = (1-b)*q

public:
       BYTE *screen;
       int width;
       int height;

       /*     These methods are for direct interaction with the camera object              */
       /*     Does not depend on projection method - all only interface with               */
       /*     cord_from_vector and painting methods.                                       */

       /* constructor and destructor - allocate and free space                             */
       camera(int height_in, int width_in);
       ~camera();

       /* Sets the perpective of the window into the graphical region                      */
       void setlens(float *v_in);

       /* Draws the xyz axises                                                             */
       void drawAxis();

       /* clears the screen                                                                */
       void clear_screen();

       /* Draws the cube at the origin                                                     */
       void draw_cube(cube c);

       /* Draws a vector from v1 to v2                                                     */
       void draw_vector(float *v1, float *v2);

       void draw_triangle(float *v1, float *v2, float *v3);

       /* Draws the given bitmap on the square v1 -> v2                                    */
       void draw_rectangular_bitmap(float *v1, float* v2, float *P, BYTE *picture, int pix_height, int pix_width);

       void draw_STL(char *filename, float *p, float scaleDown);



       /*     These functions do not change with methods also                              */

       /* given a vector that goes onto the q plane, gives the cords where the screen is   */
       /*     q +- h1 and h2                                                               */
       void screen_cords(float *w, float *depth, float *cords);

       /* Returns true iff cords is within range of 0-width, 0-height;                     */
       static bool in_range(float *cords, int height, int width);



       /*     Dependant on method                                                          */

       /* Gives the point on the screen (q +- h1/h2) from a vector                         */
       /*     Returns false if outside the screen. True otherwise                          */
       bool point_from_vector(float *w, float *ans, float *depth);

       bool point_from_vector_other(float *w, float *ans, float *depth);

       bool point_from_vector_other_other(float *w, float *ans, float *depth);

       bool point_from_vector_4(float *w, float *ans, float *depth);

       /* Gives the position on the screen from a vector/point                             */
       bool cord_from_vector(float *w, float *cords);

       void set_constants_Lt(float *w1, float *w2, float *a1mag, float *a3, float *a4, float *lTotal);

       void pait_hor_line_from_bitmap(float *w1, float *w2, BYTE *bitmap_at_h, int pix_width);




       /*     These functions are non-specic, only based on cords on the bitmap            */

       /* Paints a line from cord_i to cord_f with clor clr and size size                  */
       void paint_line(float *cord_i, float *cord_f, BYTE *clr, int size);

       /* Paints a square from line AB to CD                                               */
       void paint_square(float *cordA, float *cordB, float *cordC, float *cordD, BYTE *clr, int size);

       /* Paints an outline for a square                                                   */
       void outline_sqaure(float *cordA, float *cordB, float *cordC, float *cordD, BYTE *clr, int size);

       /* Pain an induvidual coordanate                                                    */
       void paint_cord(float *cord, int size, BYTE *clr);

       /* Fills a color based on arbitrary number                                          */
       void setColor(BYTE *clr, int clrNum);

};
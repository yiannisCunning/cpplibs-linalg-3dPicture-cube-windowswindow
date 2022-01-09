#pragma once
#include "C:\Users\yiann\Documents\Coding_Projects\cppLibs\linalg\linalg.h"
#include <math.h>



struct square
{
       float **pointers;
       int *cols;
       float *posVec;
};


class cube{

       const float Azccw[9] = {0, -1, 0, 1, 0, 0, 0, 0, 1};
       const float Azcw[9] = { 0, 1, 0, -1, 0, 0, 0, 0, 1};
       const float Axccw[9] = { 1, 0, 0, 0, 0,-1, 0, 1, 0};
       const float Axcw[9] = {1, 0, 0, 0, 0, 1, 0, -1, 0};
       const float Ayccw[9] = {0, 0, 1, 0, 1, 0, -1, 0, 0};
       const float Aycw[9] = {0, 0, -1, 0, 1, 0, 1, 0, 0};
       const float *d[6] = {Axccw, Ayccw, Azccw, Axcw, Aycw, Azcw};

public:
       struct square **sqrs;


       cube();
       void color_all_faces();
       bool turn(float *w, bool cw);
       bool rotate_face(float *w, bool cw);



       struct square *makeSquare(float x, float y, float z);
       bool color_face(float *w, int clr);
};
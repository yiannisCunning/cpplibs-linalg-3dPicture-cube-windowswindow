#include "cube.h"


struct square *cube::makeSquare(float x, float y, float z)
{
       // initialize pointer holder; - square
       struct square *ans = (struct square *)calloc(1, sizeof(struct square));

       // initialize posVec and give pointer to square
       float *posVec = (float *)calloc(3, sizeof(float));
       ans->posVec = posVec;
       setVector(ans->posVec, x, y, z);

       // iniitlaize color array and give pointer to square
       int *clrs = (int *)calloc(6, sizeof(int));
       ans->cols = clrs;

       float **point = (float **)calloc(6, sizeof(float *));
       ans->pointers = point;

       float *temp;
       for(int i = 0; i < 3; i++){
              for(int mag = -1; mag <= 1; mag += 2)
              {
                     temp = (float *)calloc(3, sizeof(float));
                     temp[i] = mag;
                     ans->pointers[i + (mag == 1)*3] = temp;

              }
       }
       return ans;
}





cube::cube()
{
       sqrs = (struct square **)calloc(27, sizeof(struct square *));
       struct square *temp;

       for(int x = -1; x <= 1; x++)
       {
              for(int y = -1; y <= 1; y++)
              {
                     for(int z = -1; z <= 1; z++)
                     {
                            temp = makeSquare(x, y, z);
                            sqrs[(x + 1) + (y + 1)*3 + (z+1)*9] = temp;
                     }
              }
       }
}


bool cube::color_face(float *w, int clr)
{
       if(vecMag(w) != 1 || abs(w[0] + w[1] + w[2]) != 1 ){return false;}

       int nonZeroComp = abs(w[1]) + 2*abs(w[2]);
       float valueOfComp = w[0] + w[1] + w[2];
       for(int i = 0; i < 27; i++)
       {
              if(sqrs[i]->posVec[nonZeroComp] == valueOfComp) // for each square on that side of the plane
              {
                     for(int q = 0; q < 6; q++)
                     {
                            if(vecEqual(sqrs[i]->pointers[q], w)){
                                   sqrs[i]->cols[q] = clr;
                            }
                     }
              }
       }
       return true;
}



void cube::color_all_faces()
{
       float w[3] = {0,0,0};
       setVector(w, 1,0,0);
       color_face(w, 4);

       setVector(w, 0,1,0);
       color_face(w, 2);

       setVector(w, 0,0,1);
       color_face(w, 0);

       setVector(w, -1,0,0);
       color_face(w, 5);

       setVector(w, 0,-1,0);
       color_face(w, 3);

       setVector(w, 0,0,-1);
       color_face(w, 1);
}




bool cube::turn(float *w, bool cw)
{
       if(vecMag(w) != 1 || abs(w[0] + w[1] + w[2]) != 1 ){return false;}

       int nonZeroComp = abs(w[1]) + 2*abs(w[2]);
       float valueOfComp = w[0] + w[1] + w[2];

       // find the rotation matrix: (could be wrong?)
       int temp = cw ? 1:-1;
       int ai = nonZeroComp + (valueOfComp == temp)*3;
       const float *A = d[ai];

       // Then apply it to all sqrs posVec, and all of their pointers
       for(int i = 0; i<27; i++)
       {
              matix_mult(A, sqrs[i]->posVec, sqrs[i]->posVec);
              for(int q = 0; q < 6; q++)
              {
                     matix_mult(A, sqrs[i]->pointers[q], sqrs[i]->pointers[q]);
              }
       }
       return true;
}


bool cube::rotate_face(float *w, bool cw)
{
       if(vecMag(w) != 1 || abs(w[0] + w[1] + w[2]) != 1 ){return false;}

       int nonZeroComp = abs(w[1]) + 2*abs(w[2]);
       float valueOfComp = w[0] + w[1] + w[2];

       // find the rotation matrix: (could be wrong?)
       int temp = cw ? 1:-1;
       int ai = nonZeroComp + (valueOfComp == temp)*3;
       const float *A = d[ai];


       // Then apply it to only squares in the plane of the rotation
       //     change their posVec and pos vec of all their pointers
       for(int i = 0; i<27; i++)
       {
              if(sqrs[i]->posVec[nonZeroComp] == valueOfComp)
              {
              matix_mult(A, sqrs[i]->posVec, sqrs[i]->posVec);
              for(int q = 0; q < 6; q++)
              {
                     matix_mult(A, sqrs[i]->pointers[q], sqrs[i]->pointers[q]);
              }
              }
       }
       return true;
}
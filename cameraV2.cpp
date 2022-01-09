#include "cameraV2.h"



cameraV2::cameraV2(int width_in, int height_in)
{
       width = width_in;
       height = height_in;


       screen = (BYTE *)calloc(width * height * 4, sizeof(BYTE));
       depthScreen = (float *)calloc(width * height, sizeof(float));

       for(int i =0; i < width*height; i++){depthScreen[i] = 999999999;}

       v = (float *)calloc(3, sizeof(float));
       h1 = (float *)calloc(3, sizeof(float));
       h2 = (float *)calloc(3, sizeof(float));
       yAxis = (float *)calloc(3, sizeof(float));
       xAxis = (float *)calloc(3, sizeof(float));

       xFact = 0;
       yFact = 0;
}



cameraV2::~cameraV2()
{
       if(screen != NULL){free(screen);}
}

void cameraV2::setlens(float *v_in)
{
       cpyVec(v_in, v);
       mag = vecMag(v);

       float temp[3] = {0,0,1};
       cross(v, temp, h2);
       cross(h2, v, h1);
       
       // Set size of screen - base is 1
       normalize(h1);
       normalize(h2);
       constMult(2, h1, h1);
       constMult(2, h2, h2);


       constMult(-2, h1, yAxis);
       constMult(2, h2, xAxis);
       xFact = width/vecMag(xAxis);
       yFact = height/vecMag(yAxis);
}

void cameraV2::clear_screen()
{
       BYTE component[4] = {0xFF, 0xFF, 0xFF, 0xFF};
       for(int i = 0; i < width*height*4; i++){
              screen[i] = component[i%4];
              if(i % 4 == 0){
                     depthScreen[i/4] = 999999999;
              }
       }
}

void cameraV2::drawAxis()
{


       BYTE clr[4] = {0x00, 0x00, 0x00, 0xFF};
       bool res;

       float point[3] = {-10,-10,-10};
       float cordOrigin[3] = {0,0,0};
       float cord2[3] = {0,0,0};
       cord_from_vector(point, cordOrigin);

       for(int i = 0; i < 6; i++){
              setVector(point, -10, -10, -10);
              point[i%3] = -9;
              if(i>=3){point[i%3] = -11;}

              res = cord_from_vector(point, cord2);
              if(res){
                     if(i>=3){paint_line(cordOrigin, cord2, clr, 1);}
                     else{paint_line(cordOrigin, cord2, clr, 1);}
              }
       }
       
}

















void cameraV2::point_from_vector(float *w, float *ans, float *depth)
{
       *depth = vecMag(w);
       constMult(mag/(*depth), w, ans);
}


bool cameraV2::screen_cords(float *w, float *depth, float *cords)
{
       if(dotProduct(w, v) <= 0){return 0;}
       // NOTE: Can do this with arc distance or with projection


       //     1) Project onto the plane normal to v
       float wOnScreen[3] = {0,0,0};
       project(w, v, wOnScreen);
       subVec(w, wOnScreen, wOnScreen);
       subVec(wOnScreen, h1, wOnScreen);
       addVec(wOnScreen, h2, wOnScreen);

       if(dotProduct(wOnScreen, yAxis) <= 0){return 0;}
       if(dotProduct(wOnScreen, xAxis) <= 0){return 0;}


       //     2) Project projected vector onto h1, h2 to get cords
       float yComp[3] = {0,0,0};
       project(wOnScreen, yAxis, yComp);  // To find y cord -> want to project onto -2 * h1
       cords[1] = yFact * vecMag(yComp);


       float *xComp = yComp;              // ( just changing name )
       project(wOnScreen, xAxis, xComp);  // To find x cord -> want to project onto -2 * h2
       cords[0] = xFact * vecMag(xComp);

       //     3) Set depth
       cords[2] = *depth;

       return true;
}






/* Gives the position on the screen from a vector/point                             */
bool cameraV2::cord_from_vector(float *w, float *cords)
{
       float onScreen[3] = {0,0,0};
       bool tf;
       point_from_vector(w, onScreen, cords + 2);
       tf = screen_cords(onScreen, cords + 2, cords);

       // could be done with - return tf and !in_range()
       if(!tf){return false;}
       if(!in_range(cords, height, width)){return false;}
       return true;
}



// is cords in range
bool cameraV2::in_range(float *cords, int height, int width){
       if(cords[0] < 0 || cords[0] > width - 1){
              return false;
       }

       if(cords[1] < 0 || cords[1] > height - 1){
              return false;
       }
       return true;
}













// paints a line on the screen from cord to cord
void cameraV2::paint_line(float *cord_i, float *cord_f, BYTE *clr, int size)
{      
       // paint a whole bunch of points along the line
       

       float dx = cord_f[0] - cord_i[0];
       float dy = cord_f[1] - cord_i[1];
       float dz = cord_f[2] - cord_i[2];
       float mag = sqrt(dx*dx + dy*dy);
       float pos[3] = {0,0,0};
       float frac;
       for(int i = 0; i <= ceil(mag); i++){
              frac = i/mag;
              pos[0] = frac*dx + cord_i[0];
              pos[1] = frac*dy + cord_i[1];
              pos[2] = frac*dz + cord_i[2];

              paint_cord(pos, size, clr);
       }
}



// paits a induvidual pixel from cord
void cameraV2::paint_cord(float *cord, int size, BYTE *clr)
{
       // Paint a cord on 'screen', also put relative depth for each pixel on 'depthScreen'
       float temp[3] = {0,0,0};
       int n;
       for(int dx = -1*size; dx <= size; dx++){
              for(int dy = -1*size; dy <= size; dy++){
                     temp[0] = cord[0] + dx;
                     temp[1] = cord[1] + dy;
                     if(in_range(temp, height, width))
                     {
                            n = ((int)round(temp[0]) + (int)round(temp[1])*width);
                            if(depthScreen[n] >= cord[2]){
                                   depthScreen[n] = cord[2];
                                   screen[n*4] = clr[0];
                                   screen[n*4+1] = clr[1];
                                   screen[n*4+2] = clr[2];
                                   screen[n*4+3] = clr[3];
                            }
                     }
              }
       }
}







void cameraV2::draw_cube(cube c)
{
       float cordA[3] = {0,0,0};
       float cordB[3] = {0,0,0};
       float cordC[3] = {0,0,0};
       float cordD[3] = {0,0,0};

       float point[3] = {0,0,0};
       float center[3] = {0,0,0};
       float hp1[3] = {0,0,0};
       float hp2[3] = {0,0,0};
       int normalDirec = 0;
       float toadd[3] = {-10,-10,-10};

       BYTE clr[4] = {0x00, 0x00, 0x00, 0xFF};
       BYTE clr2[4] = {0x00, 0x00, 0x00, 0xFF};
       // cube uses 0 - red, 1 - orange, 2 - blue, 3 - green, 4 - yellow, 5 - white
       for(int i = 0; i < 27; i++)
       {
              for(int q = 0; q < 6; q++)
              {
                     if(cosAngle(c.sqrs[i]->pointers[q], c.sqrs[i]->posVec) < 1.57)
                     {      
                            // 0) set color
                            setColor(clr, c.sqrs[i]->cols[q]);

                            // 1) find the center of the square
                            constMult(0.5, c.sqrs[i]->pointers[q], center);
                            addVec(center, c.sqrs[i]->posVec, center);
                            addVec(center, toadd, center);

                            normalDirec = abs(c.sqrs[i]->pointers[q][1]) + abs(c.sqrs[i]->pointers[q][2])*2;

                            // 2) Find h1 and h2 for the square 
                            setVector(hp1, 0,0,0);
                            setVector(hp2, 0,0,0);
                            hp1[(normalDirec + 1)%3] = 0.5;
                            hp2[(normalDirec + 2)%3] = 0.5;
                     
                            // 3) find each cord of the corners
                            addVec(center, hp1, point);
                            addVec(point, hp2, point);
                            cord_from_vector(point, cordA);

                            addVec(center, hp1, point);
                            subVec(point, hp2, point);
                            cord_from_vector(point, cordB);

                            subVec(center, hp1, point);
                            subVec(point, hp2, point);
                            cord_from_vector(point, cordC);

                            subVec(center, hp1, point);
                            addVec(point, hp2, point);
                            cord_from_vector(point, cordD);

                            paint_square(cordA, cordB, cordC, cordD, clr, 1);
                            outline_sqaure(cordA, cordB, cordC, cordD, clr2, 2);
                     }
              }
       }

}



// for cube - sets a clr vector based on numbers
// cube uses 0 - red, 1 - orange, 2 - blue, 3 - green, 4 - yellow, 5 - white
void cameraV2::setColor(BYTE *clr, int clrNum)
{
       clr[0] = 0x0;
       clr[1] = 0x0;
       clr[2] = 0x0;
       clr[3] = 0xFF;

       switch (clrNum)
       {
       case 0:
              clr[2] = 0xFF;
              break;
       case 1:
              clr[1] = 128;
              clr[2] = 0xFF;
              break;
       case 2:
              clr[0] = 0xFF;
              break;
       case 3:
              clr[1] = 0xFF;
              break;
       case 4:
              clr[1] = 0xFF;
              clr[2] = 0xFF;
              break;
       case 5:
              clr[0] = 150;
              clr[1] = 150;
              clr[2] = 150;
              break;
       default:
              break;
       }


}


// pains a square in between cords
void cameraV2::paint_square(float *cordA, float *cordB, float *cordC, float *cordD, BYTE *clr, int size)
{
       // Start at a -> move towards b-> draw a line from each point to 
       float dxAB = cordA[0] - cordB[0];
       float dyAB = cordA[1] - cordB[1];
       float dzAB = cordA[2] - cordB[2];
       float magAB = sqrt(dxAB*dxAB + dyAB*dyAB);
       float dxCD = cordD[0] - cordC[0];
       float dyCD = cordD[1] - cordC[1];
       float dzCD = cordD[2] - cordC[2];
       float magCD = sqrt(dxCD*dxCD + dyCD*dyCD);


       float pos[3] = {0,0,0};
       float pos2[3] = {0,0,0};
       float frac;
       float length;

       length = fmax(magAB, magCD);

       for(int i = 0; i <= ceil(length); i++){
              frac = i/length;
              pos[0] = frac*dxAB + cordB[0];
              pos[1] = frac*dyAB + cordB[1];
              pos[2] = frac*dzAB + cordB[2];

              pos2[0] = frac*dxCD + cordC[0];
              pos2[1] = frac*dyCD + cordC[1];
              pos2[2] = frac*dzCD + cordC[2];

              paint_line(pos, pos2, clr, size);
       }       


}


// outlines square
void cameraV2::outline_sqaure(float *cordA, float *cordB, float *cordC, float *cordD, BYTE *clr, int size)
{
       paint_line(cordA, cordB, clr, size);
       paint_line(cordB, cordC, clr, size);
       paint_line(cordC, cordD, clr, size);
       paint_line(cordD, cordA, clr, size);
}


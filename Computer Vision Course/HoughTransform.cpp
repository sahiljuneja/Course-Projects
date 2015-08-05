 /*
 *  HoughTransform.cpp
 *
 *      Course: NCSU Spring 2013 - ECE 763
 *  Assignment: Hough Transform
 *      Author: Sahil Juneja
 *     Purpose: Implement Hough Transform to identify holes in circuit board.
 *    Requires: -
 */


#include <iostream>
#include <ifs.h>
#include <math.h>
#include <ifsmatrix.h>

using namespace std;

int len[4];
int len2[3];

void DrawCircle2(IFSIMG img, int x0, int y0, int radius); 
void DrawCircle(IFSIMG img, int frame, int x0, int y0, int radius) {
    // x0 -> row,  y0 -> column
    int x = radius, y = 0;
    int xChange = 1 - (radius << 1);
    int yChange = 0;
    int radiusError = 0;

    while (x >= y) {

        if (x + x0 > 0 && x + x0 < len[2] && y + y0 > 0 && y + y0 < len[1]) ifsfpp3d(img, frame, x + x0, y + y0, ifsfgp3d(img, frame, x + x0, y + y0) + 1);
        if (y + x0 > 0 && y + x0 < len[2] && x + y0 > 0 && x + y0 < len[1]) ifsfpp3d(img, frame, y + x0, x + y0, ifsfgp3d(img, frame, y + x0, x + y0) + 1);
        if (-x + x0 > 0 && -x + x0 < len[2] && y + y0 > 0 && y + y0 < len[1]) ifsfpp3d(img, frame, -x + x0, y + y0, ifsfgp3d(img, frame, -x + x0, y + y0) + 1);
        if (-y + x0 > 0 && -y + x0 < len[2] && x + y0 > 0 && x + y0 < len[1]) ifsfpp3d(img, frame, -y + x0, x + y0, ifsfgp3d(img, frame, -y + x0, x + y0) + 1);
        if (-x + x0 > 0 && -x + x0 < len[2] && -y + y0 > 0 && -y + y0 < len[1]) ifsfpp3d(img, frame, -x + x0, -y + y0, ifsfgp3d(img, frame, -x + x0, -y + y0) + 1);
        if (-y + x0 > 0 && -y + x0 < len[2] && -x + y0 > 0 && -x + y0 < len[1]) ifsfpp3d(img, frame, -y + x0, -x + y0, ifsfgp3d(img, frame, -y + x0, -x + y0) + 1);
        if (x + x0 > 0 && x + x0 < len[2] && -y + y0 > 0 && -y + y0 < len[1]) ifsfpp3d(img, frame, x + x0, -y + y0, ifsfgp3d(img, frame, x + x0, -y + y0) + 1);
        if (y + x0 > 0 && y + x0 < len[2] && -x + y0 > 0 && -x + y0 < len[1]) ifsfpp3d(img, frame, y + x0, -x + y0, ifsfgp3d(img, frame, y + x0, -x + y0) + 1);
        y++;
        radiusError += yChange;
        yChange += 2;
        if (((radiusError << 1) + xChange) > 0) {
            x--;
            radiusError += xChange;
            xChange += 2;
        }
    }
}

int main() {
    IFSIMG inpimg, opimg, fpimg;

    int imr, imc, row, col, frame;

    inpimg = ifspin("/home/sahil/ece763/images/s14sobel.ifs");

    row = ifsdimen(inpimg, 1);
    col = ifsdimen(inpimg, 0);


    len[0] = 3; 
    len[1] = col;
    len[2] = row;
    len[3] = 18;

    opimg = ifscreate("float", len, IFS_CR_ALL, 0);
    
    
    len2[0] = 2;
    len2[1] = col;
    len2[2] = row;
    fpimg = ifscreate("float", len2, IFS_CR_ALL, 0);


    int radius = 0;
    for (frame = 0; frame < len[3]; frame++) 
    {
        for (imr = 0; imr < row; imr++) 
        {
            for (imc = 0; imc < col; imc++) 
            {
                if ((ifsfgp(inpimg, imr, imc) == 255)) 
                {
                    DrawCircle(opimg, frame, imr, imc, radius);
                }
            }
        }
        radius++;
    }

    int val = 0;
    for (imr = 0; imr < row; imr++)
    {   
        for (imc = 0; imc < col; imc++)
        {
            for (frame = 0; frame < len[3]; frame++)
            {
                
                if ((ifsfgp3d(opimg,frame,imr,imc) >= ifsfgp3d(opimg,frame + 1,imr,imc)) && (ifsfgp3d(opimg,frame,imr,imc) >= val))
                {
                    val = ifsfgp3d(opimg,frame,imr,imc);
                }
            }
        }
   
     }
 printf("%i ", val);
    for (frame = 0; frame < len[3]; frame++) 
    {
        for (imr = 0; imr < row; imr++) 
        {
            for (imc = 0; imc < col; imc++) 
            {
                if ((ifsfgp3d(opimg, frame, imr, imc) >= val - 3) && (ifsfgp3d(opimg, frame, imr, imc) <= val)) 
                {
                    ifsfpp(fpimg, imr, imc, 255);
                    ifsfpp(fpimg, imr- 1, imc - 1, 255);
                    ifsfpp(fpimg, imr + 1, imc + 1, 255);
                    ifsfpp(fpimg, imr + 1, imc - 1, 255);
                    ifsfpp(fpimg, imr - 1, imc + 1, 255);
                    DrawCircle2(fpimg,imr, imc, frame);
                    printf("%i ",frame);
                    system("ChooseFrame -i /home/sahil/ece763/images/s14hough.ifs -o /home/sahil/ece763/images/s14houghfr.ifs -f 10");
                    system("any2any -i /home/sahil/ece763/images/s14houghfr.ifs -o /home/sahil/ece763/images/s14houghfr.png");
                }
            }
        }
    }
    
    ifspot(opimg, "/home/sahil/ece763/images/s14hough.ifs");
    ifspot(fpimg, "/home/sahil/ece763/images/s14final.ifs");
    system("any2any -i /home/sahil/ece763/images/s14final.ifs -o /home/sahil/ece763/images/s14final.png");
    
}



void DrawCircle2(IFSIMG img, int x0, int y0, int radius) 
{
    // x0 -> row,  y0 -> column
    int x = radius, y = 0;
    int xChange = 1 - (radius << 1);
    int yChange = 0;
    int radiusError = 0;

    while (x >= y) {

        if (x + x0 > 0 && x + x0 < len2[2] && y + y0 > 0 && y + y0 < len2[1]) ifsfpp(img, x + x0, y + y0, 255);
        if (y + x0 > 0 && y + x0 < len2[2] && x + y0 > 0 && x + y0 < len2[1]) ifsfpp(img, y + x0, x + y0, 255);
        if (-x + x0 > 0 && -x + x0 < len2[2] && y + y0 > 0 && y + y0 < len2[1]) ifsfpp(img, -x + x0, y + y0, 255);
        if (-y + x0 > 0 && -y + x0 < len2[2] && x + y0 > 0 && x + y0 < len2[1]) ifsfpp(img, -y + x0, x + y0, 255);
        if (-x + x0 > 0 && -x + x0 < len2[2] && -y + y0 > 0 && -y + y0 < len2[1]) ifsfpp(img, -x + x0, -y + y0, 255);
        if (-y + x0 > 0 && -y + x0 < len2[2] && -x + y0 > 0 && -x + y0 < len2[1]) ifsfpp(img, -y + x0, -x + y0, 255);
        if (x + x0 > 0 && x + x0 < len2[2] && -y + y0 > 0 && -y + y0 < len2[1]) ifsfpp(img, x + x0, -y + y0, 255);
        if (y + x0 > 0 && y + x0 < len2[2] && -x + y0 > 0 && -x + y0 < len2[1]) ifsfpp(img, y + x0, -x + y0, 255);
        y++;
        radiusError += yChange;
        yChange += 2;
        if (((radiusError << 1) + xChange) > 0) {
            x--;
            radiusError += xChange;
            xChange += 2;
        }
    }
}

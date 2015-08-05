
 /*
 *  VariableConductanceDiffusion.cpp
 *
 *      Course: NCSU Spring 2013 - ECE 763
 *  Assignment: Variable Conductance Diffusion
 *      Author: Sahil Juneja
 *     Purpose: Implement VCD on given images
 *    Requires: -
 */

#include <iostream>
#include <ifs.h>
#include <math.h>
#include <ifsmatrix.h>

using namespace std;



int main(int argc, char *argv[])
{

	cout << "Diffusing..." << endl;

	IFSIMG inpimg,dfdt,cimg;


	float kmr[3][3] = {{-1,0,1},{-2,0,2},{-1,0,1}};
	float kmc[3][3] = {{-1,-2,-1},{0,0,0},{1,2,1}};
	float dx,dy;
	int imr,imc,kr,kc,row,col,x,y;
	float sum,sum1,sum2;
	float d2x,d2y;
	float sigma = 20;
	int iterations = 100;
	float alpha = 0.01;

	inpimg = ifspin("/home/sahil/ece763/images/angio.ifs");
	row = ifsdimen(inpimg,1);
	col = ifsdimen(inpimg,0);

	int len[3]; len[0] =2; len[1] =row; len[2] =col;
	float c;
	float cdx[row][col];
	float cdy[row][col];

	dfdt = ifscreate("float",len,IFS_CR_ALL,0);
	cimg = ifscreate("float",len,IFS_CR_ALL,0);




	for (int i =0; i< iterations; i++)
	{
		for (imr = 0; imr < row; imr++)
		{

			for (imc =0; imc < col; imc++)
			{
				dx =0;
				dy=0;


				for (kr = 0;kr < 3;kr++)
				{
					for (kc = 0;kc < 3;kc++)
					{
						x = imr + kr -1;
						y = imc + kc - 1;
						if (x >= 0 && y >= 0 && x < row && y < col)
						{

							dx += kmr[kr][kc]*ifsfgp(inpimg,x,y);

							dy += kmc[kr][kc]*ifsfgp(inpimg,x,y);

						}

					}
				}
				dx /= 8;
				dy /= 8;
				c = (exp(-((dx*dx) + (dy*dy))/(sigma*sigma)));
				//ifsfpp(cimg, imr, imc, c);
				cdx[imr][imc] = (dx)*c;
				cdy[imr][imc] = (dy)*c;

			}
		}

		//ifspot(cimg, "/home/sahil/ece763/images/alpha0.01/cimg.ifs");


		for (imr = 0; imr < row; imr++)
			{
				for (imc =0; imc < col; imc++)
				{
					d2x = 0;
					d2y = 0;
					for (kr = 0;kr < 3;kr++)
					{
						for (kc = 0;kc < 3;kc++)
						{
							x = imr + kr -1;
							y = imc + kc - 1;
							if (x >= 0 && y >= 0 && x < row && y < col)
							{
								d2x += kmr[kr][kc]*cdx[x][y];
								d2y += kmc[kr][kc]*cdy[x][y];
							}
						}

					}
					d2x /= 8;
					d2y /= 8;

					sum = ((d2x + d2y));
					ifsfpp(dfdt,imr,imc,sum);

				}
			}
		for (imr = 0; imr < row; imr++)
		{
			for (imc =0; imc < col; imc++)
			{
				sum1 = alpha*(ifsfgp(dfdt,imr,imc)) + ifsfgp(inpimg,imr,imc);

				ifsfpp(inpimg,imr,imc,sum1);
			}
		}
	}



	ifspot(inpimg,"/home/sahil/ece763/images/angio5_30.ifs");
	
	system("vidscale -i /home/sahil/ece763/images/angio5_30.ifs -o /home/sahil/ece763/images/angio5_30.ifs");

	system("any2any -i /home/sahil/ece763/images/angio5_30.ifs -o /home/sahil/ece763/images/angio5_30.jpg");

	cout << "Done" << endl;

}
